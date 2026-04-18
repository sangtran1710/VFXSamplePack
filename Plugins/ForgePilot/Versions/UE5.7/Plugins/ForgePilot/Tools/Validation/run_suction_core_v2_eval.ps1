[CmdletBinding()]
param(
    [string]$SuitePath = "",
    [string]$OutputRoot = "",
    [switch]$RunSemanticOps,
    [string]$HeroMaterialPath,
    [string]$CoreMaterialPath,
    [switch]$KeepAssets
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($SuitePath)) {
    $SuitePath = Join-Path $PSScriptRoot 'suction_core_v2_eval_tasks.json'
}

if ([string]::IsNullOrWhiteSpace($OutputRoot)) {
    $savedRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..\..\..\Saved')
    $OutputRoot = Join-Path $savedRoot.Path 'ForgePilot\SuctionCoreV2Eval'
}

$genericScript = Join-Path $PSScriptRoot 'run_effect_v2_eval.ps1'
$invokeArgs = @(
    '-ExecutionPolicy', 'Bypass',
    '-File', $genericScript,
    '-SuitePath', $SuitePath,
    '-OutputRoot', $OutputRoot,
    '-WorkflowToolName', 'run_suction_core_v2_workflow'
)

if ($RunSemanticOps) {
    $invokeArgs += '-RunSemanticOps'
}
if ($KeepAssets) {
    $invokeArgs += '-KeepAssets'
}
if (-not [string]::IsNullOrWhiteSpace($HeroMaterialPath)) {
    $invokeArgs += @('-HeroMaterialPath', $HeroMaterialPath)
}
if (-not [string]::IsNullOrWhiteSpace($CoreMaterialPath)) {
    $invokeArgs += @('-CoreMaterialPath', $CoreMaterialPath)
}

& powershell @invokeArgs
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
