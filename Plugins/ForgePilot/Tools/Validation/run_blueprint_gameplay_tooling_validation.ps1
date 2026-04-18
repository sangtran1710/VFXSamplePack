param(
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 0,
    [double]$Timeout = 180.0,
    [string]$OutputDir
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}

$RequestedBridgeHost = $BridgeHost
$RequestedBridgePort = $BridgePort
. $BridgeHelper

if (-not [string]::IsNullOrWhiteSpace($RequestedBridgeHost)) {
    $BridgeHost = $RequestedBridgeHost
}
elseif ([string]::IsNullOrWhiteSpace($BridgeHost)) {
    $BridgeHost = Get-ForgePilotBridgeHostDefault
}

if ($RequestedBridgePort -gt 0) {
    $BridgePort = $RequestedBridgePort
}
elseif ($BridgePort -le 0) {
    $BridgePort = Get-ForgePilotBridgePortDefault
}

if (-not $OutputDir) {
    $stamp = Get-Date -Format "yyyyMMdd_HHmmss"
    $OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotBlueprintToolingValidation" "blueprint_gameplay_validation_$stamp"
}
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$ScratchFolder = "/Game/ForgePilot_Test/Blueprint/ToolingSandbox"
$ScratchBlueprintName = "BP_ForgePilot_BlueprintGameplaySmoke_{0}" -f (Get-Date -Format "yyyyMMdd_HHmmss")
$ScratchBlueprintPath = "$ScratchFolder/$ScratchBlueprintName.$ScratchBlueprintName"
$ScratchAssetCreated = $false

$Summary = [ordered]@{
    success = $false
    action = "run_blueprint_gameplay_tooling_validation"
    output_dir = $OutputDir
    bridge_host = $BridgeHost
    bridge_port = $BridgePort
    scratch_blueprint_path = $ScratchBlueprintPath
    started_at = (Get-Date).ToString("o")
    steps = @()
    tool_presence = [ordered]@{}
    dry_run_tests = [ordered]@{}
    failure_tests = [ordered]@{}
    assertions = [ordered]@{}
}

function Add-StepResult {
    param(
        [Parameter(Mandatory = $true)][string]$Name,
        [Parameter(Mandatory = $true)]$Result
    )

    $script:Summary.steps += [pscustomobject]@{
        step = $Name
        timestamp = (Get-Date).ToString("o")
        result = $Result
    }
}

function Save-JsonArtifact {
    param(
        [Parameter(Mandatory = $true)][string]$Name,
        [Parameter(Mandatory = $true)]$Value
    )

    $safeName = ($Name -replace '[^A-Za-z0-9._-]', '_')
    $path = Join-Path $OutputDir "$safeName.json"
    $Value | ConvertTo-Json -Depth 30 | Set-Content -Path $path -Encoding UTF8
    return $path
}

function Invoke-BridgeAction {
    param(
        [Parameter(Mandatory = $true)][string]$Action,
        [hashtable]$Payload = @{},
        [string]$StepName = ""
    )

    $request = @{} + $Payload
    $request.action = $Action

    $result = Invoke-ForgePilotBridgeRequest `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -Payload $request `
        -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    if ($StepName) {
        Add-StepResult -Name $StepName -Result $result
        Save-JsonArtifact -Name $StepName -Value $result | Out-Null
    }

    if (-not $result.ok) {
        throw "Bridge action '$Action' failed: $($result | ConvertTo-Json -Depth 30 -Compress)"
    }

    return $result
}

function Invoke-ValidationTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{},
        [string]$StepName = ""
    )

    $result = Invoke-ForgePilotTool `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -ToolName $ToolName `
        -InputObject $InputObject `
        -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

    if ($StepName) {
        Add-StepResult -Name $StepName -Result $result
        Save-JsonArtifact -Name $StepName -Value $result | Out-Null
    }

    return $result
}

function Invoke-ExpectedFailure {
    param(
        [Parameter(Mandatory = $true)][string]$Name,
        [Parameter(Mandatory = $true)][scriptblock]$ScriptBlock
    )

    try {
        & $ScriptBlock | Out-Null
        $entry = [ordered]@{
            success = $false
            expected_failure = $true
            message = "Command unexpectedly succeeded."
        }
    }
    catch {
        $entry = [ordered]@{
            success = $true
            expected_failure = $true
            message = $_.Exception.Message
        }
    }

    $script:Summary.failure_tests[$Name] = $entry
    Add-StepResult -Name ("failure_" + $Name) -Result $entry
}

function Get-NodePins {
    param(
        [Parameter(Mandatory = $true)]$NodeResult
    )

    if ($null -eq $NodeResult -or $null -eq $NodeResult.node_details -or $null -eq $NodeResult.node_details.pins) {
        return @()
    }

    return @($NodeResult.node_details.pins)
}

function Find-NodePinName {
    param(
        [Parameter(Mandatory = $true)]$NodeResult,
        [string]$Direction = "",
        [string]$Type = "",
        [string[]]$PreferredNames = @(),
        [string[]]$ExcludeNames = @()
    )

    $pins = Get-NodePins -NodeResult $NodeResult
    $excludeLookup = @{}
    foreach ($exclude in @($ExcludeNames)) {
        if (-not [string]::IsNullOrWhiteSpace($exclude)) {
            $excludeLookup[$exclude.ToLowerInvariant()] = $true
        }
    }

    $matches = @($pins | Where-Object {
        $pinName = [string]$_.name
        $pinDirection = [string]$_.direction
        $pinType = [string]$_.type

        if ($excludeLookup.ContainsKey($pinName.ToLowerInvariant())) {
            return $false
        }
        if ($Direction -and -not $pinDirection.Equals($Direction, [System.StringComparison]::OrdinalIgnoreCase)) {
            return $false
        }
        if ($Type -and -not $pinType.Equals($Type, [System.StringComparison]::OrdinalIgnoreCase)) {
            return $false
        }

        return $true
    })

    foreach ($preferredName in @($PreferredNames)) {
        $preferredMatch = @($matches | Where-Object {
            ([string]$_.name).Equals($preferredName, [System.StringComparison]::OrdinalIgnoreCase)
        }) | Select-Object -First 1

        if ($preferredMatch) {
            return [string]$preferredMatch.name
        }
    }

    $fallback = $matches | Select-Object -First 1
    if ($fallback) {
        return [string]$fallback.name
    }

    return $null
}

function Get-NthNodePinName {
    param(
        [Parameter(Mandatory = $true)]$NodeResult,
        [Parameter(Mandatory = $true)][string]$Direction,
        [Parameter(Mandatory = $true)][string]$Type,
        [int]$Index = 0
    )

    $matches = @(Get-NodePins -NodeResult $NodeResult | Where-Object {
        ([string]$_.direction).Equals($Direction, [System.StringComparison]::OrdinalIgnoreCase) -and
        ([string]$_.type).Equals($Type, [System.StringComparison]::OrdinalIgnoreCase)
    })

    if ($Index -lt 0 -or $Index -ge $matches.Count) {
        return $null
    }

    return [string]$matches[$Index].name
}

try {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    Add-StepResult -Name "wait_for_bridge" -Result @{ success = $true }

    $PingResponse = Invoke-BridgeAction -Action "ping" -StepName "bridge_ping"
    $ListToolsResponse = Invoke-BridgeAction -Action "list_tools" -StepName "list_tools"
    $ToolNames = @($ListToolsResponse.tools | ForEach-Object { [string]$_.name })

    $RequiredTools = @(
        "create_blueprint",
        "add_variable_to_blueprint",
        "add_component_to_blueprint",
        "create_blueprint_variable_get_node",
        "create_blueprint_variable_set_node",
        "create_custom_event_node",
        "create_blueprint_call_function_node",
        "connect_blueprint_pins",
        "set_blueprint_node_pin_default",
        "compile_blueprint_with_feedback",
        "review_blueprint_asset",
        "trace_blueprint_exec_chain",
        "delete_asset"
    )

    foreach ($toolName in $RequiredTools) {
        $script:Summary.tool_presence[$toolName] = $ToolNames -contains $toolName
    }

    $Summary.assertions.bridge_initialized = [bool]$PingResponse.ok
    $Summary.assertions.bridge_has_tool_registry = ($ToolNames.Count -gt 0)
    $Summary.assertions.required_tools_available = -not (@($Summary.tool_presence.GetEnumerator() | Where-Object { -not $_.Value })).Count

    $CreateBlueprintResult = Invoke-ValidationTool -ToolName "create_blueprint" -InputObject @{
        blueprint_name = $ScratchBlueprintName
        parent_class = "Actor"
        save_path = $ScratchFolder
        apply_changes = $true
    } -StepName "create_blueprint"
    $ScratchAssetCreated = $true

    $DryRunCustomEvent = Invoke-ValidationTool -ToolName "create_custom_event_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        event_name = "DryRunOnlyEvent"
        node_pos_x = -1200
        node_pos_y = 0
        apply_changes = $false
    } -StepName "dry_run_custom_event"

    $AddVariableResult = Invoke-ValidationTool -ToolName "add_variable_to_blueprint" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        variable_name = "bSmokeFlag"
        variable_type = "bool"
        default_value = "false"
        apply_changes = $true
    } -StepName "add_variable_to_blueprint"

    $DryRunVariableSet = Invoke-ValidationTool -ToolName "create_blueprint_variable_set_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        variable_name = "bSmokeFlag"
        node_pos_x = -500
        node_pos_y = 0
        apply_changes = $false
    } -StepName "dry_run_variable_set"

    $DryRunVariableGet = Invoke-ValidationTool -ToolName "create_blueprint_variable_get_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        variable_name = "bSmokeFlag"
        node_pos_x = -760
        node_pos_y = 0
        apply_changes = $false
    } -StepName "dry_run_variable_get"

    $Summary.dry_run_tests.custom_event = [ordered]@{ success = [bool]$DryRunCustomEvent.dry_run; event_name = $DryRunCustomEvent.event_name }
    $Summary.dry_run_tests.variable_set = [ordered]@{ success = [bool]$DryRunVariableSet.dry_run }
    $Summary.dry_run_tests.variable_get = [ordered]@{ success = [bool]$DryRunVariableGet.dry_run; variable_name = $DryRunVariableGet.variable_name }

    $AddComponentResult = Invoke-ValidationTool -ToolName "add_component_to_blueprint" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        component_class = "SceneComponent"
        component_name = "SmokeValidationComponent"
        apply_changes = $true
    } -StepName "add_component_to_blueprint"

    $CustomEventResult = Invoke-ValidationTool -ToolName "create_custom_event_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        event_name = "RunSmokeFlow"
        node_pos_x = -1200
        node_pos_y = 0
        apply_changes = $true
    } -StepName "create_custom_event"

    $VariableGetResult = Invoke-ValidationTool -ToolName "create_blueprint_variable_get_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        variable_name = "bSmokeFlag"
        node_pos_x = -1000
        node_pos_y = 220
        apply_changes = $true
    } -StepName "create_variable_get"

    $CustomEventExecPin = Find-NodePinName -NodeResult $CustomEventResult -Direction "output" -Type "exec" -PreferredNames @("Then")
    $VariableGetValuePin = Find-NodePinName -NodeResult $VariableGetResult -Direction "output" -Type "bool" -PreferredNames @("bSmokeFlag")
    $VariableSetResult = Invoke-ValidationTool -ToolName "create_blueprint_variable_set_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        variable_name = "bSmokeFlag"
        node_pos_x = -520
        node_pos_y = -20
        apply_changes = $true
    } -StepName "create_variable_set"
    $VariableSetExecPin = Find-NodePinName -NodeResult $VariableSetResult -Direction "input" -Type "exec" -PreferredNames @("execute")
    $VariableSetValuePin = Find-NodePinName -NodeResult $VariableSetResult -Direction "input" -Type "bool" -PreferredNames @("bSmokeFlag") -ExcludeNames @("self")

    if (-not $CustomEventExecPin -or -not $VariableGetValuePin -or -not $VariableSetExecPin -or -not $VariableSetValuePin) {
        throw "Could not resolve required pins for the Custom Event / Variable node setup."
    }

    Invoke-ValidationTool -ToolName "connect_blueprint_pins" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        from_node_guid = $CustomEventResult.node_guid
        from_pin_name = $CustomEventExecPin
        to_node_guid = $VariableSetResult.node_guid
        to_pin_name = $VariableSetExecPin
        apply_changes = $true
    } -StepName "connect_custom_event_to_variable_set" | Out-Null

    Invoke-ValidationTool -ToolName "set_blueprint_node_pin_default" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        node_guid = $VariableSetResult.node_guid
        pin_name = $VariableSetValuePin
        default_value = "true"
        apply_changes = $true
    } -StepName "set_variable_set_default" | Out-Null

    $CallFunctionResult = Invoke-ValidationTool -ToolName "create_blueprint_call_function_node" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        graph_name = "EventGraph"
        function_owner_class = "KismetSystemLibrary"
        function_name = "PrintString"
        node_pos_x = -60
        node_pos_y = -20
        apply_changes = $true
    } -StepName "create_call_function"

    $VariableSetThenPin = Find-NodePinName -NodeResult $VariableSetResult -Direction "output" -Type "exec" -PreferredNames @("then")
    $CallExecPin = Find-NodePinName -NodeResult $CallFunctionResult -Direction "input" -Type "exec" -PreferredNames @("execute")
    $CallStringPin = Find-NodePinName -NodeResult $CallFunctionResult -Direction "input" -Type "string" -PreferredNames @("InString") -ExcludeNames @("self")

    if (-not $VariableSetThenPin -or -not $CallExecPin -or -not $CallStringPin) {
        throw "Could not resolve required pins for the Variable Set -> Call Function setup."
    }

    Invoke-ValidationTool -ToolName "connect_blueprint_pins" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        from_node_guid = $VariableSetResult.node_guid
        from_pin_name = $VariableSetThenPin
        to_node_guid = $CallFunctionResult.node_guid
        to_pin_name = $CallExecPin
        apply_changes = $true
    } -StepName "connect_variable_set_to_call_function" | Out-Null

    Invoke-ValidationTool -ToolName "set_blueprint_node_pin_default" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        node_guid = $CallFunctionResult.node_guid
        pin_name = $CallStringPin
        default_value = "ForgePilot gameplay smoke validation"
        apply_changes = $true
    } -StepName "set_call_function_default" | Out-Null

    $CompileResult = Invoke-ValidationTool -ToolName "compile_blueprint_with_feedback" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        apply_changes = $true
        save_asset = $true
    } -StepName "compile_blueprint"

    $ReviewResult = Invoke-ValidationTool -ToolName "review_blueprint_asset" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
    } -StepName "review_blueprint"

    $TraceResult = Invoke-ValidationTool -ToolName "trace_blueprint_exec_chain" -InputObject @{
        blueprint_path = $ScratchBlueprintPath
        node_guid = $CustomEventResult.node_guid
        max_depth = 8
        max_paths = 20
    } -StepName "trace_exec_chain"

    $Summary.assertions.compile_success = [bool]$CompileResult.success
    $Summary.assertions.compile_status = [string]$CompileResult.status
    $Summary.assertions.trace_path_count = [int]$TraceResult.path_count
    $Summary.assertions.review_finding_count = [int]$ReviewResult.finding_count
    $Summary.assertions.custom_event_created = [bool]($null -ne $CustomEventResult.node_guid)
    $Summary.assertions.variable_get_created = [bool]($null -ne $VariableGetResult.node_guid)
    $Summary.assertions.variable_set_created = [bool]($null -ne $VariableSetResult.node_guid)

    Invoke-ExpectedFailure -Name "missing_blueprint" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "create_custom_event_node" -InputObject @{
            blueprint_path = "/Game/DoesNotExist/BP_Nope.BP_Nope"
            graph_name = "EventGraph"
            event_name = "Nope"
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    Invoke-ExpectedFailure -Name "missing_variable" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "create_blueprint_variable_set_node" -InputObject @{
            blueprint_path = $ScratchBlueprintPath
            graph_name = "EventGraph"
            variable_name = "NoSuchVariable"
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    Invoke-ExpectedFailure -Name "duplicate_custom_event" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "create_custom_event_node" -InputObject @{
            blueprint_path = $ScratchBlueprintPath
            graph_name = "EventGraph"
            event_name = "RunSmokeFlow"
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    Invoke-ExpectedFailure -Name "incompatible_pin_connection" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "connect_blueprint_pins" -InputObject @{
            blueprint_path = $ScratchBlueprintPath
            from_node_guid = $VariableGetResult.node_guid
            from_pin_name = $VariableGetValuePin
            to_node_guid = $CallFunctionResult.node_guid
            to_pin_name = $CallExecPin
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    $Summary.success = $true
}
catch {
    $Summary.success = $false
    Add-StepResult -Name "validation_exception" -Result @{
        success = $false
        message = $_.Exception.Message
    }
}
finally {
    if ($ScratchAssetCreated) {
        try {
            $DeleteResult = Invoke-ValidationTool -ToolName "delete_asset" -InputObject @{
                asset_path = $ScratchBlueprintPath
                apply_changes = $true
            } -StepName "cleanup_delete_scratch_blueprint"
            $Summary.cleanup = [ordered]@{
                success = $true
                result = $DeleteResult
            }
        }
        catch {
            $Summary.cleanup = [ordered]@{
                success = $false
                message = $_.Exception.Message
            }
            Add-StepResult -Name "cleanup_delete_scratch_blueprint" -Result @{
                success = $false
                message = $_.Exception.Message
            }
        }
    }
}

$Summary.finished_at = (Get-Date).ToString("o")
$Summary.summary_path = Save-JsonArtifact -Name "summary" -Value $Summary
$Summary | ConvertTo-Json -Depth 30
