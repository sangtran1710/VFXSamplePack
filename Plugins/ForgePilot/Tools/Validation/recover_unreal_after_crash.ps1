param(
    [string]$ProjectRoot,
    [string[]]$AssetRelativePaths,
    [switch]$ApplyRestore
)

. "$PSScriptRoot\forgepilot_recovery.ps1"

if (-not $ProjectRoot) {
    $ProjectRoot = Resolve-ForgePilotProjectRoot -StartDir $PSScriptRoot
}

$state = Load-ForgePilotRecoveryState -ProjectRoot $ProjectRoot
$assets = @()
if ($AssetRelativePaths) {
    $assets += $AssetRelativePaths
}
if ($state.touched_assets) {
    $assets += @($state.touched_assets)
}
$assets = $assets | Where-Object { -not [string]::IsNullOrWhiteSpace($_) } | Select-Object -Unique

$candidates = Get-ForgePilotAutosaveCandidates -ProjectRoot $ProjectRoot -AssetRelativePaths $assets
$grouped = $candidates | Group-Object asset_relative_path

$summary = [ordered]@{
    success = $true
    action = "recover_unreal_after_crash"
    project_root = $ProjectRoot
    apply_restore = [bool]$ApplyRestore
    touched_asset_count = @($assets).Count
    candidate_count = @($candidates).Count
    assets = @()
}

foreach ($group in $grouped) {
    $latest = $group.Group | Sort-Object last_write_time -Descending | Select-Object -First 1
    $entry = [ordered]@{
        asset_relative_path = $group.Name
        candidate_count = @($group.Group).Count
        latest_autosave = $latest.autosave_path
        latest_autosave_time = $latest.last_write_time
    }

    if ($ApplyRestore) {
        $runningEditor = Get-Process UnrealEditor -ErrorAction SilentlyContinue
        if ($runningEditor) {
            throw "Close UnrealEditor before applying autosave restore."
        }

        $restore = Restore-ForgePilotAutosave -ProjectRoot $ProjectRoot -AssetRelativePath $group.Name -AutosavePath $latest.autosave_path
        $entry.restore = $restore
    }

    $summary.assets += [pscustomobject]$entry
}

$summary | ConvertTo-Json -Depth 10
