param(
    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [double]$TimeoutSeconds = 30.0
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
. (Join-Path $ScriptRoot 'invoke_bridge.ps1')

if ([string]::IsNullOrWhiteSpace($BridgeHost)) {
    $BridgeHost = Get-ForgePilotBridgeHostDefault
}

if ($BridgePort -le 0) {
    $BridgePort = Get-ForgePilotBridgePortDefault
}

function Add-StepResult {
    param(
        [Parameter(Mandatory = $true)][string]$Name,
        [Parameter(Mandatory = $true)]$Result
    )

    $script:Summary.steps += [pscustomobject]@{
        name = $Name
        result = $Result
    }
}

function Invoke-ValidationTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{},
        [Parameter(Mandatory = $true)][string]$StepName,
        [int]$ToolTimeoutSeconds = 60
    )

    $result = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $ToolTimeoutSeconds
    Add-StepResult -Name $StepName -Result $result
    return $result
}

function Assert-ToolMetadata {
    param(
        [Parameter(Mandatory = $true)][hashtable]$ToolMap,
        [Parameter(Mandatory = $true)][string]$ToolName,
        [Parameter(Mandatory = $true)][string]$ExpectedMode,
        [Parameter(Mandatory = $true)][string]$ExpectedMutationLevel,
        [Parameter(Mandatory = $true)][bool]$ExpectedSafeDefault,
        [Parameter(Mandatory = $true)][string]$ExpectedDefaultExposure
    )

    if (-not $ToolMap.ContainsKey($ToolName)) {
        throw "Tool '$ToolName' not found in list_tools output."
    }

    $tool = $ToolMap[$ToolName]
    if ([string]$tool.mode -ne $ExpectedMode) {
        throw "Tool '$ToolName' mode mismatch. Expected '$ExpectedMode', got '$($tool.mode)'."
    }
    if ([string]$tool.mutation_level -ne $ExpectedMutationLevel) {
        throw "Tool '$ToolName' mutation_level mismatch. Expected '$ExpectedMutationLevel', got '$($tool.mutation_level)'."
    }
    if ([bool]$tool.safe_default -ne $ExpectedSafeDefault) {
        throw "Tool '$ToolName' safe_default mismatch. Expected '$ExpectedSafeDefault', got '$($tool.safe_default)'."
    }
    if ([string]$tool.default_exposure -ne $ExpectedDefaultExposure) {
        throw "Tool '$ToolName' default_exposure mismatch. Expected '$ExpectedDefaultExposure', got '$($tool.default_exposure)'."
    }
}

$Summary = [ordered]@{
    success = $false
    bridge = [ordered]@{}
    metadata = [ordered]@{}
    blueprint = [ordered]@{}
    material = [ordered]@{}
    niagara = [ordered]@{}
    cleanup = [ordered]@{}
    audit = [ordered]@{}
    assertions = [ordered]@{}
    steps = @()
}

$ScratchRoot = '/Game/ForgePilot_Test/Assist/ContractValidation'
$Stamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$BlueprintName = "BP_FP_Contract_$Stamp"
$MaterialName = "M_FP_Contract_$Stamp"
$SystemName = "NS_FP_Contract_$Stamp"
$BlueprintAssetPath = $null
$MaterialAssetPath = $null
$NiagaraAssetPath = $null
$CleanupAssetPaths = New-Object System.Collections.Generic.List[string]

try {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($TimeoutSeconds))

    $PingResponse = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = 'ping' } -TimeoutSeconds 15
    Add-StepResult -Name 'bridge_ping' -Result $PingResponse
    $Summary.bridge.ping = $PingResponse

    $ListToolsResponse = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = 'list_tools' } -TimeoutSeconds 30
    Add-StepResult -Name 'bridge_list_tools' -Result $ListToolsResponse
    if (-not $ListToolsResponse.ok) {
        throw 'Bridge list_tools failed.'
    }

    $ToolMap = @{}
    foreach ($tool in @($ListToolsResponse.tools)) {
        if ($tool -and $tool.name) {
            $ToolMap[[string]$tool.name] = $tool
        }
    }

    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'create_blueprint' -ExpectedMode 'build' -ExpectedMutationLevel 'advanced_authoring' -ExpectedSafeDefault $false -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'compile_blueprint_with_feedback' -ExpectedMode 'build' -ExpectedMutationLevel 'advanced_authoring' -ExpectedSafeDefault $false -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'create_material' -ExpectedMode 'build' -ExpectedMutationLevel 'advanced_authoring' -ExpectedSafeDefault $false -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'create_empty_niagara_system' -ExpectedMode 'build' -ExpectedMutationLevel 'advanced_authoring' -ExpectedSafeDefault $false -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_blueprint_summary' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_material_info' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_niagara_system_info' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'delete_asset' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $false -ExpectedDefaultExposure 'advanced'

    $ExpectedGroups = @{
        get_context = 'Scene'
        create_blueprint = 'Blueprint'
        create_material = 'Material'
        create_empty_niagara_system = 'Niagara'
        run_effect_workflow_v2 = 'General'
    }

    foreach ($ToolName in @('get_context', 'create_blueprint', 'create_material', 'create_empty_niagara_system', 'run_effect_workflow_v2')) {
        $tool = $ToolMap[$ToolName]
        foreach ($FieldName in @('tool_group', 'unreal_domain', 'workflow_lane', 'surface_area', 'primary_lane', 'vfx_affinity', 'capability_tags')) {
            if (-not $tool.PSObject.Properties.Name.Contains($FieldName)) {
                throw "Tool '$ToolName' general surface metadata field '$FieldName' is missing."
            }
        }
        if ([string]$tool.tool_group -ne $ExpectedGroups[$ToolName]) {
            throw "Tool '$ToolName' tool_group mismatch. Expected '$($ExpectedGroups[$ToolName])', got '$($tool.tool_group)'."
        }
    }
    $Summary.assertions.general_surface_metadata_present = $true

    $Summary.metadata.checked_tools = @(
        'create_blueprint',
        'compile_blueprint_with_feedback',
        'create_material',
        'create_empty_niagara_system',
        'get_blueprint_summary',
        'get_material_info',
        'get_niagara_system_info',
        'delete_asset'
    )

    Invoke-ValidationTool -ToolName 'create_asset_folder' -InputObject @{
        folder_path = $ScratchRoot
        apply_changes = $true
    } -StepName 'create_contract_folder' | Out-Null

    $BlueprintDryRun = Invoke-ValidationTool -ToolName 'create_blueprint' -InputObject @{
        blueprint_name = $BlueprintName
        parent_class = 'Actor'
        save_path = $ScratchRoot
        apply_changes = $false
    } -StepName 'create_blueprint_dry_run'

    $BlueprintApply = Invoke-ValidationTool -ToolName 'create_blueprint' -InputObject @{
        blueprint_name = $BlueprintName
        parent_class = 'Actor'
        save_path = $ScratchRoot
        apply_changes = $true
    } -StepName 'create_blueprint_apply'
    $BlueprintAssetPath = [string]$BlueprintApply.asset_path
    $CleanupAssetPaths.Add($BlueprintAssetPath)

    $BlueprintSummary = Invoke-ValidationTool -ToolName 'get_blueprint_summary' -InputObject @{
        blueprint_path = $BlueprintAssetPath
    } -StepName 'get_blueprint_summary'

    $CompileDryRun = Invoke-ValidationTool -ToolName 'compile_blueprint_with_feedback' -InputObject @{
        blueprint_path = $BlueprintAssetPath
        apply_changes = $false
        save_asset = $true
    } -StepName 'compile_blueprint_dry_run'

    $CompileApply = Invoke-ValidationTool -ToolName 'compile_blueprint_with_feedback' -InputObject @{
        blueprint_path = $BlueprintAssetPath
        apply_changes = $true
        save_asset = $true
    } -StepName 'compile_blueprint_apply'

    $MaterialDryRun = Invoke-ValidationTool -ToolName 'create_material' -InputObject @{
        material_name = $MaterialName
        save_path = $ScratchRoot
        base_color = @(0.2, 0.6, 1.0, 1.0)
        apply_changes = $false
    } -StepName 'create_material_dry_run'

    $MaterialApply = Invoke-ValidationTool -ToolName 'create_material' -InputObject @{
        material_name = $MaterialName
        save_path = $ScratchRoot
        base_color = @(0.2, 0.6, 1.0, 1.0)
        apply_changes = $true
    } -StepName 'create_material_apply'
    $MaterialAssetPath = [string]$MaterialApply.asset_path
    $CleanupAssetPaths.Add($MaterialAssetPath)

    $MaterialInfo = Invoke-ValidationTool -ToolName 'get_material_info' -InputObject @{
        material_path = $MaterialAssetPath
    } -StepName 'get_material_info'

    $NiagaraDryRun = Invoke-ValidationTool -ToolName 'create_empty_niagara_system' -InputObject @{
        system_name = $SystemName
        save_path = $ScratchRoot
        apply_changes = $false
    } -StepName 'create_empty_niagara_system_dry_run'

    $NiagaraApply = Invoke-ValidationTool -ToolName 'create_empty_niagara_system' -InputObject @{
        system_name = $SystemName
        save_path = $ScratchRoot
        apply_changes = $true
    } -StepName 'create_empty_niagara_system_apply'
    $NiagaraAssetPath = [string]$NiagaraApply.asset_path
    if ($NiagaraAssetPath -and $NiagaraAssetPath -notmatch '\.') {
        $NiagaraAssetPath = "$NiagaraAssetPath.$SystemName"
    }
    $CleanupAssetPaths.Add($NiagaraAssetPath)

    $NiagaraInfo = Invoke-ValidationTool -ToolName 'get_niagara_system_info' -InputObject @{
        system_path = $NiagaraAssetPath
    } -StepName 'get_niagara_system_info'

    $AuditBeforeDelete = Invoke-ValidationTool -ToolName 'get_mutation_audit_entries' -InputObject @{
        asset_path_contains = $Stamp
        include_details = $true
        max_results = 50
    } -StepName 'audit_before_delete'

    $Summary.blueprint = [ordered]@{
        dry_run = [bool]$BlueprintDryRun.dry_run
        created_asset_path = $BlueprintAssetPath
        parent_class = [string]$BlueprintSummary.parent_class
        component_count = if ($BlueprintSummary.components) { @($BlueprintSummary.components).Count } else { 0 }
        compile_dry_run = [bool]$CompileDryRun.dry_run
        compile_apply_success = [bool]$CompileApply.success
        compile_status = [string]$CompileApply.status
    }
    $Summary.material = [ordered]@{
        dry_run = [bool]$MaterialDryRun.dry_run
        created_asset_path = $MaterialAssetPath
        expression_count = [int]$MaterialInfo.expression_count
    }
    $Summary.niagara = [ordered]@{
        dry_run = [bool]$NiagaraDryRun.dry_run
        created_asset_path = $NiagaraAssetPath
        emitter_count = [int]$NiagaraInfo.emitter_count
        compile_log_checked = [bool]$NiagaraApply.compile_log_checked
    }
    $Summary.audit.before_delete_count = [int]$AuditBeforeDelete.count

    $Summary.assertions.bridge_ping_ok = [bool]$PingResponse.ok
    $Summary.assertions.blueprint_create_dry_run = [bool]$BlueprintDryRun.dry_run
    $Summary.assertions.blueprint_compile_dry_run = [bool]$CompileDryRun.dry_run
    $Summary.assertions.blueprint_compile_apply = [bool]$CompileApply.success
    $Summary.assertions.material_create_dry_run = [bool]$MaterialDryRun.dry_run
    $Summary.assertions.material_asset_created = -not [string]::IsNullOrWhiteSpace($MaterialAssetPath)
    $Summary.assertions.niagara_create_dry_run = [bool]$NiagaraDryRun.dry_run
    $Summary.assertions.niagara_asset_created = -not [string]::IsNullOrWhiteSpace($NiagaraAssetPath)
    $Summary.assertions.audit_before_delete_has_entries = ([int]$AuditBeforeDelete.count -ge 3)
}
finally {
    $CleanupResults = @()
    foreach ($AssetPath in ($CleanupAssetPaths | Select-Object -Unique | Sort-Object -Descending)) {
        if ([string]::IsNullOrWhiteSpace($AssetPath)) {
            continue
        }

        try {
            $DeleteResult = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName 'delete_asset' -InputObject @{
                asset_path = $AssetPath
                apply_changes = $true
            } -TimeoutSeconds 30

            $CleanupResults += [pscustomobject]@{
                asset_path = $AssetPath
                success = [bool]$DeleteResult.success
                message = [string]$DeleteResult.message
            }
        }
        catch {
            $CleanupResults += [pscustomobject]@{
                asset_path = $AssetPath
                success = $false
                message = $_.Exception.Message
            }
        }
    }

    $Summary.cleanup.assets = $CleanupResults

    try {
        $AuditAfterDelete = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName 'get_mutation_audit_entries' -InputObject @{
            asset_path_contains = $Stamp
            include_details = $true
            max_results = 50
        } -TimeoutSeconds 30
        $Summary.audit.after_delete_count = [int]$AuditAfterDelete.count
        $Summary.audit.after_delete = $AuditAfterDelete
        $Summary.assertions.cleanup_succeeded = -not (@($CleanupResults | Where-Object { -not $_.success })).Count
        $Summary.assertions.audit_after_delete_has_delete_entries = ([int]$AuditAfterDelete.count -ge 5)
    }
    catch {
        $Summary.audit.after_delete_error = $_.Exception.Message
        $Summary.assertions.cleanup_succeeded = $false
        $Summary.assertions.audit_after_delete_has_delete_entries = $false
    }

    $Summary.success = -not (@($Summary.assertions.GetEnumerator() | Where-Object { -not $_.Value })).Count
}

$Summary | ConvertTo-Json -Depth 20
