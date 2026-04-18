param(
    [string]$ProbeBridgeHost = "127.0.0.1",
    [int]$ProbeBridgePort = 0,
    [double]$ProbeTimeout = 120.0,
    [string]$MaterialPath = "/Game/M5VFXVOL2/Niagara/Fire/Polish/M_FireFlipbook_SubUV_A01.M_FireFlipbook_SubUV_A01",
    [string]$PreviewMIPath = "/Game/M5VFXVOL2/Niagara/Fire/Polish/MI_FireFlipbook_SubUV_Preview_A01.MI_FireFlipbook_SubUV_Preview_A01",
    [string]$RuntimeMIPath = "/Game/M5VFXVOL2/Niagara/Fire/Polish/MI_FireFlipbook_SubUV_Runtime_A01.MI_FireFlipbook_SubUV_Runtime_A01",
    [string]$SystemPath = "/Game/M5VFXVOL2/Niagara/Fire/Polish/NS_FireFlipbook_SubUV_A01.NS_FireFlipbook_SubUV_A01",
    [string]$EmitterName = "Fire_Core"
)

$ErrorActionPreference = "Stop"

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}

. $BridgeHelper
if ($ProbeBridgePort -le 0) { $ProbeBridgePort = Get-ForgePilotBridgePortDefault }

Wait-ForgePilotBridge -BridgeHost $ProbeBridgeHost -BridgePort $ProbeBridgePort -TimeoutSeconds ([int][Math]::Ceiling($ProbeTimeout))

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotFireFlipbookProbes" "fire_flipbook_subuv_$Stamp"
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

function Invoke-FireProbeTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{}
    )

    Invoke-ForgePilotTool `
        -BridgeHost $ProbeBridgeHost `
        -BridgePort $ProbeBridgePort `
        -ToolName $ToolName `
        -InputObject $InputObject `
        -TimeoutSeconds ([int][Math]::Ceiling($ProbeTimeout))
}

$Report = [ordered]@{
    success = $false
    run_id = $Stamp
    output_dir = $OutputDir
    inputs = [ordered]@{
        material_path = $MaterialPath
        preview_mi_path = $PreviewMIPath
        runtime_mi_path = $RuntimeMIPath
        system_path = $SystemPath
        emitter_name = $EmitterName
    }
    diagnostics = [ordered]@{}
    summary = [ordered]@{}
}

try {
    $Report.diagnostics.material = Invoke-FireProbeTool -ToolName "compile_material_and_get_diagnostics" -InputObject @{
        material_path = $MaterialPath
    }

    $Report.diagnostics.preview_mi = Invoke-FireProbeTool -ToolName "compile_material_and_get_diagnostics" -InputObject @{
        material_path = $PreviewMIPath
    }

    $Report.diagnostics.runtime_mi = Invoke-FireProbeTool -ToolName "compile_material_and_get_diagnostics" -InputObject @{
        material_path = $RuntimeMIPath
    }

    $Report.diagnostics.material_info = Invoke-FireProbeTool -ToolName "get_material_info" -InputObject @{
        material_path = $MaterialPath
    }

    $Report.diagnostics.system_compile = Invoke-FireProbeTool -ToolName "compile_niagara_system" -InputObject @{
        system_path = $SystemPath
        emitter_name = $EmitterName
        apply_changes = "true"
        notes = "Fire flipbook SubUV validation probe"
    }

    $Report.diagnostics.emitter_details = Invoke-FireProbeTool -ToolName "get_niagara_emitter_details" -InputObject @{
        system_path = $SystemPath
        emitter_name = $EmitterName
    }

    $Report.diagnostics.emitter_stack = Invoke-FireProbeTool -ToolName "get_niagara_emitter_stack" -InputObject @{
        system_path = $SystemPath
        emitter_name = $EmitterName
    }

    $Report.diagnostics.renderer_details = Invoke-FireProbeTool -ToolName "get_niagara_renderer_details" -InputObject @{
        system_path = $SystemPath
        emitter_name = $EmitterName
        renderer_index = 0
    }

    $Report.summary.material_clean = [bool]$Report.diagnostics.material.is_clean
    $Report.summary.preview_mi_clean = [bool]$Report.diagnostics.preview_mi.is_clean
    $Report.summary.runtime_mi_clean = [bool]$Report.diagnostics.runtime_mi.is_clean
    $Report.summary.niagara_clean = -not [bool]$Report.diagnostics.system_compile.compile_diagnostics.has_errors
    $Report.summary.renderer_material = $Report.diagnostics.renderer_details.material_path
    $Report.summary.used_materials = @($Report.diagnostics.emitter_details.renderers[0].used_materials)
    $Report.summary.emitter_findings = @($Report.diagnostics.emitter_details.findings | ForEach-Object { $_.finding })
    $Report.summary.required_modules_present = [ordered]@{
        initialize_particle = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.initialize_particle
        scale_color = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.scale_color
        add_velocity = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.add_velocity
        drag = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.drag
        curl_noise_force = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.curl_noise_force
        solve_forces_and_velocity = [bool]$Report.diagnostics.emitter_stack.standard_module_presence.solve_forces_and_velocity
    }

    $Report.summary.recipe_status = if (
        $Report.summary.material_clean -and
        $Report.summary.preview_mi_clean -and
        $Report.summary.runtime_mi_clean -and
        $Report.summary.niagara_clean
    ) { "PASS" } else { "FAIL" }

    $Report.success = $true
}
catch {
    $Report.success = $false
    $Report.error = $_.Exception.Message
}

$ReportPath = Join-Path $OutputDir "fire_flipbook_subuv_probe_report.json"
$Report | ConvertTo-Json -Depth 100 | Set-Content -Path $ReportPath -Encoding UTF8
Write-Output $ReportPath


