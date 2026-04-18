#include "VFXToolRegistry.h"

#include "VFXNiagaraStackToolRegistration.h"
#include "VFXNiagaraStackTools.h"
#include "VFXToolRegistryHelpers.h"

#include "NiagaraSystem.h"
#include "NiagaraScript.h"
#include "Misc/Paths.h"
#include "ViewModels/Stack/NiagaraStackFunctionInput.h"
#include "ViewModels/Stack/NiagaraStackModuleItem.h"

namespace
{
	const TCHAR* ModuleSpawnRatePath = TEXT("/Niagara/Modules/Emitter/SpawnRate.SpawnRate");
	const TCHAR* ModuleInitializeParticlePath = TEXT("/Niagara/Modules/Spawn/Initialization/V2/InitializeParticle.InitializeParticle");
	const TCHAR* ModuleScaleColorPath = TEXT("/Niagara/Modules/Update/Color/ScaleColor.ScaleColor");
	const TCHAR* ModuleAddVelocityPath = TEXT("/Niagara/Modules/Spawn/Velocity/AddVelocity.AddVelocity");
	const TCHAR* ModuleAddVelocityInConePath = TEXT("/Niagara/Modules/Spawn/Velocity/AddVelocityInCone.AddVelocityInCone");
	const TCHAR* ModuleGravityForcePath = TEXT("/Niagara/Modules/Update/Forces/GravityForce.GravityForce");
	const TCHAR* ModuleDragPath = TEXT("/Niagara/Modules/Update/Forces/Drag.Drag");
	const TCHAR* ModuleCurlNoiseForcePath = TEXT("/Niagara/Modules/Update/Forces/CurlNoiseForce.CurlNoiseForce");
	const TCHAR* ModuleSolveForcesPath = TEXT("/Niagara/Modules/Solvers/SolveForcesAndVelocity.SolveForcesAndVelocity");

	FString BuildNiagaraStackErrorJson(const FString& ErrorCode, const FString& ErrorMessage)
	{
		TSharedPtr<FJsonObject> ErrorObject = MakeShared<FJsonObject>();
		ErrorObject->SetBoolField(TEXT("success"), false);
		ErrorObject->SetStringField(TEXT("error_code"), ErrorCode);
		ErrorObject->SetStringField(TEXT("error_message"), ErrorMessage);
		return JsonObjToString(ErrorObject);
	}

	bool ReturnNiagaraStackError(FString& OutResult, const FString& ErrorCode, const FString& ErrorMessage)
	{
		OutResult = BuildNiagaraStackErrorJson(ErrorCode, ErrorMessage);
		return false;
	}

	bool ParseOptionalBoolFieldLocal(const TSharedPtr<FJsonObject>& Input, const FString& FieldName, bool bDefaultValue)
	{
		if (!Input.IsValid())
		{
			return bDefaultValue;
		}

		bool BoolValue = bDefaultValue;
		if (Input->TryGetBoolField(FieldName, BoolValue))
		{
			return BoolValue;
		}

		FString BoolString;
		if (Input->TryGetStringField(FieldName, BoolString) && !BoolString.IsEmpty())
		{
			return BoolString.Equals(TEXT("true"), ESearchCase::IgnoreCase) || BoolString.Equals(TEXT("1"));
		}

		return bDefaultValue;
	}

	bool ModuleMatchesTokens(
		const VFXNiagaraStackTools::FStackModuleSummary& ModuleSummary,
		const FString& DesiredStageUsage,
		const FString& PreferredScriptAssetPath,
		const TArray<FString>& Tokens,
		int32& OutScore)
	{
		OutScore = MIN_int32;
		if (!DesiredStageUsage.IsEmpty() && !ModuleSummary.StageUsage.Equals(DesiredStageUsage, ESearchCase::IgnoreCase))
		{
			return false;
		}

		const FString DisplayName = ModuleSummary.DisplayName.ToLower();
		const FString ScriptAssetPath = ModuleSummary.ScriptAssetPath.ToLower();
		const FString PreferredScriptPathLower = PreferredScriptAssetPath.ToLower();
		if (!PreferredScriptPathLower.IsEmpty() && ScriptAssetPath.Equals(PreferredScriptPathLower, ESearchCase::IgnoreCase))
		{
			OutScore = 300;
			return true;
		}

		for (const FString& Token : Tokens)
		{
			const FString Normalized = Token.ToLower();
			if (DisplayName.Contains(Normalized) || ScriptAssetPath.Contains(Normalized))
			{
				OutScore = FMath::Max(OutScore, DisplayName.Equals(Normalized, ESearchCase::IgnoreCase) ? 200 : 100);
			}
		}
		return OutScore != MIN_int32;
	}

	bool FindStandardModule(
		VFXNiagaraStackTools::FEmitterContext& Context,
		const FString& StageUsage,
		const FString& PreferredScriptAssetPath,
		const TArray<FString>& ModuleTokens,
		VFXNiagaraStackTools::FStackModuleSummary& OutSummary)
	{
		TArray<VFXNiagaraStackTools::FStackModuleSummary> Modules;
		VFXNiagaraStackTools::CollectModuleSummaries(Context, Modules);
		int32 BestScore = MIN_int32;
		for (VFXNiagaraStackTools::FStackModuleSummary& ModuleSummary : Modules)
		{
			int32 Score = MIN_int32;
			if (ModuleMatchesTokens(ModuleSummary, StageUsage, PreferredScriptAssetPath, ModuleTokens, Score) && Score > BestScore)
			{
				BestScore = Score;
				OutSummary = ModuleSummary;
			}
		}
		return BestScore != MIN_int32;
	}

	bool EnsureStandardModule(
		VFXNiagaraStackTools::FEmitterContext& Context,
		const FString& StageUsage,
		const FString& ModulePath,
		const TArray<FString>& ModuleTokens,
		bool bApplyChanges,
		VFXNiagaraStackTools::FStackModuleSummary& OutSummary,
		FString& OutAction,
		FString& OutErrorMessage)
	{
		if (FindStandardModule(Context, StageUsage, ModulePath, ModuleTokens, OutSummary))
		{
			OutAction = TEXT("existing");
			return true;
		}

		if (!bApplyChanges)
		{
			OutAction = TEXT("would_add");
			OutSummary.StageUsage = StageUsage;
			OutSummary.ScriptAssetPath = ModulePath;
			OutSummary.DisplayName = FPaths::GetBaseFilename(ModulePath);
			return true;
		}

		FString ModuleKey;
		if (!VFXNiagaraStackTools::AddModuleToStage(Context, StageUsage, ModulePath, INDEX_NONE, FString(), ModuleKey, OutErrorMessage))
		{
			return false;
		}

		TSharedPtr<FJsonObject> RefreshInput = MakeShared<FJsonObject>();
		RefreshInput->SetStringField(TEXT("system_path"), Context.System->GetPathName());
		RefreshInput->SetNumberField(TEXT("emitter_index"), Context.EmitterIndex);
		const FString RefreshEmitterName = Context.EmitterHandle != nullptr ? Context.EmitterHandle->GetName().ToString() : FString();
		if (!RefreshEmitterName.IsEmpty())
		{
			RefreshInput->SetStringField(TEXT("emitter_name"), RefreshEmitterName);
		}

		FString RefreshErrorCode;
		if (!VFXNiagaraStackTools::ResolveEmitterContext(Context.System->GetPathName(), RefreshInput, Context, RefreshErrorCode, OutErrorMessage))
		{
			OutErrorMessage = FString::Printf(TEXT("Module '%s' was added, but the Niagara emitter context could not be refreshed (%s: %s)."), *ModulePath, *RefreshErrorCode, *OutErrorMessage);
			return false;
		}

		if (!FindStandardModule(Context, StageUsage, ModulePath, ModuleTokens, OutSummary))
		{
			UNiagaraStackModuleItem* ModuleItem = VFXNiagaraStackTools::FindModuleByKey(Context, ModuleKey, &OutSummary);
			if (!ModuleItem)
			{
				OutErrorMessage = FString::Printf(TEXT("Module '%s' was added but could not be resolved afterwards."), *ModulePath);
				return false;
			}
		}

		OutAction = TEXT("added");
		return true;
	}

	UNiagaraStackFunctionInput* ResolveHeuristicInput(
		VFXNiagaraStackTools::FEmitterContext& Context,
		const FString& ModuleKey,
		const TArray<FString>& InputTokens,
		FString& OutInputKey,
		FString& OutErrorMessage)
	{
		UNiagaraStackFunctionInput* Input = VFXNiagaraStackTools::FindInputByHeuristics(Context, ModuleKey, InputTokens, OutInputKey);
		if (!Input)
		{
			OutErrorMessage = FString::Printf(TEXT("Could not resolve an input on module '%s' using tokens: %s"), *ModuleKey, *FString::Join(InputTokens, TEXT(", ")));
		}
		return Input;
	}

	bool FinalizeMutation(
		VFXNiagaraStackTools::FEmitterContext& Context,
		bool bApplyChanges,
		TSharedPtr<FJsonObject> Result,
		FString& OutResult,
		FString& OutErrorMessage)
	{
		if (!bApplyChanges)
		{
			Result->SetBoolField(TEXT("success"), true);
			OutResult = JsonObjToString(Result);
			return true;
		}

		TSharedPtr<FJsonObject> Diagnostics;
		if (!VFXNiagaraStackTools::CompileAndSaveSystem(Context, OutErrorMessage, Diagnostics))
		{
			TSharedPtr<FJsonObject> ErrorObject = MakeShared<FJsonObject>();
			ErrorObject->SetBoolField(TEXT("success"), false);
			ErrorObject->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
			ErrorObject->SetStringField(TEXT("error_message"), OutErrorMessage);
			ErrorObject->SetStringField(TEXT("system_path"), Context.System ? Context.System->GetPathName() : FString());
			ErrorObject->SetBoolField(TEXT("compile_log_checked"), Diagnostics.IsValid());
			if (Diagnostics.IsValid())
			{
				ErrorObject->SetObjectField(TEXT("compile_diagnostics"), Diagnostics);
			}
			OutResult = JsonObjToString(ErrorObject);
			return false;
		}

		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("compiled"), true);
		Result->SetBoolField(TEXT("saved"), true);
		Result->SetBoolField(TEXT("compile_log_checked"), Diagnostics.IsValid());
		if (Diagnostics.IsValid())
		{
			Result->SetObjectField(TEXT("compile_diagnostics"), Diagnostics);
		}
		OutResult = JsonObjToString(Result);
		return true;
	}

	TSharedPtr<FJsonObject> MakeModuleTargetSchema(const FString& Description)
	{
		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
		Properties->SetObjectField(TEXT("system_path"), MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Properties->SetObjectField(TEXT("emitter_name"), MakeStringProperty(TEXT("Optional emitter name. Defaults to the first emitter.")));
		Properties->SetObjectField(TEXT("emitter_index"), MakeStringProperty(TEXT("Optional emitter index. Overrides emitter_name when set.")));
		Properties->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default false. Set true to mutate the Niagara asset.")));
		Properties->SetObjectField(TEXT("allow_custom_expression"), MakeStringProperty(TEXT("Optional bool string. Default false. Required only for tools/paths that would otherwise author a custom expression instead of a native Niagara UI value.")));
		Properties->SetObjectField(TEXT("notes"), MakeStringProperty(Description));
		Schema->SetObjectField(TEXT("properties"), Properties);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShared<FJsonValueString>(TEXT("system_path")));
		Schema->SetArrayField(TEXT("required"), Required);
		return Schema;
	}
}

void RegisterNiagaraStackEditingTools(FVFXToolRegistry& Registry)
{
	auto Register = [&Registry](const FString& Name, const FString& Description, const TSharedPtr<FJsonObject>& Schema, FVFXToolExecuteDelegate Delegate)
	{
		FVFXToolDefinition Def;
		Def.Name = Name;
		Def.Description = Description;
		Def.InputSchema = Schema;
		Def.ExecuteDelegate = Delegate;
		Registry.RegisterTool(Def);
	};

	Register(TEXT("get_niagara_module_inputs"),
		TEXT("Inspect a Niagara emitter through the Niagara stack view model and return module keys, input keys, value sources, and serializable values."),
		MakeModuleTargetSchema(TEXT("Stack-aware Niagara module/input inspection.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_GetNiagaraModuleInputs));

	Register(TEXT("set_niagara_module_input_constant"),
		TEXT("Set a Niagara module input to a local constant value using a stack-aware module/input selector."),
		MakeModuleTargetSchema(TEXT("Requires module_key, input_key, and value.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraModuleInputConstant));

	Register(TEXT("set_niagara_module_input_linked"),
		TEXT("Link a Niagara module input to an existing exposed Niagara user parameter."),
		MakeModuleTargetSchema(TEXT("Requires module_key, input_key, and linked_param_name.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraModuleInputLinked));

	Register(TEXT("set_niagara_module_input_curve"),
		TEXT("Set a Niagara module input to a curve-like custom expression built from ordered points. This does not author a native Niagara curve UI value and requires allow_custom_expression=true."),
		MakeModuleTargetSchema(TEXT("Requires module_key, input_key, points, and allow_custom_expression=true.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraModuleInputCurve));

	Register(TEXT("set_niagara_module_input_dynamic"),
		TEXT("Set a Niagara module input to a supported dynamic input. Supported kinds: random_range, float_from_curve."),
		MakeModuleTargetSchema(TEXT("Requires module_key, input_key, dynamic_input_kind, and per-kind fields. random_range uses min_value/max_value. float_from_curve uses points.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraModuleInputDynamic));

	Register(TEXT("reset_niagara_module_input"),
		TEXT("Reset a Niagara module input back to default or base."),
		MakeModuleTargetSchema(TEXT("Requires module_key and input_key.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_ResetNiagaraModuleInput));

	Register(TEXT("add_niagara_module_to_stage"),
		TEXT("Add a Niagara module script to a target stage on an emitter."),
		MakeModuleTargetSchema(TEXT("Requires stage_usage and module_script_path.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_AddNiagaraModuleToStage));

	Register(TEXT("remove_niagara_module_from_stage"),
		TEXT("Remove a Niagara module from an emitter by module_key."),
		MakeModuleTargetSchema(TEXT("Requires module_key.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_RemoveNiagaraModuleFromStage));

	Register(TEXT("move_niagara_module_in_stage"),
		TEXT("Move a Niagara module to a new stack index inside its stage."),
		MakeModuleTargetSchema(TEXT("Requires module_key and target_index.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_MoveNiagaraModuleInStage));

	Register(TEXT("compile_niagara_system"),
		TEXT("Refresh, compile, and save a Niagara System deterministically after stack edits."),
		MakeModuleTargetSchema(TEXT("Compile/save Niagara after a batch of edits.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_CompileNiagaraSystem));

	Register(TEXT("set_niagara_spawn_rate"),
		TEXT("Ensure/edit the Spawn Rate module in Emitter Update. Supports constant values and native Float from Curve authoring when points are supplied."),
		MakeModuleTargetSchema(TEXT("Use value, or pass points to author a native Float from Curve dynamic input for spawn rate.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraSpawnRate));

	Register(TEXT("set_niagara_lifetime"),
		TEXT("Ensure/edit Initialize Particle lifetime. Supports constant or random range."),
		MakeModuleTargetSchema(TEXT("Use value or min_value/max_value.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraLifetime));

	Register(TEXT("set_niagara_sprite_size"),
		TEXT("Ensure/edit base sprite size in Initialize Particle. Supports constant or random range and auto-enables Uniform Sprite Size mode when needed."),
		MakeModuleTargetSchema(TEXT("Use value or min_value/max_value.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraSpriteSize));

	Register(TEXT("set_niagara_color_over_life"),
		TEXT("Ensure/edit a color-over-life module. Supports constant or random range. Point-authored color curves still require the custom-expression fallback path."),
		MakeModuleTargetSchema(TEXT("Use value or min_value/max_value. For vector/color points, pass allow_custom_expression=true only when the custom-expression fallback is acceptable.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraColorOverLife));

	Register(TEXT("set_niagara_opacity_over_life"),
		TEXT("Ensure/edit alpha over life on a color module. Supports constant values and native Float from Curve authoring when points are supplied."),
		MakeModuleTargetSchema(TEXT("Use value, or pass points to author a native Float from Curve dynamic input for alpha over life.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraOpacityOverLife));

	Register(TEXT("set_niagara_velocity"),
		TEXT("Ensure/edit Add Velocity in Particle Spawn."),
		MakeModuleTargetSchema(TEXT("Use value or min_value/max_value.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraVelocity));

	Register(TEXT("set_niagara_cone_velocity"),
		TEXT("Ensure/edit Add Velocity in Cone in Particle Spawn."),
		MakeModuleTargetSchema(TEXT("Use speed, direction, and optional cone angle.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraConeVelocity));

	Register(TEXT("set_niagara_gravity_force"),
		TEXT("Ensure/edit Gravity Force in Particle Update and keep solver support present. Supports constant values and native curve-based dynamic authoring when points are supplied."),
		MakeModuleTargetSchema(TEXT("Use value, or pass points to author a native Scale Vector by Curve dynamic input for gravity force.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraGravityForce));

	Register(TEXT("set_niagara_drag"),
		TEXT("Ensure/edit Drag in Particle Update and keep solver support present. Supports constant values and native Float from Curve authoring when points are supplied."),
		MakeModuleTargetSchema(TEXT("Use value, or pass points to author a native Float from Curve dynamic input for drag.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraDrag));

	Register(TEXT("set_niagara_curl_noise_force"),
		TEXT("Ensure/edit Curl Noise Force in Particle Update and keep solver support present. Supports constant values and native Float from Curve authoring when points are supplied."),
		MakeModuleTargetSchema(TEXT("Use value, or pass points to author a native Float from Curve dynamic input for curl noise strength.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraCurlNoiseForce));

	Register(TEXT("set_niagara_local_space"),
		TEXT("Toggle Niagara emitter local-space behavior."),
		MakeModuleTargetSchema(TEXT("Requires enabled or local_space.")),
		FVFXToolExecuteDelegate::CreateRaw(&Registry, &FVFXToolRegistry::Tool_SetNiagaraLocalSpace));
}

bool FVFXToolRegistry::Tool_GetNiagaraModuleInputs(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TArray<VFXNiagaraStackTools::FStackModuleSummary> Modules;
	VFXNiagaraStackTools::CollectModuleSummaries(Context, Modules);

	TArray<TSharedPtr<FJsonValue>> ModuleArray;
	for (const VFXNiagaraStackTools::FStackModuleSummary& ModuleSummary : Modules)
	{
		ModuleArray.Add(MakeShared<FJsonValueObject>(VFXNiagaraStackTools::BuildModuleJsonObject(ModuleSummary, true)));
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_niagara_module_inputs"));
	Result->SetStringField(TEXT("system_path"), Context.System->GetPathName());
	Result->SetStringField(TEXT("emitter_name"), Context.EmitterHandle->GetName().ToString());
	Result->SetNumberField(TEXT("emitter_index"), Context.EmitterIndex);
	Result->SetNumberField(TEXT("module_count"), ModuleArray.Num());
	Result->SetArrayField(TEXT("modules"), ModuleArray);
	Result->SetObjectField(TEXT("standard_module_presence"), VFXNiagaraStackTools::BuildStandardModulePresenceObject(Modules));
	Result->SetStringField(TEXT("message"), TEXT("Niagara module inputs extracted from stack view model."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetNiagaraModuleInputConstant(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	FString InputKey;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}
	if (!Input->TryGetStringField(TEXT("input_key"), InputKey) || InputKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_INPUT_KEY"), TEXT("Missing required field: input_key"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	UNiagaraStackFunctionInput* TargetInput = VFXNiagaraStackTools::FindInputByKey(Context, ModuleKey, InputKey, &ModuleSummary);
	if (!TargetInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), FString::Printf(TEXT("Input '%s' was not found on module '%s'."), *InputKey, *ModuleKey));
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_module_input_constant"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("system_path"), Context.System->GetPathName());
	Result->SetStringField(TEXT("emitter_name"), Context.EmitterHandle->GetName().ToString());
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);
	Result->SetStringField(TEXT("module_display_name"), ModuleSummary.DisplayName);
	Result->SetStringField(TEXT("input_display_name"), TargetInput->GetDisplayName().ToString());

	FString AppliedValueString;
	if (!bApplyChanges)
	{
		const TSharedPtr<FJsonValue> ValueField = Input->TryGetField(TEXT("value"));
		if (ValueField.IsValid())
		{
			if (ValueField->Type == EJson::String)
			{
				AppliedValueString = ValueField->AsString();
			}
			else
			{
				TSharedPtr<FJsonObject> Wrapper = MakeShared<FJsonObject>();
				Wrapper->SetField(TEXT("value"), ValueField);
				AppliedValueString = JsonObjToString(Wrapper);
			}
		}
		Result->SetStringField(TEXT("planned_value"), AppliedValueString);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would set Niagara module input to a constant local value."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!VFXNiagaraStackTools::SetConstantInputValue(*TargetInput, Input, TEXT("value"), AppliedValueString, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_CONSTANT_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("applied_value"), AppliedValueString);
	Result->SetStringField(TEXT("message"), TEXT("Niagara module input constant updated."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraModuleInputLinked(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	FString InputKey;
	FString LinkedParamName;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}
	if (!Input->TryGetStringField(TEXT("input_key"), InputKey) || InputKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_INPUT_KEY"), TEXT("Missing required field: input_key"));
	}
	if (!Input->TryGetStringField(TEXT("linked_param_name"), LinkedParamName) || LinkedParamName.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_LINKED_PARAM_NAME"), TEXT("Missing required field: linked_param_name"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	if (!LinkedParamName.StartsWith(TEXT("User."), ESearchCase::IgnoreCase))
	{
		LinkedParamName = FString::Printf(TEXT("User.%s"), *LinkedParamName);
	}

	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	UNiagaraStackFunctionInput* TargetInput = VFXNiagaraStackTools::FindInputByKey(Context, ModuleKey, InputKey, &ModuleSummary);
	if (!TargetInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), FString::Printf(TEXT("Input '%s' was not found on module '%s'."), *InputKey, *ModuleKey));
	}

	const FNiagaraVariableWithOffset* FoundParam = nullptr;
	for (const FNiagaraVariableWithOffset& ExistingVar : Context.System->GetExposedParameters().ReadParameterVariables())
	{
		if (ExistingVar.GetName().ToString().Equals(LinkedParamName, ESearchCase::IgnoreCase))
		{
			FoundParam = &ExistingVar;
			break;
		}
	}

	if (!FoundParam)
	{
		return ReturnNiagaraStackError(
			OutResult,
			TEXT("NI_PARAM_NOT_FOUND"),
			FString::Printf(TEXT("User parameter '%s' was not found on '%s'."), *LinkedParamName, *Context.System->GetPathName()));
	}

	const FNiagaraVariable LinkedVariable(FoundParam->GetType(), FoundParam->GetName());

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_module_input_linked"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("system_path"), Context.System->GetPathName());
	Result->SetStringField(TEXT("emitter_name"), Context.EmitterHandle->GetName().ToString());
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);
	Result->SetStringField(TEXT("module_display_name"), ModuleSummary.DisplayName);
	Result->SetStringField(TEXT("input_display_name"), TargetInput->GetDisplayName().ToString());
	Result->SetStringField(TEXT("linked_param_name"), LinkedParamName);
	Result->SetStringField(TEXT("linked_param_type"), FoundParam->GetType().GetName());

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would link the target Niagara input to an exposed user parameter."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString AppliedLinkedValue;
	if (!VFXNiagaraStackTools::SetLinkedInputValue(*TargetInput, LinkedVariable, AppliedLinkedValue, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_LINKED_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("applied_linked_value"), AppliedLinkedValue);
	Result->SetStringField(TEXT("message"), TEXT("Niagara module input linked to user parameter."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraModuleInputCurve(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	FString InputKey;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}
	if (!Input->TryGetStringField(TEXT("input_key"), InputKey) || InputKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_INPUT_KEY"), TEXT("Missing required field: input_key"));
	}

	const TArray<TSharedPtr<FJsonValue>>* CurvePoints = nullptr;
	if (!Input->TryGetArrayField(TEXT("points"), CurvePoints) || CurvePoints == nullptr || CurvePoints->Num() < 2)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_CURVE_POINTS"), TEXT("Missing required field: points (needs at least two points)."));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	UNiagaraStackFunctionInput* TargetInput = VFXNiagaraStackTools::FindInputByKey(Context, ModuleKey, InputKey, &ModuleSummary);
	if (!TargetInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), FString::Printf(TEXT("Input '%s' was not found on module '%s'."), *InputKey, *ModuleKey));
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_module_input_curve"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);
	Result->SetNumberField(TEXT("point_count"), CurvePoints->Num());

	if (!ParseOptionalBoolFieldLocal(Input, TEXT("allow_custom_expression"), false))
	{
		return ReturnNiagaraStackError(
			OutResult,
			TEXT("NI_CUSTOM_EXPRESSION_NOT_ALLOWED"),
			TEXT("set_niagara_module_input_curve produces a Niagara custom expression, not a native Niagara curve UI value. Re-run with allow_custom_expression=true only when that tradeoff is explicitly acceptable."));
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would convert the target Niagara input to a custom curve expression."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString AppliedExpression;
	if (!VFXNiagaraStackTools::SetCurveInputValue(*TargetInput, *CurvePoints, AppliedExpression, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_CURVE_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("expression"), AppliedExpression);
	Result->SetStringField(TEXT("message"), TEXT("Niagara module input curve expression updated."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraModuleInputDynamic(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	FString InputKey;
	FString DynamicKind;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}
	if (!Input->TryGetStringField(TEXT("input_key"), InputKey) || InputKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_INPUT_KEY"), TEXT("Missing required field: input_key"));
	}
	if (!Input->TryGetStringField(TEXT("dynamic_input_kind"), DynamicKind) || DynamicKind.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_DYNAMIC_KIND"), TEXT("Missing required field: dynamic_input_kind"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	UNiagaraStackFunctionInput* TargetInput = VFXNiagaraStackTools::FindInputByKey(Context, ModuleKey, InputKey, &ModuleSummary);
	if (!TargetInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), FString::Printf(TEXT("Input '%s' was not found on module '%s'."), *InputKey, *ModuleKey));
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_module_input_dynamic"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);
	Result->SetStringField(TEXT("dynamic_input_kind"), DynamicKind);

	if (!bApplyChanges)
	{
		if (DynamicKind.Equals(TEXT("random_range"), ESearchCase::IgnoreCase))
		{
			Result->SetStringField(TEXT("message"), TEXT("Dry-run: would convert the target Niagara input to a random-range dynamic input."));
		}
		else if (DynamicKind.Equals(TEXT("float_from_curve"), ESearchCase::IgnoreCase))
		{
			Result->SetStringField(TEXT("message"), TEXT("Dry-run: would convert the target Niagara input to a native Float from Curve dynamic input."));
		}
		else
		{
			return ReturnNiagaraStackError(
				OutResult,
				TEXT("NI_UNSUPPORTED_DYNAMIC_KIND"),
				TEXT("Supported dynamic_input_kind values are random_range and float_from_curve."));
		}
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString DynamicScriptPath;
	if (DynamicKind.Equals(TEXT("random_range"), ESearchCase::IgnoreCase))
	{
		if (!VFXNiagaraStackTools::SetDynamicRandomRangeValue(*TargetInput, Input, TEXT("min_value"), TEXT("max_value"), DynamicScriptPath, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_DYNAMIC_FAILED"), ErrorMessage);
		}
	}
	else if (DynamicKind.Equals(TEXT("float_from_curve"), ESearchCase::IgnoreCase))
	{
		if (!VFXNiagaraStackTools::SetDynamicFloatFromCurveValue(*TargetInput, Input, TEXT("points"), DynamicScriptPath, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_DYNAMIC_FAILED"), ErrorMessage);
		}
		Result->SetNumberField(TEXT("point_count"), Input->HasField(TEXT("points")) ? Input->GetArrayField(TEXT("points")).Num() : 0);
	}
	else
	{
		return ReturnNiagaraStackError(
			OutResult,
			TEXT("NI_UNSUPPORTED_DYNAMIC_KIND"),
			TEXT("Supported dynamic_input_kind values are random_range and float_from_curve."));
	}

	Result->SetStringField(TEXT("dynamic_script_path"), DynamicScriptPath);
	if (DynamicKind.Equals(TEXT("random_range"), ESearchCase::IgnoreCase))
	{
		Result->SetStringField(TEXT("message"), TEXT("Niagara module input converted to a random-range dynamic input."));
	}
	else
	{
		Result->SetStringField(TEXT("message"), TEXT("Niagara module input converted to a native Float from Curve dynamic input."));
	}
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_ResetNiagaraModuleInput(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	FString InputKey;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}
	if (!Input->TryGetStringField(TEXT("input_key"), InputKey) || InputKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_INPUT_KEY"), TEXT("Missing required field: input_key"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	const bool bResetToBaseIfAvailable = ParseOptionalBoolFieldLocal(Input, TEXT("reset_to_base_if_available"), true);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	UNiagaraStackFunctionInput* TargetInput = VFXNiagaraStackTools::FindInputByKey(Context, ModuleKey, InputKey);
	if (!TargetInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), FString::Printf(TEXT("Input '%s' was not found on module '%s'."), *InputKey, *ModuleKey));
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("reset_niagara_module_input"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("reset_to_base_if_available"), bResetToBaseIfAvailable);
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would reset the target Niagara input."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!VFXNiagaraStackTools::ResetInputValue(*TargetInput, bResetToBaseIfAvailable, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_RESET_INPUT_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("message"), TEXT("Niagara module input reset."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_AddNiagaraModuleToStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString StageUsage;
	FString ModuleScriptPath;
	FString SuggestedName;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("stage_usage"), StageUsage) || StageUsage.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_STAGE_USAGE"), TEXT("Missing required field: stage_usage"));
	}
	if (!Input->TryGetStringField(TEXT("module_script_path"), ModuleScriptPath) || ModuleScriptPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_SCRIPT_PATH"), TEXT("Missing required field: module_script_path"));
	}
	Input->TryGetStringField(TEXT("suggested_name"), SuggestedName);

	int32 TargetIndex = INDEX_NONE;
	double TargetIndexNumber = 0.0;
	if (Input->TryGetNumberField(TEXT("target_index"), TargetIndexNumber))
	{
		TargetIndex = static_cast<int32>(TargetIndexNumber);
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("add_niagara_module_to_stage"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("stage_usage"), StageUsage);
	Result->SetStringField(TEXT("module_script_path"), ModuleScriptPath);
	Result->SetNumberField(TEXT("target_index"), TargetIndex);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would add a Niagara module to the requested stage."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString ModuleKey;
	if (!VFXNiagaraStackTools::AddModuleToStage(Context, StageUsage, ModuleScriptPath, TargetIndex, SuggestedName, ModuleKey, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ADD_MODULE_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetStringField(TEXT("message"), TEXT("Niagara module added to stage."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_RemoveNiagaraModuleFromStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("remove_niagara_module_from_stage"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_key"), ModuleKey);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would remove the requested Niagara module."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!VFXNiagaraStackTools::RemoveModuleFromStack(Context, ModuleKey, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_REMOVE_MODULE_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("message"), TEXT("Niagara module removed from stage."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_MoveNiagaraModuleInStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	FString ModuleKey;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}
	if (!Input->TryGetStringField(TEXT("module_key"), ModuleKey) || ModuleKey.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_MODULE_KEY"), TEXT("Missing required field: module_key"));
	}

	double TargetIndexNumber = -1.0;
	if (!Input->TryGetNumberField(TEXT("target_index"), TargetIndexNumber))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_TARGET_INDEX"), TEXT("Missing required field: target_index"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("move_niagara_module_in_stage"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_key"), ModuleKey);
	Result->SetNumberField(TEXT("target_index"), TargetIndexNumber);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would move the requested Niagara module."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString MovedModuleKey;
	if (!VFXNiagaraStackTools::MoveModuleInStage(Context, ModuleKey, static_cast<int32>(TargetIndexNumber), MovedModuleKey, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MOVE_MODULE_FAILED"), ErrorMessage);
	}

	Result->SetStringField(TEXT("moved_module_key"), MovedModuleKey);
	Result->SetBoolField(TEXT("rebuilt_module_instance"), true);
	Result->SetStringField(TEXT("message"), TEXT("Niagara module moved inside its stage by reinserting a new instance and deleting the original."));
	return FinalizeMutation(Context, bApplyChanges, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_CompileNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Diagnostics;
	if (!VFXNiagaraStackTools::CompileAndSaveSystem(Context, ErrorMessage, Diagnostics))
	{
		TSharedPtr<FJsonObject> ErrorObject = MakeShared<FJsonObject>();
		ErrorObject->SetBoolField(TEXT("success"), false);
		ErrorObject->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
		ErrorObject->SetStringField(TEXT("error_message"), ErrorMessage);
		ErrorObject->SetStringField(TEXT("system_path"), Context.System->GetPathName());
		ErrorObject->SetBoolField(TEXT("compile_log_checked"), Diagnostics.IsValid());
		if (Diagnostics.IsValid())
		{
			ErrorObject->SetObjectField(TEXT("compile_diagnostics"), Diagnostics);
		}
		OutResult = JsonObjToString(ErrorObject);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("compile_niagara_system"));
	Result->SetStringField(TEXT("system_path"), Context.System->GetPathName());
	Result->SetBoolField(TEXT("compile_log_checked"), Diagnostics.IsValid());
	if (Diagnostics.IsValid())
	{
		Result->SetObjectField(TEXT("compile_diagnostics"), Diagnostics);
	}
	Result->SetStringField(TEXT("message"), TEXT("Niagara system compiled and saved."));
	OutResult = JsonObjToString(Result);
	return true;
}

namespace
{
	bool IsCustomExpressionExplicitlyAllowed(const TSharedPtr<FJsonObject>& Input)
	{
		return ParseOptionalBoolFieldLocal(Input, TEXT("allow_custom_expression"), false);
	}

	bool EnsureSupportModule(
		VFXNiagaraStackTools::FEmitterContext& Context,
		const FString& StageUsage,
		const FString& ModulePath,
		const TArray<FString>& ModuleTokens,
		bool bApplyChanges,
		FString& OutErrorMessage)
	{
		VFXNiagaraStackTools::FStackModuleSummary IgnoredSummary;
		FString IgnoredAction;
		return EnsureStandardModule(Context, StageUsage, ModulePath, ModuleTokens, bApplyChanges, IgnoredSummary, IgnoredAction, OutErrorMessage);
	}

	bool ApplyValueModeToStandardInput(
		VFXNiagaraStackTools::FEmitterContext& Context,
		const TSharedPtr<FJsonObject>& Input,
		const FString& ModuleKey,
		const TArray<FString>& InputTokens,
		bool bApplyChanges,
		bool bAllowCurve,
		bool bAllowRandomRange,
		TSharedPtr<FJsonObject> Result,
		FString& OutResult,
		FString& OutErrorMessage)
	{
		FString InputKey;
		UNiagaraStackFunctionInput* TargetInput = ResolveHeuristicInput(Context, ModuleKey, InputTokens, InputKey, OutErrorMessage);
		if (!TargetInput)
		{
			OutResult = BuildNiagaraStackErrorJson(TEXT("NI_INPUT_NOT_FOUND"), OutErrorMessage);
			return false;
		}

		Result->SetStringField(TEXT("module_key"), ModuleKey);
		Result->SetStringField(TEXT("input_key"), InputKey);
		Result->SetStringField(TEXT("input_display_name"), TargetInput->GetDisplayName().ToString());

		const TArray<TSharedPtr<FJsonValue>>* CurvePoints = nullptr;
		if (bAllowCurve && Input->TryGetArrayField(TEXT("points"), CurvePoints) && CurvePoints != nullptr && CurvePoints->Num() >= 2)
		{
			const bool bCanUseNativeFloatCurve = TargetInput->GetInputType() == FNiagaraTypeDefinition::GetFloatDef();
			if (!bApplyChanges)
			{
				Result->SetStringField(TEXT("planned_value_mode"), bCanUseNativeFloatCurve ? TEXT("float_from_curve_dynamic") : TEXT("custom_expression_curve"));
				OutResult = JsonObjToString(Result);
				return true;
			}

			if (bCanUseNativeFloatCurve)
			{
				FString DynamicScriptPath;
				if (!VFXNiagaraStackTools::SetDynamicFloatFromCurveValue(*TargetInput, Input, TEXT("points"), DynamicScriptPath, OutErrorMessage))
				{
					OutResult = BuildNiagaraStackErrorJson(TEXT("NI_SET_DYNAMIC_FAILED"), OutErrorMessage);
					return false;
				}

				Result->SetStringField(TEXT("applied_value_mode"), TEXT("float_from_curve_dynamic"));
				Result->SetStringField(TEXT("dynamic_script_path"), DynamicScriptPath);
				Result->SetNumberField(TEXT("point_count"), CurvePoints->Num());
				return FinalizeMutation(Context, true, Result, OutResult, OutErrorMessage);
			}

			if (!IsCustomExpressionExplicitlyAllowed(Input))
			{
				OutErrorMessage = TEXT("Curve points for this input type currently compile into a Niagara custom expression, not a native Niagara curve UI value. Re-run with allow_custom_expression=true only when that tradeoff is explicitly acceptable.");
				OutResult = BuildNiagaraStackErrorJson(TEXT("NI_CUSTOM_EXPRESSION_NOT_ALLOWED"), OutErrorMessage);
				return false;
			}

			FString AppliedExpression;
			if (!VFXNiagaraStackTools::SetCurveInputValue(*TargetInput, *CurvePoints, AppliedExpression, OutErrorMessage))
			{
				OutResult = BuildNiagaraStackErrorJson(TEXT("NI_SET_CURVE_FAILED"), OutErrorMessage);
				return false;
			}

			Result->SetStringField(TEXT("applied_value_mode"), TEXT("custom_expression_curve"));
			Result->SetStringField(TEXT("expression"), AppliedExpression);
			return FinalizeMutation(Context, true, Result, OutResult, OutErrorMessage);
		}

		if (bAllowRandomRange && Input->HasField(TEXT("min_value")) && Input->HasField(TEXT("max_value")))
		{
			if (!bApplyChanges)
			{
				Result->SetStringField(TEXT("planned_value_mode"), TEXT("random_range_dynamic"));
				OutResult = JsonObjToString(Result);
				return true;
			}

			FString DynamicScriptPath;
			if (!VFXNiagaraStackTools::SetDynamicRandomRangeValue(*TargetInput, Input, TEXT("min_value"), TEXT("max_value"), DynamicScriptPath, OutErrorMessage))
			{
				OutResult = BuildNiagaraStackErrorJson(TEXT("NI_SET_DYNAMIC_FAILED"), OutErrorMessage);
				return false;
			}

			Result->SetStringField(TEXT("applied_value_mode"), TEXT("random_range_dynamic"));
			Result->SetStringField(TEXT("dynamic_script_path"), DynamicScriptPath);
			return FinalizeMutation(Context, true, Result, OutResult, OutErrorMessage);
		}

		if (!Input->HasField(TEXT("value")))
		{
			OutErrorMessage = TEXT("Missing required field: value (or points / min_value + max_value).");
			OutResult = BuildNiagaraStackErrorJson(TEXT("NI_MISSING_VALUE"), OutErrorMessage);
			return false;
		}

		if (!bApplyChanges)
		{
			Result->SetStringField(TEXT("planned_value_mode"), TEXT("constant"));
			OutResult = JsonObjToString(Result);
			return true;
		}

		FString AppliedValueString;
		if (!VFXNiagaraStackTools::SetConstantInputValue(*TargetInput, Input, TEXT("value"), AppliedValueString, OutErrorMessage))
		{
			OutResult = BuildNiagaraStackErrorJson(TEXT("NI_SET_CONSTANT_FAILED"), OutErrorMessage);
			return false;
		}

		Result->SetStringField(TEXT("applied_value_mode"), TEXT("constant"));
		Result->SetStringField(TEXT("applied_value"), AppliedValueString);
		return FinalizeMutation(Context, true, Result, OutResult, OutErrorMessage);
	}
}

bool FVFXToolRegistry::Tool_SetNiagaraSpawnRate(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("emitter_update"), ModuleSpawnRatePath, { TEXT("spawnrate"), TEXT("spawn rate") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_spawn_rate"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	Result->SetStringField(TEXT("module_display_name"), ModuleSummary.DisplayName);
	Result->SetStringField(TEXT("message"), TEXT("Spawn Rate module targeted."));
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("spawnrate"), TEXT("spawn rate") }, bApplyChanges, true, false, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraLifetime(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_spawn"), ModuleInitializeParticlePath, { TEXT("initializeparticle"), TEXT("initialize particle") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_lifetime"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("lifetime") }, bApplyChanges, false, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraSpriteSize(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_spawn"), ModuleInitializeParticlePath, { TEXT("initializeparticle"), TEXT("initialize particle") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_sprite_size"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);

	FString ResolvedInputKey;
	UNiagaraStackFunctionInput* ResolvedInput = ResolveHeuristicInput(
		Context,
		ModuleSummary.ModuleKey,
		{ TEXT("module.uniform sprite size"), TEXT("uniform sprite size"), TEXT("module.sprite size"), TEXT("sprite size"), TEXT("spritesize") },
		ResolvedInputKey,
		ErrorMessage);

	const bool bNeedsUniformMode = !ResolvedInput || ResolvedInputKey.Contains(TEXT("mode"), ESearchCase::IgnoreCase);
	if (bNeedsUniformMode)
	{
		FString ModeInputKey;
		UNiagaraStackFunctionInput* ModeInput = ResolveHeuristicInput(
			Context,
			ModuleSummary.ModuleKey,
			{ TEXT("sprite size mode") },
			ModeInputKey,
			ErrorMessage);
		if (!ModeInput)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), ErrorMessage);
		}

		Result->SetBoolField(TEXT("uniform_mode_required"), true);
		Result->SetStringField(TEXT("sprite_size_mode_input_key"), ModeInputKey);

		if (!bApplyChanges)
		{
			Result->SetStringField(TEXT("planned_mode_value"), TEXT("1"));
		}
		else
		{
			TSharedPtr<FJsonObject> ModePayload = MakeShared<FJsonObject>();
			ModePayload->SetNumberField(TEXT("value"), 1.0);

			FString AppliedModeValue;
			if (!VFXNiagaraStackTools::SetConstantInputValue(*ModeInput, ModePayload, TEXT("value"), AppliedModeValue, ErrorMessage))
			{
				return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_CONSTANT_FAILED"), ErrorMessage);
			}

			Result->SetStringField(TEXT("applied_mode_value"), AppliedModeValue);

			TSharedPtr<FJsonObject> RefreshInput = MakeShared<FJsonObject>();
			RefreshInput->SetStringField(TEXT("system_path"), Context.System->GetPathName());
			RefreshInput->SetNumberField(TEXT("emitter_index"), Context.EmitterIndex);
			if (Context.EmitterHandle != nullptr)
			{
				RefreshInput->SetStringField(TEXT("emitter_name"), Context.EmitterHandle->GetName().ToString());
			}

			FString RefreshErrorCode;
			if (!VFXNiagaraStackTools::ResolveEmitterContext(Context.System->GetPathName(), RefreshInput, Context, RefreshErrorCode, ErrorMessage))
			{
				return ReturnNiagaraStackError(OutResult, RefreshErrorCode, ErrorMessage);
			}
		}
	}

	return ApplyValueModeToStandardInput(
		Context,
		Input,
		ModuleSummary.ModuleKey,
		{ TEXT("module.uniform sprite size"), TEXT("uniform sprite size"), TEXT("module.sprite size"), TEXT("sprite size"), TEXT("spritesize") },
		bApplyChanges,
		false,
		true,
		Result,
		OutResult,
		ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraColorOverLife(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleScaleColorPath, { TEXT("scalecolor"), TEXT("scale color") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_color_over_life"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("scale rgb"), TEXT("rgb"), TEXT("color") }, bApplyChanges, true, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraOpacityOverLife(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleScaleColorPath, { TEXT("scalecolor"), TEXT("scale color") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_opacity_over_life"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("scale alpha"), TEXT("alpha"), TEXT("opacity") }, bApplyChanges, true, false, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraVelocity(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_spawn"), ModuleAddVelocityPath, { TEXT("addvelocity"), TEXT("add velocity") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_velocity"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("velocity") }, bApplyChanges, false, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraConeVelocity(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_spawn"), ModuleAddVelocityInConePath, { TEXT("addvelocityincone"), TEXT("add velocity in cone"), TEXT("cone velocity") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_cone_velocity"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("module_action"), ModuleAction);

	if (!bApplyChanges)
	{
		TArray<FString> PlannedInputs;
		if (Input->HasField(TEXT("direction")))
		{
			PlannedInputs.Add(TEXT("direction"));
		}
		if (Input->HasField(TEXT("angle")))
		{
			PlannedInputs.Add(TEXT("angle"));
		}
		if (Input->HasField(TEXT("speed")) || (Input->HasField(TEXT("min_speed")) && Input->HasField(TEXT("max_speed"))))
		{
			PlannedInputs.Add(TEXT("speed"));
		}
		TArray<TSharedPtr<FJsonValue>> PlannedJson;
		for (const FString& Entry : PlannedInputs)
		{
			PlannedJson.Add(MakeShared<FJsonValueString>(Entry));
		}
		Result->SetArrayField(TEXT("planned_inputs"), PlannedJson);
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (Input->HasField(TEXT("direction")))
	{
		FString AppliedDirection;
		FString ResolvedInputKey;
		UNiagaraStackFunctionInput* DirectionInput = VFXNiagaraStackTools::FindInputByHeuristics(Context, ModuleSummary.ModuleKey, { TEXT("direction"), TEXT("cone axis") }, ResolvedInputKey);
		if (!DirectionInput)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), TEXT("Could not resolve cone direction input."));
		}
		if (!VFXNiagaraStackTools::SetConstantInputValue(*DirectionInput, Input, TEXT("direction"), AppliedDirection, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_INPUT_FAILED"), ErrorMessage);
		}
		Result->SetStringField(TEXT("applied_direction"), AppliedDirection);
	}

	if (Input->HasField(TEXT("angle")))
	{
		FString AppliedAngle;
		FString ResolvedInputKey;
		UNiagaraStackFunctionInput* AngleInput = VFXNiagaraStackTools::FindInputByHeuristics(Context, ModuleSummary.ModuleKey, { TEXT("angle"), TEXT("cone angle") }, ResolvedInputKey);
		if (!AngleInput)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), TEXT("Could not resolve cone angle input."));
		}
		if (!VFXNiagaraStackTools::SetConstantInputValue(*AngleInput, Input, TEXT("angle"), AppliedAngle, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_INPUT_FAILED"), ErrorMessage);
		}
		Result->SetStringField(TEXT("applied_angle"), AppliedAngle);
	}

	if (Input->HasField(TEXT("min_speed")) && Input->HasField(TEXT("max_speed")))
	{
		FString ResolvedInputKey;
		UNiagaraStackFunctionInput* SpeedInput = VFXNiagaraStackTools::FindInputByHeuristics(Context, ModuleSummary.ModuleKey, { TEXT("velocity strength"), TEXT("speed"), TEXT("velocity") }, ResolvedInputKey);
		if (!SpeedInput)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), TEXT("Could not resolve cone speed input."));
		}

		FString DynamicScriptPath;
		if (!VFXNiagaraStackTools::SetDynamicRandomRangeValue(*SpeedInput, Input, TEXT("min_speed"), TEXT("max_speed"), DynamicScriptPath, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_INPUT_FAILED"), ErrorMessage);
		}
		Result->SetStringField(TEXT("speed_mode"), TEXT("random_range_dynamic"));
		Result->SetStringField(TEXT("dynamic_script_path"), DynamicScriptPath);
	}
	else if (Input->HasField(TEXT("speed")))
	{
		FString AppliedSpeed;
		FString ResolvedInputKey;
		UNiagaraStackFunctionInput* SpeedInput = VFXNiagaraStackTools::FindInputByHeuristics(Context, ModuleSummary.ModuleKey, { TEXT("velocity strength"), TEXT("speed"), TEXT("velocity") }, ResolvedInputKey);
		if (!SpeedInput)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), TEXT("Could not resolve cone speed input."));
		}

		if (!VFXNiagaraStackTools::SetConstantInputValue(*SpeedInput, Input, TEXT("speed"), AppliedSpeed, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_INPUT_FAILED"), ErrorMessage);
		}
		Result->SetStringField(TEXT("speed_mode"), TEXT("constant"));
		Result->SetStringField(TEXT("applied_speed"), AppliedSpeed);
	}

	return FinalizeMutation(Context, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraGravityForce(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary SolveSummary;
	FString SolveAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleSolveForcesPath, { TEXT("solveforcesandvelocity"), TEXT("solve forces and velocity") }, bApplyChanges, SolveSummary, SolveAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_SOLVER_FAILED"), ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleGravityForcePath, { TEXT("gravityforce"), TEXT("gravity force") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_gravity_force"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("solver_action"), SolveAction);
	Result->SetStringField(TEXT("module_action"), ModuleAction);

	FString InputKey;
	UNiagaraStackFunctionInput* GravityInput = ResolveHeuristicInput(Context, ModuleSummary.ModuleKey, { TEXT("gravity force"), TEXT("acceleration"), TEXT("gravity") }, InputKey, ErrorMessage);
	if (!GravityInput)
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_INPUT_NOT_FOUND"), ErrorMessage);
	}

	Result->SetStringField(TEXT("module_key"), ModuleSummary.ModuleKey);
	Result->SetStringField(TEXT("input_key"), InputKey);
	Result->SetStringField(TEXT("input_display_name"), GravityInput->GetDisplayName().ToString());

	const TArray<TSharedPtr<FJsonValue>>* CurvePoints = nullptr;
	if (Input->TryGetArrayField(TEXT("points"), CurvePoints) && CurvePoints != nullptr && CurvePoints->Num() >= 2)
	{
		if (!bApplyChanges)
		{
			Result->SetStringField(TEXT("planned_value_mode"), TEXT("scale_vector_by_curve_dynamic"));
			OutResult = JsonObjToString(Result);
			return true;
		}

		FString DynamicScriptPath;
		if (!VFXNiagaraStackTools::SetDynamicScaleVectorByCurveValue(*GravityInput, Input, TEXT("points"), TEXT("base_vector"), DynamicScriptPath, ErrorMessage))
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_DYNAMIC_FAILED"), ErrorMessage);
		}

		Result->SetStringField(TEXT("applied_value_mode"), TEXT("scale_vector_by_curve_dynamic"));
		Result->SetStringField(TEXT("dynamic_script_path"), DynamicScriptPath);
		Result->SetNumberField(TEXT("point_count"), CurvePoints->Num());
		return FinalizeMutation(Context, true, Result, OutResult, ErrorMessage);
	}

	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("gravity force"), TEXT("acceleration"), TEXT("gravity") }, bApplyChanges, false, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraDrag(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary SolveSummary;
	FString SolveAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleSolveForcesPath, { TEXT("solveforcesandvelocity"), TEXT("solve forces and velocity") }, bApplyChanges, SolveSummary, SolveAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_SOLVER_FAILED"), ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleDragPath, { TEXT("drag") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_drag"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("solver_action"), SolveAction);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("drag") }, bApplyChanges, true, false, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraCurlNoiseForce(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary SolveSummary;
	FString SolveAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleSolveForcesPath, { TEXT("solveforcesandvelocity"), TEXT("solve forces and velocity") }, bApplyChanges, SolveSummary, SolveAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_SOLVER_FAILED"), ErrorMessage);
	}

	VFXNiagaraStackTools::FStackModuleSummary ModuleSummary;
	FString ModuleAction;
	if (!EnsureStandardModule(Context, TEXT("particle_update"), ModuleCurlNoiseForcePath, { TEXT("curlnoiseforce"), TEXT("curl noise force") }, bApplyChanges, ModuleSummary, ModuleAction, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_ENSURE_MODULE_FAILED"), ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_curl_noise_force"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("solver_action"), SolveAction);
	Result->SetStringField(TEXT("module_action"), ModuleAction);
	return ApplyValueModeToStandardInput(Context, Input, ModuleSummary.ModuleKey, { TEXT("strength"), TEXT("curl noise force"), TEXT("noise strength") }, bApplyChanges, true, true, Result, OutResult, ErrorMessage);
}

bool FVFXToolRegistry::Tool_SetNiagaraLocalSpace(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	bool bLocalSpace = false;
	if (Input->HasField(TEXT("enabled")))
	{
		bLocalSpace = ParseOptionalBoolFieldLocal(Input, TEXT("enabled"), false);
	}
	else
	{
		bLocalSpace = ParseOptionalBoolFieldLocal(Input, TEXT("local_space"), false);
	}

	const bool bApplyChanges = ParseOptionalBoolFieldLocal(Input, TEXT("apply_changes"), false);
	VFXNiagaraStackTools::FEmitterContext Context;
	FString ErrorCode;
	FString ErrorMessage;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, Context, ErrorCode, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_local_space"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("local_space"), bLocalSpace);
	const bool bSetPersistentIds = Input->HasField(TEXT("requires_persistent_ids"));
	const bool bRequiresPersistentIds = ParseOptionalBoolFieldLocal(Input, TEXT("requires_persistent_ids"), false);
	if (bSetPersistentIds)
	{
		Result->SetBoolField(TEXT("requires_persistent_ids"), bRequiresPersistentIds);
	}

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("success"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!VFXNiagaraStackTools::SetEmitterLocalSpace(Context, bLocalSpace, ErrorMessage))
	{
		return ReturnNiagaraStackError(OutResult, TEXT("NI_SET_LOCAL_SPACE_FAILED"), ErrorMessage);
	}
	if (bSetPersistentIds)
	{
		if (!Context.EmitterData)
		{
			return ReturnNiagaraStackError(OutResult, TEXT("NI_EMITTER_DATA_MISSING"), TEXT("Emitter data is missing."));
		}
		Context.EmitterData->bRequiresPersistentIDs = bRequiresPersistentIds;
		Context.System->MarkPackageDirty();
	}

	return FinalizeMutation(Context, true, Result, OutResult, ErrorMessage);
}
