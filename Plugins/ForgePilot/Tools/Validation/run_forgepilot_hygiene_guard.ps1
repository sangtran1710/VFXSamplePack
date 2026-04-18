[CmdletBinding()]
param(
    [switch]$RunBuild,
    [switch]$RunBridge,
    [switch]$RunAskAssist,
    [string]$JsonOut = '',
    [int]$LargeFileThresholdMB = 50,
    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [int]$TimeoutSeconds = 30
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$Healthcheck = Join-Path $ScriptRoot 'run_forgepilot_healthcheck.ps1'

$argsList = @(
    '-ExecutionPolicy', 'Bypass',
    '-File', $Healthcheck,
    '-RequireClean',
    '-LargeFileThresholdMB', "$LargeFileThresholdMB",
    '-TimeoutSeconds', "$TimeoutSeconds"
)

if ($RunBuild) {
    $argsList += '-RunBuild'
}
if ($RunBridge) {
    $argsList += '-RunBridge'
}
if ($RunAskAssist) {
    $argsList += '-RunAskAssist'
}
if (-not [string]::IsNullOrWhiteSpace($JsonOut)) {
    $argsList += @('-JsonOut', $JsonOut)
}
if (-not [string]::IsNullOrWhiteSpace($BridgeHost)) {
    $argsList += @('-BridgeHost', $BridgeHost)
}
if ($BridgePort -gt 0) {
    $argsList += @('-BridgePort', "$BridgePort")
}

& powershell @argsList
exit $LASTEXITCODE
