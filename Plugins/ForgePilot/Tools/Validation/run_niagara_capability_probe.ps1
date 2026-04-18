param(
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 0,
    [int]$BridgeTimeoutSeconds = 120,
    [string]$OutputDir = "$env:USERPROFILE\Downloads\ForgePilotNiagaraProbes",
    [ValidateSet("full-chain","asset-only","renderer-only")]
    [string]$Mode = "full-chain",
    [switch]$KeepAsset,
    [switch]$KeepActor
)

$ErrorActionPreference = "Stop"
$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}
. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

$ProbeScript = Join-Path $PSScriptRoot "niagara_capability_probe.py"
if (-not (Test-Path $ProbeScript)) {
    throw "Probe script not found: $ProbeScript"
}

$Args = @(
    $ProbeScript,
    "--host", $BridgeHost,
    "--port", "$BridgePort",
    "--timeout", "$BridgeTimeoutSeconds",
    "--output-dir", $OutputDir,
    "--mode", $Mode
)

if ($KeepAsset) {
    $Args += "--keep-asset"
}
if ($KeepActor) {
    $Args += "--keep-actor"
}

& python @Args
if ($LASTEXITCODE -ne 0) {
    throw "Niagara capability probe failed."
}


