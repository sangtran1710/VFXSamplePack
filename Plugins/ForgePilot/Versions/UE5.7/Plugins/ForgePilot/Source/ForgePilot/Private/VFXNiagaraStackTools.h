#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class UNiagaraSystem;
class UNiagaraScript;
class UNiagaraNodeOutput;
class UNiagaraNodeFunctionCall;
class UNiagaraStackModuleItem;
class UNiagaraStackFunctionInput;
class UNiagaraStackViewModel;
enum class ENiagaraScriptCompileStatus : uint8;
class FNiagaraEmitterHandleViewModel;
class FNiagaraSystemViewModel;
struct FNiagaraEmitterHandle;
struct FNiagaraVariable;
struct FVersionedNiagaraEmitterData;
struct FNiagaraTypeDefinition;

namespace VFXNiagaraStackTools
{
	struct FStackInputSummary
	{
		FString InputKey;
		FString DisplayName;
		FString TypeName;
		FString ValueSource;
		FString ValueString;
		bool bHasEditableLocalValue = false;
		bool bCanReset = false;
	};

	struct FStackModuleSummary
	{
		FString ModuleKey;
		FString DisplayName;
		FString ScriptAssetPath;
		FString StageUsage;
		FString UsageId;
		int32 StageIndex = INDEX_NONE;
		UNiagaraStackModuleItem* ModuleItem = nullptr;
		UNiagaraNodeFunctionCall* ModuleNode = nullptr;
		TArray<FStackInputSummary> Inputs;
	};

	struct FEmitterContext
	{
		UNiagaraSystem* System = nullptr;
		TSharedPtr<FNiagaraSystemViewModel> SystemViewModel;
		TSharedPtr<FNiagaraEmitterHandleViewModel> EmitterHandleViewModel;
		UNiagaraStackViewModel* StackViewModel = nullptr;
		FNiagaraEmitterHandle* EmitterHandle = nullptr;
		FVersionedNiagaraEmitterData* EmitterData = nullptr;
		int32 EmitterIndex = INDEX_NONE;
	};

	bool ResolveEmitterContext(
		const FString& SystemPath,
		const TSharedPtr<FJsonObject>& Input,
		FEmitterContext& OutContext,
		FString& OutErrorCode,
		FString& OutErrorMessage);

	void CollectModuleSummaries(
		FEmitterContext& Context,
		TArray<FStackModuleSummary>& OutModules);

	FString UsageToToolStageString(int32 UsageValue);

	UNiagaraStackModuleItem* FindModuleByKey(
		FEmitterContext& Context,
		const FString& ModuleKey,
		FStackModuleSummary* OutSummary = nullptr);

	UNiagaraStackFunctionInput* FindInputByKey(
		FEmitterContext& Context,
		const FString& ModuleKey,
		const FString& InputKey,
		FStackModuleSummary* OutModuleSummary = nullptr);

	UNiagaraStackFunctionInput* FindInputByHeuristics(
		FEmitterContext& Context,
		const FString& ModuleKey,
		const TArray<FString>& CandidateTokens,
		FString& OutMatchedInputKey,
		FStackModuleSummary* OutModuleSummary = nullptr);

	bool SetConstantInputValue(
		UNiagaraStackFunctionInput& Input,
		const TSharedPtr<FJsonObject>& ToolInput,
		const FString& ValueFieldName,
		FString& OutAppliedValueString,
		FString& OutErrorMessage);

	bool SetLinkedInputValue(
		UNiagaraStackFunctionInput& Input,
		const FNiagaraVariable& LinkedVariable,
		FString& OutAppliedValueString,
		FString& OutErrorMessage);

	bool SetCurveInputValue(
		UNiagaraStackFunctionInput& Input,
		const TArray<TSharedPtr<FJsonValue>>& CurvePoints,
		FString& OutExpression,
		FString& OutErrorMessage);

	bool SetDynamicRandomRangeValue(
		UNiagaraStackFunctionInput& Input,
		const TSharedPtr<FJsonObject>& ToolInput,
		const FString& MinFieldName,
		const FString& MaxFieldName,
		FString& OutDynamicScriptPath,
		FString& OutErrorMessage);

	bool SetDynamicFloatFromCurveValue(
		UNiagaraStackFunctionInput& Input,
		const TSharedPtr<FJsonObject>& ToolInput,
		const FString& PointsFieldName,
		FString& OutDynamicScriptPath,
		FString& OutErrorMessage);

	bool SetDynamicScaleVectorByCurveValue(
		UNiagaraStackFunctionInput& Input,
		const TSharedPtr<FJsonObject>& ToolInput,
		const FString& PointsFieldName,
		const FString& BaseVectorFieldName,
		FString& OutDynamicScriptPath,
		FString& OutErrorMessage);

	bool ResetInputValue(
		UNiagaraStackFunctionInput& Input,
		bool bResetToBaseIfAvailable,
		FString& OutErrorMessage);

	bool AddModuleToStage(
		FEmitterContext& Context,
		const FString& StageUsage,
		const FString& ModuleScriptPath,
		int32 TargetIndex,
		const FString& SuggestedName,
		FString& OutModuleKey,
		FString& OutErrorMessage);

	bool RemoveModuleFromStack(
		FEmitterContext& Context,
		const FString& ModuleKey,
		FString& OutErrorMessage);

	bool MoveModuleInStage(
		FEmitterContext& Context,
		const FString& ModuleKey,
		int32 TargetIndex,
		FString& OutMovedModuleKey,
		FString& OutErrorMessage);

	bool CompileAndSaveSystem(
		FEmitterContext& Context,
		FString& OutErrorMessage);

	bool CompileAndSaveSystem(
		FEmitterContext& Context,
		FString& OutErrorMessage,
		TSharedPtr<FJsonObject>& OutDiagnostics);

	bool SetEmitterLocalSpace(
		FEmitterContext& Context,
		bool bLocalSpace,
		FString& OutErrorMessage);

	TSharedPtr<FJsonValue> SerializeInputValue(const UNiagaraStackFunctionInput& Input);
	TSharedPtr<FJsonObject> BuildModuleJsonObject(const FStackModuleSummary& ModuleSummary, bool bIncludeInputs);
	TSharedPtr<FJsonObject> BuildStandardModulePresenceObject(const TArray<FStackModuleSummary>& Modules);
}
