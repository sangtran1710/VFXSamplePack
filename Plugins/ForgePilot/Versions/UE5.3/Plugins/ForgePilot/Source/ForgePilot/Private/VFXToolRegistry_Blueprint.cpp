// Copyright 2026, HenryFX. ForgePilot Plugin.
// Phase 2: Blueprint Tools - Registration + Implementation
#include "VFXToolRegistry.h"
#include "VFXMutationAudit.h"
#include "VFXToolRegistryHelpers.h"

#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EdGraphSchema_K2.h"
#include "EditorAssetLibrary.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "K2Node_Event.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_CallFunction.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_Comment.h"
#include "UObject/SavePackage.h"
#include "UObject/UnrealType.h"
#include "UObject/SoftObjectPtr.h"
#include "Misc/PackageName.h"
#include "Components/SceneComponent.h"
#include "Components/LightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/TimelineTemplate.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "Components/PrimitiveComponent.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"
#include "PackageTools.h"
#include "Subsystems/AssetEditorSubsystem.h"

static TSharedPtr<FJsonObject> BuildBlueprintNodeDetailsObject(UEdGraphNode* Node, bool bIncludeLinks)
{
	TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
	if (!Node)
	{
		NodeObj->SetBoolField(TEXT("valid"), false);
		return NodeObj;
	}

	NodeObj->SetBoolField(TEXT("valid"), true);
	NodeObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	NodeObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
	NodeObj->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	NodeObj->SetStringField(TEXT("node_comment"), Node->NodeComment);
	NodeObj->SetNumberField(TEXT("node_pos_x"), Node->NodePosX);
	NodeObj->SetNumberField(TEXT("node_pos_y"), Node->NodePosY);

	UEdGraph* Graph = Node->GetGraph();
	NodeObj->SetStringField(TEXT("graph_name"), Graph ? Graph->GetName() : TEXT(""));
	UBlueprint* OwnerBlueprint = Graph ? FBlueprintEditorUtils::FindBlueprintForGraph(Graph) : nullptr;
	NodeObj->SetStringField(TEXT("blueprint_path"), OwnerBlueprint ? OwnerBlueprint->GetPathName() : TEXT(""));

	TArray<TSharedPtr<FJsonValue>> PinsArray;
	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (!Pin) continue;

		TSharedPtr<FJsonObject> PinObj = MakeShareable(new FJsonObject());
		PinObj->SetStringField(TEXT("name"), Pin->PinName.ToString());
		PinObj->SetStringField(TEXT("direction"),
			(Pin->Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("input") : TEXT("output"));
		PinObj->SetStringField(TEXT("type"), PinTypeToDisplayString(Pin->PinType));
		PinObj->SetStringField(TEXT("default_value"), Pin->DefaultValue);
		PinObj->SetStringField(TEXT("default_object"),
			Pin->DefaultObject ? Pin->DefaultObject->GetPathName() : TEXT(""));
		PinObj->SetStringField(TEXT("default_text"), Pin->DefaultTextValue.ToString());
		PinObj->SetBoolField(TEXT("is_orphaned"), Pin->bOrphanedPin);
		PinObj->SetBoolField(TEXT("is_hidden"), Pin->bHidden);

		if (bIncludeLinks)
		{
			TArray<TSharedPtr<FJsonValue>> LinksArray;
			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin || !LinkedPin->GetOwningNode()) continue;
				TSharedPtr<FJsonObject> LinkObj = MakeShareable(new FJsonObject());
				LinkObj->SetStringField(TEXT("node_guid"),
					LinkedPin->GetOwningNode()->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				LinkObj->SetStringField(TEXT("node_title"),
					LinkedPin->GetOwningNode()->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				LinkObj->SetStringField(TEXT("pin_name"), LinkedPin->PinName.ToString());
				LinksArray.Add(MakeShareable(new FJsonValueObject(LinkObj)));
			}
			PinObj->SetArrayField(TEXT("links"), LinksArray);
		}

		PinsArray.Add(MakeShareable(new FJsonValueObject(PinObj)));
	}

	NodeObj->SetArrayField(TEXT("pins"), PinsArray);
	return NodeObj;
}

static TSharedPtr<FJsonObject> BuildCompactBlueprintNodeObject(UEdGraphNode* Node)
{
	TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
	if (!Node)
	{
		NodeObj->SetBoolField(TEXT("valid"), false);
		return NodeObj;
	}

	NodeObj->SetBoolField(TEXT("valid"), true);
	NodeObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	NodeObj->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	NodeObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
	NodeObj->SetStringField(TEXT("graph_name"), Node->GetGraph() ? Node->GetGraph()->GetName() : TEXT(""));
	NodeObj->SetNumberField(TEXT("node_pos_x"), Node->NodePosX);
	NodeObj->SetNumberField(TEXT("node_pos_y"), Node->NodePosY);
	return NodeObj;
}

static FString NormalizeAssetPathForMatch(const FString& AssetPath)
{
	FString Normalized = AssetPath.TrimStartAndEnd();
	Normalized.RemoveFromStart(TEXT("\""));
	Normalized.RemoveFromEnd(TEXT("\""));
	return Normalized;
}

static FString GetAssetPackagePathFromObjectPath(const FString& AssetPath)
{
	const FString Normalized = NormalizeAssetPathForMatch(AssetPath);
	FString PackagePath;
	if (Normalized.Split(TEXT("."), &PackagePath, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		return PackagePath;
	}
	return Normalized;
}

static FString GetAssetNameFromObjectPath(const FString& AssetPath)
{
	const FString PackagePath = GetAssetPackagePathFromObjectPath(AssetPath);
	return FPackageName::GetShortName(PackagePath);
}

static bool StringReferencesAsset(
	const FString& Candidate,
	const FString& TargetObjectPath,
	const FString& TargetPackagePath,
	const FString& TargetAssetName)
{
	const FString CandidateLower = Candidate.ToLower();
	if (CandidateLower.IsEmpty())
	{
		return false;
	}

	if (!TargetObjectPath.IsEmpty() && CandidateLower.Contains(TargetObjectPath.ToLower()))
	{
		return true;
	}

	if (!TargetPackagePath.IsEmpty() && CandidateLower.Contains(TargetPackagePath.ToLower()))
	{
		return true;
	}

	if (!TargetAssetName.IsEmpty() && CandidateLower.Equals(TargetAssetName.ToLower(), ESearchCase::CaseSensitive))
	{
		return true;
	}

	return false;
}

static FString ExportPropertyValueToString(FProperty* Property, const void* ValuePtr)
{
	if (!Property || !ValuePtr)
	{
		return FString();
	}

	FString ExportedValue;
	Property->ExportTextItem_Direct(ExportedValue, ValuePtr, nullptr, nullptr, PPF_None);
	return ExportedValue;
}

static void AppendAssetPropertyMatch(
	TArray<TSharedPtr<FJsonValue>>& MatchesArray,
	const FString& MatchSource,
	const FString& OwnerName,
	const FString& PropertyPath,
	const FString& MatchType,
	const FString& ValueText,
	const FString& ResolvedObjectPath)
{
	TSharedPtr<FJsonObject> MatchObj = MakeShareable(new FJsonObject());
	MatchObj->SetStringField(TEXT("match_source"), MatchSource);
	MatchObj->SetStringField(TEXT("owner_name"), OwnerName);
	MatchObj->SetStringField(TEXT("property_path"), PropertyPath);
	MatchObj->SetStringField(TEXT("match_type"), MatchType);
	MatchObj->SetStringField(TEXT("value_text"), ValueText);
	MatchObj->SetStringField(TEXT("resolved_object_path"), ResolvedObjectPath);
	MatchesArray.Add(MakeShareable(new FJsonValueObject(MatchObj)));
}

static void GatherAssetPropertyMatchesRecursive(
	FProperty* Property,
	const void* ValuePtr,
	const FString& PropertyPath,
	const FString& MatchSource,
	const FString& OwnerName,
	UObject* TargetAsset,
	const FString& TargetObjectPath,
	const FString& TargetPackagePath,
	const FString& TargetAssetName,
	TArray<TSharedPtr<FJsonValue>>& MatchesArray,
	const int32 Depth = 0)
{
	if (!Property || !ValuePtr || Depth > 6)
	{
		return;
	}

	if (const FObjectPropertyBase* ObjectProp = CastField<FObjectPropertyBase>(Property))
	{
		UObject* ObjectValue = ObjectProp->GetObjectPropertyValue(ValuePtr);
		const FString ResolvedPath = ObjectValue ? ObjectValue->GetPathName() : FString();
		const FString ValueText = ExportPropertyValueToString(Property, ValuePtr);
		if ((TargetAsset && ObjectValue == TargetAsset) ||
			StringReferencesAsset(ResolvedPath, TargetObjectPath, TargetPackagePath, TargetAssetName) ||
			StringReferencesAsset(ValueText, TargetObjectPath, TargetPackagePath, TargetAssetName))
		{
			AppendAssetPropertyMatch(
				MatchesArray,
				MatchSource,
				OwnerName,
				PropertyPath,
				TEXT("object_property"),
				ValueText,
				ResolvedPath);
		}
		return;
	}

	if (const FSoftObjectProperty* SoftObjectProp = CastField<FSoftObjectProperty>(Property))
	{
		const FSoftObjectPtr SoftValue = SoftObjectProp->GetPropertyValue(ValuePtr);
		const FString ResolvedPath = SoftValue.ToSoftObjectPath().ToString();
		const FString ValueText = ExportPropertyValueToString(Property, ValuePtr);
		if (StringReferencesAsset(ResolvedPath, TargetObjectPath, TargetPackagePath, TargetAssetName) ||
			StringReferencesAsset(ValueText, TargetObjectPath, TargetPackagePath, TargetAssetName))
		{
			AppendAssetPropertyMatch(
				MatchesArray,
				MatchSource,
				OwnerName,
				PropertyPath,
				TEXT("soft_object_property"),
				ValueText,
				ResolvedPath);
		}
		return;
	}

	if (const FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, ValuePtr);
		for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
		{
			const FString ElementPath = FString::Printf(TEXT("%s[%d]"), *PropertyPath, Index);
			GatherAssetPropertyMatchesRecursive(
				ArrayProp->Inner,
				ArrayHelper.GetRawPtr(Index),
				ElementPath,
				MatchSource,
				OwnerName,
				TargetAsset,
				TargetObjectPath,
				TargetPackagePath,
				TargetAssetName,
				MatchesArray,
				Depth + 1);
		}
		return;
	}

	if (const FStructProperty* StructProp = CastField<FStructProperty>(Property))
	{
		for (TFieldIterator<FProperty> It(StructProp->Struct); It; ++It)
		{
			FProperty* InnerProperty = *It;
			if (!InnerProperty)
			{
				continue;
			}

			const void* InnerValuePtr = InnerProperty->ContainerPtrToValuePtr<void>(ValuePtr);
			GatherAssetPropertyMatchesRecursive(
				InnerProperty,
				InnerValuePtr,
				FString::Printf(TEXT("%s.%s"), *PropertyPath, *InnerProperty->GetName()),
				MatchSource,
				OwnerName,
				TargetAsset,
				TargetObjectPath,
				TargetPackagePath,
				TargetAssetName,
				MatchesArray,
				Depth + 1);
		}
	}
}

static void CollectExecOutgoingNodes(
	UEdGraphNode* Node,
	TArray<TTuple<UEdGraphNode*, FString, FString>>& OutLinks)
{
	OutLinks.Reset();
	if (!Node)
	{
		return;
	}

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (!Pin || Pin->Direction != EGPD_Output || Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec)
		{
			continue;
		}

		for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			UEdGraphNode* OtherNode = LinkedPin ? LinkedPin->GetOwningNode() : nullptr;
			if (!OtherNode)
			{
				continue;
			}

			OutLinks.Emplace(
				OtherNode,
				Pin->PinName.ToString(),
				LinkedPin->PinName.ToString());
		}
	}
}

static TArray<TSharedPtr<FJsonValue>> MakeBlueprintStringJsonArray(const TArray<FString>& Strings)
{
	TArray<TSharedPtr<FJsonValue>> Array;
	Array.Reserve(Strings.Num());
	for (const FString& Value : Strings)
	{
		Array.Add(MakeShareable(new FJsonValueString(Value)));
	}
	return Array;
}

static void GetBlueprintGraphs(UBlueprint* Blueprint, TArray<UEdGraph*>& OutGraphs)
{
	if (!Blueprint) return;

	OutGraphs.Append(Blueprint->UbergraphPages);
	OutGraphs.Append(Blueprint->FunctionGraphs);
	OutGraphs.Append(Blueprint->MacroGraphs);
	OutGraphs.Append(Blueprint->DelegateSignatureGraphs);
}

static UEdGraph* FindBlueprintGraphByName(UBlueprint* Blueprint, const FString& GraphName)
{
	if (!Blueprint)
	{
		return nullptr;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);
	for (UEdGraph* Graph : Graphs)
	{
		if (Graph && Graph->GetName().Equals(GraphName, ESearchCase::IgnoreCase))
		{
			return Graph;
		}
	}

	return nullptr;
}

static UEdGraphNode* FindBlueprintNodeByGuid(UBlueprint* Blueprint, const FGuid& NodeGuid)
{
	if (!Blueprint || !NodeGuid.IsValid())
	{
		return nullptr;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph)
		{
			continue;
		}

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (Node && Node->NodeGuid == NodeGuid)
			{
				return Node;
			}
		}
	}

	return nullptr;
}

static UK2Node_CustomEvent* FindBlueprintCustomEventByName(UBlueprint* Blueprint, const FString& EventName)
{
	if (!Blueprint || EventName.IsEmpty())
	{
		return nullptr;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph)
		{
			continue;
		}

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			UK2Node_CustomEvent* CustomEvent = Cast<UK2Node_CustomEvent>(Node);
			if (CustomEvent &&
				CustomEvent->CustomFunctionName.ToString().Equals(EventName, ESearchCase::IgnoreCase))
			{
				return CustomEvent;
			}
		}
	}

	return nullptr;
}

static bool ResolveBlueprintVariableProperty(
	UBlueprint* Blueprint,
	const FString& VariableName,
	FProperty*& OutProperty,
	bool& bOutSelfContext,
	UClass*& OutOwnerClass,
	FString& OutResolvedOwnerPath,
	FString& OutError)
{
	if (!Blueprint)
	{
		OutError = TEXT("Blueprint is null.");
		return false;
	}

	const FName VariableFName(*VariableName);
	if (VariableFName.IsNone())
	{
		OutError = TEXT("Variable name is empty.");
		return false;
	}

	auto TryResolvePropertyInClassChain =
		[&](UClass* StartClass) -> bool
		{
			for (UClass* SearchClass = StartClass; SearchClass; SearchClass = SearchClass->GetSuperClass())
			{
				if (FProperty* FoundProperty = SearchClass->FindPropertyByName(VariableFName))
				{
					OutProperty = FoundProperty;
					OutOwnerClass = SearchClass;
					OutResolvedOwnerPath = SearchClass->GetPathName();
					bOutSelfContext = true;
					return true;
				}
			}

			return false;
		};

	for (const FBPVariableDescription& VariableDesc : Blueprint->NewVariables)
	{
		if (VariableDesc.VarName == VariableFName)
		{
			if (TryResolvePropertyInClassChain(Blueprint->SkeletonGeneratedClass) || TryResolvePropertyInClassChain(Blueprint->GeneratedClass))
			{
				return true;
			}
		}
	}

	if (Blueprint->SimpleConstructionScript)
	{
		for (USCS_Node* Node : Blueprint->SimpleConstructionScript->GetAllNodes())
		{
			if (Node && Node->GetVariableName() == VariableFName)
			{
				if (TryResolvePropertyInClassChain(Blueprint->SkeletonGeneratedClass) || TryResolvePropertyInClassChain(Blueprint->GeneratedClass))
				{
					return true;
				}
			}
		}
	}

	if (TryResolvePropertyInClassChain(Blueprint->SkeletonGeneratedClass) || TryResolvePropertyInClassChain(Blueprint->GeneratedClass))
	{
		return true;
	}

	OutError = FString::Printf(TEXT("Variable '%s' was not found in Blueprint '%s'."), *VariableName, *Blueprint->GetPathName());
	return false;
}

static bool ParseOptionalBoolField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, bool DefaultValue)
{
	if (!Input.IsValid())
	{
		return DefaultValue;
	}

	bool BoolValue = DefaultValue;
	if (Input->TryGetBoolField(FieldName, BoolValue))
	{
		return BoolValue;
	}

	FString StringValue;
	if (Input->TryGetStringField(FieldName, StringValue))
	{
		return StringValue.Equals(TEXT("true"), ESearchCase::IgnoreCase) || StringValue == TEXT("1");
	}

	return DefaultValue;
}

static bool BuildBlueprintToolError(
	FString& OutResult,
	const FString& ErrorCode,
	const FString& ErrorMessage,
	const TSharedPtr<FJsonObject>& DebugContext = nullptr)
{
	TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
	ErrorObj->SetBoolField(TEXT("success"), false);
	ErrorObj->SetStringField(TEXT("error_code"), ErrorCode);
	ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
	if (DebugContext.IsValid())
	{
		ErrorObj->SetObjectField(TEXT("debug_context"), DebugContext);
	}
	OutResult = JsonObjToString(ErrorObj);
	return false;
}

static UClass* ResolveBlueprintOwnerClass(const FString& ClassIdentifier)
{
	FString Normalized = ClassIdentifier.TrimStartAndEnd();
	Normalized.RemoveFromStart(TEXT("\""));
	Normalized.RemoveFromEnd(TEXT("\""));
	if (Normalized.IsEmpty())
	{
		return nullptr;
	}

	if (UClass* LoadedClass = LoadObject<UClass>(nullptr, *Normalized))
	{
		return LoadedClass;
	}

	if (UClass* FoundClass = FindFirstObject<UClass>(*Normalized, EFindFirstObjectOptions::NativeFirst))
	{
		return FoundClass;
	}

	if (!Normalized.StartsWith(TEXT("U")))
	{
		if (UClass* PrefixedClass = FindFirstObject<UClass>(*(TEXT("U") + Normalized), EFindFirstObjectOptions::NativeFirst))
		{
			return PrefixedClass;
		}
	}

	return nullptr;
}

static UFunction* ResolveBlueprintFunction(UClass* OwnerClass, const FString& FunctionName)
{
	if (!OwnerClass || FunctionName.IsEmpty())
	{
		return nullptr;
	}

	return OwnerClass->FindFunctionByName(FName(*FunctionName));
}

static UEdGraphPin* FindBlueprintNodePinByName(UEdGraphNode* Node, const FString& PinName)
{
	if (!Node || PinName.IsEmpty())
	{
		return nullptr;
	}

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin && Pin->PinName.ToString().Equals(PinName, ESearchCase::CaseSensitive))
		{
			return Pin;
		}
	}

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin && Pin->PinName.ToString().Equals(PinName, ESearchCase::IgnoreCase))
		{
			return Pin;
		}
	}

	return nullptr;
}

static FString SanitizeBlueprintMutationToken(FString Value)
{
	Value = Value.TrimStartAndEnd();
	for (TCHAR& Character : Value)
	{
		if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
		{
			Character = TEXT('_');
		}
	}

	return Value.IsEmpty() ? TEXT("blueprint") : Value;
}

static FString GetBlueprintMutationSnapshotRoot()
{
	return FPaths::Combine(
		GetForgePilotDownloadsRoot(),
		TEXT("BlueprintMutationSnapshots"));
	}

static FString BlueprintStatusToString(const EBlueprintStatus Status)
{
	switch (Status)
	{
	case BS_Unknown:
		return TEXT("unknown");
	case BS_Dirty:
		return TEXT("dirty");
	case BS_Error:
		return TEXT("error");
	case BS_UpToDate:
		return TEXT("up_to_date");
	case BS_BeingCreated:
		return TEXT("being_created");
	case BS_UpToDateWithWarnings:
		return TEXT("up_to_date_with_warnings");
	default:
		return TEXT("unrecognized");
	}
}

static bool CreateBlueprintMutationSnapshot(UBlueprint* Blueprint, const FString& MutationAction, FString& OutSnapshotPath, FString& OutError)
{
	if (!Blueprint)
	{
		OutError = TEXT("Blueprint is null.");
		return false;
	}

	const FString SnapshotRoot = GetBlueprintMutationSnapshotRoot();
	if (SnapshotRoot.IsEmpty())
	{
		OutError = TEXT("Could not resolve ForgePilot snapshot directory.");
		return false;
	}

	const FString PackageName = Blueprint->GetOutermost()->GetName();
	const FString PackageFilename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	if (!FPaths::FileExists(PackageFilename))
	{
		OutError = FString::Printf(TEXT("Blueprint package file does not exist on disk: %s"), *PackageFilename);
		return false;
	}

	const FString BlueprintToken = SanitizeBlueprintMutationToken(Blueprint->GetName());
	const FString ActionToken = SanitizeBlueprintMutationToken(MutationAction);
	const FString SnapshotDir = FPaths::Combine(SnapshotRoot, BlueprintToken);
	IFileManager::Get().MakeDirectory(*SnapshotDir, true);

	const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
	OutSnapshotPath = FPaths::Combine(SnapshotDir, FString::Printf(TEXT("%s_%s_%s%s"), *BlueprintToken, *ActionToken, *Timestamp, *FPackageName::GetAssetPackageExtension()));

	const uint32 CopyResult = IFileManager::Get().Copy(*OutSnapshotPath, *PackageFilename, true, true);
	if (CopyResult != COPY_OK)
	{
		OutError = FString::Printf(TEXT("Failed to create blueprint snapshot (copy code %u)."), CopyResult);
		OutSnapshotPath.Reset();
		return false;
	}

	return true;
}

static TSharedPtr<FJsonObject> BuildBlueprintMutationPatchReport(
	const FString& Action,
	UBlueprint* Blueprint,
	const FString& SnapshotPath,
	const bool bSaveSucceeded,
	const TSharedPtr<FJsonObject>& Details = nullptr)
{
	TSharedPtr<FJsonObject> PatchReport = MakeShareable(new FJsonObject());
	PatchReport->SetStringField(TEXT("action"), Action);
	PatchReport->SetStringField(TEXT("blueprint_path"), Blueprint ? Blueprint->GetPathName() : TEXT(""));
	PatchReport->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	PatchReport->SetStringField(TEXT("compile_status"), Blueprint ? BlueprintStatusToString(Blueprint->Status) : TEXT("unknown"));
	PatchReport->SetBoolField(TEXT("compile_succeeded"), Blueprint ? (Blueprint->Status == BS_UpToDate || Blueprint->Status == BS_UpToDateWithWarnings) : false);
	PatchReport->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	if (Details.IsValid())
	{
		PatchReport->SetObjectField(TEXT("details"), Details);
	}
	return PatchReport;
}

static FString GetBlueprintPackageFilename(UBlueprint* Blueprint);

static void AppendBlueprintMutationAudit(
	TSharedPtr<FJsonObject> Result,
	const FString& ToolName,
	UBlueprint* Blueprint,
	const FString& SnapshotPath,
	const bool bSucceeded,
	const FString& Message,
	const TSharedPtr<FJsonObject>& Details = nullptr)
{
	if (!Result.IsValid() || !Blueprint)
	{
		return;
	}

	FString AuditEntryPath;
	const bool bAuditRecorded = FVFXMutationAudit::RecordAssetMutation(
		ToolName,
		Blueprint->GetPathName(),
		TEXT("Blueprint"),
		ToolName,
		SnapshotPath,
		GetBlueprintPackageFilename(Blueprint),
		bSucceeded,
		Message,
		AuditEntryPath,
		Details);

	Result->SetBoolField(TEXT("audit_recorded"), bAuditRecorded);
	Result->SetStringField(TEXT("audit_entry_path"), AuditEntryPath);
}

static FString GetBlueprintPackageFilename(UBlueprint* Blueprint)
{
	if (!Blueprint)
	{
		return FString();
	}

	return FPackageName::LongPackageNameToFilename(
		Blueprint->GetOutermost()->GetName(),
		FPackageName::GetAssetPackageExtension());
}

static void GetBlueprintSnapshotFiles(UBlueprint* Blueprint, TArray<FString>& OutSnapshotFiles)
{
	OutSnapshotFiles.Reset();
	if (!Blueprint)
	{
		return;
	}

	const FString SnapshotRoot = GetBlueprintMutationSnapshotRoot();
	if (SnapshotRoot.IsEmpty())
	{
		return;
	}

	const FString SnapshotDir = FPaths::Combine(SnapshotRoot, SanitizeBlueprintMutationToken(Blueprint->GetName()));
	if (!IFileManager::Get().DirectoryExists(*SnapshotDir))
	{
		return;
	}

	IFileManager::Get().FindFiles(OutSnapshotFiles, *FPaths::Combine(SnapshotDir, TEXT("*.uasset")), true, false);
	for (FString& FileName : OutSnapshotFiles)
	{
		FileName = FPaths::Combine(SnapshotDir, FileName);
	}

	OutSnapshotFiles.Sort([](const FString& Left, const FString& Right)
	{
		return IFileManager::Get().GetTimeStamp(*Left) > IFileManager::Get().GetTimeStamp(*Right);
	});
}

static bool IsSnapshotPathUnderRoot(const FString& SnapshotPath)
{
	const FString SnapshotRoot = FPaths::ConvertRelativePathToFull(GetBlueprintMutationSnapshotRoot());
	const FString FullSnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
	return !SnapshotRoot.IsEmpty() && FullSnapshotPath.StartsWith(SnapshotRoot, ESearchCase::IgnoreCase);
}

// ============================================================
// Registration
// ============================================================

void FVFXToolRegistry::RegisterBlueprintTools()
{
	// ---- Tool: get_blueprint_summary ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_summary");
		Def.Description = TEXT(
			"Get a structured summary of a Blueprint's contents: parent class, list of variables (name + type), "
			"components (name + class), and event graph event names. Useful for understanding a Blueprint before modifying it.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint (e.g. '/Game/VFX/BP_Emitter.BP_Emitter')")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintSummary);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_variables ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_variables");
		Def.Description = TEXT(
			"Get all member variables defined in a Blueprint with their names, types, default values, and property flags "
			"(editable, expose on spawn, etc.).");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint to inspect")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintVariables);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_components ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_components");
		Def.Description = TEXT(
			"Get all components in a Blueprint's component hierarchy with names, classes, parent-child relationships, "
			"and key property values (location, intensity, mesh asset).");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint to inspect")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintComponents);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_node_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_node_details");
		Def.Description = TEXT(
			"Get detailed information for one Blueprint node by node guid, including pins and links.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Props->SetObjectField(TEXT("node_guid"),
			MakeStringProperty(TEXT("Node guid string, e.g. 12345678-1234-1234-1234-1234567890ab")));
		Props->SetObjectField(TEXT("include_links"),
			MakeStringProperty(TEXT("Optional bool. Include linked pin details. Default true")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("node_guid"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintNodeDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_blueprint_node_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_blueprint_node_details");
		Def.Description = TEXT(
			"Get details for currently selected Blueprint graph nodes in the editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Optional Blueprint path filter")));
		Props->SetObjectField(TEXT("include_links"),
			MakeStringProperty(TEXT("Optional bool. Include linked pin details. Default true")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedBlueprintNodeDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_inventory ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_inventory");
		Def.Description = TEXT(
			"Get a My Blueprint style inventory: variables, graphs, functions, macros, dispatchers and interfaces.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintInventory);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_component_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_component_details");
		Def.Description = TEXT(
			"Get detailed component hierarchy and key component default properties from a Blueprint.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Props->SetObjectField(TEXT("component_name"),
			MakeStringProperty(TEXT("Optional component name to inspect only one component")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintComponentDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_graph_layout ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_graph_layout");
		Def.Description = TEXT(
			"Get graph-level layout details including graph list, comment boxes and node summaries.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Props->SetObjectField(TEXT("include_node_titles"),
			MakeStringProperty(TEXT("Optional bool. Include node titles. Default false")));
		Props->SetObjectField(TEXT("max_nodes_per_graph"),
			MakeStringProperty(TEXT("Optional max nodes per graph in output. Default 200")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintGraphLayout);
		RegisterTool(Def);
	}

	// ---- Tool: search_blueprint_nodes ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("search_blueprint_nodes");
		Def.Description = TEXT(
			"Search Blueprint nodes by text query in title/class/comment/pin names.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Props->SetObjectField(TEXT("query"),
			MakeStringProperty(TEXT("Query text to search in nodes")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max results. Default 100")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("query"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SearchBlueprintNodes);
		RegisterTool(Def);
	}

	// ---- Tool: find_blueprint_asset_usage ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("find_blueprint_asset_usage");
		Def.Description = TEXT(
			"Find where a specific asset is referenced inside Blueprint graph pins, class defaults, array defaults, "
			"and component template defaults. Can inspect one Blueprint or scan Blueprint assets project-wide.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset path to find, for example '/Game/VFX/NS_Fire.NS_Fire'.")));
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Optional Blueprint path to inspect only one Blueprint. If omitted, scans Blueprint assets project-wide.")));
		Props->SetObjectField(TEXT("max_blueprints"),
			MakeStringProperty(TEXT("Optional max Blueprint assets to inspect when blueprint_path is omitted. Default 5000.")));
		Props->SetObjectField(TEXT("max_results_per_blueprint"),
			MakeStringProperty(TEXT("Optional cap per Blueprint. Default 100.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FindBlueprintAssetUsage);
		RegisterTool(Def);
	}

	// ---- Tool: trace_blueprint_exec_chain ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("trace_blueprint_exec_chain");
		Def.Description = TEXT(
			"Trace forward exec-chain paths from a Blueprint event/function/node query. "
			"Useful for understanding exactly what will fire after a suspicious event node.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint.")));
		Props->SetObjectField(TEXT("node_guid"),
			MakeStringProperty(TEXT("Optional seed node guid.")));
		Props->SetObjectField(TEXT("event_name"),
			MakeStringProperty(TEXT("Optional event name, such as 'ReceiveAnyDamage'.")));
		Props->SetObjectField(TEXT("function_name"),
			MakeStringProperty(TEXT("Optional function/member name, such as 'SpawnSystemAttached'.")));
		Props->SetObjectField(TEXT("node_title_query"),
			MakeStringProperty(TEXT("Optional node title/class/comment query.")));
		Props->SetObjectField(TEXT("max_depth"),
			MakeStringProperty(TEXT("Optional max exec depth. Default 8.")));
		Props->SetObjectField(TEXT("max_paths"),
			MakeStringProperty(TEXT("Optional max returned paths. Default 50.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_TraceBlueprintExecChain);
		RegisterTool(Def);
	}

	// ---- Tool: summarize_blueprint_value_flow ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("summarize_blueprint_value_flow");
		Def.Description = TEXT(
			"Summarize value flow around a Blueprint variable/parameter/query. "
			"Returns matched seed nodes, nearby connected nodes, links, and a human-readable flow summary.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint.")));
		Props->SetObjectField(TEXT("variable_name"),
			MakeStringProperty(TEXT("Optional variable name to seed flow search.")));
		Props->SetObjectField(TEXT("parameter_name"),
			MakeStringProperty(TEXT("Optional material parameter name to seed flow search (e.g. 'HotValue').")));
		Props->SetObjectField(TEXT("node_title_query"),
			MakeStringProperty(TEXT("Optional query to match node title/class/comment/pins.")));
		Props->SetObjectField(TEXT("max_depth"),
			MakeStringProperty(TEXT("Optional BFS depth from seed nodes. Default 3, range 1..8.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SummarizeBlueprintValueFlow);
		RegisterTool(Def);
	}

	// ---- Tool: summarize_blueprint_material_runtime ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("summarize_blueprint_material_runtime");
		Def.Description = TEXT(
			"Trace runtime material parameter writes in a Blueprint. "
			"Finds dynamic material instance creation nodes and Set Scalar/Vector/Texture Parameter Value nodes, "
			"groups them by parameter name, and summarizes execution/value sources.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint.")));
		Props->SetObjectField(TEXT("parameter_name"),
			MakeStringProperty(TEXT("Optional material parameter name to filter writes (e.g. 'HotValue').")));
		Props->SetObjectField(TEXT("material_asset_path"),
			MakeStringProperty(TEXT("Optional Material or MIC asset path to filter nearby dynamic material creation nodes.")));
		Props->SetObjectField(TEXT("material_instance_path"),
			MakeStringProperty(TEXT("Optional Material Instance path to cross-reference overridden values back to the parent material graph.")));
		Props->SetObjectField(TEXT("node_title_query"),
			MakeStringProperty(TEXT("Optional query to match node title/class/comment/pins.")));
		Props->SetObjectField(TEXT("max_depth"),
			MakeStringProperty(TEXT("Optional BFS depth from seed nodes. Default 3, range 1..8.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SummarizeBlueprintMaterialRuntime);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_class_defaults ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_class_defaults");
		Def.Description = TEXT(
			"Get Blueprint class default values from the generated class default object.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintClassDefaults);
		RegisterTool(Def);
	}

	// ---- Tool: inspect_blueprint_array_defaults ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("inspect_blueprint_array_defaults");
		Def.Description = TEXT(
			"Inspect array defaults on a Blueprint class default object. "
			"Useful for pooled effect arrays like BloodEffects or GibEffects.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint.")));
		Props->SetObjectField(TEXT("array_name"),
			MakeStringProperty(TEXT("Optional specific array property name to inspect. If omitted, all array defaults are returned.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_InspectBlueprintArrayDefaults);
		RegisterTool(Def);
	}

	// ---- Tool: get_blueprint_mutation_snapshots ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_blueprint_mutation_snapshots");
		Def.Description = TEXT("List saved mutation snapshots for a Blueprint, ordered newest first. Use this before restore_blueprint_mutation_snapshot.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint.")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max number of snapshots to return. Default 20.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetBlueprintMutationSnapshots);
		RegisterTool(Def);
	}

	// ---- Tool: create_blueprint ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_blueprint");
		Def.Description = TEXT(
			"Create a new Blueprint asset. Supports specifying a parent class (default: Actor) and save location. "
			"Returns the asset path of the created Blueprint.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_name"),
			MakeStringProperty(TEXT("Name for the new Blueprint (e.g. 'BP_VFXEmitter')")));
		Props->SetObjectField(TEXT("parent_class"),
			MakeStringProperty(TEXT("Parent class name. Default: 'Actor'. Also supports: Pawn, Character, ActorComponent, etc.")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Content Browser folder path (e.g. '/Game/VFX/Blueprints/'). Default: '/Game/'")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to create asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateBlueprint);
		RegisterTool(Def);
	}

	// ---- Tool: add_variable_to_blueprint ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_variable_to_blueprint");
		Def.Description = TEXT(
			"Add a member variable to an existing Blueprint. "
			"Supports types: bool, int, float, double, string, text, name, byte, int64, Vector, Rotator, Transform, LinearColor. "
			"For object/struct/enum types, provide the full asset path.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the target Blueprint")));
		Props->SetObjectField(TEXT("variable_name"),
			MakeStringProperty(TEXT("Name of the variable to add (e.g. 'SpawnRate')")));
		Props->SetObjectField(TEXT("variable_type"),
			MakeStringProperty(TEXT("Variable type: bool, int, float, double, string, Vector, Rotator, Transform, LinearColor, or full asset path for struct/enum/object types")));
		Props->SetObjectField(TEXT("default_value"),
			MakeStringProperty(TEXT("Optional default value as string (e.g. '5.0', 'true')")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply changes.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("variable_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("variable_type"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddVariable);
		RegisterTool(Def);
	}

	// ---- Tool: add_component_to_blueprint ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_component_to_blueprint");
		Def.Description = TEXT(
			"Add a component to a Blueprint's component hierarchy. "
			"Common VFX components: NiagaraComponent, StaticMeshComponent, PointLightComponent, SpotLightComponent, "
			"AudioComponent, ArrowComponent, SphereCollision, BoxCollision, SceneComponent.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the target Blueprint")));
		Props->SetObjectField(TEXT("component_class"),
			MakeStringProperty(TEXT("Component class name (e.g. 'NiagaraComponent', 'StaticMeshComponent', 'PointLightComponent')")));
		Props->SetObjectField(TEXT("component_name"),
			MakeStringProperty(TEXT("Name for the new component instance (e.g. 'VFXEmitter', 'MeshComp')")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply changes.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("component_class"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("component_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddComponent);
		RegisterTool(Def);
	}

	// ---- Tool: edit_component_property ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("edit_component_property");
		Def.Description = TEXT(
			"Set a default property value on a component in a Blueprint. Uses the UE5 reflection system. "
			"For Vector values use '(X=1.0,Y=2.0,Z=3.0)', for Rotator '(Pitch=0,Yaw=90,Roll=0)', "
			"for colors '(R=1.0,G=0.5,B=0.0,A=1.0)', booleans 'true'/'false', numbers '100.0'.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the target Blueprint")));
		Props->SetObjectField(TEXT("component_name"),
			MakeStringProperty(TEXT("Name of the component to edit")));
		Props->SetObjectField(TEXT("property_name"),
			MakeStringProperty(TEXT("Name of the property to set (e.g. 'Intensity', 'RelativeLocation', 'bVisible')")));
		Props->SetObjectField(TEXT("property_value"),
			MakeStringProperty(TEXT("New value as string. Vectors: '(X=1,Y=2,Z=3)', booleans: 'true', numbers: '100.0'")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply changes.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("component_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("property_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("property_value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_EditComponentProperty);
		RegisterTool(Def);
	}

	// ---- Tool: create_custom_event_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_custom_event_node");
		Def.Description = TEXT(
			"Create a Custom Event node in a Blueprint graph. "
			"Useful for wiring gameplay entry points like OnDeath or FireWeapon without hand-editing the Event Graph.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("graph_name"), MakeStringProperty(TEXT("Target graph name, e.g. EventGraph.")));
		Props->SetObjectField(TEXT("event_name"), MakeStringProperty(TEXT("Name of the custom event to create, e.g. OnDeath.")));
		Props->SetObjectField(TEXT("node_pos_x"), MakeStringProperty(TEXT("Optional X graph position. Default 0.")));
		Props->SetObjectField(TEXT("node_pos_y"), MakeStringProperty(TEXT("Optional Y graph position. Default 0.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to add the node.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("graph_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("event_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateCustomEventNode);
		RegisterTool(Def);
	}

	// ---- Tool: create_blueprint_call_function_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_blueprint_call_function_node");
		Def.Description = TEXT(
			"Create a Blueprint call-function node in a target graph. "
			"Useful for wiring Niagara spawn, socket queries, delays, and other runtime logic without hand-editing graphs.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("graph_name"), MakeStringProperty(TEXT("Target graph name, e.g. EventGraph.")));
		Props->SetObjectField(TEXT("function_owner_class"), MakeStringProperty(TEXT("Owning class path or native class name, e.g. '/Script/Niagara.NiagaraFunctionLibrary' or 'NiagaraFunctionLibrary'.")));
		Props->SetObjectField(TEXT("function_name"), MakeStringProperty(TEXT("Exact UFunction name, e.g. 'SpawnSystemAttached'.")));
		Props->SetObjectField(TEXT("node_pos_x"), MakeStringProperty(TEXT("Optional X graph position. Default 0.")));
		Props->SetObjectField(TEXT("node_pos_y"), MakeStringProperty(TEXT("Optional Y graph position. Default 0.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to add the node.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("graph_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("function_owner_class"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("function_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateBlueprintCallFunctionNode);
		RegisterTool(Def);
	}

	// ---- Tool: create_blueprint_variable_set_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_blueprint_variable_set_node");
		Def.Description = TEXT(
			"Create a Variable Set node in a Blueprint graph for an existing Blueprint variable, inherited property, or component member. "
			"Useful for pushing runtime values like damage, charge, or Niagara user parameters through Blueprint logic.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("graph_name"), MakeStringProperty(TEXT("Target graph name, e.g. EventGraph or a function graph name.")));
		Props->SetObjectField(TEXT("variable_name"), MakeStringProperty(TEXT("Exact variable/component/property name to create a setter for.")));
		Props->SetObjectField(TEXT("node_pos_x"), MakeStringProperty(TEXT("Optional X graph position. Default 0.")));
		Props->SetObjectField(TEXT("node_pos_y"), MakeStringProperty(TEXT("Optional Y graph position. Default 0.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to add the node.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("graph_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("variable_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateBlueprintVariableSetNode);
		RegisterTool(Def);
	}

	// ---- Tool: create_blueprint_variable_get_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_blueprint_variable_get_node");
		Def.Description = TEXT(
			"Create a Variable Get node in a Blueprint graph for an existing Blueprint variable, inherited property, or component member. "
			"Useful for wiring Event Graph logic without manually opening the graph editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("graph_name"), MakeStringProperty(TEXT("Target graph name, e.g. EventGraph or a function graph name.")));
		Props->SetObjectField(TEXT("variable_name"), MakeStringProperty(TEXT("Exact variable/component/property name to create a getter for.")));
		Props->SetObjectField(TEXT("node_pos_x"), MakeStringProperty(TEXT("Optional X graph position. Default 0.")));
		Props->SetObjectField(TEXT("node_pos_y"), MakeStringProperty(TEXT("Optional Y graph position. Default 0.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to add the node.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("graph_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("variable_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateBlueprintVariableGetNode);
		RegisterTool(Def);
	}

	// ---- Tool: attach_component_to_component_socket ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("attach_component_to_component_socket");
		Def.Description = TEXT(
			"Attach one Blueprint scene component under another component and optionally set a socket or bone name. "
			"Useful for batch-fixing Niagara, audio, and muzzle-flash component hierarchies without opening the Blueprint editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("child_component_name"), MakeStringProperty(TEXT("Name of the component that should be attached as the child.")));
		Props->SetObjectField(TEXT("parent_component_name"), MakeStringProperty(TEXT("Name of the component that should become the new parent.")));
		Props->SetObjectField(TEXT("socket_name"), MakeStringProperty(TEXT("Optional socket or bone name to attach to, e.g. Muzzle.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to apply the attachment.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("child_component_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("parent_component_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AttachComponentToComponentSocket);
		RegisterTool(Def);
	}

	// ---- Tool: delete_blueprint_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("delete_blueprint_node");
		Def.Description = TEXT(
			"Delete a Blueprint graph node by guid. Useful for replacing broken auto-generated nodes safely.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("node_guid"), MakeStringProperty(TEXT("Guid of the node to delete.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to delete the node.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("node_guid"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DeleteBlueprintNode);
		RegisterTool(Def);
	}

	// ---- Tool: connect_blueprint_pins ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("connect_blueprint_pins");
		Def.Description = TEXT(
			"Connect one Blueprint node pin to another, with optional rewiring of existing links. "
			"Works for exec and data pins.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("from_node_guid"), MakeStringProperty(TEXT("Source node guid.")));
		Props->SetObjectField(TEXT("from_pin_name"), MakeStringProperty(TEXT("Source pin name.")));
		Props->SetObjectField(TEXT("to_node_guid"), MakeStringProperty(TEXT("Target node guid.")));
		Props->SetObjectField(TEXT("to_pin_name"), MakeStringProperty(TEXT("Target pin name.")));
		Props->SetObjectField(TEXT("break_source_links"), MakeStringProperty(TEXT("Optional bool-like flag. Break existing links on the source pin first. Default false.")));
		Props->SetObjectField(TEXT("break_target_links"), MakeStringProperty(TEXT("Optional bool-like flag. Break existing links on the target pin first. Default false.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to connect pins.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true when apply_changes is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("from_node_guid"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("from_pin_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("to_node_guid"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("to_pin_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ConnectBlueprintPins);
		RegisterTool(Def);
	}

	// ---- Tool: set_blueprint_node_pin_default ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_blueprint_node_pin_default");
		Def.Description = TEXT(
			"Set the default value on a Blueprint node pin by node guid and pin name. "
			"Supports scalar/text values and object reference pins when given an asset path.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to edit.")));
		Props->SetObjectField(TEXT("node_guid"), MakeStringProperty(TEXT("Node guid string, e.g. 12345678-1234-1234-1234-1234567890ab.")));
		Props->SetObjectField(TEXT("pin_name"), MakeStringProperty(TEXT("Exact pin name to edit.")));
		Props->SetObjectField(TEXT("default_value"), MakeStringProperty(TEXT("New default value string or full asset path for object pins.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("node_guid"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("pin_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("default_value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetBlueprintNodePinDefault);
		RegisterTool(Def);
	}

	// ---- Tool: compile_blueprint_with_feedback ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("compile_blueprint_with_feedback");
		Def.Description = TEXT(
			"Compile a Blueprint asset, optionally save it, and return compile status feedback.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"), MakeStringProperty(TEXT("Full asset path of the Blueprint to compile.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run only). Set true to compile the Blueprint.")));
		Props->SetObjectField(TEXT("save_asset"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CompileBlueprintWithFeedback);
		RegisterTool(Def);
	}

	// ---- Tool: restore_blueprint_mutation_snapshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("restore_blueprint_mutation_snapshot");
		Def.Description = TEXT("Restore a Blueprint package from a saved snapshot created by ForgePilot, then reload the package in editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint to restore.")));
		Props->SetObjectField(TEXT("snapshot_path"),
			MakeStringProperty(TEXT("Optional absolute snapshot file path. If omitted, the latest snapshot is used.")));
		Props->SetObjectField(TEXT("use_latest"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Use the newest snapshot when snapshot_path is omitted.")));
		Props->SetObjectField(TEXT("reopen_editor"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Re-open the Blueprint editor after restore.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually restore the snapshot. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RestoreBlueprintMutationSnapshot);
		RegisterTool(Def);
	}

	// ---- Tool: review_blueprint_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_blueprint_asset");
		Def.Description = TEXT(
			"Analyze a Blueprint and return structured findings for maintainability, performance risks, and setup quality.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("blueprint_path"),
			MakeStringProperty(TEXT("Full asset path of the Blueprint to review")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("blueprint_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewBlueprintAsset);
		RegisterTool(Def);
	}
}

// ============================================================
// Helper: ConvertTypeToPinType
// ============================================================

bool FVFXToolRegistry::ConvertTypeToPinType(const FString& TypeStr, FEdGraphPinType& OutPinType)
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	if (TypeStr.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Boolean;
	else if (TypeStr.Equals(TEXT("byte"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Byte;
	else if (TypeStr.Equals(TEXT("int"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Int;
	else if (TypeStr.Equals(TEXT("int64"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Int64;
	else if (TypeStr.Equals(TEXT("float"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Float;
	else if (TypeStr.Equals(TEXT("double"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Double;
	else if (TypeStr.Equals(TEXT("real"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Real;
	else if (TypeStr.Equals(TEXT("string"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_String;
	else if (TypeStr.Equals(TEXT("text"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Text;
	else if (TypeStr.Equals(TEXT("name"), ESearchCase::IgnoreCase))
		OutPinType.PinCategory = K2Schema->PC_Name;
	else if (TypeStr.Equals(TEXT("Vector"), ESearchCase::IgnoreCase))
	{
		OutPinType.PinCategory = K2Schema->PC_Struct;
		OutPinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();
	}
	else if (TypeStr.Equals(TEXT("Rotator"), ESearchCase::IgnoreCase))
	{
		OutPinType.PinCategory = K2Schema->PC_Struct;
		OutPinType.PinSubCategoryObject = TBaseStructure<FRotator>::Get();
	}
	else if (TypeStr.Equals(TEXT("Transform"), ESearchCase::IgnoreCase))
	{
		OutPinType.PinCategory = K2Schema->PC_Struct;
		OutPinType.PinSubCategoryObject = TBaseStructure<FTransform>::Get();
	}
	else if (TypeStr.Equals(TEXT("LinearColor"), ESearchCase::IgnoreCase) || TypeStr.Equals(TEXT("Color"), ESearchCase::IgnoreCase))
	{
		OutPinType.PinCategory = K2Schema->PC_Struct;
		OutPinType.PinSubCategoryObject = TBaseStructure<FLinearColor>::Get();
	}
	else if (TypeStr.Equals(TEXT("Vector2D"), ESearchCase::IgnoreCase))
	{
		OutPinType.PinCategory = K2Schema->PC_Struct;
		OutPinType.PinSubCategoryObject = TBaseStructure<FVector2D>::Get();
	}
	else
	{
		// Try to find as asset path (struct/enum/class)
		UObject* FoundType = LoadObject<UObject>(nullptr, *TypeStr);
		if (!FoundType)
		{
			FoundType = FindFirstObject<UObject>(*TypeStr, EFindFirstObjectOptions::NativeFirst);
		}

		if (UClass* FoundClass = Cast<UClass>(FoundType))
		{
			OutPinType.PinCategory = K2Schema->PC_Object;
			OutPinType.PinSubCategoryObject = FoundClass;
		}
		else if (UScriptStruct* FoundStruct = Cast<UScriptStruct>(FoundType))
		{
			OutPinType.PinCategory = K2Schema->PC_Struct;
			OutPinType.PinSubCategoryObject = FoundStruct;
		}
		else if (UEnum* FoundEnum = Cast<UEnum>(FoundType))
		{
			OutPinType.PinCategory = K2Schema->PC_Byte;
			OutPinType.PinSubCategoryObject = FoundEnum;
		}
		else
		{
			return false;
		}
	}

	return true;
}

// ============================================================
// Blueprint Tool Implementations
// ============================================================

bool FVFXToolRegistry::Tool_GetBlueprintSummary(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());

	// Parent class
	Result->SetStringField(TEXT("parent_class"),
		Blueprint->ParentClass ? Blueprint->ParentClass->GetName() : TEXT("None"));

	// Variables
	TArray<TSharedPtr<FJsonValue>> VarsArray;
	for (const FBPVariableDescription& VarDesc : Blueprint->NewVariables)
	{
		TSharedPtr<FJsonObject> VarObj = MakeShareable(new FJsonObject());
		VarObj->SetStringField(TEXT("name"), VarDesc.VarName.ToString());
		VarObj->SetStringField(TEXT("type"), PinTypeToDisplayString(VarDesc.VarType));
		if (VarDesc.VarType.ContainerType == EPinContainerType::Array)
			VarObj->SetStringField(TEXT("container"), TEXT("Array"));
		VarsArray.Add(MakeShareable(new FJsonValueObject(VarObj)));
	}
	Result->SetArrayField(TEXT("variables"), VarsArray);

	// Components
	TArray<TSharedPtr<FJsonValue>> CompsArray;
	if (Blueprint->SimpleConstructionScript)
	{
		const TArray<USCS_Node*>& AllNodes = Blueprint->SimpleConstructionScript->GetAllNodes();
		for (const USCS_Node* Node : AllNodes)
		{
			if (!Node || !Node->ComponentTemplate) continue;
			TSharedPtr<FJsonObject> CompObj = MakeShareable(new FJsonObject());
			CompObj->SetStringField(TEXT("name"), Node->GetVariableName().ToString());
			CompObj->SetStringField(TEXT("class"), Node->ComponentTemplate->GetClass()->GetName());
			CompsArray.Add(MakeShareable(new FJsonValueObject(CompObj)));
		}
	}
	Result->SetArrayField(TEXT("components"), CompsArray);

	// Events in event graph
	TArray<TSharedPtr<FJsonValue>> EventsArray;
	UEdGraph* EventGraph = FBlueprintEditorUtils::FindEventGraph(Blueprint);
	if (EventGraph)
	{
		for (UEdGraphNode* Node : EventGraph->Nodes)
		{
			if (UK2Node_Event* EventNode = Cast<UK2Node_Event>(Node))
			{
				EventsArray.Add(MakeShareable(new FJsonValueString(
					EventNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString())));
			}
		}
	}
	Result->SetArrayField(TEXT("events"), EventsArray);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintVariables(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> VarsArray;
	for (const FBPVariableDescription& VarDesc : Blueprint->NewVariables)
	{
		TSharedPtr<FJsonObject> VarObj = MakeShareable(new FJsonObject());
		VarObj->SetStringField(TEXT("name"), VarDesc.VarName.ToString());
		VarObj->SetStringField(TEXT("type"), PinTypeToDisplayString(VarDesc.VarType));

		if (VarDesc.VarType.ContainerType == EPinContainerType::Array)
			VarObj->SetStringField(TEXT("container"), TEXT("Array"));

		if (!VarDesc.DefaultValue.IsEmpty())
			VarObj->SetStringField(TEXT("default_value"), VarDesc.DefaultValue);

		if (!VarDesc.Category.IsEmpty())
			VarObj->SetStringField(TEXT("category"), VarDesc.Category.ToString());

		VarObj->SetBoolField(TEXT("is_editable"),
			(VarDesc.PropertyFlags & CPF_Edit) != 0);
		VarObj->SetBoolField(TEXT("expose_on_spawn"),
			(VarDesc.PropertyFlags & CPF_ExposeOnSpawn) != 0);
		VarObj->SetBoolField(TEXT("blueprint_read_only"),
			(VarDesc.PropertyFlags & CPF_BlueprintReadOnly) != 0);

		VarsArray.Add(MakeShareable(new FJsonValueObject(VarObj)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetNumberField(TEXT("count"), VarsArray.Num());
	Result->SetArrayField(TEXT("variables"), VarsArray);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintComponents(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> CompsArray;

	if (Blueprint->SimpleConstructionScript)
	{
		const TArray<USCS_Node*> RootNodes = Blueprint->SimpleConstructionScript->GetRootNodes();
		const TArray<USCS_Node*>& AllNodes = Blueprint->SimpleConstructionScript->GetAllNodes();

		for (const USCS_Node* Node : AllNodes)
		{
			if (!Node || !Node->ComponentTemplate) continue;

			TSharedPtr<FJsonObject> CompObj = MakeShareable(new FJsonObject());
			CompObj->SetStringField(TEXT("name"), Node->GetVariableName().ToString());
			CompObj->SetStringField(TEXT("class"), Node->ComponentTemplate->GetClass()->GetName());
			CompObj->SetBoolField(TEXT("is_root"), RootNodes.Contains(Node));

			// Parent info
			if (!Node->ParentComponentOrVariableName.IsNone())
			{
				CompObj->SetStringField(TEXT("parent"), Node->ParentComponentOrVariableName.ToString());
			}

			// Key properties for SceneComponent
			if (USceneComponent* SceneComp = Cast<USceneComponent>(Node->ComponentTemplate))
			{
				FVector Loc = SceneComp->GetRelativeLocation();
				if (!Loc.IsZero())
				{
					CompObj->SetStringField(TEXT("relative_location"),
						FString::Printf(TEXT("(%.1f, %.1f, %.1f)"), Loc.X, Loc.Y, Loc.Z));
				}

				FRotator Rot = SceneComp->GetRelativeRotation();
				if (!Rot.IsZero())
				{
					CompObj->SetStringField(TEXT("relative_rotation"),
						FString::Printf(TEXT("(P=%.1f, Y=%.1f, R=%.1f)"), Rot.Pitch, Rot.Yaw, Rot.Roll));
				}

				FVector Scale = SceneComp->GetRelativeScale3D();
				if (!Scale.Equals(FVector::OneVector))
				{
					CompObj->SetStringField(TEXT("relative_scale"),
						FString::Printf(TEXT("(%.1f, %.1f, %.1f)"), Scale.X, Scale.Y, Scale.Z));
				}
			}

			// Light intensity
			if (ULightComponent* LightComp = Cast<ULightComponent>(Node->ComponentTemplate))
			{
				CompObj->SetNumberField(TEXT("intensity"), LightComp->Intensity);
			}

			// Static mesh
			if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Node->ComponentTemplate))
			{
				if (UStaticMesh* Mesh = MeshComp->GetStaticMesh())
				{
					CompObj->SetStringField(TEXT("static_mesh"), Mesh->GetPathName());
				}
			}

			CompsArray.Add(MakeShareable(new FJsonValueObject(CompObj)));
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetNumberField(TEXT("count"), CompsArray.Num());
	Result->SetArrayField(TEXT("components"), CompsArray);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintNodeDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	FString NodeGuidString;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) ||
		!Input->TryGetStringField(TEXT("node_guid"), NodeGuidString))
	{
		OutResult = TEXT("Missing required fields: blueprint_path, node_guid");
		return false;
	}

	FGuid NodeGuid;
	if (!FGuid::Parse(NodeGuidString, NodeGuid))
	{
		OutResult = FString::Printf(TEXT("Invalid node_guid format: %s"), *NodeGuidString);
		return false;
	}

	bool bIncludeLinks = true;
	Input->TryGetBoolField(TEXT("include_links"), bIncludeLinks);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;
		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node) continue;
			if (Node->NodeGuid == NodeGuid)
			{
				TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
				ResultObj->SetBoolField(TEXT("success"), true);
				ResultObj->SetObjectField(TEXT("node"), BuildBlueprintNodeDetailsObject(Node, bIncludeLinks));
				OutResult = JsonObjToString(ResultObj);
				return true;
			}
		}
	}

	OutResult = FString::Printf(TEXT("Node with guid '%s' was not found in Blueprint '%s'."), *NodeGuidString, *BpPath);
	return false;
}

bool FVFXToolRegistry::Tool_GetSelectedBlueprintNodeDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString BlueprintPathFilter;
	bool bIncludeLinks = true;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("blueprint_path"), BlueprintPathFilter);
		Input->TryGetBoolField(TEXT("include_links"), bIncludeLinks);
	}

	if (!GEditor)
	{
		OutResult = TEXT("Editor is not available.");
		return false;
	}

	USelection* SelectedObjects = GEditor->GetSelectedObjects();
	if (!SelectedObjects)
	{
		OutResult = TEXT("No editor selection available.");
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> NodesArray;
	for (FSelectionIterator It(*SelectedObjects); It; ++It)
	{
		UObject* SelectedObj = *It;
		UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedObj);
		if (!Node || !Node->GetGraph()) continue;

		UBlueprint* OwnerBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(Node->GetGraph());
		if (!BlueprintPathFilter.IsEmpty())
		{
			if (!OwnerBlueprint || !OwnerBlueprint->GetPathName().Equals(BlueprintPathFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		NodesArray.Add(MakeShareable(new FJsonValueObject(
			BuildBlueprintNodeDetailsObject(Node, bIncludeLinks))));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetNumberField(TEXT("count"), NodesArray.Num());
	ResultObj->SetArrayField(TEXT("nodes"), NodesArray);
	ResultObj->SetBoolField(TEXT("success"), NodesArray.Num() > 0);

	if (NodesArray.Num() == 0)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("No Blueprint graph nodes are selected."));
	}

	OutResult = JsonObjToString(ResultObj);
	return NodesArray.Num() > 0;
}

bool FVFXToolRegistry::Tool_GetBlueprintInventory(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> VarsArray;
	for (const FBPVariableDescription& VarDesc : Blueprint->NewVariables)
	{
		TSharedPtr<FJsonObject> VarObj = MakeShareable(new FJsonObject());
		VarObj->SetStringField(TEXT("name"), VarDesc.VarName.ToString());
		VarObj->SetStringField(TEXT("type"), PinTypeToDisplayString(VarDesc.VarType));
		VarsArray.Add(MakeShareable(new FJsonValueObject(VarObj)));
	}

	TArray<TSharedPtr<FJsonValue>> GraphsArray;
	for (UEdGraph* Graph : Blueprint->UbergraphPages)
	{
		if (Graph)
		{
			GraphsArray.Add(MakeShareable(new FJsonValueString(Graph->GetName())));
		}
	}

	TArray<TSharedPtr<FJsonValue>> FunctionsArray;
	for (UEdGraph* Graph : Blueprint->FunctionGraphs)
	{
		if (Graph)
		{
			FunctionsArray.Add(MakeShareable(new FJsonValueString(Graph->GetName())));
		}
	}

	TArray<TSharedPtr<FJsonValue>> MacrosArray;
	for (UEdGraph* Graph : Blueprint->MacroGraphs)
	{
		if (Graph)
		{
			MacrosArray.Add(MakeShareable(new FJsonValueString(Graph->GetName())));
		}
	}

	TArray<TSharedPtr<FJsonValue>> DispatchersArray;
	for (UEdGraph* Graph : Blueprint->DelegateSignatureGraphs)
	{
		if (Graph)
		{
			DispatchersArray.Add(MakeShareable(new FJsonValueString(Graph->GetName())));
		}
	}

	TArray<TSharedPtr<FJsonValue>> InterfacesArray;
	for (const FBPInterfaceDescription& InterfaceDesc : Blueprint->ImplementedInterfaces)
	{
		if (InterfaceDesc.Interface)
		{
			InterfacesArray.Add(MakeShareable(new FJsonValueString(InterfaceDesc.Interface->GetName())));
		}
	}

	TArray<TSharedPtr<FJsonValue>> TimelinesArray;
	for (UTimelineTemplate* Timeline : Blueprint->Timelines)
	{
		if (Timeline)
		{
			TimelinesArray.Add(MakeShareable(new FJsonValueString(Timeline->GetName())));
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("blueprint"), Blueprint->GetPathName());
	ResultObj->SetArrayField(TEXT("variables"), VarsArray);
	ResultObj->SetArrayField(TEXT("graphs"), GraphsArray);
	ResultObj->SetArrayField(TEXT("functions"), FunctionsArray);
	ResultObj->SetArrayField(TEXT("macros"), MacrosArray);
	ResultObj->SetArrayField(TEXT("dispatchers"), DispatchersArray);
	ResultObj->SetArrayField(TEXT("interfaces"), InterfacesArray);
	ResultObj->SetArrayField(TEXT("timelines"), TimelinesArray);
	ResultObj->SetNumberField(TEXT("variable_count"), VarsArray.Num());
	ResultObj->SetNumberField(TEXT("graph_count"), GraphsArray.Num());
	ResultObj->SetNumberField(TEXT("function_count"), FunctionsArray.Num());
	ResultObj->SetNumberField(TEXT("macro_count"), MacrosArray.Num());
	ResultObj->SetNumberField(TEXT("dispatcher_count"), DispatchersArray.Num());
	ResultObj->SetNumberField(TEXT("interface_count"), InterfacesArray.Num());
	ResultObj->SetNumberField(TEXT("timeline_count"), TimelinesArray.Num());

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintComponentDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	FString ComponentNameFilter;
	Input->TryGetStringField(TEXT("component_name"), ComponentNameFilter);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	if (!Blueprint->SimpleConstructionScript)
	{
		OutResult = TEXT("Blueprint has no SimpleConstructionScript.");
		return false;
	}

	const TArray<USCS_Node*>& AllNodes = Blueprint->SimpleConstructionScript->GetAllNodes();
	const TArray<USCS_Node*> RootNodes = Blueprint->SimpleConstructionScript->GetRootNodes();

	TArray<TSharedPtr<FJsonValue>> ComponentsArray;
	for (USCS_Node* Node : AllNodes)
	{
		if (!Node || !Node->ComponentTemplate) continue;
		const FString CurrentName = Node->GetVariableName().ToString();
		if (!ComponentNameFilter.IsEmpty() && !CurrentName.Equals(ComponentNameFilter, ESearchCase::IgnoreCase))
		{
			continue;
		}

		TSharedPtr<FJsonObject> CompObj = MakeShareable(new FJsonObject());
		CompObj->SetStringField(TEXT("name"), CurrentName);
		CompObj->SetStringField(TEXT("class"), Node->ComponentTemplate->GetClass()->GetName());
		CompObj->SetBoolField(TEXT("is_root"), RootNodes.Contains(Node));
		if (!Node->ParentComponentOrVariableName.IsNone())
		{
			CompObj->SetStringField(TEXT("parent"), Node->ParentComponentOrVariableName.ToString());
		}

		TArray<TSharedPtr<FJsonValue>> ChildrenArray;
		for (USCS_Node* ChildNode : Node->GetChildNodes())
		{
			if (!ChildNode) continue;
			ChildrenArray.Add(MakeShareable(new FJsonValueString(ChildNode->GetVariableName().ToString())));
		}
		CompObj->SetArrayField(TEXT("children"), ChildrenArray);

		if (USceneComponent* SceneComp = Cast<USceneComponent>(Node->ComponentTemplate))
		{
			const FVector Loc = SceneComp->GetRelativeLocation();
			const FRotator Rot = SceneComp->GetRelativeRotation();
			const FVector Scale = SceneComp->GetRelativeScale3D();
			CompObj->SetStringField(TEXT("relative_location"),
				FString::Printf(TEXT("(%.3f, %.3f, %.3f)"), Loc.X, Loc.Y, Loc.Z));
			CompObj->SetStringField(TEXT("relative_rotation"),
				FString::Printf(TEXT("(P=%.3f, Y=%.3f, R=%.3f)"), Rot.Pitch, Rot.Yaw, Rot.Roll));
			CompObj->SetStringField(TEXT("relative_scale"),
				FString::Printf(TEXT("(%.3f, %.3f, %.3f)"), Scale.X, Scale.Y, Scale.Z));
			CompObj->SetStringField(TEXT("mobility"), UEnum::GetValueAsString(SceneComp->Mobility));
			CompObj->SetBoolField(TEXT("visible"), SceneComp->GetVisibleFlag());
		}

		if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Node->ComponentTemplate))
		{
			CompObj->SetBoolField(TEXT("cast_shadow"), PrimitiveComp->CastShadow);
			CompObj->SetStringField(TEXT("collision_profile"), PrimitiveComp->GetCollisionProfileName().ToString());
			CompObj->SetStringField(TEXT("collision_enabled"), UEnum::GetValueAsString(PrimitiveComp->GetCollisionEnabled()));
		}

		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Node->ComponentTemplate))
		{
			UStaticMesh* Mesh = MeshComp->GetStaticMesh();
			CompObj->SetStringField(TEXT("static_mesh"), Mesh ? Mesh->GetPathName() : TEXT(""));

			TArray<TSharedPtr<FJsonValue>> MaterialArray;
			for (int32 MaterialIdx = 0; MaterialIdx < MeshComp->GetNumMaterials(); ++MaterialIdx)
			{
				UMaterialInterface* Mat = MeshComp->GetMaterial(MaterialIdx);
				TSharedPtr<FJsonObject> MatObj = MakeShareable(new FJsonObject());
				MatObj->SetNumberField(TEXT("index"), MaterialIdx);
				MatObj->SetStringField(TEXT("material"), Mat ? Mat->GetPathName() : TEXT(""));
				MaterialArray.Add(MakeShareable(new FJsonValueObject(MatObj)));
			}
			CompObj->SetArrayField(TEXT("materials"), MaterialArray);
		}

		if (ULightComponent* LightComp = Cast<ULightComponent>(Node->ComponentTemplate))
		{
			CompObj->SetNumberField(TEXT("intensity"), LightComp->Intensity);
			CompObj->SetStringField(TEXT("light_color"), LightComp->GetLightColor().ToString());
		}

		ComponentsArray.Add(MakeShareable(new FJsonValueObject(CompObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("blueprint"), Blueprint->GetPathName());
	ResultObj->SetStringField(TEXT("component_name_filter"), ComponentNameFilter);
	ResultObj->SetNumberField(TEXT("count"), ComponentsArray.Num());
	ResultObj->SetArrayField(TEXT("components"), ComponentsArray);

	OutResult = JsonObjToString(ResultObj);
	return ComponentsArray.Num() > 0;
}

bool FVFXToolRegistry::Tool_GetBlueprintGraphLayout(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	bool bIncludeNodeTitles = false;
	Input->TryGetBoolField(TEXT("include_node_titles"), bIncludeNodeTitles);
	int32 MaxNodesPerGraph = 200;
	if (Input->HasField(TEXT("max_nodes_per_graph")))
	{
		MaxNodesPerGraph = FMath::Clamp((int32)Input->GetNumberField(TEXT("max_nodes_per_graph")), 1, 2000);
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);

	TArray<TSharedPtr<FJsonValue>> GraphsArray;
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;

		TSharedPtr<FJsonObject> GraphObj = MakeShareable(new FJsonObject());
		GraphObj->SetStringField(TEXT("name"), Graph->GetName());
		GraphObj->SetNumberField(TEXT("node_count"), Graph->Nodes.Num());

		TArray<TSharedPtr<FJsonValue>> CommentArray;
		TArray<TSharedPtr<FJsonValue>> NodesArray;
		int32 NodeAddedCount = 0;

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node) continue;

			if (UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(Node))
			{
				TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
				CommentObj->SetStringField(TEXT("text"), CommentNode->NodeComment);
				CommentObj->SetNumberField(TEXT("x"), CommentNode->NodePosX);
				CommentObj->SetNumberField(TEXT("y"), CommentNode->NodePosY);
				CommentObj->SetNumberField(TEXT("width"), CommentNode->NodeWidth);
				CommentObj->SetNumberField(TEXT("height"), CommentNode->NodeHeight);
				CommentArray.Add(MakeShareable(new FJsonValueObject(CommentObj)));
			}

			if (NodeAddedCount < MaxNodesPerGraph)
			{
				TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
				NodeObj->SetStringField(TEXT("guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				NodeObj->SetStringField(TEXT("class"), Node->GetClass()->GetName());
				if (bIncludeNodeTitles)
				{
					NodeObj->SetStringField(TEXT("title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				}
				NodeObj->SetNumberField(TEXT("x"), Node->NodePosX);
				NodeObj->SetNumberField(TEXT("y"), Node->NodePosY);
				NodesArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));
				NodeAddedCount++;
			}
		}

		GraphObj->SetNumberField(TEXT("comment_count"), CommentArray.Num());
		GraphObj->SetArrayField(TEXT("comments"), CommentArray);
		GraphObj->SetArrayField(TEXT("nodes"), NodesArray);
		GraphsArray.Add(MakeShareable(new FJsonValueObject(GraphObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("blueprint"), Blueprint->GetPathName());
	ResultObj->SetNumberField(TEXT("graph_count"), GraphsArray.Num());
	ResultObj->SetArrayField(TEXT("graphs"), GraphsArray);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_SearchBlueprintNodes(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	FString Query;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) ||
		!Input->TryGetStringField(TEXT("query"), Query))
	{
		OutResult = TEXT("Missing required fields: blueprint_path, query");
		return false;
	}

	int32 MaxResults = 100;
	if (Input->HasField(TEXT("max_results")))
	{
		MaxResults = FMath::Clamp((int32)Input->GetNumberField(TEXT("max_results")), 1, 5000);
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	const FString QueryLower = Query.ToLower();
	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);

	TArray<TSharedPtr<FJsonValue>> ResultsArray;
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node) continue;

			const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
			const FString NodeClass = Node->GetClass()->GetName();
			const FString NodeComment = Node->NodeComment;

			bool bPinMatch = false;
			for (UEdGraphPin* Pin : Node->Pins)
			{
				if (!Pin) continue;
				if (Pin->PinName.ToString().ToLower().Contains(QueryLower))
				{
					bPinMatch = true;
					break;
				}
			}

			const bool bMatch =
				NodeTitle.ToLower().Contains(QueryLower) ||
				NodeClass.ToLower().Contains(QueryLower) ||
				NodeComment.ToLower().Contains(QueryLower) ||
				bPinMatch;

			if (!bMatch) continue;

			TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
			NodeObj->SetStringField(TEXT("graph_name"), Graph->GetName());
			NodeObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
			NodeObj->SetStringField(TEXT("node_title"), NodeTitle);
			NodeObj->SetStringField(TEXT("node_class"), NodeClass);
			NodeObj->SetStringField(TEXT("node_comment"), NodeComment);
			NodeObj->SetNumberField(TEXT("x"), Node->NodePosX);
			NodeObj->SetNumberField(TEXT("y"), Node->NodePosY);
			ResultsArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));

			if (ResultsArray.Num() >= MaxResults)
			{
				TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
				ResultObj->SetStringField(TEXT("query"), Query);
				ResultObj->SetNumberField(TEXT("count"), ResultsArray.Num());
				ResultObj->SetArrayField(TEXT("results"), ResultsArray);
				OutResult = JsonObjToString(ResultObj);
				return true;
			}
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("query"), Query);
	ResultObj->SetNumberField(TEXT("count"), ResultsArray.Num());
	ResultObj->SetArrayField(TEXT("results"), ResultsArray);
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_FindBlueprintAssetUsage(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), AssetPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_ASSET_PATH"), TEXT("Missing required field: asset_path"));
	}

	FString BlueprintPath;
	Input->TryGetStringField(TEXT("blueprint_path"), BlueprintPath);

	int32 MaxBlueprints = 5000;
	Input->TryGetNumberField(TEXT("max_blueprints"), MaxBlueprints);
	MaxBlueprints = FMath::Clamp(MaxBlueprints, 1, 5000);

	int32 MaxResultsPerBlueprint = 100;
	Input->TryGetNumberField(TEXT("max_results_per_blueprint"), MaxResultsPerBlueprint);
	MaxResultsPerBlueprint = FMath::Clamp(MaxResultsPerBlueprint, 1, 1000);

	const FString TargetObjectPath = NormalizeAssetPathForMatch(AssetPath);
	const FString TargetPackagePath = GetAssetPackagePathFromObjectPath(TargetObjectPath);
	const FString TargetAssetName = GetAssetNameFromObjectPath(TargetObjectPath);
	UObject* TargetAsset = UEditorAssetLibrary::LoadAsset(TargetObjectPath);

	TArray<UBlueprint*> BlueprintsToInspect;
	if (!BlueprintPath.IsEmpty())
	{
		UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BlueprintPath));
		if (!Blueprint)
		{
			return BuildBlueprintToolError(
				OutResult,
				TEXT("BP_NOT_FOUND"),
				FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BlueprintPath));
		}

		BlueprintsToInspect.Add(Blueprint);
	}
	else
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> CandidateBlueprintAssets;
		TArray<FName> ReferencerPackages;
		AssetRegistryModule.Get().GetReferencers(
			*TargetPackagePath,
			ReferencerPackages,
			UE::AssetRegistry::EDependencyCategory::Package);

		for (const FName& PackageName : ReferencerPackages)
		{
			TArray<FAssetData> PackageAssets;
			AssetRegistryModule.Get().GetAssetsByPackageName(PackageName, PackageAssets);
			for (const FAssetData& AssetData : PackageAssets)
			{
				const FName AssetClassName = AssetData.AssetClassPath.GetAssetName();
				if (!AssetClassName.ToString().Contains(TEXT("Blueprint")))
				{
					continue;
				}

				CandidateBlueprintAssets.AddUnique(AssetData);
			}
		}

		if (CandidateBlueprintAssets.Num() == 0)
		{
			AssetRegistryModule.Get().GetAllAssets(CandidateBlueprintAssets, true);
		}

		for (const FAssetData& AssetData : CandidateBlueprintAssets)
		{
			if (BlueprintsToInspect.Num() >= MaxBlueprints)
			{
				break;
			}

			const FName AssetClassName = AssetData.AssetClassPath.GetAssetName();
			if (!AssetClassName.ToString().Contains(TEXT("Blueprint")))
			{
				continue;
			}

			UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
			if (Blueprint)
			{
				BlueprintsToInspect.Add(Blueprint);
			}
		}
	}

	TArray<TSharedPtr<FJsonValue>> BlueprintMatchesArray;
	int32 InspectedBlueprintCount = 0;

	for (UBlueprint* Blueprint : BlueprintsToInspect)
	{
		if (!Blueprint)
		{
			continue;
		}

		++InspectedBlueprintCount;

		TArray<TSharedPtr<FJsonValue>> NodePinMatchesArray;
		TArray<TSharedPtr<FJsonValue>> ClassDefaultMatchesArray;
		TArray<TSharedPtr<FJsonValue>> ComponentTemplateMatchesArray;

		TArray<UEdGraph*> Graphs;
		GetBlueprintGraphs(Blueprint, Graphs);

		for (UEdGraph* Graph : Graphs)
		{
			if (!Graph)
			{
				continue;
			}

			for (UEdGraphNode* Node : Graph->Nodes)
			{
				if (!Node)
				{
					continue;
				}

				for (UEdGraphPin* Pin : Node->Pins)
				{
					if (!Pin)
					{
						continue;
					}

					const FString DefaultObjectPath = Pin->DefaultObject ? Pin->DefaultObject->GetPathName() : FString();
					const FString DefaultValue = Pin->DefaultValue;
					const FString DefaultText = Pin->DefaultTextValue.ToString();

					const bool bObjectMatch =
						(TargetAsset && Pin->DefaultObject == TargetAsset) ||
						StringReferencesAsset(DefaultObjectPath, TargetObjectPath, TargetPackagePath, TargetAssetName);
					const bool bValueMatch =
						StringReferencesAsset(DefaultValue, TargetObjectPath, TargetPackagePath, TargetAssetName) ||
						StringReferencesAsset(DefaultText, TargetObjectPath, TargetPackagePath, TargetAssetName);

					if (!bObjectMatch && !bValueMatch)
					{
						continue;
					}

					TSharedPtr<FJsonObject> MatchObj = BuildCompactBlueprintNodeObject(Node);
					MatchObj->SetStringField(TEXT("match_source"), TEXT("node_pin"));
					MatchObj->SetStringField(TEXT("graph_name"), Graph->GetName());
					MatchObj->SetStringField(TEXT("pin_name"), Pin->PinName.ToString());
					MatchObj->SetStringField(TEXT("pin_direction"),
						(Pin->Direction == EGPD_Input) ? TEXT("input") : TEXT("output"));
					MatchObj->SetStringField(TEXT("pin_default_value"), DefaultValue);
					MatchObj->SetStringField(TEXT("pin_default_text"), DefaultText);
					MatchObj->SetStringField(TEXT("pin_default_object"), DefaultObjectPath);
					MatchObj->SetStringField(TEXT("match_type"), bObjectMatch ? TEXT("pin_default_object") : TEXT("pin_default_text"));
					NodePinMatchesArray.Add(MakeShareable(new FJsonValueObject(MatchObj)));

					if (NodePinMatchesArray.Num() >= MaxResultsPerBlueprint)
					{
						break;
					}
				}

				if (NodePinMatchesArray.Num() >= MaxResultsPerBlueprint)
				{
					break;
				}
			}
		}

		if (Blueprint->GeneratedClass)
		{
			UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject();
			if (CDO)
			{
				for (TFieldIterator<FProperty> It(Blueprint->GeneratedClass, EFieldIterationFlags::IncludeSuper); It; ++It)
				{
					FProperty* Property = *It;
					if (!Property)
					{
						continue;
					}

					const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);
					GatherAssetPropertyMatchesRecursive(
						Property,
						ValuePtr,
						Property->GetName(),
						TEXT("class_default"),
						Blueprint->GetName(),
						TargetAsset,
						TargetObjectPath,
						TargetPackagePath,
						TargetAssetName,
						ClassDefaultMatchesArray);
				}
			}
		}

		if (Blueprint->SimpleConstructionScript)
		{
			for (USCS_Node* SCSNode : Blueprint->SimpleConstructionScript->GetAllNodes())
			{
				if (!SCSNode || !SCSNode->ComponentTemplate)
				{
					continue;
				}

				UObject* ComponentTemplate = SCSNode->ComponentTemplate;
				for (TFieldIterator<FProperty> It(ComponentTemplate->GetClass(), EFieldIterationFlags::IncludeSuper); It; ++It)
				{
					FProperty* Property = *It;
					if (!Property)
					{
						continue;
					}

					const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ComponentTemplate);
					GatherAssetPropertyMatchesRecursive(
						Property,
						ValuePtr,
						Property->GetName(),
						TEXT("component_template"),
						SCSNode->GetVariableName().ToString(),
						TargetAsset,
						TargetObjectPath,
						TargetPackagePath,
						TargetAssetName,
						ComponentTemplateMatchesArray);
				}
			}
		}

		const int32 TotalMatches =
			NodePinMatchesArray.Num() +
			ClassDefaultMatchesArray.Num() +
			ComponentTemplateMatchesArray.Num();

		if (TotalMatches == 0)
		{
			continue;
		}

		TSharedPtr<FJsonObject> BlueprintMatchObj = MakeShareable(new FJsonObject());
		BlueprintMatchObj->SetStringField(TEXT("blueprint_path"), Blueprint->GetPathName());
		BlueprintMatchObj->SetNumberField(TEXT("node_pin_match_count"), NodePinMatchesArray.Num());
		BlueprintMatchObj->SetNumberField(TEXT("class_default_match_count"), ClassDefaultMatchesArray.Num());
		BlueprintMatchObj->SetNumberField(TEXT("component_template_match_count"), ComponentTemplateMatchesArray.Num());
		BlueprintMatchObj->SetNumberField(TEXT("total_match_count"), TotalMatches);
		BlueprintMatchObj->SetArrayField(TEXT("node_pin_matches"), NodePinMatchesArray);
		BlueprintMatchObj->SetArrayField(TEXT("class_default_matches"), ClassDefaultMatchesArray);
		BlueprintMatchObj->SetArrayField(TEXT("component_template_matches"), ComponentTemplateMatchesArray);
		BlueprintMatchesArray.Add(MakeShareable(new FJsonValueObject(BlueprintMatchObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("asset_path"), TargetObjectPath);
	ResultObj->SetStringField(TEXT("asset_package_path"), TargetPackagePath);
	ResultObj->SetStringField(TEXT("asset_name"), TargetAssetName);
	ResultObj->SetBoolField(TEXT("asset_loaded"), TargetAsset != nullptr);
	ResultObj->SetNumberField(TEXT("inspected_blueprint_count"), InspectedBlueprintCount);
	ResultObj->SetNumberField(TEXT("matched_blueprint_count"), BlueprintMatchesArray.Num());
	ResultObj->SetArrayField(TEXT("blueprint_matches"), BlueprintMatchesArray);
	ResultObj->SetStringField(TEXT("message"),
		BlueprintMatchesArray.Num() > 0
			? TEXT("Blueprint asset usage matches found.")
			: TEXT("No Blueprint asset usage matches found."));

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_TraceBlueprintExecChain(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	FString NodeGuidString;
	FString EventName;
	FString FunctionName;
	FString NodeTitleQuery;
	Input->TryGetStringField(TEXT("node_guid"), NodeGuidString);
	Input->TryGetStringField(TEXT("event_name"), EventName);
	Input->TryGetStringField(TEXT("function_name"), FunctionName);
	Input->TryGetStringField(TEXT("node_title_query"), NodeTitleQuery);

	if (NodeGuidString.IsEmpty() && EventName.IsEmpty() && FunctionName.IsEmpty() && NodeTitleQuery.IsEmpty())
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_MISSING_TRACE_SEED"),
			TEXT("Provide at least one of: node_guid, event_name, function_name, node_title_query."));
	}

	int32 MaxDepth = 8;
	Input->TryGetNumberField(TEXT("max_depth"), MaxDepth);
	MaxDepth = FMath::Clamp(MaxDepth, 1, 32);

	int32 MaxPaths = 50;
	Input->TryGetNumberField(TEXT("max_paths"), MaxPaths);
	MaxPaths = FMath::Clamp(MaxPaths, 1, 500);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);

	TArray<TPair<UEdGraphNode*, FString>> Seeds;
	auto AddSeedUnique = [&Seeds](UEdGraphNode* Node, const FString& Reason)
	{
		for (const TPair<UEdGraphNode*, FString>& ExistingSeed : Seeds)
		{
			if (ExistingSeed.Key == Node && ExistingSeed.Value.Equals(Reason, ESearchCase::CaseSensitive))
			{
				return;
			}
		}

		Seeds.Add(TPair<UEdGraphNode*, FString>(Node, Reason));
	};
	const FString EventNameLower = EventName.ToLower();
	const FString FunctionNameLower = FunctionName.ToLower();
	const FString TitleQueryLower = NodeTitleQuery.ToLower();

	if (!NodeGuidString.IsEmpty())
	{
		FGuid NodeGuid;
		if (FGuid::Parse(NodeGuidString, NodeGuid))
		{
			if (UEdGraphNode* ExactNode = FindBlueprintNodeByGuid(Blueprint, NodeGuid))
			{
				AddSeedUnique(ExactNode, TEXT("node_guid"));
			}
		}
	}

	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph)
		{
			continue;
		}

		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node)
			{
				continue;
			}

			const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
			const FString NodeTitleLower = NodeTitle.ToLower();
			const FString NodeClassLower = Node->GetClass()->GetName().ToLower();
			const FString NodeCommentLower = Node->NodeComment.ToLower();

			if (!EventNameLower.IsEmpty())
			{
				if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(Node))
				{
					const FString MemberName = EventNode->EventReference.GetMemberName().ToString();
					if (MemberName.Equals(EventName, ESearchCase::IgnoreCase) ||
						NodeTitleLower.Contains(EventNameLower))
					{
						AddSeedUnique(Node, TEXT("event_name"));
					}
				}
			}

			if (!FunctionNameLower.IsEmpty())
			{
				if (const UK2Node_CallFunction* CallFunctionNode = Cast<UK2Node_CallFunction>(Node))
				{
					const FString MemberName = CallFunctionNode->FunctionReference.GetMemberName().ToString();
					if (MemberName.Equals(FunctionName, ESearchCase::IgnoreCase) ||
						NodeTitleLower.Contains(FunctionNameLower))
					{
						AddSeedUnique(Node, TEXT("function_name"));
					}
				}
			}

			if (!TitleQueryLower.IsEmpty() &&
				(NodeTitleLower.Contains(TitleQueryLower) ||
				 NodeClassLower.Contains(TitleQueryLower) ||
				 NodeCommentLower.Contains(TitleQueryLower)))
			{
				AddSeedUnique(Node, TEXT("node_title_query"));
			}
		}
	}

	if (Seeds.Num() == 0)
	{
		TSharedPtr<FJsonObject> DebugObj = MakeShareable(new FJsonObject());
		DebugObj->SetStringField(TEXT("blueprint_path"), BpPath);
		DebugObj->SetStringField(TEXT("event_name"), EventName);
		DebugObj->SetStringField(TEXT("function_name"), FunctionName);
		DebugObj->SetStringField(TEXT("node_title_query"), NodeTitleQuery);
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_TRACE_SEED_NOT_FOUND"),
			TEXT("No Blueprint nodes matched the provided seed query."),
			DebugObj);
	}

	TArray<TSharedPtr<FJsonValue>> SeedArray;
	TArray<TSharedPtr<FJsonValue>> PathArray;
	int32 PathCount = 0;

	for (const TPair<UEdGraphNode*, FString>& SeedPair : Seeds)
	{
		if (!SeedPair.Key)
		{
			continue;
		}

		TSharedPtr<FJsonObject> SeedObj = BuildCompactBlueprintNodeObject(SeedPair.Key);
		SeedObj->SetStringField(TEXT("seed_reason"), SeedPair.Value);
		SeedArray.Add(MakeShareable(new FJsonValueObject(SeedObj)));
	}

	TFunction<void(UEdGraphNode*, const FString&, TArray<UEdGraphNode*>&, TSet<FGuid>&, TArray<TSharedPtr<FJsonValue>>&)> TraceExecPath;
	TraceExecPath =
		[&](UEdGraphNode* CurrentNode, const FString& SeedReason, TArray<UEdGraphNode*>& PathNodes, TSet<FGuid>& VisitedNodes, TArray<TSharedPtr<FJsonValue>>& EdgeArray)
	{
		if (!CurrentNode || PathCount >= MaxPaths)
		{
			return;
		}

		const int32 CurrentDepth = PathNodes.Num() - 1;
		TArray<TTuple<UEdGraphNode*, FString, FString>> OutLinks;
		CollectExecOutgoingNodes(CurrentNode, OutLinks);

		auto EmitPath = [&](const FString& StopReason)
		{
			TSharedPtr<FJsonObject> PathObj = MakeShareable(new FJsonObject());
			PathObj->SetStringField(TEXT("seed_reason"), SeedReason);
			PathObj->SetStringField(TEXT("stop_reason"), StopReason);
			PathObj->SetNumberField(TEXT("depth"), CurrentDepth);

			TArray<TSharedPtr<FJsonValue>> PathNodesArray;
			TArray<FString> PathTitles;
			for (UEdGraphNode* PathNode : PathNodes)
			{
				PathNodesArray.Add(MakeShareable(new FJsonValueObject(BuildCompactBlueprintNodeObject(PathNode))));
				PathTitles.Add(PathNode ? PathNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString() : TEXT("<null>"));
			}

			PathObj->SetArrayField(TEXT("nodes"), PathNodesArray);
			PathObj->SetArrayField(TEXT("edges"), EdgeArray);
			PathObj->SetArrayField(TEXT("path_titles"), MakeBlueprintStringJsonArray(PathTitles));
			PathObj->SetStringField(TEXT("path_summary"), FString::Join(PathTitles, TEXT(" -> ")));
			PathArray.Add(MakeShareable(new FJsonValueObject(PathObj)));
			++PathCount;
		};

		if (OutLinks.Num() == 0)
		{
			EmitPath(TEXT("leaf"));
			return;
		}

		if (CurrentDepth >= MaxDepth)
		{
			EmitPath(TEXT("max_depth"));
			return;
		}

		for (const TTuple<UEdGraphNode*, FString, FString>& Link : OutLinks)
		{
			if (PathCount >= MaxPaths)
			{
				return;
			}

			UEdGraphNode* NextNode = Link.Get<0>();
			if (!NextNode)
			{
				continue;
			}

			TArray<TSharedPtr<FJsonValue>> NextEdgeArray = EdgeArray;
			TSharedPtr<FJsonObject> EdgeObj = MakeShareable(new FJsonObject());
			EdgeObj->SetStringField(TEXT("from_node_guid"), CurrentNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
			EdgeObj->SetStringField(TEXT("to_node_guid"), NextNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
			EdgeObj->SetStringField(TEXT("source_exec_pin"), Link.Get<1>());
			EdgeObj->SetStringField(TEXT("target_exec_pin"), Link.Get<2>());
			NextEdgeArray.Add(MakeShareable(new FJsonValueObject(EdgeObj)));

			if (VisitedNodes.Contains(NextNode->NodeGuid))
			{
				PathNodes.Add(NextNode);
				EmitPath(TEXT("cycle_detected"));
				PathNodes.Pop();
				continue;
			}

			VisitedNodes.Add(NextNode->NodeGuid);
			PathNodes.Add(NextNode);
			TraceExecPath(NextNode, SeedReason, PathNodes, VisitedNodes, NextEdgeArray);
			PathNodes.Pop();
			VisitedNodes.Remove(NextNode->NodeGuid);
		}
	};

	for (const TPair<UEdGraphNode*, FString>& SeedPair : Seeds)
	{
		if (PathCount >= MaxPaths || !SeedPair.Key)
		{
			break;
		}

		TArray<UEdGraphNode*> PathNodes;
		PathNodes.Add(SeedPair.Key);
		TSet<FGuid> VisitedNodes;
		VisitedNodes.Add(SeedPair.Key->NodeGuid);
		TArray<TSharedPtr<FJsonValue>> EdgeArray;
		TraceExecPath(SeedPair.Key, SeedPair.Value, PathNodes, VisitedNodes, EdgeArray);
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("blueprint_path"), BpPath);
	ResultObj->SetNumberField(TEXT("seed_count"), SeedArray.Num());
	ResultObj->SetArrayField(TEXT("seeds"), SeedArray);
	ResultObj->SetNumberField(TEXT("path_count"), PathArray.Num());
	ResultObj->SetArrayField(TEXT("paths"), PathArray);
	ResultObj->SetStringField(TEXT("message"),
		PathArray.Num() > 0
			? TEXT("Blueprint exec chains traced.")
			: TEXT("No exec paths were traced from the resolved seed nodes."));

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_SummarizeBlueprintValueFlow(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	FString VariableName;
	FString ParameterName;
	FString NodeTitleQuery;
	Input->TryGetStringField(TEXT("variable_name"), VariableName);
	Input->TryGetStringField(TEXT("parameter_name"), ParameterName);
	Input->TryGetStringField(TEXT("node_title_query"), NodeTitleQuery);

	int32 MaxDepth = 3;
	if (Input->HasTypedField<EJson::Number>(TEXT("max_depth")))
	{
		MaxDepth = static_cast<int32>(Input->GetNumberField(TEXT("max_depth")));
	}
	else
	{
		FString MaxDepthStr;
		if (Input->TryGetStringField(TEXT("max_depth"), MaxDepthStr) && !MaxDepthStr.IsEmpty())
		{
			MaxDepth = FCString::Atoi(*MaxDepthStr);
		}
	}
	MaxDepth = FMath::Clamp(MaxDepth, 1, 8);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);

	TArray<UEdGraphNode*> AllNodes;
	AllNodes.Reserve(2048);
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;
		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (Node)
			{
				AllNodes.Add(Node);
			}
		}
	}

	const FString VariableNameLower = VariableName.ToLower();
	const FString ParameterNameLower = ParameterName.ToLower();
	const FString QueryLower = NodeTitleQuery.ToLower();

	TMap<UEdGraphNode*, TArray<FString>> SeedReasons;
	auto AddReasonUnique = [](TArray<FString>& Reasons, const FString& Reason)
	{
		if (!Reasons.Contains(Reason))
		{
			Reasons.Add(Reason);
		}
	};

	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node) continue;

		const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		const FString NodeClass = Node->GetClass()->GetName();
		const FString NodeComment = Node->NodeComment;

		const FString NodeTitleLower = NodeTitle.ToLower();
		const FString NodeClassLower = NodeClass.ToLower();
		const FString NodeCommentLower = NodeComment.ToLower();

		TArray<FString> Reasons;
		bool bPinNameMatch = false;
		bool bPinValueMatch = false;
		bool bParameterNameExactPinMatch = false;

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin) continue;
			const FString PinName = Pin->PinName.ToString();
			const FString PinNameLower = PinName.ToLower();
			const FString DefaultValueLower = Pin->DefaultValue.ToLower();

			if (!VariableNameLower.IsEmpty())
			{
				if (PinNameLower.Contains(VariableNameLower))
				{
					bPinNameMatch = true;
				}
				if (DefaultValueLower.Contains(VariableNameLower))
				{
					bPinValueMatch = true;
				}
			}

			if (!ParameterNameLower.IsEmpty())
			{
				if (PinNameLower.Contains(ParameterNameLower))
				{
					bPinNameMatch = true;
				}
				if (DefaultValueLower.Contains(ParameterNameLower))
				{
					bPinValueMatch = true;
				}
				if (PinName.Equals(TEXT("ParameterName"), ESearchCase::IgnoreCase) &&
					Pin->DefaultValue.Equals(ParameterName, ESearchCase::IgnoreCase))
				{
					bParameterNameExactPinMatch = true;
				}
			}

			if (!QueryLower.IsEmpty())
			{
				if (PinNameLower.Contains(QueryLower) || DefaultValueLower.Contains(QueryLower))
				{
					bPinNameMatch = true;
				}
			}
		}

		if (!QueryLower.IsEmpty())
		{
			if (NodeTitleLower.Contains(QueryLower) ||
				NodeClassLower.Contains(QueryLower) ||
				NodeCommentLower.Contains(QueryLower) ||
				bPinNameMatch)
			{
				AddReasonUnique(Reasons, TEXT("query_match"));
			}
		}

		if (!VariableNameLower.IsEmpty())
		{
			if (NodeTitleLower.Contains(VariableNameLower) ||
				NodeCommentLower.Contains(VariableNameLower) ||
				bPinNameMatch ||
				bPinValueMatch)
			{
				AddReasonUnique(Reasons, TEXT("variable_match"));
			}
		}

		if (!ParameterNameLower.IsEmpty())
		{
			if (bParameterNameExactPinMatch)
			{
				AddReasonUnique(Reasons, TEXT("parameter_exact_match"));
			}
			else if (NodeTitleLower.Contains(ParameterNameLower) ||
				bPinNameMatch ||
				bPinValueMatch)
			{
				AddReasonUnique(Reasons, TEXT("parameter_match"));
			}
		}

		if (Reasons.Num() > 0)
		{
			SeedReasons.Add(Node, Reasons);
		}
	}

	TMap<UEdGraphNode*, TSet<UEdGraphNode*>> Adjacency;
	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node) continue;
		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin) continue;
			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin) continue;
				UEdGraphNode* OtherNode = LinkedPin->GetOwningNode();
				if (!OtherNode) continue;
				Adjacency.FindOrAdd(Node).Add(OtherNode);
				Adjacency.FindOrAdd(OtherNode).Add(Node);
			}
		}
	}

	TSet<UEdGraphNode*> FocusNodes;
	TArray<UEdGraphNode*> Frontier;
	for (const TPair<UEdGraphNode*, TArray<FString>>& Pair : SeedReasons)
	{
		FocusNodes.Add(Pair.Key);
		Frontier.Add(Pair.Key);
	}

	for (int32 Depth = 0; Depth < MaxDepth && Frontier.Num() > 0; ++Depth)
	{
		TArray<UEdGraphNode*> Next;
		for (UEdGraphNode* Current : Frontier)
		{
			if (const TSet<UEdGraphNode*>* Neighbors = Adjacency.Find(Current))
			{
				for (UEdGraphNode* Neighbor : *Neighbors)
				{
					if (Neighbor && !FocusNodes.Contains(Neighbor))
					{
						FocusNodes.Add(Neighbor);
						Next.Add(Neighbor);
					}
				}
			}
		}
		Frontier = MoveTemp(Next);
	}

	TArray<TSharedPtr<FJsonValue>> SeedsArray;
	for (const TPair<UEdGraphNode*, TArray<FString>>& Pair : SeedReasons)
	{
		UEdGraphNode* Node = Pair.Key;
		TSharedPtr<FJsonObject> SeedObj = MakeShareable(new FJsonObject());
		SeedObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
		SeedObj->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		SeedObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
		SeedObj->SetStringField(TEXT("graph_name"), Node->GetGraph() ? Node->GetGraph()->GetName() : TEXT(""));
		TArray<TSharedPtr<FJsonValue>> ReasonValues;
		for (const FString& Reason : Pair.Value)
		{
			ReasonValues.Add(MakeShareable(new FJsonValueString(Reason)));
		}
		SeedObj->SetArrayField(TEXT("reasons"), ReasonValues);
		SeedsArray.Add(MakeShareable(new FJsonValueObject(SeedObj)));
	}

	TArray<TSharedPtr<FJsonValue>> NodesArray;
	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node || !FocusNodes.Contains(Node)) continue;

		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
		NodeObj->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		NodeObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
		NodeObj->SetStringField(TEXT("graph_name"), Node->GetGraph() ? Node->GetGraph()->GetName() : TEXT(""));
		NodeObj->SetNumberField(TEXT("x"), Node->NodePosX);
		NodeObj->SetNumberField(TEXT("y"), Node->NodePosY);
		NodeObj->SetBoolField(TEXT("is_seed"), SeedReasons.Contains(Node));
		NodeObj->SetNumberField(TEXT("pin_count"), Node->Pins.Num());
		NodesArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));
	}

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	TSet<FString> SeenEdges;
	TArray<TSharedPtr<FJsonValue>> EdgesArray;
	for (UEdGraphNode* Node : FocusNodes)
	{
		if (!Node) continue;
		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin || Pin->Direction != EGPD_Output) continue;
			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin || !LinkedPin->GetOwningNode()) continue;
				UEdGraphNode* TargetNode = LinkedPin->GetOwningNode();
				if (!FocusNodes.Contains(TargetNode)) continue;

				const FString Key = FString::Printf(
					TEXT("%s|%s|%s|%s"),
					*Node->NodeGuid.ToString(EGuidFormats::Digits),
					*Pin->PinName.ToString(),
					*TargetNode->NodeGuid.ToString(EGuidFormats::Digits),
					*LinkedPin->PinName.ToString());
				if (SeenEdges.Contains(Key))
				{
					continue;
				}
				SeenEdges.Add(Key);

				TSharedPtr<FJsonObject> EdgeObj = MakeShareable(new FJsonObject());
				EdgeObj->SetStringField(TEXT("source_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				EdgeObj->SetStringField(TEXT("source_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				EdgeObj->SetStringField(TEXT("source_pin"), Pin->PinName.ToString());
				EdgeObj->SetStringField(TEXT("target_guid"), TargetNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				EdgeObj->SetStringField(TEXT("target_title"), TargetNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				EdgeObj->SetStringField(TEXT("target_pin"), LinkedPin->PinName.ToString());
				const bool bExec = (Pin->PinType.PinCategory == K2Schema->PC_Exec);
				EdgeObj->SetStringField(TEXT("link_type"), bExec ? TEXT("exec") : TEXT("data"));
				EdgesArray.Add(MakeShareable(new FJsonValueObject(EdgeObj)));
			}
		}
	}

	TArray<FString> SummaryLines;
	TArray<UEdGraphNode*> SetParamNodes;
	TArray<UEdGraphNode*> TimelineNodes;
	TArray<UEdGraphNode*> CreateDynamicMaterialNodes;

	for (UEdGraphNode* Node : FocusNodes)
	{
		if (!Node) continue;
		const FString Title = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		const FString ClassName = Node->GetClass()->GetName();
		if (Title.Contains(TEXT("Set Scalar Parameter Value")) ||
			Title.Contains(TEXT("Set Vector Parameter Value")) ||
			Title.Contains(TEXT("Set Texture Parameter Value")))
		{
			SetParamNodes.Add(Node);
		}
		if (ClassName.Contains(TEXT("K2Node_Timeline")))
		{
			TimelineNodes.Add(Node);
		}
		if (Title.Contains(TEXT("Create Dynamic Material Instance")))
		{
			CreateDynamicMaterialNodes.Add(Node);
		}
	}

	for (UEdGraphNode* Node : SetParamNodes)
	{
		FString EffectiveParamName;
		FString ValueSourceSummary;
		FString TargetSummary;
		FString ExecSourceSummary;

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin) continue;
			if (Pin->PinName == TEXT("ParameterName"))
			{
				EffectiveParamName = Pin->DefaultValue;
			}
			else if (Pin->PinName == TEXT("Value"))
			{
				TArray<FString> Sources;
				for (UEdGraphPin* Linked : Pin->LinkedTo)
				{
					if (Linked && Linked->GetOwningNode())
					{
						Sources.Add(Linked->GetOwningNode()->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
					}
				}
				ValueSourceSummary = Sources.Num() > 0 ? FString::Join(Sources, TEXT(", ")) : TEXT("const/default");
			}
			else if (Pin->PinName == TEXT("self"))
			{
				TArray<FString> Sources;
				for (UEdGraphPin* Linked : Pin->LinkedTo)
				{
					if (Linked && Linked->GetOwningNode())
					{
						Sources.Add(Linked->GetOwningNode()->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
					}
				}
				TargetSummary = Sources.Num() > 0 ? FString::Join(Sources, TEXT(", ")) : TEXT("self/default");
			}
			else if (Pin->PinName == TEXT("execute"))
			{
				TArray<FString> Sources;
				for (UEdGraphPin* Linked : Pin->LinkedTo)
				{
					if (Linked && Linked->GetOwningNode())
					{
						Sources.Add(Linked->GetOwningNode()->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
					}
				}
				ExecSourceSummary = Sources.Num() > 0 ? FString::Join(Sources, TEXT(", ")) : TEXT("none");
			}
		}

		if (!ParameterName.IsEmpty() && !EffectiveParamName.Equals(ParameterName, ESearchCase::IgnoreCase))
		{
			continue;
		}

		SummaryLines.Add(FString::Printf(
			TEXT("Set param node '%s' writes ParameterName='%s', Value from [%s], target [%s], execute from [%s]."),
			*Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString(),
			EffectiveParamName.IsEmpty() ? TEXT("<empty>") : *EffectiveParamName,
			*ValueSourceSummary,
			*TargetSummary,
			*ExecSourceSummary));
	}

	for (UEdGraphNode* Node : TimelineNodes)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("Timeline '%s' is in flow neighborhood and may drive update pins each tick."),
			*Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString()));
	}

	for (UEdGraphNode* Node : CreateDynamicMaterialNodes)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("Dynamic material creation node found: '%s'."),
			*Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString()));
	}

	if (SummaryLines.Num() == 0)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("Found %d seed node(s), %d focus node(s), and %d link(s)."),
			SeedReasons.Num(),
			FocusNodes.Num(),
			EdgesArray.Num()));
	}

	TSharedPtr<FJsonObject> QueryObj = MakeShareable(new FJsonObject());
	QueryObj->SetStringField(TEXT("variable_name"), VariableName);
	QueryObj->SetStringField(TEXT("parameter_name"), ParameterName);
	QueryObj->SetStringField(TEXT("node_title_query"), NodeTitleQuery);
	QueryObj->SetNumberField(TEXT("max_depth"), MaxDepth);

	TArray<TSharedPtr<FJsonValue>> SummaryLinesArray;
	for (const FString& Line : SummaryLines)
	{
		SummaryLinesArray.Add(MakeShareable(new FJsonValueString(Line)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("blueprint_path"), BpPath);
	ResultObj->SetObjectField(TEXT("query"), QueryObj);
	ResultObj->SetBoolField(TEXT("has_matches"), SeedReasons.Num() > 0);
	ResultObj->SetNumberField(TEXT("seed_count"), SeedReasons.Num());
	ResultObj->SetNumberField(TEXT("node_count"), FocusNodes.Num());
	ResultObj->SetNumberField(TEXT("edge_count"), EdgesArray.Num());
	ResultObj->SetArrayField(TEXT("seeds"), SeedsArray);
	ResultObj->SetArrayField(TEXT("nodes"), NodesArray);
	ResultObj->SetArrayField(TEXT("edges"), EdgesArray);
	ResultObj->SetArrayField(TEXT("summary_lines"), SummaryLinesArray);
	ResultObj->SetStringField(TEXT("human_summary"), FString::Join(SummaryLines, TEXT("\n")));
	if (SeedReasons.Num() == 0)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("No seed nodes matched provided filters."));
	}

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_SummarizeBlueprintMaterialRuntime(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	FString ParameterName;
	FString MaterialAssetPath;
	FString MaterialInstancePath;
	FString NodeTitleQuery;
	Input->TryGetStringField(TEXT("parameter_name"), ParameterName);
	Input->TryGetStringField(TEXT("material_asset_path"), MaterialAssetPath);
	Input->TryGetStringField(TEXT("material_instance_path"), MaterialInstancePath);
	Input->TryGetStringField(TEXT("node_title_query"), NodeTitleQuery);

	int32 MaxDepth = 3;
	if (Input->HasTypedField<EJson::Number>(TEXT("max_depth")))
	{
		MaxDepth = static_cast<int32>(Input->GetNumberField(TEXT("max_depth")));
	}
	else
	{
		FString MaxDepthStr;
		if (Input->TryGetStringField(TEXT("max_depth"), MaxDepthStr) && !MaxDepthStr.IsEmpty())
		{
			MaxDepth = FCString::Atoi(*MaxDepthStr);
		}
	}
	MaxDepth = FMath::Clamp(MaxDepth, 1, 8);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);

	TArray<UEdGraphNode*> AllNodes;
	AllNodes.Reserve(2048);
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;
		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (Node)
			{
				AllNodes.Add(Node);
			}
		}
	}

	const FString ParameterNameLower = ParameterName.ToLower();
	const FString MaterialAssetPathLower = MaterialAssetPath.ToLower();
	const FString MaterialAssetNameLower = FPackageName::ObjectPathToObjectName(MaterialAssetPath).ToLower();
	const FString QueryLower = NodeTitleQuery.ToLower();
	const bool bHasAnyFilter = !ParameterNameLower.IsEmpty() || !MaterialAssetPathLower.IsEmpty() || !QueryLower.IsEmpty();

	auto AddReasonUnique = [](TArray<FString>& Reasons, const FString& Reason)
	{
		if (!Reasons.Contains(Reason))
		{
			Reasons.Add(Reason);
		}
	};

	auto MakeStringJsonArray = [](const TArray<FString>& Strings) -> TArray<TSharedPtr<FJsonValue>>
	{
		TArray<TSharedPtr<FJsonValue>> Values;
		for (const FString& Value : Strings)
		{
			Values.Add(MakeShareable(new FJsonValueString(Value)));
		}
		return Values;
	};

	auto CollectLinkedNodeTitles = [](UEdGraphPin* Pin) -> TArray<FString>
	{
		TArray<FString> Titles;
		if (!Pin)
		{
			return Titles;
		}

		for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			if (!LinkedPin || !LinkedPin->GetOwningNode())
			{
				continue;
			}

			Titles.AddUnique(LinkedPin->GetOwningNode()->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		}

		return Titles;
	};

	auto CollectMaterialRefsFromNode = [](UEdGraphNode* Node) -> TArray<FString>
	{
		TArray<FString> MaterialRefs;
		if (!Node)
		{
			return MaterialRefs;
		}

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin)
			{
				continue;
			}

			if (UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(Pin->DefaultObject))
			{
				MaterialRefs.AddUnique(MaterialInterface->GetPathName());
			}
		}

		return MaterialRefs;
	};

	auto BuildBasicNodeObject = [&](UEdGraphNode* Node) -> TSharedPtr<FJsonObject>
	{
		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetStringField(TEXT("node_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
		NodeObj->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		NodeObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
		NodeObj->SetStringField(TEXT("graph_name"), Node->GetGraph() ? Node->GetGraph()->GetName() : TEXT(""));
		NodeObj->SetNumberField(TEXT("x"), Node->NodePosX);
		NodeObj->SetNumberField(TEXT("y"), Node->NodePosY);
		return NodeObj;
	};

	auto IsSetMaterialParameterNode = [](const FString& NodeTitle) -> bool
	{
		return NodeTitle.Contains(TEXT("Set Scalar Parameter Value")) ||
			NodeTitle.Contains(TEXT("Set Vector Parameter Value")) ||
			NodeTitle.Contains(TEXT("Set Texture Parameter Value"));
	};

	auto GetMaterialWriteType = [](const FString& NodeTitle) -> FString
	{
		if (NodeTitle.Contains(TEXT("Set Scalar Parameter Value")))
		{
			return TEXT("scalar");
		}
		if (NodeTitle.Contains(TEXT("Set Vector Parameter Value")))
		{
			return TEXT("vector");
		}
		if (NodeTitle.Contains(TEXT("Set Texture Parameter Value")))
		{
			return TEXT("texture");
		}
		return TEXT("unknown");
	};

	auto IsCreateDynamicMaterialNode = [](const FString& NodeTitle) -> bool
	{
		return NodeTitle.Contains(TEXT("Create Dynamic Material Instance"));
	};

	auto NodeMatchesQuery = [&](UEdGraphNode* Node) -> bool
	{
		if (!Node || QueryLower.IsEmpty())
		{
			return false;
		}

		const FString NodeTitleLower = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString().ToLower();
		const FString NodeClassLower = Node->GetClass()->GetName().ToLower();
		const FString NodeCommentLower = Node->NodeComment.ToLower();
		if (NodeTitleLower.Contains(QueryLower) || NodeClassLower.Contains(QueryLower) || NodeCommentLower.Contains(QueryLower))
		{
			return true;
		}

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin)
			{
				continue;
			}

			if (Pin->PinName.ToString().ToLower().Contains(QueryLower) || Pin->DefaultValue.ToLower().Contains(QueryLower))
			{
				return true;
			}
		}

		return false;
	};

	auto NodeMatchesMaterialAsset = [&](UEdGraphNode* Node, const TArray<FString>& MaterialRefs) -> bool
	{
		if (!Node || MaterialAssetPathLower.IsEmpty())
		{
			return false;
		}

		for (const FString& Ref : MaterialRefs)
		{
			const FString RefLower = Ref.ToLower();
			if (RefLower.Contains(MaterialAssetPathLower) ||
				(!MaterialAssetNameLower.IsEmpty() && RefLower.Contains(MaterialAssetNameLower)))
			{
				return true;
			}
		}

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin)
			{
				continue;
			}

			const FString DefaultLower = Pin->DefaultValue.ToLower();
			if (DefaultLower.Contains(MaterialAssetPathLower) ||
				(!MaterialAssetNameLower.IsEmpty() && DefaultLower.Contains(MaterialAssetNameLower)))
			{
				return true;
			}
		}

		return false;
	};

	TMap<UEdGraphNode*, TSet<UEdGraphNode*>> Adjacency;
	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node) continue;
		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin) continue;
			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin) continue;
				UEdGraphNode* OtherNode = LinkedPin->GetOwningNode();
				if (!OtherNode) continue;
				Adjacency.FindOrAdd(Node).Add(OtherNode);
				Adjacency.FindOrAdd(OtherNode).Add(Node);
			}
		}
	}

	TMap<UEdGraphNode*, TArray<FString>> SeedReasons;
	TArray<UEdGraphNode*> AllSetParamNodes;
	TArray<UEdGraphNode*> AllDynamicMaterialNodes;

	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node) continue;

		const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		const bool bIsSetParamNode = IsSetMaterialParameterNode(NodeTitle);
		const bool bIsDynamicMaterialNode = IsCreateDynamicMaterialNode(NodeTitle);
		if (!bIsSetParamNode && !bIsDynamicMaterialNode)
		{
			continue;
		}

		if (bIsSetParamNode)
		{
			AllSetParamNodes.Add(Node);
		}
		if (bIsDynamicMaterialNode)
		{
			AllDynamicMaterialNodes.Add(Node);
		}

		TArray<FString> Reasons;
		const TArray<FString> MaterialRefs = CollectMaterialRefsFromNode(Node);

		if (!bHasAnyFilter)
		{
			AddReasonUnique(Reasons, bIsSetParamNode ? TEXT("material_param_write") : TEXT("dynamic_material_node"));
		}

		if (NodeMatchesQuery(Node))
		{
			AddReasonUnique(Reasons, TEXT("query_match"));
		}

		if (NodeMatchesMaterialAsset(Node, MaterialRefs))
		{
			AddReasonUnique(Reasons, TEXT("material_asset_match"));
		}

		if (bIsSetParamNode && !ParameterNameLower.IsEmpty())
		{
			bool bParameterMatched = false;
			for (UEdGraphPin* Pin : Node->Pins)
			{
				if (!Pin)
				{
					continue;
				}

				if (Pin->PinName == TEXT("ParameterName"))
				{
					if (!Pin->DefaultValue.IsEmpty() &&
						Pin->DefaultValue.Equals(ParameterName, ESearchCase::IgnoreCase))
					{
						bParameterMatched = true;
						break;
					}

					if (Pin->DefaultValue.ToLower().Contains(ParameterNameLower))
					{
						bParameterMatched = true;
					}

					const TArray<FString> LinkedTitles = CollectLinkedNodeTitles(Pin);
					for (const FString& LinkedTitle : LinkedTitles)
					{
						if (LinkedTitle.ToLower().Contains(ParameterNameLower))
						{
							bParameterMatched = true;
							break;
						}
					}
				}
			}

			if (bParameterMatched)
			{
				AddReasonUnique(Reasons, TEXT("parameter_match"));
			}
		}

		if (Reasons.Num() > 0)
		{
			SeedReasons.Add(Node, Reasons);
		}
	}

	if (SeedReasons.Num() == 0 && !bHasAnyFilter)
	{
		for (UEdGraphNode* Node : AllSetParamNodes)
		{
			SeedReasons.Add(Node, TArray<FString>{ TEXT("material_param_write") });
		}
		for (UEdGraphNode* Node : AllDynamicMaterialNodes)
		{
			if (!SeedReasons.Contains(Node))
			{
				SeedReasons.Add(Node, TArray<FString>{ TEXT("dynamic_material_node") });
			}
		}
	}

	TSet<UEdGraphNode*> FocusNodes;
	TArray<UEdGraphNode*> Frontier;
	for (const TPair<UEdGraphNode*, TArray<FString>>& Pair : SeedReasons)
	{
		FocusNodes.Add(Pair.Key);
		Frontier.Add(Pair.Key);
	}

	for (int32 Depth = 0; Depth < MaxDepth && Frontier.Num() > 0; ++Depth)
	{
		TArray<UEdGraphNode*> Next;
		for (UEdGraphNode* Current : Frontier)
		{
			if (const TSet<UEdGraphNode*>* Neighbors = Adjacency.Find(Current))
			{
				for (UEdGraphNode* Neighbor : *Neighbors)
				{
					if (Neighbor && !FocusNodes.Contains(Neighbor))
					{
						FocusNodes.Add(Neighbor);
						Next.Add(Neighbor);
					}
				}
			}
		}
		Frontier = MoveTemp(Next);
	}

	TArray<TSharedPtr<FJsonValue>> SeedsArray;
	for (const TPair<UEdGraphNode*, TArray<FString>>& Pair : SeedReasons)
	{
		TSharedPtr<FJsonObject> SeedObj = BuildBasicNodeObject(Pair.Key);
		SeedObj->SetArrayField(TEXT("reasons"), MakeStringJsonArray(Pair.Value));
		SeedsArray.Add(MakeShareable(new FJsonValueObject(SeedObj)));
	}

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	TSet<FString> SeenEdges;
	TArray<TSharedPtr<FJsonValue>> FocusNodesArray;
	TArray<TSharedPtr<FJsonValue>> FocusEdgesArray;
	for (UEdGraphNode* Node : AllNodes)
	{
		if (!Node || !FocusNodes.Contains(Node))
		{
			continue;
		}

		TSharedPtr<FJsonObject> NodeObj = BuildBasicNodeObject(Node);
		NodeObj->SetBoolField(TEXT("is_seed"), SeedReasons.Contains(Node));
		NodeObj->SetNumberField(TEXT("pin_count"), Node->Pins.Num());
		FocusNodesArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin || Pin->Direction != EGPD_Output)
			{
				continue;
			}

			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin || !LinkedPin->GetOwningNode())
				{
					continue;
				}

				UEdGraphNode* TargetNode = LinkedPin->GetOwningNode();
				if (!FocusNodes.Contains(TargetNode))
				{
					continue;
				}

				const FString Key = FString::Printf(
					TEXT("%s|%s|%s|%s"),
					*Node->NodeGuid.ToString(EGuidFormats::Digits),
					*Pin->PinName.ToString(),
					*TargetNode->NodeGuid.ToString(EGuidFormats::Digits),
					*LinkedPin->PinName.ToString());
				if (SeenEdges.Contains(Key))
				{
					continue;
				}
				SeenEdges.Add(Key);

				TSharedPtr<FJsonObject> EdgeObj = MakeShareable(new FJsonObject());
				EdgeObj->SetStringField(TEXT("source_guid"), Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				EdgeObj->SetStringField(TEXT("source_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				EdgeObj->SetStringField(TEXT("source_pin"), Pin->PinName.ToString());
				EdgeObj->SetStringField(TEXT("target_guid"), TargetNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
				EdgeObj->SetStringField(TEXT("target_title"), TargetNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
				EdgeObj->SetStringField(TEXT("target_pin"), LinkedPin->PinName.ToString());
				EdgeObj->SetStringField(TEXT("link_type"), (Pin->PinType.PinCategory == K2Schema->PC_Exec) ? TEXT("exec") : TEXT("data"));
				FocusEdgesArray.Add(MakeShareable(new FJsonValueObject(EdgeObj)));
			}
		}
	}

	struct FMaterialParameterGroup
	{
		int32 Count = 0;
		TSet<FString> GraphNames;
		TSet<FString> WriteTypes;
		TSet<FString> TargetSummaries;
	};

	TMap<FString, FMaterialParameterGroup> ParameterGroups;
	TArray<TSharedPtr<FJsonValue>> ParameterWritesArray;
	TArray<TSharedPtr<FJsonValue>> DynamicMaterialNodesArray;
	TArray<TSharedPtr<FJsonValue>> MaterialParameterNodesArray;
	TArray<TSharedPtr<FJsonValue>> MaterialInstanceOverridesArray;
	TArray<TSharedPtr<FJsonValue>> RuntimeToMaterialMatchesArray;
	TArray<FString> SummaryLines;

	for (UEdGraphNode* Node : AllSetParamNodes)
	{
		if (!Node || !FocusNodes.Contains(Node))
		{
			continue;
		}

		const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		const FString WriteType = GetMaterialWriteType(NodeTitle);
		FString EffectiveParameterName;
		TArray<FString> ParameterNameSources;
		TArray<FString> ValueSources;
		TArray<FString> TargetSources;
		TArray<FString> ExecSources;
		TArray<FString> MaterialRefs = CollectMaterialRefsFromNode(Node);
		TArray<FString> NeighborDynamicNodes;

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin)
			{
				continue;
			}

			if (Pin->PinName == TEXT("ParameterName"))
			{
				EffectiveParameterName = Pin->DefaultValue;
				ParameterNameSources = CollectLinkedNodeTitles(Pin);
			}
			else if (Pin->PinName == TEXT("Value"))
			{
				ValueSources = CollectLinkedNodeTitles(Pin);
			}
			else if (Pin->PinName == TEXT("self") || Pin->PinName == TEXT("Target"))
			{
				TargetSources = CollectLinkedNodeTitles(Pin);
			}
			else if (Pin->Direction == EGPD_Input && Pin->PinType.PinCategory == K2Schema->PC_Exec)
			{
				ExecSources = CollectLinkedNodeTitles(Pin);
			}
		}

		if (!ParameterNameLower.IsEmpty())
		{
			const bool bDirectMatch = !EffectiveParameterName.IsEmpty() &&
				EffectiveParameterName.Equals(ParameterName, ESearchCase::IgnoreCase);

			bool bSourceMatch = false;
			for (const FString& SourceTitle : ParameterNameSources)
			{
				if (SourceTitle.ToLower().Contains(ParameterNameLower))
				{
					bSourceMatch = true;
					break;
				}
			}

			if (!bDirectMatch && !bSourceMatch)
			{
				continue;
			}
		}

		if (const TSet<UEdGraphNode*>* Neighbors = Adjacency.Find(Node))
		{
			for (UEdGraphNode* Neighbor : *Neighbors)
			{
				if (!Neighbor)
				{
					continue;
				}

				const FString NeighborTitle = Neighbor->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
				if (IsCreateDynamicMaterialNode(NeighborTitle))
				{
					NeighborDynamicNodes.AddUnique(NeighborTitle);
					const TArray<FString> NeighborMaterialRefs = CollectMaterialRefsFromNode(Neighbor);
					for (const FString& Ref : NeighborMaterialRefs)
					{
						MaterialRefs.AddUnique(Ref);
					}
				}
			}
		}

		TSharedPtr<FJsonObject> WriteObj = BuildBasicNodeObject(Node);
		WriteObj->SetStringField(TEXT("write_type"), WriteType);
		WriteObj->SetStringField(TEXT("parameter_name"), EffectiveParameterName);
		WriteObj->SetArrayField(TEXT("parameter_name_sources"), MakeStringJsonArray(ParameterNameSources));
		WriteObj->SetArrayField(TEXT("value_sources"), MakeStringJsonArray(ValueSources));
		WriteObj->SetArrayField(TEXT("target_sources"), MakeStringJsonArray(TargetSources));
		WriteObj->SetArrayField(TEXT("execute_sources"), MakeStringJsonArray(ExecSources));
		WriteObj->SetArrayField(TEXT("material_asset_refs"), MakeStringJsonArray(MaterialRefs));
		WriteObj->SetArrayField(TEXT("direct_neighbor_dynamic_material_nodes"), MakeStringJsonArray(NeighborDynamicNodes));
		WriteObj->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(Node, true));
		ParameterWritesArray.Add(MakeShareable(new FJsonValueObject(WriteObj)));

		const FString GroupKey = EffectiveParameterName.IsEmpty() ? TEXT("<linked-or-empty>") : EffectiveParameterName;
		FMaterialParameterGroup& Group = ParameterGroups.FindOrAdd(GroupKey);
		Group.Count += 1;
		Group.GraphNames.Add(Node->GetGraph() ? Node->GetGraph()->GetName() : TEXT(""));
		Group.WriteTypes.Add(WriteType);
		if (TargetSources.Num() == 0)
		{
			Group.TargetSummaries.Add(TEXT("self/default"));
		}
		else
		{
			for (const FString& TargetSource : TargetSources)
			{
				Group.TargetSummaries.Add(TargetSource);
			}
		}
	}

	for (UEdGraphNode* Node : AllDynamicMaterialNodes)
	{
		if (!Node || !FocusNodes.Contains(Node))
		{
			continue;
		}

		const TArray<FString> MaterialRefs = CollectMaterialRefsFromNode(Node);
		if (!MaterialAssetPathLower.IsEmpty() && !NodeMatchesMaterialAsset(Node, MaterialRefs))
		{
			continue;
		}

		TArray<FString> TargetSources;
		TArray<FString> ExecSources;
		TArray<FString> NearbySetParamNodes;
		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin)
			{
				continue;
			}

			if (Pin->PinName == TEXT("self") || Pin->PinName == TEXT("Target"))
			{
				TargetSources = CollectLinkedNodeTitles(Pin);
			}
			else if (Pin->Direction == EGPD_Input && Pin->PinType.PinCategory == K2Schema->PC_Exec)
			{
				ExecSources = CollectLinkedNodeTitles(Pin);
			}
		}

		if (const TSet<UEdGraphNode*>* Neighbors = Adjacency.Find(Node))
		{
			for (UEdGraphNode* Neighbor : *Neighbors)
			{
				if (!Neighbor)
				{
					continue;
				}

				const FString NeighborTitle = Neighbor->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
				if (IsSetMaterialParameterNode(NeighborTitle))
				{
					NearbySetParamNodes.AddUnique(NeighborTitle);
				}
			}
		}

		TSharedPtr<FJsonObject> DynamicObj = BuildBasicNodeObject(Node);
		DynamicObj->SetArrayField(TEXT("material_asset_refs"), MakeStringJsonArray(MaterialRefs));
		DynamicObj->SetArrayField(TEXT("target_sources"), MakeStringJsonArray(TargetSources));
		DynamicObj->SetArrayField(TEXT("execute_sources"), MakeStringJsonArray(ExecSources));
		DynamicObj->SetArrayField(TEXT("nearby_parameter_write_nodes"), MakeStringJsonArray(NearbySetParamNodes));
		DynamicObj->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(Node, true));
		DynamicMaterialNodesArray.Add(MakeShareable(new FJsonValueObject(DynamicObj)));
	}

	TArray<TSharedPtr<FJsonValue>> ParameterGroupsArray;
	for (const TPair<FString, FMaterialParameterGroup>& Pair : ParameterGroups)
	{
		TArray<FString> GraphNames = Pair.Value.GraphNames.Array();
		TArray<FString> WriteTypes = Pair.Value.WriteTypes.Array();
		TArray<FString> TargetSummaries = Pair.Value.TargetSummaries.Array();

		TSharedPtr<FJsonObject> GroupObj = MakeShareable(new FJsonObject());
		GroupObj->SetStringField(TEXT("parameter_name"), Pair.Key);
		GroupObj->SetNumberField(TEXT("write_count"), Pair.Value.Count);
		GroupObj->SetArrayField(TEXT("graph_names"), MakeStringJsonArray(GraphNames));
		GroupObj->SetArrayField(TEXT("write_types"), MakeStringJsonArray(WriteTypes));
		GroupObj->SetArrayField(TEXT("target_summaries"), MakeStringJsonArray(TargetSummaries));
		ParameterGroupsArray.Add(MakeShareable(new FJsonValueObject(GroupObj)));

		SummaryLines.Add(FString::Printf(
			TEXT("Parameter '%s' is written %d time(s) as [%s] in graphs [%s], targets [%s]."),
			*Pair.Key,
			Pair.Value.Count,
			*FString::Join(WriteTypes, TEXT(", ")),
			*FString::Join(GraphNames, TEXT(", ")),
			*FString::Join(TargetSummaries, TEXT(", "))));
	}

	for (const TSharedPtr<FJsonValue>& DynamicValue : DynamicMaterialNodesArray)
	{
		const TSharedPtr<FJsonObject> DynamicObj = DynamicValue->AsObject();
		if (!DynamicObj.IsValid())
		{
			continue;
		}

		const TArray<TSharedPtr<FJsonValue>>* MaterialRefsArray = nullptr;
		DynamicObj->TryGetArrayField(TEXT("material_asset_refs"), MaterialRefsArray);
		TArray<FString> MaterialRefs;
		if (MaterialRefsArray)
		{
			for (const TSharedPtr<FJsonValue>& RefValue : *MaterialRefsArray)
			{
				MaterialRefs.Add(RefValue->AsString());
			}
		}

		SummaryLines.Add(FString::Printf(
			TEXT("Dynamic material node '%s' appears in graph '%s' with material refs [%s]."),
			*DynamicObj->GetStringField(TEXT("node_title")),
			*DynamicObj->GetStringField(TEXT("graph_name")),
			MaterialRefs.Num() > 0 ? *FString::Join(MaterialRefs, TEXT(", ")) : TEXT("none-detected")));
	}

	if (!MaterialInstancePath.IsEmpty())
	{
		UMaterialInstanceConstant* MaterialInstance = Cast<UMaterialInstanceConstant>(UEditorAssetLibrary::LoadAsset(MaterialInstancePath));
		if (MaterialInstance)
		{
			UMaterial* ParentMaterial = Cast<UMaterial>(MaterialInstance->Parent);
			if (ParentMaterial)
			{
				TMap<FString, TSharedPtr<FJsonObject>> ParameterNodeByName;
				const TArray<UMaterialExpression*>& ParentExpressions = ParentMaterial->GetExpressionCollection().Expressions;
				for (int32 ExpressionIndex = 0; ExpressionIndex < ParentExpressions.Num(); ++ExpressionIndex)
				{
					UMaterialExpression* Expression = ParentExpressions[ExpressionIndex];
					if (!Expression)
					{
						continue;
					}

					FString ParameterNodeName;
					FString ParameterType;
					if (UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expression))
					{
						ParameterNodeName = ScalarParam->ParameterName.ToString();
						ParameterType = TEXT("scalar");
					}
					else if (UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expression))
					{
						ParameterNodeName = VectorParam->ParameterName.ToString();
						ParameterType = TEXT("vector");
					}
					else if (UMaterialExpressionTextureSampleParameter2D* TextureParam = Cast<UMaterialExpressionTextureSampleParameter2D>(Expression))
					{
						ParameterNodeName = TextureParam->ParameterName.ToString();
						ParameterType = TEXT("texture");
					}

					if (ParameterNodeName.IsEmpty())
					{
						continue;
					}

					TSharedPtr<FJsonObject> ParameterNodeObj = MakeShareable(new FJsonObject());
					TArray<FString> CaptionLines;
					Expression->GetCaption(CaptionLines);
					ParameterNodeObj->SetStringField(TEXT("parameter_name"), ParameterNodeName);
					ParameterNodeObj->SetStringField(TEXT("parameter_type"), ParameterType);
					ParameterNodeObj->SetStringField(TEXT("expression_class"), Expression->GetClass()->GetName());
					ParameterNodeObj->SetStringField(TEXT("caption"), CaptionLines.Num() > 0 ? FString::Join(CaptionLines, TEXT(" | ")) : Expression->GetDescription());
					ParameterNodeObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
					ParameterNodeObj->SetNumberField(TEXT("editor_x"), Expression->MaterialExpressionEditorX);
					ParameterNodeObj->SetNumberField(TEXT("editor_y"), Expression->MaterialExpressionEditorY);
					ParameterNodeObj->SetStringField(TEXT("parent_material_path"), ParentMaterial->GetPathName());
					MaterialParameterNodesArray.Add(MakeShareable(new FJsonValueObject(ParameterNodeObj)));
					ParameterNodeByName.Add(ParameterNodeName, ParameterNodeObj);
				}

				auto AddOverrideEntry = [&](const FString& Name, const FString& Type, const FString& ValueSummary)
				{
					TSharedPtr<FJsonObject> OverrideObj = MakeShareable(new FJsonObject());
					OverrideObj->SetStringField(TEXT("parameter_name"), Name);
					OverrideObj->SetStringField(TEXT("parameter_type"), Type);
					OverrideObj->SetStringField(TEXT("value_summary"), ValueSummary);
					OverrideObj->SetBoolField(TEXT("matched_parent_parameter"), ParameterNodeByName.Contains(Name));
					MaterialInstanceOverridesArray.Add(MakeShareable(new FJsonValueObject(OverrideObj)));
				};

				for (const FScalarParameterValue& ScalarValue : MaterialInstance->ScalarParameterValues)
				{
					AddOverrideEntry(ScalarValue.ParameterInfo.Name.ToString(), TEXT("scalar"), FString::SanitizeFloat(ScalarValue.ParameterValue));
				}
				for (const FVectorParameterValue& VectorValue : MaterialInstance->VectorParameterValues)
				{
					AddOverrideEntry(VectorValue.ParameterInfo.Name.ToString(), TEXT("vector"), VectorValue.ParameterValue.ToString());
				}
				for (const FTextureParameterValue& TextureValue : MaterialInstance->TextureParameterValues)
				{
					AddOverrideEntry(
						TextureValue.ParameterInfo.Name.ToString(),
						TEXT("texture"),
						TextureValue.ParameterValue ? TextureValue.ParameterValue->GetPathName() : TEXT("None"));
				}

				for (const TPair<FString, FMaterialParameterGroup>& Pair : ParameterGroups)
				{
					TSharedPtr<FJsonObject> MatchObj = MakeShareable(new FJsonObject());
					MatchObj->SetStringField(TEXT("parameter_name"), Pair.Key);
					MatchObj->SetNumberField(TEXT("runtime_write_count"), Pair.Value.Count);
					MatchObj->SetBoolField(TEXT("has_parent_material_parameter"), ParameterNodeByName.Contains(Pair.Key));

					bool bHasInstanceOverride = false;
					for (const TSharedPtr<FJsonValue>& OverrideValue : MaterialInstanceOverridesArray)
					{
						const TSharedPtr<FJsonObject> OverrideObj = OverrideValue->AsObject();
						if (OverrideObj.IsValid() &&
							OverrideObj->GetStringField(TEXT("parameter_name")).Equals(Pair.Key, ESearchCase::CaseSensitive))
						{
							bHasInstanceOverride = true;
							break;
						}
					}
					MatchObj->SetBoolField(TEXT("has_material_instance_override"), bHasInstanceOverride);
					RuntimeToMaterialMatchesArray.Add(MakeShareable(new FJsonValueObject(MatchObj)));
				}

				if (MaterialParameterNodesArray.Num() > 0)
				{
					SummaryLines.Add(FString::Printf(
						TEXT("Resolved %d parent material parameter node(s) from '%s'."),
						MaterialParameterNodesArray.Num(),
						*ParentMaterial->GetName()));
				}
				if (MaterialInstanceOverridesArray.Num() > 0)
				{
					SummaryLines.Add(FString::Printf(
						TEXT("Material Instance '%s' overrides %d parameter(s)."),
						*MaterialInstance->GetName(),
						MaterialInstanceOverridesArray.Num()));
				}
			}
		}
	}

	if (SummaryLines.Num() == 0)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("Found %d seed node(s), %d focus node(s), %d material parameter write(s), and %d dynamic material node(s)."),
			SeedReasons.Num(),
			FocusNodes.Num(),
			ParameterWritesArray.Num(),
			DynamicMaterialNodesArray.Num()));
	}

	TSharedPtr<FJsonObject> QueryObj = MakeShareable(new FJsonObject());
	QueryObj->SetStringField(TEXT("parameter_name"), ParameterName);
	QueryObj->SetStringField(TEXT("material_asset_path"), MaterialAssetPath);
	QueryObj->SetStringField(TEXT("material_instance_path"), MaterialInstancePath);
	QueryObj->SetStringField(TEXT("node_title_query"), NodeTitleQuery);
	QueryObj->SetNumberField(TEXT("max_depth"), MaxDepth);

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("blueprint_path"), BpPath);
	ResultObj->SetObjectField(TEXT("query"), QueryObj);
	ResultObj->SetBoolField(TEXT("has_matches"), SeedReasons.Num() > 0);
	ResultObj->SetNumberField(TEXT("seed_count"), SeedReasons.Num());
	ResultObj->SetNumberField(TEXT("focus_node_count"), FocusNodes.Num());
	ResultObj->SetNumberField(TEXT("focus_edge_count"), FocusEdgesArray.Num());
	ResultObj->SetNumberField(TEXT("material_parameter_write_count"), ParameterWritesArray.Num());
	ResultObj->SetNumberField(TEXT("dynamic_material_node_count"), DynamicMaterialNodesArray.Num());
	ResultObj->SetArrayField(TEXT("seeds"), SeedsArray);
	ResultObj->SetArrayField(TEXT("focus_nodes"), FocusNodesArray);
	ResultObj->SetArrayField(TEXT("focus_edges"), FocusEdgesArray);
	ResultObj->SetArrayField(TEXT("material_parameter_writes"), ParameterWritesArray);
	ResultObj->SetArrayField(TEXT("dynamic_material_nodes"), DynamicMaterialNodesArray);
	ResultObj->SetArrayField(TEXT("parameter_groups"), ParameterGroupsArray);
	ResultObj->SetArrayField(TEXT("material_parameter_nodes"), MaterialParameterNodesArray);
	ResultObj->SetArrayField(TEXT("material_instance_overrides"), MaterialInstanceOverridesArray);
	ResultObj->SetArrayField(TEXT("runtime_to_material_matches"), RuntimeToMaterialMatchesArray);
	ResultObj->SetArrayField(TEXT("summary_lines"), MakeStringJsonArray(SummaryLines));
	ResultObj->SetStringField(TEXT("human_summary"), FString::Join(SummaryLines, TEXT("\n")));
	if (SeedReasons.Num() == 0)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("No runtime material nodes matched provided filters."));
	}

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintClassDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		OutResult = TEXT("Missing required field: blueprint_path");
		return false;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		OutResult = FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath);
		return false;
	}

	if (!Blueprint->GeneratedClass)
	{
		OutResult = TEXT("Blueprint has no GeneratedClass.");
		return false;
	}

	UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject();
	if (!CDO)
	{
		OutResult = TEXT("Could not access class default object.");
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> DefaultsArray;
	for (const FBPVariableDescription& VarDesc : Blueprint->NewVariables)
	{
		TSharedPtr<FJsonObject> VarObj = MakeShareable(new FJsonObject());
		VarObj->SetStringField(TEXT("name"), VarDesc.VarName.ToString());
		VarObj->SetStringField(TEXT("type"), PinTypeToDisplayString(VarDesc.VarType));

		FProperty* Prop = FindFProperty<FProperty>(Blueprint->GeneratedClass, VarDesc.VarName);
		FString ExportedValue;
		if (Prop)
		{
			Prop->ExportText_InContainer(0, ExportedValue, CDO, CDO, CDO, PPF_None);
		}

		if (ExportedValue.IsEmpty())
		{
			ExportedValue = VarDesc.DefaultValue;
		}

		VarObj->SetStringField(TEXT("default_value"), ExportedValue);
		DefaultsArray.Add(MakeShareable(new FJsonValueObject(VarObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("blueprint"), Blueprint->GetPathName());
	ResultObj->SetNumberField(TEXT("count"), DefaultsArray.Num());
	ResultObj->SetArrayField(TEXT("defaults"), DefaultsArray);
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_InspectBlueprintArrayDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	FString ArrayNameFilter;
	Input->TryGetStringField(TEXT("array_name"), ArrayNameFilter);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	if (!Blueprint->GeneratedClass)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NO_GENERATED_CLASS"), TEXT("Blueprint has no GeneratedClass."));
	}

	UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject();
	if (!CDO)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NO_CDO"), TEXT("Could not access class default object."));
	}

	const FString ArrayNameFilterLower = ArrayNameFilter.ToLower();
	TArray<TSharedPtr<FJsonValue>> ArraysArray;

	for (TFieldIterator<FProperty> It(Blueprint->GeneratedClass, EFieldIterationFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;
		FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property);
		if (!ArrayProp)
		{
			continue;
		}

		const FString PropertyName = ArrayProp->GetName();
		if (!ArrayNameFilterLower.IsEmpty() &&
			!PropertyName.Equals(ArrayNameFilter, ESearchCase::IgnoreCase) &&
			!PropertyName.ToLower().Contains(ArrayNameFilterLower))
		{
			continue;
		}

		const void* ValuePtr = ArrayProp->ContainerPtrToValuePtr<void>(CDO);
		FScriptArrayHelper ArrayHelper(ArrayProp, ValuePtr);

		TArray<TSharedPtr<FJsonValue>> ElementsArray;
		for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
		{
			const void* ElementPtr = ArrayHelper.GetRawPtr(Index);
			const FString ExportedValue = ExportPropertyValueToString(ArrayProp->Inner, ElementPtr);

			FString ObjectPath;
			if (const FObjectPropertyBase* ObjectProp = CastField<FObjectPropertyBase>(ArrayProp->Inner))
			{
				if (UObject* ObjectValue = ObjectProp->GetObjectPropertyValue(ElementPtr))
				{
					ObjectPath = ObjectValue->GetPathName();
				}
			}
			else if (const FSoftObjectProperty* SoftObjectProp = CastField<FSoftObjectProperty>(ArrayProp->Inner))
			{
				ObjectPath = SoftObjectProp->GetPropertyValue(ElementPtr).ToSoftObjectPath().ToString();
			}

			TSharedPtr<FJsonObject> ElementObj = MakeShareable(new FJsonObject());
			ElementObj->SetNumberField(TEXT("index"), Index);
			ElementObj->SetStringField(TEXT("exported_value"), ExportedValue);
			ElementObj->SetStringField(TEXT("object_path"), ObjectPath);
			ElementsArray.Add(MakeShareable(new FJsonValueObject(ElementObj)));
		}

		TSharedPtr<FJsonObject> ArrayObj = MakeShareable(new FJsonObject());
		ArrayObj->SetStringField(TEXT("property_name"), PropertyName);
		ArrayObj->SetStringField(TEXT("inner_cpp_type"), ArrayProp->Inner ? ArrayProp->Inner->GetCPPType() : TEXT(""));
		ArrayObj->SetStringField(TEXT("property_owner_struct"), ArrayProp->GetOwnerStruct() ? ArrayProp->GetOwnerStruct()->GetPathName() : TEXT(""));
		ArrayObj->SetNumberField(TEXT("element_count"), ArrayHelper.Num());
		ArrayObj->SetArrayField(TEXT("elements"), ElementsArray);
		ArraysArray.Add(MakeShareable(new FJsonValueObject(ArrayObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("blueprint_path"), BpPath);
	ResultObj->SetStringField(TEXT("array_name_filter"), ArrayNameFilter);
	ResultObj->SetNumberField(TEXT("count"), ArraysArray.Num());
	ResultObj->SetArrayField(TEXT("arrays"), ArraysArray);
	ResultObj->SetStringField(TEXT("message"),
		ArraysArray.Num() > 0
			? TEXT("Blueprint array defaults inspected.")
			: TEXT("No matching Blueprint array defaults found."));

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetBlueprintMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	int32 MaxResults = 20;
	Input->TryGetNumberField(TEXT("max_results"), MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 200);

	TArray<FString> SnapshotFiles;
	GetBlueprintSnapshotFiles(Blueprint, SnapshotFiles);

	TArray<TSharedPtr<FJsonValue>> SnapshotArray;
	for (int32 Index = 0; Index < SnapshotFiles.Num() && Index < MaxResults; ++Index)
	{
		const FString& SnapshotPath = SnapshotFiles[Index];
		TSharedPtr<FJsonObject> SnapshotObj = MakeShareable(new FJsonObject());
		SnapshotObj->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		SnapshotObj->SetStringField(TEXT("file_name"), FPaths::GetCleanFilename(SnapshotPath));
		SnapshotObj->SetStringField(TEXT("timestamp_utc"), IFileManager::Get().GetTimeStamp(*SnapshotPath).ToIso8601());
		SnapshotObj->SetNumberField(TEXT("size_bytes"), static_cast<double>(IFileManager::Get().FileSize(*SnapshotPath)));
		SnapshotArray.Add(MakeShareable(new FJsonValueObject(SnapshotObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("blueprint_path"), BpPath);
	ResultObj->SetStringField(TEXT("snapshot_root"), GetBlueprintMutationSnapshotRoot());
	ResultObj->SetNumberField(TEXT("count"), SnapshotArray.Num());
	ResultObj->SetArrayField(TEXT("snapshots"), SnapshotArray);
	ResultObj->SetStringField(TEXT("message"), SnapshotArray.Num() > 0
		? TEXT("Blueprint mutation snapshots found.")
		: TEXT("No blueprint mutation snapshots found."));

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_CreateBlueprint(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpName;
	if (!Input->TryGetStringField(TEXT("blueprint_name"), BpName))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_NAME"), TEXT("Missing required field: blueprint_name"));
	}

	FString ParentClassStr = TEXT("Actor");
	Input->TryGetStringField(TEXT("parent_class"), ParentClassStr);

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	// Find parent class
	UClass* FoundParentClass = FindFirstObject<UClass>(*ParentClassStr, EFindFirstObjectOptions::NativeFirst);
	if (!FoundParentClass)
	{
		FoundParentClass = FindFirstObject<UClass>(*(TEXT("A") + ParentClassStr), EFindFirstObjectOptions::NativeFirst);
	}
	if (!FoundParentClass)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		AssetRegistryModule.Get().GetAllAssets(AssetData);
		for (const FAssetData& Data : AssetData)
		{
			if (Data.AssetName.ToString() == ParentClassStr)
			{
				UBlueprint* BpAsset = Cast<UBlueprint>(Data.GetAsset());
				if (BpAsset && BpAsset->GeneratedClass)
				{
					FoundParentClass = BpAsset->GeneratedClass;
					break;
				}
			}
		}
	}

	if (!FoundParentClass)
	{
		TSharedPtr<FJsonObject> DebugObj = MakeShareable(new FJsonObject());
		DebugObj->SetStringField(TEXT("requested_parent_class"), ParentClassStr);
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_PARENT_CLASS_NOT_FOUND"),
			FString::Printf(TEXT("Parent class '%s' could not be found."), *ParentClassStr),
			DebugObj);
	}

	FString PackagePath = SavePath;
	if (!PackagePath.EndsWith(TEXT("/")))
	{
		PackagePath += TEXT("/");
	}
	PackagePath += BpName;

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		TSharedPtr<FJsonObject> DebugObj = MakeShareable(new FJsonObject());
		DebugObj->SetStringField(TEXT("package_path"), PackagePath);
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_ASSET_ALREADY_EXISTS"),
			FString::Printf(TEXT("Asset already exists at path '%s'."), *PackagePath),
			DebugObj);
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("create_blueprint"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("planned_asset_path"), PackagePath);
		Result->SetStringField(TEXT("resolved_parent_class"), FoundParentClass->GetPathName());
		Result->SetStringField(TEXT("safety_snapshot_root"), GetBlueprintMutationSnapshotRoot());
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create Blueprint '%s' at '%s'"), *BpName, *PackagePath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
		FoundParentClass,
		CreatePackage(*PackagePath),
		FName(*BpName),
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass());

	if (!NewBlueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_CREATE_FAILED"),
			TEXT("FKismetEditorUtilities::CreateBlueprint returned null."));
	}

	FAssetRegistryModule::AssetCreated(NewBlueprint);
	NewBlueprint->MarkPackageDirty();
	FKismetEditorUtilities::CompileBlueprint(NewBlueprint);

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	FString PackageFileName = FPackageName::LongPackageNameToFilename(
		PackagePath, FPackageName::GetAssetPackageExtension());
	const bool bSaveSucceeded = UPackage::SavePackage(NewBlueprint->GetPackage(), NewBlueprint, *PackageFileName, SaveArgs);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("create_blueprint"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("asset_path"), NewBlueprint->GetPathName());
	Result->SetStringField(TEXT("resolved_parent_class"), FoundParentClass->GetPathName());
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(TEXT("create_blueprint"), NewBlueprint, TEXT(""), bSaveSucceeded));
	const FString CreateMessage = FString::Printf(TEXT("Created Blueprint '%s' with parent class '%s'"), *BpName, *FoundParentClass->GetName());
	Result->SetStringField(TEXT("message"), CreateMessage);
	AppendBlueprintMutationAudit(Result, TEXT("create_blueprint"), NewBlueprint, TEXT(""), bSaveSucceeded, CreateMessage);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_AddVariable(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath, VarName, VarType;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) ||
		!Input->TryGetStringField(TEXT("variable_name"), VarName) ||
		!Input->TryGetStringField(TEXT("variable_type"), VarType))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_MISSING_REQUIRED_FIELDS"),
			TEXT("Missing required fields: blueprint_path, variable_name, variable_type"));
	}

	FString DefaultValue;
	Input->TryGetStringField(TEXT("default_value"), DefaultValue);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		TSharedPtr<FJsonObject> DebugObj = MakeShareable(new FJsonObject());
		DebugObj->SetStringField(TEXT("blueprint_path"), BpPath);
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_LOAD_FAILED"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath),
			DebugObj);
	}

	// Check if variable already exists
	if (FBlueprintEditorUtils::FindMemberVariableGuidByName(Blueprint, FName(*VarName)).IsValid())
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_VARIABLE_ALREADY_EXISTS"),
			FString::Printf(TEXT("Variable '%s' already exists in this Blueprint."), *VarName));
	}

	// Convert type
	FEdGraphPinType PinType;
	if (!ConvertTypeToPinType(VarType, PinType))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_UNSUPPORTED_VARIABLE_TYPE"),
			FString::Printf(TEXT("Unknown or unsupported variable type: %s"), *VarType));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("add_variable_to_blueprint"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("blueprint_path"), BpPath);
		Result->SetStringField(TEXT("variable_name"), VarName);
		Result->SetStringField(TEXT("variable_type"), VarType);
		Result->SetStringField(TEXT("default_value"), DefaultValue);
		Result->SetStringField(TEXT("safety_snapshot_root"), GetBlueprintMutationSnapshotRoot());
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would add variable '%s' (%s)"), *VarName, *VarType));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateBlueprintMutationSnapshot(Blueprint, TEXT("add_variable"), SnapshotPath, SnapshotError))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_FAILED"),
			SnapshotError);
	}

	// Add variable
	FBlueprintEditorUtils::AddMemberVariable(Blueprint, FName(*VarName), PinType, DefaultValue);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint);
	const bool bSaveSucceeded = UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("variable_name"), VarName);
	PatchDetails->SetStringField(TEXT("variable_type"), VarType);
	PatchDetails->SetStringField(TEXT("default_value"), DefaultValue);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("add_variable_to_blueprint"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(TEXT("add_variable_to_blueprint"), Blueprint, SnapshotPath, bSaveSucceeded, PatchDetails));
	const FString AddVariableMessage = FString::Printf(TEXT("Added variable '%s' (%s) to Blueprint"), *VarName, *VarType);
	Result->SetStringField(TEXT("message"), AddVariableMessage);
	AppendBlueprintMutationAudit(Result, TEXT("add_variable_to_blueprint"), Blueprint, SnapshotPath, bSaveSucceeded, AddVariableMessage, PatchDetails);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_AddComponent(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath, ComponentClass, ComponentName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) ||
		!Input->TryGetStringField(TEXT("component_class"), ComponentClass) ||
		!Input->TryGetStringField(TEXT("component_name"), ComponentName))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_MISSING_REQUIRED_FIELDS"),
			TEXT("Missing required fields: blueprint_path, component_class, component_name"));
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_LOAD_FAILED"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	// Find component class
	UClass* FoundComponentClass = FindFirstObject<UClass>(*ComponentClass, EFindFirstObjectOptions::NativeFirst);
	if (!FoundComponentClass)
	{
		// Try appending "Component" suffix
		FoundComponentClass = FindFirstObject<UClass>(*(ComponentClass + TEXT("Component")), EFindFirstObjectOptions::NativeFirst);
	}
	if (!FoundComponentClass)
	{
		// Try with U prefix
		FoundComponentClass = FindFirstObject<UClass>(*(TEXT("U") + ComponentClass), EFindFirstObjectOptions::NativeFirst);
	}

	if (!FoundComponentClass || !FoundComponentClass->IsChildOf(UActorComponent::StaticClass()))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_CLASS_INVALID"),
			FString::Printf(TEXT("Component class '%s' not found or not a valid Actor Component."), *ComponentClass));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("add_component_to_blueprint"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("blueprint_path"), BpPath);
		Result->SetStringField(TEXT("component_name"), ComponentName);
		Result->SetStringField(TEXT("resolved_component_class"), FoundComponentClass->GetPathName());
		Result->SetStringField(TEXT("safety_snapshot_root"), GetBlueprintMutationSnapshotRoot());
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would add component '%s' of class '%s'"), *ComponentName, *FoundComponentClass->GetName()));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateBlueprintMutationSnapshot(Blueprint, TEXT("add_component"), SnapshotPath, SnapshotError))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_FAILED"),
			SnapshotError);
	}

	// Create instance
	UActorComponent* NewComponentInstance = NewObject<UActorComponent>(
		GetTransientPackage(), FoundComponentClass, FName(*ComponentName));
	if (!NewComponentInstance)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_CREATE_FAILED"),
			FString::Printf(TEXT("Failed to create component instance for '%s'."), *ComponentName));
	}

	TArray<UActorComponent*> ComponentsToAdd;
	ComponentsToAdd.Add(NewComponentInstance);
	FKismetEditorUtilities::FAddComponentsToBlueprintParams Params;
	FKismetEditorUtilities::AddComponentsToBlueprint(Blueprint, ComponentsToAdd, Params);

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint);
	const bool bSaveSucceeded = UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("component_name"), ComponentName);
	PatchDetails->SetStringField(TEXT("resolved_component_class"), FoundComponentClass->GetPathName());

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("add_component_to_blueprint"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(TEXT("add_component_to_blueprint"), Blueprint, SnapshotPath, bSaveSucceeded, PatchDetails));
	const FString AddComponentMessage = FString::Printf(TEXT("Added %s '%s' to Blueprint"), *FoundComponentClass->GetName(), *ComponentName);
	Result->SetStringField(TEXT("message"), AddComponentMessage);
	AppendBlueprintMutationAudit(Result, TEXT("add_component_to_blueprint"), Blueprint, SnapshotPath, bSaveSucceeded, AddComponentMessage, PatchDetails);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_EditComponentProperty(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath, ComponentName, PropertyName, PropertyValue;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) ||
		!Input->TryGetStringField(TEXT("component_name"), ComponentName) ||
		!Input->TryGetStringField(TEXT("property_name"), PropertyName) ||
		!Input->TryGetStringField(TEXT("property_value"), PropertyValue))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_MISSING_REQUIRED_FIELDS"),
			TEXT("Missing required fields: blueprint_path, component_name, property_name, property_value"));
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_LOAD_FAILED"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	if (!Blueprint->SimpleConstructionScript)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NO_SCS"),
			TEXT("This Blueprint has no SimpleConstructionScript (not an Actor Blueprint)."));
	}

	USCS_Node* TargetNode = Blueprint->SimpleConstructionScript->FindSCSNode(FName(*ComponentName));
	if (!TargetNode)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Component '%s' not found in Blueprint."), *ComponentName));
	}

	UActorComponent* ComponentTemplate = TargetNode->ComponentTemplate;
	if (!ComponentTemplate)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_TEMPLATE_INVALID"),
			FString::Printf(TEXT("Component template for '%s' is invalid."), *ComponentName));
	}

	// Find property
	FProperty* Property = ComponentTemplate->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		// Collect suggestions
		TArray<FString> Suggestions;
		for (TFieldIterator<FProperty> PropIt(ComponentTemplate->GetClass()); PropIt; ++PropIt)
		{
			FString PropName = PropIt->GetName();
			if (PropName.Contains(PropertyName, ESearchCase::IgnoreCase))
			{
				Suggestions.Add(PropName);
				if (Suggestions.Num() >= 10) break;
			}
		}

		FString SuggestionStr = Suggestions.Num() > 0
			? FString::Join(Suggestions, TEXT(", "))
			: TEXT("(no similar names found)");

		const FString PropertyErrorMessage = FString::Printf(
			TEXT("Property '%s' not found on component '%s'. Did you mean: %s"),
			*PropertyName, *ComponentName, *SuggestionStr);
		return BuildBlueprintToolError(OutResult, TEXT("BP_PROPERTY_NOT_FOUND"), PropertyErrorMessage);
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("edit_component_property"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("blueprint_path"), BpPath);
		Result->SetStringField(TEXT("component_name"), ComponentName);
		Result->SetStringField(TEXT("property_name"), PropertyName);
		Result->SetStringField(TEXT("property_value"), PropertyValue);
		Result->SetStringField(TEXT("safety_snapshot_root"), GetBlueprintMutationSnapshotRoot());
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would set '%s.%s' = '%s'"), *ComponentName, *PropertyName, *PropertyValue));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateBlueprintMutationSnapshot(Blueprint, TEXT("edit_component_property"), SnapshotPath, SnapshotError))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_FAILED"),
			SnapshotError);
	}

	// Set property value
	bool bSuccess = false;
	void* PropertyData = Property->ContainerPtrToValuePtr<void>(ComponentTemplate);
	if (PropertyData)
	{
		if (Property->ImportText_Direct(*PropertyValue, PropertyData, nullptr, PPF_None))
		{
			bSuccess = true;
		}
	}

	// Fallback for struct types
	if (!bSuccess && PropertyData)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
		{
			if (StructProp->Struct == TBaseStructure<FVector>::Get())
			{
				FVector VectorValue;
				if (VectorValue.InitFromString(PropertyValue))
				{
					*static_cast<FVector*>(PropertyData) = VectorValue;
					bSuccess = true;
				}
			}
			else if (StructProp->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator RotatorValue;
				if (RotatorValue.InitFromString(PropertyValue))
				{
					*static_cast<FRotator*>(PropertyData) = RotatorValue;
					bSuccess = true;
				}
			}
			else if (StructProp->Struct == TBaseStructure<FLinearColor>::Get())
			{
				FLinearColor ColorValue;
				if (ColorValue.InitFromString(PropertyValue))
				{
					*static_cast<FLinearColor*>(PropertyData) = ColorValue;
					bSuccess = true;
				}
			}
		}
	}

	if (!bSuccess)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_PROPERTY_SET_FAILED"),
			FString::Printf(
				TEXT("Failed to set property '%s' to value '%s'. The value format may be incorrect for the property type."),
				*PropertyName, *PropertyValue));
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint);
	const bool bSaveSucceeded = UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("component_name"), ComponentName);
	PatchDetails->SetStringField(TEXT("property_name"), PropertyName);
	PatchDetails->SetStringField(TEXT("property_value"), PropertyValue);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("edit_component_property"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(TEXT("edit_component_property"), Blueprint, SnapshotPath, bSaveSucceeded, PatchDetails));
	const FString EditPropertyMessage = FString::Printf(TEXT("Set property '%s' on component '%s' to '%s'"), *PropertyName, *ComponentName, *PropertyValue);
	Result->SetStringField(TEXT("message"), EditPropertyMessage);
	AppendBlueprintMutationAudit(Result, TEXT("edit_component_property"), Blueprint, SnapshotPath, bSaveSucceeded, EditPropertyMessage, PatchDetails);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RestoreBlueprintMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	FString SnapshotPath;
	Input->TryGetStringField(TEXT("snapshot_path"), SnapshotPath);
	const bool bUseLatest = ParseOptionalBoolField(Input, TEXT("use_latest"), true);
	const bool bReopenEditor = ParseOptionalBoolField(Input, TEXT("reopen_editor"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	if (SnapshotPath.IsEmpty() && bUseLatest)
	{
		TArray<FString> SnapshotFiles;
		GetBlueprintSnapshotFiles(Blueprint, SnapshotFiles);
		if (SnapshotFiles.Num() > 0)
		{
			SnapshotPath = SnapshotFiles[0];
		}
	}

	if (SnapshotPath.IsEmpty())
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_NOT_SPECIFIED"),
			TEXT("No snapshot_path was provided and no latest snapshot could be resolved."));
	}

	SnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
	if (!FPaths::FileExists(SnapshotPath))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_NOT_FOUND"),
			FString::Printf(TEXT("Snapshot file does not exist: %s"), *SnapshotPath));
	}

	if (!IsSnapshotPathUnderRoot(SnapshotPath))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_OUT_OF_BOUNDS"),
			TEXT("Snapshot path must live under the ForgePilot BlueprintMutationSnapshots directory."));
	}

	const FString PackageFilename = GetBlueprintPackageFilename(Blueprint);
	if (PackageFilename.IsEmpty() || !FPaths::FileExists(PackageFilename))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_PACKAGE_FILE_MISSING"),
			FString::Printf(TEXT("Blueprint package file does not exist on disk: %s"), *PackageFilename));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("restore_blueprint_mutation_snapshot"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("blueprint_path"), BpPath);
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetStringField(TEXT("package_filename"), PackageFilename);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would restore the Blueprint package from the selected snapshot."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString PreRestoreSnapshotPath;
	FString PreRestoreSnapshotError;
	if (!CreateBlueprintMutationSnapshot(Blueprint, TEXT("pre_restore"), PreRestoreSnapshotPath, PreRestoreSnapshotError))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_FAILED"),
			PreRestoreSnapshotError);
	}

	int32 ClosedEditorCount = 0;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			ClosedEditorCount = AssetEditorSubsystem->CloseAllEditorsForAsset(Blueprint);
		}
	}

	const uint32 CopyResult = IFileManager::Get().Copy(*PackageFilename, *SnapshotPath, true, true);
	if (CopyResult != COPY_OK)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_RESTORE_COPY_FAILED"),
			FString::Printf(TEXT("Failed to copy snapshot into package file (copy code %u)."), CopyResult));
	}

	FText ReloadError;
	const bool bReloadSucceeded = UPackageTools::ReloadPackages(
		{ Blueprint->GetOutermost() },
		ReloadError,
		EReloadPackagesInteractionMode::AssumePositive);

	UObject* ReloadedAsset = UEditorAssetLibrary::LoadAsset(BpPath);
	UBlueprint* ReloadedBlueprint = Cast<UBlueprint>(ReloadedAsset);

	if (bReopenEditor && ReloadedAsset && GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			AssetEditorSubsystem->OpenEditorForAsset(ReloadedAsset);
		}
	}

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("restored_from_snapshot"), SnapshotPath);
	PatchDetails->SetStringField(TEXT("pre_restore_snapshot"), PreRestoreSnapshotPath);
	PatchDetails->SetStringField(TEXT("package_filename"), PackageFilename);
	PatchDetails->SetNumberField(TEXT("closed_editor_count"), ClosedEditorCount);
	if (!ReloadError.IsEmpty())
	{
		PatchDetails->SetStringField(TEXT("reload_error"), ReloadError.ToString());
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), bReloadSucceeded);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("restore_blueprint_mutation_snapshot"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(
		TEXT("restore_blueprint_mutation_snapshot"),
		ReloadedBlueprint ? ReloadedBlueprint : Blueprint,
		PreRestoreSnapshotPath,
		bReloadSucceeded,
		PatchDetails));
	const FString RestoreMessage = bReloadSucceeded
		? TEXT("Blueprint snapshot restored and package reloaded.")
		: TEXT("Snapshot file was copied, but package reload reported a problem. See patch_report details.");
	Result->SetStringField(TEXT("message"), RestoreMessage);
	AppendBlueprintMutationAudit(
		Result,
		TEXT("restore_blueprint_mutation_snapshot"),
		ReloadedBlueprint ? ReloadedBlueprint : Blueprint,
		PreRestoreSnapshotPath,
		bReloadSucceeded,
		RestoreMessage,
		PatchDetails);

	OutResult = JsonObjToString(Result);
	return bReloadSucceeded;
}

bool FVFXToolRegistry::Tool_CreateCustomEventNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString GraphName;
	FString EventName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("graph_name"), GraphName) || GraphName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_GRAPH_NAME"), TEXT("Missing required field: graph_name"));
	}
	if (!Input->TryGetStringField(TEXT("event_name"), EventName) || EventName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_EVENT_NAME"), TEXT("Missing required field: event_name"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraph* TargetGraph = FindBlueprintGraphByName(Blueprint, GraphName);
	if (!TargetGraph)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_GRAPH_NOT_FOUND"), FString::Printf(TEXT("Graph '%s' was not found in Blueprint '%s'."), *GraphName, *BpPath));
	}

	if (UK2Node_CustomEvent* ExistingEvent = FindBlueprintCustomEventByName(Blueprint, EventName))
	{
		TSharedPtr<FJsonObject> DebugObj = MakeShareable(new FJsonObject());
		DebugObj->SetStringField(TEXT("existing_node_guid"), ExistingEvent->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
		DebugObj->SetStringField(TEXT("existing_graph_name"), ExistingEvent->GetGraph() ? ExistingEvent->GetGraph()->GetName() : TEXT(""));
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_CUSTOM_EVENT_ALREADY_EXISTS"),
			FString::Printf(TEXT("Custom event '%s' already exists in Blueprint '%s'."), *EventName, *BpPath),
			DebugObj);
	}

	int32 NodePosX = 0;
	int32 NodePosY = 0;
	double NumberValue = 0.0;
	if (Input->TryGetNumberField(TEXT("node_pos_x"), NumberValue))
	{
		NodePosX = FMath::RoundToInt(NumberValue);
	}
	if (Input->TryGetNumberField(TEXT("node_pos_y"), NumberValue))
	{
		NodePosY = FMath::RoundToInt(NumberValue);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_custom_event_node"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("graph_name"), TargetGraph->GetName());
	Result->SetStringField(TEXT("event_name"), EventName);
	Result->SetNumberField(TEXT("node_pos_x"), NodePosX);
	Result->SetNumberField(TEXT("node_pos_y"), NodePosY);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "CreateCustomEventNode", "Create Custom Event Node"));
	Blueprint->Modify();
	TargetGraph->Modify();

	UK2Node_CustomEvent* EventNode = NewObject<UK2Node_CustomEvent>(TargetGraph);
	EventNode->SetFlags(RF_Transactional);
	EventNode->CreateNewGuid();
	EventNode->CustomFunctionName = FName(*EventName);
	EventNode->NodePosX = NodePosX;
	EventNode->NodePosY = NodePosY;
	TargetGraph->AddNode(EventNode, true, false);
	EventNode->PostPlacedNewNode();
	EventNode->AllocateDefaultPins();
	EventNode->ReconstructNode();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetStringField(TEXT("node_guid"), EventNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetStringField(TEXT("node_title"), EventNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	Result->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(EventNode, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateBlueprintVariableSetNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString GraphName;
	FString VariableName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("graph_name"), GraphName) || GraphName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_GRAPH_NAME"), TEXT("Missing required field: graph_name"));
	}
	if (!Input->TryGetStringField(TEXT("variable_name"), VariableName) || VariableName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_VARIABLE_NAME"), TEXT("Missing required field: variable_name"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraph* TargetGraph = FindBlueprintGraphByName(Blueprint, GraphName);
	if (!TargetGraph)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_GRAPH_NOT_FOUND"), FString::Printf(TEXT("Graph '%s' was not found in Blueprint '%s'."), *GraphName, *BpPath));
	}

	int32 NodePosX = 0;
	int32 NodePosY = 0;
	double NumberValue = 0.0;
	if (Input->TryGetNumberField(TEXT("node_pos_x"), NumberValue))
	{
		NodePosX = FMath::RoundToInt(NumberValue);
	}
	if (Input->TryGetNumberField(TEXT("node_pos_y"), NumberValue))
	{
		NodePosY = FMath::RoundToInt(NumberValue);
	}

	FProperty* VariableProperty = nullptr;
	bool bSelfContext = true;
	UClass* OwnerClass = nullptr;
	FString ResolvedOwnerPath;
	FString ResolveError;
	if (!ResolveBlueprintVariableProperty(Blueprint, VariableName, VariableProperty, bSelfContext, OwnerClass, ResolvedOwnerPath, ResolveError))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_VARIABLE_NOT_FOUND"), ResolveError);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_blueprint_variable_set_node"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("graph_name"), TargetGraph->GetName());
	Result->SetStringField(TEXT("variable_name"), VariableName);
	Result->SetStringField(TEXT("resolved_owner_path"), ResolvedOwnerPath);
	Result->SetNumberField(TEXT("node_pos_x"), NodePosX);
	Result->SetNumberField(TEXT("node_pos_y"), NodePosY);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "CreateBlueprintVariableSetNode", "Create Blueprint Variable Set Node"));
	Blueprint->Modify();
	TargetGraph->Modify();

	UK2Node_VariableSet* SetterNode = NewObject<UK2Node_VariableSet>(TargetGraph);
	SetterNode->SetFlags(RF_Transactional);
	SetterNode->SetFromProperty(VariableProperty, bSelfContext, OwnerClass);
	SetterNode->NodePosX = NodePosX;
	SetterNode->NodePosY = NodePosY;
	TargetGraph->AddNode(SetterNode, true, false);
	SetterNode->CreateNewGuid();
	SetterNode->PostPlacedNewNode();
	SetterNode->AllocateDefaultPins();
	SetterNode->ReconstructNode();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetStringField(TEXT("node_guid"), SetterNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetStringField(TEXT("node_title"), SetterNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	Result->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(SetterNode, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateBlueprintVariableGetNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString GraphName;
	FString VariableName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("graph_name"), GraphName) || GraphName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_GRAPH_NAME"), TEXT("Missing required field: graph_name"));
	}
	if (!Input->TryGetStringField(TEXT("variable_name"), VariableName) || VariableName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_VARIABLE_NAME"), TEXT("Missing required field: variable_name"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraph* TargetGraph = FindBlueprintGraphByName(Blueprint, GraphName);
	if (!TargetGraph)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_GRAPH_NOT_FOUND"), FString::Printf(TEXT("Graph '%s' was not found in Blueprint '%s'."), *GraphName, *BpPath));
	}

	int32 NodePosX = 0;
	int32 NodePosY = 0;
	double NumberValue = 0.0;
	if (Input->TryGetNumberField(TEXT("node_pos_x"), NumberValue))
	{
		NodePosX = FMath::RoundToInt(NumberValue);
	}
	if (Input->TryGetNumberField(TEXT("node_pos_y"), NumberValue))
	{
		NodePosY = FMath::RoundToInt(NumberValue);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	FProperty* VariableProperty = nullptr;
	bool bSelfContext = true;
	UClass* OwnerClass = nullptr;
	FString ResolvedOwnerPath;
	FString ResolveError;
	if (!ResolveBlueprintVariableProperty(Blueprint, VariableName, VariableProperty, bSelfContext, OwnerClass, ResolvedOwnerPath, ResolveError))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_VARIABLE_NOT_FOUND"), ResolveError);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_blueprint_variable_get_node"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("graph_name"), TargetGraph->GetName());
	Result->SetStringField(TEXT("variable_name"), VariableName);
	Result->SetStringField(TEXT("resolved_owner_path"), ResolvedOwnerPath);
	Result->SetNumberField(TEXT("node_pos_x"), NodePosX);
	Result->SetNumberField(TEXT("node_pos_y"), NodePosY);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "CreateBlueprintVariableGetNode", "Create Blueprint Variable Get Node"));
	Blueprint->Modify();
	TargetGraph->Modify();

	UK2Node_VariableGet* GetterNode = NewObject<UK2Node_VariableGet>(TargetGraph);
	GetterNode->SetFromProperty(VariableProperty, bSelfContext, OwnerClass);
	GetterNode->NodePosX = NodePosX;
	GetterNode->NodePosY = NodePosY;
	TargetGraph->AddNode(GetterNode, true, false);
	GetterNode->CreateNewGuid();
	GetterNode->PostPlacedNewNode();
	GetterNode->AllocateDefaultPins();
	GetterNode->ReconstructNode();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	Result->SetStringField(TEXT("node_guid"), GetterNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetStringField(TEXT("node_title"), GetterNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	Result->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(GetterNode, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_AttachComponentToComponentSocket(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString ChildComponentName;
	FString ParentComponentName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("child_component_name"), ChildComponentName) || ChildComponentName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_CHILD_COMPONENT_NAME"), TEXT("Missing required field: child_component_name"));
	}
	if (!Input->TryGetStringField(TEXT("parent_component_name"), ParentComponentName) || ParentComponentName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_PARENT_COMPONENT_NAME"), TEXT("Missing required field: parent_component_name"));
	}

	FString SocketName;
	Input->TryGetStringField(TEXT("socket_name"), SocketName);

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_LOAD_FAILED"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	if (!Blueprint->SimpleConstructionScript)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NO_SCS"),
			TEXT("This Blueprint has no SimpleConstructionScript (not an Actor Blueprint)."));
	}

	USCS_Node* ChildNode = Blueprint->SimpleConstructionScript->FindSCSNode(FName(*ChildComponentName));
	if (!ChildNode)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_CHILD_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Child component '%s' was not found in Blueprint '%s'."), *ChildComponentName, *BpPath));
	}

	USCS_Node* ParentNode = Blueprint->SimpleConstructionScript->FindSCSNode(FName(*ParentComponentName));
	if (!ParentNode)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_PARENT_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Parent component '%s' was not found in Blueprint '%s'."), *ParentComponentName, *BpPath));
	}

	if (ChildNode == ParentNode)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_ATTACH_SELF"),
			TEXT("child_component_name and parent_component_name must be different."));
	}

	if (ParentNode->IsChildOf(ChildNode))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_COMPONENT_ATTACH_CYCLE"),
			FString::Printf(TEXT("Cannot attach '%s' under '%s' because it would create a cycle in the component hierarchy."), *ChildComponentName, *ParentComponentName));
	}

	if (!Cast<USceneComponent>(ChildNode->ComponentTemplate))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_CHILD_NOT_SCENE_COMPONENT"),
			FString::Printf(TEXT("Child component '%s' is not a SceneComponent and cannot be attached to a socket."), *ChildComponentName));
	}

	if (!Cast<USceneComponent>(ParentNode->ComponentTemplate))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_PARENT_NOT_SCENE_COMPONENT"),
			FString::Printf(TEXT("Parent component '%s' is not a SceneComponent and cannot host child socket attachments."), *ParentComponentName));
	}

	USCS_Node* PreviousParent = Blueprint->SimpleConstructionScript->FindParentNode(ChildNode);
	const FString PreviousParentName = PreviousParent ? PreviousParent->GetVariableName().ToString() : ChildNode->ParentComponentOrVariableName.ToString();
	const FString PreviousSocketName = ChildNode->AttachToName.ToString();

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	if (!bApplyChanges)
	{
		const FString SocketSuffix = SocketName.IsEmpty()
			? FString()
			: FString::Printf(TEXT(" at socket '%s'"), *SocketName);

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("attach_component_to_component_socket"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("blueprint_path"), BpPath);
		Result->SetStringField(TEXT("child_component_name"), ChildComponentName);
		Result->SetStringField(TEXT("parent_component_name"), ParentComponentName);
		Result->SetStringField(TEXT("socket_name"), SocketName);
		Result->SetStringField(TEXT("previous_parent_component_name"), PreviousParentName);
		Result->SetStringField(TEXT("previous_socket_name"), PreviousSocketName);
		Result->SetStringField(TEXT("safety_snapshot_root"), GetBlueprintMutationSnapshotRoot());
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would attach '%s' under '%s'%s"),
				*ChildComponentName,
				*ParentComponentName,
				*SocketSuffix));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateBlueprintMutationSnapshot(Blueprint, TEXT("attach_component_to_component_socket"), SnapshotPath, SnapshotError))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_SNAPSHOT_FAILED"),
			SnapshotError);
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "AttachComponentToComponentSocket", "Attach Component To Component Socket"));
	Blueprint->Modify();
	Blueprint->SimpleConstructionScript->Modify();
	ChildNode->Modify();
	ParentNode->Modify();

	if (PreviousParent)
	{
		PreviousParent->Modify();
		PreviousParent->RemoveChildNode(ChildNode, false);
	}

	ChildNode->SetParent(ParentNode);
	ChildNode->AttachToName = SocketName.IsEmpty() ? NAME_None : FName(*SocketName);
	ParentNode->AddChildNode(ChildNode, false);

	Blueprint->SimpleConstructionScript->ValidateSceneRootNodes();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint);

	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("child_component_name"), ChildComponentName);
	PatchDetails->SetStringField(TEXT("parent_component_name"), ParentComponentName);
	PatchDetails->SetStringField(TEXT("socket_name"), SocketName);
	PatchDetails->SetStringField(TEXT("previous_parent_component_name"), PreviousParentName);
	PatchDetails->SetStringField(TEXT("previous_socket_name"), PreviousSocketName);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("attach_component_to_component_socket"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("child_component_name"), ChildComponentName);
	Result->SetStringField(TEXT("parent_component_name"), ParentComponentName);
	Result->SetStringField(TEXT("socket_name"), SocketName);
	Result->SetObjectField(TEXT("patch_report"), BuildBlueprintMutationPatchReport(TEXT("attach_component_to_component_socket"), Blueprint, SnapshotPath, bSaveSucceeded, PatchDetails));
	const FString AttachMessage = SocketName.IsEmpty()
		? FString::Printf(TEXT("Attached component '%s' under '%s'."), *ChildComponentName, *ParentComponentName)
		: FString::Printf(TEXT("Attached component '%s' under '%s' at socket '%s'."), *ChildComponentName, *ParentComponentName, *SocketName);
	Result->SetStringField(TEXT("message"), AttachMessage);
	AppendBlueprintMutationAudit(Result, TEXT("attach_component_to_component_socket"), Blueprint, SnapshotPath, bSaveSucceeded, AttachMessage, PatchDetails);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_DeleteBlueprintNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString NodeGuidString;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("node_guid"), NodeGuidString) || NodeGuidString.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_NODE_GUID"), TEXT("Missing required field: node_guid"));
	}

	FGuid NodeGuid;
	if (!FGuid::Parse(NodeGuidString, NodeGuid))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INVALID_NODE_GUID"), FString::Printf(TEXT("Invalid node_guid format: %s"), *NodeGuidString));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_LOAD_FAILED"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraphNode* Node = FindBlueprintNodeByGuid(Blueprint, NodeGuid);
	if (!Node)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NODE_NOT_FOUND"), FString::Printf(TEXT("Node '%s' was not found in Blueprint '%s'."), *NodeGuidString, *BpPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), bApplyChanges);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("delete_blueprint_node"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("node_guid"), NodeGuidString);
	Result->SetStringField(TEXT("node_title"), Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "DeleteBlueprintNode", "Delete Blueprint Node"));
	Blueprint->Modify();
	if (Node->GetGraph())
	{
		Node->GetGraph()->Modify();
	}
	FBlueprintEditorUtils::RemoveNode(Blueprint, Node, true);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	if (bSaveAsset)
	{
		Result->SetBoolField(TEXT("save_succeeded"), UEditorAssetLibrary::SaveLoadedAsset(Blueprint));
	}

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateBlueprintCallFunctionNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString GraphName;
	FString FunctionOwnerClass;
	FString FunctionName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("graph_name"), GraphName) || GraphName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_GRAPH_NAME"), TEXT("Missing required field: graph_name"));
	}
	if (!Input->TryGetStringField(TEXT("function_owner_class"), FunctionOwnerClass) || FunctionOwnerClass.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_FUNCTION_OWNER_CLASS"), TEXT("Missing required field: function_owner_class"));
	}
	if (!Input->TryGetStringField(TEXT("function_name"), FunctionName) || FunctionName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_FUNCTION_NAME"), TEXT("Missing required field: function_name"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraph* TargetGraph = FindBlueprintGraphByName(Blueprint, GraphName);
	if (!TargetGraph)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_GRAPH_NOT_FOUND"), FString::Printf(TEXT("Graph '%s' was not found in Blueprint '%s'."), *GraphName, *BpPath));
	}

	UClass* OwnerClass = ResolveBlueprintOwnerClass(FunctionOwnerClass);
	if (!OwnerClass)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_FUNCTION_OWNER_CLASS_NOT_FOUND"), FString::Printf(TEXT("Could not resolve function owner class '%s'."), *FunctionOwnerClass));
	}

	UFunction* Function = ResolveBlueprintFunction(OwnerClass, FunctionName);
	if (!Function)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_FUNCTION_NOT_FOUND"), FString::Printf(TEXT("Function '%s' was not found on class '%s'."), *FunctionName, *OwnerClass->GetPathName()));
	}

	int32 NodePosX = 0;
	int32 NodePosY = 0;
	double NumberValue = 0.0;
	if (Input->TryGetNumberField(TEXT("node_pos_x"), NumberValue))
	{
		NodePosX = FMath::RoundToInt(NumberValue);
	}
	if (Input->TryGetNumberField(TEXT("node_pos_y"), NumberValue))
	{
		NodePosY = FMath::RoundToInt(NumberValue);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_blueprint_call_function_node"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("graph_name"), TargetGraph->GetName());
	Result->SetStringField(TEXT("function_owner_class"), OwnerClass->GetPathName());
	Result->SetStringField(TEXT("function_name"), Function->GetName());
	Result->SetNumberField(TEXT("node_pos_x"), NodePosX);
	Result->SetNumberField(TEXT("node_pos_y"), NodePosY);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "CreateBlueprintCallFunctionNode", "Create Blueprint Call Function Node"));
	Blueprint->Modify();
	TargetGraph->Modify();

	UK2Node_CallFunction* CallNode = NewObject<UK2Node_CallFunction>(TargetGraph);
	CallNode->SetFlags(RF_Transactional);
	CallNode->CreateNewGuid();
	CallNode->SetFromFunction(Function);
	CallNode->NodePosX = NodePosX;
	CallNode->NodePosY = NodePosY;
	TargetGraph->AddNode(CallNode, true, false);
	CallNode->PostPlacedNewNode();
	CallNode->AllocateDefaultPins();
	CallNode->ReconstructNode();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetStringField(TEXT("node_guid"), CallNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetStringField(TEXT("node_title"), CallNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
	Result->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(CallNode, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ConnectBlueprintPins(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString FromNodeGuidString;
	FString FromPinName;
	FString ToNodeGuidString;
	FString ToPinName;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("from_node_guid"), FromNodeGuidString) || FromNodeGuidString.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_FROM_NODE_GUID"), TEXT("Missing required field: from_node_guid"));
	}
	if (!Input->TryGetStringField(TEXT("from_pin_name"), FromPinName) || FromPinName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_FROM_PIN_NAME"), TEXT("Missing required field: from_pin_name"));
	}
	if (!Input->TryGetStringField(TEXT("to_node_guid"), ToNodeGuidString) || ToNodeGuidString.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_TO_NODE_GUID"), TEXT("Missing required field: to_node_guid"));
	}
	if (!Input->TryGetStringField(TEXT("to_pin_name"), ToPinName) || ToPinName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_TO_PIN_NAME"), TEXT("Missing required field: to_pin_name"));
	}

	FGuid FromNodeGuid;
	FGuid ToNodeGuid;
	if (!FGuid::Parse(FromNodeGuidString, FromNodeGuid))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INVALID_FROM_NODE_GUID"), FString::Printf(TEXT("Invalid from_node_guid format: %s"), *FromNodeGuidString));
	}
	if (!FGuid::Parse(ToNodeGuidString, ToNodeGuid))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INVALID_TO_NODE_GUID"), FString::Printf(TEXT("Invalid to_node_guid format: %s"), *ToNodeGuidString));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraphNode* FromNode = FindBlueprintNodeByGuid(Blueprint, FromNodeGuid);
	UEdGraphNode* ToNode = FindBlueprintNodeByGuid(Blueprint, ToNodeGuid);
	if (!FromNode)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_FROM_NODE_NOT_FOUND"), FString::Printf(TEXT("Node with guid '%s' was not found."), *FromNodeGuidString));
	}
	if (!ToNode)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_TO_NODE_NOT_FOUND"), FString::Printf(TEXT("Node with guid '%s' was not found."), *ToNodeGuidString));
	}

	UEdGraphPin* FromPin = FindBlueprintNodePinByName(FromNode, FromPinName);
	UEdGraphPin* ToPin = FindBlueprintNodePinByName(ToNode, ToPinName);
	if (!FromPin)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_FROM_PIN_NOT_FOUND"), FString::Printf(TEXT("Pin '%s' was not found on source node '%s'."), *FromPinName, *FromNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString()));
	}
	if (!ToPin)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_TO_PIN_NOT_FOUND"), FString::Printf(TEXT("Pin '%s' was not found on target node '%s'."), *ToPinName, *ToNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString()));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bBreakSourceLinks = ParseOptionalBoolField(Input, TEXT("break_source_links"), false);
	const bool bBreakTargetLinks = ParseOptionalBoolField(Input, TEXT("break_target_links"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("connect_blueprint_pins"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("from_node_guid"), FromNodeGuidString);
	Result->SetStringField(TEXT("from_pin_name"), FromPin->PinName.ToString());
	Result->SetStringField(TEXT("to_node_guid"), ToNodeGuidString);
	Result->SetStringField(TEXT("to_pin_name"), ToPin->PinName.ToString());
	Result->SetBoolField(TEXT("break_source_links"), bBreakSourceLinks);
	Result->SetBoolField(TEXT("break_target_links"), bBreakTargetLinks);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "ConnectBlueprintPins", "Connect Blueprint Pins"));
	Blueprint->Modify();
	FromNode->Modify();
	ToNode->Modify();

	if (bBreakSourceLinks)
	{
		FromPin->BreakAllPinLinks();
	}
	if (bBreakTargetLinks)
	{
		ToPin->BreakAllPinLinks();
	}

	if (!K2Schema->TryCreateConnection(FromPin, ToPin))
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_CONNECT_FAILED"),
			FString::Printf(TEXT("Failed to connect '%s.%s' -> '%s.%s'."), *FromNode->GetNodeTitle(ENodeTitleType::ListView).ToString(), *FromPin->PinName.ToString(), *ToNode->GetNodeTitle(ENodeTitleType::ListView).ToString(), *ToPin->PinName.ToString()));
	}

	FromNode->NodeConnectionListChanged();
	ToNode->NodeConnectionListChanged();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetObjectField(TEXT("from_node_details"), BuildBlueprintNodeDetailsObject(FromNode, true));
	Result->SetObjectField(TEXT("to_node_details"), BuildBlueprintNodeDetailsObject(ToNode, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetBlueprintNodePinDefault(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	FString NodeGuidString;
	FString PinName;
	FString DefaultValue;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}
	if (!Input->TryGetStringField(TEXT("node_guid"), NodeGuidString) || NodeGuidString.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_NODE_GUID"), TEXT("Missing required field: node_guid"));
	}
	if (!Input->TryGetStringField(TEXT("pin_name"), PinName) || PinName.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_PIN_NAME"), TEXT("Missing required field: pin_name"));
	}
	if (!Input->TryGetStringField(TEXT("default_value"), DefaultValue))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_DEFAULT_VALUE"), TEXT("Missing required field: default_value"));
	}

	FGuid NodeGuid;
	if (!FGuid::Parse(NodeGuidString, NodeGuid))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INVALID_NODE_GUID"), FString::Printf(TEXT("Invalid node_guid format: %s"), *NodeGuidString));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	UEdGraphNode* Node = FindBlueprintNodeByGuid(Blueprint, NodeGuid);
	if (!Node)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NODE_NOT_FOUND"), FString::Printf(TEXT("Node with guid '%s' was not found in Blueprint '%s'."), *NodeGuidString, *BpPath));
	}

	UEdGraphPin* TargetPin = nullptr;
	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin && Pin->PinName.ToString().Equals(PinName, ESearchCase::CaseSensitive))
		{
			TargetPin = Pin;
			break;
		}
	}

	if (!TargetPin)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_PIN_NOT_FOUND"), FString::Printf(TEXT("Pin '%s' was not found on node '%s'."), *PinName, *Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString()));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("set_blueprint_node_pin_default"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetStringField(TEXT("node_guid"), NodeGuidString);
	Result->SetStringField(TEXT("pin_name"), TargetPin->PinName.ToString());
	Result->SetStringField(TEXT("pin_type"), PinTypeToDisplayString(TargetPin->PinType));
	Result->SetStringField(TEXT("old_default_value"), TargetPin->DefaultValue);
	Result->SetStringField(TEXT("old_default_object"), TargetPin->DefaultObject ? TargetPin->DefaultObject->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("new_default_value_request"), DefaultValue);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		OutResult = JsonObjToString(Result);
		return true;
	}

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	const FScopedTransaction Transaction(NSLOCTEXT("ForgePilot", "SetBlueprintNodePinDefault", "Set Blueprint Node Pin Default"));
	Blueprint->Modify();
	Node->Modify();

	UObject* LoadedObject = nullptr;
	if (TargetPin->PinType.PinCategory == K2Schema->PC_Object
		|| TargetPin->PinType.PinCategory == K2Schema->PC_Class
		|| TargetPin->PinType.PinCategory == K2Schema->PC_SoftObject
		|| TargetPin->PinType.PinCategory == K2Schema->PC_SoftClass)
	{
		LoadedObject = UEditorAssetLibrary::LoadAsset(DefaultValue);
		if (LoadedObject)
		{
			K2Schema->TrySetDefaultObject(*TargetPin, LoadedObject);
		}
	}

	if (!LoadedObject)
	{
		K2Schema->TrySetDefaultValue(*TargetPin, DefaultValue);
	}

	Node->ReconstructNode();
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result->SetStringField(TEXT("resolved_default_value"), TargetPin->DefaultValue);
	Result->SetStringField(TEXT("resolved_default_object"), TargetPin->DefaultObject ? TargetPin->DefaultObject->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("resolved_default_text"), TargetPin->DefaultTextValue.ToString());
	Result->SetObjectField(TEXT("node_details"), BuildBlueprintNodeDetailsObject(Node, true));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CompileBlueprintWithFeedback(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath) || BpPath.IsEmpty())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_NOT_FOUND"), FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("compile_blueprint_with_feedback"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetBoolField(TEXT("apply_changes"), bApplyChanges);
	Result->SetBoolField(TEXT("save_asset"), bSaveAsset);
	Result->SetStringField(TEXT("status"), BlueprintStatusToString(Blueprint->Status));
	Result->SetBoolField(TEXT("has_generated_class"), Blueprint->GeneratedClass != nullptr);
	Result->SetBoolField(TEXT("is_data_only"), FBlueprintEditorUtils::IsDataOnlyBlueprint(Blueprint));
	Result->SetNumberField(TEXT("new_variable_count"), Blueprint->NewVariables.Num());
	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetBoolField(TEXT("save_succeeded"), false);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run only. Set apply_changes=true to compile the Blueprint."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FKismetEditorUtilities::CompileBlueprint(Blueprint);
	const bool bSaveSucceeded = !bSaveAsset || UEditorAssetLibrary::SaveLoadedAsset(Blueprint, true);

	Result->SetBoolField(TEXT("success"), Blueprint->Status != BS_Error);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("status"), BlueprintStatusToString(Blueprint->Status));
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetStringField(
		TEXT("message"),
		(Blueprint->Status == BS_Error)
			? TEXT("Blueprint compiled with errors. Check editor compiler results for detailed messages.")
			: TEXT("Blueprint compiled successfully."));
	OutResult = JsonObjToString(Result);
	return Blueprint->Status != BS_Error;
}

bool FVFXToolRegistry::Tool_ReviewBlueprintAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString BpPath;
	if (!Input->TryGetStringField(TEXT("blueprint_path"), BpPath))
	{
		return BuildBlueprintToolError(OutResult, TEXT("BP_MISSING_BLUEPRINT_PATH"), TEXT("Missing required field: blueprint_path"));
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(BpPath));
	if (!Blueprint)
	{
		return BuildBlueprintToolError(
			OutResult,
			TEXT("BP_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Blueprint at path: %s"), *BpPath));
	}

	TArray<TSharedPtr<FJsonValue>> Findings;
	auto AddFinding = [&Findings](const FString& Severity, const FString& Finding, const FString& Reason, const FString& SuggestedFix)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("finding"), Finding);
		FindingObj->SetStringField(TEXT("reason"), Reason);
		FindingObj->SetStringField(TEXT("suggested_fix"), SuggestedFix);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	};

	int32 ComponentCount = 0;
	if (Blueprint->SimpleConstructionScript)
	{
		ComponentCount = Blueprint->SimpleConstructionScript->GetAllNodes().Num();
	}

	TArray<UEdGraph*> Graphs;
	GetBlueprintGraphs(Blueprint, Graphs);
	int32 TotalNodeCount = 0;
	int32 EventTickCount = 0;
	int32 CommentNodeCount = 0;
	for (UEdGraph* Graph : Graphs)
	{
		if (!Graph) continue;
		TotalNodeCount += Graph->Nodes.Num();
		for (UEdGraphNode* Node : Graph->Nodes)
		{
			if (!Node) continue;
			const FString NodeTitle = Node->GetNodeTitle(ENodeTitleType::ListView).ToString();
			if (NodeTitle.Contains(TEXT("Event Tick"), ESearchCase::IgnoreCase))
			{
				++EventTickCount;
			}
			if (Node->IsA<UEdGraphNode_Comment>())
			{
				++CommentNodeCount;
			}
		}
	}

	if (!Blueprint->GeneratedClass)
	{
		AddFinding(
			TEXT("error"),
			TEXT("Missing GeneratedClass"),
			TEXT("Blueprint has no generated class; compile state may be invalid."),
			TEXT("Open the Blueprint and recompile; resolve all compiler errors."));
	}

	if (ComponentCount == 0 && Blueprint->ParentClass && Blueprint->ParentClass->IsChildOf(AActor::StaticClass()))
	{
		AddFinding(
			TEXT("warning"),
			TEXT("No scene components"),
			TEXT("Actor Blueprint has zero SCS components, which often indicates incomplete setup."),
			TEXT("Add at least a SceneComponent or required runtime components."));
	}

	if (EventTickCount > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Event Tick usage detected"),
			FString::Printf(TEXT("Found %d Event Tick node(s). Tick-heavy logic can become expensive."), EventTickCount),
			TEXT("Gate Tick with conditions, reduce frequency, or move repeated logic to timers/events."));
	}

	if (TotalNodeCount > 40 && CommentNodeCount == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Large graph lacks comments"),
			FString::Printf(TEXT("Blueprint has %d nodes and no comment nodes."), TotalNodeCount),
			TEXT("Add comment blocks around major logic regions for maintainability."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious issues"),
			TEXT("No high-signal issues detected by automated review rules."),
			TEXT("Optionally run manual gameplay validation and profiling."));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_blueprint_asset"));
	Result->SetStringField(TEXT("blueprint_path"), BpPath);
	Result->SetNumberField(TEXT("component_count"), ComponentCount);
	Result->SetNumberField(TEXT("graph_count"), Graphs.Num());
	Result->SetNumberField(TEXT("node_count"), TotalNodeCount);
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);
	OutResult = JsonObjToString(Result);
	return true;
}
