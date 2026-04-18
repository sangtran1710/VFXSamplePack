# Tool Surface

This guide summarizes the current ForgePilot product surface in 3 user-facing groups:

- `Ask`
- `Assist`
- `Advanced Build`

These groupings reflect the current tool metadata returned by `list_tools` and MCP `tools/list`.

## Ask

Use these by default when you want to inspect, trace, review, explain, or ground yourself in the current project state.

| Scope | Tools | Notes |
|---|---|---|
| Project and content context | `find_assets_modified_since`, `find_empty_folders`, `find_large_assets`, `get_context`, `get_scene_actors`, `get_selected_actor_details`, `get_selected_assets`, `index_vfx_content_library`, `list_recent_assets`, `scan_assets`, `search_assets`, `trace_actor_relationships`, `trace_asset_relationships` | Default project-grounding entrypoints plus daily content hygiene/library queries. |
| Knowledge and project lookup | `load_vfx_recipe`, `get_mutation_audit_entries`, `inspect_skeletal_attachment_context` | Use after editor-state grounding when needed. |
| Blueprint inspection | `find_blueprint_asset_usage`, `get_blueprint_class_defaults`, `get_blueprint_component_details`, `get_blueprint_components`, `get_blueprint_graph_layout`, `get_blueprint_inventory`, `get_blueprint_mutation_snapshots`, `get_blueprint_node_details`, `get_blueprint_summary`, `get_blueprint_variables`, `get_selected_blueprint_node_details`, `get_selected_blueprint_path`, `inspect_blueprint_array_defaults`, `review_blueprint_asset`, `search_blueprint_nodes`, `summarize_blueprint_material_runtime`, `summarize_blueprint_value_flow`, `trace_blueprint_exec_chain` | Read-only Blueprint understanding and review tools. |
| Material and texture inspection | `get_material_editor_context`, `get_material_function_info`, `get_material_info`, `get_material_instance_info`, `get_material_mutation_audit_entries`, `get_material_mutation_snapshots`, `get_selected_material_expression_details`, `get_selected_texture_info`, `get_texture_info`, `get_texture_material_inspection`, `review_material_asset`, `review_material_function`, `review_material_graph_hygiene`, `review_material_graph_simplification`, `summarize_material_block_wiring` | Read-only graph inspection and review. |
| Niagara inspection | `get_niagara_actor_user_parameter_values`, `get_niagara_editor_context`, `get_niagara_emitter_details`, `get_niagara_emitter_stack`, `get_niagara_module_inputs`, `get_niagara_mutation_snapshots`, `get_niagara_renderer_details`, `get_niagara_system_info`, `get_niagara_user_parameter_defaults`, `review_niagara_emitter`, `review_niagara_renderer`, `review_niagara_system` | Read-only Niagara understanding and review. |
| Animation inspection | `get_animation_asset_info`, `get_animation_editor_context`, `review_animation_asset` | Animation-side inspection and review. |

## Assist

Use these for safe, deterministic content-browser operations. Default to dry-run or non-destructive focus behavior where possible.

| Scope | Tools | Notes |
|---|---|---|
| Safe defaults | `batch_move_assets`, `batch_rename_assets`, `browse_to_asset`, `create_asset_folder`, `duplicate_asset`, `fix_redirectors_in_folder`, `move_asset`, `rename_asset` | Default Assist path for day-to-day organization work. Mutating tools dry-run by default. |
| Advanced exceptions | `delete_asset`, `import_asset_from_file`, `open_asset` | Exposed but not safe-default. `open_asset` is intentionally not part of the normal bridge flow in this editor environment. |

## Advanced Build

Use these only when the user explicitly wants authoring, execution, runtime probes, or workflow automation.

| Scope | Tools | Notes |
|---|---|---|
| Blueprint authoring | `add_component_to_blueprint`, `add_variable_to_blueprint`, `attach_component_to_component_socket`, `compile_blueprint_with_feedback`, `connect_blueprint_pins`, `create_blueprint`, `create_blueprint_call_function_node`, `create_blueprint_variable_get_node`, `create_blueprint_variable_set_node`, `create_custom_event_node`, `delete_blueprint_node`, `edit_component_property`, `restore_blueprint_mutation_snapshot`, `set_blueprint_node_pin_default` | Mutates Blueprint structure or graph authoring state. |
| Material authoring | `add_material_expression`, `apply_material_to_probe_actor`, `cleanup_material_graph`, `compile_material_and_get_diagnostics`, `connect_material_nodes`, `create_material`, `create_material_comment`, `create_material_instance`, `create_material_parameter_pack`, `create_material_pattern_block`, `create_material_test_instance_set`, `delete_material_comments`, `focus_material_graph_comment`, `focus_material_graph_expression`, `focus_material_graph_region`, `frame_probe_actor_for_capture`, `layout_material_expressions`, `pan_material_graph`, `restore_material_mutation_snapshot`, `run_suction_core_material_op`, `run_v2_material_op`, `set_actor_material`, `set_material_custom_node`, `set_material_graph_zoom`, `set_material_properties`, `set_material_scalar_param`, `set_material_texture_param`, `set_material_vector_param`, `spawn_material_probe_actor`, `sweep_material_instance_parameters`, `update_material_comment`, `validate_material_graph_block` | Includes graph authoring plus probe-oriented build helpers. Default material cleanup is layout-only; comment boxes are opt-in. |
| Niagara authoring | `add_empty_niagara_emitter`, `add_niagara_emitter_to_system`, `add_niagara_module_to_stage`, `add_niagara_user_parameter`, `cleanup_niagara_audit_assets`, `cleanup_niagara_authoring_residue`, `compile_niagara_system`, `create_empty_niagara_system`, `create_niagara_system`, `duplicate_niagara_system`, `move_niagara_module_in_stage`, `remove_niagara_emitters_from_system`, `remove_niagara_module_from_stage`, `reset_niagara_module_input`, `restore_niagara_mutation_snapshot`, `run_suction_core_niagara_op`, `run_v2_niagara_op`, `set_niagara_color_over_life`, `set_niagara_cone_velocity`, `set_niagara_curl_noise_force`, `set_niagara_drag`, `set_niagara_emitter_enabled`, `set_niagara_emitter_fixed_bounds`, `set_niagara_gravity_force`, `set_niagara_lifetime`, `set_niagara_local_space`, `set_niagara_module_input_constant`, `set_niagara_module_input_curve`, `set_niagara_module_input_dynamic`, `set_niagara_module_input_linked`, `set_niagara_opacity_over_life`, `set_niagara_renderer_enabled`, `set_niagara_renderer_material`, `set_niagara_renderer_material_user_param`, `set_niagara_ribbon_deterministic_streams`, `set_niagara_spawn_rate`, `set_niagara_sprite_renderer_subuv`, `set_niagara_sprite_size`, `set_niagara_system_fixed_bounds`, `set_niagara_user_parameter`, `set_niagara_user_parameter_default`, `set_niagara_velocity`, `spawn_niagara_at_location` | Mutates Niagara systems, emitters, stacks, renderers, and user params. |
| Runtime and scene execution | `destroy_actor_by_label`, `run_pie_probe_scenario`, `send_pie_key_input`, `spawn_actor`, `start_play_in_viewport`, `stop_play_session` | Explicit runtime or scene-execution paths. |
| Workflow automation | `generate_visual_contract_data_asset`, `resolve_effect_plan`, `resolve_effect_plan_v2`, `run_effect_workflow_v2`, `run_suction_core_v2_workflow` | Planner and workflow-level automation. |
| Advanced read-only utilities | `capture_asset_thumbnail`, `capture_material_graph_screenshot`, `capture_material_visual_probe`, `capture_viewport_screenshot`, `compare_material_probe_captures`, `find_niagara_template_emitters`, `get_pie_session_state`, `review_effect_failure_taxonomy_v2`, `review_suction_core_failure_taxonomy`, `verify_viewport_capture_state` | Read-only, but still advanced because they belong to workflow, probe, or critic flows rather than everyday Ask usage. |
