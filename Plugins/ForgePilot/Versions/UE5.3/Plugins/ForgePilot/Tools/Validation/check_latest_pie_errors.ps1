param(
    [string]$ProjectRoot = ""
)

$ErrorActionPreference = "Stop"

$PluginRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
if ([string]::IsNullOrWhiteSpace($ProjectRoot)) {
    $ProjectRoot = Split-Path -Parent (Split-Path -Parent $PluginRoot)
}

$LogDir = Join-Path $ProjectRoot "Saved\Logs"
if (-not (Test-Path $LogDir)) {
    throw "Log directory not found: $LogDir"
}

$LatestLog = Get-ChildItem -Path $LogDir -Filter "*.log" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
if (-not $LatestLog) {
    throw "No log files found in $LogDir"
}

$Patterns = @(
    'Blueprint Runtime Error:',
    'LogScript: Error:',
    'PIE: Error:',
    'Accessed None trying to read property'
)

$MatchedLines = foreach ($Line in Get-Content -Path $LatestLog.FullName) {
    foreach ($Pattern in $Patterns) {
        if ($Line -like "*$Pattern*") {
            [pscustomobject]@{
                pattern = $Pattern
                line = $Line.Trim()
            }
            break
        }
    }
}

if (-not $MatchedLines) {
    Write-Host "PASS: No PIE runtime errors found in latest session."
    Write-Host "Log: $($LatestLog.FullName)"
    exit 0
}

$Grouped = $MatchedLines | Group-Object line | Sort-Object Count -Descending

Write-Host "FAIL: PIE/runtime errors found in latest session."
Write-Host "Log: $($LatestLog.FullName)"
Write-Host ""
Write-Host "=== Error Summary ==="
foreach ($Group in $Grouped) {
    Write-Host ("[{0}] {1}" -f $Group.Count, $Group.Name)
}

exit 1
