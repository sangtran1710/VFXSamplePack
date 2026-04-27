// Copyright 2026, HenryFX. ForgePilot Plugin.
// Core Registry: Constructor, RegisterTool, ExecuteTool, GetToolDefinitionsJson, RegisterDefaultTools
// Tool registrations and implementations are in per-phase files:
//   VFXToolRegistry_Scene.cpp     - Phase 1: Scene tools
//   VFXToolRegistry_Blueprint.cpp - Phase 2: Blueprint tools
//   VFXToolRegistry_Material.cpp  - Phase 3 + 3.5: Material tools
//   VFXToolRegistry_Niagara.cpp   - Phase 4: Niagara VFX tools
//   VFXToolRegistry_Animation.cpp - Phase 5: Animation-adjacent inspection tools

#include "VFXToolRegistry.h"
#include "ForgePilotCache.h"
#include "ForgePilotLogging.h"
#include "ForgePilotMetrics.h"
#include "ForgePilotSettings.h"
#include "VFXToolRegistryHelpers.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"

namespace
{
static bool IsAssistToolName(const FString& ToolName);

static FString BuildRegistryErrorJson(
	const FString& ErrorCode,
	const FString& ErrorMessage,
	const FString& ToolName = TEXT(""))
{
	TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
	ErrorObj->SetBoolField(TEXT("success"), false);
	ErrorObj->SetStringField(TEXT("error_code"), ErrorCode);
	ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
	if (!ToolName.IsEmpty())
	{
		TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
		DebugContext->SetStringField(TEXT("tool_name"), ToolName);
		ErrorObj->SetObjectField(TEXT("debug_context"), DebugContext);
	}
	return JsonObjToString(ErrorObj);
}

static bool TryGetBoolLikeField(
	const TSharedPtr<FJsonObject>& Obj,
	const FString& FieldName,
	bool& OutValue)
{
	if (!Obj.IsValid())
	{
		return false;
	}

	const TSharedPtr<FJsonValue> Value = Obj->TryGetField(FieldName);
	if (!Value.IsValid() || Value->Type == EJson::Null || Value->Type == EJson::None)
	{
		return false;
	}

	if (Value->Type == EJson::Boolean)
	{
		OutValue = Value->AsBool();
		return true;
	}

	if (Value->Type == EJson::String)
	{
		const FString Raw = Value->AsString().TrimStartAndEnd();
		if (Raw.Equals(TEXT("true"), ESearchCase::IgnoreCase) ||
			Raw.Equals(TEXT("1"), ESearchCase::IgnoreCase) ||
			Raw.Equals(TEXT("yes"), ESearchCase::IgnoreCase))
		{
			OutValue = true;
			return true;
		}
		if (Raw.Equals(TEXT("false"), ESearchCase::IgnoreCase) ||
			Raw.Equals(TEXT("0"), ESearchCase::IgnoreCase) ||
			Raw.Equals(TEXT("no"), ESearchCase::IgnoreCase))
		{
			OutValue = false;
			return true;
		}
	}

	if (Value->Type == EJson::Number)
	{
		OutValue = Value->AsNumber() != 0.0;
		return true;
	}

	return false;
}

static FString InferToolCategory(const FString& ToolName)
{
	if (ToolName.StartsWith(TEXT("get_blueprint")) ||
		ToolName.StartsWith(TEXT("search_blueprint")) ||
		ToolName.StartsWith(TEXT("find_blueprint")) ||
		ToolName.StartsWith(TEXT("trace_blueprint")) ||
		ToolName.StartsWith(TEXT("inspect_blueprint")) ||
		ToolName.StartsWith(TEXT("summarize_blueprint")) ||
		ToolName.StartsWith(TEXT("review_blueprint")) ||
		ToolName.StartsWith(TEXT("restore_blueprint")) ||
		ToolName == TEXT("create_blueprint") ||
		ToolName == TEXT("add_variable") ||
		ToolName == TEXT("add_component") ||
		ToolName == TEXT("edit_component_property"))
	{
		return TEXT("blueprint");
	}

	if (ToolName.StartsWith(TEXT("get_material")) ||
		ToolName.StartsWith(TEXT("get_texture")) ||
		ToolName.StartsWith(TEXT("get_selected_material")) ||
		ToolName.StartsWith(TEXT("get_selected_texture")) ||
		ToolName.StartsWith(TEXT("run_v2_material")) ||
		ToolName.StartsWith(TEXT("run_suction_core_material")) ||
		ToolName.StartsWith(TEXT("set_material")) ||
		ToolName.StartsWith(TEXT("connect_material")) ||
		ToolName.StartsWith(TEXT("add_material")) ||
		ToolName.StartsWith(TEXT("focus_material")) ||
		ToolName.StartsWith(TEXT("pan_material")) ||
		ToolName.StartsWith(TEXT("validate_material")) ||
		ToolName.StartsWith(TEXT("summarize_material")) ||
		ToolName.StartsWith(TEXT("review_material")) ||
		ToolName.StartsWith(TEXT("capture_material")) ||
		ToolName.StartsWith(TEXT("compare_material")) ||
		ToolName == TEXT("frame_probe_actor_for_capture") ||
		ToolName == TEXT("create_material_comment") ||
		ToolName == TEXT("update_material_comment") ||
		ToolName == TEXT("delete_material_comments") ||
		ToolName == TEXT("layout_material_expressions") ||
		ToolName == TEXT("cleanup_material_graph") ||
		ToolName == TEXT("create_material") ||
		ToolName == TEXT("create_material_instance"))
	{
		return TEXT("material");
	}

	if (ToolName.StartsWith(TEXT("get_niagara")) ||
		ToolName.StartsWith(TEXT("find_niagara")) ||
		ToolName.StartsWith(TEXT("run_v2_niagara")) ||
		ToolName.StartsWith(TEXT("run_suction_core_niagara")) ||
		ToolName.StartsWith(TEXT("set_niagara")) ||
		ToolName.StartsWith(TEXT("add_niagara")) ||
		ToolName.StartsWith(TEXT("remove_niagara")) ||
		ToolName.StartsWith(TEXT("move_niagara")) ||
		ToolName.StartsWith(TEXT("reset_niagara")) ||
		ToolName.StartsWith(TEXT("compile_niagara")) ||
		ToolName.StartsWith(TEXT("review_niagara")) ||
		ToolName == TEXT("create_niagara_system") ||
		ToolName == TEXT("create_empty_niagara_system") ||
		ToolName == TEXT("add_empty_niagara_emitter") ||
		ToolName == TEXT("duplicate_niagara_system") ||
		ToolName == TEXT("spawn_niagara_at_location"))
	{
		return TEXT("niagara");
	}

	if (ToolName.StartsWith(TEXT("get_animation")) ||
		ToolName.StartsWith(TEXT("review_animation")))
	{
		return TEXT("animation");
	}

	if (ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("review_effect_failure_taxonomy_v2") ||
		ToolName == TEXT("review_suction_core_failure_taxonomy") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow"))
	{
		return TEXT("scene");
	}

	return TEXT("scene");
}

static FString InferToolSurfaceArea(const FString& ToolName)
{
	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("material"))
	{
		return TEXT("material_authoring");
	}
	if (Category == TEXT("niagara"))
	{
		return TEXT("niagara_systems");
	}
	if (Category == TEXT("blueprint"))
	{
		return TEXT("blueprint_gameplay");
	}
	if (Category == TEXT("animation"))
	{
		return TEXT("animation_inspection");
	}
	if (IsAssistToolName(ToolName))
	{
		return TEXT("asset_operations");
	}
	if (ToolName == TEXT("start_play_in_viewport") ||
		ToolName == TEXT("stop_play_session") ||
		ToolName == TEXT("get_pie_session_state") ||
		ToolName == TEXT("send_pie_key_input") ||
		ToolName == TEXT("run_pie_probe_scenario") ||
		ToolName == TEXT("verify_viewport_capture_state") ||
		ToolName == TEXT("capture_viewport_screenshot"))
	{
		return TEXT("runtime_preview");
	}
	if (ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow"))
	{
		return TEXT("effect_orchestration");
	}
	return TEXT("scene_world");
}

static bool IsRuntimePreviewToolName(const FString& ToolName)
{
	return ToolName == TEXT("start_play_in_viewport") ||
		ToolName == TEXT("stop_play_session") ||
		ToolName == TEXT("get_pie_session_state") ||
		ToolName == TEXT("send_pie_key_input") ||
		ToolName == TEXT("run_pie_probe_scenario") ||
		ToolName == TEXT("verify_viewport_capture_state") ||
		ToolName == TEXT("capture_viewport_screenshot") ||
		ToolName == TEXT("capture_asset_thumbnail") ||
		ToolName == TEXT("capture_material_graph_screenshot") ||
		ToolName == TEXT("capture_material_visual_probe") ||
		ToolName == TEXT("compare_material_probe_captures");
}

static bool IsVisualEffectWorkflowToolName(const FString& ToolName)
{
	return ToolName == TEXT("load_vfx_recipe") ||
		ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("generate_visual_contract_data_asset") ||
		ToolName == TEXT("review_effect_failure_taxonomy_v2") ||
		ToolName == TEXT("review_suction_core_failure_taxonomy") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow");
}

static FString InferUnrealDomain(const FString& ToolName)
{
	if (IsVisualEffectWorkflowToolName(ToolName))
	{
		return TEXT("visual_effect");
	}
	if (IsRuntimePreviewToolName(ToolName))
	{
		return TEXT("runtime_preview");
	}
	if (IsAssistToolName(ToolName) ||
		ToolName == TEXT("scan_assets") ||
		ToolName == TEXT("search_assets") ||
		ToolName == TEXT("trace_asset_relationships") ||
		ToolName == TEXT("find_empty_folders") ||
		ToolName == TEXT("list_recent_assets") ||
		ToolName == TEXT("find_large_assets") ||
		ToolName == TEXT("find_assets_modified_since"))
	{
		return TEXT("content_browser");
	}

	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("blueprint") ||
		Category == TEXT("material") ||
		Category == TEXT("niagara") ||
		Category == TEXT("animation"))
	{
		return Category;
	}

	return TEXT("scene");
}

static FString InferWorkflowLane(const FString& ToolName)
{
	if (IsVisualEffectWorkflowToolName(ToolName))
	{
		return TEXT("vfx_orchestration");
	}
	if (IsRuntimePreviewToolName(ToolName))
	{
		return TEXT("runtime_preview");
	}
	if (IsAssistToolName(ToolName) ||
		InferUnrealDomain(ToolName) == TEXT("content_browser"))
	{
		return TEXT("content_ops");
	}

	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("material"))
	{
		return TEXT("material_authoring");
	}
	if (Category == TEXT("niagara"))
	{
		return TEXT("niagara_authoring");
	}
	if (Category == TEXT("blueprint"))
	{
		return TEXT("blueprint_gameplay");
	}
	if (Category == TEXT("animation"))
	{
		return TEXT("animation_inspection");
	}

	return TEXT("scene_world");
}

static FString InferToolGroup(const FString& ToolName)
{
	if (IsVisualEffectWorkflowToolName(ToolName) ||
		IsRuntimePreviewToolName(ToolName) ||
		IsAssistToolName(ToolName) ||
		InferUnrealDomain(ToolName) == TEXT("content_browser"))
	{
		return TEXT("General");
	}

	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("blueprint"))
	{
		return TEXT("Blueprint");
	}
	if (Category == TEXT("material"))
	{
		return TEXT("Material");
	}
	if (Category == TEXT("niagara"))
	{
		return TEXT("Niagara");
	}
	if (Category == TEXT("animation"))
	{
		return TEXT("Animation");
	}

	return TEXT("Scene");
}

static FString InferPrimaryLane(const FString& ToolName)
{
	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("material"))
	{
		return TEXT("material");
	}
	if (Category == TEXT("niagara"))
	{
		return TEXT("niagara");
	}
	if (Category == TEXT("blueprint"))
	{
		return TEXT("gameplay");
	}
	if (Category == TEXT("animation"))
	{
		return TEXT("animation");
	}
	return TEXT("general_unreal");
}

static FString InferVfxAffinity(const FString& ToolName)
{
	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("material") || Category == TEXT("niagara"))
	{
		return TEXT("primary");
	}
	if (ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow") ||
		ToolName == TEXT("load_vfx_recipe") ||
		ToolName == TEXT("generate_visual_contract_data_asset"))
	{
		return TEXT("primary");
	}
	if (Category == TEXT("scene"))
	{
		return TEXT("supporting");
	}
	return TEXT("adjacent");
}

static TArray<FString> InferCapabilityTags(const FString& ToolName)
{
	TArray<FString> Tags;
	Tags.Add(TEXT("general_unreal"));

	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("material"))
	{
		Tags.Add(TEXT("material"));
		Tags.Add(TEXT("shader_graph"));
		Tags.Add(TEXT("vfx_primary"));
	}
	else if (Category == TEXT("niagara"))
	{
		Tags.Add(TEXT("niagara"));
		Tags.Add(TEXT("vfx_primary"));
	}
	else if (Category == TEXT("blueprint"))
	{
		Tags.Add(TEXT("gameplay"));
		Tags.Add(TEXT("blueprint"));
	}
	else if (Category == TEXT("animation"))
	{
		Tags.Add(TEXT("animation"));
		Tags.Add(TEXT("inspection"));
	}
	else
	{
		Tags.Add(TEXT("scene"));
		if (IsAssistToolName(ToolName))
		{
			Tags.Add(TEXT("asset_management"));
			Tags.Add(TEXT("content_browser"));
		}
		if (ToolName == TEXT("start_play_in_viewport") ||
			ToolName == TEXT("stop_play_session") ||
			ToolName == TEXT("get_pie_session_state") ||
			ToolName == TEXT("send_pie_key_input") ||
			ToolName == TEXT("run_pie_probe_scenario"))
		{
			Tags.Add(TEXT("runtime_preview"));
			Tags.Add(TEXT("playtest"));
		}
	}

	if (ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow"))
	{
		Tags.Add(TEXT("workflow"));
		Tags.Add(TEXT("effect_orchestration"));
	}

	TArray<FString> UniqueTags;
	for (const FString& Tag : Tags)
	{
		UniqueTags.AddUnique(Tag);
	}
	UniqueTags.Sort();
	return UniqueTags;
}

static bool IsAssistToolName(const FString& ToolName)
{
	return ToolName == TEXT("create_asset_folder") ||
		ToolName == TEXT("rename_asset") ||
		ToolName == TEXT("move_asset") ||
		ToolName == TEXT("duplicate_asset") ||
		ToolName == TEXT("fix_redirectors_in_folder") ||
		ToolName == TEXT("batch_rename_assets") ||
		ToolName == TEXT("batch_move_assets") ||
		ToolName == TEXT("delete_asset") ||
		ToolName == TEXT("import_asset_from_file") ||
		ToolName == TEXT("open_asset") ||
		ToolName == TEXT("browse_to_asset");
}

static bool HasAskPrefix(const FString& ToolName)
{
	return ToolName == TEXT("load_vfx_recipe") ||
		ToolName == TEXT("list_recent_assets") ||
		ToolName.StartsWith(TEXT("get_")) ||
		ToolName.StartsWith(TEXT("search_")) ||
		ToolName.StartsWith(TEXT("find_")) ||
		ToolName.StartsWith(TEXT("index_")) ||
		ToolName.StartsWith(TEXT("list_")) ||
		ToolName.StartsWith(TEXT("trace_")) ||
		ToolName.StartsWith(TEXT("inspect_")) ||
		ToolName.StartsWith(TEXT("summarize_")) ||
		ToolName.StartsWith(TEXT("review_")) ||
		ToolName.StartsWith(TEXT("scan_"));
}

static bool IsAdvancedReadOnlyBuildToolName(const FString& ToolName)
{
	return ToolName == TEXT("find_niagara_template_emitters") ||
		ToolName == TEXT("get_pie_session_state") ||
		ToolName == TEXT("review_effect_failure_taxonomy_v2") ||
		ToolName == TEXT("review_suction_core_failure_taxonomy") ||
		ToolName == TEXT("verify_viewport_capture_state") ||
		ToolName == TEXT("capture_asset_thumbnail") ||
		ToolName == TEXT("capture_material_graph_screenshot") ||
		ToolName == TEXT("capture_material_visual_probe") ||
		ToolName == TEXT("capture_viewport_screenshot") ||
		ToolName == TEXT("compare_material_probe_captures");
}

static bool IsBuildDespiteAskLikeName(const FString& ToolName)
{
	return IsAdvancedReadOnlyBuildToolName(ToolName);
}

static bool IsBuildToolName(const FString& ToolName)
{
	if (IsAdvancedReadOnlyBuildToolName(ToolName) ||
		ToolName == TEXT("spawn_actor") ||
		ToolName == TEXT("destroy_actor_by_label") ||
		ToolName == TEXT("start_play_in_viewport") ||
		ToolName == TEXT("stop_play_session") ||
		ToolName == TEXT("send_pie_key_input") ||
		ToolName == TEXT("run_pie_probe_scenario") ||
		ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("generate_visual_contract_data_asset") ||
		ToolName == TEXT("run_effect_workflow_v2") ||
		ToolName == TEXT("run_suction_core_v2_workflow"))
	{
		return true;
	}

	const FString Category = InferToolCategory(ToolName);
	if (Category == TEXT("blueprint"))
	{
		return ToolName == TEXT("create_blueprint") ||
			ToolName == TEXT("add_variable") ||
			ToolName == TEXT("add_component") ||
			ToolName == TEXT("edit_component_property") ||
			ToolName == TEXT("add_variable_to_blueprint") ||
			ToolName == TEXT("add_component_to_blueprint") ||
			ToolName == TEXT("attach_component_to_component_socket") ||
			ToolName == TEXT("create_custom_event_node") ||
			ToolName == TEXT("create_blueprint_call_function_node") ||
			ToolName == TEXT("create_blueprint_variable_get_node") ||
			ToolName == TEXT("create_blueprint_variable_set_node") ||
			ToolName == TEXT("delete_blueprint_node") ||
			ToolName == TEXT("connect_blueprint_pins") ||
			ToolName == TEXT("set_blueprint_node_pin_default") ||
			ToolName == TEXT("compile_blueprint_with_feedback") ||
			ToolName == TEXT("restore_blueprint_mutation_snapshot");
	}

	if (Category == TEXT("material"))
	{
		return ToolName.StartsWith(TEXT("run_v2_material")) ||
			ToolName.StartsWith(TEXT("run_suction_core_material")) ||
			ToolName.StartsWith(TEXT("set_material")) ||
			ToolName.StartsWith(TEXT("connect_material")) ||
			ToolName.StartsWith(TEXT("add_material")) ||
			ToolName.StartsWith(TEXT("focus_material")) ||
			ToolName.StartsWith(TEXT("pan_material")) ||
			ToolName.StartsWith(TEXT("validate_material")) ||
			ToolName.StartsWith(TEXT("capture_material")) ||
			ToolName.StartsWith(TEXT("compare_material")) ||
			ToolName == TEXT("frame_probe_actor_for_capture") ||
			ToolName == TEXT("create_material_comment") ||
			ToolName == TEXT("update_material_comment") ||
			ToolName == TEXT("delete_material_comments") ||
			ToolName == TEXT("layout_material_expressions") ||
			ToolName == TEXT("cleanup_material_graph") ||
			ToolName == TEXT("create_material") ||
			ToolName == TEXT("create_material_instance") ||
			ToolName == TEXT("create_material_parameter_pack") ||
			ToolName == TEXT("create_material_pattern_block") ||
			ToolName == TEXT("create_material_test_instance_set") ||
			ToolName == TEXT("spawn_material_probe_actor") ||
			ToolName == TEXT("apply_material_to_probe_actor") ||
			ToolName == TEXT("sweep_material_instance_parameters") ||
			ToolName == TEXT("restore_material_mutation_snapshot");
	}

	if (Category == TEXT("niagara"))
	{
		return ToolName == TEXT("create_niagara_system") ||
			ToolName == TEXT("create_empty_niagara_system") ||
			ToolName == TEXT("add_empty_niagara_emitter") ||
			ToolName == TEXT("duplicate_niagara_system") ||
			ToolName == TEXT("spawn_niagara_at_location") ||
			ToolName.StartsWith(TEXT("run_v2_niagara")) ||
			ToolName.StartsWith(TEXT("run_suction_core_niagara")) ||
			ToolName.StartsWith(TEXT("set_niagara")) ||
			ToolName.StartsWith(TEXT("add_niagara")) ||
			ToolName.StartsWith(TEXT("remove_niagara")) ||
			ToolName.StartsWith(TEXT("move_niagara")) ||
			ToolName.StartsWith(TEXT("reset_niagara")) ||
			ToolName.StartsWith(TEXT("compile_niagara")) ||
			ToolName == TEXT("restore_niagara_mutation_snapshot");
	}

	return false;
}

static FString InferToolMode(const FString& ToolName)
{
	if (IsAssistToolName(ToolName))
	{
		return TEXT("assist");
	}

	if (HasAskPrefix(ToolName) && !IsBuildDespiteAskLikeName(ToolName))
	{
		return TEXT("ask");
	}

	if (IsBuildToolName(ToolName))
	{
		return TEXT("build");
	}

	return TEXT("build");
}

static FString InferMutationLevel(const FString& ToolName, const FString& Mode)
{
	if (Mode == TEXT("ask") || IsAdvancedReadOnlyBuildToolName(ToolName))
	{
		return TEXT("read_only");
	}

	if (Mode == TEXT("assist"))
	{
		if (ToolName == TEXT("open_asset") || ToolName == TEXT("browse_to_asset"))
		{
			return TEXT("read_only");
		}

		return TEXT("safe_content_op");
	}

	if (Mode == TEXT("build"))
	{
		return TEXT("advanced_authoring");
	}

	return TEXT("advanced_authoring");
}

static bool InferSafeDefault(const FString& ToolName, const FString& Mode)
{
	if (Mode == TEXT("ask"))
	{
		return true;
	}

	if (Mode == TEXT("assist"))
	{
		return ToolName != TEXT("delete_asset") &&
			ToolName != TEXT("import_asset_from_file") &&
			ToolName != TEXT("open_asset");
	}

	return false;
}

static bool InferProjectGrounded(const FString& ToolName)
{
	if (ToolName == TEXT("load_vfx_recipe") ||
		ToolName == TEXT("resolve_effect_plan") ||
		ToolName == TEXT("resolve_effect_plan_v2") ||
		ToolName == TEXT("generate_visual_contract_data_asset"))
	{
		return false;
	}

	return true;
}

static FString InferDefaultExposure(const FString& ToolName, const FString& Mode)
{
	if (Mode == TEXT("build"))
	{
		return TEXT("advanced");
	}

	if (ToolName == TEXT("delete_asset") ||
		ToolName == TEXT("import_asset_from_file") ||
		ToolName == TEXT("open_asset"))
	{
		return TEXT("advanced");
	}

	return TEXT("default");
}

static FString BuildToolCacheKey(const FString& ToolName, const TSharedPtr<FJsonObject>& Input)
{
	const FString InputJson = JsonObjToString(Input.IsValid() ? Input : MakeShareable(new FJsonObject()));
	const uint32 Hash = GetTypeHash(ToolName + TEXT("|") + InputJson);
	return FString::Printf(TEXT("tool_%u"), Hash);
}
}

// ============================================================
// Constructor + Core Registry
// ============================================================

FVFXToolRegistry::FVFXToolRegistry()
{
}

void FVFXToolRegistry::RegisterTool(const FVFXToolDefinition& ToolDef)
{
	FVFXToolDefinition FinalDef = ToolDef;
	if (FinalDef.ToolGroup.IsEmpty())
	{
		FinalDef.ToolGroup = InferToolGroup(FinalDef.Name);
	}
	if (FinalDef.UnrealDomain.IsEmpty())
	{
		FinalDef.UnrealDomain = InferUnrealDomain(FinalDef.Name);
	}
	if (FinalDef.WorkflowLane.IsEmpty())
	{
		FinalDef.WorkflowLane = InferWorkflowLane(FinalDef.Name);
	}
	if (FinalDef.SurfaceArea.IsEmpty())
	{
		FinalDef.SurfaceArea = InferToolSurfaceArea(FinalDef.Name);
	}
	if (FinalDef.PrimaryLane.IsEmpty())
	{
		FinalDef.PrimaryLane = InferPrimaryLane(FinalDef.Name);
	}
	if (FinalDef.VfxAffinity.IsEmpty())
	{
		FinalDef.VfxAffinity = InferVfxAffinity(FinalDef.Name);
	}
	if (FinalDef.CapabilityTags.Num() == 0)
	{
		FinalDef.CapabilityTags = InferCapabilityTags(FinalDef.Name);
	}
	if (FinalDef.Mode.IsEmpty())
	{
		FinalDef.Mode = InferToolMode(FinalDef.Name);
	}
	if (FinalDef.MutationLevel.IsEmpty())
	{
		FinalDef.MutationLevel = InferMutationLevel(FinalDef.Name, FinalDef.Mode);
	}
	FinalDef.bSafeDefault = InferSafeDefault(FinalDef.Name, FinalDef.Mode);
	FinalDef.bProjectGrounded = InferProjectGrounded(FinalDef.Name);
	if (FinalDef.DefaultExposure.IsEmpty())
	{
		FinalDef.DefaultExposure = InferDefaultExposure(FinalDef.Name, FinalDef.Mode);
	}

	Tools.Add(FinalDef.Name, FinalDef);
	FForgePilotCache::Get().Invalidate(FForgePilotCache::GenerateToolListKey());
}

void FVFXToolRegistry::UnregisterTool(const FString& ToolName)
{
	Tools.Remove(ToolName);
	FForgePilotCache::Get().Invalidate(FForgePilotCache::GenerateToolListKey());
}

bool FVFXToolRegistry::HasTool(const FString& ToolName) const
{
	return Tools.Contains(ToolName);
}

bool FVFXToolRegistry::ExecuteTool(const FString& ToolName, const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FDateTime StartTime = FDateTime::Now();
	const TSharedPtr<FJsonObject> SafeInput = Input.IsValid() ? Input : MakeShareable(new FJsonObject());
	const bool bCacheable = FForgePilotCache::IsCacheable(ToolName, &SafeInput);
	bool bApplyChanges = false;
	TryGetBoolLikeField(SafeInput, TEXT("apply_changes"), bApplyChanges);

	FVFXToolDefinition* Found = Tools.Find(ToolName);
	if (!Found)
	{
		RegisterToolsForCategory(InferToolCategory(ToolName));
		Found = Tools.Find(ToolName);
		if (!Found)
		{
			OutResult = BuildRegistryErrorJson(
				TEXT("REG_TOOL_NOT_FOUND"),
				FString::Printf(TEXT("Unknown tool: %s"), *ToolName),
				ToolName);
			return false;
		}
	}

	if (!Found->ExecuteDelegate.IsBound())
	{
		OutResult = BuildRegistryErrorJson(
			TEXT("REG_TOOL_HANDLER_MISSING"),
			FString::Printf(TEXT("Tool %s has no handler"), *ToolName),
			ToolName);
		return false;
	}

	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bAllowAssetModification &&
		bApplyChanges &&
		!Found->MutationLevel.Equals(TEXT("read_only"), ESearchCase::IgnoreCase))
	{
		OutResult = BuildRegistryErrorJson(
			TEXT("REG_ASSET_MODIFICATION_DISABLED"),
			TEXT("Asset/editor mutation is disabled in ForgePilot settings. Re-run without apply_changes or enable asset modification."),
			ToolName);
		return false;
	}

	VFX_LOG_TOOL_START(ToolName, FString());

	if (bCacheable && !bApplyChanges)
	{
		const FString CacheKey = BuildToolCacheKey(ToolName, SafeInput);
		if (FForgePilotCache::Get().GetCached(CacheKey, OutResult))
		{
			VFX_LOG_TOOL_COMPLETE(ToolName, 0.0, true);
			FForgePilotMetrics::Get().RecordToolExecution(ToolName, 0.0, true);
			return true;
		}
	}

	bool bSuccess = Found->ExecuteDelegate.Execute(SafeInput, OutResult);

	double DurationMs = (FDateTime::Now() - StartTime).GetTotalMilliseconds();
	VFX_LOG_TOOL_COMPLETE(ToolName, DurationMs, bSuccess);
	FForgePilotMetrics::Get().RecordToolExecution(ToolName, DurationMs, bSuccess);

	if (bSuccess)
	{
		if (bCacheable && !bApplyChanges)
		{
			FForgePilotCache::Get().Put(BuildToolCacheKey(ToolName, SafeInput), OutResult);
		}
		else if (bApplyChanges)
		{
			FForgePilotCache::Get().Clear();
		}
	}

	return bSuccess;
}

TArray<TSharedPtr<FJsonValue>> FVFXToolRegistry::GetToolDefinitionsJson() const
{
	TArray<TSharedPtr<FJsonValue>> ToolsArray;

	for (const auto& Pair : Tools)
	{
		const FVFXToolDefinition& Def = Pair.Value;

		TSharedPtr<FJsonObject> ToolObj = MakeShareable(new FJsonObject());
		ToolObj->SetStringField(TEXT("name"), Def.Name);
		ToolObj->SetStringField(TEXT("description"), Def.Description);
		if (Def.InputSchema.IsValid())
		{
			ToolObj->SetObjectField(TEXT("input_schema"), Def.InputSchema);
		}
		ToolObj->SetStringField(TEXT("tool_group"), Def.ToolGroup);
		ToolObj->SetStringField(TEXT("unreal_domain"), Def.UnrealDomain);
		ToolObj->SetStringField(TEXT("workflow_lane"), Def.WorkflowLane);
		ToolObj->SetStringField(TEXT("surface_area"), Def.SurfaceArea);
		ToolObj->SetStringField(TEXT("primary_lane"), Def.PrimaryLane);
		ToolObj->SetStringField(TEXT("vfx_affinity"), Def.VfxAffinity);
		{
			TArray<TSharedPtr<FJsonValue>> TagValues;
			for (const FString& Tag : Def.CapabilityTags)
			{
				TagValues.Add(MakeShareable(new FJsonValueString(Tag)));
			}
			ToolObj->SetArrayField(TEXT("capability_tags"), TagValues);
		}
		ToolObj->SetStringField(TEXT("mode"), Def.Mode);
		ToolObj->SetStringField(TEXT("mutation_level"), Def.MutationLevel);
		ToolObj->SetBoolField(TEXT("safe_default"), Def.bSafeDefault);
		ToolObj->SetBoolField(TEXT("project_grounded"), Def.bProjectGrounded);
		ToolObj->SetStringField(TEXT("default_exposure"), Def.DefaultExposure);

		ToolsArray.Add(MakeShareable(new FJsonValueObject(ToolObj)));
	}

	return ToolsArray;
}

// ============================================================
// Register All Tools (delegates to per-phase methods)
// ============================================================

void FVFXToolRegistry::RegisterDefaultTools()
{
	if (bDefaultToolsRegistered)
	{
		return;
	}

	RegisterSceneTools();      // Phase 1: Scene manipulation
	RegisterBlueprintTools();  // Phase 2: Blueprint manipulation
	RegisterMaterialTools();   // Phase 3 + 3.5: Material creation & graph editing
	RegisterNiagaraTools();    // Phase 4: Niagara VFX
	RegisterAnimationTools();  // Phase 5: Animation-adjacent inspection

	bDefaultToolsRegistered = true;
	LoadedCategories.Add(TEXT("scene"));
	LoadedCategories.Add(TEXT("blueprint"));
	LoadedCategories.Add(TEXT("material"));
	LoadedCategories.Add(TEXT("niagara"));
	LoadedCategories.Add(TEXT("animation"));
	FForgePilotCache::Get().Invalidate(FForgePilotCache::GenerateToolListKey());
}

// ============================================================
// Lazy Loading Implementation
// ============================================================

void FVFXToolRegistry::RegisterToolsForCategory(const FString& Category)
{
	const FString CatLower = Category.ToLower();
	if (LoadedCategories.Contains(CatLower))
	{
		return;
	}

	if (CatLower == TEXT("scene"))
	{
		RegisterSceneTools();
		LoadedCategories.Add(TEXT("scene"));
	}
	else if (CatLower == TEXT("blueprint"))
	{
		RegisterBlueprintTools();
		LoadedCategories.Add(TEXT("blueprint"));
	}
	else if (CatLower == TEXT("material"))
	{
		RegisterMaterialTools();
		LoadedCategories.Add(TEXT("material"));
	}
	else if (CatLower == TEXT("niagara"))
	{
		RegisterNiagaraTools();
		LoadedCategories.Add(TEXT("niagara"));
	}
	else if (CatLower == TEXT("animation"))
	{
		RegisterAnimationTools();
		LoadedCategories.Add(TEXT("animation"));
	}

	FForgePilotCache::Get().Invalidate(FForgePilotCache::GenerateToolListKey());
}

bool FVFXToolRegistry::IsCategoryLoaded(const FString& Category) const
{
	return LoadedCategories.Contains(Category.ToLower());
}

TArray<FString> FVFXToolRegistry::GetLoadedCategories() const
{
	TArray<FString> Categories;
	for (const FString& Cat : LoadedCategories)
	{
		Categories.Add(Cat);
	}
	return Categories;
}

void FVFXToolRegistry::UnloadAllTools()
{
	Tools.Empty();
	LoadedCategories.Empty();
	bDefaultToolsRegistered = false;
	FForgePilotCache::Get().Clear();
}
