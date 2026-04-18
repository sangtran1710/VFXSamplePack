#include "VFXNiagaraStackTools.h"

#include "EditorAssetLibrary.h"
#include "NiagaraDataInterfaceCurve.h"
#include "NiagaraDataInterfaceVectorCurve.h"
#include "NiagaraEmitter.h"
#include "NiagaraNodeAssignment.h"
#include "NiagaraNodeFunctionCall.h"
#include "NiagaraNodeOutput.h"
#include "NiagaraModule.h"
#include "NiagaraScript.h"
#include "NiagaraScriptSource.h"
#include "NiagaraSystem.h"
#include "NiagaraSystemEditorData.h"
#include "NiagaraTypes.h"
#include "FileHelpers.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "UObject/ObjectRedirector.h"
#include "UObject/SavePackage.h"
#include "ViewModels/NiagaraEmitterHandleViewModel.h"
#include "ViewModels/NiagaraSystemViewModel.h"
#include "ViewModels/Stack/NiagaraStackEntry.h"
#include "ViewModels/Stack/NiagaraStackFunctionInput.h"
#include "ViewModels/Stack/NiagaraStackGraphUtilities.h"
#include "ViewModels/Stack/NiagaraStackModuleItem.h"
#include "ViewModels/Stack/NiagaraParameterHandle.h"
#include "ViewModels/Stack/NiagaraStackViewModel.h"
#include "VFXToolRegistryHelpers.h"

namespace VFXNiagaraStackTools
{
TSharedPtr<FJsonValue> SerializeInputValue(const UNiagaraStackFunctionInput& Input);

namespace
{
	const TCHAR* StageEmitterSpawn = TEXT("emitter_spawn");
	const TCHAR* StageEmitterUpdate = TEXT("emitter_update");
	const TCHAR* StageParticleSpawn = TEXT("particle_spawn");
	const TCHAR* StageParticleUpdate = TEXT("particle_update");
	const TCHAR* StageSimulationStage = TEXT("simulation_stage");
	const TCHAR* StageEvent = TEXT("event");

	FString NormalizeToken(const FString& InValue)
	{
		return InValue.TrimStartAndEnd().ToLower();
	}

	FString ValueModeToString(UNiagaraStackFunctionInput::EValueMode Mode)
	{
		using EValueMode = UNiagaraStackFunctionInput::EValueMode;
		switch (Mode)
		{
		case EValueMode::Local: return TEXT("local");
		case EValueMode::Linked: return TEXT("linked");
		case EValueMode::Dynamic: return TEXT("dynamic");
		case EValueMode::Data: return TEXT("data");
		case EValueMode::ObjectAsset: return TEXT("object_asset");
		case EValueMode::Expression: return TEXT("expression");
		case EValueMode::DefaultFunction: return TEXT("default_function");
		case EValueMode::InvalidOverride: return TEXT("invalid_override");
		case EValueMode::UnsupportedDefault: return TEXT("unsupported_default");
		case EValueMode::None: default: return TEXT("none");
		}
	}

	FString NiagaraTypeToString(const FNiagaraTypeDefinition& Type)
	{
		if (Type == FNiagaraTypeDefinition::GetFloatDef())
		{
			return TEXT("float");
		}
		if (Type == FNiagaraTypeDefinition::GetIntDef())
		{
			return TEXT("int");
		}
		if (Type == FNiagaraTypeDefinition::GetBoolDef())
		{
			return TEXT("bool");
		}
		if (Type == FNiagaraTypeDefinition::GetVec2Def())
		{
			return TEXT("vector2");
		}
		if (Type == FNiagaraTypeDefinition::GetVec3Def() || Type == FNiagaraTypeDefinition::GetPositionDef())
		{
			return TEXT("vector3");
		}
		if (Type == FNiagaraTypeDefinition::GetColorDef())
		{
			return TEXT("color");
		}
		if (const UScriptStruct* Struct = Cast<UScriptStruct>(Type.GetStruct()))
		{
			return Struct->GetName();
		}
		if (const UClass* Class = Type.GetClass())
		{
			return Class->GetName();
		}
		return TEXT("unknown");
	}

	void AppendInputSummaryRecursive(
		UNiagaraStackFunctionInput& Input,
		const FString& ParentPath,
		TArray<VFXNiagaraStackTools::FStackInputSummary>& OutSummaries)
	{
		const FString RawInputKey = Input.GetInputParameterHandle().GetParameterHandleString().ToString();
		const FString InputPath = ParentPath.IsEmpty()
			? RawInputKey
			: FString::Printf(TEXT("%s > %s"), *ParentPath, *RawInputKey);

		VFXNiagaraStackTools::FStackInputSummary InputSummary;
		InputSummary.InputKey = InputPath;
		InputSummary.DisplayName = Input.GetDisplayName().ToString();
		InputSummary.TypeName = NiagaraTypeToString(Input.GetInputType());
		InputSummary.ValueSource = ValueModeToString(Input.GetValueMode());
		InputSummary.bHasEditableLocalValue = Input.GetInputType().GetStruct() != nullptr;
		InputSummary.bCanReset = Input.CanReset() || Input.CanResetToBase();

		const TSharedPtr<FJsonValue> SerializedValue = SerializeInputValue(Input);
		if (SerializedValue.IsValid())
		{
			if (SerializedValue->Type == EJson::String)
			{
				InputSummary.ValueString = SerializedValue->AsString();
			}
			else
			{
				TSharedPtr<FJsonObject> Wrapper = MakeShared<FJsonObject>();
				Wrapper->SetField(TEXT("value"), SerializedValue);
				InputSummary.ValueString = JsonObjToString(Wrapper);
			}
		}

		OutSummaries.Add(InputSummary);

		TArray<UNiagaraStackFunctionInput*> ChildInputs = Input.GetChildInputs();
		for (UNiagaraStackFunctionInput* Child : ChildInputs)
		{
			if (Child)
			{
				AppendInputSummaryRecursive(*Child, InputPath, OutSummaries);
			}
		}
	}

	UNiagaraStackFunctionInput* FindInputByKeyRecursive(
		UNiagaraStackFunctionInput& Input,
		const FString& InputKey,
		const FString& ParentPath)
	{
		const FString RawInputKey = Input.GetInputParameterHandle().GetParameterHandleString().ToString();
		const FString InputPath = ParentPath.IsEmpty()
			? RawInputKey
			: FString::Printf(TEXT("%s > %s"), *ParentPath, *RawInputKey);

		if (InputPath.Equals(InputKey, ESearchCase::IgnoreCase) ||
			RawInputKey.Equals(InputKey, ESearchCase::IgnoreCase))
		{
			return &Input;
		}

		TArray<UNiagaraStackFunctionInput*> ChildInputs = Input.GetChildInputs();
		for (UNiagaraStackFunctionInput* Child : ChildInputs)
		{
			if (!Child)
			{
				continue;
			}

			if (UNiagaraStackFunctionInput* Match = FindInputByKeyRecursive(*Child, InputKey, InputPath))
			{
				return Match;
			}
		}

		return nullptr;
	}

	bool LooksLikeNiagaraExpressionString(const FString& Value)
	{
		const FString Trimmed = Value.TrimStartAndEnd();
		if (Trimmed.IsEmpty())
		{
			return false;
		}

		if (Trimmed.Contains(TEXT("Particles."), ESearchCase::IgnoreCase) ||
			Trimmed.Contains(TEXT("Engine."), ESearchCase::IgnoreCase) ||
			Trimmed.Contains(TEXT("Module."), ESearchCase::IgnoreCase) ||
			Trimmed.Contains(TEXT("Transient."), ESearchCase::IgnoreCase))
		{
			return true;
		}

		return Trimmed.Contains(TEXT("("))
			|| Trimmed.Contains(TEXT(")"))
			|| Trimmed.Contains(TEXT("?"))
			|| Trimmed.Contains(TEXT("/"))
			|| Trimmed.Contains(TEXT("*"))
			|| Trimmed.Contains(TEXT("+"))
			|| Trimmed.Contains(TEXT("-"));
	}

	FString UsageToToolStageStringInternal(ENiagaraScriptUsage Usage)
	{
		switch (Usage)
		{
		case ENiagaraScriptUsage::EmitterSpawnScript:
			return StageEmitterSpawn;
		case ENiagaraScriptUsage::EmitterUpdateScript:
			return StageEmitterUpdate;
		case ENiagaraScriptUsage::ParticleSpawnScript:
		case ENiagaraScriptUsage::ParticleSpawnScriptInterpolated:
			return StageParticleSpawn;
		case ENiagaraScriptUsage::ParticleUpdateScript:
			return StageParticleUpdate;
		case ENiagaraScriptUsage::ParticleSimulationStageScript:
			return StageSimulationStage;
		default:
			return StageEvent;
		}
	}

	FString CompileStatusToString(ENiagaraScriptCompileStatus Status)
	{
		if (const UEnum* CompileStatusEnum = StaticEnum<ENiagaraScriptCompileStatus>())
		{
			return CompileStatusEnum->GetNameStringByValue(static_cast<int64>(Status));
		}
		return TEXT("Unknown");
	}

	void GatherSystemScripts(UNiagaraSystem* NiagaraSystem, TArray<UNiagaraScript*>& OutScripts)
	{
		OutScripts.Reset();
		if (!NiagaraSystem)
		{
			return;
		}

		if (UNiagaraScript* SystemSpawnScript = NiagaraSystem->GetSystemSpawnScript())
		{
			OutScripts.Add(SystemSpawnScript);
		}
		if (UNiagaraScript* SystemUpdateScript = NiagaraSystem->GetSystemUpdateScript())
		{
			OutScripts.Add(SystemUpdateScript);
		}

		for (const FNiagaraEmitterHandle& Handle : NiagaraSystem->GetEmitterHandles())
		{
			FVersionedNiagaraEmitter EmitterInSystem = Handle.GetInstance();
			if (FVersionedNiagaraEmitterData* EmitterData = EmitterInSystem.GetEmitterData())
			{
				TArray<UNiagaraScript*> EmitterScripts;
				EmitterData->GetScripts(EmitterScripts, false);
				for (UNiagaraScript* EmitterScript : EmitterScripts)
				{
					if (EmitterScript)
					{
						OutScripts.AddUnique(EmitterScript);
					}
				}
			}
		}
	}

	bool IsCompileStatusPending(ENiagaraScriptCompileStatus Status)
	{
		return Status == ENiagaraScriptCompileStatus::NCS_Unknown
			|| Status == ENiagaraScriptCompileStatus::NCS_Dirty
			|| Status == ENiagaraScriptCompileStatus::NCS_BeingCreated;
	}

	bool IsBenignPendingScript(const UNiagaraScript* Script)
	{
		if (!Script)
		{
			return false;
		}

		switch (Script->GetUsage())
		{
		case ENiagaraScriptUsage::EmitterSpawnScript:
		case ENiagaraScriptUsage::EmitterUpdateScript:
			return true;
		default:
			return false;
		}
	}

	bool IsCompileStatusWarning(ENiagaraScriptCompileStatus Status)
	{
		return Status == ENiagaraScriptCompileStatus::NCS_UpToDateWithWarnings
			|| Status == ENiagaraScriptCompileStatus::NCS_ComputeUpToDateWithWarnings;
	}

	ENiagaraScriptCompileStatus AggregateScriptStatuses(const TArray<UNiagaraScript*>& Scripts)
	{
		bool bHasWarnings = false;
		bool bAnyPending = false;
		for (UNiagaraScript* Script : Scripts)
		{
			if (!Script)
			{
				continue;
			}

			const ENiagaraScriptCompileStatus ScriptStatus = Script->GetLastCompileStatus();
			if (ScriptStatus == ENiagaraScriptCompileStatus::NCS_Error)
			{
				return ENiagaraScriptCompileStatus::NCS_Error;
			}
			if ((IsCompileStatusPending(ScriptStatus) || !Script->AreScriptAndSourceSynchronized()) && !IsBenignPendingScript(Script))
			{
				bAnyPending = true;
			}
			if (IsCompileStatusWarning(ScriptStatus))
			{
				bHasWarnings = true;
			}
		}

		if (bAnyPending)
		{
			return ENiagaraScriptCompileStatus::NCS_Dirty;
		}
		return bHasWarnings
			? ENiagaraScriptCompileStatus::NCS_UpToDateWithWarnings
			: ENiagaraScriptCompileStatus::NCS_UpToDate;
	}

	TSharedPtr<FJsonObject> BuildCompileDiagnostics(
		FEmitterContext& Context,
		ENiagaraScriptCompileStatus LatestCompileStatus,
		const TArray<UNiagaraScript*>& Scripts)
	{
		TSharedPtr<FJsonObject> Diagnostics = MakeShared<FJsonObject>();
		Diagnostics->SetStringField(TEXT("latest_compile_status"), CompileStatusToString(LatestCompileStatus));
		Diagnostics->SetBoolField(TEXT("system_is_valid"), Context.System ? Context.System->IsValid() : false);

		TArray<TSharedPtr<FJsonValue>> ScriptArray;
		int32 ErrorCount = 0;
		int32 WarningCount = 0;

		for (UNiagaraScript* Script : Scripts)
		{
			if (!Script)
			{
				continue;
			}

			const ENiagaraScriptCompileStatus ScriptStatus = Script->GetLastCompileStatus();
			TSharedPtr<FJsonObject> ScriptObject = MakeShared<FJsonObject>();
			ScriptObject->SetStringField(TEXT("script_path"), Script->GetPathName());
			ScriptObject->SetStringField(TEXT("compile_status"), CompileStatusToString(ScriptStatus));
			ScriptObject->SetBoolField(TEXT("is_synchronized"), Script->AreScriptAndSourceSynchronized());

			TArray<TSharedPtr<FJsonValue>> EventArray;
			bool bScriptHasErrorEvent = false;
			bool bScriptHasWarningEvent = false;
			for (const FNiagaraCompileEvent& CompileEvent : Script->GetVMExecutableData().LastCompileEvents)
			{
				TSharedPtr<FJsonObject> EventObject = MakeShared<FJsonObject>();
				FString SeverityString = TEXT("info");
				switch (CompileEvent.Severity)
				{
				case FNiagaraCompileEventSeverity::Error:
					SeverityString = TEXT("error");
					++ErrorCount;
					bScriptHasErrorEvent = true;
					break;
				case FNiagaraCompileEventSeverity::Warning:
					SeverityString = TEXT("warning");
					++WarningCount;
					bScriptHasWarningEvent = true;
					break;
				default:
					break;
				}

				EventObject->SetStringField(TEXT("severity"), SeverityString);
				EventObject->SetStringField(TEXT("message"), CompileEvent.Message);
				if (CompileEvent.NodeGuid.IsValid())
				{
					EventObject->SetStringField(TEXT("node_guid"), CompileEvent.NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				}
				if (CompileEvent.PinGuid.IsValid())
				{
					EventObject->SetStringField(TEXT("pin_guid"), CompileEvent.PinGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				}
				EventArray.Add(MakeShared<FJsonValueObject>(EventObject));
			}

			if (ScriptStatus == ENiagaraScriptCompileStatus::NCS_Error && !bScriptHasErrorEvent)
			{
				TSharedPtr<FJsonObject> EventObject = MakeShared<FJsonObject>();
				EventObject->SetStringField(TEXT("severity"), TEXT("error"));
				EventObject->SetStringField(
					TEXT("message"),
					TEXT("Niagara script compile status reports errors, but no compile error event was exposed. Check the Niagara Log before continuing."));
				EventArray.Add(MakeShared<FJsonValueObject>(EventObject));
				++ErrorCount;
			}

			if (IsCompileStatusWarning(ScriptStatus) && !bScriptHasWarningEvent)
			{
				TSharedPtr<FJsonObject> EventObject = MakeShared<FJsonObject>();
				EventObject->SetStringField(TEXT("severity"), TEXT("warning"));
				EventObject->SetStringField(
					TEXT("message"),
					TEXT("Niagara script compile status reports warnings, but no compile warning event was exposed. Check the Niagara Log and fix warnings before continuing."));
				EventArray.Add(MakeShared<FJsonValueObject>(EventObject));
				++WarningCount;
			}

			ScriptObject->SetArrayField(TEXT("compile_events"), EventArray);
			ScriptArray.Add(MakeShared<FJsonValueObject>(ScriptObject));
		}

		Diagnostics->SetNumberField(TEXT("error_count"), ErrorCount);
		Diagnostics->SetNumberField(TEXT("warning_count"), WarningCount);
		Diagnostics->SetBoolField(TEXT("log_checked"), true);
		Diagnostics->SetBoolField(TEXT("has_errors"), ErrorCount > 0 || LatestCompileStatus == ENiagaraScriptCompileStatus::NCS_Error);
		Diagnostics->SetBoolField(TEXT("has_warnings"), WarningCount > 0 || IsCompileStatusWarning(LatestCompileStatus));
		Diagnostics->SetNumberField(TEXT("blocking_issue_count"), ErrorCount + WarningCount);
		Diagnostics->SetStringField(TEXT("guardrail"), TEXT("clean_niagara_compile_log_required"));
		Diagnostics->SetArrayField(TEXT("scripts"), ScriptArray);
		return Diagnostics;
	}

	void CollectModuleItemsRecursive(UNiagaraStackEntry* Entry, TArray<UNiagaraStackModuleItem*>& OutModules)
	{
		if (!Entry)
		{
			return;
		}

		if (UNiagaraStackModuleItem* ModuleItem = Cast<UNiagaraStackModuleItem>(Entry))
		{
			OutModules.Add(ModuleItem);
		}

		TArray<UNiagaraStackEntry*> Children;
		Entry->GetUnfilteredChildren(Children);
		for (UNiagaraStackEntry* Child : Children)
		{
			CollectModuleItemsRecursive(Child, OutModules);
		}
	}
}

FString UsageToToolStageString(int32 UsageValue)
{
	return UsageToToolStageStringInternal(static_cast<ENiagaraScriptUsage>(UsageValue));
}

bool ResolveEmitterContext(
	const FString& SystemPath,
	const TSharedPtr<FJsonObject>& Input,
	FEmitterContext& OutContext,
	FString& OutErrorCode,
	FString& OutErrorMessage)
{
	OutContext = FEmitterContext();

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		OutErrorCode = TEXT("NI_SYSTEM_NOT_FOUND");
		OutErrorMessage = FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath);
		return false;
	}

	FNiagaraSystemViewModelOptions Options;
	Options.bIsForDataProcessingOnly = true;
	Options.bCanAutoCompile = false;
	Options.bCanSimulate = false;
	Options.bCanModifyEmittersFromTimeline = false;
	Options.MessageLogGuid = NiagaraSystem->GetAssetGuid();

	OutContext.System = NiagaraSystem;
	OutContext.SystemViewModel = MakeShared<FNiagaraSystemViewModel>();
	OutContext.SystemViewModel->Initialize(*NiagaraSystem, Options);
	if (!OutContext.SystemViewModel.IsValid())
	{
		OutErrorCode = TEXT("NI_VIEWMODEL_INIT_FAILED");
		OutErrorMessage = FString::Printf(TEXT("Failed to initialize Niagara view model for '%s'."), *SystemPath);
		return false;
	}

	int32 EmitterIndex = INDEX_NONE;
	FString EmitterName;
	double EmitterIndexNumber = 0.0;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("emitter_name"), EmitterName);
		if (Input->TryGetNumberField(TEXT("emitter_index"), EmitterIndexNumber))
		{
			EmitterIndex = static_cast<int32>(EmitterIndexNumber);
		}
	}

	const TArray<TSharedRef<FNiagaraEmitterHandleViewModel>>& EmitterHandleViewModels = OutContext.SystemViewModel->GetEmitterHandleViewModels();
	if (EmitterHandleViewModels.Num() == 0)
	{
		OutErrorCode = TEXT("NI_NO_EMITTERS");
		OutErrorMessage = FString::Printf(TEXT("Niagara System '%s' has no emitters."), *NiagaraSystem->GetName());
		return false;
	}

	if (EmitterIndex != INDEX_NONE)
	{
		if (!EmitterHandleViewModels.IsValidIndex(EmitterIndex))
		{
			OutErrorCode = TEXT("NI_EMITTER_INDEX_OUT_OF_RANGE");
			OutErrorMessage = FString::Printf(TEXT("Emitter index %d is out of range for '%s'."), EmitterIndex, *SystemPath);
			return false;
		}
		OutContext.EmitterHandleViewModel = EmitterHandleViewModels[EmitterIndex];
	}
	else if (!EmitterName.IsEmpty())
	{
		for (const TSharedRef<FNiagaraEmitterHandleViewModel>& Candidate : EmitterHandleViewModels)
		{
			if (Candidate->GetName().ToString().Equals(EmitterName, ESearchCase::IgnoreCase))
			{
				OutContext.EmitterHandleViewModel = Candidate;
				break;
			}
		}

		if (!OutContext.EmitterHandleViewModel.IsValid())
		{
			OutErrorCode = TEXT("NI_EMITTER_NOT_FOUND");
			OutErrorMessage = FString::Printf(TEXT("Emitter '%s' was not found in '%s'."), *EmitterName, *SystemPath);
			return false;
		}
	}
	else
	{
		OutContext.EmitterHandleViewModel = EmitterHandleViewModels[0];
	}

	OutContext.EmitterIndex = EmitterHandleViewModels.IndexOfByPredicate(
		[&](const TSharedRef<FNiagaraEmitterHandleViewModel>& Candidate)
		{
			return Candidate->GetId() == OutContext.EmitterHandleViewModel->GetId();
		});
	OutContext.EmitterHandle = OutContext.EmitterHandleViewModel->GetEmitterHandle();
	OutContext.EmitterData = OutContext.EmitterHandle ? OutContext.EmitterHandle->GetEmitterData() : nullptr;
	OutContext.StackViewModel = OutContext.EmitterHandleViewModel->GetEmitterStackViewModel();

	if (!OutContext.EmitterHandle || !OutContext.EmitterData || !OutContext.StackViewModel)
	{
		OutErrorCode = TEXT("NI_EMITTER_CONTEXT_INVALID");
		OutErrorMessage = FString::Printf(TEXT("Emitter context for '%s' could not be initialized."), *SystemPath);
		return false;
	}

	if (UNiagaraStackEntry* RootEntry = OutContext.StackViewModel->GetRootEntry())
	{
		RootEntry->RefreshChildren();
	}

	return true;
}

void CollectModuleSummaries(FEmitterContext& Context, TArray<FStackModuleSummary>& OutModules)
{
	OutModules.Reset();
	if (!Context.StackViewModel || !Context.SystemViewModel.IsValid() || !Context.EmitterHandle)
	{
		return;
	}

	TArray<UNiagaraStackModuleItem*> ModuleItems;
	if (UNiagaraStackEntry* RootEntry = Context.StackViewModel->GetRootEntry())
	{
		CollectModuleItemsRecursive(RootEntry, ModuleItems);
	}

	for (UNiagaraStackModuleItem* ModuleItem : ModuleItems)
	{
		if (!ModuleItem)
		{
			continue;
		}

		UNiagaraNodeFunctionCall& ModuleNode = ModuleItem->GetModuleNode();
		UNiagaraNodeAssignment* AssignmentNode = Cast<UNiagaraNodeAssignment>(&ModuleNode);
		FStackModuleSummary Summary;
		Summary.ModuleItem = ModuleItem;
		Summary.ModuleNode = &ModuleNode;
		Summary.ModuleKey = ModuleNode.NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower);
		Summary.DisplayName = ModuleItem->GetDisplayName().ToString();
		Summary.ScriptAssetPath = ModuleNode.FunctionScript ? ModuleNode.FunctionScript->GetPathName() : TEXT("");

		if (UNiagaraNodeOutput* OutputNode = ModuleItem->GetOutputNode())
		{
			Summary.StageUsage = UsageToToolStageStringInternal(OutputNode->GetUsage());
			Summary.UsageId = OutputNode->GetUsageId().ToString(EGuidFormats::DigitsWithHyphensLower);
			Summary.StageIndex = ModuleItem->GetModuleIndex();
		}

		TArray<UNiagaraStackFunctionInput*> Inputs;
		ModuleItem->GetParameterInputs(Inputs);
		for (UNiagaraStackFunctionInput* Input : Inputs)
		{
			if (!Input)
			{
				continue;
			}

			if (AssignmentNode)
			{
				const TArray<FNiagaraVariable>& AssignmentTargets = AssignmentNode->GetAssignmentTargets();
				const TArray<FString>& AssignmentDefaults = AssignmentNode->GetAssignmentDefaults();
				const int32 SummaryStartIndex = Summary.Inputs.Num();
				AppendInputSummaryRecursive(*Input, FString(), Summary.Inputs);
				for (int32 SummaryIndex = SummaryStartIndex; SummaryIndex < Summary.Inputs.Num(); ++SummaryIndex)
				{
					FStackInputSummary& InputSummary = Summary.Inputs[SummaryIndex];
					for (int32 AssignmentIndex = 0; AssignmentIndex < AssignmentTargets.Num() && AssignmentIndex < AssignmentDefaults.Num(); ++AssignmentIndex)
					{
						const FString AssignmentTargetName = AssignmentTargets[AssignmentIndex].GetName().ToString();
						if (InputSummary.DisplayName.Equals(AssignmentTargetName, ESearchCase::IgnoreCase) ||
							InputSummary.InputKey.EndsWith(AssignmentTargetName, ESearchCase::IgnoreCase))
						{
							const FString& AssignmentDefaultValue = AssignmentDefaults[AssignmentIndex];
							if (!AssignmentDefaultValue.IsEmpty())
							{
								InputSummary.ValueSource = LooksLikeNiagaraExpressionString(AssignmentDefaultValue)
									? TEXT("expression")
									: TEXT("local");
								InputSummary.ValueString = AssignmentDefaultValue;
								InputSummary.bCanReset = true;
							}
							break;
						}
					}
				}
			}
			else
			{
				AppendInputSummaryRecursive(*Input, FString(), Summary.Inputs);
			}
		}

		OutModules.Add(Summary);
	}
}

UNiagaraStackModuleItem* FindModuleByKey(
	FEmitterContext& Context,
	const FString& ModuleKey,
	FStackModuleSummary* OutSummary)
{
	TArray<FStackModuleSummary> Modules;
	CollectModuleSummaries(Context, Modules);
	for (FStackModuleSummary& Summary : Modules)
	{
		if (Summary.ModuleKey.Equals(ModuleKey, ESearchCase::IgnoreCase))
		{
			if (OutSummary)
			{
				*OutSummary = Summary;
			}
			return Summary.ModuleItem;
		}
	}
	return nullptr;
}

UNiagaraStackFunctionInput* FindInputByKey(
	FEmitterContext& Context,
	const FString& ModuleKey,
	const FString& InputKey,
	FStackModuleSummary* OutModuleSummary)
{
	FStackModuleSummary ModuleSummary;
	UNiagaraStackModuleItem* ModuleItem = FindModuleByKey(Context, ModuleKey, &ModuleSummary);
	if (!ModuleItem)
	{
		return nullptr;
	}

	if (OutModuleSummary)
	{
		*OutModuleSummary = ModuleSummary;
	}

	TArray<UNiagaraStackFunctionInput*> Inputs;
	ModuleItem->GetParameterInputs(Inputs);
	for (UNiagaraStackFunctionInput* Input : Inputs)
	{
		if (!Input)
		{
			continue;
		}

		if (UNiagaraStackFunctionInput* Match = FindInputByKeyRecursive(*Input, InputKey, FString()))
		{
			return Match;
		}
	}
	return nullptr;
}

UNiagaraStackFunctionInput* FindInputByHeuristics(
	FEmitterContext& Context,
	const FString& ModuleKey,
	const TArray<FString>& CandidateTokens,
	FString& OutMatchedInputKey,
	FStackModuleSummary* OutModuleSummary)
{
	FStackModuleSummary ModuleSummary;
	UNiagaraStackModuleItem* ModuleItem = FindModuleByKey(Context, ModuleKey, &ModuleSummary);
	if (!ModuleItem)
	{
		return nullptr;
	}

	if (OutModuleSummary)
	{
		*OutModuleSummary = ModuleSummary;
	}

	TArray<UNiagaraStackFunctionInput*> Inputs;
	ModuleItem->GetParameterInputs(Inputs);
	int32 BestScore = MIN_int32;
	UNiagaraStackFunctionInput* BestInput = nullptr;
	FString BestInputKey;
	for (UNiagaraStackFunctionInput* Input : Inputs)
	{
		if (!Input)
		{
			continue;
		}

		const FString RawInputKey = Input->GetInputParameterHandle().GetParameterHandleString().ToString();
		const FString DisplayName = NormalizeToken(Input->GetDisplayName().ToString());
		const FString HandleName = NormalizeToken(RawInputKey);
		const FString TypeName = NormalizeToken(Input->GetInputType().GetName());
		for (const FString& Token : CandidateTokens)
		{
			const FString NormalizedToken = NormalizeToken(Token);
			int32 Score = MIN_int32;
			if (DisplayName.Equals(NormalizedToken) || HandleName.Equals(NormalizedToken))
			{
				Score = 100;
			}
			else if (DisplayName.EndsWith(NormalizedToken) || HandleName.EndsWith(NormalizedToken))
			{
				Score = 80;
			}
			else if (DisplayName.Contains(NormalizedToken) || HandleName.Contains(NormalizedToken))
			{
				Score = 50;
			}

			if (Score == MIN_int32)
			{
				continue;
			}

			const bool bTokenTargetsMode = NormalizedToken.Contains(TEXT("mode"));
			const bool bInputLooksLikeMode = DisplayName.Contains(TEXT("mode")) || HandleName.Contains(TEXT("mode"));
			if (!bTokenTargetsMode && bInputLooksLikeMode)
			{
				Score -= 40;
			}

			if (!bTokenTargetsMode && (TypeName.Contains(TEXT("niagaraint32")) || TypeName.Contains(TEXT("enum"))))
			{
				Score -= 15;
			}

			if (HandleName.Contains(TEXT("module")))
			{
				Score += 5;
			}

			if (Score > BestScore)
			{
				BestScore = Score;
				BestInput = Input;
				BestInputKey = RawInputKey;
			}
		}
	}

	if (BestInput)
	{
		OutMatchedInputKey = BestInputKey;
	}
	return BestInput;
}

TSharedPtr<FJsonValue> SerializeInputValue(const UNiagaraStackFunctionInput& Input)
{
	switch (Input.GetValueMode())
	{
	case UNiagaraStackFunctionInput::EValueMode::Local:
		if (TSharedPtr<const FStructOnScope> LocalValueStruct = const_cast<UNiagaraStackFunctionInput&>(Input).GetLocalValueStruct())
		{
			const void* StructMemory = LocalValueStruct->GetStructMemory();
			const FNiagaraTypeDefinition InputType = Input.GetInputType();
			if (StructMemory == nullptr)
			{
				return nullptr;
			}

			if (InputType == FNiagaraTypeDefinition::GetFloatDef())
			{
				return MakeShared<FJsonValueNumber>(reinterpret_cast<const FNiagaraFloat*>(StructMemory)->Value);
			}
			if (InputType == FNiagaraTypeDefinition::GetIntDef())
			{
				return MakeShared<FJsonValueNumber>(reinterpret_cast<const FNiagaraInt32*>(StructMemory)->Value);
			}
			if (InputType == FNiagaraTypeDefinition::GetBoolDef())
			{
				return MakeShared<FJsonValueBoolean>(reinterpret_cast<const FNiagaraBool*>(StructMemory)->GetValue());
			}
			if (InputType == FNiagaraTypeDefinition::GetVec2Def())
			{
				const FVector2f& VectorValue = *reinterpret_cast<const FVector2f*>(StructMemory);
				TArray<TSharedPtr<FJsonValue>> Values;
				Values.Add(MakeShared<FJsonValueNumber>(VectorValue.X));
				Values.Add(MakeShared<FJsonValueNumber>(VectorValue.Y));
				return MakeShared<FJsonValueArray>(Values);
			}
			if (InputType == FNiagaraTypeDefinition::GetVec3Def() || InputType == FNiagaraTypeDefinition::GetPositionDef())
			{
				const FVector3f& VectorValue = *reinterpret_cast<const FVector3f*>(StructMemory);
				TArray<TSharedPtr<FJsonValue>> Values;
				Values.Add(MakeShared<FJsonValueNumber>(VectorValue.X));
				Values.Add(MakeShared<FJsonValueNumber>(VectorValue.Y));
				Values.Add(MakeShared<FJsonValueNumber>(VectorValue.Z));
				return MakeShared<FJsonValueArray>(Values);
			}
			if (InputType == FNiagaraTypeDefinition::GetColorDef())
			{
				const FLinearColor& ColorValue = *reinterpret_cast<const FLinearColor*>(StructMemory);
				TArray<TSharedPtr<FJsonValue>> Values;
				Values.Add(MakeShared<FJsonValueNumber>(ColorValue.R));
				Values.Add(MakeShared<FJsonValueNumber>(ColorValue.G));
				Values.Add(MakeShared<FJsonValueNumber>(ColorValue.B));
				Values.Add(MakeShared<FJsonValueNumber>(ColorValue.A));
				return MakeShared<FJsonValueArray>(Values);
			}
		}
		return nullptr;
	case UNiagaraStackFunctionInput::EValueMode::Linked:
		return MakeShared<FJsonValueString>(Input.GetLinkedParameterValue().GetName().ToString());
	case UNiagaraStackFunctionInput::EValueMode::Dynamic:
		if (UNiagaraNodeFunctionCall* DynamicNode = Input.GetDynamicInputNode())
		{
			return MakeShared<FJsonValueString>(DynamicNode->GetNodeTitle(ENodeTitleType::ListView).ToString());
		}
		return MakeShared<FJsonValueString>(TEXT("dynamic_input"));
	case UNiagaraStackFunctionInput::EValueMode::Expression:
		return MakeShared<FJsonValueString>(Input.GetCustomExpressionText().ToString());
	case UNiagaraStackFunctionInput::EValueMode::ObjectAsset:
		if (UObject* AssetValue = const_cast<UNiagaraStackFunctionInput&>(Input).GetObjectAssetValue())
		{
			return MakeShared<FJsonValueString>(AssetValue->GetPathName());
		}
		return MakeShared<FJsonValueString>(TEXT(""));
	default:
		break;
	}

	return nullptr;
}

TSharedPtr<FJsonObject> BuildModuleJsonObject(const FStackModuleSummary& ModuleSummary, bool bIncludeInputs)
{
	TSharedPtr<FJsonObject> ModuleObject = MakeShared<FJsonObject>();
	ModuleObject->SetStringField(TEXT("module_key"), ModuleSummary.ModuleKey);
	ModuleObject->SetStringField(TEXT("display_name"), ModuleSummary.DisplayName);
	ModuleObject->SetStringField(TEXT("script_asset_path"), ModuleSummary.ScriptAssetPath);
	ModuleObject->SetStringField(TEXT("stage_usage"), ModuleSummary.StageUsage);
	ModuleObject->SetStringField(TEXT("usage_id"), ModuleSummary.UsageId);
	ModuleObject->SetNumberField(TEXT("stage_index"), ModuleSummary.StageIndex);

	if (bIncludeInputs)
	{
		TArray<TSharedPtr<FJsonValue>> InputArray;
		for (const FStackInputSummary& InputSummary : ModuleSummary.Inputs)
		{
			TSharedPtr<FJsonObject> InputObject = MakeShared<FJsonObject>();
			InputObject->SetStringField(TEXT("input_key"), InputSummary.InputKey);
			InputObject->SetStringField(TEXT("display_name"), InputSummary.DisplayName);
			InputObject->SetStringField(TEXT("type_name"), InputSummary.TypeName);
			InputObject->SetStringField(TEXT("value_source"), InputSummary.ValueSource);
			InputObject->SetStringField(TEXT("value_string"), InputSummary.ValueString);
			InputObject->SetBoolField(TEXT("has_editable_local_value"), InputSummary.bHasEditableLocalValue);
			InputObject->SetBoolField(TEXT("can_reset"), InputSummary.bCanReset);
			InputArray.Add(MakeShared<FJsonValueObject>(InputObject));
		}
		ModuleObject->SetArrayField(TEXT("inputs"), InputArray);
	}

	return ModuleObject;
}

TSharedPtr<FJsonObject> BuildStandardModulePresenceObject(const TArray<FStackModuleSummary>& Modules)
{
	TArray<TPair<FString, TArray<FString>>> PresenceChecks;
	PresenceChecks.Emplace(TEXT("spawn_rate"), TArray<FString>{ TEXT("spawnrate"), TEXT("spawn rate") });
	PresenceChecks.Emplace(TEXT("initialize_particle"), TArray<FString>{ TEXT("initializeparticle"), TEXT("initialize particle") });
	PresenceChecks.Emplace(TEXT("scale_color"), TArray<FString>{ TEXT("scalecolor"), TEXT("scale color") });
	PresenceChecks.Emplace(TEXT("add_velocity"), TArray<FString>{ TEXT("addvelocity"), TEXT("add velocity") });
	PresenceChecks.Emplace(TEXT("add_velocity_in_cone"), TArray<FString>{ TEXT("addvelocityincone"), TEXT("add velocity in cone") });
	PresenceChecks.Emplace(TEXT("gravity_force"), TArray<FString>{ TEXT("gravityforce"), TEXT("gravity force") });
	PresenceChecks.Emplace(TEXT("drag"), TArray<FString>{ TEXT("drag") });
	PresenceChecks.Emplace(TEXT("curl_noise_force"), TArray<FString>{ TEXT("curlnoiseforce"), TEXT("curl noise force") });
	PresenceChecks.Emplace(TEXT("sphere_location"), TArray<FString>{ TEXT("spherelocation"), TEXT("sphere location") });
	PresenceChecks.Emplace(TEXT("torus_location"), TArray<FString>{ TEXT("toruslocation"), TEXT("torus location") });
	PresenceChecks.Emplace(TEXT("initialize_ribbon"), TArray<FString>{ TEXT("initializeribbon"), TEXT("initialize ribbon") });
	PresenceChecks.Emplace(TEXT("point_attraction_force"), TArray<FString>{ TEXT("pointattractionforce"), TEXT("point attraction force") });
	PresenceChecks.Emplace(TEXT("vortex_force"), TArray<FString>{ TEXT("vortexforce"), TEXT("vortex force") });
	PresenceChecks.Emplace(TEXT("vortex_velocity"), TArray<FString>{ TEXT("vortexvelocity"), TEXT("vortex velocity") });
	PresenceChecks.Emplace(TEXT("scale_ribbon_width"), TArray<FString>{ TEXT("scaleribbonwidth"), TEXT("scale ribbon width") });
	PresenceChecks.Emplace(TEXT("solve_forces_and_velocity"), TArray<FString>{ TEXT("solveforcesandvelocity"), TEXT("solve forces and velocity") });

	TSharedPtr<FJsonObject> PresenceObject = MakeShared<FJsonObject>();
	for (const TPair<FString, TArray<FString>>& Check : PresenceChecks)
	{
		bool bFound = false;
		for (const FStackModuleSummary& ModuleSummary : Modules)
		{
			const FString DisplayName = NormalizeToken(ModuleSummary.DisplayName);
			const FString ScriptAssetPath = NormalizeToken(ModuleSummary.ScriptAssetPath);
			for (const FString& Token : Check.Value)
			{
				const FString NormalizedToken = NormalizeToken(Token);
				if (DisplayName.Contains(NormalizedToken) || ScriptAssetPath.Contains(NormalizedToken))
				{
					bFound = true;
					break;
				}
			}
			if (bFound)
			{
				break;
			}
		}
		PresenceObject->SetBoolField(Check.Key, bFound);
	}

	return PresenceObject;
}

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

	const TCHAR* DynUniformFloatPath = TEXT("/Niagara/DynamicInputs/UniformRange/UniformRangedFloat.UniformRangedFloat");
	const TCHAR* DynUniformIntPath = TEXT("/Niagara/DynamicInputs/UniformRange/UniformRangedInt.UniformRangedInt");
	const TCHAR* DynUniformVec2Path = TEXT("/Niagara/DynamicInputs/UniformRange/UniformRangedVector2D.UniformRangedVector2D");
	const TCHAR* DynUniformVec3Path = TEXT("/Niagara/DynamicInputs/UniformRange/UniformRangedVector.UniformRangedVector");
	const TCHAR* DynUniformColorPath = TEXT("/Niagara/DynamicInputs/UniformRange/UniformRangedLinearColor.UniformRangedLinearColor");
	const TCHAR* DynFloatFromCurvePath = TEXT("/Niagara/DynamicInputs/ValueFromCurve/FloatFromCurve.FloatFromCurve");
	const TCHAR* DynScaleVectorByCurvePath = TEXT("/Niagara/DynamicInputs/ScaleByCurve/ScaleVectorByCurve.ScaleVectorByCurve");

	bool SaveNiagaraAsset(UNiagaraSystem* NiagaraSystem)
	{
		if (!NiagaraSystem)
		{
			return false;
		}

		if (UEditorAssetLibrary::SaveLoadedAsset(NiagaraSystem, true))
		{
			return true;
		}

		UPackage* Package = NiagaraSystem->GetOutermost();
		if (!Package)
		{
			return false;
		}

		Package->MarkPackageDirty();

		TArray<UPackage*> PackagesToSave;
		PackagesToSave.Add(Package);
		if (UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, false))
		{
			return true;
		}

		const FString PackageName = Package->GetName();
		if (PackageName.IsEmpty())
		{
			return false;
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;
		const FString PackageFilename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		return UPackage::SavePackage(Package, NiagaraSystem, *PackageFilename, SaveArgs);
	}

	UNiagaraNodeOutput* FindOutputNodeForScript(UNiagaraScript* Script)
	{
		if (!Script)
		{
			return nullptr;
		}

		UNiagaraScriptSource* ScriptSource = Cast<UNiagaraScriptSource>(Script->GetLatestSource());
		if (!ScriptSource || !ScriptSource->NodeGraph)
		{
			return nullptr;
		}

		TArray<UNiagaraNodeOutput*> OutputNodes;
		ScriptSource->NodeGraph->GetNodesOfClass<UNiagaraNodeOutput>(OutputNodes);
		const ENiagaraScriptUsage TargetUsage = Script->GetUsage();
		const FGuid TargetUsageId = Script->GetUsageId();
		for (UNiagaraNodeOutput* OutputNode : OutputNodes)
		{
			if (!OutputNode)
			{
				continue;
			}

			if (UNiagaraScript::IsEquivalentUsage(OutputNode->GetUsage(), TargetUsage))
			{
				if (!TargetUsageId.IsValid() || OutputNode->GetUsageId() == TargetUsageId)
				{
					return OutputNode;
				}
			}
		}

		return OutputNodes.Num() > 0 ? OutputNodes[0] : nullptr;
	}

	UNiagaraNodeOutput* FindExistingStageOutputNode(FEmitterContext& Context, const FString& StageUsage)
	{
		TArray<FStackModuleSummary> Modules;
		CollectModuleSummaries(Context, Modules);
		for (const FStackModuleSummary& ModuleSummary : Modules)
		{
			if (ModuleSummary.StageUsage.Equals(StageUsage, ESearchCase::IgnoreCase) &&
				ModuleSummary.ModuleItem != nullptr &&
				ModuleSummary.ModuleItem->GetOutputNode() != nullptr)
			{
				return ModuleSummary.ModuleItem->GetOutputNode();
			}
		}
		return nullptr;
	}

	bool ResolveStageTarget(
		FEmitterContext& Context,
		const FString& StageUsage,
		UNiagaraScript*& OutScript,
		UNiagaraNodeOutput*& OutOutputNode,
		ENiagaraScriptUsage& OutUsage,
		FGuid& OutUsageId,
		FString& OutErrorMessage)
	{
		OutScript = nullptr;
		OutOutputNode = nullptr;
		OutUsage = ENiagaraScriptUsage::Function;
		OutUsageId.Invalidate();

		const FString NormalizedStage = NormalizeToken(StageUsage);
		if (NormalizedStage == StageEmitterSpawn)
		{
			OutScript = Context.EmitterData->EmitterSpawnScriptProps.Script;
			OutUsage = ENiagaraScriptUsage::EmitterSpawnScript;
		}
		else if (NormalizedStage == StageEmitterUpdate)
		{
			OutScript = Context.EmitterData->EmitterUpdateScriptProps.Script;
			OutUsage = ENiagaraScriptUsage::EmitterUpdateScript;
		}
		else if (NormalizedStage == StageParticleSpawn)
		{
			OutScript = Context.EmitterData->SpawnScriptProps.Script;
			OutUsage = ENiagaraScriptUsage::ParticleSpawnScript;
		}
		else if (NormalizedStage == StageParticleUpdate)
		{
			OutScript = Context.EmitterData->UpdateScriptProps.Script;
			OutUsage = ENiagaraScriptUsage::ParticleUpdateScript;
		}
		else
		{
			OutErrorMessage = FString::Printf(TEXT("Unsupported or unavailable stage_usage '%s'."), *StageUsage);
			return false;
		}

		if (!OutScript)
		{
			OutErrorMessage = FString::Printf(TEXT("Stage '%s' does not have a Niagara script."), *StageUsage);
			return false;
		}

		OutOutputNode = FindExistingStageOutputNode(Context, NormalizedStage);
		if (OutOutputNode == nullptr)
		{
			OutOutputNode = FindOutputNodeForScript(OutScript);
		}
		if (!OutOutputNode)
		{
			OutErrorMessage = FString::Printf(TEXT("Could not find output node for stage '%s'."), *StageUsage);
			return false;
		}

		OutUsageId = OutOutputNode->GetUsageId();
		return true;
	}

	bool CopyJsonValueToStructMemory(
		const FNiagaraTypeDefinition& InputType,
		const TSharedPtr<FJsonValue>& ValueJson,
		void* StructMemory,
		FString& OutAppliedValueString,
		FString& OutErrorMessage)
	{
		auto TryReadNumericJson = [&OutErrorMessage](const TSharedPtr<FJsonValue>& InValue, const FString& ExpectedTypeLabel, double& OutNumber) -> bool
		{
			if (!InValue.IsValid())
			{
				OutErrorMessage = TEXT("Missing numeric value payload.");
				return false;
			}

			if (InValue->Type != EJson::Number)
			{
				OutErrorMessage = FString::Printf(TEXT("Type '%s' expects a numeric JSON value."), *ExpectedTypeLabel);
				return false;
			}

			OutNumber = InValue->AsNumber();
			return true;
		};

		if (!ValueJson.IsValid())
		{
			OutErrorMessage = TEXT("Missing value payload.");
			return false;
		}

		if (InputType == FNiagaraTypeDefinition::GetFloatDef())
		{
			double NumericValueRaw = 0.0;
			if (!TryReadNumericJson(ValueJson, NiagaraTypeToString(InputType), NumericValueRaw))
			{
				return false;
			}
			const float NumericValue = static_cast<float>(NumericValueRaw);
			reinterpret_cast<FNiagaraFloat*>(StructMemory)->Value = NumericValue;
			OutAppliedValueString = FString::SanitizeFloat(NumericValue);
			return true;
		}

		if (InputType == FNiagaraTypeDefinition::GetIntDef()
			|| InputType.GetStruct() == FNiagaraInt32::StaticStruct())
		{
			double IntValueRaw = 0.0;
			if (!TryReadNumericJson(ValueJson, NiagaraTypeToString(InputType), IntValueRaw))
			{
				return false;
			}
			const int32 IntValue = static_cast<int32>(IntValueRaw);
			reinterpret_cast<FNiagaraInt32*>(StructMemory)->Value = IntValue;
			OutAppliedValueString = FString::FromInt(IntValue);
			return true;
		}

		if (InputType == FNiagaraTypeDefinition::GetBoolDef())
		{
			bool bValue = false;
			if (ValueJson->Type == EJson::Boolean)
			{
				bValue = ValueJson->AsBool();
			}
			else if (ValueJson->Type == EJson::String)
			{
				bValue = ValueJson->AsString().Equals(TEXT("true"), ESearchCase::IgnoreCase);
			}
			else
			{
				bValue = !FMath::IsNearlyZero(static_cast<float>(ValueJson->AsNumber()));
			}
			reinterpret_cast<FNiagaraBool*>(StructMemory)->SetValue(bValue);
			OutAppliedValueString = bValue ? TEXT("true") : TEXT("false");
			return true;
		}

		const TArray<TSharedPtr<FJsonValue>>* ValueArray = nullptr;
		if (!ValueJson->TryGetArray(ValueArray))
		{
			OutErrorMessage = FString::Printf(TEXT("Type '%s' expects an array value."), *NiagaraTypeToString(InputType));
			return false;
		}

		if (InputType == FNiagaraTypeDefinition::GetVec2Def())
		{
			if (ValueArray->Num() < 2)
			{
				OutErrorMessage = TEXT("vector2 requires [x, y].");
				return false;
			}
			FVector2f& VectorValue = *reinterpret_cast<FVector2f*>(StructMemory);
			double X = 0.0;
			double Y = 0.0;
			if (!TryReadNumericJson((*ValueArray)[0], NiagaraTypeToString(InputType), X) ||
				!TryReadNumericJson((*ValueArray)[1], NiagaraTypeToString(InputType), Y))
			{
				return false;
			}
			VectorValue.X = static_cast<float>(X);
			VectorValue.Y = static_cast<float>(Y);
			OutAppliedValueString = FString::Printf(TEXT("[%.4f, %.4f]"), VectorValue.X, VectorValue.Y);
			return true;
		}

		if (InputType == FNiagaraTypeDefinition::GetVec3Def() || InputType == FNiagaraTypeDefinition::GetPositionDef())
		{
			if (ValueArray->Num() < 3)
			{
				OutErrorMessage = TEXT("vector3 requires [x, y, z].");
				return false;
			}
			FVector3f& VectorValue = *reinterpret_cast<FVector3f*>(StructMemory);
			double X = 0.0;
			double Y = 0.0;
			double Z = 0.0;
			if (!TryReadNumericJson((*ValueArray)[0], NiagaraTypeToString(InputType), X) ||
				!TryReadNumericJson((*ValueArray)[1], NiagaraTypeToString(InputType), Y) ||
				!TryReadNumericJson((*ValueArray)[2], NiagaraTypeToString(InputType), Z))
			{
				return false;
			}
			VectorValue.X = static_cast<float>(X);
			VectorValue.Y = static_cast<float>(Y);
			VectorValue.Z = static_cast<float>(Z);
			OutAppliedValueString = FString::Printf(TEXT("[%.4f, %.4f, %.4f]"), VectorValue.X, VectorValue.Y, VectorValue.Z);
			return true;
		}

		if (InputType == FNiagaraTypeDefinition::GetColorDef())
		{
			if (ValueArray->Num() < 3)
			{
				OutErrorMessage = TEXT("color requires [r, g, b] or [r, g, b, a].");
				return false;
			}
			FLinearColor& ColorValue = *reinterpret_cast<FLinearColor*>(StructMemory);
			double R = 0.0;
			double G = 0.0;
			double B = 0.0;
			double A = 1.0;
			if (!TryReadNumericJson((*ValueArray)[0], NiagaraTypeToString(InputType), R) ||
				!TryReadNumericJson((*ValueArray)[1], NiagaraTypeToString(InputType), G) ||
				!TryReadNumericJson((*ValueArray)[2], NiagaraTypeToString(InputType), B))
			{
				return false;
			}
			if (ValueArray->Num() > 3 && !TryReadNumericJson((*ValueArray)[3], NiagaraTypeToString(InputType), A))
			{
				return false;
			}
			ColorValue.R = static_cast<float>(R);
			ColorValue.G = static_cast<float>(G);
			ColorValue.B = static_cast<float>(B);
			ColorValue.A = static_cast<float>(A);
			OutAppliedValueString = FString::Printf(TEXT("[%.4f, %.4f, %.4f, %.4f]"), ColorValue.R, ColorValue.G, ColorValue.B, ColorValue.A);
			return true;
		}

		OutErrorMessage = FString::Printf(TEXT("Unsupported local-value type '%s'."), *NiagaraTypeToString(InputType));
		return false;
	}

	bool SetInputFromJsonField(
		UNiagaraStackFunctionInput& Input,
		const TSharedPtr<FJsonObject>& ToolInput,
		const FString& FieldName,
		FString& OutAppliedValueString,
		FString& OutErrorMessage)
	{
		if (!ToolInput.IsValid())
		{
			OutErrorMessage = TEXT("Input payload is missing.");
			return false;
		}

		const TSharedPtr<FJsonValue> ValueField = ToolInput->TryGetField(FieldName);
		if (!ValueField.IsValid())
		{
			OutErrorMessage = FString::Printf(TEXT("Missing required field: %s"), *FieldName);
			return false;
		}

		const FNiagaraTypeDefinition& InputType = Input.GetInputType();
		if (InputType.GetStruct() == nullptr)
		{
			OutErrorMessage = FString::Printf(TEXT("Input type '%s' does not expose a local struct value."), *NiagaraTypeToString(InputType));
			return false;
		}

		TSharedRef<FStructOnScope> ValueStruct = MakeShared<FStructOnScope>(InputType.GetStruct());
		if (!CopyJsonValueToStructMemory(InputType, ValueField, ValueStruct->GetStructMemory(), OutAppliedValueString, OutErrorMessage))
		{
			return false;
		}

		Input.SetLocalValue(ValueStruct);
		return true;
	}

	int32 GetComponentCountForType(const FNiagaraTypeDefinition& InputType)
	{
		if (InputType == FNiagaraTypeDefinition::GetFloatDef() ||
			InputType == FNiagaraTypeDefinition::GetIntDef() ||
			InputType == FNiagaraTypeDefinition::GetBoolDef())
		{
			return 1;
		}
		if (InputType == FNiagaraTypeDefinition::GetVec2Def())
		{
			return 2;
		}
		if (InputType == FNiagaraTypeDefinition::GetVec3Def() || InputType == FNiagaraTypeDefinition::GetPositionDef())
		{
			return 3;
		}
		if (InputType == FNiagaraTypeDefinition::GetColorDef())
		{
			return 4;
		}
		return 0;
	}

	FString JsonPointValueToLiteral(const TSharedPtr<FJsonValue>& ValueJson, int32 ComponentCount, FString& OutError)
	{
		if (!ValueJson.IsValid())
		{
			OutError = TEXT("Curve point is missing a value.");
			return FString();
		}

		if (ComponentCount == 1)
		{
			if (ValueJson->Type != EJson::Number)
			{
				OutError = TEXT("Curve scalar value must be numeric.");
				return FString();
			}
			return FString::SanitizeFloat(static_cast<float>(ValueJson->AsNumber()));
		}

		const TArray<TSharedPtr<FJsonValue>>* ValueArray = nullptr;
		if (!ValueJson->TryGetArray(ValueArray) || ValueArray->Num() < ComponentCount)
		{
			OutError = FString::Printf(TEXT("Curve point requires %d value components."), ComponentCount);
			return FString();
		}

		TArray<FString> Components;
		for (int32 Index = 0; Index < ComponentCount; ++Index)
		{
			if (!(*ValueArray)[Index].IsValid() || (*ValueArray)[Index]->Type != EJson::Number)
			{
				OutError = TEXT("Curve vector/color values must be numeric.");
				return FString();
			}
			Components.Add(FString::SanitizeFloat(static_cast<float>((*ValueArray)[Index]->AsNumber())));
		}

		if (ComponentCount == 2)
		{
			return FString::Printf(TEXT("float2(%s, %s)"), *Components[0], *Components[1]);
		}
		if (ComponentCount == 3)
		{
			return FString::Printf(TEXT("float3(%s, %s, %s)"), *Components[0], *Components[1], *Components[2]);
		}
		return FString::Printf(TEXT("float4(%s, %s, %s, %s)"), *Components[0], *Components[1], *Components[2], *Components[3]);
	}

	FString BuildPiecewiseCurveExpression(const TArray<TSharedPtr<FJsonValue>>& CurvePoints, int32 ComponentCount, FString& OutError)
	{
		if (CurvePoints.Num() < 2)
		{
			OutError = TEXT("Curve needs at least two points.");
			return FString();
		}

		struct FCurvePoint
		{
			float Time = 0.0f;
			FString Literal;
		};

		TArray<FCurvePoint> Points;
		for (const TSharedPtr<FJsonValue>& PointValue : CurvePoints)
		{
			const TSharedPtr<FJsonObject>* PointObject = nullptr;
			if (!PointValue.IsValid() || !PointValue->TryGetObject(PointObject) || !PointObject->IsValid())
			{
				OutError = TEXT("Curve points must be objects with numeric time and value.");
				return FString();
			}

			double TimeNumber = 0.0;
			if (!(*PointObject)->TryGetNumberField(TEXT("time"), TimeNumber))
			{
				OutError = TEXT("Curve point is missing numeric time.");
				return FString();
			}

			const TSharedPtr<FJsonValue> ValueField = (*PointObject)->TryGetField(TEXT("value"));
			if (!ValueField.IsValid())
			{
				OutError = TEXT("Curve point is missing value.");
				return FString();
			}

			FString LiteralError;
			const FString Literal = JsonPointValueToLiteral(ValueField, ComponentCount, LiteralError);
			if (Literal.IsEmpty())
			{
				OutError = LiteralError;
				return FString();
			}

			FCurvePoint Point;
			Point.Time = static_cast<float>(TimeNumber);
			Point.Literal = Literal;
			Points.Add(Point);
		}

		Points.Sort([](const FCurvePoint& A, const FCurvePoint& B) { return A.Time < B.Time; });

		FString Expression = Points.Last().Literal;
		for (int32 Index = Points.Num() - 2; Index >= 0; --Index)
		{
			const FCurvePoint& Current = Points[Index];
			const FCurvePoint& Next = Points[Index + 1];
			const float Delta = FMath::Max(Next.Time - Current.Time, KINDA_SMALL_NUMBER);
			const FString AlphaExpr = FString::Printf(TEXT("saturate((Particles.NormalizedAge - %.6ff) / %.6ff)"), Current.Time, Delta);
			const FString Segment = FString::Printf(TEXT("lerp(%s, %s, %s)"), *Current.Literal, *Next.Literal, *AlphaExpr);
			Expression = FString::Printf(TEXT("(Particles.NormalizedAge <= %.6ff ? %s : %s)"), Next.Time, *Segment, *Expression);
		}

		return Expression;
	}

	UNiagaraScript* LoadDynamicScriptForType(const FNiagaraTypeDefinition& InputType)
	{
		if (InputType == FNiagaraTypeDefinition::GetFloatDef())
		{
			return Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynUniformFloatPath));
		}
		if (InputType == FNiagaraTypeDefinition::GetIntDef())
		{
			return Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynUniformIntPath));
		}
		if (InputType == FNiagaraTypeDefinition::GetVec2Def())
		{
			return Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynUniformVec2Path));
		}
		if (InputType == FNiagaraTypeDefinition::GetVec3Def() || InputType == FNiagaraTypeDefinition::GetPositionDef())
		{
			return Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynUniformVec3Path));
		}
		if (InputType == FNiagaraTypeDefinition::GetColorDef())
		{
			return Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynUniformColorPath));
		}
		return nullptr;
	}

	UNiagaraStackFunctionInput* FindChildInputByTokens(
		const TArray<UNiagaraStackFunctionInput*>& ChildInputs,
		const TArray<FString>& CandidateTokens)
	{
		int32 BestScore = MIN_int32;
		UNiagaraStackFunctionInput* BestInput = nullptr;
		for (UNiagaraStackFunctionInput* Child : ChildInputs)
		{
			if (!Child)
			{
				continue;
			}

			const FString DisplayName = NormalizeToken(Child->GetDisplayName().ToString());
			const FString HandleName = NormalizeToken(Child->GetInputParameterHandle().GetParameterHandleString().ToString());
			for (const FString& Token : CandidateTokens)
			{
				const FString NormalizedToken = NormalizeToken(Token);
				int32 Score = MIN_int32;
				if (DisplayName.Equals(NormalizedToken) || HandleName.Equals(NormalizedToken))
				{
					Score = 100;
				}
				else if (DisplayName.EndsWith(NormalizedToken) || HandleName.EndsWith(NormalizedToken))
				{
					Score = 80;
				}
				else if (DisplayName.Contains(NormalizedToken) || HandleName.Contains(NormalizedToken))
				{
					Score = 50;
				}

				if (Score > BestScore)
				{
					BestScore = Score;
					BestInput = Child;
				}
			}
		}
		return BestInput;
	}

	bool BuildRichCurveFromJsonPoints(
		const TArray<TSharedPtr<FJsonValue>>& CurvePoints,
		FRichCurve& OutCurve,
		FString& OutErrorMessage)
	{
		if (CurvePoints.Num() < 2)
		{
			OutErrorMessage = TEXT("Curve needs at least two points.");
			return false;
		}

		struct FCurvePoint
		{
			float Time = 0.0f;
			float Value = 0.0f;
		};

		TArray<FCurvePoint> Points;
		for (const TSharedPtr<FJsonValue>& PointValue : CurvePoints)
		{
			const TSharedPtr<FJsonObject>* PointObject = nullptr;
			if (!PointValue.IsValid() || !PointValue->TryGetObject(PointObject) || !PointObject->IsValid())
			{
				OutErrorMessage = TEXT("Curve points must be objects with numeric time and value.");
				return false;
			}

			double TimeNumber = 0.0;
			double ValueNumber = 0.0;
			if (!(*PointObject)->TryGetNumberField(TEXT("time"), TimeNumber))
			{
				OutErrorMessage = TEXT("Curve point is missing numeric time.");
				return false;
			}
			if (!(*PointObject)->TryGetNumberField(TEXT("value"), ValueNumber))
			{
				OutErrorMessage = TEXT("Float-from-curve points require numeric scalar values.");
				return false;
			}

			FCurvePoint Point;
			Point.Time = static_cast<float>(TimeNumber);
			Point.Value = static_cast<float>(ValueNumber);
			Points.Add(Point);
		}

		Points.Sort([](const FCurvePoint& A, const FCurvePoint& B) { return A.Time < B.Time; });

		OutCurve.Reset();
		for (const FCurvePoint& Point : Points)
		{
			const FKeyHandle KeyHandle = OutCurve.AddKey(Point.Time, Point.Value);
			FRichCurveKey& Key = OutCurve.GetKey(KeyHandle);
			Key.InterpMode = ERichCurveInterpMode::RCIM_Linear;
			Key.TangentMode = ERichCurveTangentMode::RCTM_Auto;
		}

		return true;
	}
}

bool SetConstantInputValue(
	UNiagaraStackFunctionInput& Input,
	const TSharedPtr<FJsonObject>& ToolInput,
	const FString& ValueFieldName,
	FString& OutAppliedValueString,
	FString& OutErrorMessage)
{
	return SetInputFromJsonField(Input, ToolInput, ValueFieldName, OutAppliedValueString, OutErrorMessage);
}

bool SetLinkedInputValue(
	UNiagaraStackFunctionInput& Input,
	const FNiagaraVariable& LinkedVariable,
	FString& OutAppliedValueString,
	FString& OutErrorMessage)
{
	const FNiagaraTypeDefinition& InputType = Input.GetInputType();
	if (!InputType.IsSameBaseDefinition(LinkedVariable.GetType()))
	{
		OutErrorMessage = FString::Printf(
			TEXT("Linked parameter '%s' has type '%s', but target input expects '%s'."),
			*LinkedVariable.GetName().ToString(),
			*NiagaraTypeToString(LinkedVariable.GetType()),
			*NiagaraTypeToString(InputType));
		return false;
	}

	Input.SetLinkedParameterValue(FNiagaraVariableBase(LinkedVariable.GetType(), LinkedVariable.GetName()));
	OutAppliedValueString = LinkedVariable.GetName().ToString();
	return true;
}

bool SetCurveInputValue(
	UNiagaraStackFunctionInput& Input,
	const TArray<TSharedPtr<FJsonValue>>& CurvePoints,
	FString& OutExpression,
	FString& OutErrorMessage)
{
	const int32 ComponentCount = GetComponentCountForType(Input.GetInputType());
	if (ComponentCount == 0)
	{
		OutErrorMessage = FString::Printf(TEXT("Input type '%s' is not supported for curve expressions."), *NiagaraTypeToString(Input.GetInputType()));
		return false;
	}

	OutExpression = BuildPiecewiseCurveExpression(CurvePoints, ComponentCount, OutErrorMessage);
	if (OutExpression.IsEmpty())
	{
		return false;
	}

	Input.SetCustomExpression(OutExpression);
	return true;
}

bool SetDynamicRandomRangeValue(
	UNiagaraStackFunctionInput& Input,
	const TSharedPtr<FJsonObject>& ToolInput,
	const FString& MinFieldName,
	const FString& MaxFieldName,
	FString& OutDynamicScriptPath,
	FString& OutErrorMessage)
{
	UNiagaraScript* DynamicScript = LoadDynamicScriptForType(Input.GetInputType());
	if (!DynamicScript)
	{
		OutErrorMessage = FString::Printf(TEXT("No supported random-range dynamic input exists for type '%s'."), *NiagaraTypeToString(Input.GetInputType()));
		return false;
	}

	OutDynamicScriptPath = DynamicScript->GetPathName();
	Input.SetDynamicInput(DynamicScript);

	TArray<UNiagaraStackFunctionInput*> ChildInputs = Input.GetChildInputs();
	UNiagaraStackFunctionInput* MinInput = nullptr;
	UNiagaraStackFunctionInput* MaxInput = nullptr;
	for (UNiagaraStackFunctionInput* Child : ChildInputs)
	{
		if (!Child)
		{
			continue;
		}

		const FString DisplayName = NormalizeToken(Child->GetDisplayName().ToString());
		const FString HandleName = NormalizeToken(Child->GetInputParameterHandle().GetParameterHandleString().ToString());
		if (MinInput == nullptr && (DisplayName.Contains(TEXT("min")) || HandleName.Contains(TEXT("min"))))
		{
			MinInput = Child;
		}
		if (MaxInput == nullptr && (DisplayName.Contains(TEXT("max")) || HandleName.Contains(TEXT("max"))))
		{
			MaxInput = Child;
		}
	}

	if (!MinInput || !MaxInput)
	{
		OutErrorMessage = FString::Printf(TEXT("Random-range dynamic input '%s' did not expose min/max child inputs."), *OutDynamicScriptPath);
		return false;
	}

	FString IgnoredAppliedValue;
	if (!SetInputFromJsonField(*MinInput, ToolInput, MinFieldName, IgnoredAppliedValue, OutErrorMessage))
	{
		return false;
	}
	if (!SetInputFromJsonField(*MaxInput, ToolInput, MaxFieldName, IgnoredAppliedValue, OutErrorMessage))
	{
		return false;
	}

	return true;
}

bool SetDynamicFloatFromCurveValue(
	UNiagaraStackFunctionInput& Input,
	const TSharedPtr<FJsonObject>& ToolInput,
	const FString& PointsFieldName,
	FString& OutDynamicScriptPath,
	FString& OutErrorMessage)
{
	if (Input.GetInputType() != FNiagaraTypeDefinition::GetFloatDef())
	{
		OutErrorMessage = FString::Printf(
			TEXT("float_from_curve currently supports only float inputs, but target input type is '%s'."),
			*NiagaraTypeToString(Input.GetInputType()));
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* CurvePoints = nullptr;
	if (!ToolInput.IsValid() || !ToolInput->TryGetArrayField(PointsFieldName, CurvePoints) || CurvePoints == nullptr)
	{
		OutErrorMessage = FString::Printf(TEXT("Missing required field: %s"), *PointsFieldName);
		return false;
	}

	UNiagaraScript* DynamicScript = Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynFloatFromCurvePath));
	if (!DynamicScript)
	{
		OutErrorMessage = TEXT("Float-from-curve dynamic input asset could not be loaded.");
		return false;
	}

	OutDynamicScriptPath = DynamicScript->GetPathName();
	Input.SetDynamicInput(DynamicScript);

	TArray<UNiagaraStackFunctionInput*> ChildInputs = Input.GetChildInputs();
	UNiagaraStackFunctionInput* CurveChildInput = nullptr;
	for (UNiagaraStackFunctionInput* Child : ChildInputs)
	{
		if (!Child)
		{
			continue;
		}

		const FNiagaraTypeDefinition& ChildType = Child->GetInputType();
		if (ChildType.IsDataInterface() && ChildType.GetClass() != nullptr && ChildType.GetClass()->IsChildOf(UNiagaraDataInterfaceCurve::StaticClass()))
		{
			CurveChildInput = Child;
			break;
		}
	}

	if (!CurveChildInput)
	{
		CurveChildInput = FindChildInputByTokens(ChildInputs, { TEXT("curve"), TEXT("float curve") });
	}

	if (!CurveChildInput)
	{
		OutErrorMessage = FString::Printf(TEXT("Float-from-curve dynamic input '%s' did not expose a curve child input."), *OutDynamicScriptPath);
		return false;
	}

	UNiagaraDataInterfaceCurve* CurveDataInterface = Cast<UNiagaraDataInterfaceCurve>(CurveChildInput->GetDataValueObject());
	if (!CurveDataInterface && CurveChildInput->GetInputType() == FNiagaraTypeDefinition::GetFloatDef())
	{
		FString NestedDynamicScriptPath;
		if (!SetDynamicFloatFromCurveValue(*CurveChildInput, ToolInput, PointsFieldName, NestedDynamicScriptPath, OutErrorMessage))
		{
			return false;
		}

		return true;
	}

	if (!CurveDataInterface)
	{
		OutErrorMessage = FString::Printf(
			TEXT("Curve child input '%s' did not resolve to UNiagaraDataInterfaceCurve or float curve input (type '%s', mode '%s')."),
			*CurveChildInput->GetInputParameterHandle().GetParameterHandleString().ToString(),
			*NiagaraTypeToString(CurveChildInput->GetInputType()),
			*ValueModeToString(CurveChildInput->GetValueMode()));
		return false;
	}

	FRichCurve AuthoredCurve;
	if (!BuildRichCurveFromJsonPoints(*CurvePoints, AuthoredCurve, OutErrorMessage))
	{
		return false;
	}

	CurveDataInterface->Modify();
	CurveDataInterface->Curve = AuthoredCurve;
#if WITH_EDITORONLY_DATA
	CurveDataInterface->CurveAsset = nullptr;
	CurveDataInterface->UpdateLUT();
#endif
	CurveDataInterface->MarkRenderDataDirty();

	if (const TSharedPtr<FJsonValue> ScaleField = ToolInput->TryGetField(TEXT("scale")); ScaleField.IsValid())
	{
		UNiagaraStackFunctionInput* ScaleChildInput = FindChildInputByTokens(ChildInputs, { TEXT("scale"), TEXT("curve scale") });
		if (ScaleChildInput)
		{
			FString IgnoredAppliedValue;
			if (!SetInputFromJsonField(*ScaleChildInput, ToolInput, TEXT("scale"), IgnoredAppliedValue, OutErrorMessage))
			{
				return false;
			}
		}
	}

	return true;
}

bool SetDynamicScaleVectorByCurveValue(
	UNiagaraStackFunctionInput& Input,
	const TSharedPtr<FJsonObject>& ToolInput,
	const FString& PointsFieldName,
	const FString& BaseVectorFieldName,
	FString& OutDynamicScriptPath,
	FString& OutErrorMessage)
{
	const FNiagaraTypeDefinition& InputType = Input.GetInputType();
	if (InputType != FNiagaraTypeDefinition::GetVec3Def() && InputType != FNiagaraTypeDefinition::GetPositionDef())
	{
		OutErrorMessage = FString::Printf(
			TEXT("scale_vector_by_curve currently supports only vector3/position inputs, but target input type is '%s'."),
			*NiagaraTypeToString(InputType));
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* CurvePoints = nullptr;
	if (!ToolInput.IsValid() || !ToolInput->TryGetArrayField(PointsFieldName, CurvePoints) || CurvePoints == nullptr)
	{
		OutErrorMessage = FString::Printf(TEXT("Missing required field: %s"), *PointsFieldName);
		return false;
	}

	FVector3f BaseVector(0.0f, 0.0f, 1.0f);
	if (const TSharedPtr<FJsonValue> BaseVectorField = ToolInput->TryGetField(BaseVectorFieldName); BaseVectorField.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>>* BaseVectorArray = nullptr;
		if (!BaseVectorField->TryGetArray(BaseVectorArray) || BaseVectorArray->Num() < 3)
		{
			OutErrorMessage = FString::Printf(TEXT("%s must be a numeric [x, y, z] array."), *BaseVectorFieldName);
			return false;
		}

		double X = 0.0;
		double Y = 0.0;
		double Z = 0.0;
		if (!(*BaseVectorArray)[0].IsValid() || (*BaseVectorArray)[0]->Type != EJson::Number ||
			!(*BaseVectorArray)[1].IsValid() || (*BaseVectorArray)[1]->Type != EJson::Number ||
			!(*BaseVectorArray)[2].IsValid() || (*BaseVectorArray)[2]->Type != EJson::Number)
		{
			OutErrorMessage = FString::Printf(TEXT("%s must contain numeric components."), *BaseVectorFieldName);
			return false;
		}
		X = (*BaseVectorArray)[0]->AsNumber();
		Y = (*BaseVectorArray)[1]->AsNumber();
		Z = (*BaseVectorArray)[2]->AsNumber();

		BaseVector = FVector3f(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z));
	}
	else if (TSharedPtr<const FStructOnScope> ExistingValue = Input.GetLocalValueStruct(); ExistingValue.IsValid() && ExistingValue->GetStructMemory() != nullptr)
	{
		BaseVector = *reinterpret_cast<const FVector3f*>(ExistingValue->GetStructMemory());
	}

	if (BaseVector.IsNearlyZero())
	{
		BaseVector = FVector3f(0.0f, 0.0f, 1.0f);
	}
	else
	{
		BaseVector = BaseVector.GetSafeNormal();
	}

	UNiagaraScript* DynamicScript = Cast<UNiagaraScript>(UEditorAssetLibrary::LoadAsset(DynScaleVectorByCurvePath));
	if (!DynamicScript)
	{
		OutErrorMessage = TEXT("Scale-vector-by-curve dynamic input asset could not be loaded.");
		return false;
	}

	OutDynamicScriptPath = DynamicScript->GetPathName();
	Input.SetDynamicInput(DynamicScript);

	TArray<UNiagaraStackFunctionInput*> ChildInputs = Input.GetChildInputs();
	UNiagaraStackFunctionInput* VectorChildInput = nullptr;
	UNiagaraStackFunctionInput* CurveChildInput = nullptr;
	for (UNiagaraStackFunctionInput* Child : ChildInputs)
	{
		if (!Child)
		{
			continue;
		}

		const FNiagaraTypeDefinition& ChildType = Child->GetInputType();
		if (!VectorChildInput && (ChildType == FNiagaraTypeDefinition::GetVec3Def() || ChildType == FNiagaraTypeDefinition::GetPositionDef()))
		{
			VectorChildInput = Child;
		}
		if (!CurveChildInput && ChildType.IsDataInterface() && ChildType.GetClass() != nullptr && ChildType.GetClass()->IsChildOf(UNiagaraDataInterfaceCurve::StaticClass()))
		{
			CurveChildInput = Child;
		}
	}

	if (!VectorChildInput)
	{
		VectorChildInput = FindChildInputByTokens(ChildInputs, { TEXT("vector"), TEXT("value"), TEXT("a") });
	}
	if (!CurveChildInput)
	{
		CurveChildInput = FindChildInputByTokens(ChildInputs, { TEXT("curve"), TEXT("float curve"), TEXT("scale curve") });
	}

	if (!VectorChildInput)
	{
		OutErrorMessage = FString::Printf(TEXT("Scale-vector-by-curve dynamic input '%s' did not expose a vector child input."), *OutDynamicScriptPath);
		return false;
	}
	if (!CurveChildInput)
	{
		OutErrorMessage = FString::Printf(TEXT("Scale-vector-by-curve dynamic input '%s' did not expose a curve child input."), *OutDynamicScriptPath);
		return false;
	}

	TSharedRef<FStructOnScope> VectorValue = MakeShared<FStructOnScope>(VectorChildInput->GetInputType().GetStruct());
	*reinterpret_cast<FVector3f*>(VectorValue->GetStructMemory()) = BaseVector;
	VectorChildInput->SetLocalValue(VectorValue);

	UNiagaraDataInterfaceCurve* CurveDataInterface = Cast<UNiagaraDataInterfaceCurve>(CurveChildInput->GetDataValueObject());
	UNiagaraDataInterfaceVectorCurve* VectorCurveDataInterface = Cast<UNiagaraDataInterfaceVectorCurve>(CurveChildInput->GetDataValueObject());
	if (!CurveDataInterface && CurveChildInput->GetInputType() == FNiagaraTypeDefinition::GetFloatDef())
	{
		FString NestedDynamicScriptPath;
		if (!SetDynamicFloatFromCurveValue(*CurveChildInput, ToolInput, PointsFieldName, NestedDynamicScriptPath, OutErrorMessage))
		{
			return false;
		}

		return true;
	}

	if (VectorCurveDataInterface)
	{
		VectorCurveDataInterface->Modify();
		VectorCurveDataInterface->XCurve.Reset();
		VectorCurveDataInterface->YCurve.Reset();
		VectorCurveDataInterface->ZCurve.Reset();

		for (const TSharedPtr<FJsonValue>& PointValue : *CurvePoints)
		{
			const TSharedPtr<FJsonObject>* PointObject = nullptr;
			if (!PointValue.IsValid() || !PointValue->TryGetObject(PointObject) || !PointObject->IsValid())
			{
				OutErrorMessage = TEXT("Curve points must be objects with numeric time and value.");
				return false;
			}

			double TimeNumber = 0.0;
			double ValueNumber = 0.0;
			if (!(*PointObject)->TryGetNumberField(TEXT("time"), TimeNumber) || !(*PointObject)->TryGetNumberField(TEXT("value"), ValueNumber))
			{
				OutErrorMessage = TEXT("Vector scale curve points require numeric time and scalar value.");
				return false;
			}

			const float Time = static_cast<float>(TimeNumber);
			const float Scalar = static_cast<float>(ValueNumber);

			auto AddLinearKey = [Time](FRichCurve& Curve, float ComponentValue)
			{
				const FKeyHandle KeyHandle = Curve.AddKey(Time, ComponentValue);
				FRichCurveKey& Key = Curve.GetKey(KeyHandle);
				Key.InterpMode = ERichCurveInterpMode::RCIM_Linear;
				Key.TangentMode = ERichCurveTangentMode::RCTM_Auto;
			};

			AddLinearKey(VectorCurveDataInterface->XCurve, BaseVector.X * Scalar);
			AddLinearKey(VectorCurveDataInterface->YCurve, BaseVector.Y * Scalar);
			AddLinearKey(VectorCurveDataInterface->ZCurve, BaseVector.Z * Scalar);
		}

#if WITH_EDITORONLY_DATA
		VectorCurveDataInterface->CurveAsset = nullptr;
		VectorCurveDataInterface->UpdateLUT();
#endif
		VectorCurveDataInterface->MarkRenderDataDirty();
		return true;
	}

	if (!CurveDataInterface)
	{
		OutErrorMessage = FString::Printf(
			TEXT("Curve child input '%s' did not resolve to UNiagaraDataInterfaceCurve, UNiagaraDataInterfaceVectorCurve, or float curve input (type '%s', mode '%s')."),
			*CurveChildInput->GetInputParameterHandle().GetParameterHandleString().ToString(),
			*NiagaraTypeToString(CurveChildInput->GetInputType()),
			*ValueModeToString(CurveChildInput->GetValueMode()));
		return false;
	}

	FRichCurve AuthoredCurve;
	if (!BuildRichCurveFromJsonPoints(*CurvePoints, AuthoredCurve, OutErrorMessage))
	{
		return false;
	}

	CurveDataInterface->Modify();
	CurveDataInterface->Curve = AuthoredCurve;
#if WITH_EDITORONLY_DATA
	CurveDataInterface->CurveAsset = nullptr;
	CurveDataInterface->UpdateLUT();
#endif
	CurveDataInterface->MarkRenderDataDirty();

	return true;
}

bool ResetInputValue(
	UNiagaraStackFunctionInput& Input,
	bool bResetToBaseIfAvailable,
	FString& OutErrorMessage)
{
	if (bResetToBaseIfAvailable && Input.CanResetToBase())
	{
		Input.ResetToBase();
		return true;
	}
	if (Input.CanReset())
	{
		Input.Reset();
		return true;
	}

	OutErrorMessage = TEXT("Input cannot be reset.");
	return false;
}

bool AddModuleToStage(
	FEmitterContext& Context,
	const FString& StageUsage,
	const FString& ModuleScriptPath,
	int32 TargetIndex,
	const FString& SuggestedName,
	FString& OutModuleKey,
	FString& OutErrorMessage)
{
	UNiagaraScript* TargetScript = nullptr;
	UNiagaraNodeOutput* OutputNode = nullptr;
	ENiagaraScriptUsage Usage;
	FGuid UsageId;
	if (!ResolveStageTarget(Context, StageUsage, TargetScript, OutputNode, Usage, UsageId, OutErrorMessage))
	{
		return false;
	}

	UObject* LoadedModuleAsset = UEditorAssetLibrary::LoadAsset(ModuleScriptPath);
	if (UObjectRedirector* Redirector = Cast<UObjectRedirector>(LoadedModuleAsset))
	{
		LoadedModuleAsset = Redirector->DestinationObject;
	}

	UNiagaraScript* ModuleScript = Cast<UNiagaraScript>(LoadedModuleAsset);
	if (!ModuleScript)
	{
		const FString LoadedClass = LoadedModuleAsset ? LoadedModuleAsset->GetClass()->GetName() : TEXT("null");
		OutErrorMessage = FString::Printf(TEXT("Niagara module script not found at '%s' (loaded class: %s)."), *ModuleScriptPath, *LoadedClass);
		return false;
	}

	UNiagaraNodeFunctionCall* AddedNode = FNiagaraStackGraphUtilities::AddScriptModuleToStack(ModuleScript, *OutputNode, TargetIndex, SuggestedName);
	if (!AddedNode)
	{
		OutErrorMessage = FString::Printf(TEXT("Failed to add module '%s' to stage '%s'."), *ModuleScriptPath, *StageUsage);
		return false;
	}

	OutModuleKey = AddedNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower);
	Context.System->MarkPackageDirty();
	return true;
}

bool RemoveModuleFromStack(
	FEmitterContext& Context,
	const FString& ModuleKey,
	FString& OutErrorMessage)
{
	FStackModuleSummary ModuleSummary;
	UNiagaraStackModuleItem* ModuleItem = FindModuleByKey(Context, ModuleKey, &ModuleSummary);
	if (!ModuleItem || ModuleSummary.ModuleNode == nullptr)
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' was not found."), *ModuleKey);
		return false;
	}

	if (!ModuleItem->CanMoveAndDelete())
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' cannot be deleted through the Niagara stack."), *ModuleSummary.DisplayName);
		return false;
	}

	ModuleItem->Delete();
	if (FindModuleByKey(Context, ModuleKey, nullptr) != nullptr)
	{
		OutErrorMessage = FString::Printf(TEXT("Failed to remove module '%s'."), *ModuleSummary.DisplayName);
		return false;
	}

	Context.System->MarkPackageDirty();
	return true;
}

bool MoveModuleInStage(
	FEmitterContext& Context,
	const FString& ModuleKey,
	int32 TargetIndex,
	FString& OutMovedModuleKey,
	FString& OutErrorMessage)
{
	FStackModuleSummary ModuleSummary;
	UNiagaraStackModuleItem* ModuleItem = FindModuleByKey(Context, ModuleKey, &ModuleSummary);
	if (!ModuleItem || ModuleSummary.ModuleNode == nullptr)
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' was not found."), *ModuleKey);
		return false;
	}

	if (ModuleSummary.ScriptAssetPath.IsEmpty())
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' does not have a script asset path to reinsert."), *ModuleSummary.DisplayName);
		return false;
	}

	if (ModuleSummary.StageUsage.IsEmpty())
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' does not have a resolvable stage usage."), *ModuleSummary.DisplayName);
		return false;
	}

	FString AddedModuleKey;
	if (!AddModuleToStage(Context, ModuleSummary.StageUsage, ModuleSummary.ScriptAssetPath, TargetIndex, ModuleSummary.DisplayName, AddedModuleKey, OutErrorMessage))
	{
		return false;
	}

	if (!ModuleItem->CanMoveAndDelete())
	{
		OutErrorMessage = FString::Printf(TEXT("Module '%s' was duplicated at the target index but the original instance could not be deleted."), *ModuleSummary.DisplayName);
		return false;
	}

	ModuleItem->Delete();
	OutMovedModuleKey = AddedModuleKey;
	Context.System->MarkPackageDirty();
	return true;
}

bool CompileAndSaveSystem(
	FEmitterContext& Context,
	FString& OutErrorMessage)
{
	TSharedPtr<FJsonObject> IgnoredDiagnostics;
	return CompileAndSaveSystem(Context, OutErrorMessage, IgnoredDiagnostics);
}

bool CompileAndSaveSystem(
	FEmitterContext& Context,
	FString& OutErrorMessage,
	TSharedPtr<FJsonObject>& OutDiagnostics)
{
	if (!Context.System)
	{
		OutErrorMessage = TEXT("Emitter context is not initialized.");
		return false;
	}

	if (Context.StackViewModel && Context.StackViewModel->GetRootEntry())
	{
		Context.StackViewModel->GetRootEntry()->RefreshChildren();
	}

	FModuleManager::LoadModuleChecked<INiagaraModule>(TEXT("Niagara")).RequestCompileSystem(Context.System, true, nullptr);
	Context.System->RequestCompile(false);
	Context.System->WaitForCompilationComplete(true, false);

	TArray<UNiagaraScript*> Scripts;
	GatherSystemScripts(Context.System, Scripts);

	const ENiagaraScriptCompileStatus LatestCompileStatus = AggregateScriptStatuses(Scripts);
	OutDiagnostics = BuildCompileDiagnostics(Context, LatestCompileStatus, Scripts);

	bool bHasCompileError = LatestCompileStatus == ENiagaraScriptCompileStatus::NCS_Error;
	bool bHasCompileWarning = IsCompileStatusWarning(LatestCompileStatus);
	for (UNiagaraScript* Script : Scripts)
	{
		if (!Script)
		{
			continue;
		}

		const ENiagaraScriptCompileStatus ScriptStatus = Script->GetLastCompileStatus();
		if (ScriptStatus == ENiagaraScriptCompileStatus::NCS_Error)
		{
			bHasCompileError = true;
		}
		if (IsCompileStatusWarning(ScriptStatus))
		{
			bHasCompileWarning = true;
		}

		for (const FNiagaraCompileEvent& CompileEvent : Script->GetVMExecutableData().LastCompileEvents)
		{
			if (CompileEvent.Severity == FNiagaraCompileEventSeverity::Error)
			{
				bHasCompileError = true;
				break;
			}
			if (CompileEvent.Severity == FNiagaraCompileEventSeverity::Warning)
			{
				bHasCompileWarning = true;
			}
		}

		if (bHasCompileError)
		{
			break;
		}
	}

	if (bHasCompileError)
	{
		if (OutErrorMessage.IsEmpty())
		{
			OutErrorMessage = FString::Printf(
				TEXT("Niagara system '%s' still has compile errors or is invalid after compile. Latest compile status: %s."),
				*Context.System->GetPathName(),
				*CompileStatusToString(LatestCompileStatus));
		}
		return false;
	}

	if (bHasCompileWarning)
	{
		if (OutErrorMessage.IsEmpty())
		{
			OutErrorMessage = FString::Printf(
				TEXT("Niagara system '%s' still has compile warnings after compile. Fix the Niagara Log warnings before continuing. Latest compile status: %s."),
				*Context.System->GetPathName(),
				*CompileStatusToString(LatestCompileStatus));
		}
		return false;
	}

	if (!SaveNiagaraAsset(Context.System))
	{
		OutErrorMessage = FString::Printf(TEXT("Failed to save Niagara system '%s'."), *Context.System->GetPathName());
		return false;
	}
	return true;
}

bool SetEmitterLocalSpace(
	FEmitterContext& Context,
	bool bLocalSpace,
	FString& OutErrorMessage)
{
	if (!Context.EmitterData)
	{
		OutErrorMessage = TEXT("Emitter data is missing.");
		return false;
	}

	Context.EmitterData->bLocalSpace = bLocalSpace;
	Context.System->MarkPackageDirty();
	return true;
}
}
