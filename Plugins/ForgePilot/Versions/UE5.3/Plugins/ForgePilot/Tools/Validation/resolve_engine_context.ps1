[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-ForgePilotInstalledUERoot {
    param(
        [string]$PreferredRoot = ""
    )

    $candidates = @()
    if (-not [string]::IsNullOrWhiteSpace($PreferredRoot)) {
        $candidates += $PreferredRoot
    }

    if (-not [string]::IsNullOrWhiteSpace($env:FORGEPILOT_UE_ROOT)) {
        $candidates += $env:FORGEPILOT_UE_ROOT
    }

    if (-not [string]::IsNullOrWhiteSpace($env:UE_ROOT)) {
        $candidates += $env:UE_ROOT
    }

    $candidates += @(
        "C:/Program Files/Epic Games/UE_5.7",
        "C:/Program Files/Epic Games/UE_5.6",
        "C:/Program Files/Epic Games/UE_5.5",
        "C:/Program Files/Epic Games/UE_5.4",
        "C:/Program Files/Epic Games/UE_5.3"
    )

    foreach ($candidate in $candidates | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }) {
        $resolved = [System.IO.Path]::GetFullPath($candidate)
        if (Test-Path (Join-Path $resolved 'Engine')) {
            return $resolved
        }
    }

    throw "Unable to resolve a valid Unreal Engine root. Set FORGEPILOT_UE_ROOT or UE_ROOT explicitly."
}

function Get-ForgePilotEngineVersionTag {
    param(
        [Parameter(Mandatory = $true)][string]$UERoot
    )

    $leaf = Split-Path -Leaf ([System.IO.Path]::GetFullPath($UERoot))
    if ($leaf -match '^UE_(\d+)\.(\d+)$') {
        return [pscustomobject]@{
            Label = "Unreal Engine $($matches[1]).$($matches[2])"
            Slug = "ue$($matches[1])$($matches[2])"
            Short = "$($matches[1]).$($matches[2])"
        }
    }

    return [pscustomobject]@{
        Label = "Unreal Engine"
        Slug = "ue_current"
        Short = ""
    }
}
