function Resolve-ForgePilotProjectRoot {
    param(
        [string]$StartDir = $PSScriptRoot
    )

    $dir = Resolve-Path $StartDir
    for ($i = 0; $i -lt 8; $i++) {
        $uproject = Get-ChildItem -Path $dir -Filter *.uproject -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($uproject) {
            return Split-Path -Parent $uproject.FullName
        }
        $parent = Split-Path -Parent $dir
        if (-not $parent -or $parent -eq $dir) {
            break
        }
        $dir = $parent
    }

    throw "Could not locate project root from $StartDir"
}

function ConvertTo-ForgePilotHashtable {
    param(
        [Parameter(Mandatory = $true)]
        $Value
    )

    if ($null -eq $Value) {
        return $null
    }

    if ($Value -is [System.Collections.IDictionary]) {
        $result = @{}
        foreach ($key in $Value.Keys) {
            $result[[string]$key] = ConvertTo-ForgePilotHashtable -Value $Value[$key]
        }
        return $result
    }

    if ($Value -is [System.Collections.IEnumerable] -and -not ($Value -is [string])) {
        $items = @()
        foreach ($item in $Value) {
            $items += ,(ConvertTo-ForgePilotHashtable -Value $item)
        }
        return $items
    }

    if ($Value -is [pscustomobject]) {
        $result = @{}
        foreach ($prop in $Value.PSObject.Properties) {
            $result[$prop.Name] = ConvertTo-ForgePilotHashtable -Value $prop.Value
        }
        return $result
    }

    return $Value
}

function Read-ForgePilotJsonAsHashtable {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path
    )

    $raw = Get-Content $Path -Raw
    if ($PSVersionTable.PSVersion.Major -ge 7) {
        return $raw | ConvertFrom-Json -AsHashtable
    }

    return ConvertTo-ForgePilotHashtable -Value ($raw | ConvertFrom-Json)
}

function Get-ForgePilotRecoveryStatePath {
    param(
        [string]$ProjectRoot
    )

    $root = Join-Path $ProjectRoot "Saved\ForgePilotRecovery"
    if (-not (Test-Path $root)) {
        New-Item -ItemType Directory -Path $root -Force | Out-Null
    }
    return Join-Path $root "recovery_state.json"
}

function Save-ForgePilotRecoveryState {
    param(
        [string]$ProjectRoot,
        [hashtable]$State
    )

    $statePath = Get-ForgePilotRecoveryStatePath -ProjectRoot $ProjectRoot
    $State | ConvertTo-Json -Depth 10 | Set-Content -Path $statePath -Encoding UTF8
    return $statePath
}

function Load-ForgePilotRecoveryState {
    param(
        [string]$ProjectRoot
    )

    $statePath = Get-ForgePilotRecoveryStatePath -ProjectRoot $ProjectRoot
    if (-not (Test-Path $statePath)) {
        return @{
            project_root = $ProjectRoot
            touched_assets = @()
            last_update_utc = [DateTime]::UtcNow.ToString("o")
        }
    }

    return Read-ForgePilotJsonAsHashtable -Path $statePath
}

function Update-ForgePilotTouchedAssets {
    param(
        [string]$ProjectRoot,
        [string[]]$AssetRelativePaths
    )

    $state = Load-ForgePilotRecoveryState -ProjectRoot $ProjectRoot
    $existing = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
    foreach ($item in @($state.touched_assets)) {
        [void]$existing.Add([string]$item)
    }
    foreach ($asset in @($AssetRelativePaths)) {
        if (-not [string]::IsNullOrWhiteSpace($asset)) {
            [void]$existing.Add($asset)
        }
    }
    $state.touched_assets = @($existing)
    $state.last_update_utc = [DateTime]::UtcNow.ToString("o")
    Save-ForgePilotRecoveryState -ProjectRoot $ProjectRoot -State $state | Out-Null
    return $state
}

function Get-ForgePilotAutosaveCandidates {
    param(
        [string]$ProjectRoot,
        [string[]]$AssetRelativePaths
    )

    $autosaveRoot = Join-Path $ProjectRoot "Saved\Autosaves\Game"
    $results = @()
    foreach ($asset in @($AssetRelativePaths)) {
        if ([string]::IsNullOrWhiteSpace($asset)) {
            continue
        }

        $relative = $asset -replace '^[\\/]+', ''
        $dir = Split-Path $relative -Parent
        $name = [System.IO.Path]::GetFileNameWithoutExtension($relative)
        $ext = [System.IO.Path]::GetExtension($relative)
        $searchDir = Join-Path $autosaveRoot $dir
        if (-not (Test-Path $searchDir)) {
            continue
        }

        $pattern = "{0}_Auto*{1}" -f $name, $ext
        $matches = Get-ChildItem -Path $searchDir -Filter $pattern -File -ErrorAction SilentlyContinue |
            Sort-Object LastWriteTime -Descending

        foreach ($match in $matches) {
            $results += [pscustomobject]@{
                asset_relative_path = $relative
                package_path = Join-Path $ProjectRoot "Content\$relative"
                autosave_path = $match.FullName
                last_write_time = $match.LastWriteTime
                length = $match.Length
            }
        }
    }

    return $results
}

function Restore-ForgePilotAutosave {
    param(
        [string]$ProjectRoot,
        [string]$AssetRelativePath,
        [string]$AutosavePath
    )

    $targetPath = Join-Path $ProjectRoot "Content\$AssetRelativePath"
    if (-not (Test-Path $AutosavePath)) {
        throw "Autosave file not found: $AutosavePath"
    }

    $targetDir = Split-Path -Parent $targetPath
    if (-not (Test-Path $targetDir)) {
        New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
    }

    $backupPath = $null
    if (Test-Path $targetPath) {
        $backupPath = "$targetPath.bak_$(Get-Date -Format yyyyMMdd_HHmmss)"
        Copy-Item -Path $targetPath -Destination $backupPath -Force
    }

    Copy-Item -Path $AutosavePath -Destination $targetPath -Force
    return [pscustomobject]@{
        restored = $true
        asset_relative_path = $AssetRelativePath
        target_path = $targetPath
        autosave_path = $AutosavePath
        backup_path = $backupPath
    }
}
