[CmdletBinding()]
param(
    [string]$BridgeHost = '',
    [int]$BridgePort = 0,
    [double]$TimeoutSeconds = 30.0
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)
$McpServerPath = Join-Path $PluginRoot 'Tools\MCP\forgepilot_mcp_server.py'

$RequestedBridgeHost = $BridgeHost
$RequestedBridgePort = $BridgePort
. (Join-Path $ScriptRoot 'invoke_bridge.ps1')

if ([string]::IsNullOrWhiteSpace($BridgeHost)) {
    $BridgeHost = if ([string]::IsNullOrWhiteSpace($RequestedBridgeHost)) { Get-ForgePilotBridgeHostDefault } else { $RequestedBridgeHost }
}

if ($BridgePort -le 0) {
    $BridgePort = if ($RequestedBridgePort -gt 0) { $RequestedBridgePort } else { Get-ForgePilotBridgePortDefault }
}

$Summary = [ordered]@{
    success = $false
    bridge = [ordered]@{}
    metadata = [ordered]@{}
    ask = [ordered]@{}
    assist = [ordered]@{}
    audit = [ordered]@{}
    assertions = [ordered]@{}
    cleanup = [ordered]@{}
    steps = @()
}

function Add-StepResult {
    param(
        [Parameter(Mandatory = $true)][string]$Name,
        [Parameter(Mandatory = $true)]$Result
    )

    $Summary.steps += [pscustomobject]@{
        name = $Name
        result = $Result
    }
}

function Get-PythonCommand {
    foreach ($Candidate in @('python', 'py')) {
        try {
            & $Candidate --version *> $null
            if ($LASTEXITCODE -eq 0) {
                return $Candidate
            }
        }
        catch {
        }
    }

    return $null
}

function Invoke-ValidationTool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{},
        [Parameter(Mandatory = $true)][string]$StepName,
        [int]$ToolTimeoutSeconds = 60
    )

    $result = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds $ToolTimeoutSeconds
    Add-StepResult -Name $StepName -Result $result
    return $result
}

function Assert-ToolMetadata {
    param(
        [Parameter(Mandatory = $true)][hashtable]$ToolMap,
        [Parameter(Mandatory = $true)][string]$ToolName,
        [Parameter(Mandatory = $true)][string]$ExpectedMode,
        [Parameter(Mandatory = $true)][string]$ExpectedMutationLevel,
        [Parameter(Mandatory = $true)][bool]$ExpectedSafeDefault,
        [Parameter(Mandatory = $true)][bool]$ExpectedProjectGrounded,
        [Parameter(Mandatory = $true)][string]$ExpectedDefaultExposure
    )

    if (-not $ToolMap.ContainsKey($ToolName)) {
        throw "Tool '$ToolName' not found in list_tools output."
    }

    $tool = $ToolMap[$ToolName]
    $record = [ordered]@{
        mode = [string]$tool.mode
        mutation_level = [string]$tool.mutation_level
        safe_default = [bool]$tool.safe_default
        project_grounded = [bool]$tool.project_grounded
        default_exposure = [string]$tool.default_exposure
    }
    $Summary.metadata[$ToolName] = $record

    if ($record.mode -ne $ExpectedMode) {
        throw "Tool '$ToolName' mode mismatch. Expected '$ExpectedMode', got '$($record.mode)'."
    }
    if ($record.mutation_level -ne $ExpectedMutationLevel) {
        throw "Tool '$ToolName' mutation_level mismatch. Expected '$ExpectedMutationLevel', got '$($record.mutation_level)'."
    }
    if ($record.safe_default -ne $ExpectedSafeDefault) {
        throw "Tool '$ToolName' safe_default mismatch. Expected '$ExpectedSafeDefault', got '$($record.safe_default)'."
    }
    if ($record.project_grounded -ne $ExpectedProjectGrounded) {
        throw "Tool '$ToolName' project_grounded mismatch. Expected '$ExpectedProjectGrounded', got '$($record.project_grounded)'."
    }
    if ($record.default_exposure -ne $ExpectedDefaultExposure) {
        throw "Tool '$ToolName' default_exposure mismatch. Expected '$ExpectedDefaultExposure', got '$($record.default_exposure)'."
    }
}

$ScratchRoot = '/Game/ForgePilot_Test/Assist'
$ScratchMoved = '/Game/ForgePilot_Test/Assist/Moved'
$ScratchStamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$DuplicateName = "M_AskAssistProbe_$ScratchStamp"
$RenamedName = "M_AskAssistProbe_Renamed_$ScratchStamp"
$FinalName = "M_AskAssistProbe_Final_$ScratchStamp"
$DuplicatePath = "$ScratchRoot/$DuplicateName.$DuplicateName"
$RenamedPath = "$ScratchRoot/$RenamedName.$RenamedName"
$FinalPath = "$ScratchMoved/$FinalName.$FinalName"
$CleanupAssetPaths = New-Object System.Collections.Generic.List[string]

try {
    Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($TimeoutSeconds))

    $PingResponse = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = 'ping' } -TimeoutSeconds 15
    Add-StepResult -Name 'bridge_ping' -Result $PingResponse
    $Summary.bridge.ping = $PingResponse

    $ListToolsResponse = Invoke-ForgePilotBridgeRequest -BridgeHost $BridgeHost -BridgePort $BridgePort -Payload @{ action = 'list_tools' } -TimeoutSeconds 30
    Add-StepResult -Name 'bridge_list_tools' -Result $ListToolsResponse
    if (-not $ListToolsResponse.ok) {
        throw "Bridge list_tools failed."
    }

    $ToolMap = @{}
    foreach ($tool in @($ListToolsResponse.tools)) {
        if ($tool -and $tool.name) {
            $ToolMap[[string]$tool.name] = $tool
        }
    }

    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'search_assets' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'find_empty_folders' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'index_vfx_content_library' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'list_recent_assets' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'find_large_assets' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'find_assets_modified_since' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'trace_asset_relationships' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_blueprint_summary' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'review_blueprint_asset' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'search_blueprint_nodes' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_material_info' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'review_material_asset' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_niagara_system_info' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'review_niagara_system' -ExpectedMode 'ask' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'create_asset_folder' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'fix_redirectors_in_folder' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'batch_rename_assets' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'batch_move_assets' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'move_asset' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'delete_asset' -ExpectedMode 'assist' -ExpectedMutationLevel 'safe_content_op' -ExpectedSafeDefault $false -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'browse_to_asset' -ExpectedMode 'assist' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $true -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'default'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'open_asset' -ExpectedMode 'assist' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $false -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'get_pie_session_state' -ExpectedMode 'build' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $false -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'review_effect_failure_taxonomy_v2' -ExpectedMode 'build' -ExpectedMutationLevel 'read_only' -ExpectedSafeDefault $false -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'advanced'
    Assert-ToolMetadata -ToolMap $ToolMap -ToolName 'run_effect_workflow_v2' -ExpectedMode 'build' -ExpectedMutationLevel 'advanced_authoring' -ExpectedSafeDefault $false -ExpectedProjectGrounded $true -ExpectedDefaultExposure 'advanced'

    $Summary.assertions.bridge_ping_ok = [bool]$PingResponse.ok
    $Summary.assertions.list_tools_has_metadata = $true

    $PythonCmd = Get-PythonCommand
    if (-not $PythonCmd) {
        throw 'No python command found on PATH. MCP tools/list metadata check cannot run.'
    }
    $mcpPayload = @(
        '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}',
        '{"jsonrpc":"2.0","id":2,"method":"tools/list","params":{}}'
    ) -join "`n"
    $mcpLines = $mcpPayload | & $PythonCmd $McpServerPath
    $mcpObjects = @($mcpLines | Where-Object { $_ -and $_.Trim() } | ForEach-Object { $_ | ConvertFrom-Json })
    $mcpToolList = $mcpObjects | Where-Object { $_.id -eq 2 } | Select-Object -First 1
    if (-not $mcpToolList -or -not $mcpToolList.result -or -not $mcpToolList.result.tools) {
        throw 'MCP tools/list did not return a valid tools payload.'
    }
    Add-StepResult -Name 'mcp_tools_list' -Result $mcpToolList

    $McpTool = @($mcpToolList.result.tools | Where-Object { $_.name -eq 'create_asset_folder' }) | Select-Object -First 1
    if (-not $McpTool) {
        throw "MCP tools/list did not include 'create_asset_folder'."
    }
    foreach ($FieldName in @('tool_group', 'unreal_domain', 'workflow_lane', 'surface_area', 'primary_lane', 'vfx_affinity', 'capability_tags', 'mode', 'mutation_level', 'safe_default', 'project_grounded', 'default_exposure')) {
        if (-not $McpTool.PSObject.Properties.Name.Contains($FieldName)) {
            throw "MCP tools/list metadata field '$FieldName' is missing."
        }
    }
    $Summary.assertions.mcp_tools_list_has_metadata = $true

    $ContextResult = Invoke-ValidationTool -ToolName 'get_context' -StepName 'get_context'
    $SelectedAssetsResult = Invoke-ValidationTool -ToolName 'get_selected_assets' -StepName 'get_selected_assets'
    $SearchResult = Invoke-ValidationTool -ToolName 'search_assets' -InputObject @{
        query = 'M_FireTrail'
        class_names = 'Material'
        max_results = 5
    } -StepName 'search_assets'

    if (@($SearchResult.assets).Count -eq 0) {
        $SearchResult = Invoke-ValidationTool -ToolName 'search_assets' -InputObject @{
            query = 'M_Fire'
            class_names = 'Material'
            max_results = 10
        } -StepName 'search_assets_fallback'
    }

    if (@($SearchResult.assets).Count -eq 0) {
        $ScanResult = Invoke-ValidationTool -ToolName 'scan_assets' -InputObject @{
            folder_path = '/Game'
            asset_type = 'Material'
            max_results = 25
        } -StepName 'scan_assets_fallback'

        if (@($ScanResult.assets).Count -gt 0) {
            $SearchResult = [pscustomobject]@{
                assets = @($ScanResult.assets | Select-Object -First 5 | ForEach-Object {
                    [pscustomobject]@{
                        path = $_.path
                    }
                })
                returned_count = @($ScanResult.assets).Count
            }
        }
    }

    $SourceAssetPath = ''
    if (@($SearchResult.assets).Count -gt 0) {
        $SourceAssetPath = [string]$SearchResult.assets[0].path
    }

    $Summary.ask.context = [ordered]@{
        selected_asset_count = $ContextResult.selected_asset_count
        selected_folder_count = $ContextResult.selected_folder_count
        opened_asset_count = $ContextResult.opened_asset_count
    }
    $Summary.ask.selected_assets = [ordered]@{
        count = $SelectedAssetsResult.count
    }

    $CreateFolderDryRun = Invoke-ValidationTool -ToolName 'create_asset_folder' -InputObject @{
        folder_path = $ScratchRoot
        apply_changes = $false
    } -StepName 'create_asset_folder_dry_run'
    $CreateFolderApply = Invoke-ValidationTool -ToolName 'create_asset_folder' -InputObject @{
        folder_path = $ScratchRoot
        apply_changes = $true
    } -StepName 'create_asset_folder_apply'
    $CreateMovedApply = Invoke-ValidationTool -ToolName 'create_asset_folder' -InputObject @{
        folder_path = $ScratchMoved
        apply_changes = $true
    } -StepName 'create_asset_folder_moved_apply'

    if ([string]::IsNullOrWhiteSpace($SourceAssetPath)) {
        $SeedAssetName = "T_AskAssistSeed_$ScratchStamp"
        $SeedImport = Invoke-ValidationTool -ToolName 'import_asset_from_file' -InputObject @{
            source_file = (Join-Path $PluginRoot 'Tools\Validation\ice_realistic_inspect.jpg')
            destination_path = $ScratchRoot
            destination_name = $SeedAssetName
            replace_existing = $true
            save_asset = $true
            sync_browser = $true
            apply_changes = $true
        } -StepName 'import_asset_from_file_seed'

        $SourceAssetPath = [string]$SeedImport.primary_object_path
        $CleanupAssetPaths.Add($SourceAssetPath)
    }

    if ([string]::IsNullOrWhiteSpace($SourceAssetPath)) {
        throw 'No source asset was available for the Assist validation flow.'
    }

    $TraceResult = Invoke-ValidationTool -ToolName 'trace_asset_relationships' -InputObject @{
        asset_path = $SourceAssetPath
    } -StepName 'trace_asset_relationships'

    $Summary.ask.search_assets = [ordered]@{
        source_asset_path = $SourceAssetPath
        returned_count = $SearchResult.returned_count
    }
    $Summary.ask.trace_asset_relationships = [ordered]@{
        asset_path = $SourceAssetPath
        dependency_count = if ($TraceResult.dependencies) { @($TraceResult.dependencies).Count } else { 0 }
        referencer_count = if ($TraceResult.referencers) { @($TraceResult.referencers).Count } else { 0 }
    }

    $RecentAssetsResult = Invoke-ValidationTool -ToolName 'list_recent_assets' -InputObject @{
        folder_path = '/Game'
        max_results = 5
    } -StepName 'list_recent_assets'
    $LargeAssetsResult = Invoke-ValidationTool -ToolName 'find_large_assets' -InputObject @{
        folder_path = '/Game'
        threshold_mb = 1
        max_results = 5
    } -StepName 'find_large_assets'
    $ModifiedAssetsResult = Invoke-ValidationTool -ToolName 'find_assets_modified_since' -InputObject @{
        folder_path = '/Game'
        days_ago = 30
        max_results = 5
    } -StepName 'find_assets_modified_since'
    $EmptyFoldersResult = Invoke-ValidationTool -ToolName 'find_empty_folders' -InputObject @{
        folder_path = '/Game'
        max_results = 5
    } -StepName 'find_empty_folders'
    $VFXContentIndexResult = Invoke-ValidationTool -ToolName 'index_vfx_content_library' -InputObject @{
        folder_path = '/Game/VFX'
        max_assets_per_section = 5
        max_dependency_edges = 25
    } -StepName 'index_vfx_content_library'

    $Summary.ask.daily_content_queries = [ordered]@{
        recent_count = $RecentAssetsResult.returned_count
        large_count = $LargeAssetsResult.returned_count
        modified_count = $ModifiedAssetsResult.returned_count
        empty_folder_count = $EmptyFoldersResult.returned_count
        vfx_index_total_assets = $VFXContentIndexResult.effect_library_summary.total_asset_count
        vfx_index_niagara_systems = $VFXContentIndexResult.effect_library_summary.niagara_system_count
        vfx_index_material_instances = $VFXContentIndexResult.effect_library_summary.material_instance_count
    }

    $FixRedirectorsDryRun = Invoke-ValidationTool -ToolName 'fix_redirectors_in_folder' -InputObject @{
        folder_path = $ScratchRoot
        apply_changes = $false
    } -StepName 'fix_redirectors_in_folder_dry_run'
    $BatchRenameDryRun = Invoke-ValidationTool -ToolName 'batch_rename_assets' -InputObject @{
        asset_paths = @($SourceAssetPath)
        suffix = "_BatchDry_$ScratchStamp"
        apply_changes = $false
    } -StepName 'batch_rename_assets_dry_run'
    $BatchMoveDryRun = Invoke-ValidationTool -ToolName 'batch_move_assets' -InputObject @{
        operations = @(
            @{
                asset_path = $SourceAssetPath
                destination_path = $ScratchMoved
                destination_name = "M_BatchMoveDry_$ScratchStamp"
            }
        )
        apply_changes = $false
    } -StepName 'batch_move_assets_dry_run'

    $BrowseResult = Invoke-ValidationTool -ToolName 'browse_to_asset' -InputObject @{
        asset_path = $SourceAssetPath
    } -StepName 'browse_to_asset'

    $DuplicateDryRun = Invoke-ValidationTool -ToolName 'duplicate_asset' -InputObject @{
        asset_path = $SourceAssetPath
        destination_path = $ScratchRoot
        new_name = $DuplicateName
        apply_changes = $false
    } -StepName 'duplicate_asset_dry_run'
    $DuplicateApply = Invoke-ValidationTool -ToolName 'duplicate_asset' -InputObject @{
        asset_path = $SourceAssetPath
        destination_path = $ScratchRoot
        new_name = $DuplicateName
        apply_changes = $true
    } -StepName 'duplicate_asset_apply'
    $CleanupAssetPaths.Add([string]$DuplicateApply.after_asset_path)

    $RenameDryRun = Invoke-ValidationTool -ToolName 'rename_asset' -InputObject @{
        asset_path = $DuplicateApply.after_asset_path
        new_name = $RenamedName
        apply_changes = $false
    } -StepName 'rename_asset_dry_run'
    $RenameApply = Invoke-ValidationTool -ToolName 'rename_asset' -InputObject @{
        asset_path = $DuplicateApply.after_asset_path
        new_name = $RenamedName
        apply_changes = $true
    } -StepName 'rename_asset_apply'
    [void]$CleanupAssetPaths.Remove([string]$DuplicateApply.after_asset_path)
    $CleanupAssetPaths.Add([string]$RenameApply.after_asset_path)

    $MoveDryRun = Invoke-ValidationTool -ToolName 'move_asset' -InputObject @{
        asset_path = $RenameApply.after_asset_path
        destination_path = $ScratchMoved
        destination_name = $FinalName
        apply_changes = $false
    } -StepName 'move_asset_dry_run'
    $MoveApply = Invoke-ValidationTool -ToolName 'move_asset' -InputObject @{
        asset_path = $RenameApply.after_asset_path
        destination_path = $ScratchMoved
        destination_name = $FinalName
        apply_changes = $true
    } -StepName 'move_asset_apply'
    [void]$CleanupAssetPaths.Remove([string]$RenameApply.after_asset_path)
    $CleanupAssetPaths.Add([string]$MoveApply.after_asset_path)

    $DeleteDryRun = Invoke-ValidationTool -ToolName 'delete_asset' -InputObject @{
        asset_path = $MoveApply.after_asset_path
        apply_changes = $false
    } -StepName 'delete_asset_dry_run'
    $DeleteApply = Invoke-ValidationTool -ToolName 'delete_asset' -InputObject @{
        asset_path = $MoveApply.after_asset_path
        apply_changes = $true
    } -StepName 'delete_asset_apply'
    [void]$CleanupAssetPaths.Remove([string]$MoveApply.after_asset_path)

    $Summary.assist.create_asset_folder = [ordered]@{
        dry_run = [bool]$CreateFolderDryRun.dry_run
        root_created = [bool]$CreateFolderApply.created
        moved_created = [bool]$CreateMovedApply.created
    }
    $Summary.assist.duplicate_asset = [ordered]@{
        dry_run = [bool]$DuplicateDryRun.dry_run
        before_asset_path = [string]$DuplicateApply.before_asset_path
        after_asset_path = [string]$DuplicateApply.after_asset_path
    }
    $Summary.assist.rename_asset = [ordered]@{
        dry_run = [bool]$RenameDryRun.dry_run
        before_asset_path = [string]$RenameApply.before_asset_path
        after_asset_path = [string]$RenameApply.after_asset_path
    }
    $Summary.assist.move_asset = [ordered]@{
        dry_run = [bool]$MoveDryRun.dry_run
        before_asset_path = [string]$MoveApply.before_asset_path
        after_asset_path = [string]$MoveApply.after_asset_path
    }
    $Summary.assist.daily_content_ops = [ordered]@{
        fix_redirectors_dry_run = [bool]$FixRedirectorsDryRun.dry_run
        batch_rename_dry_run = [bool]$BatchRenameDryRun.dry_run
        batch_move_dry_run = [bool]$BatchMoveDryRun.dry_run
    }
    $Summary.assist.open_browse = [ordered]@{
        browse_asset_path = [string]$BrowseResult.asset_path
        open_asset_supported = $false
    }
    $Summary.assist.delete_asset = [ordered]@{
        dry_run = [bool]$DeleteDryRun.dry_run
        asset_path = [string]$DeleteApply.asset_path
    }

    $AuditResult = Invoke-ValidationTool -ToolName 'get_mutation_audit_entries' -InputObject @{
        max_results = 100
        include_details = $true
        asset_path_contains = '/Game/ForgePilot_Test/Assist'
    } -StepName 'get_mutation_audit_entries'

    $AuditToolNames = @(
        @($AuditResult.entries | ForEach-Object {
            if ($_.entry -and $_.entry.tool_name) { [string]$_.entry.tool_name }
        }) | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    )

    foreach ($ExpectedAuditTool in @('create_asset_folder', 'duplicate_asset', 'rename_asset', 'move_asset', 'delete_asset')) {
        if ($AuditToolNames -notcontains $ExpectedAuditTool) {
            throw "Mutation audit did not contain expected tool '$ExpectedAuditTool'."
        }
    }

    $Summary.audit = [ordered]@{
        count = $AuditResult.count
        tool_names = $AuditToolNames
    }

    $Summary.assertions.ask_tools_grounded = ($Summary.ask.search_assets.source_asset_path -like '/Game/*')
    $Summary.assertions.assist_flow_completed = $true
    $Summary.assertions.audit_entries_recorded = $true
    $Summary.assertions.build_tools_marked_advanced = ([string]$ToolMap['run_effect_workflow_v2'].default_exposure -eq 'advanced')

    $Summary.success = -not (@($Summary.assertions.GetEnumerator() | Where-Object { -not $_.Value })).Count
}
finally {
    $CleanupResults = @()
    foreach ($AssetPath in ($CleanupAssetPaths | Select-Object -Unique | Sort-Object -Descending)) {
        if ([string]::IsNullOrWhiteSpace($AssetPath)) {
            continue
        }

        try {
            $DeleteResult = Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName 'delete_asset' -InputObject @{
                asset_path = $AssetPath
                apply_changes = $true
            } -TimeoutSeconds 30

            $DeleteSucceeded = [bool]$DeleteResult.success
            $DeleteMessage = $DeleteResult.message
            if (-not $DeleteSucceeded -and [string]$DeleteMessage -like '*DELETE_ASSET_NOT_FOUND*') {
                $DeleteSucceeded = $true
                $DeleteMessage = 'Asset already absent during cleanup.'
            }

            $CleanupResults += [pscustomobject]@{
                asset_path = $AssetPath
                success = $DeleteSucceeded
                message = $DeleteMessage
            }
        }
        catch {
            $CleanupResults += [pscustomobject]@{
                asset_path = $AssetPath
                success = $false
                message = $_.Exception.Message
            }
        }
    }

    $Summary.cleanup.assets = $CleanupResults
}

$Summary | ConvertTo-Json -Depth 20
