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

. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

if (-not $OutputDir) {
    $stamp = Get-Date -Format "yyyyMMdd_HHmmss"
    $OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotNiagaraToolingValidation" "niagara_stack_validation_$stamp"
}
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$SandboxFolder = "/Game/ForgePilot_Test/Niagara/ToolingSandbox"
$SandboxSystemName = "NS_ForgePilot_SmokeBurstRegression_Clean"
$SandboxSystemPath = "$SandboxFolder/$SandboxSystemName.$SandboxSystemName"
$SandboxSourceSystemPath = "/Game/Realistic_Starter_VFX_Pack_Niagara_Vol2/Niagara/Smoke/NS_Smoke_1.NS_Smoke_1"
$LegacyBrokenSystemPath = "/Game/ForgePilot_Test/Niagara/ToolingSandbox/NS_ForgePilot_NiagaraToolingSandbox.NS_ForgePilot_NiagaraToolingSandbox"

$Summary = [ordered]@{
    success = $false
    action = "run_niagara_stack_tooling_validation"
    output_dir = $OutputDir
    bridge_host = $BridgeHost
    bridge_port = $BridgePort
    sandbox_system_path = $SandboxSystemPath
    started_at = (Get-Date).ToString("o")
    steps = @()
    assertions = [ordered]@{}
    failure_tests = [ordered]@{}
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
    $Value | ConvertTo-Json -Depth 20 | Set-Content -Path $path -Encoding UTF8
    return $path
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

function Get-ModuleByDisplayName {
    param(
        [Parameter(Mandatory = $true)]$ModuleInputResult,
        [Parameter(Mandatory = $true)][string]$DisplayName
    )

    return @($ModuleInputResult.modules | Where-Object { $_.display_name -eq $DisplayName }) | Select-Object -First 1
}

function Get-InputValueSummary {
    param(
        [Parameter(Mandatory = $true)]$Module,
        [Parameter(Mandatory = $true)][string]$InputKey
    )

    $targetInput = @($Module.inputs | Where-Object { $_.input_key -eq $InputKey }) | Select-Object -First 1
    if (-not $targetInput) {
        return $null
    }

    return [ordered]@{
        input_key = $targetInput.input_key
        value_source = $targetInput.value_source
        value_string = $targetInput.value_string
    }
}

function Ensure-SandboxSystem {
    try {
        $existing = Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_index = 0
        } -StepName "ensure_sandbox_existing"
        Invoke-ValidationTool -ToolName "compile_niagara_system" -InputObject @{
            system_path = $SandboxSystemPath
            apply_changes = $true
        } -StepName "ensure_sandbox_compile_existing" | Out-Null
        return $existing
    }
    catch {
        Add-StepResult -Name "ensure_sandbox_existing" -Result @{
            success = $false
            message = $_.Exception.Message
        }

        Invoke-ValidationTool -ToolName "duplicate_niagara_system" -InputObject @{
            source_system_path = $SandboxSourceSystemPath
            new_system_name = $SandboxSystemName
            save_path = $SandboxFolder
            apply_changes = $true
        } -StepName "create_sandbox_system" | Out-Null

        $created = Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_index = 0
        } -StepName "query_sandbox_after_create"
        Invoke-ValidationTool -ToolName "compile_niagara_system" -InputObject @{
            system_path = $SandboxSystemPath
            apply_changes = $true
        } -StepName "compile_sandbox_after_create" | Out-Null
        return $created
    }
}

try {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    Add-StepResult -Name "wait_for_bridge" -Result @{ success = $true }

    $initialInputs = Ensure-SandboxSystem
    $initialStack = Invoke-ValidationTool -ToolName "get_niagara_emitter_stack" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        include_inputs = $true
    } -StepName "initial_emitter_stack"

    $spawnRateModule = Get-ModuleByDisplayName -ModuleInputResult $initialInputs -DisplayName "Spawn Rate"
    $initializeModule = Get-ModuleByDisplayName -ModuleInputResult $initialInputs -DisplayName "Initialize Particle"

    if (-not $spawnRateModule -or -not $initializeModule) {
        throw "Sandbox system is missing one or more required base modules."
    }

    Invoke-ValidationTool -ToolName "set_niagara_spawn_rate" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        value = 42.0
        apply_changes = $true
    } -StepName "set_spawn_rate" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_lifetime" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        min_value = 0.35
        max_value = 1.15
        apply_changes = $true
    } -StepName "set_lifetime" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_sprite_size" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        min_value = 20.0
        max_value = 65.0
        apply_changes = $true
    } -StepName "set_sprite_size" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_color_over_life" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        points = @(
            @{ time = 0.0; value = @(0.15, 0.12, 0.08, 1.0) },
            @{ time = 0.35; value = @(1.0, 0.45, 0.1, 1.0) },
            @{ time = 1.0; value = @(0.08, 0.02, 0.01, 1.0) }
        )
        apply_changes = $true
    } -StepName "set_color_over_life" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_opacity_over_life" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        points = @(
            @{ time = 0.0; value = 0.0 },
            @{ time = 0.18; value = 0.85 },
            @{ time = 1.0; value = 0.0 }
        )
        apply_changes = $true
    } -StepName "set_opacity_over_life" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_velocity" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        value = @(25.0, 0.0, 80.0)
        apply_changes = $true
    } -StepName "set_velocity" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_cone_velocity" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        direction = @(0.0, 0.0, 1.0)
        angle = 18.0
        min_speed = 45.0
        max_speed = 90.0
        apply_changes = $true
    } -StepName "set_cone_velocity" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_gravity_force" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        value = @(0.0, 0.0, 30.0)
        apply_changes = $true
    } -StepName "set_gravity_force" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_drag" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        value = 1.5
        apply_changes = $true
    } -StepName "set_drag" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_curl_noise_force" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        value = 18.0
        apply_changes = $true
    } -StepName "set_curl_noise_force" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_local_space" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        enabled = $true
        apply_changes = $true
    } -StepName "set_local_space" | Out-Null

    $queryAfterArtistEdits = Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
    } -StepName "query_after_artist_edits"

    $spawnRateModule = Get-ModuleByDisplayName -ModuleInputResult $queryAfterArtistEdits -DisplayName "Spawn Rate"
    $initializeModule = Get-ModuleByDisplayName -ModuleInputResult $queryAfterArtistEdits -DisplayName "Initialize Particle"
    $scaleColorModule = Get-ModuleByDisplayName -ModuleInputResult $queryAfterArtistEdits -DisplayName "Scale Color"

    Invoke-ValidationTool -ToolName "set_niagara_module_input_constant" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $spawnRateModule.module_key
        input_key = "Module.SpawnRate"
        value = 37.5
        apply_changes = $true
    } -StepName "lowlevel_set_constant_spawn_rate" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_module_input_dynamic" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $initializeModule.module_key
        input_key = "Module.Lifetime"
        dynamic_input_kind = "random_range"
        min_value = 0.2
        max_value = 0.9
        apply_changes = $true
    } -StepName "lowlevel_set_dynamic_lifetime" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_module_input_curve" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $scaleColorModule.module_key
        input_key = "Module.Scale Alpha"
        points = @(
            @{ time = 0.0; value = 0.0 },
            @{ time = 0.2; value = 1.0 },
            @{ time = 1.0; value = 0.0 }
        )
        apply_changes = $true
    } -StepName "lowlevel_set_curve_alpha" | Out-Null

    Invoke-ValidationTool -ToolName "reset_niagara_module_input" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $scaleColorModule.module_key
        input_key = "Module.Scale Alpha"
        apply_changes = $true
    } -StepName "lowlevel_reset_alpha" | Out-Null

    $moduleAddResult = Invoke-ValidationTool -ToolName "add_niagara_module_to_stage" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        stage_usage = "particle_update"
        module_script_path = "/Niagara/Modules/Update/Color/ScaleColor.ScaleColor"
        suggested_name = "Scale Color Temp"
        target_index = 0
        apply_changes = $true
    } -StepName "lowlevel_add_module"

    $moduleMoveResult = Invoke-ValidationTool -ToolName "move_niagara_module_in_stage" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $moduleAddResult.module_key
        target_index = 1
        apply_changes = $true
    } -StepName "lowlevel_move_module"

    Invoke-ValidationTool -ToolName "remove_niagara_module_from_stage" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
        module_key = $moduleMoveResult.moved_module_key
        apply_changes = $true
    } -StepName "lowlevel_remove_module" | Out-Null

    Invoke-ValidationTool -ToolName "compile_niagara_system" -InputObject @{
        system_path = $SandboxSystemPath
        apply_changes = $true
    } -StepName "compile_final"

    $finalInputs = Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_index = 0
    } -StepName "final_module_inputs"

    $finalSpawnRateModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Spawn Rate"
    $finalInitializeModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Initialize Particle"
    $finalScaleColorModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Scale Color"
    $finalVelocityModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Add Velocity"
    $finalConeModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Add Velocity in Cone"
    $finalGravityModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Gravity Force"
    $finalDragModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Drag"
    $finalCurlModule = Get-ModuleByDisplayName -ModuleInputResult $finalInputs -DisplayName "Curl Noise Force"

    $Summary.assertions.standard_module_presence = $finalInputs.standard_module_presence
    $Summary.assertions.spawn_rate = Get-InputValueSummary -Module $finalSpawnRateModule -InputKey "Module.SpawnRate"
    $Summary.assertions.lifetime = Get-InputValueSummary -Module $finalInitializeModule -InputKey "Module.Lifetime"
    $Summary.assertions.sprite_size = Get-InputValueSummary -Module $finalInitializeModule -InputKey "Module.Uniform Sprite Size"
    $Summary.assertions.scale_rgb = Get-InputValueSummary -Module $finalScaleColorModule -InputKey "Module.Scale RGB"
    $Summary.assertions.scale_alpha = Get-InputValueSummary -Module $finalScaleColorModule -InputKey "Module.Scale Alpha"
    $Summary.assertions.velocity = Get-InputValueSummary -Module $finalVelocityModule -InputKey "Module.Velocity"
    $Summary.assertions.cone_velocity_speed = Get-InputValueSummary -Module $finalConeModule -InputKey "Module.Velocity Strength"
    $Summary.assertions.gravity = Get-InputValueSummary -Module $finalGravityModule -InputKey "Module.Gravity"
    $Summary.assertions.drag = Get-InputValueSummary -Module $finalDragModule -InputKey "Module.Drag"
    $Summary.assertions.curl_noise_strength = Get-InputValueSummary -Module $finalCurlModule -InputKey "Module.Noise Strength"

    Invoke-ExpectedFailure -Name "missing_system" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "get_niagara_module_inputs" -InputObject @{
            system_path = "/Game/DoesNotExist.Nope"
            emitter_index = 0
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    Invoke-ExpectedFailure -Name "unknown_module" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "set_niagara_module_input_constant" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_index = 0
            module_key = "bogus-module-key"
            input_key = "Module.SpawnRate"
            value = 1.0
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    Invoke-ExpectedFailure -Name "type_mismatch" -ScriptBlock {
        Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName "set_niagara_module_input_constant" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_index = 0
            module_key = $finalSpawnRateModule.module_key
            input_key = "Module.SpawnRate"
            value = "abc"
            apply_changes = $true
        } -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    }

    try {
        Invoke-ValidationTool -ToolName "compile_niagara_system" -InputObject @{
            system_path = $LegacyBrokenSystemPath
            apply_changes = $true
        } -StepName "legacy_broken_compile_probe" | Out-Null

        $Summary.failure_tests.compile_detects_invalid_stage = [ordered]@{
            success = $false
            expected_failure = $true
            message = "Legacy broken sandbox unexpectedly compiled cleanly."
        }
    }
    catch {
        $entry = [ordered]@{
            success = $true
            expected_failure = $true
            message = $_.Exception.Message
        }
        $Summary.failure_tests.compile_detects_invalid_stage = $entry
        Add-StepResult -Name "failure_compile_detects_invalid_stage" -Result $entry
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

$Summary.finished_at = (Get-Date).ToString("o")
$Summary.summary_path = Save-JsonArtifact -Name "summary" -Value $Summary
$Summary | ConvertTo-Json -Depth 20


