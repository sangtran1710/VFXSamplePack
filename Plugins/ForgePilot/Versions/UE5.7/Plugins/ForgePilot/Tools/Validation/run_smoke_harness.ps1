param(
    [switch]$RunBuild,
    [string]$ProjectRoot = "",
    [string]$ProjectUproject = "",
    [string]$UEBuildBat = "",
    [string]$Target = ""
)

$ErrorActionPreference = "Stop"

. "$PSScriptRoot\forgepilot_recovery.ps1"

function Get-PythonCommand {
    foreach ($Candidate in @("python", "py")) {
        try {
            & $Candidate --version *> $null
            if ($LASTEXITCODE -eq 0) {
                return $Candidate
            }
        }
        catch {
        }
    }
    return $null
}

function Test-PowerShellSyntax {
    param(
        [Parameter(Mandatory = $true)][string[]]$Paths
    )

    foreach ($Path in $Paths) {
        $Tokens = $null
        $Errors = $null
        [void][System.Management.Automation.Language.Parser]::ParseFile($Path, [ref]$Tokens, [ref]$Errors)
        if ($Errors -and $Errors.Count -gt 0) {
            $Messages = $Errors | ForEach-Object { $_.Message }
            throw "PowerShell parse failed for '$Path': $($Messages -join '; ')"
        }
    }
}

function Add-Result {
    param(
        [string]$Phase,
        [string]$Check,
        [string]$Status,
        [string]$Detail
    )
    $script:Results += [pscustomobject]@{
        phase  = $Phase
        check  = $Check
        status = $Status
        detail = $Detail
    }
}

function Invoke-Check {
    param(
        [string]$Phase,
        [string]$Check,
        [scriptblock]$Action
    )
    try {
        & $Action
    }
    catch {
        Add-Result -Phase $Phase -Check $Check -Status "FAIL" -Detail $_.Exception.Message
    }
}

$Results = @()
$PluginRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$DetectedProjectRoot = if ([string]::IsNullOrWhiteSpace($ProjectRoot)) { Resolve-ForgePilotProjectRoot -StartDir $PSScriptRoot } else { $ProjectRoot }
$ValidationRoot = Join-Path $PluginRoot "Tools\Validation"
$SourceRoot = Join-Path $PluginRoot "Source\ForgePilot\Private"
$McpServer = Join-Path $PluginRoot "Tools\MCP\forgepilot_mcp_server.py"
$PythonCmd = Get-PythonCommand

Invoke-Check -Phase "S0" -Check "Core files exist" -Action {
    $Required = @(
        (Join-Path $SourceRoot "ForgePilotMCPBridgeSubsystem.cpp"),
        (Join-Path $SourceRoot "VFXMutationAudit.cpp"),
        (Join-Path $SourceRoot "VFXToolRegistry.cpp"),
        (Join-Path $SourceRoot "VFXToolRegistry_Blueprint.cpp"),
        (Join-Path $SourceRoot "VFXToolRegistry_Material.cpp"),
        (Join-Path $SourceRoot "VFXToolRegistry_Niagara.cpp"),
        (Join-Path $SourceRoot "VFXToolRegistry_Animation.cpp"),
        $McpServer
    )
    $Missing = $Required | Where-Object { -not (Test-Path $_) }
    if ($Missing.Count -gt 0) {
        throw "Missing required files: $($Missing -join ', ')"
    }
    Add-Result -Phase "S0" -Check "Core files exist" -Status "PASS" -Detail "All required files found."
}

Invoke-Check -Phase "S2" -Check "MCP server python syntax" -Action {
    if (-not $PythonCmd) {
        Add-Result -Phase "S2" -Check "MCP server python syntax" -Status "BLOCKED" -Detail "No python command was found on PATH for this shell session."
        return
    }
    $PythonOutput = & $PythonCmd -m py_compile $McpServer 2>&1
    if ($LASTEXITCODE -ne 0) {
        throw "python -m py_compile failed: $PythonOutput"
    }
    Add-Result -Phase "S2" -Check "MCP server python syntax" -Status "PASS" -Detail "Python syntax is valid."
}

Invoke-Check -Phase "M1" -Check "Runtime validation scripts exist" -Action {
    $RequiredScripts = @(
        (Join-Path $ValidationRoot "invoke_bridge.py"),
        (Join-Path $ValidationRoot "pie_scenarios.json"),
        (Join-Path $ValidationRoot "run_pie_runtime_probe.ps1"),
        (Join-Path $ValidationRoot "check_latest_pie_errors.ps1"),
        (Join-Path $ValidationRoot "compare_probe_captures.ps1")
    )
    $Missing = $RequiredScripts | Where-Object { -not (Test-Path $_) }
    if ($Missing.Count -gt 0) {
        throw "Missing runtime validation scripts: $($Missing -join ', ')"
    }
    Add-Result -Phase "M1" -Check "Runtime validation scripts exist" -Status "PASS" -Detail "All M1 validation scripts found."
}

Invoke-Check -Phase "M1" -Check "Bridge helper syntax" -Action {
    $BridgeHelperPy = Join-Path $ValidationRoot "invoke_bridge.py"
    $BridgeHelperPs1 = Join-Path $ValidationRoot "invoke_bridge.ps1"

    if (-not (Test-Path $BridgeHelperPs1)) {
        throw "Bridge helper not found: $BridgeHelperPs1"
    }

    Test-PowerShellSyntax -Paths @($BridgeHelperPs1)

    if ($PythonCmd -and (Test-Path $BridgeHelperPy)) {
        $PythonOutput = & $PythonCmd -m py_compile $BridgeHelperPy 2>&1
        if ($LASTEXITCODE -ne 0) {
            throw "python -m py_compile failed: $PythonOutput"
        }
        Add-Result -Phase "M1" -Check "Bridge helper syntax" -Status "PASS" -Detail "PowerShell bridge helper parsed; invoke_bridge.py syntax is valid."
        return
    }

    Add-Result -Phase "M1" -Check "Bridge helper syntax" -Status "PASS" -Detail "PowerShell bridge helper parsed; Python helper syntax skipped because no python command was found on PATH."
}

Invoke-Check -Phase "X" -Check "Validation and PIE tools registered in source" -Action {
    $ExpectedTools = @(
        "review_blueprint_asset",
        "review_material_asset",
        "get_niagara_renderer_details",
        "set_niagara_renderer_enabled",
        "set_niagara_renderer_material",
        "set_niagara_renderer_material_user_param",
        "get_niagara_user_parameter_defaults",
        "get_niagara_actor_user_parameter_values",
        "get_niagara_mutation_snapshots",
        "restore_niagara_mutation_snapshot",
        "cleanup_niagara_audit_assets",
        "get_niagara_emitter_stack",
        "set_niagara_user_parameter_default",
        "set_niagara_system_fixed_bounds",
        "set_niagara_emitter_fixed_bounds",
        "review_niagara_renderer",
        "review_niagara_emitter",
        "review_niagara_system",
        "get_animation_editor_context",
        "get_animation_asset_info",
        "review_animation_asset",
        "get_mutation_audit_entries",
        "get_niagara_editor_context",
        "get_niagara_emitter_details",
        "summarize_blueprint_value_flow",
        "summarize_blueprint_material_runtime",
        "get_material_editor_context",
        "get_selected_material_expression_details",
        "get_material_instance_info",
        "get_material_mutation_audit_entries",
        "create_material_pattern_block",
        "create_material_parameter_pack",
        "set_material_custom_node",
        "update_material_comment",
        "start_play_in_viewport",
        "stop_play_session",
        "get_pie_session_state",
        "send_pie_key_input",
        "run_pie_probe_scenario",
        "destroy_actor_by_label",
        "get_blueprint_mutation_snapshots",
        "restore_blueprint_mutation_snapshot"
    )
    $RegistryFile = Join-Path $SourceRoot "VFXToolRegistry_*.cpp"
    $Content = Get-ChildItem -Path $SourceRoot -Filter "VFXToolRegistry_*.cpp" | Get-Content
    $Missing = @()
    foreach ($Tool in $ExpectedTools) {
        if (-not ($Content -match [regex]::Escape($Tool))) {
            $Missing += $Tool
        }
    }
    if ($Missing.Count -gt 0) {
        throw "Missing tool registrations/implementations: $($Missing -join ', ')"
    }
    Add-Result -Phase "X" -Check "Validation and PIE tools registered in source" -Status "PASS" -Detail "Review and PIE tool names found."
}

Invoke-Check -Phase "M3" -Check "Blueprint safety hooks present in source" -Action {
    $BlueprintRegistry = Join-Path $SourceRoot "VFXToolRegistry_Blueprint.cpp"
    if (-not (Test-Path $BlueprintRegistry)) {
        throw "Blueprint registry file not found: $BlueprintRegistry"
    }

    $BlueprintContent = Get-Content $BlueprintRegistry -Raw
    $ExpectedMarkers = @(
        "CreateBlueprintMutationSnapshot",
        "BuildBlueprintMutationPatchReport",
        "BP_SNAPSHOT_FAILED",
        "patch_report",
        "Tool_GetBlueprintMutationSnapshots",
        "Tool_RestoreBlueprintMutationSnapshot"
    )

    $Missing = @()
    foreach ($Marker in $ExpectedMarkers) {
        if ($BlueprintContent -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing blueprint safety markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M3" -Check "Blueprint safety hooks present in source" -Status "PASS" -Detail "Snapshot + patch report markers found."
}

Invoke-Check -Phase "M4" -Check "Material runtime trace hooks present in source" -Action {
    $BlueprintRegistry = Join-Path $SourceRoot "VFXToolRegistry_Blueprint.cpp"
    if (-not (Test-Path $BlueprintRegistry)) {
        throw "Blueprint registry file not found: $BlueprintRegistry"
    }

    $BlueprintContent = Get-Content $BlueprintRegistry -Raw
    $ExpectedMarkers = @(
        "Tool_SummarizeBlueprintMaterialRuntime",
        "summarize_blueprint_material_runtime",
        "material_parameter_writes",
        "dynamic_material_nodes",
        "parameter_groups"
    )

    $Missing = @()
    foreach ($Marker in $ExpectedMarkers) {
        if ($BlueprintContent -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing material runtime trace markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M4" -Check "Material runtime trace hooks present in source" -Status "PASS" -Detail "Material runtime trace tool markers found."
}

Invoke-Check -Phase "M4" -Check "Material graph authoring hooks present in source" -Action {
    $MaterialRegistry = Join-Path $SourceRoot "VFXToolRegistry_Material.cpp"
    if (-not (Test-Path $MaterialRegistry)) {
        throw "Material registry file not found: $MaterialRegistry"
    }

    $MaterialContent = Get-Content $MaterialRegistry -Raw
    $ExpectedMarkers = @(
        "Tool_GetMaterialEditorContext",
        "Tool_SetMaterialTextureParam",
        "Tool_CreateMaterialParameterPack",
        "Tool_CreateMaterialComment",
        "Tool_UpdateMaterialComment",
        "Tool_LayoutMaterialExpressions",
        "Tool_CreateMaterialPatternBlock",
        "Tool_SetMaterialCustomNode",
        "Tool_GetMaterialFunctionInfo",
        "Tool_ReviewMaterialFunction",
        "Tool_GetMaterialMutationSnapshots",
        "Tool_RestoreMaterialMutationSnapshot",
        "Tool_GetMaterialMutationAuditEntries",
        "CreateMaterialMutationSnapshot",
        "BuildMaterialMutationPatchReport",
        "TextureSampleParameter2D",
        "creative_mode"
    )

    $Missing = @()
    foreach ($Marker in $ExpectedMarkers) {
        if ($MaterialContent -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing material authoring markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M4" -Check "Material graph authoring hooks present in source" -Status "PASS" -Detail "Material graph authoring markers found."
}

Invoke-Check -Phase "M2" -Check "Niagara emitter deep-inspect hooks present in source" -Action {
    $NiagaraRegistry = Join-Path $SourceRoot "VFXToolRegistry_Niagara.cpp"
    if (-not (Test-Path $NiagaraRegistry)) {
        throw "Niagara registry file not found: $NiagaraRegistry"
    }

    $NiagaraContent = Get-Content $NiagaraRegistry -Raw
    $ExpectedMarkers = @(
        "Tool_GetNiagaraEmitterDetails",
        "Tool_GetNiagaraRendererDetails",
        "Tool_ReviewNiagaraEmitter",
        "Tool_ReviewNiagaraRenderer",
        "Tool_GetNiagaraEditorContext",
        "Tool_SetNiagaraRendererEnabled",
        "Tool_SetNiagaraRendererMaterial",
        "Tool_SetNiagaraRendererMaterialUserParam",
        "Tool_SetNiagaraSystemFixedBounds",
        "Tool_SetNiagaraEmitterFixedBounds",
        "get_niagara_editor_context",
        "get_niagara_emitter_details",
        "get_niagara_renderer_details",
        "set_niagara_renderer_enabled",
        "set_niagara_renderer_material",
        "set_niagara_renderer_material_user_param",
        "get_niagara_user_parameter_defaults",
        "get_niagara_actor_user_parameter_values",
        "set_niagara_user_parameter_default",
        "set_niagara_system_fixed_bounds",
        "set_niagara_emitter_fixed_bounds",
        "review_niagara_renderer",
        "review_niagara_emitter",
        "preferred_system_path",
        "selected_systems",
        "opened_systems",
        "simulation_stage_count",
        "event_handler_count",
        "incompatible_renderer_count",
        "renderer_type",
        "used_material_count",
        "supports_emitter_sim_target",
        "target_material_path",
        "current_material_path",
        "target_user_param_name",
        "current_user_param_name",
        "SetNiagaraParameterStoreValue",
        "BuildNiagaraParameterValueObject",
        "BuildNiagaraParameterValueArray",
        "Tool_GetNiagaraUserParameterDefaults",
        "Tool_GetNiagaraActorUserParameterValues",
        "Tool_GetNiagaraMutationSnapshots",
        "Tool_RestoreNiagaraMutationSnapshot",
        "Tool_CleanupNiagaraAuditAssets",
        "Tool_GetNiagaraEmitterStack",
        "UMaterialInterface::StaticClass",
        "GetUObjectDef",
        "SetVariableMaterial",
        "SetVariableObject",
        "SetVariableTexture",
        "system_fixed_bounds",
        "target_bounds_mode",
        "target_fixed_bounds",
        "CreateNiagaraMutationSnapshot",
        "BuildNiagaraMutationPatchReport",
        "BuildNiagaraStackStageObject"
    )

    $Missing = @()
    foreach ($Marker in $ExpectedMarkers) {
        if ($NiagaraContent -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing Niagara deep-inspect markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M2" -Check "Niagara emitter deep-inspect hooks present in source" -Status "PASS" -Detail "Niagara deep-inspect markers found."
}

Invoke-Check -Phase "M2" -Check "Niagara probe scripts exist and syntax-check" -Action {
    $RequiredScripts = @(
        (Join-Path $ValidationRoot "niagara_capability_probe.py"),
        (Join-Path $ValidationRoot "run_niagara_capability_probe.ps1"),
        (Join-Path $ValidationRoot "niagara_live_verify.py"),
        (Join-Path $ValidationRoot "run_niagara_live_verify.ps1"),
        (Join-Path $ValidationRoot "compare_niagara_probe_runs.py"),
        (Join-Path $ValidationRoot "compare_niagara_probe_runs.ps1")
    )
    $Missing = $RequiredScripts | Where-Object { -not (Test-Path $_) }
    if ($Missing.Count -gt 0) {
        throw "Missing Niagara probe scripts: $($Missing -join ', ')"
    }

    $PowerShellScripts = @(
        (Join-Path $ValidationRoot "run_niagara_capability_probe.ps1"),
        (Join-Path $ValidationRoot "run_niagara_live_verify.ps1"),
        (Join-Path $ValidationRoot "compare_niagara_probe_runs.ps1")
    )
    Test-PowerShellSyntax -Paths $PowerShellScripts

    if ($PythonCmd) {
        $PythonScripts = @(
            (Join-Path $ValidationRoot "niagara_capability_probe.py"),
            (Join-Path $ValidationRoot "niagara_live_verify.py"),
            (Join-Path $ValidationRoot "compare_niagara_probe_runs.py")
        )
        $PythonOutput = & $PythonCmd -m py_compile @PythonScripts 2>&1
        if ($LASTEXITCODE -ne 0) {
            throw "python -m py_compile failed: $PythonOutput"
        }

        Add-Result -Phase "M2" -Check "Niagara probe scripts exist and syntax-check" -Status "PASS" -Detail "Niagara probe PowerShell + Python scripts are present and valid."
        return
    }

    Add-Result -Phase "M2" -Check "Niagara probe scripts exist and syntax-check" -Status "PASS" -Detail "Niagara probe PowerShell scripts parsed; Python syntax skipped because no python command was found on PATH."
}

Invoke-Check -Phase "M5" -Check "Animation-adjacent inspection hooks present in source" -Action {
    $AnimationRegistry = Join-Path $SourceRoot "VFXToolRegistry_Animation.cpp"
    if (-not (Test-Path $AnimationRegistry)) {
        throw "Animation registry file not found: $AnimationRegistry"
    }

    $AnimationContent = Get-Content $AnimationRegistry -Raw
    $ExpectedMarkers = @(
        "RegisterAnimationTools",
        "Tool_GetAnimationEditorContext",
        "Tool_GetAnimationAssetInfo",
        "Tool_ReviewAnimationAsset",
        "get_animation_editor_context",
        "get_animation_asset_info",
        "review_animation_asset",
        "preferred_asset_path",
        "notify_events",
        "montage_sections",
        "montage_slots",
        "curve_compression_settings"
    )

    $Missing = @()
    foreach ($Marker in $ExpectedMarkers) {
        if ($AnimationContent -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing animation inspection markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M5" -Check "Animation-adjacent inspection hooks present in source" -Status "PASS" -Detail "Animation inspection markers found."
}

Invoke-Check -Phase "M6" -Check "Mutation audit hooks present in source" -Action {
    $AuditImpl = Join-Path $SourceRoot "VFXMutationAudit.cpp"
    $SceneRegistry = Join-Path $SourceRoot "VFXToolRegistry_Scene.cpp"
    $BlueprintRegistry = Join-Path $SourceRoot "VFXToolRegistry_Blueprint.cpp"
    $MaterialRegistry = Join-Path $SourceRoot "VFXToolRegistry_Material.cpp"
    $NiagaraRegistry = Join-Path $SourceRoot "VFXToolRegistry_Niagara.cpp"

    foreach ($Path in @($AuditImpl, $SceneRegistry, $BlueprintRegistry, $MaterialRegistry, $NiagaraRegistry)) {
        if (-not (Test-Path $Path)) {
            throw "Required file not found: $Path"
        }
    }

    $AuditContent = Get-Content $AuditImpl -Raw
    $SceneContent = Get-Content $SceneRegistry -Raw
    $BlueprintContent = Get-Content $BlueprintRegistry -Raw
    $MaterialContent = Get-Content $MaterialRegistry -Raw
    $NiagaraContent = Get-Content $NiagaraRegistry -Raw

    $ExpectedAuditMarkers = @(
        "RecordAssetMutation",
        "GetRecentAuditEntryPaths",
        "ReadAuditEntry",
        "get_mutation_audit_entries",
        "audit_entry_path",
        "audit_recorded",
        "AppendBlueprintMutationAudit",
        "AppendMaterialMutationAudit",
        "AppendNiagaraMutationAudit"
    )

    $Combined = $AuditContent + "`n" + $SceneContent + "`n" + $BlueprintContent + "`n" + $MaterialContent + "`n" + $NiagaraContent
    $Missing = @()
    foreach ($Marker in $ExpectedAuditMarkers) {
        if ($Combined -notmatch [regex]::Escape($Marker)) {
            $Missing += $Marker
        }
    }

    if ($Missing.Count -gt 0) {
        throw "Missing mutation audit markers: $($Missing -join ', ')"
    }

    Add-Result -Phase "M6" -Check "Mutation audit hooks present in source" -Status "PASS" -Detail "Mutation audit markers found."
}

if ($RunBuild) {
    Invoke-Check -Phase "S3" -Check "Build editor target" -Action {
        if ([string]::IsNullOrWhiteSpace($ProjectUproject)) {
            $projectFiles = @(Get-ChildItem -Path $DetectedProjectRoot -Filter *.uproject -File -ErrorAction SilentlyContinue)
            if ($projectFiles.Count -ne 1) {
                throw "Expected exactly one .uproject under '$DetectedProjectRoot'. Pass -ProjectUproject explicitly."
            }
            $ProjectUproject = $projectFiles[0].FullName
        }
        if ([string]::IsNullOrWhiteSpace($Target)) {
            $targetFiles = @(Get-ChildItem -Path (Join-Path $DetectedProjectRoot 'Source') -Filter *Editor.Target.cs -File -Recurse -ErrorAction SilentlyContinue)
            if ($targetFiles.Count -ne 1 -or $targetFiles[0].Name -notmatch '^(.+Editor)\.Target\.cs$') {
                throw "Expected exactly one *Editor.Target.cs under '$DetectedProjectRoot\Source'. Pass -Target explicitly."
            }
            $Target = $Matches[1]
        }
        if ([string]::IsNullOrWhiteSpace($UEBuildBat)) {
            $projectJson = Get-Content -Path $ProjectUproject -Raw | ConvertFrom-Json
            $candidateRoots = @()
            if ([string]$projectJson.EngineAssociation -match '^\d+\.\d+') {
                $candidateRoots += "C:\Program Files\Epic Games\UE_$($projectJson.EngineAssociation)"
            }
            $candidateRoots += @(
                "C:\Program Files\Epic Games\UE_5.7",
                "C:\Program Files\Epic Games\UE_5.6",
                "C:\Program Files\Epic Games\UE_5.5",
                "C:\Program Files\Epic Games\UE_5.4"
            )
            foreach ($candidateRoot in @($candidateRoots | Select-Object -Unique)) {
                $candidateBuild = Join-Path $candidateRoot 'Engine\Build\BatchFiles\Build.bat'
                if (Test-Path $candidateBuild) {
                    $UEBuildBat = $candidateBuild
                    break
                }
            }
        }
        if (-not (Test-Path $UEBuildBat)) {
            throw "Build.bat not found at: $UEBuildBat"
        }
        if (-not (Test-Path $ProjectUproject)) {
            throw "Project not found: $ProjectUproject"
        }
        & $UEBuildBat $Target Win64 Development $ProjectUproject -WaitMutex -NoHotReloadFromIDE
        if ($LASTEXITCODE -ne 0) {
            throw "Unreal build failed with code $LASTEXITCODE"
        }
        Add-Result -Phase "S3" -Check "Build editor target" -Status "PASS" -Detail "Build succeeded for $Target."
    }
}
else {
    Add-Result -Phase "S3" -Check "Build editor target" -Status "BLOCKED" -Detail "Skipped (use -RunBuild to enable)."
}

Write-Host ""
Write-Host "=== ForgePilot Smoke Harness Results ==="
$Results | Format-Table -AutoSize

$FailCount = ($Results | Where-Object { $_.status -eq "FAIL" }).Count
if ($FailCount -gt 0) {
    exit 1
}
exit 0
