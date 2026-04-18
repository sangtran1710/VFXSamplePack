[CmdletBinding()]
param(
    [string]$FolderPath = '/Game/VFX',
    [switch]$NoDependencies,
    [switch]$NoFileInfo,
    [int]$MaxAssetsPerSection = 250,
    [int]$MaxDependencyEdges = 500,
    [string]$JsonOut = '',
    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [int]$TimeoutSeconds = 120
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)

$RequestedBridgeHost = $BridgeHost
$RequestedBridgePort = $BridgePort
. (Join-Path $ScriptRoot 'invoke_bridge.ps1')

if ([string]::IsNullOrWhiteSpace($BridgeHost)) {
    $BridgeHost = if ([string]::IsNullOrWhiteSpace($RequestedBridgeHost)) { Get-ForgePilotBridgeHostDefault } else { $RequestedBridgeHost }
}
if ($BridgePort -le 0) {
    $BridgePort = if ($RequestedBridgePort -gt 0) { $RequestedBridgePort } else { Get-ForgePilotBridgePortDefault }
}

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds $TimeoutSeconds

$result = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName 'index_vfx_content_library' -InputObject @{
    folder_path = $FolderPath
    recursive = $true
    include_dependencies = -not [bool]$NoDependencies
    include_file_info = -not [bool]$NoFileInfo
    max_assets_per_section = $MaxAssetsPerSection
    max_dependency_edges = $MaxDependencyEdges
} -TimeoutSeconds $TimeoutSeconds

if (-not [string]::IsNullOrWhiteSpace($JsonOut)) {
    $jsonPath = if ([System.IO.Path]::IsPathRooted($JsonOut)) {
        $JsonOut
    }
    else {
        Join-Path $PluginRoot $JsonOut
    }

    $jsonParent = Split-Path -Parent $jsonPath
    if (-not [string]::IsNullOrWhiteSpace($jsonParent) -and -not (Test-Path $jsonParent)) {
        New-Item -ItemType Directory -Force -Path $jsonParent | Out-Null
    }

    $result | ConvertTo-Json -Depth 100 | Set-Content -Path $jsonPath -Encoding UTF8
}

$summary = $result.effect_library_summary
[pscustomobject]@{
    success = [bool]$result.success
    folder_path = [string]$result.folder_path
    total_assets = [int]$summary.total_asset_count
    niagara_systems = [int]$summary.niagara_system_count
    material_instances = [int]$summary.material_instance_count
    templates = [int]$summary.template_count
    dependency_edges = [int]$result.asset_dependencies.returned_edge_count
    json_out = $JsonOut
} | ConvertTo-Json -Depth 10
