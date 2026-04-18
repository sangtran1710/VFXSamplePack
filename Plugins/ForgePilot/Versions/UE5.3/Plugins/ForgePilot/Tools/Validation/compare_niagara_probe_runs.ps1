param(
    [Parameter(Mandatory = $true)][string]$BaselineSummary,
    [Parameter(Mandatory = $true)][string]$CandidateSummary,
    [string]$OutputJson
)

$ErrorActionPreference = "Stop"

$ScriptPath = Join-Path $PSScriptRoot "compare_niagara_probe_runs.py"
if (-not (Test-Path $ScriptPath)) {
    throw "Compare script not found: $ScriptPath"
}

$PythonCommand = Get-Command python -ErrorAction SilentlyContinue
if (-not $PythonCommand) {
    $PythonCommand = Get-Command py -ErrorAction SilentlyContinue
}
if (-not $PythonCommand) {
    throw "Neither 'python' nor 'py' is available on PATH."
}

$Args = @(
    $ScriptPath,
    "--baseline-summary", $BaselineSummary,
    "--candidate-summary", $CandidateSummary
)

if ($OutputJson) {
    $Args += @("--output", $OutputJson)
}

& $PythonCommand.Source @Args
if ($LASTEXITCODE -ne 0) {
    throw "Niagara probe comparison failed."
}
