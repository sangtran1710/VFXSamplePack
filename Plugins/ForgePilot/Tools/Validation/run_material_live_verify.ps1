param(
    [string]$BridgeHost = $(if ($env:FORGEPILOT_BRIDGE_HOST) { $env:FORGEPILOT_BRIDGE_HOST } else { "127.0.0.1" }),
    [int]$BridgePort = 0,
    [double]$Timeout = 240.0,
    [string]$SavePath = "/Game/ForgePilot_Audit",
    [string]$TexturePath = "/Engine/EngineResources/DefaultTexture.DefaultTexture",
    [string]$TextureOverridePath = "/Engine/EngineResources/DefaultTexture.DefaultTexture",
    [string]$MaterialFunctionPath = "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture",
    [string]$BlueprintPath = ""
)

$ErrorActionPreference = "Stop"

$BridgeHelper = Join-Path $PSScriptRoot "invoke_bridge.ps1"
if (-not (Test-Path $BridgeHelper)) {
    throw "Bridge helper not found: $BridgeHelper"
}

. $BridgeHelper
if ($BridgePort -le 0) { $BridgePort = Get-ForgePilotBridgePortDefault }

Wait-ForgePilotBridge -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout))

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$MaterialName = "M_MaterialLiveVerify_$Stamp"
$MaterialPath = "$SavePath/$MaterialName"
$MaterialObjectPath = "$MaterialPath.$MaterialName"
$MaterialInstanceName = "MI_MaterialLiveVerify_$Stamp"
$MaterialInstancePath = "$SavePath/$MaterialInstanceName"
$MaterialInstanceObjectPath = "$MaterialInstancePath.$MaterialInstanceName"
$ProbeActorLabel = "ForgePilot_MaterialProbe_$Stamp"
$OutputDir = Join-Path "$env:USERPROFILE\Downloads\ForgePilotMaterialProbes" "material_live_verify_$Stamp"
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$ExpectedTools = @(
    "get_material_editor_context",
    "focus_material_graph_region",
    "focus_material_graph_expression",
    "focus_material_graph_comment",
    "pan_material_graph",
    "set_material_graph_zoom",
    "capture_material_graph_screenshot",
    "validate_material_graph_block",
    "summarize_material_block_wiring",
    "set_material_properties",
    "set_material_texture_param",
    "create_material_parameter_pack",
    "create_material_comment",
    "update_material_comment",
    "layout_material_expressions",
    "create_material_pattern_block",
    "create_material_test_instance_set",
    "sweep_material_instance_parameters",
    "spawn_material_probe_actor",
    "apply_material_to_probe_actor",
    "frame_probe_actor_for_capture",
    "capture_material_visual_probe",
    "compare_material_probe_captures",
    "review_material_graph_simplification",
    "set_material_custom_node",
    "review_material_asset",
    "get_material_function_info",
    "review_material_function",
    "get_material_mutation_snapshots",
    "restore_material_mutation_snapshot",
    "get_material_mutation_audit_entries",
    "destroy_actor_by_label"
)

$Summary = [ordered]@{
    success = $false
    run_id = $Stamp
    material_path = $MaterialPath
    material_object_path = $MaterialObjectPath
    material_instance_path = $MaterialInstancePath
    material_instance_object_path = $MaterialInstanceObjectPath
    probe_actor_label = $ProbeActorLabel
    output_dir = $OutputDir
    tool_presence = @{}
    steps = @{}
    checks = @{}
    cleanup = @{}
}

function Invoke-Tool {
    param(
        [Parameter(Mandatory = $true)][string]$ToolName,
        [hashtable]$InputObject = @{}
    )

    return Invoke-ForgePilotTool -BridgeHost $BridgeHost -BridgePort $BridgePort -ToolName $ToolName -InputObject $InputObject -TimeoutSeconds ([int][Math]::Ceiling($Timeout))
}

function Remove-AssetIfPresent {
    param([string]$AssetPath)

    if ([string]::IsNullOrWhiteSpace($AssetPath)) {
        return $null
    }

    return Invoke-Tool -ToolName "delete_asset" -InputObject @{
        asset_path = $AssetPath
        apply_changes = $true
    }
}

try {
    $LoadedToolNames = @(Get-ForgePilotBridgeToolNames -BridgeHost $BridgeHost -BridgePort $BridgePort -TimeoutSeconds ([int][Math]::Ceiling($Timeout)))
    $MissingTools = @($ExpectedTools | Where-Object { $_ -notin $LoadedToolNames })
    $Summary.tool_presence = @{
        loaded_tool_count = $LoadedToolNames.Count
        expected_tools = $ExpectedTools
        missing_tools = $MissingTools
    }
    if ($MissingTools.Count -gt 0) {
        throw "Missing Material live-verify tools in bridge session: $($MissingTools -join ', ')"
    }

    $Summary.steps.editor_context = Invoke-Tool -ToolName "get_material_editor_context"

    $Summary.steps.create_material = Invoke-Tool -ToolName "create_material" -InputObject @{
        material_name = $MaterialName
        save_path = $SavePath
        base_color = @(0.12, 0.18, 0.26)
    }

    $Summary.steps.set_material_properties = Invoke-Tool -ToolName "set_material_properties" -InputObject @{
        material_path = $MaterialObjectPath
        blend_mode = "Masked"
        shading_model = "Unlit"
        apply_changes = $true
    }

    $Summary.steps.set_texture_param_dry = Invoke-Tool -ToolName "set_material_texture_param" -InputObject @{
        material_path = $MaterialObjectPath
        param_name = "Burn_NoiseTex"
        texture_path = $TexturePath
    }

    $Summary.steps.set_texture_param = Invoke-Tool -ToolName "set_material_texture_param" -InputObject @{
        material_path = $MaterialObjectPath
        param_name = "Burn_NoiseTex"
        texture_path = $TexturePath
        apply_changes = $true
    }

    $Summary.steps.parameter_pack = Invoke-Tool -ToolName "create_material_parameter_pack" -InputObject @{
        material_path = $MaterialObjectPath
        pack_name = "fire"
        prefix = "Burn"
        texture_path = $TexturePath
        create_comment = $true
        apply_changes = $true
    }

    $Summary.steps.pattern_block_mask = Invoke-Tool -ToolName "create_material_pattern_block" -InputObject @{
        material_path = $MaterialObjectPath
        pattern_name = "dissolve_edge_chain"
        prefix = "Burn"
        texture_path = $TexturePath
        material_pin = "OpacityMask"
        apply_changes = $true
    }

    $Summary.steps.pattern_block = Invoke-Tool -ToolName "create_material_pattern_block" -InputObject @{
        material_path = $MaterialObjectPath
        pattern_name = "emissive_multiply_chain"
        prefix = "Glow"
        texture_path = $TexturePath
        material_pin = "EmissiveColor"
        apply_changes = $true
    }

    $Summary.steps.custom_node = Invoke-Tool -ToolName "set_material_custom_node" -InputObject @{
        material_path = $MaterialObjectPath
        description = "AI_CustomGlow"
        code = "return float3(1.0, 0.25, 0.0);"
        output_type = "Float3"
        input_names = @("MaskValue", "Intensity")
        apply_changes = $true
    }

    $Summary.steps.comment = Invoke-Tool -ToolName "create_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Block"
        editor_x = "-900"
        editor_y = "-60"
        width = "1600"
        height = "900"
        color_rgba = @(1.0, 0.92, 0.15, 1.0)
        apply_changes = $true
    }

    $WrappedIndices = @()
    $WrappedIndices += @($Summary.steps.pattern_block_mask.created_expression_indices | ForEach-Object { [int]$_ })
    $WrappedIndices += @($Summary.steps.pattern_block.created_expression_indices | ForEach-Object { [int]$_ })

    $Summary.steps.layout = Invoke-Tool -ToolName "layout_material_expressions" -InputObject @{
        material_path = $MaterialObjectPath
        layout_mode = "horizontal"
        start_x = "-720"
        start_y = "120"
        spacing = "240"
        apply_changes = $true
    }

    $Summary.steps.comment_update = Invoke-Tool -ToolName "update_material_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Block"
        new_text = "AI Authored Fire Block"
        wrap_expression_indices = $WrappedIndices
        padding = "140"
        apply_changes = $true
    }

    $Summary.steps.material_info_after_authoring = Invoke-Tool -ToolName "get_material_info" -InputObject @{
        material_path = $MaterialObjectPath
    }

    $Summary.steps.focus_region = Invoke-Tool -ToolName "focus_material_graph_region" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
        padding = "160"
    }

    $Summary.steps.focus_expression = Invoke-Tool -ToolName "focus_material_graph_expression" -InputObject @{
        material_path = $MaterialObjectPath
        expression_index = [string][int]$Summary.steps.pattern_block.created_expression_indices[0]
        padding = "120"
    }

    $Summary.steps.focus_comment = Invoke-Tool -ToolName "focus_material_graph_comment" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
        padding = "160"
    }

    $Summary.steps.graph_zoom = Invoke-Tool -ToolName "set_material_graph_zoom" -InputObject @{
        material_path = $MaterialObjectPath
        zoom = "1.0"
    }

    $Summary.steps.graph_pan = Invoke-Tool -ToolName "pan_material_graph" -InputObject @{
        material_path = $MaterialObjectPath
        delta_x = "160"
        delta_y = "0"
    }

    $Summary.steps.graph_capture = Invoke-Tool -ToolName "capture_material_graph_screenshot" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
        output_dir = $OutputDir
        file_name = "material_graph.png"
        image_width = "1800"
        image_height = "1000"
        padding = "180"
    }

    $Summary.steps.graph_validate = Invoke-Tool -ToolName "validate_material_graph_block" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
        required_root_pins = @("OpacityMask", "EmissiveColor")
        require_comment = $true
        expect_uv_chain = $true
    }

    $Summary.steps.graph_wiring = Invoke-Tool -ToolName "summarize_material_block_wiring" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
    }

    $Summary.steps.graph_simplification = Invoke-Tool -ToolName "review_material_graph_simplification" -InputObject @{
        material_path = $MaterialObjectPath
        comment_text = "AI Authored Fire Block"
    }

    $Summary.steps.material_review = Invoke-Tool -ToolName "review_material_asset" -InputObject @{
        material_path = $MaterialObjectPath
        creative_mode = "fire"
    }

    $Summary.steps.create_material_instance = Invoke-Tool -ToolName "create_material_instance" -InputObject @{
        instance_name = $MaterialInstanceName
        parent_material_path = $MaterialObjectPath
        save_path = $SavePath
        texture_overrides = ('{"Burn_NoiseTex":"' + $TextureOverridePath + '"}')
    }

    $Summary.steps.material_instance_info = Invoke-Tool -ToolName "get_material_instance_info" -InputObject @{
        material_instance_path = $MaterialInstanceObjectPath
        include_inherited_defaults = $true
        only_overridden = $false
    }

    $Summary.steps.test_instance_set = Invoke-Tool -ToolName "create_material_test_instance_set" -InputObject @{
        parent_material_path = $MaterialObjectPath
        save_path = $SavePath
        name_prefix = "MI_MaterialTest_$Stamp"
        preset = "fire_person_erosion"
    }

    $Summary.steps.material_sweep = Invoke-Tool -ToolName "sweep_material_instance_parameters" -InputObject @{
        parent_material_path = $MaterialObjectPath
        save_path = $SavePath
        name_prefix = "MI_MaterialSweep_$Stamp"
        scalar_sweeps = '{"Burn_Threshold":[0.25,0.55],"Glow_Intensity":[10.0,40.0]}'
    }

    $Summary.steps.spawn_probe_actor = Invoke-Tool -ToolName "spawn_material_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        mesh_path = "/Engine/BasicShapes/Sphere.Sphere"
        location = @(0, 0, 120)
        rotation = @(0, 0, 0)
        scale = @(2.0, 2.0, 2.0)
    }

    $ProbeInstances = @($Summary.steps.test_instance_set.instances)
    if ($ProbeInstances.Count -lt 2) {
        throw "Expected at least two test material instances for probe comparison."
    }

    $LowMIPath = [string]$ProbeInstances[0].asset_path
    $HotMIPath = [string]$ProbeInstances[$ProbeInstances.Count - 1].asset_path

    $Summary.steps.apply_probe_low = Invoke-Tool -ToolName "apply_material_to_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        material_path = $LowMIPath
        material_index = "0"
    }

    $Summary.steps.frame_probe_actor = Invoke-Tool -ToolName "frame_probe_actor_for_capture" -InputObject @{
        actor_label = $ProbeActorLabel
    }

    $Summary.steps.capture_probe_low = Invoke-Tool -ToolName "capture_material_visual_probe" -InputObject @{
        actor_label = $ProbeActorLabel
        output_dir = $OutputDir
        file_name = "material_probe_low.jpg"
        frame_actor = $false
        wait_ms = "250"
    }

    $Summary.steps.apply_probe_hot = Invoke-Tool -ToolName "apply_material_to_probe_actor" -InputObject @{
        actor_label = $ProbeActorLabel
        material_path = $HotMIPath
        material_index = "0"
    }

    $Summary.steps.capture_probe_hot = Invoke-Tool -ToolName "capture_material_visual_probe" -InputObject @{
        actor_label = $ProbeActorLabel
        output_dir = $OutputDir
        file_name = "material_probe_hot.jpg"
        frame_actor = $false
        wait_ms = "250"
    }

    $Summary.steps.compare_probe_captures = Invoke-Tool -ToolName "compare_material_probe_captures" -InputObject @{
        image_a_path = [string]$Summary.steps.capture_probe_low.output_path
        image_b_path = [string]$Summary.steps.capture_probe_hot.output_path
    }

    if (-not [string]::IsNullOrWhiteSpace($BlueprintPath)) {
        $Summary.steps.runtime_trace = Invoke-Tool -ToolName "summarize_blueprint_material_runtime" -InputObject @{
            blueprint_path = $BlueprintPath
            material_instance_path = $MaterialInstanceObjectPath
            max_depth = "2"
        }
    }
    else {
        $Summary.steps.runtime_trace = @{
            skipped = $true
            reason = "No BlueprintPath was provided."
        }
    }

    $Summary.steps.snapshots = Invoke-Tool -ToolName "get_material_mutation_snapshots" -InputObject @{
        material_asset_path = $MaterialObjectPath
    }

    $Summary.steps.audit_entries = Invoke-Tool -ToolName "get_material_mutation_audit_entries" -InputObject @{
        material_asset_path = $MaterialObjectPath
        max_results = "40"
    }

    if (-not [string]::IsNullOrWhiteSpace($MaterialFunctionPath)) {
        $Summary.steps.function_info = Invoke-Tool -ToolName "get_material_function_info" -InputObject @{
            material_function_path = $MaterialFunctionPath
            focus_keyword = "texture"
        }

        $Summary.steps.function_review = Invoke-Tool -ToolName "review_material_function" -InputObject @{
            material_function_path = $MaterialFunctionPath
        }
    }
    else {
        $Summary.steps.function_info = @{
            skipped = $true
            reason = "No MaterialFunctionPath was provided."
        }
        $Summary.steps.function_review = @{
            skipped = $true
            reason = "No MaterialFunctionPath was provided."
        }
    }

    $SnapshotList = @($Summary.steps.snapshots.snapshots)
    if ($SnapshotList.Count -lt 1) {
        throw "Expected at least one Material mutation snapshot after asset mutations."
    }

    $RestoreSnapshotPath = [string]$SnapshotList[$SnapshotList.Count - 1].snapshot_path
    $Summary.steps.restore_dry = Invoke-Tool -ToolName "restore_material_mutation_snapshot" -InputObject @{
        material_asset_path = $MaterialObjectPath
        snapshot_path = $RestoreSnapshotPath
    }

    $Summary.steps.restore_apply = Invoke-Tool -ToolName "restore_material_mutation_snapshot" -InputObject @{
        material_asset_path = $MaterialObjectPath
        snapshot_path = $RestoreSnapshotPath
        apply_changes = $true
    }

    $Summary.steps.material_info_after_restore = Invoke-Tool -ToolName "get_material_info" -InputObject @{
        material_path = $MaterialObjectPath
    }

    $AfterAuthoring = $Summary.steps.material_info_after_authoring
    $AfterRestore = $Summary.steps.material_info_after_restore
    $MIInfo = $Summary.steps.material_instance_info
    $RuntimeTrace = $Summary.steps.runtime_trace
    $GraphValidate = $Summary.steps.graph_validate
    $GraphWiring = $Summary.steps.graph_wiring
    $GraphSimplification = $Summary.steps.graph_simplification
    $GraphCapturePath = [string]$Summary.steps.graph_capture.output_path
    $ProbeLowPath = [string]$Summary.steps.capture_probe_low.output_path
    $ProbeHotPath = [string]$Summary.steps.capture_probe_hot.output_path

    $Summary.checks = @{
        texture_param_created = ([int]$Summary.steps.set_texture_param.editor_x -ne $null)
        pattern_block_node_count = @($Summary.steps.pattern_block.created_expression_indices).Count
        mask_block_node_count = @($Summary.steps.pattern_block_mask.created_expression_indices).Count
        custom_node_created = ([int]$Summary.steps.custom_node.expression_index -ge 0)
        material_expression_count_after_authoring = [int]$AfterAuthoring.expression_count
        material_expression_count_after_restore = [int]$AfterRestore.expression_count
        snapshot_count = [int]$Summary.steps.snapshots.count
        audit_entry_count = [int]$Summary.steps.audit_entries.count
        parameter_pack_created_count = @($Summary.steps.parameter_pack.created_expression_indices).Count
        comment_wrap_applied = [bool]$Summary.steps.comment_update.wrapped_expression_block
        material_instance_texture_override_count = [int]$MIInfo.override_counts.texture
        material_function_node_count = [int]$Summary.steps.function_info.node_count
        material_function_finding_count = [int]$Summary.steps.function_review.finding_count
        material_review_guidance_count = @($Summary.steps.material_review.guidance).Count
        runtime_trace_material_node_count = @($RuntimeTrace.material_parameter_nodes).Count
        runtime_trace_mi_override_count = @($RuntimeTrace.material_instance_overrides).Count
        graph_validation_passed = [bool]$GraphValidate.validation_passed
        graph_validation_error_count = [int]$GraphValidate.error_count
        graph_validation_warning_count = [int]$GraphValidate.warning_count
        graph_capture_exists = (Test-Path $GraphCapturePath)
        graph_internal_link_count = [int]$GraphWiring.internal_link_count
        graph_root_pin_touch_count = @($GraphWiring.root_pins_touched).Count
        graph_simplification_suggestion_count = @($GraphSimplification.suggestions).Count
        test_instance_count = [int]$Summary.steps.test_instance_set.count
        sweep_instance_count = [int]$Summary.steps.material_sweep.count
        probe_capture_low_exists = (Test-Path $ProbeLowPath)
        probe_capture_hot_exists = (Test-Path $ProbeHotPath)
        probe_changed_pixel_ratio = [double]$Summary.steps.compare_probe_captures.changed_pixel_ratio
    }

    if ($Summary.checks.pattern_block_node_count -lt 2) {
        throw "Expected emissive pattern block to create at least two nodes."
    }
    if ($Summary.checks.mask_block_node_count -lt 4) {
        throw "Expected dissolve edge pattern block to create at least four nodes."
    }
    if ($Summary.checks.parameter_pack_created_count -lt 4) {
        throw "Expected create_material_parameter_pack to create several parameter nodes."
    }
    if (-not $Summary.checks.custom_node_created) {
        throw "Custom node creation did not return a valid expression index."
    }
    if (-not $Summary.checks.comment_wrap_applied) {
        throw "Expected update_material_comment to wrap the authored expression block."
    }
    if ($Summary.checks.snapshot_count -lt 1) {
        throw "Expected at least one material mutation snapshot."
    }
    if ($Summary.checks.audit_entry_count -lt 6) {
        throw "Expected multiple material mutation audit entries."
    }
    if ($Summary.checks.material_instance_texture_override_count -lt 1) {
        throw "Expected the Material Instance to report at least one texture override."
    }
    if (-not $Summary.checks.graph_validation_passed -or $Summary.checks.graph_validation_error_count -ne 0) {
        throw "Material graph validator reported blocking errors."
    }
    if (-not $Summary.checks.graph_capture_exists) {
        throw "Expected graph screenshot to be written to Downloads."
    }
    if ($Summary.checks.graph_internal_link_count -lt 3) {
        throw "Expected graph wiring summary to find several internal links."
    }
    if ($Summary.checks.graph_root_pin_touch_count -lt 2) {
        throw "Expected focused graph block to touch at least two material root pins."
    }
    if ($Summary.checks.test_instance_count -lt 3) {
        throw "Expected at least three preset test instances."
    }
    if ($Summary.checks.sweep_instance_count -lt 2) {
        throw "Expected at least two sweep instances."
    }
    if (-not $Summary.checks.probe_capture_low_exists -or -not $Summary.checks.probe_capture_hot_exists) {
        throw "Expected material probe captures to exist on disk."
    }
    if ($Summary.checks.probe_changed_pixel_ratio -le 0.0001) {
        throw "Expected probe captures to differ after MI tuning."
    }
    if ($Summary.checks.material_expression_count_after_restore -ge $Summary.checks.material_expression_count_after_authoring) {
        throw "Expected material restore to reduce the authored expression count."
    }
    if ($Summary.checks.material_function_node_count -lt 1) {
        throw "Material Function inspection returned no nodes."
    }
    if ($Summary.checks.material_review_guidance_count -lt 1) {
        throw "Expected review_material_asset to return creative guidance."
    }

    $Summary.success = $true
}
catch {
    $Summary.error = $_.Exception.Message
}
finally {
    try {
        $Summary.cleanup.destroy_probe_actor = Invoke-Tool -ToolName "destroy_actor_by_label" -InputObject @{
            actor_label = $ProbeActorLabel
        }
    }
    catch {
        $Summary.cleanup.destroy_probe_actor_error = $_.Exception.Message
    }

    try {
        $Summary.cleanup.delete_material_instance = Remove-AssetIfPresent -AssetPath $MaterialInstanceObjectPath
    }
    catch {
        $Summary.cleanup.delete_material_instance_error = $_.Exception.Message
    }

    foreach ($InstanceCollection in @($Summary.steps.test_instance_set.instances, $Summary.steps.material_sweep.instances)) {
        foreach ($Instance in @($InstanceCollection)) {
            try {
                if ($Instance -and $Instance.asset_path) {
                    Remove-AssetIfPresent -AssetPath ([string]$Instance.asset_path) | Out-Null
                }
            }
            catch {
            }
        }
    }

    try {
        $Summary.cleanup.delete_material = Remove-AssetIfPresent -AssetPath $MaterialObjectPath
    }
    catch {
        $Summary.cleanup.delete_material_error = $_.Exception.Message
    }

    $SummaryPath = Join-Path $OutputDir "summary.json"
    $Summary | ConvertTo-Json -Depth 100 | Set-Content -Path $SummaryPath -Encoding UTF8
    [ordered]@{
        success = $Summary.success
        summary_path = $SummaryPath
        run_id = $Stamp
    } | ConvertTo-Json -Depth 10
}


