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
    $OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotNiagaraFromScratch" "niagara_from_scratch_$stamp"
}
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$SandboxFolder = "/Game/ForgePilot_Test/Niagara/ToolingSandbox"
$SandboxSystemName = "NS_ForgePilot_SmokeBurst_FromScratch_Stable"
$SandboxSystemPath = "$SandboxFolder/$SandboxSystemName.$SandboxSystemName"
$EmitterName = "smoke"

$Summary = [ordered]@{
    success = $false
    action = "run_niagara_from_scratch_validation"
    output_dir = $OutputDir
    bridge_host = $BridgeHost
    bridge_port = $BridgePort
    sandbox_system_path = $SandboxSystemPath
    emitter_name = $EmitterName
    started_at = (Get-Date).ToString("o")
    steps = @()
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

function Ensure-SystemExists {
    try {
        return Invoke-ValidationTool -ToolName "get_niagara_system_info" -InputObject @{
            system_path = $SandboxSystemPath
        } -StepName "ensure_system_existing"
    }
    catch {
        Add-StepResult -Name "ensure_system_existing" -Result @{
            success = $false
            message = $_.Exception.Message
        }

        Invoke-ValidationTool -ToolName "create_empty_niagara_system" -InputObject @{
            system_name = $SandboxSystemName
            save_path = $SandboxFolder
            apply_changes = $true
        } -StepName "create_empty_system" | Out-Null

        return Invoke-ValidationTool -ToolName "get_niagara_system_info" -InputObject @{
            system_path = $SandboxSystemPath
        } -StepName "query_system_after_create"
    }
}

function Ensure-EmitterExists {
    try {
        return Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_name = $EmitterName
        } -StepName "ensure_emitter_existing"
    }
    catch {
        Add-StepResult -Name "ensure_emitter_existing" -Result @{
            success = $false
            message = $_.Exception.Message
        }

        Invoke-ValidationTool -ToolName "add_empty_niagara_emitter" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_name = $EmitterName
            simulation_target = "cpu"
            add_sprite_renderer = $true
            apply_changes = $true
        } -StepName "add_empty_emitter" | Out-Null

        return Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
            system_path = $SandboxSystemPath
            emitter_name = $EmitterName
        } -StepName "query_emitter_after_create"
    }
}

try {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
    Add-StepResult -Name "wait_for_bridge" -Result @{ success = $true }

    $systemInfo = Ensure-SystemExists
    $initialInputs = Ensure-EmitterExists

    Invoke-ValidationTool -ToolName "set_niagara_spawn_rate" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        value = 18.0
        apply_changes = $true
    } -StepName "set_spawn_rate" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_lifetime" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        min_value = 0.45
        max_value = 0.95
        apply_changes = $true
    } -StepName "set_lifetime" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_sprite_size" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        min_value = 18.0
        max_value = 55.0
        apply_changes = $true
    } -StepName "set_sprite_size" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_velocity" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        value = @(0.0, 0.0, 55.0)
        apply_changes = $true
    } -StepName "set_velocity" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_cone_velocity" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        min_speed = 12.0
        max_speed = 38.0
        cone_angle = 18.0
        direction = @(0.0, 0.0, 1.0)
        apply_changes = $true
    } -StepName "set_cone_velocity" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_drag" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        value = 1.1
        apply_changes = $true
    } -StepName "set_drag" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_gravity_force" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        value = @(0.0, 0.0, 20.0)
        apply_changes = $true
    } -StepName "set_gravity" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_curl_noise_force" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        value = 6.0
        apply_changes = $true
    } -StepName "set_curl_noise" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_color_over_life" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        points = @(
            @{ time = 0.0; value = @(0.18, 0.18, 0.18, 1.0) },
            @{ time = 0.4; value = @(0.52, 0.52, 0.52, 1.0) },
            @{ time = 1.0; value = @(0.1, 0.1, 0.1, 1.0) }
        )
        apply_changes = $true
    } -StepName "set_color_over_life" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_opacity_over_life" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        points = @(
            @{ time = 0.0; value = 0.0 },
            @{ time = 0.15; value = 0.65 },
            @{ time = 1.0; value = 0.0 }
        )
        apply_changes = $true
    } -StepName "set_opacity_over_life" | Out-Null

    Invoke-ValidationTool -ToolName "set_niagara_local_space" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
        local_space = $false
        apply_changes = $true
    } -StepName "set_local_space" | Out-Null

    $compileResult = Invoke-ValidationTool -ToolName "compile_niagara_system" -InputObject @{
        system_path = $SandboxSystemPath
        apply_changes = $true
    } -StepName "compile_final"

    $finalInputs = Invoke-ValidationTool -ToolName "get_niagara_module_inputs" -InputObject @{
        system_path = $SandboxSystemPath
        emitter_name = $EmitterName
    } -StepName "final_module_inputs"

    $Summary.assertions["system_info"] = $systemInfo
    $Summary.assertions["final_compile"] = $compileResult
    $Summary.assertions["standard_module_presence"] = $finalInputs.standard_module_presence
    $Summary.assertions["module_count"] = $finalInputs.module_count

    $Summary.success = $true
}
catch {
    $Summary.success = $false
    $Summary.error = $_.Exception.Message
    Add-StepResult -Name "exception" -Result @{ success = $false; message = $_.Exception.Message }
}
finally {
    $Summary.finished_at = (Get-Date).ToString("o")
    $SummaryPath = Join-Path $OutputDir "summary.json"
    $Summary | ConvertTo-Json -Depth 20 | Set-Content -Path $SummaryPath -Encoding UTF8
    Write-Output ($Summary | ConvertTo-Json -Depth 20)
}


