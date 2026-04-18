param(
    [string]$ScenarioName = "basic_viewport_runtime",
    [string]$ScenarioFile = "",
    [string]$OutputDir = "$env:USERPROFILE\Downloads\ForgePilotRuntimeProbes",
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 0,
    [int]$BridgeTimeoutSeconds = 60,
    [switch]$KeepPIEOpen,
    [switch]$PrettyJson
)

$ErrorActionPreference = "Stop"

$PluginRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$ProjectRoot = Split-Path -Parent (Split-Path -Parent $PluginRoot)
$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
$DefaultScenarioFile = Join-Path $PSScriptRoot "pie_scenarios.json"

if ([string]::IsNullOrWhiteSpace($ScenarioFile)) {
    $ScenarioFile = $DefaultScenarioFile
}

if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}
if (-not (Test-Path $ScenarioFile)) {
    throw "Scenario file not found: $ScenarioFile"
}

. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

function Invoke-BridgeTool {
    param(
        [string]$ToolName,
        [hashtable]$InputObject = @{}
    )
    return @{
        ok = $true
        result = (Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $BridgeTimeoutSeconds)
    }
}

function Wait-ForBridge {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds $BridgeTimeoutSeconds
}

$RunId = "{0}_{1}" -f $ScenarioName, (Get-Date -Format "yyyyMMdd_HHmmss")
$RunDir = Join-Path $OutputDir $RunId

Wait-ForBridge

$InputObject = @{
    scenario_name = $ScenarioName
    output_dir = $RunDir
    include_log_scan = $true
    abort_on_failure = $true
}

$Response = Invoke-BridgeTool -ToolName "run_pie_probe_scenario" -InputObject $InputObject
$ResultPayload = $Response.result | ConvertTo-Json -Depth 100 -Compress | ConvertFrom-Json

if (-not $KeepPIEOpen) {
    try {
        $State = Invoke-BridgeTool -ToolName "get_pie_session_state" -InputObject @{}
        $StateResult = $State.result | ConvertTo-Json -Depth 50 -Compress | ConvertFrom-Json
        if ($StateResult.is_playing_in_editor) {
            $null = Invoke-BridgeTool -ToolName "stop_play_session" -InputObject @{}
        }
    }
    catch {
    }
}

$Status = if ($ResultPayload.success) { "PASS" } else { "FAIL" }

Write-Host ""
Write-Host "=== PIE Runtime Probe Summary ==="
Write-Host "Scenario : $ScenarioName"
Write-Host "Status   : $Status"
Write-Host "Run Dir  : $($ResultPayload.output_dir)"
Write-Host "Summary  : $($ResultPayload.summary_path)"
if ($ResultPayload.captures.Count -gt 0) {
    Write-Host "Captures :"
    foreach ($Capture in $ResultPayload.captures) {
        Write-Host "  - $($Capture.label): $($Capture.path)"
    }
}
if ($ResultPayload.latest_log_summary) {
    $LogStatus = if ($ResultPayload.latest_log_summary.error_count -eq 0) { "PASS" } else { "WARN" }
    Write-Host "Log Scan : $LogStatus"
}

if (-not $ResultPayload.success) {
    exit 1
}

exit 0


