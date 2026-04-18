param(
    [string]$BridgeHost = $(if ($env:FORGEPILOT_BRIDGE_HOST) { $env:FORGEPILOT_BRIDGE_HOST } else { "127.0.0.1" }),
    [int]$BridgePort = 0,
    [double]$Timeout = 180.0,
    [string]$SourceSystem = "/Game/ForgePilot_Test/Niagara/ToolingSandbox/NS_ForgePilot_SmokeBurst_FromScratch.NS_ForgePilot_SmokeBurst_FromScratch",
    [string]$SavePath = "/Game/ForgePilot_Audit"
)

$ErrorActionPreference = "Stop"

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}

. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$TempName = "NS_ForgePilotLiveVerify_$Stamp"
$AssetPath = "$SavePath/$TempName"
$AssetObjectPath = "$AssetPath.$TempName"
$ParamName = "User.ForgePilotLiveVerifyFloat"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotNiagaraProbes" "niagara_live_verify_$Stamp"
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$ExpectedTools = @(
    "get_niagara_user_parameter_defaults",
    "get_niagara_emitter_stack",
    "get_niagara_mutation_snapshots",
    "restore_niagara_mutation_snapshot",
    "cleanup_niagara_audit_assets"
)

$Summary = [ordered]@{
    success = $false
    run_id = $Stamp
    source_system = $SourceSystem
    asset_path = $AssetPath
    asset_object_path = $AssetObjectPath
    tool_presence = @{}
    steps = @{}
    checks = @{}
    cleanup = @{}
}

function Find-ParamByName {
    param(
        [object[]]$Parameters,
        [string]$Name
    )
    foreach ($Item in @($Parameters)) {
        if ($null -ne $Item -and [string]$Item.name -eq $Name) {
            return $Item
        }
    }
    return $null
}

try {
    $LoadedToolNames = @(Get-ForgePilotBridgeToolNames -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout)))
    $MissingTools = @($ExpectedTools | Where-Object { $_ -notin $LoadedToolNames })
    $Summary.tool_presence = @{
        loaded_tool_count = $LoadedToolNames.Count
        expected_tools = $ExpectedTools
        missing_tools = $MissingTools
    }
    if ($MissingTools.Count -gt 0) {
        throw "Missing Niagara live-verify tools in bridge session: $($MissingTools -join ', ')"
    }

    $PreferredSystem = $SourceSystem
    try {
        $EditorContext = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_editor_context" -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
        $Summary.steps.editor_context = $EditorContext
        if ($EditorContext.preferred_system_path) {
            $PreferredSystem = [string]$EditorContext.preferred_system_path
        }
    }
    catch {
        $Summary.steps.editor_context_error = $_.Exception.Message
    }

    $Summary.steps.defaults_source = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_user_parameter_defaults" -InputObject @{
        system_path = $PreferredSystem
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.emitter_stack = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_emitter_stack" -InputObject @{
        system_path = $PreferredSystem
        emitter_index = "0"
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.duplicate = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "duplicate_niagara_system" -InputObject @{
        source_system_path = $PreferredSystem
        new_system_name = $TempName
        save_path = $SavePath
        apply_changes = $true
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.add_param = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "add_niagara_user_parameter" -InputObject @{
        system_path = $AssetPath
        param_name = "ForgePilotLiveVerifyFloat"
        param_type = "float"
        apply_changes = $true
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.defaults_after_add = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_user_parameter_defaults" -InputObject @{
        system_path = $AssetPath
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.snapshots = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_mutation_snapshots" -InputObject @{
        system_path = $AssetPath
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.restore = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "restore_niagara_mutation_snapshot" -InputObject @{
        system_path = $AssetPath
        use_latest = $true
        apply_changes = $true
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $Summary.steps.defaults_after_restore = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_user_parameter_defaults" -InputObject @{
        system_path = $AssetPath
    } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    $DefaultsAfterAdd = @($Summary.steps.defaults_after_add.parameters)
    $DefaultsAfterRestore = @($Summary.steps.defaults_after_restore.parameters)

    $Summary.checks = @{
        stack_stage_count = $Summary.steps.emitter_stack.stage_count
        stack_finding_count = $Summary.steps.emitter_stack.finding_count
        snapshot_count = $Summary.steps.snapshots.count
        param_present_after_add = ($null -ne (Find-ParamByName -Parameters $DefaultsAfterAdd -Name $ParamName))
        param_present_after_restore = ($null -ne (Find-ParamByName -Parameters $DefaultsAfterRestore -Name $ParamName))
    }

    if ([int]$Summary.checks.snapshot_count -lt 1) {
        throw "Expected at least one Niagara mutation snapshot after asset mutation."
    }
    if (-not $Summary.checks.param_present_after_add) {
        throw "Live verify param was not present after add_niagara_user_parameter."
    }
    if ($Summary.checks.param_present_after_restore) {
        throw "Live verify param still existed after restore_niagara_mutation_snapshot."
    }

    $Summary.success = $true
}
catch {
    $Summary.error = $_.Exception.Message
}
finally {
    try {
        $Summary.cleanup.cleanup_audit_assets = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "cleanup_niagara_audit_assets" -InputObject @{
            folder_path = $SavePath
            name_prefix = $TempName
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }
    catch {
        $Summary.cleanup.cleanup_audit_assets_error = $_.Exception.Message
    }

    try {
        $Summary.cleanup.delete_asset = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "delete_asset" -InputObject @{
            asset_path = $AssetObjectPath
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }
    catch {
        $Summary.cleanup.delete_asset_error = $_.Exception.Message
    }

    $SummaryPath = Join-Path $OutputDir "summary.json"
    $Summary | ConvertTo-Json -Depth 100 | Set-Content -Path $SummaryPath -Encoding UTF8
    [ordered]@{
        success = $Summary.success
        summary_path = $SummaryPath
        run_id = $Stamp
    } | ConvertTo-Json -Depth 10
}


