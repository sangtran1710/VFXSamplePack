param(
    [string]$ProjectRoot,
    [string]$ProjectPath,
    [string]$UeRoot,
    [string]$Target = "",
    [string]$Platform = "Win64",
    [string]$Configuration = "Development",
    [switch]$RelaunchEditor = $true,
    [string[]]$TouchedAssets = @()
)

. "$PSScriptRoot\forgepilot_recovery.ps1"

function Resolve-ForgePilotProjectFile {
    param([Parameter(Mandatory = $true)][string]$Root)

    $projects = @(Get-ChildItem -Path $Root -Filter *.uproject -File -ErrorAction SilentlyContinue)
    if ($projects.Count -eq 1) {
        return $projects[0].FullName
    }
    if ($projects.Count -gt 1) {
        $names = ($projects | ForEach-Object { $_.Name }) -join ', '
        throw "Multiple .uproject files found under '$Root'. Pass -ProjectPath explicitly. Found: $names"
    }

    throw "No .uproject file found under '$Root'. Pass -ProjectPath explicitly."
}

function Resolve-ForgePilotEditorTarget {
    param([Parameter(Mandatory = $true)][string]$Root)

    $sourceRoot = Join-Path $Root "Source"
    if (-not (Test-Path $sourceRoot)) {
        throw "Source folder not found under '$Root'. Pass -Target explicitly."
    }

    $targets = @(Get-ChildItem -Path $sourceRoot -Filter *Editor.Target.cs -File -Recurse -ErrorAction SilentlyContinue)
    if ($targets.Count -eq 1 -and $targets[0].Name -match '^(.+Editor)\.Target\.cs$') {
        return $Matches[1]
    }
    if ($targets.Count -gt 1) {
        $names = ($targets | ForEach-Object { $_.Name }) -join ', '
        throw "Multiple editor targets found under '$sourceRoot'. Pass -Target explicitly. Found: $names"
    }

    throw "No *Editor.Target.cs file found under '$sourceRoot'. Pass -Target explicitly."
}

function Resolve-ForgePilotEngineRoot {
    param(
        [Parameter(Mandatory = $true)][string]$ResolvedProjectPath
    )

    $candidateRoots = @()
    foreach ($envName in @('UE_ROOT', 'UNREAL_ENGINE_ROOT', 'UE57_ROOT')) {
        $value = [System.Environment]::GetEnvironmentVariable($envName)
        if (-not [string]::IsNullOrWhiteSpace($value)) {
            $candidateRoots += $value
        }
    }

    try {
        $projectJson = Get-Content -Path $ResolvedProjectPath -Raw | ConvertFrom-Json
        $association = [string]$projectJson.EngineAssociation
        if ($association -match '^\d+\.\d+') {
            $candidateRoots += "C:\Program Files\Epic Games\UE_$association"
        }
        elseif (-not [string]::IsNullOrWhiteSpace($association) -and (Test-Path $association)) {
            $candidateRoots += $association
        }
    }
    catch {
    }

    $candidateRoots += @(
        "C:\Program Files\Epic Games\UE_5.7",
        "C:\Program Files\Epic Games\UE_5.6",
        "C:\Program Files\Epic Games\UE_5.5",
        "C:\Program Files\Epic Games\UE_5.4"
    )

    foreach ($candidate in @($candidateRoots | Where-Object { -not [string]::IsNullOrWhiteSpace($_) } | Select-Object -Unique)) {
        if (Test-Path (Join-Path $candidate "Engine\Build\BatchFiles\Build.bat")) {
            return $candidate
        }
    }

    return $null
}

if (-not $ProjectRoot) {
    $ProjectRoot = Resolve-ForgePilotProjectRoot -StartDir $PSScriptRoot
}
if (-not $ProjectPath) {
    $ProjectPath = Resolve-ForgePilotProjectFile -Root $ProjectRoot
}
if (-not (Test-Path $ProjectPath)) {
    throw "Project file not found: $ProjectPath"
}
if (-not $Target) {
    $Target = Resolve-ForgePilotEditorTarget -Root $ProjectRoot
}

if ($TouchedAssets.Count -gt 0) {
    Update-ForgePilotTouchedAssets -ProjectRoot $ProjectRoot -AssetRelativePaths $TouchedAssets | Out-Null
}

if (-not $UeRoot) {
    $UeRoot = Resolve-ForgePilotEngineRoot -ResolvedProjectPath $ProjectPath
}
if (-not $UeRoot) {
    throw "Could not resolve Unreal Engine root. Pass -UeRoot explicitly."
}

$buildBat = Join-Path $UeRoot "Engine\Build\BatchFiles\Build.bat"
$editorExe = Join-Path $UeRoot "Engine\Binaries\Win64\UnrealEditor.exe"
if (-not (Test-Path $buildBat)) {
    throw "Build.bat not found: $buildBat"
}

$closedEditors = @()
Get-Process UnrealEditor -ErrorAction SilentlyContinue | ForEach-Object {
    $closedEditors += $_.Id
    Stop-Process -Id $_.Id -Force
}
Get-Process CrashReportClient -ErrorAction SilentlyContinue | Stop-Process -Force
if ($closedEditors.Count -gt 0) {
    Start-Sleep -Seconds 2
}

$argList = @(
    $Target,
    $Platform,
    $Configuration,
    "`"$ProjectPath`"",
    "-WaitMutex",
    "-NoHotReloadFromIDE"
)

$buildOutput = & $buildBat @argList 2>&1
$exitCode = $LASTEXITCODE

$relaunchAttempted = $false
$relaunchSucceeded = $false
if ($RelaunchEditor -and $exitCode -eq 0 -and (Test-Path $editorExe)) {
    $relaunchAttempted = $true
    Start-Process -FilePath $editorExe -ArgumentList "`"$ProjectPath`""
    $relaunchSucceeded = $true
}

[ordered]@{
    success = ($exitCode -eq 0)
    action = "build_unreal_with_recovery"
    project_root = $ProjectRoot
    project_path = $ProjectPath
    ue_root = $UeRoot
    target = $Target
    platform = $Platform
    configuration = $Configuration
    closed_editor_pids = $closedEditors
    exit_code = $exitCode
    relaunch_attempted = $relaunchAttempted
    relaunch_succeeded = $relaunchSucceeded
    output = @($buildOutput)
} | ConvertTo-Json -Depth 8
