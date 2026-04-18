param(
    [Parameter(Mandatory = $true)][string]$BaselineSummary,
    [Parameter(Mandatory = $true)][string]$CandidateSummary
)

$ErrorActionPreference = "Stop"

Add-Type -AssemblyName System.Drawing

function Get-CaptureMap {
    param([string]$SummaryPath)

    if (-not (Test-Path $SummaryPath)) {
        throw "Summary file not found: $SummaryPath"
    }

    $Summary = Get-Content -Path $SummaryPath -Raw | ConvertFrom-Json
    $Map = @{}
    foreach ($Capture in $Summary.captures) {
        $Map[$Capture.label] = $Capture.path
    }
    return $Map
}

function Get-ImageInfo {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        return [pscustomobject]@{
            exists = $false
            width = 0
            height = 0
            bytes = 0
            sha256 = ""
        }
    }

    $Info = Get-Item $Path
    $Hash = (Get-FileHash -Path $Path -Algorithm SHA256).Hash
    $Image = [System.Drawing.Image]::FromFile($Path)
    try {
        return [pscustomobject]@{
            exists = $true
            width = $Image.Width
            height = $Image.Height
            bytes = $Info.Length
            sha256 = $Hash
        }
    }
    finally {
        $Image.Dispose()
    }
}

$Baseline = Get-CaptureMap -SummaryPath $BaselineSummary
$Candidate = Get-CaptureMap -SummaryPath $CandidateSummary
$Labels = ($Baseline.Keys + $Candidate.Keys | Sort-Object -Unique)

$Rows = foreach ($Label in $Labels) {
    $BasePath = $Baseline[$Label]
    $CandPath = $Candidate[$Label]
    $BaseInfo = Get-ImageInfo -Path $BasePath
    $CandInfo = Get-ImageInfo -Path $CandPath

    [pscustomobject]@{
        label = $Label
        baseline_exists = $BaseInfo.exists
        candidate_exists = $CandInfo.exists
        baseline_size = if ($BaseInfo.exists) { "$($BaseInfo.width)x$($BaseInfo.height)" } else { "" }
        candidate_size = if ($CandInfo.exists) { "$($CandInfo.width)x$($CandInfo.height)" } else { "" }
        baseline_bytes = $BaseInfo.bytes
        candidate_bytes = $CandInfo.bytes
        same_hash = ($BaseInfo.exists -and $CandInfo.exists -and $BaseInfo.sha256 -eq $CandInfo.sha256)
        baseline_path = $BasePath
        candidate_path = $CandPath
    }
}

Write-Host ""
Write-Host "=== Probe Capture Comparison ==="
$Rows | Format-Table -AutoSize

$Missing = $Rows | Where-Object { -not $_.baseline_exists -or -not $_.candidate_exists }
if ($Missing.Count -gt 0) {
    exit 1
}

exit 0
