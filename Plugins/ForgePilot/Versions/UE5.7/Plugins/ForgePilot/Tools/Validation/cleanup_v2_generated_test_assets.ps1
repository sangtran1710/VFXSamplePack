[CmdletBinding()]
param(
    [string]$ProjectRoot = '',
    [string[]]$TargetDirs = @()
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

. "$PSScriptRoot\forgepilot_recovery.ps1"

if ([string]::IsNullOrWhiteSpace($ProjectRoot)) {
    $ProjectRoot = Resolve-ForgePilotProjectRoot -StartDir $PSScriptRoot
}

$workspaceRoot = $ProjectRoot
$normalizedWorkspaceRoot = [System.IO.Path]::GetFullPath($workspaceRoot).TrimEnd('\')
$workspaceRootPrefix = "$normalizedWorkspaceRoot\"

if ($TargetDirs.Count -eq 0) {
    $TargetDirs = @(
        (Join-Path $workspaceRoot 'Content\ForgePilot_Test\V2Platform'),
        (Join-Path $workspaceRoot 'Content\ForgePilot_Test\CoreEnergy'),
        (Join-Path $workspaceRoot 'Content\ForgePilot_Test\UE57')
    )
}

$summary = @()

foreach ($targetDir in $TargetDirs) {
    $fullTargetDir = [System.IO.Path]::GetFullPath($targetDir).TrimEnd('\')
    if ($fullTargetDir.Equals($normalizedWorkspaceRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to delete the workspace root itself: $fullTargetDir"
    }

    if (-not $fullTargetDir.StartsWith($workspaceRootPrefix, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to delete outside workspace: $fullTargetDir"
    }

    if (-not (Test-Path -LiteralPath $fullTargetDir)) {
        $summary += [pscustomobject]@{
            path = $fullTargetDir
            existed = $false
            removed_files = 0
        }
        continue
    }

    $files = @(Get-ChildItem -LiteralPath $fullTargetDir -File -Recurse -Force)
    $fileCount = $files.Count

    Get-ChildItem -LiteralPath $fullTargetDir -Force | Remove-Item -Recurse -Force

    $summary += [pscustomobject]@{
        path = $fullTargetDir
        existed = $true
        removed_files = $fileCount
    }
}

$summary | ConvertTo-Json -Depth 5
