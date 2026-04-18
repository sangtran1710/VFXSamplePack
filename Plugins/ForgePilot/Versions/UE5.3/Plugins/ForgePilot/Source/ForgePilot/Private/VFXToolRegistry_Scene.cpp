// Copyright 2026, HenryFX. ForgePilot Plugin.
// Phase 1: Scene Tools - Registration + Implementation
#include "VFXToolRegistry.h"
#include "VFXKnowledgeLoader.h"
#include "VFXMutationAudit.h"
#include "VFXVisualContractDataAsset.h"
#include "VFXToolRegistryHelpers.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"
#include "ObjectTools.h"
#include "UnrealClient.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "IAssetViewport.h"
#include "PlayInEditorDataTypes.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "InputKeyEventArgs.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "Misc/ObjectThumbnail.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "Misc/PackageName.h"
#include "UObject/Linker.h"
#include "UObject/ObjectRedirector.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#include "EditorAssetLibrary.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "AssetImportTask.h"
#include "AssetToolsModule.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Engine/GameViewportClient.h"
#include "Interfaces/IPluginManager.h"
#include "Serialization/JsonSerializer.h"
#include "Async/Async.h"

namespace
{
	static FString SerializeJsonObjectCompact(const TSharedPtr<FJsonObject>& Object)
	{
		if (!Object.IsValid())
		{
			return FString();
		}

		FString Output;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
		FJsonSerializer::Serialize(Object.ToSharedRef(), Writer);
		return Output;
	}

	static FString SerializeJsonValueCompact(const TSharedPtr<FJsonValue>& Value)
	{
		if (!Value.IsValid())
		{
			return FString();
		}

		switch (Value->Type)
		{
		case EJson::String:
		{
			FString Output;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
			Writer->WriteValue(Value->AsString());
			Writer->Close();
			return Output;
		}
		case EJson::Number:
			return FString::SanitizeFloat(Value->AsNumber());
		case EJson::Boolean:
			return Value->AsBool() ? TEXT("true") : TEXT("false");
		case EJson::Null:
			return TEXT("null");
		case EJson::Array:
		{
			FString Output;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
			FJsonSerializer::Serialize(Value->AsArray(), Writer);
			return Output;
		}
		case EJson::Object:
			return SerializeJsonObjectCompact(Value->AsObject());
		default:
			return FString();
		}
	}

	static FString BindVisualContractPlaceholders(
		const FString& Value,
		const FString& CurrentSystemPath,
		const FString& HeroParentMaterialPath,
		const FString& CoreParentMaterialPath,
		const FString& CurrentMaterialPath,
		const FString& CurrentHeroMaterialPath,
		const FString& CurrentCoreMaterialPath)
	{
		FString Bound = Value;
		Bound.ReplaceInline(TEXT("{{created_or_duplicated_system_path}}"), *CurrentSystemPath);
		Bound.ReplaceInline(TEXT("{{hero_parent_material_path}}"), *HeroParentMaterialPath);
		Bound.ReplaceInline(TEXT("{{core_parent_material_path}}"), *CoreParentMaterialPath);
		Bound.ReplaceInline(TEXT("{{material_path}}"), *CurrentMaterialPath);
		Bound.ReplaceInline(TEXT("{{hero_material_instance_path}}"), *CurrentHeroMaterialPath);
		Bound.ReplaceInline(TEXT("{{core_material_instance_path}}"), *CurrentCoreMaterialPath);
		return Bound;
	}

	static TSharedPtr<FJsonValue> CloneJsonValueWithBoundPlaceholders(
		const TSharedPtr<FJsonValue>& Value,
		const FString& CurrentSystemPath,
		const FString& HeroParentMaterialPath,
		const FString& CoreParentMaterialPath,
		const FString& CurrentMaterialPath,
		const FString& CurrentHeroMaterialPath,
		const FString& CurrentCoreMaterialPath);

	static TSharedPtr<FJsonObject> CloneJsonObjectWithBoundPlaceholders(
		const TSharedPtr<FJsonObject>& Source,
		const FString& CurrentSystemPath,
		const FString& HeroParentMaterialPath,
		const FString& CoreParentMaterialPath,
		const FString& CurrentMaterialPath,
		const FString& CurrentHeroMaterialPath,
		const FString& CurrentCoreMaterialPath)
	{
		TSharedPtr<FJsonObject> BoundObject = MakeShareable(new FJsonObject());
		if (!Source.IsValid())
		{
			return BoundObject;
		}

		for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : Source->Values)
		{
			if (!Pair.Value.IsValid())
			{
				continue;
			}

			BoundObject->SetField(
				Pair.Key,
				CloneJsonValueWithBoundPlaceholders(
					Pair.Value,
					CurrentSystemPath,
					HeroParentMaterialPath,
					CoreParentMaterialPath,
					CurrentMaterialPath,
					CurrentHeroMaterialPath,
					CurrentCoreMaterialPath));
		}

		return BoundObject;
	}

	static TSharedPtr<FJsonValue> CloneJsonValueWithBoundPlaceholders(
		const TSharedPtr<FJsonValue>& Value,
		const FString& CurrentSystemPath,
		const FString& HeroParentMaterialPath,
		const FString& CoreParentMaterialPath,
		const FString& CurrentMaterialPath,
		const FString& CurrentHeroMaterialPath,
		const FString& CurrentCoreMaterialPath)
	{
		if (!Value.IsValid())
		{
			return MakeShared<FJsonValueNull>();
		}

		switch (Value->Type)
		{
		case EJson::String:
			return MakeShared<FJsonValueString>(BindVisualContractPlaceholders(
				Value->AsString(),
				CurrentSystemPath,
				HeroParentMaterialPath,
				CoreParentMaterialPath,
				CurrentMaterialPath,
				CurrentHeroMaterialPath,
				CurrentCoreMaterialPath));
		case EJson::Number:
			return MakeShared<FJsonValueNumber>(Value->AsNumber());
		case EJson::Boolean:
			return MakeShared<FJsonValueBoolean>(Value->AsBool());
		case EJson::Null:
			return MakeShared<FJsonValueNull>();
		case EJson::Array:
		{
			TArray<TSharedPtr<FJsonValue>> BoundArray;
			for (const TSharedPtr<FJsonValue>& ArrayValue : Value->AsArray())
			{
				BoundArray.Add(CloneJsonValueWithBoundPlaceholders(
					ArrayValue,
					CurrentSystemPath,
					HeroParentMaterialPath,
					CoreParentMaterialPath,
					CurrentMaterialPath,
					CurrentHeroMaterialPath,
					CurrentCoreMaterialPath));
			}
			return MakeShared<FJsonValueArray>(BoundArray);
		}
		case EJson::Object:
			return MakeShared<FJsonValueObject>(CloneJsonObjectWithBoundPlaceholders(
				Value->AsObject(),
				CurrentSystemPath,
				HeroParentMaterialPath,
				CoreParentMaterialPath,
				CurrentMaterialPath,
				CurrentHeroMaterialPath,
				CurrentCoreMaterialPath));
		default:
			return MakeShared<FJsonValueNull>();
		}
	}

	static bool TryExtractVectorFromJsonValue(const TSharedPtr<FJsonValue>& Value, FVector& OutVector)
	{
		if (!Value.IsValid() || Value->Type != EJson::Array)
		{
			return false;
		}

		const TArray<TSharedPtr<FJsonValue>>& ArrayValue = Value->AsArray();
		if (ArrayValue.Num() < 3 ||
			!ArrayValue[0].IsValid() ||
			!ArrayValue[1].IsValid() ||
			!ArrayValue[2].IsValid())
		{
			return false;
		}

		OutVector.X = static_cast<float>(ArrayValue[0]->AsNumber());
		OutVector.Y = static_cast<float>(ArrayValue[1]->AsNumber());
		OutVector.Z = static_cast<float>(ArrayValue[2]->AsNumber());
		return true;
	}

	static void PopulateVisualContractBinding(
		const TSharedPtr<FJsonObject>& BindingObject,
		FVFXVisualContractBinding& OutBinding)
	{
		if (!BindingObject.IsValid())
		{
			return;
		}

		BindingObject->TryGetStringField(TEXT("param_name"), OutBinding.ParameterName);
		BindingObject->TryGetStringField(TEXT("param_type"), OutBinding.ParameterType);
		BindingObject->TryGetBoolField(TEXT("skip_if_empty"), OutBinding.bSkipIfEmpty);

		const TSharedPtr<FJsonValue> ValuePtr = BindingObject->TryGetField(TEXT("value"));
		if (!ValuePtr.IsValid())
		{
			return;
		}

		OutBinding.RawValueJson = SerializeJsonValueCompact(ValuePtr);

		if (ValuePtr->Type == EJson::Number)
		{
			OutBinding.bHasScalarValue = true;
			OutBinding.ScalarValue = static_cast<float>(ValuePtr->AsNumber());
			return;
		}

		if (ValuePtr->Type == EJson::String)
		{
			OutBinding.bHasStringValue = true;
			OutBinding.StringValue = ValuePtr->AsString();

			if (OutBinding.StringValue.StartsWith(TEXT("/Game/")) || OutBinding.StringValue.StartsWith(TEXT("/Script/")))
			{
				OutBinding.bHasAssetValue = true;
				OutBinding.AssetValue = TSoftObjectPtr<UObject>(FSoftObjectPath(OutBinding.StringValue));
			}
			return;
		}

		FVector VectorValue = FVector::ZeroVector;
		if (TryExtractVectorFromJsonValue(ValuePtr, VectorValue))
		{
			OutBinding.bHasVectorValue = true;
			OutBinding.VectorValue = VectorValue;
		}
	}

	static FString MakeVisualContractAssetName(const FString& ArchetypeName)
	{
		FString Sanitized = ArchetypeName.TrimStartAndEnd();
		Sanitized.ReplaceInline(TEXT(" "), TEXT("_"));
		Sanitized.ReplaceInline(TEXT("-"), TEXT("_"));
		return FString::Printf(TEXT("DA_VC_%s"), *Sanitized);
	}

	static void PopulateVisualContractDataAsset(
		UVFXVisualContractDataAsset* DataAsset,
		const FString& ArchetypeName,
		const TSharedPtr<FJsonObject>& VisualContract)
	{
		if (!DataAsset || !VisualContract.IsValid())
		{
			return;
		}

		DataAsset->ArchetypeName = ArchetypeName;
		VisualContract->TryGetStringField(TEXT("strategy"), DataAsset->Strategy);
		VisualContract->TryGetBoolField(TEXT("allow_legacy_stack_fallback"), DataAsset->bAllowLegacyStackFallback);
		VisualContract->TryGetBoolField(TEXT("auto_create_missing_parameters"), DataAsset->bAutoCreateMissingParameters);
		DataAsset->RawVisualContractJson = SerializeJsonObjectCompact(VisualContract);
		DataAsset->OpBindings.Reset();

		const TSharedPtr<FJsonObject>* OpBindingsPtr = nullptr;
		if (!VisualContract->TryGetObjectField(TEXT("op_bindings"), OpBindingsPtr) || !OpBindingsPtr || !(*OpBindingsPtr).IsValid())
		{
			return;
		}

		for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : (*OpBindingsPtr)->Values)
		{
			if (!Pair.Value.IsValid() || Pair.Value->Type != EJson::Object)
			{
				continue;
			}

			const TSharedPtr<FJsonObject> OpObject = Pair.Value->AsObject();
			FVFXVisualContractOpEntry Entry;
			Entry.OpName = Pair.Key;
			OpObject->TryGetStringField(TEXT("layer"), Entry.Layer);
			OpObject->TryGetStringField(TEXT("strategy"), Entry.Strategy);
			if (Entry.Strategy.IsEmpty())
			{
				Entry.Strategy = DataAsset->Strategy;
			}

			if (!OpObject->TryGetBoolField(TEXT("allow_legacy_stack_fallback"), Entry.bAllowLegacyStackFallback))
			{
				Entry.bAllowLegacyStackFallback = DataAsset->bAllowLegacyStackFallback;
			}

			if (!OpObject->TryGetBoolField(TEXT("auto_create_missing_parameters"), Entry.bAutoCreateMissingParameters))
			{
				Entry.bAutoCreateMissingParameters = DataAsset->bAutoCreateMissingParameters;
			}

			const TArray<TSharedPtr<FJsonValue>>* BindingsArray = nullptr;
			if (OpObject->TryGetArrayField(TEXT("bindings"), BindingsArray) && BindingsArray)
			{
				for (const TSharedPtr<FJsonValue>& BindingValue : *BindingsArray)
				{
					if (!BindingValue.IsValid() || BindingValue->Type != EJson::Object)
					{
						continue;
					}

					FVFXVisualContractBinding Binding;
					PopulateVisualContractBinding(BindingValue->AsObject(), Binding);
					if (!Binding.ParameterName.IsEmpty())
					{
						Entry.Bindings.Add(Binding);
					}
				}
			}

			DataAsset->OpBindings.Add(Entry);
		}

		DataAsset->OpBindings.Sort([](const FVFXVisualContractOpEntry& Left, const FVFXVisualContractOpEntry& Right)
		{
			return Left.OpName < Right.OpName;
		});
	}

	static bool TryReadOptionalIntField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, int32& OutValue)
	{
		if (!Input.IsValid())
		{
			return false;
		}

		double NumberValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumberValue))
		{
			OutValue = FMath::RoundToInt(NumberValue);
			return true;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue) && !StringValue.IsEmpty())
		{
			OutValue = FCString::Atoi(*StringValue);
			return true;
		}

		return false;
	}

	struct FV2ArchetypeSceneConfig
	{
		FString Archetype;
		FString EffectType;
		FString HeroEmitter;
		FString SecondaryEmitter;
		FString CoreEmitter;
		FString SparksEmitter;
		FString GlowEmitter;
		FString HeroRebuildOp;
		FString HeroRetuneOp;
		FString HeroMaterialFamilyToken;
		FString CoreMaterialFamilyToken;
	};

	static bool GetV2ArchetypeSceneConfig(const FString& Archetype, FV2ArchetypeSceneConfig& OutConfig)
	{
		if (Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("electric_arc_orb"),
				TEXT("Electricity"),
				TEXT("HeroElectricArc"),
				TEXT("SecondaryElectricFilaments"),
				TEXT("CoreElectricMass"),
				TEXT("SparkBursts"),
				TEXT("BindGlow"),
				TEXT("rebuild_hero_electric_arc_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroElectric"),
				TEXT("CoreMerge")
			};
			return true;
		}

		if (Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("impact_pulse_core"),
				TEXT("Impact"),
				TEXT("HeroImpactBurst"),
				TEXT("SecondaryImpactStreaks"),
				TEXT("CoreImpactFlash"),
				TEXT("DebrisSparks"),
				TEXT("BindGlow"),
				TEXT("rebuild_hero_impact_burst_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroImpact"),
				TEXT("CoreMerge")
			};
			return true;
		}

		if (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("explosion_flash_core"),
				TEXT("Explosion"),
				TEXT("HeroExplosionBurst"),
				TEXT("SecondaryBlastStreaks"),
				TEXT("CoreFlashMass"),
				TEXT("DebrisSparks"),
				TEXT("ShockGlow"),
				TEXT("rebuild_hero_explosion_burst_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroExplosion"),
				TEXT("CoreMerge")
			};
			return true;
		}

		if (Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("hero_fire_orb_sweep"),
				TEXT("Fire"),
				TEXT("HeroFireSweep"),
				TEXT("SecondaryFireFilaments"),
				TEXT("CoreFlameMass"),
				TEXT("Embers"),
				TEXT("BindGlow"),
				TEXT("rebuild_hero_fire_sweep_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroFire"),
				TEXT("CoreMerge")
			};
			return true;
		}

		if (Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("arcane_pull_orb"),
				TEXT("Magic"),
				TEXT("HeroArcaneSweep"),
				TEXT("SecondaryArcaneFilaments"),
				TEXT("CoreArcaneMass"),
				TEXT("SparkMotes"),
				TEXT("BindGlow"),
				TEXT("rebuild_hero_arcane_sweep_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroArcane"),
				TEXT("CoreMerge")
			};
			return true;
		}

		if (Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("suction_core_energy"),
				TEXT("CoreEnergy"),
				TEXT("MainAttractionStreaks"),
				TEXT("SecondaryFilaments"),
				TEXT("CoreTurbulence"),
				TEXT("Sparks"),
				TEXT("ShockGlow"),
				TEXT("rebuild_main_attraction_layer"),
				TEXT("retune_motion_family"),
				TEXT("HeroStreak"),
				TEXT("CoreMerge")
			};
			return true;
		}

		return false;
	}

	static bool BuildSceneToolError(
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

	static FString NormalizeFolderPath(FString FolderPath)
	{
		FolderPath = FolderPath.TrimStartAndEnd();
		while (FolderPath.Len() > 1 && FolderPath.EndsWith(TEXT("/")))
		{
			FolderPath.LeftChopInline(1, false);
		}
		return FolderPath;
	}

	static bool IsGameFolderPath(const FString& FolderPath)
	{
		const FString Normalized = NormalizeFolderPath(FolderPath);
		return Normalized.StartsWith(TEXT("/Game/")) || Normalized == TEXT("/Game");
	}

	static bool ResolveAssetDataFromPath(
		const FString& RequestedAssetPath,
		FAssetData& OutAssetData,
		FString& OutResolvedObjectPath)
	{
		OutAssetData = FAssetData();
		OutResolvedObjectPath = RequestedAssetPath.TrimStartAndEnd();
		if (OutResolvedObjectPath.IsEmpty())
		{
			return false;
		}

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		OutAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(OutResolvedObjectPath));
		if (!OutAssetData.IsValid() && !OutResolvedObjectPath.Contains(TEXT(".")))
		{
			const FString AssetName = FPaths::GetBaseFilename(OutResolvedObjectPath);
			OutResolvedObjectPath = FString::Printf(TEXT("%s.%s"), *OutResolvedObjectPath, *AssetName);
			OutAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(OutResolvedObjectPath));
		}

		if (!OutAssetData.IsValid())
		{
			if (UObject* LoadedAsset = UEditorAssetLibrary::LoadAsset(OutResolvedObjectPath))
			{
				OutAssetData = FAssetData(LoadedAsset);
				OutResolvedObjectPath = LoadedAsset->GetPathName();
			}
		}

		return OutAssetData.IsValid();
	}

	static FString BuildAssetObjectPath(const FString& FolderPath, const FString& AssetName)
	{
		const FString NormalizedFolder = NormalizeFolderPath(FolderPath);
		return FString::Printf(TEXT("%s/%s.%s"), *NormalizedFolder, *AssetName, *AssetName);
	}

	static bool IsGameAssetObjectPath(const FString& ObjectPath)
	{
		return ObjectPath.StartsWith(TEXT("/Game/"));
	}

	static bool GetPackageFilenameForPackageName(const FString& PackageName, FString& OutFilename)
	{
		OutFilename.Reset();
		if (PackageName.IsEmpty())
		{
			return false;
		}

		if (FPackageName::DoesPackageExist(PackageName, &OutFilename))
		{
			return true;
		}

		const FString AssetFilename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		if (FPaths::FileExists(AssetFilename))
		{
			OutFilename = AssetFilename;
			return true;
		}

		const FString MapFilename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetMapPackageExtension());
		if (FPaths::FileExists(MapFilename))
		{
			OutFilename = MapFilename;
			return true;
		}

		return false;
	}

	static FString GetPackageFilenameForAssetData(const FAssetData& AssetData)
	{
		FString Filename;
		GetPackageFilenameForPackageName(AssetData.PackageName.ToString(), Filename);
		return Filename;
	}

	static int64 GetAssetFileSizeBytes(const FAssetData& AssetData)
	{
		const FString Filename = GetPackageFilenameForAssetData(AssetData);
		return Filename.IsEmpty() ? 0 : IFileManager::Get().FileSize(*Filename);
	}

	static FDateTime GetAssetFileTimestamp(const FAssetData& AssetData)
	{
		const FString Filename = GetPackageFilenameForAssetData(AssetData);
		return Filename.IsEmpty() ? FDateTime::MinValue() : IFileManager::Get().GetTimeStamp(*Filename);
	}

	static FString DateTimeToIsoString(const FDateTime& DateTime)
	{
		return DateTime == FDateTime::MinValue()
			? FString()
			: DateTime.ToString(TEXT("%Y-%m-%dT%H:%M:%S"));
	}

	static bool ParseOptionalIntField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, int32 DefaultValue, int32& OutValue)
	{
		OutValue = DefaultValue;
		if (!Input.IsValid() || !Input->HasField(FieldName))
		{
			return true;
		}

		double NumberValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumberValue))
		{
			OutValue = static_cast<int32>(NumberValue);
			return true;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue))
		{
			OutValue = FCString::Atoi(*StringValue);
			return true;
		}

		return false;
	}

	static bool ParseOptionalDoubleField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, double DefaultValue, double& OutValue)
	{
		OutValue = DefaultValue;
		if (!Input.IsValid() || !Input->HasField(FieldName))
		{
			return true;
		}

		if (Input->TryGetNumberField(FieldName, OutValue))
		{
			return true;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue))
		{
			OutValue = FCString::Atod(*StringValue);
			return true;
		}

		return false;
	}

	static TArray<FString> ParseCsvTokens(const FString& Csv)
	{
		TArray<FString> Tokens;
		Csv.ParseIntoArray(Tokens, TEXT(","), true);
		for (FString& Token : Tokens)
		{
			Token = Token.TrimStartAndEnd();
		}
		Tokens.RemoveAll([](const FString& Token) { return Token.IsEmpty(); });
		return Tokens;
	}

	static bool TryGetStringArrayField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, TArray<FString>& OutValues)
	{
		OutValues.Reset();
		if (!Input.IsValid() || !Input->HasTypedField<EJson::Array>(FieldName))
		{
			return false;
		}

		const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
		if (!Input->TryGetArrayField(FieldName, Values) || Values == nullptr)
		{
			return false;
		}

		for (const TSharedPtr<FJsonValue>& Value : *Values)
		{
			if (Value.IsValid())
			{
				OutValues.Add(Value->AsString().TrimStartAndEnd());
			}
		}

		OutValues.RemoveAll([](const FString& Value) { return Value.IsEmpty(); });
		return true;
	}

	static bool AssetMatchesClassFilters(const FAssetData& AssetData, const TArray<FString>& ClassFilters)
	{
		if (ClassFilters.Num() == 0)
		{
			return true;
		}

		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();
		for (const FString& Filter : ClassFilters)
		{
			if (ClassName.Equals(Filter, ESearchCase::IgnoreCase))
			{
				return true;
			}
		}

		return false;
	}

	static TSharedPtr<FJsonObject> BuildStringCountMapJson(const TMap<FString, int32>& Counts, int32 MaxEntries = 50)
	{
		TArray<TPair<FString, int32>> Pairs;
		for (const TPair<FString, int32>& Pair : Counts)
		{
			Pairs.Add(Pair);
		}

		Pairs.Sort([](const TPair<FString, int32>& A, const TPair<FString, int32>& B)
		{
			if (A.Value == B.Value)
			{
				return A.Key < B.Key;
			}
			return A.Value > B.Value;
		});

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		int32 Added = 0;
		for (const TPair<FString, int32>& Pair : Pairs)
		{
			Result->SetNumberField(Pair.Key, Pair.Value);
			Added++;
			if (Added >= MaxEntries)
			{
				break;
			}
		}

		return Result;
	}

	static FString GetAssetPrefixToken(const FString& AssetName)
	{
		FString Prefix;
		FString Remainder;
		if (AssetName.Split(TEXT("_"), &Prefix, &Remainder) && !Prefix.IsEmpty())
		{
			return Prefix;
		}

		return TEXT("(none)");
	}

	static FString GetAssetStemToken(const FString& AssetName)
	{
		TArray<FString> Parts;
		AssetName.ParseIntoArray(Parts, TEXT("_"), true);
		if (Parts.Num() >= 2)
		{
			return Parts[0] + TEXT("_") + Parts[1];
		}
		if (Parts.Num() == 1)
		{
			return Parts[0];
		}

		return AssetName;
	}

	static FString GetRelativeFolderUnderRoot(const FString& RootFolder, const FString& PackagePath)
	{
		const FString NormalizedRoot = NormalizeFolderPath(RootFolder);
		if (PackagePath.Equals(NormalizedRoot, ESearchCase::IgnoreCase))
		{
			return TEXT("(root)");
		}

		if (PackagePath.StartsWith(NormalizedRoot + TEXT("/")))
		{
			FString Relative = PackagePath.RightChop(NormalizedRoot.Len() + 1);
			FString FirstSegment;
			FString Remainder;
			if (Relative.Split(TEXT("/"), &FirstSegment, &Remainder))
			{
				return FirstSegment;
			}
			return Relative.IsEmpty() ? TEXT("(root)") : Relative;
		}

		return PackagePath;
	}

	static bool IsVFXTemplateCandidate(const FAssetData& AssetData, FString& OutReason)
	{
		const FString Name = AssetData.AssetName.ToString();
		const FString NameLower = Name.ToLower();
		const FString PathLower = AssetData.PackagePath.ToString().ToLower();
		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();

		if (ClassName == TEXT("NiagaraEmitter"))
		{
			OutReason = TEXT("niagara_emitter_template_asset");
			return true;
		}
		if (PathLower.Contains(TEXT("/template")) || PathLower.Contains(TEXT("/templates")))
		{
			OutReason = TEXT("template_folder");
			return true;
		}
		if (NameLower.Contains(TEXT("template")) || NameLower.Contains(TEXT("_tpl")) || NameLower.StartsWith(TEXT("tpl_")))
		{
			OutReason = TEXT("template_name");
			return true;
		}
		if (NameLower.Contains(TEXT("master")) || NameLower.Contains(TEXT("_base")) || NameLower.EndsWith(TEXT("_base")))
		{
			OutReason = TEXT("base_or_master_name");
			return true;
		}

		OutReason.Reset();
		return false;
	}

	static bool GetScopedAssets(const FString& FolderPath, bool bRecursive, TArray<FAssetData>& OutAssets)
	{
		OutAssets.Reset();
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		return AssetRegistryModule.Get().GetAssetsByPath(FName(*FolderPath), OutAssets, bRecursive);
	}

	static TSharedPtr<FJsonObject> MakeAssetSummaryJson(const FAssetData& AssetData, bool bIncludeFileInfo = false)
	{
		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("name"), AssetData.AssetName.ToString());
		AssetObj->SetStringField(TEXT("path"), AssetData.GetObjectPathString());
		AssetObj->SetStringField(TEXT("class"), AssetData.AssetClassPath.GetAssetName().ToString());
		AssetObj->SetStringField(TEXT("package_name"), AssetData.PackageName.ToString());
		AssetObj->SetStringField(TEXT("package_path"), AssetData.PackagePath.ToString());

		if (bIncludeFileInfo)
		{
			const FString Filename = GetPackageFilenameForAssetData(AssetData);
			const int64 SizeBytes = Filename.IsEmpty() ? 0 : IFileManager::Get().FileSize(*Filename);
			const FDateTime Timestamp = Filename.IsEmpty() ? FDateTime::MinValue() : IFileManager::Get().GetTimeStamp(*Filename);
			AssetObj->SetStringField(TEXT("filename"), Filename);
			AssetObj->SetNumberField(TEXT("size_bytes"), static_cast<double>(FMath::Max<int64>(SizeBytes, 0)));
			AssetObj->SetNumberField(TEXT("size_mb"), FMath::RoundToDouble((static_cast<double>(FMath::Max<int64>(SizeBytes, 0)) / (1024.0 * 1024.0)) * 100.0) / 100.0);
			AssetObj->SetStringField(TEXT("modified_time"), DateTimeToIsoString(Timestamp));
		}

		return AssetObj;
	}

	static bool ParseDateTimeInput(const FString& InputString, FDateTime& OutDateTime)
	{
		FString Trimmed = InputString.TrimStartAndEnd();
		if (Trimmed.IsEmpty())
		{
			return false;
		}

		if (FDateTime::ParseIso8601(*Trimmed, OutDateTime))
		{
			return true;
		}

		if (!Trimmed.Contains(TEXT("T")) && Trimmed.Len() == 10)
		{
			const FString DateOnlyIso = Trimmed + TEXT("T00:00:00");
			if (FDateTime::ParseIso8601(*DateOnlyIso, OutDateTime))
			{
				return true;
			}
		}

		return FDateTime::Parse(Trimmed, OutDateTime);
	}

	static FString GetPackageFilenameForObjectPath(const FString& ObjectPath)
	{
		if (!ObjectPath.StartsWith(TEXT("/")) || !ObjectPath.Contains(TEXT(".")))
		{
			return FString();
		}

		const FString PackageName = FPackageName::ObjectPathToPackageName(ObjectPath);
		if (PackageName.IsEmpty())
		{
			return FString();
		}

		return FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	}

	static void SyncContentBrowserToAsset(const FAssetData& AssetData)
	{
		if (!AssetData.IsValid())
		{
			return;
		}

		FContentBrowserModule& ContentBrowserModule =
			FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray<FAssetData> AssetsToSync;
		AssetsToSync.Add(AssetData);
		ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync);
	}

	static void SyncContentBrowserToFolder(const FString& FolderPath)
	{
		FContentBrowserModule& ContentBrowserModule =
			FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray<FString> FoldersToSync;
		FoldersToSync.Add(NormalizeFolderPath(FolderPath));
		ContentBrowserModule.Get().SyncBrowserToFolders(FoldersToSync);
	}

	static void AppendSceneMutationAudit(
		const TSharedPtr<FJsonObject>& Result,
		const FString& ToolName,
		const FString& AssetPath,
		const FString& AssetClass,
		const FString& MutationAction,
		const FString& Message,
		const TSharedPtr<FJsonObject>& Details = nullptr)
	{
		if (!Result.IsValid())
		{
			return;
		}

		FString AuditEntryPath;
		const bool bRecorded = FVFXMutationAudit::RecordAssetMutation(
			ToolName,
			AssetPath,
			AssetClass,
			MutationAction,
			TEXT(""),
			GetPackageFilenameForObjectPath(AssetPath),
			true,
			Message,
			AuditEntryPath,
			Details);

		Result->SetBoolField(TEXT("audit_recorded"), bRecorded);
		if (bRecorded)
		{
			Result->SetStringField(TEXT("audit_entry_path"), AuditEntryPath);
		}
	}

	static FString ClassifyAssetGroup(const FString& AssetClass)
	{
		if (AssetClass.StartsWith(TEXT("Texture")))
		{
			return TEXT("textures");
		}
		if (AssetClass.StartsWith(TEXT("MaterialFunction")))
		{
			return TEXT("material_functions");
		}
		if (AssetClass.StartsWith(TEXT("MaterialInstance")))
		{
			return TEXT("material_instances");
		}
		if (AssetClass == TEXT("Material"))
		{
			return TEXT("materials");
		}
		if (AssetClass == TEXT("Blueprint"))
		{
			return TEXT("blueprints");
		}
		if (AssetClass == TEXT("AnimBlueprint"))
		{
			return TEXT("animation_blueprints");
		}
		if (AssetClass.StartsWith(TEXT("Niagara")))
		{
			return TEXT("niagara");
		}
		if (AssetClass == TEXT("StaticMesh") || AssetClass == TEXT("SkeletalMesh"))
		{
			return TEXT("meshes");
		}
		if (AssetClass == TEXT("MaterialParameterCollection"))
		{
			return TEXT("parameter_collections");
		}
		if (AssetClass == TEXT("SubsurfaceProfile"))
		{
			return TEXT("rendering_profiles");
		}
		return TEXT("other");
	}

	static TSet<FString> ParseFocusGroups(const TSharedPtr<FJsonObject>& Input)
	{
		TSet<FString> Groups;
		if (!Input.IsValid())
		{
			return Groups;
		}

		FString GroupsString;
		if (!Input->TryGetStringField(TEXT("focus_groups"), GroupsString) || GroupsString.IsEmpty())
		{
			return Groups;
		}

		TArray<FString> Parts;
		GroupsString.ParseIntoArray(Parts, TEXT(","), true);
		for (FString& Part : Parts)
		{
			Part = Part.TrimStartAndEnd().ToLower();
			if (!Part.IsEmpty())
			{
				Groups.Add(Part);
			}
		}

		return Groups;
	}

	static TSet<FString> ParseTokenSet(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName)
	{
		TSet<FString> Tokens;
		if (!Input.IsValid())
		{
			return Tokens;
		}

		FString Raw;
		if (!Input->TryGetStringField(FieldName, Raw) || Raw.IsEmpty())
		{
			return Tokens;
		}

		TArray<FString> Parts;
		Raw.ParseIntoArray(Parts, TEXT(","), true);
		for (FString& Part : Parts)
		{
			Part = Part.TrimStartAndEnd().ToLower();
			if (!Part.IsEmpty())
			{
				Tokens.Add(Part);
			}
		}

		return Tokens;
	}

	static bool ParseToolJsonResult(const FString& Json, TSharedPtr<FJsonObject>& OutObject)
	{
		OutObject.Reset();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
		return FJsonSerializer::Deserialize(Reader, OutObject) && OutObject.IsValid();
	}

	static bool TryGetBoolFromValue(const TSharedPtr<FJsonValue>& Value, bool& OutValue)
	{
		if (!Value.IsValid())
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
			const FString Text = Value->AsString().TrimStartAndEnd();
			if (Text.Equals(TEXT("true"), ESearchCase::IgnoreCase) || Text == TEXT("1"))
			{
				OutValue = true;
				return true;
			}
			if (Text.Equals(TEXT("false"), ESearchCase::IgnoreCase) || Text == TEXT("0"))
			{
				OutValue = false;
				return true;
			}
		}

		return false;
	}

	static void AddFailureLabel(
		TMap<FString, TSharedPtr<FJsonObject>>& FindingsByLabel,
		const FString& Label,
		const FString& Reason,
		const FString& Severity,
		const bool bHardGate,
		const FString& RecommendedOp)
	{
		if (FindingsByLabel.Contains(Label))
		{
			return;
		}

		TSharedPtr<FJsonObject> Finding = MakeShareable(new FJsonObject());
		Finding->SetStringField(TEXT("label"), Label);
		Finding->SetStringField(TEXT("reason"), Reason);
		Finding->SetStringField(TEXT("severity"), Severity);
		Finding->SetBoolField(TEXT("hard_gate"), bHardGate);
		if (!RecommendedOp.IsEmpty())
		{
			Finding->SetArrayField(TEXT("recommended_ops"), {
				MakeShared<FJsonValueString>(RecommendedOp)
			});
		}
		FindingsByLabel.Add(Label, Finding);
	}

	static TSharedPtr<IAssetViewport> GetFirstActiveAssetViewport()
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		return LevelEditorModule.GetFirstActiveViewport();
	}

	static FString NormalizeKeyToken(FString Value)
	{
		Value = Value.TrimStartAndEnd().ToLower();
		Value.ReplaceInline(TEXT(" "), TEXT(""));
		Value.ReplaceInline(TEXT("_"), TEXT(""));
		Value.ReplaceInline(TEXT("-"), TEXT(""));
		return Value;
	}

	static bool TryResolveKeyByName(const FString& RequestedKey, FKey& OutKey, FString& OutResolvedName)
	{
		if (RequestedKey.IsEmpty())
		{
			return false;
		}

		const FString Wanted = NormalizeKeyToken(RequestedKey);
		TArray<FKey> AllKeys;
		EKeys::GetAllKeys(AllKeys);

		for (const FKey& Candidate : AllKeys)
		{
			if (!Candidate.IsValid())
			{
				continue;
			}

			const FString NameToken = NormalizeKeyToken(Candidate.GetFName().ToString());
			const FString DisplayToken = NormalizeKeyToken(Candidate.GetDisplayName().ToString());
			if (NameToken == Wanted || DisplayToken == Wanted)
			{
				OutKey = Candidate;
				OutResolvedName = Candidate.GetFName().ToString();
				return true;
			}
		}

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

	static FString InputEventToString(const EInputEvent EventType)
	{
		switch (EventType)
		{
		case IE_Pressed:
			return TEXT("pressed");
		case IE_Released:
			return TEXT("released");
		case IE_Repeat:
			return TEXT("repeat");
		case IE_DoubleClick:
			return TEXT("double_click");
		case IE_Axis:
			return TEXT("axis");
		default:
			return TEXT("unknown");
		}
	}

	static TSharedPtr<FJsonObject> BuildPIEStateObject()
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());

		const bool bHasEditor = GUnrealEd != nullptr;
		const bool bIsPlayingInEditor = bHasEditor && GUnrealEd->IsPlayingSessionInEditor();
		const bool bHasPlayWorld = GEditor != nullptr && GEditor->PlayWorld != nullptr;

		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("editor_available"), bHasEditor);
		Result->SetBoolField(TEXT("is_playing_in_editor"), bIsPlayingInEditor);
		Result->SetBoolField(TEXT("has_play_world"), bHasPlayWorld);

		if (bHasPlayWorld)
		{
			Result->SetStringField(TEXT("play_world_name"), GEditor->PlayWorld->GetName());
		}

		TSharedPtr<IAssetViewport> ActiveViewport = GetFirstActiveAssetViewport();
		const bool bHasActiveViewport = ActiveViewport.IsValid();
		Result->SetBoolField(TEXT("has_active_viewport"), bHasActiveViewport);
		if (bHasActiveViewport)
		{
			Result->SetBoolField(TEXT("active_viewport_has_pie"), ActiveViewport->HasPlayInEditorViewport());
			if (FViewport* Viewport = ActiveViewport->GetActiveViewport())
			{
				const FIntPoint Size = Viewport->GetSizeXY();
				TSharedPtr<FJsonObject> SizeObject = MakeShareable(new FJsonObject());
				SizeObject->SetNumberField(TEXT("x"), Size.X);
				SizeObject->SetNumberField(TEXT("y"), Size.Y);
				Result->SetObjectField(TEXT("viewport_size"), SizeObject);
			}
		}

		Result->SetBoolField(TEXT("can_send_input"), bIsPlayingInEditor && bHasActiveViewport);
		return Result;
	}

	static FLevelEditorViewportClient* GetBestLevelViewportClientForSceneTools()
	{
		auto ScoreViewportClient = [](FLevelEditorViewportClient* ViewportClient) -> int32
		{
			if (!ViewportClient || ViewportClient->Viewport == nullptr)
			{
				return MIN_int32;
			}

			int32 Score = 0;
			if (ViewportClient == GCurrentLevelEditingViewportClient)
			{
				Score += 1000;
			}
			if (ViewportClient == GLastKeyLevelEditingViewportClient)
			{
				Score += 500;
			}
			if (ViewportClient->IsPerspective())
			{
				Score += 200;
			}
			if (ViewportClient->IsRealtime())
			{
				Score += 50;
			}

			return Score;
		};

		FLevelEditorViewportClient* BestClient = nullptr;
		int32 BestScore = MIN_int32;

		if (GCurrentLevelEditingViewportClient)
		{
			BestClient = GCurrentLevelEditingViewportClient;
			BestScore = ScoreViewportClient(BestClient);
		}

		if (GLastKeyLevelEditingViewportClient)
		{
			const int32 LastKeyScore = ScoreViewportClient(GLastKeyLevelEditingViewportClient);
			if (LastKeyScore > BestScore)
			{
				BestClient = GLastKeyLevelEditingViewportClient;
				BestScore = LastKeyScore;
			}
		}

		if (GEditor)
		{
			for (FLevelEditorViewportClient* ViewportClient : GEditor->GetLevelViewportClients())
			{
				const int32 Score = ScoreViewportClient(ViewportClient);
				if (Score > BestScore)
				{
					BestClient = ViewportClient;
					BestScore = Score;
				}
			}
		}

		return BestClient;
	}

	static TSharedPtr<FJsonObject> BuildViewportCaptureStateObject(
		const bool bEnsurePerspective,
		const bool bEnsureRealtime,
		const int32 WaitMs,
		FLevelEditorViewportClient*& OutViewportClient,
		FViewport*& OutViewport)
	{
		OutViewportClient = nullptr;
		OutViewport = nullptr;

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("ensure_perspective_requested"), bEnsurePerspective);
		Result->SetBoolField(TEXT("ensure_realtime_requested"), bEnsureRealtime);
		Result->SetNumberField(TEXT("wait_ms"), WaitMs);

		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		TSharedPtr<SLevelViewport> ActiveViewport = LevelEditorModule.GetFirstActiveLevelViewport();
		Result->SetBoolField(TEXT("has_active_level_viewport"), ActiveViewport.IsValid());
		Result->SetBoolField(TEXT("using_fallback_level_viewport"), false);
		if (ActiveViewport.IsValid())
		{
			OutViewport = ActiveViewport->GetActiveViewport();
			OutViewportClient = &ActiveViewport->GetLevelViewportClient();
		}
		else
		{
			OutViewportClient = GetBestLevelViewportClientForSceneTools();
			if (OutViewportClient)
			{
				OutViewport = OutViewportClient->Viewport;
				Result->SetBoolField(TEXT("using_fallback_level_viewport"), true);
			}
		}

		if (!OutViewportClient || !OutViewport)
		{
			Result->SetBoolField(TEXT("ready_for_visual_capture"), false);
			Result->SetStringField(TEXT("readiness_reason"), TEXT("No usable level viewport is available."));
			return Result;
		}
		Result->SetBoolField(TEXT("has_viewport"), true);

		if (bEnsurePerspective && !OutViewportClient->IsPerspective())
		{
			OutViewportClient->SetViewportType(LVT_Perspective);
		}
		if (bEnsureRealtime)
		{
			OutViewportClient->SetRealtime(true);
		}

		OutViewportClient->Invalidate();
		if (GEditor)
		{
			GEditor->RedrawLevelEditingViewports(true);
		}
		if (WaitMs > 0)
		{
			FPlatformProcess::Sleep(static_cast<float>(WaitMs) / 1000.0f);
		}
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().Tick();
		}
		OutViewport->Draw(true);
		FlushRenderingCommands();

		const FIntPoint ViewportSize = OutViewport->GetSizeXY();
		TSharedPtr<FJsonObject> SizeObject = MakeShareable(new FJsonObject());
		SizeObject->SetNumberField(TEXT("x"), ViewportSize.X);
		SizeObject->SetNumberField(TEXT("y"), ViewportSize.Y);
		Result->SetObjectField(TEXT("viewport_size"), SizeObject);
		Result->SetBoolField(TEXT("perspective_viewport"), OutViewportClient->IsPerspective());
		Result->SetBoolField(TEXT("realtime_viewport"), OutViewportClient->IsRealtime());

		const bool bReady =
			ViewportSize.X > 0 &&
			ViewportSize.Y > 0 &&
			OutViewportClient->IsPerspective() &&
			(!bEnsureRealtime || OutViewportClient->IsRealtime());
		Result->SetBoolField(TEXT("ready_for_visual_capture"), bReady);
		if (!bReady)
		{
			if (ViewportSize.X <= 0 || ViewportSize.Y <= 0)
			{
				Result->SetStringField(TEXT("readiness_reason"), TEXT("Viewport size is zero."));
			}
			else if (!OutViewportClient->IsPerspective())
			{
				Result->SetStringField(TEXT("readiness_reason"), TEXT("Viewport is not in perspective mode."));
			}
			else if (bEnsureRealtime && !OutViewportClient->IsRealtime())
			{
				Result->SetStringField(TEXT("readiness_reason"), TEXT("Viewport is not in realtime after requesting realtime mode."));
			}
		}
		else
		{
			Result->SetStringField(TEXT("readiness_reason"), TEXT("Viewport is ready for visual capture."));
		}

		return Result;
	}

	static FString GetForgePilotPluginBaseDir()
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ForgePilot"));
		return Plugin.IsValid() ? Plugin->GetBaseDir() : FString();
	}

	static FString GetForgePilotProjectRootDir()
	{
		const FString PluginDir = GetForgePilotPluginBaseDir();
		return PluginDir.IsEmpty()
			? FString()
			: FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginDir, TEXT(".."), TEXT("..")));
	}

	static FString GetForgePilotScenarioConfigPath()
	{
		return FPaths::Combine(GetForgePilotPluginBaseDir(), TEXT("Tools"), TEXT("Validation"), TEXT("pie_scenarios.json"));
	}

	static FString SanitizeScenarioToken(FString Value)
	{
		Value = Value.TrimStartAndEnd();
		for (TCHAR& Character : Value)
		{
			if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
			{
				Character = TEXT('_');
			}
		}

		return Value.IsEmpty() ? TEXT("scenario") : Value;
	}

	static FString GetDefaultProbeOutputDir(const FString& ScenarioName)
	{
		FString UserProfileDir = FPlatformMisc::GetEnvironmentVariable(TEXT("USERPROFILE"));
		if (UserProfileDir.IsEmpty())
		{
			UserProfileDir = FPlatformProcess::UserHomeDir();
		}

		const FString OutputRoot = FPaths::Combine(UserProfileDir, TEXT("Downloads"), TEXT("ForgePilotRuntimeProbes"));
		const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
		return FPaths::Combine(OutputRoot, FString::Printf(TEXT("%s_%s"), *SanitizeScenarioToken(ScenarioName), *Timestamp));
	}

	static bool LoadPIEProbeScenarioDefinition(const FString& ScenarioName, TSharedPtr<FJsonObject>& OutScenario, FString& OutError)
	{
		const FString ConfigPath = GetForgePilotScenarioConfigPath();
		FString Contents;
		if (!FFileHelper::LoadFileToString(Contents, *ConfigPath))
		{
			OutError = FString::Printf(TEXT("Failed to read scenario config: %s"), *ConfigPath);
			return false;
		}

		TSharedPtr<FJsonObject> RootObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Contents);
		if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
		{
			OutError = FString::Printf(TEXT("Failed to parse scenario config JSON: %s"), *ConfigPath);
			return false;
		}

		const TSharedPtr<FJsonObject>* FoundScenario = nullptr;
		if (!RootObject->TryGetObjectField(ScenarioName, FoundScenario) || FoundScenario == nullptr || !FoundScenario->IsValid())
		{
			OutError = FString::Printf(TEXT("Scenario '%s' was not found in %s"), *ScenarioName, *ConfigPath);
			return false;
		}

		OutScenario = *FoundScenario;

		return true;
	}

	static bool SaveCapturePayloadToFile(const FString& Base64Payload, const FString& OutputPath, FString& OutError)
	{
		TArray<uint8> BinaryData;
		if (!FBase64::Decode(Base64Payload, BinaryData))
		{
			OutError = TEXT("Failed to decode base64 capture payload.");
			return false;
		}

		IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutputPath), true);
		if (!FFileHelper::SaveArrayToFile(BinaryData, *OutputPath))
		{
			OutError = FString::Printf(TEXT("Failed to save capture file: %s"), *OutputPath);
			return false;
		}

		return true;
	}

	static TSharedPtr<FJsonObject> BuildLatestPIELogSummary()
	{
		TSharedPtr<FJsonObject> Summary = MakeShareable(new FJsonObject());
		Summary->SetBoolField(TEXT("success"), true);

		const FString LogsDir = FPaths::Combine(GetForgePilotProjectRootDir(), TEXT("Saved"), TEXT("Logs"));
		if (LogsDir.IsEmpty() || !IFileManager::Get().DirectoryExists(*LogsDir))
		{
			Summary->SetBoolField(TEXT("available"), false);
			Summary->SetStringField(TEXT("message"), TEXT("Saved/Logs directory was not found."));
			return Summary;
		}

		TArray<FString> LogFiles;
		IFileManager::Get().FindFiles(LogFiles, *FPaths::Combine(LogsDir, TEXT("*.log")), true, false);
		if (LogFiles.Num() == 0)
		{
			Summary->SetBoolField(TEXT("available"), false);
			Summary->SetStringField(TEXT("message"), TEXT("No log files found in Saved/Logs."));
			return Summary;
		}

		LogFiles.Sort([&LogsDir](const FString& Left, const FString& Right)
		{
			return IFileManager::Get().GetTimeStamp(*FPaths::Combine(LogsDir, Left))
				> IFileManager::Get().GetTimeStamp(*FPaths::Combine(LogsDir, Right));
		});

		FString LatestLogPath;
		FString LogContents;
		for (const FString& Candidate : LogFiles)
		{
			const FString CandidatePath = FPaths::Combine(LogsDir, Candidate);
			if (FFileHelper::LoadFileToString(LogContents, *CandidatePath))
			{
				LatestLogPath = CandidatePath;
				break;
			}
		}

		if (LatestLogPath.IsEmpty())
		{
			Summary->SetBoolField(TEXT("available"), false);
			Summary->SetStringField(TEXT("message"), TEXT("Failed to read any log file in Saved/Logs."));
			return Summary;
		}

		TArray<FString> Lines;
		LogContents.ParseIntoArrayLines(Lines);

		TArray<TSharedPtr<FJsonValue>> SampleErrors;
		int32 ErrorCount = 0;
		for (const FString& Line : Lines)
		{
			if (Line.Contains(TEXT("Blueprint Runtime Error")) || Line.Contains(TEXT("Error:")) || Line.Contains(TEXT("ensureMsgf")))
			{
				++ErrorCount;
				if (SampleErrors.Num() < 12)
				{
					SampleErrors.Add(MakeShareable(new FJsonValueString(Line.TrimStartAndEnd())));
				}
			}
		}

		Summary->SetBoolField(TEXT("available"), true);
		Summary->SetStringField(TEXT("latest_log_path"), LatestLogPath);
		Summary->SetNumberField(TEXT("error_count"), ErrorCount);
		Summary->SetArrayField(TEXT("sample_errors"), SampleErrors);
		Summary->SetStringField(TEXT("message"), ErrorCount == 0
			? TEXT("No matching runtime errors found in latest log.")
			: TEXT("Runtime errors were found in the latest log."));
		return Summary;
	}

	static bool SendPIEKeyEventToActiveViewport(const FKey& Key, const EInputEvent EventType, const float Amount, FString& OutError)
	{
		TSharedPtr<IAssetViewport> ActiveViewport = GetFirstActiveAssetViewport();
		if (!ActiveViewport.IsValid())
		{
			OutError = TEXT("No active asset viewport found.");
			return false;
		}

		FViewport* Viewport = ActiveViewport->GetActiveViewport();
		if (!Viewport)
		{
			OutError = TEXT("Active viewport is invalid.");
			return false;
		}

		if (TSharedPtr<SViewport> ViewportWidget = ActiveViewport->GetViewportWidget().Pin())
		{
			FSlateApplication::Get().SetKeyboardFocus(ViewportWidget, EFocusCause::SetDirectly);
		}
		else
		{
			FSlateApplication::Get().SetKeyboardFocus(ActiveViewport->AsWidget(), EFocusCause::SetDirectly);
		}

		FInputKeyEventArgs InputArgs(
			Viewport,
			0,
			Key,
			EventType,
			Amount,
			false);
		bool bHandled = ActiveViewport->GetAssetViewportClient().InputKey(InputArgs);

		if (!bHandled && GEditor != nullptr && GEditor->PlayWorld != nullptr)
		{
			if (UGameViewportClient* GameViewport = GEditor->PlayWorld->GetGameViewport())
			{
				bHandled = GameViewport->InputKey(InputArgs);
			}
		}

		if (!bHandled)
		{
			OutError = FString::Printf(
				TEXT("Viewport did not report handling key '%s' (%s)."),
				*Key.GetFName().ToString(),
				*InputEventToString(EventType));
		}

		return bHandled;
	}

	static TSharedPtr<FJsonObject> BuildAssetRelationshipEntry(
		IAssetRegistry& AssetRegistry,
		const FName& PackageName,
		int32 Depth = 1,
		const FString* SourcePackageName = nullptr)
	{
		TSharedPtr<FJsonObject> Entry = MakeShareable(new FJsonObject());
		Entry->SetStringField(TEXT("package_name"), PackageName.ToString());
		Entry->SetNumberField(TEXT("depth"), Depth);
		if (SourcePackageName != nullptr)
		{
			Entry->SetStringField(TEXT("source_package_name"), *SourcePackageName);
		}

		TArray<FAssetData> PackageAssets;
		AssetRegistry.GetAssetsByPackageName(PackageName, PackageAssets, false);
		if (PackageAssets.Num() > 0)
		{
			const FAssetData& AssetData = PackageAssets[0];
			const FString AssetClass = AssetData.AssetClassPath.GetAssetName().ToString();
			Entry->SetStringField(TEXT("asset_name"), AssetData.AssetName.ToString());
			Entry->SetStringField(TEXT("asset_class"), AssetClass);
			Entry->SetStringField(TEXT("asset_path"), AssetData.GetObjectPathString());
			Entry->SetStringField(TEXT("group"), ClassifyAssetGroup(AssetClass));
		}
		else
		{
			Entry->SetStringField(TEXT("asset_name"), PackageName.ToString());
			Entry->SetStringField(TEXT("asset_class"), TEXT("Unknown"));
			Entry->SetStringField(TEXT("asset_path"), PackageName.ToString());
			Entry->SetStringField(TEXT("group"), TEXT("other"));
		}

		return Entry;
	}

	static bool ShouldKeepRelationshipEntry(
		const TSharedPtr<FJsonObject>& Entry,
		const TSet<FString>& FocusGroups,
		bool bIncludeScriptPackages)
	{
		if (!Entry.IsValid())
		{
			return false;
		}

		FString PackageName;
		Entry->TryGetStringField(TEXT("package_name"), PackageName);
		if (!bIncludeScriptPackages && PackageName.StartsWith(TEXT("/Script/")))
		{
			return false;
		}

		if (FocusGroups.Num() == 0)
		{
			return true;
		}

		FString GroupName = TEXT("other");
		Entry->TryGetStringField(TEXT("group"), GroupName);
		return FocusGroups.Contains(GroupName.ToLower());
	}

	static TArray<TSharedPtr<FJsonValue>> FilterRelationshipEntries(
		const TArray<TSharedPtr<FJsonValue>>& Entries,
		const TSet<FString>& FocusGroups,
		bool bIncludeScriptPackages)
	{
		TArray<TSharedPtr<FJsonValue>> Filtered;
		for (const TSharedPtr<FJsonValue>& Value : Entries)
		{
			if (!Value.IsValid() || Value->Type != EJson::Object)
			{
				continue;
			}

			const TSharedPtr<FJsonObject> Entry = Value->AsObject();
			if (ShouldKeepRelationshipEntry(Entry, FocusGroups, bIncludeScriptPackages))
			{
				Filtered.Add(Value);
			}
		}
		return Filtered;
	}

	static TSharedPtr<FJsonObject> BuildGroupedRelationshipMap(const TArray<TSharedPtr<FJsonValue>>& Entries)
	{
		TMap<FString, TArray<TSharedPtr<FJsonValue>>> Grouped;
		for (const TSharedPtr<FJsonValue>& Value : Entries)
		{
			if (!Value.IsValid() || Value->Type != EJson::Object)
			{
				continue;
			}

			const TSharedPtr<FJsonObject> Entry = Value->AsObject();
			if (!Entry.IsValid())
			{
				continue;
			}

			FString GroupName = TEXT("other");
			Entry->TryGetStringField(TEXT("group"), GroupName);
			Grouped.FindOrAdd(GroupName).Add(Value);
		}

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		const TArray<FString> GroupNames = {
			TEXT("textures"),
			TEXT("material_functions"),
			TEXT("material_instances"),
			TEXT("materials"),
			TEXT("blueprints"),
			TEXT("animation_blueprints"),
			TEXT("niagara"),
			TEXT("meshes"),
			TEXT("parameter_collections"),
			TEXT("rendering_profiles"),
			TEXT("other")
		};

		for (const FString& GroupName : GroupNames)
		{
			Result->SetArrayField(GroupName, Grouped.FindRef(GroupName));
		}

		return Result;
	}

	static TSharedPtr<FJsonObject> BuildGroupCountMap(const TArray<TSharedPtr<FJsonValue>>& Entries)
	{
		TMap<FString, int32> Counts;
		for (const TSharedPtr<FJsonValue>& Value : Entries)
		{
			if (!Value.IsValid() || Value->Type != EJson::Object)
			{
				continue;
			}

			const TSharedPtr<FJsonObject> Entry = Value->AsObject();
			if (!Entry.IsValid())
			{
				continue;
			}

			FString GroupName = TEXT("other");
			Entry->TryGetStringField(TEXT("group"), GroupName);
			Counts.FindOrAdd(GroupName)++;
		}

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		const TArray<FString> GroupNames = {
			TEXT("textures"),
			TEXT("material_functions"),
			TEXT("material_instances"),
			TEXT("materials"),
			TEXT("blueprints"),
			TEXT("animation_blueprints"),
			TEXT("niagara"),
			TEXT("meshes"),
			TEXT("parameter_collections"),
			TEXT("rendering_profiles"),
			TEXT("other")
		};

		for (const FString& GroupName : GroupNames)
		{
			Result->SetNumberField(GroupName, Counts.FindRef(GroupName));
		}

		return Result;
	}

	static TSharedPtr<FJsonObject> BuildRelationshipSummary(
		const TArray<TSharedPtr<FJsonValue>>& DirectDependencies,
		const TArray<TSharedPtr<FJsonValue>>& RecursiveDependencies,
		const TArray<TSharedPtr<FJsonValue>>& DirectReferencers,
		const TArray<TSharedPtr<FJsonValue>>& RecursiveReferencers)
	{
		TSharedPtr<FJsonObject> Summary = MakeShareable(new FJsonObject());
		Summary->SetNumberField(TEXT("direct_dependencies_total"), DirectDependencies.Num());
		Summary->SetNumberField(TEXT("recursive_dependencies_total"), RecursiveDependencies.Num());
		Summary->SetNumberField(TEXT("direct_referencers_total"), DirectReferencers.Num());
		Summary->SetNumberField(TEXT("recursive_referencers_total"), RecursiveReferencers.Num());
		Summary->SetObjectField(TEXT("direct_dependencies_by_group"), BuildGroupCountMap(DirectDependencies));
		Summary->SetObjectField(TEXT("recursive_dependencies_by_group"), BuildGroupCountMap(RecursiveDependencies));
		Summary->SetObjectField(TEXT("direct_referencers_by_group"), BuildGroupCountMap(DirectReferencers));
		Summary->SetObjectField(TEXT("recursive_referencers_by_group"), BuildGroupCountMap(RecursiveReferencers));
		return Summary;
	}

	static TSharedPtr<FJsonObject> BuildObjectAssetRef(UObject* Object)
	{
		if (!Object)
		{
			return nullptr;
		}

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetStringField(TEXT("name"), Object->GetName());
		Result->SetStringField(TEXT("class"), Object->GetClass()->GetName());
		Result->SetStringField(TEXT("path"), Object->GetPathName());
		Result->SetStringField(TEXT("group"), ClassifyAssetGroup(Object->GetClass()->GetName()));
		return Result;
	}

	static bool IsEditorOnlyAssetObject(UObject* Object)
	{
		if (!Object)
		{
			return false;
		}

		const FString Path = Object->GetPathName();
		return Path.StartsWith(TEXT("/Engine/Editor")) || Path.StartsWith(TEXT("/Engine/VREditor"));
	}

	static bool IsEditorOnlyComponent(UActorComponent* Component)
	{
		if (!Component)
		{
			return false;
		}

		const FString ClassName = Component->GetClass()->GetName();
		return ClassName == TEXT("ArrowComponent")
			|| ClassName == TEXT("BillboardComponent")
			|| ClassName == TEXT("DrawFrustumComponent");
	}

	static bool ShouldKeepActorSection(const TSet<FString>& FocusComponents, const FString& SectionName)
	{
		return FocusComponents.Num() == 0 || FocusComponents.Contains(SectionName.ToLower());
	}

	static TArray<TSharedPtr<FJsonValue>> TraceAssetPackagesRecursive(
		IAssetRegistry& AssetRegistry,
		const FName& RootPackageName,
		UE::AssetRegistry::EDependencyCategory DependencyCategory,
		const UE::AssetRegistry::FDependencyQuery& QueryFlags,
		bool bTraceDependencies,
		int32 MaxDepth)
	{
		struct FTraceNode
		{
			FName PackageName;
			int32 Depth = 0;
		};

		TArray<TSharedPtr<FJsonValue>> Entries;
		if (MaxDepth <= 1)
		{
			return Entries;
		}

		TSet<FName> Visited;
		TArray<FTraceNode> Queue;
		Visited.Add(RootPackageName);
		Queue.Add({ RootPackageName, 0 });

		int32 QueueIndex = 0;
		while (QueueIndex < Queue.Num())
		{
			const FTraceNode Current = Queue[QueueIndex++];
			if (Current.Depth >= MaxDepth)
			{
				continue;
			}

			TArray<FName> RelatedPackages;
			if (bTraceDependencies)
			{
				AssetRegistry.GetDependencies(Current.PackageName, RelatedPackages, DependencyCategory, QueryFlags);
			}
			else
			{
				AssetRegistry.GetReferencers(Current.PackageName, RelatedPackages, DependencyCategory, QueryFlags);
			}

			const FString SourcePackageName = Current.PackageName.ToString();
			for (const FName& RelatedPackage : RelatedPackages)
			{
				if (Visited.Contains(RelatedPackage))
				{
					continue;
				}

				Visited.Add(RelatedPackage);
				const int32 NextDepth = Current.Depth + 1;
				Queue.Add({ RelatedPackage, NextDepth });
				Entries.Add(MakeShareable(new FJsonValueObject(
					BuildAssetRelationshipEntry(AssetRegistry, RelatedPackage, NextDepth, &SourcePackageName))));
			}
		}

		return Entries;
	}
}

// ============================================================
// Registration
// ============================================================

void FVFXToolRegistry::RegisterSceneTools()
{
	// ---- Tool: get_scene_actors ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_scene_actors");
		Def.Description = TEXT("Get a list of all actors in the current editor level with their labels, classes, and locations.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSceneActors);
		RegisterTool(Def);
	}

	// ---- Tool: trace_actor_relationships ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("trace_actor_relationships");
		Def.Description = TEXT(
			"Trace a placed actor in the current level: components, meshes, materials, Niagara systems, child actors, "
			"and the Blueprint asset behind the actor class.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Exact actor label from the World Outliner. Optional if one actor is selected.")));
		Props->SetObjectField(TEXT("use_selected_actor"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("include_component_assets"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include mesh/material/Niagara assets from components.")));
		Props->SetObjectField(TEXT("include_child_actors"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include child actor component references.")));
		Props->SetObjectField(TEXT("include_editor_only"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include editor-only helper components/assets.")));
		Props->SetObjectField(TEXT("focus_components"),
			MakeStringProperty(TEXT("Optional comma-separated filter: actor,blueprint,components,meshes,materials,niagara,child_actors")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_TraceActorRelationships);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_actor_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_actor_details");
		Def.Description = TEXT(
			"Inspect the first selected actor in the level and return components, meshes, materials, Niagara systems, "
			"and Blueprint backing asset.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedActorDetails);
		RegisterTool(Def);
	}

	// ---- Tool: spawn_actor ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("spawn_actor");
		Def.Description = TEXT(
			"Spawn an actor in the current level. "
			"Supports basic shapes: Cube, Sphere, Cylinder, Cone. "
			"Also supports: PointLight, SpotLight, DirectionalLight, CameraActor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_class"),
			MakeStringProperty(TEXT("Actor class to spawn: Cube, Sphere, Cylinder, Cone, PointLight, etc.")));
		Props->SetObjectField(TEXT("location"),
			MakeArrayProperty(TEXT("[X, Y, Z] world coordinates. Defaults to [0, 0, 0]."), TEXT("number")));
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Custom label for the actor in World Outliner.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually spawn. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);

		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("actor_class"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SpawnActor);
		RegisterTool(Def);
	}

	// ---- Tool: get_context ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("destroy_actor_by_label");
		Def.Description = TEXT(
			"Destroy the first editor-world actor whose label exactly matches actor_label. "
			"Useful for cleaning up temporary probe actors after runtime tests.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Exact editor actor label to destroy.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually destroy the actor. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DestroyActorByLabel);
		RegisterTool(Def);
	}

	// ---- Tool: get_context ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_context");
		Def.Description = TEXT(
			"Read current editor context: selected content browser assets, selected folders, and currently opened assets.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetContext);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_assets");
		Def.Description = TEXT("Get a list of assets currently selected in the Content Browser, with name, path, and class.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedAssets);
		RegisterTool(Def);
	}

	// ---- Tool: get_mutation_audit_entries ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_mutation_audit_entries");
		Def.Description = TEXT(
			"List recent ForgePilot mutation audit entries so you can see which assets were touched, by which tool, and where any safety snapshot or package file lives.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional integer. Default 20.")));
		Props->SetObjectField(TEXT("include_details"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include parsed entry objects.")));
		Props->SetObjectField(TEXT("tool_name"),
			MakeStringProperty(TEXT("Optional exact tool_name filter.")));
		Props->SetObjectField(TEXT("asset_class"),
			MakeStringProperty(TEXT("Optional exact asset_class filter. Example: NiagaraSystem.")));
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Optional exact asset_path filter.")));
		Props->SetObjectField(TEXT("asset_path_contains"),
			MakeStringProperty(TEXT("Optional substring filter for asset_path.")));
		Props->SetObjectField(TEXT("niagara_only"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Shortcut for asset_class=NiagaraSystem.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMutationAuditEntries);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_blueprint_path ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_blueprint_path");
		Def.Description = TEXT(
			"Return the object path of the first selected Blueprint asset from the Content Browser.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedBlueprintPath);
		RegisterTool(Def);
	}

	// ---- Tool: scan_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("scan_assets");
		Def.Description = TEXT(
			"Scan assets under a folder, optionally filtered by asset class name.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"),
			MakeStringProperty(TEXT("Content folder to scan (default: /Game/)")));
		Props->SetObjectField(TEXT("asset_type"),
			MakeStringProperty(TEXT("Optional class name filter, e.g. Blueprint, Material, NiagaraSystem")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max number of results (default: 200)")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ScanAssets);
		RegisterTool(Def);
	}

	// ---- Tool: search_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("search_assets");
		Def.Description = TEXT(
			"Search assets by query text, with optional folder scope and comma-separated class filters.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("query"),
			MakeStringProperty(TEXT("Search text matched against asset name/path/class")));
		Props->SetObjectField(TEXT("folder_path"),
			MakeStringProperty(TEXT("Optional folder path scope, e.g. /Game/Characters")));
		Props->SetObjectField(TEXT("class_names"),
			MakeStringProperty(TEXT("Optional comma-separated class filters, e.g. Blueprint,Material")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max number of results (default: 100)")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("query"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SearchAssets);
		RegisterTool(Def);
	}

	// ---- Tool: open_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("import_asset_from_file");
		Def.Description = TEXT(
			"Import a file from disk into the Content Browser. "
			"Useful for textures, meshes, or other source assets that need to become /Game/ assets before material or Niagara work.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("source_file"),
			MakeStringProperty(TEXT("Absolute source file path on disk, e.g. C:/Users/Name/Downloads/T_Fire.png")));
		Props->SetObjectField(TEXT("destination_path"),
			MakeStringProperty(TEXT("Destination /Game/ folder path, e.g. /Game/ForgePilot_Test/Textures/ImportedSources")));
		Props->SetObjectField(TEXT("destination_name"),
			MakeStringProperty(TEXT("Optional asset name override. Defaults to the source filename without extension.")));
		Props->SetObjectField(TEXT("replace_existing"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("save_asset"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("sync_browser"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Sync Content Browser to the imported asset.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually import. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("source_file"))));
		Required.Add(MakeShareable(new FJsonValueString(TEXT("destination_path"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ImportAssetFromFile);
		RegisterTool(Def);
	}

	// ---- Tool: open_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("trace_asset_relationships");
		Def.Description = TEXT(
			"Trace direct asset dependencies and referencers for an asset path. "
			"Useful for finding which Blueprint, Material, Niagara, or Mesh assets are connected to a given asset.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset object path, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Props->SetObjectField(TEXT("include_dependencies"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("include_referencers"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("dependency_query"),
			MakeStringProperty(TEXT("Optional: all, hard, soft. Default all.")));
		Props->SetObjectField(TEXT("max_depth"),
			MakeStringProperty(TEXT("Optional recursion depth. Default 1 (direct only). Use 2 or 3 to trace outward further.")));
		Props->SetObjectField(TEXT("start_from"),
			MakeStringProperty(TEXT("Optional: both (default), dependencies, referencers. Narrows which direction to trace.")));
		Props->SetObjectField(TEXT("focus_groups"),
			MakeStringProperty(TEXT("Optional comma-separated group filter, e.g. blueprints,material_instances,meshes")));
		Props->SetObjectField(TEXT("include_script_packages"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include /Script/* pseudo packages in recursive results.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_TraceAssetRelationships);
		RegisterTool(Def);
	}

	// ---- Tool: index_vfx_content_library ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("index_vfx_content_library");
		Def.Description = TEXT("Index a VFX content folder, default /Game/VFX, and summarize Niagara systems, material instances, templates, naming patterns, direct dependencies, and effect-library health.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"),
			MakeStringProperty(TEXT("Folder under /Game/. Default: /Game/VFX.")));
		Props->SetObjectField(TEXT("recursive"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("include_dependencies"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include capped direct dependency/referencer edges.")));
		Props->SetObjectField(TEXT("include_file_info"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include filename, size, and modified timestamp.")));
		Props->SetObjectField(TEXT("max_assets_per_section"),
			MakeStringProperty(TEXT("Optional integer. Default 250.")));
		Props->SetObjectField(TEXT("max_dependency_edges"),
			MakeStringProperty(TEXT("Optional integer. Default 500.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_IndexVFXContentLibrary);
		RegisterTool(Def);
	}

	// ---- Tool: create_asset_folder ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_asset_folder");
		Def.Description = TEXT("Create a Content Browser folder under /Game/. Use this before moving or duplicating assets into a new location.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"),
			MakeStringProperty(TEXT("Folder path under /Game/, e.g. /Game/ForgePilot_Test/Assist.")));
		Props->SetObjectField(TEXT("sync_browser"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Sync the Content Browser to the folder after creation.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually create the folder. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("folder_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateAssetFolder);
		RegisterTool(Def);
	}

	// ---- Tool: rename_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("rename_asset");
		Def.Description = TEXT("Rename a /Game/ asset in place without moving it to a different folder.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset object path, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Props->SetObjectField(TEXT("new_name"),
			MakeStringProperty(TEXT("New asset name only, without folder path or object path separators.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually rename the asset. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("asset_path"))),
			MakeShareable(new FJsonValueString(TEXT("new_name")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RenameAsset);
		RegisterTool(Def);
	}

	// ---- Tool: move_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("move_asset");
		Def.Description = TEXT("Move a /Game/ asset to another existing /Game/ folder. Optionally rename it during the move.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset object path, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Props->SetObjectField(TEXT("destination_path"),
			MakeStringProperty(TEXT("Existing destination folder under /Game/, e.g. /Game/ForgePilot_Test/Assist/Moved")));
		Props->SetObjectField(TEXT("destination_name"),
			MakeStringProperty(TEXT("Optional new asset name to use after the move. Defaults to the current asset name.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually move the asset. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("asset_path"))),
			MakeShareable(new FJsonValueString(TEXT("destination_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_MoveAsset);
		RegisterTool(Def);
	}

	// ---- Tool: duplicate_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("duplicate_asset");
		Def.Description = TEXT("Duplicate a /Game/ asset into another existing /Game/ folder with an explicit new asset name.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset object path, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Props->SetObjectField(TEXT("destination_path"),
			MakeStringProperty(TEXT("Existing destination folder under /Game/, e.g. /Game/ForgePilot_Test/Assist")));
		Props->SetObjectField(TEXT("new_name"),
			MakeStringProperty(TEXT("New asset name for the duplicate.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually duplicate the asset. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("asset_path"))),
			MakeShareable(new FJsonValueString(TEXT("destination_path"))),
			MakeShareable(new FJsonValueString(TEXT("new_name")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DuplicateAsset);
		RegisterTool(Def);
	}

	// ---- Tool: fix_redirectors_in_folder ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("fix_redirectors_in_folder");
		Def.Description = TEXT("Find and optionally fix redirectors under a /Game/ folder. Dry-run by default.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"), MakeStringProperty(TEXT("Folder under /Game/. Default: /Game.")));
		Props->SetObjectField(TEXT("recursive"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Set true to fix redirectors. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FixRedirectorsInFolder);
		RegisterTool(Def);
	}

	// ---- Tool: find_empty_folders ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("find_empty_folders");
		Def.Description = TEXT("Find empty Content Browser folders under /Game/. A folder is empty when it has no direct assets and no direct child folders.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"), MakeStringProperty(TEXT("Folder under /Game/. Default: /Game.")));
		Props->SetObjectField(TEXT("recursive"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("include_root"), MakeStringProperty(TEXT("Optional bool-like flag. Default false.")));
		Props->SetObjectField(TEXT("max_results"), MakeStringProperty(TEXT("Optional integer. Default 200.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FindEmptyFolders);
		RegisterTool(Def);
	}

	// ---- Tool: list_recent_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("list_recent_assets");
		Def.Description = TEXT("List recently modified /Game/ assets by package timestamp.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"), MakeStringProperty(TEXT("Folder under /Game/. Default: /Game.")));
		Props->SetObjectField(TEXT("recursive"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("class_names"), MakeStringProperty(TEXT("Optional comma-separated class filters, e.g. Material,NiagaraSystem.")));
		Props->SetObjectField(TEXT("max_results"), MakeStringProperty(TEXT("Optional integer. Default 50.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ListRecentAssets);
		RegisterTool(Def);
	}

	// ---- Tool: batch_rename_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("batch_rename_assets");
		Def.Description = TEXT("Batch rename /Game/ assets with explicit operations or asset_paths plus find/replace/prefix/suffix. Dry-run by default; preflights all targets before applying.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("operations"), MakeStringProperty(TEXT("Array of objects: {asset_path,new_name}. Preferred for exact renames.")));
		Props->SetObjectField(TEXT("asset_paths"), MakeStringProperty(TEXT("Optional array of asset paths used with find/replace/prefix/suffix.")));
		Props->SetObjectField(TEXT("find"), MakeStringProperty(TEXT("Optional substring to replace in each asset name.")));
		Props->SetObjectField(TEXT("replace"), MakeStringProperty(TEXT("Replacement for find. Default empty.")));
		Props->SetObjectField(TEXT("prefix"), MakeStringProperty(TEXT("Optional prefix added after find/replace.")));
		Props->SetObjectField(TEXT("suffix"), MakeStringProperty(TEXT("Optional suffix added after find/replace.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Set true to actually rename assets. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_BatchRenameAssets);
		RegisterTool(Def);
	}

	// ---- Tool: batch_move_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("batch_move_assets");
		Def.Description = TEXT("Batch move /Game/ assets using explicit operations or asset_paths plus one destination_path. Dry-run by default; destination folders must already exist.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("operations"), MakeStringProperty(TEXT("Array of objects: {asset_path,destination_path,optional destination_name}.")));
		Props->SetObjectField(TEXT("asset_paths"), MakeStringProperty(TEXT("Optional array of asset paths used with destination_path.")));
		Props->SetObjectField(TEXT("destination_path"), MakeStringProperty(TEXT("Existing destination folder under /Game/ when using asset_paths.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Set true to actually move assets. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_BatchMoveAssets);
		RegisterTool(Def);
	}

	// ---- Tool: find_large_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("find_large_assets");
		Def.Description = TEXT("Find large /Game/ assets by package file size.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"), MakeStringProperty(TEXT("Folder under /Game/. Default: /Game.")));
		Props->SetObjectField(TEXT("recursive"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("threshold_mb"), MakeStringProperty(TEXT("Optional number. Default 50.")));
		Props->SetObjectField(TEXT("class_names"), MakeStringProperty(TEXT("Optional comma-separated class filters.")));
		Props->SetObjectField(TEXT("max_results"), MakeStringProperty(TEXT("Optional integer. Default 100.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FindLargeAssets);
		RegisterTool(Def);
	}

	// ---- Tool: find_assets_modified_since ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("find_assets_modified_since");
		Def.Description = TEXT("Find /Game/ assets modified since an ISO date/time or within the last N days.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("modified_since"), MakeStringProperty(TEXT("ISO date/time, e.g. 2026-04-10 or 2026-04-10T13:30:00. Optional if days_ago is set.")));
		Props->SetObjectField(TEXT("days_ago"), MakeStringProperty(TEXT("Optional number of days back from now. Used when modified_since is omitted.")));
		Props->SetObjectField(TEXT("folder_path"), MakeStringProperty(TEXT("Folder under /Game/. Default: /Game.")));
		Props->SetObjectField(TEXT("recursive"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("class_names"), MakeStringProperty(TEXT("Optional comma-separated class filters.")));
		Props->SetObjectField(TEXT("max_results"), MakeStringProperty(TEXT("Optional integer. Default 200.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FindAssetsModifiedSince);
		RegisterTool(Def);
	}

	// ---- Tool: open_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("open_asset");
		Def.Description = TEXT("Resolve an asset and report bridge-safe open status. For stability, use browse_to_asset as the default way to focus assets in the editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full object path to open, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_OpenAsset);
		RegisterTool(Def);
	}

	// ---- Tool: browse_to_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("browse_to_asset");
		Def.Description = TEXT("Focus an asset in the Content Browser without opening the asset editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full object path, e.g. /Game/VFX/M_MyMat.M_MyMat")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_BrowseToAsset);
		RegisterTool(Def);
	}

	// ---- Tool: delete_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("delete_asset");
		Def.Description = TEXT(
			"Delete an asset from the project Content Browser. "
			"Only works on /Game/ assets. Cannot delete engine or plugin assets.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));

		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset path to delete, e.g. /Game/VFX/Materials/M_Fresnel_Energy")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually delete. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);

		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DeleteAsset);
		RegisterTool(Def);
	}

	// ---- Tool: capture_viewport_screenshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("verify_viewport_capture_state");
		Def.Description = TEXT(
			"Verify the active editor level viewport is in a trustworthy state for visual review captures. "
			"Can optionally force perspective/realtime first, then report whether the viewport is ready.");
		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("ensure_perspective"), MakeStringProperty(TEXT("Optional bool string. Default true. Forces perspective before verification.")));
		Properties->SetObjectField(TEXT("ensure_realtime"), MakeStringProperty(TEXT("Optional bool string. Default true. Forces realtime before verification.")));
		Properties->SetObjectField(TEXT("wait_ms"), MakeStringProperty(TEXT("Optional integer wait before final verification. Default 100.")));
		Properties->SetObjectField(TEXT("fail_if_unready"), MakeStringProperty(TEXT("Optional bool string. Default false. When true, the tool returns an error if the viewport is still not ready.")));
		Schema->SetObjectField(TEXT("properties"), Properties);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_VerifyViewportCaptureState);
		RegisterTool(Def);
	}

	// ---- Tool: capture_viewport_screenshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("capture_viewport_screenshot");
		Def.Description = TEXT(
			"Capture a screenshot of the active editor viewport. "
			"Returns the screenshot as base64 image data that you can analyze to check visual results. "
			"Use this to verify your work after creating materials, spawning actors, or modifying the scene.");
		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("ensure_perspective"), MakeStringProperty(TEXT("Optional bool string. Default true. Forces perspective before capture.")));
		Properties->SetObjectField(TEXT("ensure_realtime"), MakeStringProperty(TEXT("Optional bool string. Default true. Forces realtime before capture.")));
		Properties->SetObjectField(TEXT("wait_ms"), MakeStringProperty(TEXT("Optional integer wait before reading back pixels. Default 100.")));
		Properties->SetObjectField(TEXT("require_ready"), MakeStringProperty(TEXT("Optional bool string. Default true. Fails instead of capturing when the active viewport is still not in a trustworthy state.")));
		Schema->SetObjectField(TEXT("properties"), Properties);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CaptureViewport);
		RegisterTool(Def);
	}

	// ---- Tool: capture_asset_thumbnail ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("capture_asset_thumbnail");
		Def.Description = TEXT(
			"Render an asset thumbnail without relying on an active level viewport. "
			"Useful for smoke-test verification when editor viewport state is unavailable.");
		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("asset_path"), MakeStringProperty(TEXT("Required object path to the asset, e.g. /Game/VFX/NS_Test.NS_Test")));
		Properties->SetObjectField(TEXT("width"), MakeStringProperty(TEXT("Optional thumbnail width. Default 512.")));
		Properties->SetObjectField(TEXT("height"), MakeStringProperty(TEXT("Optional thumbnail height. Default 512.")));
		Properties->SetObjectField(TEXT("filename"), MakeStringProperty(TEXT("Optional output file stem. Saved under Project/Saved/ForgePilot/AssetThumbnails.")));
		Schema->SetObjectField(TEXT("properties"), Properties);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Required);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CaptureAssetThumbnail);
		RegisterTool(Def);
	}

	// ---- Tool: start_play_in_viewport ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("start_play_in_viewport");
		Def.Description = TEXT("Start a Play In Editor session in the active level viewport. Use this before runtime probes or gameplay input tests.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("simulate"), MakeStringProperty(TEXT("Optional bool-like value. If true, start Simulate In Editor instead of Play In Editor.")));
		Properties->SetObjectField(TEXT("play_from_camera"), MakeStringProperty(TEXT("Optional bool-like value. If true, start from the active editor camera transform.")));
		Schema->SetObjectField(TEXT("properties"), Properties);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_StartPlayInViewport);
		RegisterTool(Def);
	}

	// ---- Tool: stop_play_session ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("stop_play_session");
		Def.Description = TEXT("Stop the current Play In Editor session if one is running.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_StopPlaySession);
		RegisterTool(Def);
	}

	// ---- Tool: get_pie_session_state ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_pie_session_state");
		Def.Description = TEXT("Return the current editor Play In Editor state, active viewport status, and whether input can be sent.");
		Def.InputSchema = MakeEmptyObjectSchema();
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetPIESessionState);
		RegisterTool(Def);
	}

	// ---- Tool: send_pie_key_input ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("send_pie_key_input");
		Def.Description = TEXT("Send a key press, release, repeat, or tap to the active PIE viewport without using the system mouse.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("key"), MakeStringProperty(TEXT("Required key name such as H, B, SpaceBar, LeftMouseButton, One, or Gamepad_FaceButton_Bottom.")));
		Properties->SetObjectField(TEXT("event"), MakeStringProperty(TEXT("Optional event type: tap, pressed, released, repeat. Default is tap.")));
		Properties->SetObjectField(TEXT("amount"), MakeStringProperty(TEXT("Optional analog amount. Defaults to 1.0.")));
		Schema->SetObjectField(TEXT("properties"), Properties);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SendPIEKeyInput);
		RegisterTool(Def);
	}

	// ---- Tool: run_pie_probe_scenario ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_pie_probe_scenario");
		Def.Description = TEXT("Run a preset Play In Editor scenario from pie_scenarios.json, save captures to disk, and return a structured summary.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Properties = MakeShareable(new FJsonObject());
		Properties->SetObjectField(TEXT("scenario_name"), MakeStringProperty(TEXT("Required scenario key from Tools/Validation/pie_scenarios.json.")));
		Properties->SetObjectField(TEXT("output_dir"), MakeStringProperty(TEXT("Optional absolute output directory for saved captures and summary. Defaults to ~/Downloads/ForgePilotRuntimeProbes/<scenario_timestamp>.")));
		Properties->SetObjectField(TEXT("include_log_scan"), MakeStringProperty(TEXT("Optional bool-like value. Default true. Include latest PIE log summary in the result.")));
		Properties->SetObjectField(TEXT("abort_on_failure"), MakeStringProperty(TEXT("Optional bool-like value. Default true. Stop the scenario on the first failed step.")));
		Schema->SetObjectField(TEXT("properties"), Properties);
		TArray<TSharedPtr<FJsonValue>> Required;
		Required.Add(MakeShareable(new FJsonValueString(TEXT("scenario_name"))));
		Schema->SetArrayField(TEXT("required"), Required);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunPIEProbeScenario);
		RegisterTool(Def);
	}

	// ---- Tool: inspect_skeletal_attachment_context ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("inspect_skeletal_attachment_context");
		Def.Description = TEXT(
			"Inspect a placed actor's skeletal mesh attachment context: skeletal components, sockets, bones, and focused socket/bone transforms. "
			"Useful for attaching Niagara to the correct hand, forearm, or weapon socket without guessing offsets.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"), MakeStringProperty(TEXT("Exact actor label from the level. Optional if one actor is selected.")));
		Props->SetObjectField(TEXT("use_selected_actor"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("component_name"), MakeStringProperty(TEXT("Optional skeletal mesh component name filter.")));
		Props->SetObjectField(TEXT("socket_name"), MakeStringProperty(TEXT("Optional socket name to focus and resolve transform for.")));
		Props->SetObjectField(TEXT("bone_name"), MakeStringProperty(TEXT("Optional bone name to focus and resolve transform for.")));
		Props->SetObjectField(TEXT("include_all_socket_names"), MakeStringProperty(TEXT("Optional bool-like flag. Default true.")));
		Props->SetObjectField(TEXT("include_all_bone_names"), MakeStringProperty(TEXT("Optional bool-like flag. Default false.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_InspectSkeletalAttachmentContext);
		RegisterTool(Def);
	}

	// ---- Tool: load_vfx_recipe ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("load_vfx_recipe");
		Def.Description = TEXT(
			"Load an effect recipe from Plugins/ForgePilot/Knowledge/Effects/<Type>/recipe.json and attach relevant timing, color, and scale guidance from Knowledge/Principles.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("effect_type"),
			MakeStringProperty(TEXT("Effect type to load, e.g. fire, smoke, explosion, electricity, magic, water.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("effect_type"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_LoadVFXRecipe);
		RegisterTool(Def);
	}

	// ---- Tool: resolve_effect_plan ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("resolve_effect_plan");
		Def.Description = TEXT(
			"Resolve a freeform effect description into an ordered execution plan of ForgePilot tool calls based on Knowledge/ recipes and principles.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("description"),
			MakeStringProperty(TEXT("Freeform effect description such as 'campfire with embers and smoke'.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("description"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ResolveEffectPlan);
		RegisterTool(Def);
	}

	// ---- Tool: resolve_effect_plan_v2 ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("resolve_effect_plan_v2");
		Def.Description = TEXT(
			"Resolve a suction-core / pull-to-core description into the V2 sidecar archetype plan with semantic ops, failure taxonomy, repair policy, and stop conditions.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("description"),
			MakeStringProperty(TEXT("Freeform suction-core description such as 'fire energy being pulled into a hot core orb'.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("description"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ResolveEffectPlanV2);
		RegisterTool(Def);
	}

	// ---- Tool: generate_visual_contract_data_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("generate_visual_contract_data_asset");
		Def.Description = TEXT(
			"Generate a Blueprint-queryable data asset from a V2 archetype visual_contract. "
			"Stores both the raw JSON contract and typed op/binding entries for runtime query in Blueprint or C++.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("archetype"),
			MakeStringProperty(TEXT("Resolved V2 archetype, such as explosion_flash_core, impact_pulse_core, or suction_core_energy.")));
		Props->SetObjectField(TEXT("asset_name"),
			MakeStringProperty(TEXT("Optional asset name. Default: DA_VC_<archetype>.")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Optional package path. Default: /Game/ForgePilot_Generated/VisualContracts.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false (dry-run). Set true to create the data asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("archetype"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GenerateVisualContractDataAsset);
		RegisterTool(Def);
	}

	// ---- Tool: review_suction_core_failure_taxonomy ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_suction_core_failure_taxonomy");
		Def.Description = TEXT(
			"Run the V2 suction-core rules-based critic. Reviews emitter structure, module grammar, material family hints, and Layer A rebuild-gate state.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"), MakeStringProperty(TEXT("Niagara System asset path to review.")));
		Props->SetObjectField(TEXT("main_layer_emitter_name"), MakeStringProperty(TEXT("Optional Layer A emitter name. Default: MainAttractionStreaks.")));
		Props->SetObjectField(TEXT("secondary_layer_emitter_name"), MakeStringProperty(TEXT("Optional Layer B emitter name. Default: SecondaryFilaments.")));
		Props->SetObjectField(TEXT("core_layer_emitter_name"), MakeStringProperty(TEXT("Optional Layer C emitter name. Default: CoreTurbulence.")));
		Props->SetObjectField(TEXT("sparks_layer_emitter_name"), MakeStringProperty(TEXT("Optional Layer D emitter name. Default: Sparks.")));
		Props->SetObjectField(TEXT("shock_glow_emitter_name"), MakeStringProperty(TEXT("Optional Layer E emitter name. Default: ShockGlow.")));
		Props->SetObjectField(TEXT("hero_material_path"), MakeStringProperty(TEXT("Optional hero streak material path for hygiene/material-family review.")));
		Props->SetObjectField(TEXT("core_material_path"), MakeStringProperty(TEXT("Optional core material path for hygiene/material-family review.")));
		Props->SetObjectField(TEXT("consecutive_layer_a_motion_failures"), MakeStringProperty(TEXT("Optional previous consecutive Layer A motion-family failures. Default: 0.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("system_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewSuctionCoreFailureTaxonomy);
		RegisterTool(Def);
	}

	// ---- Tool: review_effect_failure_taxonomy_v2 ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_effect_failure_taxonomy_v2");
		Def.Description = TEXT(
			"Run the generic V2 rules-based critic for supported orb-centric archetypes. Reviews layer grammar, visual proxy metrics, and hero-layer rebuild-gate state.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("archetype"), MakeStringProperty(TEXT("Resolved V2 archetype: suction_core_energy, electric_arc_orb, hero_fire_orb_sweep, arcane_pull_orb, impact_pulse_core, or explosion_flash_core.")));
		Props->SetObjectField(TEXT("system_path"), MakeStringProperty(TEXT("Niagara System asset path to review.")));
		Props->SetObjectField(TEXT("hero_material_path"), MakeStringProperty(TEXT("Optional hero material path for material-family review.")));
		Props->SetObjectField(TEXT("core_material_path"), MakeStringProperty(TEXT("Optional core material path for material-family review.")));
		Props->SetObjectField(TEXT("consecutive_layer_a_motion_failures"), MakeStringProperty(TEXT("Optional previous consecutive hero-layer motion-family failures. Default: 0.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("archetype"))),
			MakeShareable(new FJsonValueString(TEXT("system_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewEffectFailureTaxonomyV2);
		RegisterTool(Def);
	}

	// ---- Tool: run_suction_core_v2_workflow ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_suction_core_v2_workflow");
		Def.Description = TEXT(
			"Run the full V2 suction-core workflow from plan resolution through material setup, Niagara authoring, critic review, "
			"optional Layer A rebuild enforcement, and final thumbnail capture.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("description"), MakeStringProperty(TEXT("Freeform suction-core description.")));
		Props->SetObjectField(TEXT("hero_parent_material_path"), MakeStringProperty(TEXT("Parent material path for hero streak Material Instance authoring.")));
		Props->SetObjectField(TEXT("core_parent_material_path"), MakeStringProperty(TEXT("Parent material path for core merge glow Material Instance authoring.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Optional generated asset folder. Default: /Game/ForgePilot_Generated/V2")));
		Props->SetObjectField(TEXT("system_name"), MakeStringProperty(TEXT("Optional explicit system name. Default uses a sanitized description token plus timestamp.")));
		Props->SetObjectField(TEXT("max_repair_iterations"), MakeStringProperty(TEXT("Optional max Layer A rebuild attempts after critic review. Default: 1.")));
		Props->SetObjectField(TEXT("capture_thumbnail"), MakeStringProperty(TEXT("Optional bool string. Default: true.")));
		Props->SetObjectField(TEXT("keep_assets"), MakeStringProperty(TEXT("Optional bool string. Default: false. When false, generated workflow assets are cleaned up after review artifacts are written.")));
		Props->SetObjectField(TEXT("thumbnail_filename"), MakeStringProperty(TEXT("Optional thumbnail output filename.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false. When false, returns a bound execution plan without creating assets.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("description"))),
			MakeShareable(new FJsonValueString(TEXT("hero_parent_material_path"))),
			MakeShareable(new FJsonValueString(TEXT("core_parent_material_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunSuctionCoreV2Workflow);
		RegisterTool(Def);
	}

	// ---- Tool: run_effect_workflow_v2 ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_effect_workflow_v2");
		Def.Description = TEXT(
			"Run the generic V2 orb-centric workflow for suction_core_energy, electric_arc_orb, hero_fire_orb_sweep, arcane_pull_orb, impact_pulse_core, or explosion_flash_core."
			" Resolves the plan, authors semantic layers, runs the critic, applies the rebuild gate, and captures a thumbnail.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("description"), MakeStringProperty(TEXT("Freeform orb-centric effect description.")));
		Props->SetObjectField(TEXT("archetype"), MakeStringProperty(TEXT("Optional explicit archetype override. Default: resolve from description.")));
		Props->SetObjectField(TEXT("hero_parent_material_path"), MakeStringProperty(TEXT("Parent material path for hero Material Instance authoring.")));
		Props->SetObjectField(TEXT("core_parent_material_path"), MakeStringProperty(TEXT("Parent material path for core Material Instance authoring.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Optional generated asset folder. Default: /Game/ForgePilot_Generated/V2")));
		Props->SetObjectField(TEXT("system_name"), MakeStringProperty(TEXT("Optional explicit system name.")));
		Props->SetObjectField(TEXT("max_repair_iterations"), MakeStringProperty(TEXT("Optional max hero-layer rebuild attempts after critic review. Default: 1.")));
		Props->SetObjectField(TEXT("capture_thumbnail"), MakeStringProperty(TEXT("Optional bool string. Default: true.")));
		Props->SetObjectField(TEXT("keep_assets"), MakeStringProperty(TEXT("Optional bool string. Default: false. When false, generated workflow assets are cleaned up after review artifacts are written.")));
		Props->SetObjectField(TEXT("thumbnail_filename"), MakeStringProperty(TEXT("Optional thumbnail output filename.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("description"))),
			MakeShareable(new FJsonValueString(TEXT("hero_parent_material_path"))),
			MakeShareable(new FJsonValueString(TEXT("core_parent_material_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunEffectWorkflowV2);
		RegisterTool(Def);
	}
}

// ============================================================
// Implementations
// ============================================================

bool FVFXToolRegistry::Tool_GetSceneActors(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> ActorsArray;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor || Actor->IsHiddenEd()) continue;

		TSharedPtr<FJsonObject> ActorObj = MakeShareable(new FJsonObject());
		ActorObj->SetStringField(TEXT("label"), Actor->GetActorLabel());
		ActorObj->SetStringField(TEXT("class"), Actor->GetClass()->GetName());

		FVector Loc = Actor->GetActorLocation();
		ActorObj->SetStringField(TEXT("location"),
			FString::Printf(TEXT("(%.1f, %.1f, %.1f)"), Loc.X, Loc.Y, Loc.Z));

		ActorsArray.Add(MakeShareable(new FJsonValueObject(ActorObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetNumberField(TEXT("count"), ActorsArray.Num());
	ResultObj->SetArrayField(TEXT("actors"), ActorsArray);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetSelectedActorDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	TSharedPtr<FJsonObject> ProxyInput = MakeShareable(new FJsonObject());
	ProxyInput->SetBoolField(TEXT("use_selected_actor"), true);
	ProxyInput->SetBoolField(TEXT("include_component_assets"), true);
	ProxyInput->SetBoolField(TEXT("include_child_actors"), true);

	if (Input.IsValid())
	{
		bool bValue = false;
		if (Input->TryGetBoolField(TEXT("include_component_assets"), bValue))
		{
			ProxyInput->SetBoolField(TEXT("include_component_assets"), bValue);
		}
		if (Input->TryGetBoolField(TEXT("include_child_actors"), bValue))
		{
			ProxyInput->SetBoolField(TEXT("include_child_actors"), bValue);
		}
		if (Input->TryGetBoolField(TEXT("include_editor_only"), bValue))
		{
			ProxyInput->SetBoolField(TEXT("include_editor_only"), bValue);
		}

		FString FocusComponents;
		if (Input->TryGetStringField(TEXT("focus_components"), FocusComponents))
		{
			ProxyInput->SetStringField(TEXT("focus_components"), FocusComponents);
		}
	}

	return Tool_TraceActorRelationships(ProxyInput, OutResult);
}

bool FVFXToolRegistry::Tool_TraceActorRelationships(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	FString ActorLabel;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("actor_label"), ActorLabel);
	}

	bool bUseSelectedActor = true;
	bool bIncludeComponentAssets = true;
	bool bIncludeChildActors = true;
	bool bIncludeEditorOnly = false;
	if (Input.IsValid())
	{
		Input->TryGetBoolField(TEXT("use_selected_actor"), bUseSelectedActor);
		Input->TryGetBoolField(TEXT("include_component_assets"), bIncludeComponentAssets);
		Input->TryGetBoolField(TEXT("include_child_actors"), bIncludeChildActors);
		Input->TryGetBoolField(TEXT("include_editor_only"), bIncludeEditorOnly);
	}
	const TSet<FString> FocusComponents = ParseTokenSet(Input, TEXT("focus_components"));

	AActor* TargetActor = nullptr;
	if (!ActorLabel.IsEmpty())
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (AActor* Actor = *It; Actor && Actor->GetActorLabel() == ActorLabel)
			{
				TargetActor = Actor;
				break;
			}
		}
	}

	if (!TargetActor && bUseSelectedActor && GEditor)
	{
		USelection* Selection = GEditor->GetSelectedActors();
		if (Selection && Selection->Num() > 0)
		{
			TargetActor = Cast<AActor>(Selection->GetSelectedObject(0));
		}
	}

	if (!TargetActor)
	{
		OutResult = ActorLabel.IsEmpty()
			? TEXT("No target actor found. Provide actor_label or select an actor in the level.")
			: FString::Printf(TEXT("Actor '%s' was not found in the current level."), *ActorLabel);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	if (ShouldKeepActorSection(FocusComponents, TEXT("actor")))
	{
		Result->SetStringField(TEXT("actor_label"), TargetActor->GetActorLabel());
		Result->SetStringField(TEXT("actor_name"), TargetActor->GetName());
		Result->SetStringField(TEXT("actor_class"), TargetActor->GetClass()->GetName());
		Result->SetStringField(TEXT("actor_path"), TargetActor->GetPathName());
		Result->SetStringField(TEXT("level_name"), TargetActor->GetLevel() ? TargetActor->GetLevel()->GetName() : TEXT("Unknown"));
		Result->SetBoolField(TEXT("is_hidden"), TargetActor->IsHidden());
		Result->SetBoolField(TEXT("is_temporarily_hidden_in_editor"), TargetActor->IsTemporarilyHiddenInEditor());
	}

	const FVector Location = TargetActor->GetActorLocation();
	const FRotator Rotation = TargetActor->GetActorRotation();
	const FVector Scale = TargetActor->GetActorScale3D();
	TSharedPtr<FJsonObject> TransformObj = MakeShareable(new FJsonObject());
	TransformObj->SetStringField(TEXT("location"), FString::Printf(TEXT("(X=%.2f,Y=%.2f,Z=%.2f)"), Location.X, Location.Y, Location.Z));
	TransformObj->SetStringField(TEXT("rotation"), FString::Printf(TEXT("(Pitch=%.2f,Yaw=%.2f,Roll=%.2f)"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
	TransformObj->SetStringField(TEXT("scale"), FString::Printf(TEXT("(X=%.2f,Y=%.2f,Z=%.2f)"), Scale.X, Scale.Y, Scale.Z));
	if (ShouldKeepActorSection(FocusComponents, TEXT("actor")))
	{
		Result->SetObjectField(TEXT("transform"), TransformObj);
	}

	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(TargetActor->GetClass()))
	{
		if (UBlueprint* BlueprintAsset = Cast<UBlueprint>(BPClass->ClassGeneratedBy))
		{
			if (TSharedPtr<FJsonObject> BlueprintRef = BuildObjectAssetRef(BlueprintAsset))
			{
				if ((bIncludeEditorOnly || !IsEditorOnlyAssetObject(BlueprintAsset))
					&& ShouldKeepActorSection(FocusComponents, TEXT("blueprint")))
				{
					Result->SetObjectField(TEXT("blueprint_asset"), BlueprintRef);
				}
			}
		}
	}

	TArray<UActorComponent*> ActorComponents;
	TargetActor->GetComponents(ActorComponents);

	TArray<TSharedPtr<FJsonValue>> ComponentsArray;
	TArray<TSharedPtr<FJsonValue>> MaterialRefs;
	TArray<TSharedPtr<FJsonValue>> NiagaraRefs;
	TArray<TSharedPtr<FJsonValue>> MeshRefs;
	TArray<TSharedPtr<FJsonValue>> ChildActorRefs;

	int32 PrimitiveComponentCount = 0;
	int32 TracedComponentCount = 0;
	int32 MaterialSlotCount = 0;
	int32 NiagaraComponentCount = 0;
	int32 MeshComponentCount = 0;

	for (UActorComponent* Component : ActorComponents)
	{
		if (!Component)
		{
			continue;
		}
		if (!bIncludeEditorOnly && IsEditorOnlyComponent(Component))
		{
			continue;
		}
		TracedComponentCount++;

		TSharedPtr<FJsonObject> ComponentObj = MakeShareable(new FJsonObject());
		ComponentObj->SetStringField(TEXT("name"), Component->GetName());
		ComponentObj->SetStringField(TEXT("class"), Component->GetClass()->GetName());
		ComponentObj->SetBoolField(TEXT("active"), Component->IsActive());

		if (USceneComponent* SceneComp = Cast<USceneComponent>(Component))
		{
			const FVector RelLoc = SceneComp->GetRelativeLocation();
			const FRotator RelRot = SceneComp->GetRelativeRotation();
			const FVector RelScale = SceneComp->GetRelativeScale3D();
			ComponentObj->SetStringField(TEXT("relative_location"), FString::Printf(TEXT("(X=%.2f,Y=%.2f,Z=%.2f)"), RelLoc.X, RelLoc.Y, RelLoc.Z));
			ComponentObj->SetStringField(TEXT("relative_rotation"), FString::Printf(TEXT("(Pitch=%.2f,Yaw=%.2f,Roll=%.2f)"), RelRot.Pitch, RelRot.Yaw, RelRot.Roll));
			ComponentObj->SetStringField(TEXT("relative_scale"), FString::Printf(TEXT("(X=%.2f,Y=%.2f,Z=%.2f)"), RelScale.X, RelScale.Y, RelScale.Z));
		}

		if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Component))
		{
			PrimitiveComponentCount++;
			ComponentObj->SetBoolField(TEXT("visible"), PrimitiveComp->IsVisible());
			ComponentObj->SetStringField(TEXT("mobility"), StaticEnum<EComponentMobility::Type>()->GetNameStringByValue(static_cast<int64>(PrimitiveComp->Mobility)));

			if (bIncludeComponentAssets)
			{
				TArray<TSharedPtr<FJsonValue>> ComponentMaterials;
				const int32 MaterialCount = PrimitiveComp->GetNumMaterials();
				for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
				{
					UMaterialInterface* Material = PrimitiveComp->GetMaterial(MaterialIndex);
					if (!Material)
					{
						continue;
					}
					if (!bIncludeEditorOnly && IsEditorOnlyAssetObject(Material))
					{
						continue;
					}

					TSharedPtr<FJsonObject> MaterialObj = BuildObjectAssetRef(Material);
					MaterialObj->SetNumberField(TEXT("slot_index"), MaterialIndex);
					ComponentMaterials.Add(MakeShareable(new FJsonValueObject(MaterialObj)));
					MaterialRefs.Add(MakeShareable(new FJsonValueObject(MaterialObj)));
					MaterialSlotCount++;
				}
				ComponentObj->SetArrayField(TEXT("materials"), ComponentMaterials);
			}
		}

		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Component))
		{
			if (UStaticMesh* StaticMesh = StaticMeshComp->GetStaticMesh())
			{
				if (bIncludeEditorOnly || !IsEditorOnlyAssetObject(StaticMesh))
				{
					TSharedPtr<FJsonObject> MeshObj = BuildObjectAssetRef(StaticMesh);
					if (MeshObj.IsValid())
					{
						MeshObj->SetStringField(TEXT("component_name"), Component->GetName());
						ComponentObj->SetObjectField(TEXT("static_mesh"), MeshObj);
						MeshRefs.Add(MakeShareable(new FJsonValueObject(MeshObj)));
						MeshComponentCount++;
					}
				}
			}
		}

		if (USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Component))
		{
			if (USkeletalMesh* SkeletalMesh = SkeletalMeshComp->GetSkeletalMeshAsset())
			{
				if (bIncludeEditorOnly || !IsEditorOnlyAssetObject(SkeletalMesh))
				{
					TSharedPtr<FJsonObject> MeshObj = BuildObjectAssetRef(SkeletalMesh);
					if (MeshObj.IsValid())
					{
						MeshObj->SetStringField(TEXT("component_name"), Component->GetName());
						ComponentObj->SetObjectField(TEXT("skeletal_mesh"), MeshObj);
						MeshRefs.Add(MakeShareable(new FJsonValueObject(MeshObj)));
						MeshComponentCount++;
					}
				}
			}

			if (UClass* AnimClass = SkeletalMeshComp->GetAnimClass())
			{
				TSharedPtr<FJsonObject> AnimObj = MakeShareable(new FJsonObject());
				AnimObj->SetStringField(TEXT("class"), AnimClass->GetName());
				AnimObj->SetStringField(TEXT("path"), AnimClass->GetPathName());
				ComponentObj->SetObjectField(TEXT("anim_class"), AnimObj);
			}
		}

		if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Component))
		{
			if (UNiagaraSystem* NiagaraSystem = NiagaraComp->GetAsset())
			{
				if (bIncludeEditorOnly || !IsEditorOnlyAssetObject(NiagaraSystem))
				{
					TSharedPtr<FJsonObject> NiagaraObj = BuildObjectAssetRef(NiagaraSystem);
					if (NiagaraObj.IsValid())
					{
						NiagaraObj->SetStringField(TEXT("component_name"), Component->GetName());
						ComponentObj->SetObjectField(TEXT("niagara_system"), NiagaraObj);
						NiagaraRefs.Add(MakeShareable(new FJsonValueObject(NiagaraObj)));
						NiagaraComponentCount++;
					}
				}
			}
		}

		if (UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(Component))
		{
			if (UParticleSystem* ParticleSystem = ParticleComp->Template)
			{
				if (bIncludeEditorOnly || !IsEditorOnlyAssetObject(ParticleSystem))
				{
					TSharedPtr<FJsonObject> ParticleObj = BuildObjectAssetRef(ParticleSystem);
					if (ParticleObj.IsValid())
					{
						ParticleObj->SetStringField(TEXT("component_name"), Component->GetName());
						ComponentObj->SetObjectField(TEXT("particle_system"), ParticleObj);
					}
				}
			}
		}

		if (bIncludeChildActors)
		{
			if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Component))
			{
				if (AActor* ChildActor = ChildActorComp->GetChildActor())
				{
					TSharedPtr<FJsonObject> ChildObj = MakeShareable(new FJsonObject());
					ChildObj->SetStringField(TEXT("actor_label"), ChildActor->GetActorLabel());
					ChildObj->SetStringField(TEXT("actor_name"), ChildActor->GetName());
					ChildObj->SetStringField(TEXT("actor_class"), ChildActor->GetClass()->GetName());
					ChildObj->SetStringField(TEXT("actor_path"), ChildActor->GetPathName());
					ComponentObj->SetObjectField(TEXT("child_actor"), ChildObj);
					ChildActorRefs.Add(MakeShareable(new FJsonValueObject(ChildObj)));
				}
			}
		}

		if (ShouldKeepActorSection(FocusComponents, TEXT("components")))
		{
			ComponentsArray.Add(MakeShareable(new FJsonValueObject(ComponentObj)));
		}
	}

	TSharedPtr<FJsonObject> Summary = MakeShareable(new FJsonObject());
	Summary->SetNumberField(TEXT("component_count"), TracedComponentCount);
	Summary->SetNumberField(TEXT("returned_component_count"), ComponentsArray.Num());
	Summary->SetNumberField(TEXT("primitive_component_count"), PrimitiveComponentCount);
	Summary->SetNumberField(TEXT("mesh_component_count"), MeshComponentCount);
	Summary->SetNumberField(TEXT("niagara_component_count"), NiagaraComponentCount);
	Summary->SetNumberField(TEXT("material_slot_count"), MaterialSlotCount);
	Summary->SetNumberField(TEXT("child_actor_count"), ChildActorRefs.Num());

	Result->SetObjectField(TEXT("summary"), Summary);
	if (ShouldKeepActorSection(FocusComponents, TEXT("components")))
	{
		Result->SetArrayField(TEXT("components"), ComponentsArray);
	}
	if (ShouldKeepActorSection(FocusComponents, TEXT("meshes")))
	{
		Result->SetArrayField(TEXT("mesh_assets"), MeshRefs);
	}
	if (ShouldKeepActorSection(FocusComponents, TEXT("materials")))
	{
		Result->SetArrayField(TEXT("material_assets"), MaterialRefs);
	}
	if (ShouldKeepActorSection(FocusComponents, TEXT("niagara")))
	{
		Result->SetArrayField(TEXT("niagara_assets"), NiagaraRefs);
	}
	if (ShouldKeepActorSection(FocusComponents, TEXT("child_actors")))
	{
		Result->SetArrayField(TEXT("child_actors"), ChildActorRefs);
	}

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_TraceAssetRelationships(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString AssetPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		OutResult = TEXT("Missing required field: asset_path");
		return false;
	}

	bool bIncludeDependencies = true;
	bool bIncludeReferencers = true;
	Input->TryGetBoolField(TEXT("include_dependencies"), bIncludeDependencies);
	Input->TryGetBoolField(TEXT("include_referencers"), bIncludeReferencers);

	FString DependencyQueryStr = TEXT("all");
	Input->TryGetStringField(TEXT("dependency_query"), DependencyQueryStr);
	DependencyQueryStr = DependencyQueryStr.ToLower();

	FString StartFrom = TEXT("both");
	Input->TryGetStringField(TEXT("start_from"), StartFrom);
	StartFrom = StartFrom.ToLower();

	int32 MaxDepth = 1;
	TryReadOptionalIntField(Input, TEXT("max_depth"), MaxDepth);
	MaxDepth = FMath::Clamp(MaxDepth, 1, 4);

	bool bIncludeScriptPackages = false;
	Input->TryGetBoolField(TEXT("include_script_packages"), bIncludeScriptPackages);
	const TSet<FString> FocusGroups = ParseFocusGroups(Input);
	if (StartFrom == TEXT("dependencies"))
	{
		bIncludeDependencies = true;
		bIncludeReferencers = false;
	}
	else if (StartFrom == TEXT("referencers"))
	{
		bIncludeDependencies = false;
		bIncludeReferencers = true;
	}

	UObject* AssetObject = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
	if (!AssetObject)
	{
		OutResult = FString::Printf(TEXT("Asset not found at '%s'"), *AssetPath);
		return false;
	}

	const FString PackageNameString = FPackageName::ObjectPathToPackageName(AssetPath);
	if (PackageNameString.IsEmpty())
	{
		OutResult = FString::Printf(TEXT("Could not resolve package name from '%s'"), *AssetPath);
		return false;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	UE::AssetRegistry::FDependencyQuery QueryFlags;
	if (DependencyQueryStr == TEXT("hard"))
	{
		QueryFlags = UE::AssetRegistry::EDependencyQuery::Hard;
	}
	else if (DependencyQueryStr == TEXT("soft"))
	{
		QueryFlags = UE::AssetRegistry::EDependencyQuery::Soft;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetStringField(TEXT("asset_path"), AssetPath);
	Result->SetStringField(TEXT("package_name"), PackageNameString);
	Result->SetStringField(TEXT("asset_name"), AssetObject->GetName());
	Result->SetStringField(TEXT("asset_class"), AssetObject->GetClass()->GetName());
	Result->SetStringField(TEXT("dependency_query"), DependencyQueryStr);
	Result->SetStringField(TEXT("start_from"), StartFrom);
	Result->SetNumberField(TEXT("max_depth"), MaxDepth);

	const FName PackageFName(*PackageNameString);
	TArray<TSharedPtr<FJsonValue>> SummaryDirectDependencies;
	TArray<TSharedPtr<FJsonValue>> SummaryRecursiveDependencies;
	TArray<TSharedPtr<FJsonValue>> SummaryDirectReferencers;
	TArray<TSharedPtr<FJsonValue>> SummaryRecursiveReferencers;

	if (bIncludeDependencies)
	{
		TArray<FName> Dependencies;
		AssetRegistry.GetDependencies(
			PackageFName,
			Dependencies,
			UE::AssetRegistry::EDependencyCategory::Package,
			QueryFlags);

		TArray<TSharedPtr<FJsonValue>> DependencyEntries;
		for (const FName& DependencyPackage : Dependencies)
		{
			DependencyEntries.Add(MakeShareable(new FJsonValueObject(
				BuildAssetRelationshipEntry(AssetRegistry, DependencyPackage))));
		}
		DependencyEntries = FilterRelationshipEntries(DependencyEntries, FocusGroups, bIncludeScriptPackages);
		SummaryDirectDependencies = DependencyEntries;

		Result->SetNumberField(TEXT("dependency_count"), DependencyEntries.Num());
		Result->SetArrayField(TEXT("dependencies"), DependencyEntries);
		Result->SetObjectField(TEXT("grouped_dependencies"), BuildGroupedRelationshipMap(DependencyEntries));

		const TArray<TSharedPtr<FJsonValue>> RecursiveDependencies = TraceAssetPackagesRecursive(
			AssetRegistry,
			PackageFName,
			UE::AssetRegistry::EDependencyCategory::Package,
			QueryFlags,
			true,
			MaxDepth);
		const TArray<TSharedPtr<FJsonValue>> FilteredRecursiveDependencies =
			FilterRelationshipEntries(RecursiveDependencies, FocusGroups, bIncludeScriptPackages);
		SummaryRecursiveDependencies = FilteredRecursiveDependencies;
		Result->SetNumberField(TEXT("recursive_dependency_count"), FilteredRecursiveDependencies.Num());
		Result->SetArrayField(TEXT("recursive_dependencies"), FilteredRecursiveDependencies);
		Result->SetObjectField(TEXT("grouped_recursive_dependencies"), BuildGroupedRelationshipMap(FilteredRecursiveDependencies));
	}
	else
	{
		Result->SetNumberField(TEXT("dependency_count"), 0);
		Result->SetArrayField(TEXT("dependencies"), {});
		Result->SetObjectField(TEXT("grouped_dependencies"), BuildGroupedRelationshipMap({}));
		Result->SetNumberField(TEXT("recursive_dependency_count"), 0);
		Result->SetArrayField(TEXT("recursive_dependencies"), {});
		Result->SetObjectField(TEXT("grouped_recursive_dependencies"), BuildGroupedRelationshipMap({}));
	}

	if (bIncludeReferencers)
	{
		TArray<FName> Referencers;
		AssetRegistry.GetReferencers(
			PackageFName,
			Referencers,
			UE::AssetRegistry::EDependencyCategory::Package,
			QueryFlags);

		TArray<TSharedPtr<FJsonValue>> ReferencerEntries;
		for (const FName& ReferencerPackage : Referencers)
		{
			ReferencerEntries.Add(MakeShareable(new FJsonValueObject(
				BuildAssetRelationshipEntry(AssetRegistry, ReferencerPackage))));
		}
		ReferencerEntries = FilterRelationshipEntries(ReferencerEntries, FocusGroups, bIncludeScriptPackages);
		SummaryDirectReferencers = ReferencerEntries;

		Result->SetNumberField(TEXT("referencer_count"), ReferencerEntries.Num());
		Result->SetArrayField(TEXT("referencers"), ReferencerEntries);
		Result->SetObjectField(TEXT("grouped_referencers"), BuildGroupedRelationshipMap(ReferencerEntries));

		const TArray<TSharedPtr<FJsonValue>> RecursiveReferencers = TraceAssetPackagesRecursive(
			AssetRegistry,
			PackageFName,
			UE::AssetRegistry::EDependencyCategory::Package,
			QueryFlags,
			false,
			MaxDepth);
		const TArray<TSharedPtr<FJsonValue>> FilteredRecursiveReferencers =
			FilterRelationshipEntries(RecursiveReferencers, FocusGroups, bIncludeScriptPackages);
		SummaryRecursiveReferencers = FilteredRecursiveReferencers;
		Result->SetNumberField(TEXT("recursive_referencer_count"), FilteredRecursiveReferencers.Num());
		Result->SetArrayField(TEXT("recursive_referencers"), FilteredRecursiveReferencers);
		Result->SetObjectField(TEXT("grouped_recursive_referencers"), BuildGroupedRelationshipMap(FilteredRecursiveReferencers));
	}
	else
	{
		Result->SetNumberField(TEXT("referencer_count"), 0);
		Result->SetArrayField(TEXT("referencers"), {});
		Result->SetObjectField(TEXT("grouped_referencers"), BuildGroupedRelationshipMap({}));
		Result->SetNumberField(TEXT("recursive_referencer_count"), 0);
		Result->SetArrayField(TEXT("recursive_referencers"), {});
		Result->SetObjectField(TEXT("grouped_recursive_referencers"), BuildGroupedRelationshipMap({}));
	}

	Result->SetObjectField(TEXT("summary"), BuildRelationshipSummary(
		SummaryDirectDependencies,
		SummaryRecursiveDependencies,
		SummaryDirectReferencers,
		SummaryRecursiveReferencers));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_IndexVFXContentLibrary(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game/VFX");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("INDEX_VFX_CONTENT_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	const bool bIncludeDependencies = ParseOptionalBoolField(Input, TEXT("include_dependencies"), true);
	const bool bIncludeFileInfo = ParseOptionalBoolField(Input, TEXT("include_file_info"), true);
	int32 MaxAssetsPerSection = 250;
	ParseOptionalIntField(Input, TEXT("max_assets_per_section"), MaxAssetsPerSection, MaxAssetsPerSection);
	MaxAssetsPerSection = FMath::Clamp(MaxAssetsPerSection, 1, 5000);
	int32 MaxDependencyEdges = 500;
	ParseOptionalIntField(Input, TEXT("max_dependency_edges"), MaxDependencyEdges, MaxDependencyEdges);
	MaxDependencyEdges = FMath::Clamp(MaxDependencyEdges, 0, 5000);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> Assets;
	GetScopedAssets(FolderPath, bRecursive, Assets);

	TMap<FString, int32> ClassCounts;
	TMap<FString, int32> GroupCounts;
	TMap<FString, int32> EffectFolderCounts;
	TMap<FString, int32> PrefixCounts;
	TMap<FString, int32> StemCounts;
	TMap<FString, TMap<FString, int32>> ClassPrefixCounts;

	TArray<TSharedPtr<FJsonValue>> NiagaraSystems;
	TArray<TSharedPtr<FJsonValue>> MaterialInstances;
	TArray<TSharedPtr<FJsonValue>> Templates;
	TArray<FAssetData> DependencySeedAssets;
	int32 NiagaraSystemTotal = 0;
	int32 MaterialInstanceTotal = 0;
	int32 TemplateTotal = 0;
	int64 TotalSizeBytes = 0;

	for (const FAssetData& AssetData : Assets)
	{
		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();
		const FString GroupName = ClassifyAssetGroup(ClassName);
		const FString AssetName = AssetData.AssetName.ToString();
		const FString Prefix = GetAssetPrefixToken(AssetName);
		const FString Stem = GetAssetStemToken(AssetName);
		const FString EffectFolder = GetRelativeFolderUnderRoot(FolderPath, AssetData.PackagePath.ToString());

		ClassCounts.FindOrAdd(ClassName)++;
		GroupCounts.FindOrAdd(GroupName)++;
		EffectFolderCounts.FindOrAdd(EffectFolder)++;
		PrefixCounts.FindOrAdd(Prefix)++;
		StemCounts.FindOrAdd(Stem)++;
		ClassPrefixCounts.FindOrAdd(ClassName).FindOrAdd(Prefix)++;

		if (bIncludeFileInfo)
		{
			TotalSizeBytes += FMath::Max<int64>(GetAssetFileSizeBytes(AssetData), 0);
		}

		if (ClassName == TEXT("NiagaraSystem"))
		{
			NiagaraSystemTotal++;
			DependencySeedAssets.Add(AssetData);
			if (NiagaraSystems.Num() < MaxAssetsPerSection)
			{
				NiagaraSystems.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(AssetData, bIncludeFileInfo))));
			}
		}

		if (ClassName.StartsWith(TEXT("MaterialInstance")))
		{
			MaterialInstanceTotal++;
			DependencySeedAssets.Add(AssetData);
			if (MaterialInstances.Num() < MaxAssetsPerSection)
			{
				MaterialInstances.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(AssetData, bIncludeFileInfo))));
			}
		}

		FString TemplateReason;
		if (IsVFXTemplateCandidate(AssetData, TemplateReason))
		{
			TemplateTotal++;
			DependencySeedAssets.Add(AssetData);
			if (Templates.Num() < MaxAssetsPerSection)
			{
				TSharedPtr<FJsonObject> TemplateObj = MakeAssetSummaryJson(AssetData, bIncludeFileInfo);
				TemplateObj->SetStringField(TEXT("template_reason"), TemplateReason);
				Templates.Add(MakeShareable(new FJsonValueObject(TemplateObj)));
			}
		}
	}

	TSharedPtr<FJsonObject> ClassPrefixJson = MakeShareable(new FJsonObject());
	for (const TPair<FString, TMap<FString, int32>>& Pair : ClassPrefixCounts)
	{
		ClassPrefixJson->SetObjectField(Pair.Key, BuildStringCountMapJson(Pair.Value, 25));
	}

	TArray<TSharedPtr<FJsonValue>> DependencyEdges;
	TMap<FString, int32> DependencyTargetGroupCounts;
	TSet<FName> DependencySeedPackages;
	for (const FAssetData& SeedAsset : DependencySeedAssets)
	{
		DependencySeedPackages.Add(SeedAsset.PackageName);
	}

	int32 DependencySeedCount = 0;
	if (bIncludeDependencies && MaxDependencyEdges > 0)
	{
		UE::AssetRegistry::FDependencyQuery QueryFlags;
		TSet<FString> SeenEdges;
		for (const FAssetData& SeedAsset : DependencySeedAssets)
		{
			if (DependencyEdges.Num() >= MaxDependencyEdges)
			{
				break;
			}

			DependencySeedCount++;
			TArray<FName> Dependencies;
			AssetRegistry.GetDependencies(SeedAsset.PackageName, Dependencies, UE::AssetRegistry::EDependencyCategory::Package, QueryFlags);
			for (const FName& DependencyPackage : Dependencies)
			{
				if (DependencyEdges.Num() >= MaxDependencyEdges)
				{
					break;
				}
				const FString TargetPackage = DependencyPackage.ToString();
				if (TargetPackage.StartsWith(TEXT("/Script/")))
				{
					continue;
				}

				const FString EdgeKey = SeedAsset.PackageName.ToString() + TEXT("|dependencies|") + TargetPackage;
				if (SeenEdges.Contains(EdgeKey))
				{
					continue;
				}
				SeenEdges.Add(EdgeKey);

				TSharedPtr<FJsonObject> EdgeObj = MakeShareable(new FJsonObject());
				EdgeObj->SetStringField(TEXT("direction"), TEXT("dependency"));
				EdgeObj->SetStringField(TEXT("source_asset_path"), SeedAsset.GetObjectPathString());
				EdgeObj->SetStringField(TEXT("source_class"), SeedAsset.AssetClassPath.GetAssetName().ToString());
				EdgeObj->SetObjectField(TEXT("target"), BuildAssetRelationshipEntry(AssetRegistry, DependencyPackage));
				EdgeObj->SetBoolField(TEXT("target_in_index_scope"), TargetPackage.StartsWith(FolderPath));
				DependencyEdges.Add(MakeShareable(new FJsonValueObject(EdgeObj)));

				TArray<FAssetData> TargetAssets;
				AssetRegistry.GetAssetsByPackageName(DependencyPackage, TargetAssets, false);
				const FString TargetClass = TargetAssets.Num() > 0 ? TargetAssets[0].AssetClassPath.GetAssetName().ToString() : TEXT("Unknown");
				DependencyTargetGroupCounts.FindOrAdd(ClassifyAssetGroup(TargetClass))++;
			}

			TArray<FName> Referencers;
			AssetRegistry.GetReferencers(SeedAsset.PackageName, Referencers, UE::AssetRegistry::EDependencyCategory::Package, QueryFlags);
			for (const FName& ReferencerPackage : Referencers)
			{
				if (DependencyEdges.Num() >= MaxDependencyEdges)
				{
					break;
				}
				const FString TargetPackage = ReferencerPackage.ToString();
				if (TargetPackage.StartsWith(TEXT("/Script/")))
				{
					continue;
				}

				const FString EdgeKey = SeedAsset.PackageName.ToString() + TEXT("|referencers|") + TargetPackage;
				if (SeenEdges.Contains(EdgeKey))
				{
					continue;
				}
				SeenEdges.Add(EdgeKey);

				TSharedPtr<FJsonObject> EdgeObj = MakeShareable(new FJsonObject());
				EdgeObj->SetStringField(TEXT("direction"), TEXT("referencer"));
				EdgeObj->SetStringField(TEXT("source_asset_path"), SeedAsset.GetObjectPathString());
				EdgeObj->SetStringField(TEXT("source_class"), SeedAsset.AssetClassPath.GetAssetName().ToString());
				EdgeObj->SetObjectField(TEXT("target"), BuildAssetRelationshipEntry(AssetRegistry, ReferencerPackage));
				EdgeObj->SetBoolField(TEXT("target_in_index_scope"), TargetPackage.StartsWith(FolderPath));
				DependencyEdges.Add(MakeShareable(new FJsonValueObject(EdgeObj)));
			}
		}
	}

	TArray<TSharedPtr<FJsonValue>> Warnings;
	if (!UEditorAssetLibrary::DoesDirectoryExist(FolderPath))
	{
		Warnings.Add(MakeShareable(new FJsonValueString(TEXT("Folder does not currently exist in the Content Browser."))));
	}
	if (NiagaraSystemTotal == 0)
	{
		Warnings.Add(MakeShareable(new FJsonValueString(TEXT("No NiagaraSystem assets found in this scope."))));
	}
	if (MaterialInstanceTotal == 0)
	{
		Warnings.Add(MakeShareable(new FJsonValueString(TEXT("No MaterialInstance assets found in this scope."))));
	}
	if (bIncludeDependencies && DependencyEdges.Num() >= MaxDependencyEdges)
	{
		Warnings.Add(MakeShareable(new FJsonValueString(TEXT("Dependency edge output was capped by max_dependency_edges."))));
	}

	TSharedPtr<FJsonObject> NamingPatterns = MakeShareable(new FJsonObject());
	NamingPatterns->SetObjectField(TEXT("prefix_counts"), BuildStringCountMapJson(PrefixCounts, 50));
	NamingPatterns->SetObjectField(TEXT("stem_counts"), BuildStringCountMapJson(StemCounts, 50));
	NamingPatterns->SetObjectField(TEXT("class_prefix_counts"), ClassPrefixJson);

	TSharedPtr<FJsonObject> AssetDependencies = MakeShareable(new FJsonObject());
	AssetDependencies->SetBoolField(TEXT("included"), bIncludeDependencies);
	AssetDependencies->SetNumberField(TEXT("seed_asset_count"), DependencySeedPackages.Num());
	AssetDependencies->SetNumberField(TEXT("processed_seed_asset_count"), DependencySeedCount);
	AssetDependencies->SetNumberField(TEXT("returned_edge_count"), DependencyEdges.Num());
	AssetDependencies->SetNumberField(TEXT("max_dependency_edges"), MaxDependencyEdges);
	AssetDependencies->SetObjectField(TEXT("target_group_counts"), BuildStringCountMapJson(DependencyTargetGroupCounts, 25));
	AssetDependencies->SetArrayField(TEXT("edges"), DependencyEdges);

	TSharedPtr<FJsonObject> Summary = MakeShareable(new FJsonObject());
	Summary->SetStringField(TEXT("folder_path"), FolderPath);
	Summary->SetBoolField(TEXT("folder_exists"), UEditorAssetLibrary::DoesDirectoryExist(FolderPath));
	Summary->SetBoolField(TEXT("recursive"), bRecursive);
	Summary->SetNumberField(TEXT("total_asset_count"), Assets.Num());
	Summary->SetNumberField(TEXT("niagara_system_count"), NiagaraSystemTotal);
	Summary->SetNumberField(TEXT("material_instance_count"), MaterialInstanceTotal);
	Summary->SetNumberField(TEXT("template_count"), TemplateTotal);
	Summary->SetNumberField(TEXT("total_size_bytes"), static_cast<double>(TotalSizeBytes));
	Summary->SetNumberField(TEXT("total_size_mb"), FMath::RoundToDouble((static_cast<double>(TotalSizeBytes) / (1024.0 * 1024.0)) * 100.0) / 100.0);
	Summary->SetObjectField(TEXT("class_counts"), BuildStringCountMapJson(ClassCounts, 80));
	Summary->SetObjectField(TEXT("group_counts"), BuildStringCountMapJson(GroupCounts, 25));
	Summary->SetObjectField(TEXT("effect_folder_counts"), BuildStringCountMapJson(EffectFolderCounts, 80));
	Summary->SetArrayField(TEXT("warnings"), Warnings);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("index_vfx_content_library"));
	Result->SetStringField(TEXT("folder_path"), FolderPath);
	Result->SetNumberField(TEXT("max_assets_per_section"), MaxAssetsPerSection);
	Result->SetObjectField(TEXT("effect_library_summary"), Summary);
	Result->SetArrayField(TEXT("niagara_systems"), NiagaraSystems);
	Result->SetArrayField(TEXT("material_instances"), MaterialInstances);
	Result->SetArrayField(TEXT("templates"), Templates);
	Result->SetObjectField(TEXT("naming_patterns"), NamingPatterns);
	Result->SetObjectField(TEXT("asset_dependencies"), AssetDependencies);
	Result->SetStringField(TEXT("message"), TEXT("VFX content library indexed."));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	TArray<FString> SelectedFolders;
	ContentBrowserModule.Get().GetSelectedFolders(SelectedFolders);

	TArray<TSharedPtr<FJsonValue>> SelectedAssetsArray;
	for (const FAssetData& AssetData : SelectedAssets)
	{
		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("name"), AssetData.AssetName.ToString());
		AssetObj->SetStringField(TEXT("path"), AssetData.GetObjectPathString());
		AssetObj->SetStringField(TEXT("class"), AssetData.AssetClassPath.GetAssetName().ToString());
		SelectedAssetsArray.Add(MakeShareable(new FJsonValueObject(AssetObj)));
	}

	TArray<TSharedPtr<FJsonValue>> SelectedFoldersArray;
	for (const FString& Folder : SelectedFolders)
	{
		SelectedFoldersArray.Add(MakeShareable(new FJsonValueString(Folder)));
	}

	TArray<TSharedPtr<FJsonValue>> OpenedAssetsArray;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			const TArray<UObject*> OpenedAssets = AssetEditorSubsystem->GetAllEditedAssets();
			for (UObject* AssetObj : OpenedAssets)
			{
				if (!AssetObj) continue;
				TSharedPtr<FJsonObject> OpenObj = MakeShareable(new FJsonObject());
				OpenObj->SetStringField(TEXT("name"), AssetObj->GetName());
				OpenObj->SetStringField(TEXT("path"), AssetObj->GetPathName());
				OpenObj->SetStringField(TEXT("class"), AssetObj->GetClass()->GetName());
				OpenedAssetsArray.Add(MakeShareable(new FJsonValueObject(OpenObj)));
			}
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetArrayField(TEXT("selected_assets"), SelectedAssetsArray);
	ResultObj->SetArrayField(TEXT("selected_folders"), SelectedFoldersArray);
	ResultObj->SetArrayField(TEXT("opened_assets"), OpenedAssetsArray);
	ResultObj->SetNumberField(TEXT("selected_asset_count"), SelectedAssetsArray.Num());
	ResultObj->SetNumberField(TEXT("selected_folder_count"), SelectedFoldersArray.Num());
	ResultObj->SetNumberField(TEXT("opened_asset_count"), OpenedAssetsArray.Num());

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_SpawnActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ActorClass;
	if (!Input->TryGetStringField(TEXT("actor_class"), ActorClass))
	{
		OutResult = TEXT("Missing required field: actor_class");
		return false;
	}

	// Parse location
	FVector Location = FVector::ZeroVector;
	const TArray<TSharedPtr<FJsonValue>>* LocArray;
	if (Input->TryGetArrayField(TEXT("location"), LocArray) && LocArray->Num() >= 3)
	{
		Location.X = (*LocArray)[0]->AsNumber();
		Location.Y = (*LocArray)[1]->AsNumber();
		Location.Z = (*LocArray)[2]->AsNumber();
	}

	FString ActorLabel;
	Input->TryGetStringField(TEXT("actor_label"), ActorLabel);
	bool bApplyChanges = false;
	Input->TryGetBoolField(TEXT("apply_changes"), bApplyChanges);

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("message"), TEXT("Dry-run only. Set apply_changes=true to spawn actor."));
		DryRunResult->SetStringField(TEXT("actor_class"), ActorClass);
		DryRunResult->SetStringField(TEXT("actor_label"), ActorLabel);
		DryRunResult->SetStringField(TEXT("location"),
			FString::Printf(TEXT("(%.1f, %.1f, %.1f)"), Location.X, Location.Y, Location.Z));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	AActor* SpawnedActor = nullptr;

	TMap<FString, FString> ShapeMeshPaths;
	ShapeMeshPaths.Add(TEXT("Cube"), TEXT("/Engine/BasicShapes/Cube.Cube"));
	ShapeMeshPaths.Add(TEXT("Sphere"), TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	ShapeMeshPaths.Add(TEXT("Cylinder"), TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	ShapeMeshPaths.Add(TEXT("Cone"), TEXT("/Engine/BasicShapes/Cone.Cone"));

	FString* MeshPath = ShapeMeshPaths.Find(ActorClass);
	if (MeshPath)
	{
		UStaticMesh* Mesh = Cast<UStaticMesh>(
			StaticLoadObject(UStaticMesh::StaticClass(), nullptr, **MeshPath));

		if (Mesh)
		{
			FActorSpawnParameters SpawnParams;
			SpawnedActor = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, SpawnParams);

			if (AStaticMeshActor* SMA = Cast<AStaticMeshActor>(SpawnedActor))
			{
				SMA->GetStaticMeshComponent()->SetStaticMesh(Mesh);
				SMA->SetMobility(EComponentMobility::Movable);
			}
		}
	}
	else
	{
		FString ClassName = ActorClass;
		if (!ClassName.StartsWith(TEXT("A")))
		{
			ClassName = TEXT("A") + ClassName;
		}

		UClass* FoundClass = FindFirstObject<UClass>(*ClassName, EFindFirstObjectOptions::NativeFirst);
		if (!FoundClass)
		{
			FoundClass = FindFirstObject<UClass>(*ActorClass, EFindFirstObjectOptions::NativeFirst);
		}

		if (FoundClass && FoundClass->IsChildOf(AActor::StaticClass()))
		{
			FTransform SpawnTransform(FRotator::ZeroRotator, Location);
			FActorSpawnParameters ClassSpawnParams;
			SpawnedActor = World->SpawnActor(FoundClass, &SpawnTransform, ClassSpawnParams);
		}
	}

	if (!SpawnedActor)
	{
		OutResult = FString::Printf(TEXT("Failed to spawn actor of class: %s"), *ActorClass);
		return false;
	}

	if (!ActorLabel.IsEmpty())
	{
		SpawnedActor->SetActorLabel(ActorLabel);
	}

	OutResult = FString::Printf(
		TEXT("{\"success\":true, \"message\":\"Spawned %s at (%.1f, %.1f, %.1f)\", \"label\":\"%s\"}"),
		*ActorClass, Location.X, Location.Y, Location.Z,
		*SpawnedActor->GetActorLabel());

	return true;
}

bool FVFXToolRegistry::Tool_GetSelectedAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	TArray<TSharedPtr<FJsonValue>> AssetsArray;

	for (const FAssetData& AssetData : SelectedAssets)
	{
		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("name"), AssetData.AssetName.ToString());
		AssetObj->SetStringField(TEXT("path"), AssetData.GetObjectPathString());
		AssetObj->SetStringField(TEXT("class"), AssetData.AssetClassPath.GetAssetName().ToString());
		AssetsArray.Add(MakeShareable(new FJsonValueObject(AssetObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetNumberField(TEXT("count"), AssetsArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), AssetsArray);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_DestroyActorByLabel(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ActorLabel;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel) || ActorLabel.IsEmpty())
	{
		OutResult = TEXT("Missing required field: actor_label");
		return false;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	AActor* TargetActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel() == ActorLabel)
		{
			TargetActor = *It;
			break;
		}
	}

	if (!TargetActor)
	{
		OutResult = FString::Printf(TEXT("Actor with label '%s' not found."), *ActorLabel);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("destroy_actor_by_label"));
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetStringField(TEXT("actor_name"), TargetActor->GetName());
	Result->SetStringField(TEXT("actor_class"), TargetActor->GetClass()->GetName());
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_level"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would destroy actor '%s'."), *ActorLabel));
		OutResult = JsonObjToString(Result);
		return true;
	}

	const bool bDestroyed = World->EditorDestroyActor(TargetActor, true);
	Result->SetBoolField(TEXT("success"), bDestroyed);
	Result->SetStringField(TEXT("message"),
		bDestroyed
			? FString::Printf(TEXT("Destroyed actor '%s'."), *ActorLabel)
			: FString::Printf(TEXT("Failed to destroy actor '%s'."), *ActorLabel));
	OutResult = JsonObjToString(Result);
	return bDestroyed;
}

bool FVFXToolRegistry::Tool_GetMutationAuditEntries(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	int32 MaxResults = 20;
	TryReadOptionalIntField(Input, TEXT("max_results"), MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 200);
	const bool bIncludeDetails = ParseOptionalBoolField(Input, TEXT("include_details"), true);
	FString ToolNameFilter;
	FString AssetClassFilter;
	FString AssetPathFilter;
	FString AssetPathContainsFilter;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("tool_name"), ToolNameFilter);
		Input->TryGetStringField(TEXT("asset_class"), AssetClassFilter);
		Input->TryGetStringField(TEXT("asset_path"), AssetPathFilter);
		Input->TryGetStringField(TEXT("asset_path_contains"), AssetPathContainsFilter);
	}
	const bool bNiagaraOnly = ParseOptionalBoolField(Input, TEXT("niagara_only"), false);
	if (bNiagaraOnly && AssetClassFilter.IsEmpty())
	{
		AssetClassFilter = TEXT("NiagaraSystem");
	}

	TArray<FString> EntryPaths;
	FVFXMutationAudit::GetRecentAuditEntryPaths(200, EntryPaths);

	TArray<TSharedPtr<FJsonValue>> EntryArray;
	for (const FString& EntryPath : EntryPaths)
	{
		TSharedPtr<FJsonObject> EntryObj = MakeShareable(new FJsonObject());
		EntryObj->SetStringField(TEXT("entry_path"), EntryPath);

		if (bIncludeDetails)
		{
			TSharedPtr<FJsonObject> ParsedEntry;
			if (FVFXMutationAudit::ReadAuditEntry(EntryPath, ParsedEntry) && ParsedEntry.IsValid())
			{
				const FString ParsedToolName = ParsedEntry->HasField(TEXT("tool_name")) ? ParsedEntry->GetStringField(TEXT("tool_name")) : TEXT("");
				const FString ParsedAssetClass = ParsedEntry->HasField(TEXT("asset_class")) ? ParsedEntry->GetStringField(TEXT("asset_class")) : TEXT("");
				const FString ParsedAssetPath = ParsedEntry->HasField(TEXT("asset_path")) ? ParsedEntry->GetStringField(TEXT("asset_path")) : TEXT("");

				if (!ToolNameFilter.IsEmpty() && !ParsedToolName.Equals(ToolNameFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetClassFilter.IsEmpty() && !ParsedAssetClass.Equals(AssetClassFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetPathFilter.IsEmpty() && !ParsedAssetPath.Equals(AssetPathFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetPathContainsFilter.IsEmpty() && !ParsedAssetPath.Contains(AssetPathContainsFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}

				EntryObj->SetObjectField(TEXT("entry"), ParsedEntry);
			}
			else
			{
				continue;
			}
		}
		else
		{
			TSharedPtr<FJsonObject> ParsedEntry;
			if (FVFXMutationAudit::ReadAuditEntry(EntryPath, ParsedEntry) && ParsedEntry.IsValid())
			{
				const FString ParsedToolName = ParsedEntry->HasField(TEXT("tool_name")) ? ParsedEntry->GetStringField(TEXT("tool_name")) : TEXT("");
				const FString ParsedAssetClass = ParsedEntry->HasField(TEXT("asset_class")) ? ParsedEntry->GetStringField(TEXT("asset_class")) : TEXT("");
				const FString ParsedAssetPath = ParsedEntry->HasField(TEXT("asset_path")) ? ParsedEntry->GetStringField(TEXT("asset_path")) : TEXT("");
				if (!ToolNameFilter.IsEmpty() && !ParsedToolName.Equals(ToolNameFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetClassFilter.IsEmpty() && !ParsedAssetClass.Equals(AssetClassFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetPathFilter.IsEmpty() && !ParsedAssetPath.Equals(AssetPathFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
				if (!AssetPathContainsFilter.IsEmpty() && !ParsedAssetPath.Contains(AssetPathContainsFilter, ESearchCase::IgnoreCase))
				{
					continue;
				}
			}
		}

		EntryArray.Add(MakeShareable(new FJsonValueObject(EntryObj)));
		if (EntryArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("get_mutation_audit_entries"));
	ResultObj->SetStringField(TEXT("audit_root"), FVFXMutationAudit::GetAuditRoot());
	ResultObj->SetStringField(TEXT("tool_name_filter"), ToolNameFilter);
	ResultObj->SetStringField(TEXT("asset_class_filter"), AssetClassFilter);
	ResultObj->SetStringField(TEXT("asset_path_filter"), AssetPathFilter);
	ResultObj->SetStringField(TEXT("asset_path_contains_filter"), AssetPathContainsFilter);
	ResultObj->SetBoolField(TEXT("niagara_only"), bNiagaraOnly);
	ResultObj->SetNumberField(TEXT("count"), EntryArray.Num());
	ResultObj->SetArrayField(TEXT("entries"), EntryArray);
	ResultObj->SetStringField(TEXT("message"),
		EntryArray.Num() > 0 ? TEXT("Mutation audit entries found.") : TEXT("No mutation audit entries found."));

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetSelectedBlueprintPath(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	for (const FAssetData& AssetData : SelectedAssets)
	{
		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();
		if (ClassName.Contains(TEXT("Blueprint")))
		{
			TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
			ResultObj->SetBoolField(TEXT("success"), true);
			ResultObj->SetStringField(TEXT("blueprint_path"), AssetData.GetObjectPathString());
			ResultObj->SetStringField(TEXT("asset_name"), AssetData.AssetName.ToString());
			OutResult = JsonObjToString(ResultObj);
			return true;
		}
	}

	OutResult = TEXT("{\"success\":false,\"message\":\"No Blueprint asset is selected in Content Browser.\"}");
	return false;
}

bool FVFXToolRegistry::Tool_ScanAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game/");
	FString AssetType;
	int32 MaxResults = 200;

	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
		Input->TryGetStringField(TEXT("asset_type"), AssetType);
		if (Input->HasField(TEXT("max_results")))
		{
			MaxResults = FMath::Clamp((int32)Input->GetNumberField(TEXT("max_results")), 1, 5000);
		}
	}

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> Assets;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*FolderPath), Assets, true);

	TArray<TSharedPtr<FJsonValue>> AssetsArray;
	for (const FAssetData& AssetData : Assets)
	{
		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();
		if (!AssetType.IsEmpty() && !ClassName.Equals(AssetType, ESearchCase::IgnoreCase))
		{
			continue;
		}

		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("name"), AssetData.AssetName.ToString());
		AssetObj->SetStringField(TEXT("path"), AssetData.GetObjectPathString());
		AssetObj->SetStringField(TEXT("class"), ClassName);
		AssetObj->SetStringField(TEXT("package_path"), AssetData.PackagePath.ToString());
		AssetsArray.Add(MakeShareable(new FJsonValueObject(AssetObj)));

		if (AssetsArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetStringField(TEXT("asset_type"), AssetType);
	ResultObj->SetNumberField(TEXT("scanned_count"), Assets.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), AssetsArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), AssetsArray);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_SearchAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString Query;
	if (!Input->TryGetStringField(TEXT("query"), Query) || Query.TrimStartAndEnd().IsEmpty())
	{
		OutResult = TEXT("Missing required field: query");
		return false;
	}

	FString FolderPath;
	Input->TryGetStringField(TEXT("folder_path"), FolderPath);
	FString ClassNamesCsv;
	Input->TryGetStringField(TEXT("class_names"), ClassNamesCsv);

	int32 MaxResults = 100;
	if (Input->HasField(TEXT("max_results")))
	{
		MaxResults = FMath::Clamp((int32)Input->GetNumberField(TEXT("max_results")), 1, 5000);
	}

	TArray<FString> ClassFilters;
	if (!ClassNamesCsv.IsEmpty())
	{
		ClassNamesCsv.ParseIntoArray(ClassFilters, TEXT(","), true);
		for (FString& Name : ClassFilters)
		{
			Name = Name.TrimStartAndEnd();
		}
	}

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> Candidates;
	if (!FolderPath.IsEmpty())
	{
		AssetRegistryModule.Get().GetAssetsByPath(FName(*FolderPath), Candidates, true);
	}
	else
	{
		AssetRegistryModule.Get().GetAllAssets(Candidates, true);
	}

	const FString QueryLower = Query.ToLower();
	TArray<TSharedPtr<FJsonValue>> MatchesArray;
	for (const FAssetData& AssetData : Candidates)
	{
		const FString Name = AssetData.AssetName.ToString();
		const FString Path = AssetData.GetObjectPathString();
		const FString ClassName = AssetData.AssetClassPath.GetAssetName().ToString();

		if (ClassFilters.Num() > 0)
		{
			bool bClassMatch = false;
			for (const FString& Filter : ClassFilters)
			{
				if (ClassName.Equals(Filter, ESearchCase::IgnoreCase))
				{
					bClassMatch = true;
					break;
				}
			}

			if (!bClassMatch)
			{
				continue;
			}
		}

		const bool bMatch =
			Name.ToLower().Contains(QueryLower) ||
			Path.ToLower().Contains(QueryLower) ||
			ClassName.ToLower().Contains(QueryLower);

		if (!bMatch)
		{
			continue;
		}

		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("name"), Name);
		AssetObj->SetStringField(TEXT("path"), Path);
		AssetObj->SetStringField(TEXT("class"), ClassName);
		AssetObj->SetStringField(TEXT("package_path"), AssetData.PackagePath.ToString());
		MatchesArray.Add(MakeShareable(new FJsonValueObject(AssetObj)));

		if (MatchesArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetStringField(TEXT("query"), Query);
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetStringField(TEXT("class_names"), ClassNamesCsv);
	ResultObj->SetNumberField(TEXT("scanned_count"), Candidates.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), MatchesArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), MatchesArray);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_OpenAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("OPEN_ASSET_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString RequestedAssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("OPEN_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}

	FAssetData AssetData;
	FString ResolvedAssetPath;
	if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, ResolvedAssetPath))
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("OPEN_ASSET_NOT_FOUND"),
			FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
	}

	TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
	DebugContext->SetStringField(TEXT("requested_asset_path"), RequestedAssetPath);
	DebugContext->SetStringField(TEXT("resolved_asset_path"), ResolvedAssetPath);
	DebugContext->SetStringField(TEXT("asset_class"), AssetData.AssetClassPath.GetAssetName().ToString());
	DebugContext->SetStringField(TEXT("recommended_tool"), TEXT("browse_to_asset"));

	return BuildSceneToolError(
		OutResult,
		TEXT("OPEN_ASSET_DISABLED_FOR_BRIDGE"),
		TEXT("Opening asset editor tabs through the bridge is disabled for stability because this environment can crash when editor toolkits spawn. Use browse_to_asset instead."),
		DebugContext);
}

bool FVFXToolRegistry::Tool_CreateAssetFolder(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("ASSET_FOLDER_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString FolderPath;
	if (!Input->TryGetStringField(TEXT("folder_path"), FolderPath) || FolderPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("ASSET_FOLDER_PATH_REQUIRED"), TEXT("Missing required field: folder_path."));
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("ASSET_FOLDER_NOT_GAME_PATH"), TEXT("folder_path must stay under /Game/."));
	}
	if (FolderPath.Contains(TEXT(".")))
	{
		return BuildSceneToolError(OutResult, TEXT("ASSET_FOLDER_OBJECT_PATH_NOT_ALLOWED"), TEXT("folder_path must be a folder path, not an asset object path."));
	}

	const bool bSyncBrowser = ParseOptionalBoolField(Input, TEXT("sync_browser"), true);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bExists = UEditorAssetLibrary::DoesDirectoryExist(FolderPath);

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("create_asset_folder"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	ResultObj->SetBoolField(TEXT("existed"), bExists);
	ResultObj->SetBoolField(TEXT("sync_browser"), bSyncBrowser);

	if (!bApplyChanges)
	{
		ResultObj->SetBoolField(TEXT("will_create"), !bExists);
		ResultObj->SetStringField(TEXT("message"), bExists ? TEXT("Dry-run: folder already exists.") : TEXT("Dry-run: would create folder."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	bool bCreated = false;
	if (!bExists)
	{
		bCreated = UEditorAssetLibrary::MakeDirectory(FolderPath);
		if (!bCreated)
		{
			return BuildSceneToolError(
				OutResult,
				TEXT("ASSET_FOLDER_CREATE_FAILED"),
				FString::Printf(TEXT("Failed to create folder '%s'."), *FolderPath));
		}
	}

	if (bSyncBrowser)
	{
		SyncContentBrowserToFolder(FolderPath);
	}

	ResultObj->SetBoolField(TEXT("created"), bCreated);
	ResultObj->SetStringField(TEXT("message"), bCreated ? TEXT("Folder created successfully.") : TEXT("Folder already existed."));
	if (bCreated)
	{
		TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
		AuditDetails->SetStringField(TEXT("folder_path"), FolderPath);
		AppendSceneMutationAudit(ResultObj, TEXT("create_asset_folder"), FolderPath, TEXT("ContentFolder"), TEXT("create_folder"), ResultObj->GetStringField(TEXT("message")), AuditDetails);
	}

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_RenameAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString RequestedAssetPath;
	FString NewName;
	if (!Input->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}
	if (!Input->TryGetStringField(TEXT("new_name"), NewName) || NewName.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_NEW_NAME_REQUIRED"), TEXT("Missing required field: new_name."));
	}

	FAssetData AssetData;
	FString SourceObjectPath;
	if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, SourceObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
	}
	if (!SourceObjectPath.StartsWith(TEXT("/Game/")))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be renamed."));
	}

	const FString SourceFolderPath = AssetData.PackagePath.ToString();
	const FString TargetObjectPath = BuildAssetObjectPath(SourceFolderPath, NewName);
	if (!FPackageName::IsValidObjectPath(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_INVALID_TARGET"), FString::Printf(TEXT("Invalid renamed asset path: %s"), *TargetObjectPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("rename_asset"));
	ResultObj->SetStringField(TEXT("before_asset_path"), SourceObjectPath);
	ResultObj->SetStringField(TEXT("after_asset_path"), TargetObjectPath);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);

	if (SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase))
	{
		ResultObj->SetBoolField(TEXT("renamed"), false);
		ResultObj->SetStringField(TEXT("message"), TEXT("Asset already has the requested name."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (UEditorAssetLibrary::DoesAssetExist(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_TARGET_EXISTS"), FString::Printf(TEXT("Target asset already exists: %s"), *TargetObjectPath));
	}

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run: would rename asset."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (!UEditorAssetLibrary::RenameAsset(SourceObjectPath, TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_FAILED"), FString::Printf(TEXT("Failed to rename asset '%s'."), *SourceObjectPath));
	}

	FAssetData RenamedAssetData;
	FString VerifiedTargetObjectPath;
	if (!ResolveAssetDataFromPath(TargetObjectPath, RenamedAssetData, VerifiedTargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("RENAME_ASSET_VERIFY_FAILED"), FString::Printf(TEXT("Asset rename succeeded, but the renamed asset could not be resolved at '%s'."), *TargetObjectPath));
	}

	SyncContentBrowserToAsset(RenamedAssetData);
	ResultObj->SetBoolField(TEXT("renamed"), true);
	ResultObj->SetStringField(TEXT("after_asset_path"), VerifiedTargetObjectPath);
	ResultObj->SetStringField(TEXT("message"), TEXT("Asset renamed successfully."));

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("source_asset_path"), SourceObjectPath);
	AuditDetails->SetStringField(TEXT("target_asset_path"), VerifiedTargetObjectPath);
	AppendSceneMutationAudit(
		ResultObj,
		TEXT("rename_asset"),
		VerifiedTargetObjectPath,
		RenamedAssetData.AssetClassPath.GetAssetName().ToString(),
		TEXT("rename_asset"),
		ResultObj->GetStringField(TEXT("message")),
		AuditDetails);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_MoveAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString RequestedAssetPath;
	FString DestinationPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}
	if (!Input->TryGetStringField(TEXT("destination_path"), DestinationPath) || DestinationPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_DESTINATION_REQUIRED"), TEXT("Missing required field: destination_path."));
	}

	FAssetData AssetData;
	FString SourceObjectPath;
	if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, SourceObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
	}
	if (!SourceObjectPath.StartsWith(TEXT("/Game/")))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be moved."));
	}

	DestinationPath = NormalizeFolderPath(DestinationPath);
	if (!IsGameFolderPath(DestinationPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_DESTINATION_NOT_GAME"), TEXT("destination_path must stay under /Game/."));
	}
	if (!UEditorAssetLibrary::DoesDirectoryExist(DestinationPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_DESTINATION_MISSING"), TEXT("Destination folder does not exist. Create it explicitly first."));
	}

	FString DestinationName;
	Input->TryGetStringField(TEXT("destination_name"), DestinationName);
	if (DestinationName.IsEmpty())
	{
		DestinationName = AssetData.AssetName.ToString();
	}

	const FString TargetObjectPath = BuildAssetObjectPath(DestinationPath, DestinationName);
	if (!FPackageName::IsValidObjectPath(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_INVALID_TARGET"), FString::Printf(TEXT("Invalid moved asset path: %s"), *TargetObjectPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("move_asset"));
	ResultObj->SetStringField(TEXT("before_asset_path"), SourceObjectPath);
	ResultObj->SetStringField(TEXT("after_asset_path"), TargetObjectPath);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);

	if (SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase))
	{
		ResultObj->SetBoolField(TEXT("moved"), false);
		ResultObj->SetStringField(TEXT("message"), TEXT("Asset is already at the requested destination."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (UEditorAssetLibrary::DoesAssetExist(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_TARGET_EXISTS"), FString::Printf(TEXT("Target asset already exists: %s"), *TargetObjectPath));
	}

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run: would move asset."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (!UEditorAssetLibrary::RenameAsset(SourceObjectPath, TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_FAILED"), FString::Printf(TEXT("Failed to move asset '%s'."), *SourceObjectPath));
	}

	FAssetData MovedAssetData;
	FString VerifiedTargetObjectPath;
	if (!ResolveAssetDataFromPath(TargetObjectPath, MovedAssetData, VerifiedTargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("MOVE_ASSET_VERIFY_FAILED"), FString::Printf(TEXT("Asset move succeeded, but the moved asset could not be resolved at '%s'."), *TargetObjectPath));
	}

	SyncContentBrowserToAsset(MovedAssetData);
	ResultObj->SetBoolField(TEXT("moved"), true);
	ResultObj->SetStringField(TEXT("after_asset_path"), VerifiedTargetObjectPath);
	ResultObj->SetStringField(TEXT("message"), TEXT("Asset moved successfully."));

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("source_asset_path"), SourceObjectPath);
	AuditDetails->SetStringField(TEXT("target_asset_path"), VerifiedTargetObjectPath);
	AppendSceneMutationAudit(
		ResultObj,
		TEXT("move_asset"),
		VerifiedTargetObjectPath,
		MovedAssetData.AssetClassPath.GetAssetName().ToString(),
		TEXT("move_asset"),
		ResultObj->GetStringField(TEXT("message")),
		AuditDetails);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_DuplicateAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString RequestedAssetPath;
	FString DestinationPath;
	FString NewName;
	if (!Input->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}
	if (!Input->TryGetStringField(TEXT("destination_path"), DestinationPath) || DestinationPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_DESTINATION_REQUIRED"), TEXT("Missing required field: destination_path."));
	}
	if (!Input->TryGetStringField(TEXT("new_name"), NewName) || NewName.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_NAME_REQUIRED"), TEXT("Missing required field: new_name."));
	}

	FAssetData AssetData;
	FString SourceObjectPath;
	if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, SourceObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
	}
	if (!SourceObjectPath.StartsWith(TEXT("/Game/")))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be duplicated."));
	}

	DestinationPath = NormalizeFolderPath(DestinationPath);
	if (!IsGameFolderPath(DestinationPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_DESTINATION_NOT_GAME"), TEXT("destination_path must stay under /Game/."));
	}
	if (!UEditorAssetLibrary::DoesDirectoryExist(DestinationPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_DESTINATION_MISSING"), TEXT("Destination folder does not exist. Create it explicitly first."));
	}

	const FString TargetObjectPath = BuildAssetObjectPath(DestinationPath, NewName);
	if (!FPackageName::IsValidObjectPath(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_INVALID_TARGET"), FString::Printf(TEXT("Invalid duplicate asset path: %s"), *TargetObjectPath));
	}
	if (UEditorAssetLibrary::DoesAssetExist(TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_TARGET_EXISTS"), FString::Printf(TEXT("Target asset already exists: %s"), *TargetObjectPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("duplicate_asset"));
	ResultObj->SetStringField(TEXT("before_asset_path"), SourceObjectPath);
	ResultObj->SetStringField(TEXT("after_asset_path"), TargetObjectPath);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run: would duplicate asset."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (!UEditorAssetLibrary::DuplicateAsset(SourceObjectPath, TargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_FAILED"), FString::Printf(TEXT("Failed to duplicate asset '%s'."), *SourceObjectPath));
	}

	FAssetData DuplicatedAssetData;
	FString VerifiedTargetObjectPath;
	if (!ResolveAssetDataFromPath(TargetObjectPath, DuplicatedAssetData, VerifiedTargetObjectPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DUPLICATE_ASSET_VERIFY_FAILED"), FString::Printf(TEXT("Asset duplicate succeeded, but the new asset could not be resolved at '%s'."), *TargetObjectPath));
	}

	SyncContentBrowserToAsset(DuplicatedAssetData);
	ResultObj->SetBoolField(TEXT("duplicated"), true);
	ResultObj->SetStringField(TEXT("after_asset_path"), VerifiedTargetObjectPath);
	ResultObj->SetStringField(TEXT("message"), TEXT("Asset duplicated successfully."));

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("source_asset_path"), SourceObjectPath);
	AuditDetails->SetStringField(TEXT("target_asset_path"), VerifiedTargetObjectPath);
	AppendSceneMutationAudit(
		ResultObj,
		TEXT("duplicate_asset"),
		VerifiedTargetObjectPath,
		DuplicatedAssetData.AssetClassPath.GetAssetName().ToString(),
		TEXT("duplicate_asset"),
		ResultObj->GetStringField(TEXT("message")),
		AuditDetails);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_FixRedirectorsInFolder(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("FIX_REDIRECTORS_FOLDER_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	TArray<FAssetData> Assets;
	GetScopedAssets(FolderPath, bRecursive, Assets);

	TArray<FAssetData> RedirectorAssets;
	TArray<TSharedPtr<FJsonValue>> RedirectorArray;
	for (const FAssetData& AssetData : Assets)
	{
		if (AssetData.AssetClassPath.GetAssetName().ToString() != TEXT("ObjectRedirector"))
		{
			continue;
		}

		RedirectorAssets.Add(AssetData);
		RedirectorArray.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(AssetData, true))));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("fix_redirectors_in_folder"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetBoolField(TEXT("recursive"), bRecursive);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	ResultObj->SetNumberField(TEXT("redirector_count"), RedirectorAssets.Num());
	ResultObj->SetArrayField(TEXT("redirectors"), RedirectorArray);

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), RedirectorAssets.Num() > 0 ? TEXT("Dry-run: would fix redirectors.") : TEXT("Dry-run: no redirectors found."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	if (RedirectorAssets.Num() == 0)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("No redirectors found."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	if (AssetToolsModule.Get().IsFixupReferencersInProgress())
	{
		return BuildSceneToolError(OutResult, TEXT("FIX_REDIRECTORS_ALREADY_IN_PROGRESS"), TEXT("AssetTools redirector fixup is already in progress."));
	}

	TArray<UObjectRedirector*> Redirectors;
	for (const FAssetData& AssetData : RedirectorAssets)
	{
		if (UObjectRedirector* Redirector = Cast<UObjectRedirector>(AssetData.GetAsset()))
		{
			Redirectors.Add(Redirector);
		}
	}

	if (Redirectors.Num() == 0)
	{
		return BuildSceneToolError(OutResult, TEXT("FIX_REDIRECTORS_LOAD_FAILED"), TEXT("Redirector assets were found but could not be loaded."));
	}

	AssetToolsModule.Get().FixupReferencers(Redirectors, false, ERedirectFixupMode::DeleteFixedUpRedirectors);

	TArray<FAssetData> RemainingAssets;
	GetScopedAssets(FolderPath, bRecursive, RemainingAssets);
	int32 RemainingRedirectors = 0;
	for (const FAssetData& AssetData : RemainingAssets)
	{
		if (AssetData.AssetClassPath.GetAssetName().ToString() == TEXT("ObjectRedirector"))
		{
			RemainingRedirectors++;
		}
	}

	ResultObj->SetNumberField(TEXT("loaded_redirector_count"), Redirectors.Num());
	ResultObj->SetNumberField(TEXT("remaining_redirector_count"), RemainingRedirectors);
	ResultObj->SetStringField(TEXT("message"), TEXT("Redirector fixup requested."));

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("folder_path"), FolderPath);
	AuditDetails->SetNumberField(TEXT("redirector_count"), RedirectorAssets.Num());
	AuditDetails->SetNumberField(TEXT("remaining_redirector_count"), RemainingRedirectors);
	AppendSceneMutationAudit(ResultObj, TEXT("fix_redirectors_in_folder"), FolderPath, TEXT("ContentFolder"), TEXT("fix_redirectors"), ResultObj->GetStringField(TEXT("message")), AuditDetails);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_FindEmptyFolders(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("FIND_EMPTY_FOLDERS_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	const bool bIncludeRoot = ParseOptionalBoolField(Input, TEXT("include_root"), false);
	int32 MaxResults = 200;
	ParseOptionalIntField(Input, TEXT("max_results"), MaxResults, MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 5000);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FString> CandidateFolders;
	if (bIncludeRoot)
	{
		CandidateFolders.Add(FolderPath);
	}
	AssetRegistryModule.Get().GetSubPaths(FolderPath, CandidateFolders, bRecursive);
	CandidateFolders.Sort();

	TArray<FAssetData> RecursiveAssets;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*FolderPath), RecursiveAssets, true);

	TSet<FString> FoldersWithDirectAssets;
	for (const FAssetData& AssetData : RecursiveAssets)
	{
		FoldersWithDirectAssets.Add(AssetData.PackagePath.ToString());
	}

	TArray<FString> AllSubFolders;
	AssetRegistryModule.Get().GetSubPaths(FolderPath, AllSubFolders, true);
	TSet<FString> FoldersWithDirectChildren;
	for (const FString& SubFolder : AllSubFolders)
	{
		FString ParentFolder;
		if (SubFolder.Split(TEXT("/"), &ParentFolder, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd) && !ParentFolder.IsEmpty())
		{
			FoldersWithDirectChildren.Add(ParentFolder);
		}
	}

	TArray<TSharedPtr<FJsonValue>> EmptyFolders;
	for (const FString& CandidateFolder : CandidateFolders)
	{
		if (!FoldersWithDirectAssets.Contains(CandidateFolder) && !FoldersWithDirectChildren.Contains(CandidateFolder))
		{
			EmptyFolders.Add(MakeShareable(new FJsonValueString(CandidateFolder)));
			if (EmptyFolders.Num() >= MaxResults)
			{
				break;
			}
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("find_empty_folders"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetBoolField(TEXT("recursive"), bRecursive);
	ResultObj->SetBoolField(TEXT("include_root"), bIncludeRoot);
	ResultObj->SetNumberField(TEXT("scanned_folder_count"), CandidateFolders.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), EmptyFolders.Num());
	ResultObj->SetArrayField(TEXT("folders"), EmptyFolders);
	ResultObj->SetStringField(TEXT("message"), EmptyFolders.Num() > 0 ? TEXT("Empty folders found.") : TEXT("No empty folders found."));
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_ListRecentAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game");
	FString ClassNamesCsv;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
		Input->TryGetStringField(TEXT("class_names"), ClassNamesCsv);
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("LIST_RECENT_ASSETS_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	int32 MaxResults = 50;
	ParseOptionalIntField(Input, TEXT("max_results"), MaxResults, MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 5000);
	const TArray<FString> ClassFilters = ParseCsvTokens(ClassNamesCsv);

	TArray<FAssetData> Assets;
	GetScopedAssets(FolderPath, bRecursive, Assets);

	struct FRecentAsset
	{
		FAssetData AssetData;
		FDateTime ModifiedTime;
	};

	TArray<FRecentAsset> Matches;
	for (const FAssetData& AssetData : Assets)
	{
		if (!AssetMatchesClassFilters(AssetData, ClassFilters))
		{
			continue;
		}

		const FDateTime ModifiedTime = GetAssetFileTimestamp(AssetData);
		if (ModifiedTime == FDateTime::MinValue())
		{
			continue;
		}

		Matches.Add({ AssetData, ModifiedTime });
	}

	Matches.Sort([](const FRecentAsset& A, const FRecentAsset& B)
	{
		return A.ModifiedTime > B.ModifiedTime;
	});

	TArray<TSharedPtr<FJsonValue>> AssetArray;
	for (const FRecentAsset& Match : Matches)
	{
		AssetArray.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(Match.AssetData, true))));
		if (AssetArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("list_recent_assets"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetStringField(TEXT("class_names"), ClassNamesCsv);
	ResultObj->SetNumberField(TEXT("scanned_count"), Assets.Num());
	ResultObj->SetNumberField(TEXT("matched_count"), Matches.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), AssetArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), AssetArray);
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_BatchRenameAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_INPUT_MISSING"), TEXT("Missing input."));
	}

	struct FRenamePlan
	{
		FAssetData AssetData;
		FString SourceObjectPath;
		FString TargetObjectPath;
		FString NewName;
	};

	TArray<TPair<FString, FString>> RequestedRenames;
	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (Input->TryGetArrayField(TEXT("operations"), Operations) && Operations != nullptr && Operations->Num() > 0)
	{
		for (const TSharedPtr<FJsonValue>& OperationValue : *Operations)
		{
			const TSharedPtr<FJsonObject> Operation = OperationValue.IsValid() ? OperationValue->AsObject() : nullptr;
			if (!Operation.IsValid())
			{
				return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_INVALID_OPERATION"), TEXT("Each operation must be an object."));
			}

			FString AssetPath;
			FString NewName;
			if (!Operation->TryGetStringField(TEXT("asset_path"), AssetPath) || AssetPath.TrimStartAndEnd().IsEmpty() ||
				!Operation->TryGetStringField(TEXT("new_name"), NewName) || NewName.TrimStartAndEnd().IsEmpty())
			{
				return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_OPERATION_FIELDS_REQUIRED"), TEXT("Each operation requires asset_path and new_name."));
			}
			RequestedRenames.Add(TPair<FString, FString>(AssetPath, NewName.TrimStartAndEnd()));
		}
	}
	else
	{
		TArray<FString> AssetPaths;
		if (!TryGetStringArrayField(Input, TEXT("asset_paths"), AssetPaths) || AssetPaths.Num() == 0)
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_ASSET_PATHS_REQUIRED"), TEXT("Provide operations or asset_paths."));
		}

		FString Find;
		FString Replace;
		FString Prefix;
		FString Suffix;
		Input->TryGetStringField(TEXT("find"), Find);
		Input->TryGetStringField(TEXT("replace"), Replace);
		Input->TryGetStringField(TEXT("prefix"), Prefix);
		Input->TryGetStringField(TEXT("suffix"), Suffix);
		if (Find.IsEmpty() && Prefix.IsEmpty() && Suffix.IsEmpty())
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_RULE_REQUIRED"), TEXT("asset_paths mode requires find, prefix, or suffix."));
		}

		for (const FString& AssetPath : AssetPaths)
		{
			FAssetData AssetData;
			FString SourceObjectPath;
			if (!ResolveAssetDataFromPath(AssetPath, AssetData, SourceObjectPath))
			{
				return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *AssetPath));
			}

			FString NewName = AssetData.AssetName.ToString();
			if (!Find.IsEmpty())
			{
				NewName = NewName.Replace(*Find, *Replace, ESearchCase::CaseSensitive);
			}
			NewName = Prefix + NewName + Suffix;
			RequestedRenames.Add(TPair<FString, FString>(SourceObjectPath, NewName));
		}
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TArray<FRenamePlan> Plans;
	TSet<FString> TargetPaths;
	TArray<TSharedPtr<FJsonValue>> OperationArray;

	for (const TPair<FString, FString>& RequestedRename : RequestedRenames)
	{
		FAssetData AssetData;
		FString SourceObjectPath;
		if (!ResolveAssetDataFromPath(RequestedRename.Key, AssetData, SourceObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedRename.Key));
		}
		if (!IsGameAssetObjectPath(SourceObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be renamed."));
		}

		const FString TargetObjectPath = BuildAssetObjectPath(AssetData.PackagePath.ToString(), RequestedRename.Value);
		if (!FPackageName::IsValidObjectPath(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_INVALID_TARGET"), FString::Printf(TEXT("Invalid target path: %s"), *TargetObjectPath));
		}
		if (!SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase) && UEditorAssetLibrary::DoesAssetExist(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_TARGET_EXISTS"), FString::Printf(TEXT("Target asset already exists: %s"), *TargetObjectPath));
		}
		if (TargetPaths.Contains(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_DUPLICATE_TARGET"), FString::Printf(TEXT("Multiple operations target the same path: %s"), *TargetObjectPath));
		}
		TargetPaths.Add(TargetObjectPath);
		Plans.Add({ AssetData, SourceObjectPath, TargetObjectPath, RequestedRename.Value });

		TSharedPtr<FJsonObject> OperationObj = MakeShareable(new FJsonObject());
		OperationObj->SetStringField(TEXT("before_asset_path"), SourceObjectPath);
		OperationObj->SetStringField(TEXT("after_asset_path"), TargetObjectPath);
		OperationObj->SetStringField(TEXT("new_name"), RequestedRename.Value);
		OperationObj->SetBoolField(TEXT("will_rename"), !SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase));
		OperationArray.Add(MakeShareable(new FJsonValueObject(OperationObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("batch_rename_assets"));
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	ResultObj->SetNumberField(TEXT("operation_count"), Plans.Num());
	ResultObj->SetArrayField(TEXT("operations"), OperationArray);

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run: batch rename preflight passed."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	TArray<TSharedPtr<FJsonValue>> AppliedArray;
	for (const FRenamePlan& Plan : Plans)
	{
		if (Plan.SourceObjectPath.Equals(Plan.TargetObjectPath, ESearchCase::IgnoreCase))
		{
			continue;
		}
		if (!UEditorAssetLibrary::RenameAsset(Plan.SourceObjectPath, Plan.TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_APPLY_FAILED"), FString::Printf(TEXT("Failed to rename asset '%s'."), *Plan.SourceObjectPath));
		}

		FAssetData RenamedAssetData;
		FString VerifiedTargetPath;
		if (!ResolveAssetDataFromPath(Plan.TargetObjectPath, RenamedAssetData, VerifiedTargetPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_RENAME_VERIFY_FAILED"), FString::Printf(TEXT("Renamed asset could not be resolved at '%s'."), *Plan.TargetObjectPath));
		}

		TSharedPtr<FJsonObject> AppliedObj = MakeShareable(new FJsonObject());
		AppliedObj->SetStringField(TEXT("before_asset_path"), Plan.SourceObjectPath);
		AppliedObj->SetStringField(TEXT("after_asset_path"), VerifiedTargetPath);
		AppliedArray.Add(MakeShareable(new FJsonValueObject(AppliedObj)));

		TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
		AuditDetails->SetStringField(TEXT("source_asset_path"), Plan.SourceObjectPath);
		AuditDetails->SetStringField(TEXT("target_asset_path"), VerifiedTargetPath);
		AppendSceneMutationAudit(ResultObj, TEXT("batch_rename_assets"), VerifiedTargetPath, RenamedAssetData.AssetClassPath.GetAssetName().ToString(), TEXT("batch_rename_asset"), TEXT("Batch asset rename applied."), AuditDetails);
	}

	ResultObj->SetArrayField(TEXT("applied_operations"), AppliedArray);
	ResultObj->SetNumberField(TEXT("applied_count"), AppliedArray.Num());
	ResultObj->SetStringField(TEXT("message"), TEXT("Batch rename completed."));
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_BatchMoveAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_INPUT_MISSING"), TEXT("Missing input."));
	}

	struct FMovePlan
	{
		FAssetData AssetData;
		FString SourceObjectPath;
		FString TargetObjectPath;
		FString DestinationPath;
		FString DestinationName;
	};

	TArray<TSharedPtr<FJsonObject>> RequestedMoves;
	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (Input->TryGetArrayField(TEXT("operations"), Operations) && Operations != nullptr && Operations->Num() > 0)
	{
		for (const TSharedPtr<FJsonValue>& OperationValue : *Operations)
		{
			const TSharedPtr<FJsonObject> Operation = OperationValue.IsValid() ? OperationValue->AsObject() : nullptr;
			if (!Operation.IsValid())
			{
				return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_INVALID_OPERATION"), TEXT("Each operation must be an object."));
			}
			RequestedMoves.Add(Operation);
		}
	}
	else
	{
		TArray<FString> AssetPaths;
		FString DestinationPath;
		Input->TryGetStringField(TEXT("destination_path"), DestinationPath);
		if (!TryGetStringArrayField(Input, TEXT("asset_paths"), AssetPaths) || AssetPaths.Num() == 0 || DestinationPath.TrimStartAndEnd().IsEmpty())
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_FIELDS_REQUIRED"), TEXT("Provide operations or asset_paths plus destination_path."));
		}

		for (const FString& AssetPath : AssetPaths)
		{
			TSharedPtr<FJsonObject> Operation = MakeShareable(new FJsonObject());
			Operation->SetStringField(TEXT("asset_path"), AssetPath);
			Operation->SetStringField(TEXT("destination_path"), DestinationPath);
			RequestedMoves.Add(Operation);
		}
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TArray<FMovePlan> Plans;
	TSet<FString> TargetPaths;
	TArray<TSharedPtr<FJsonValue>> OperationArray;

	for (const TSharedPtr<FJsonObject>& Operation : RequestedMoves)
	{
		FString RequestedAssetPath;
		FString DestinationPath;
		FString DestinationName;
		if (!Operation->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty() ||
			!Operation->TryGetStringField(TEXT("destination_path"), DestinationPath) || DestinationPath.TrimStartAndEnd().IsEmpty())
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_OPERATION_FIELDS_REQUIRED"), TEXT("Each operation requires asset_path and destination_path."));
		}

		FAssetData AssetData;
		FString SourceObjectPath;
		if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, SourceObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
		}
		if (!IsGameAssetObjectPath(SourceObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be moved."));
		}

		DestinationPath = NormalizeFolderPath(DestinationPath);
		if (!IsGameFolderPath(DestinationPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_DESTINATION_NOT_GAME"), TEXT("destination_path must stay under /Game/."));
		}
		if (!UEditorAssetLibrary::DoesDirectoryExist(DestinationPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_DESTINATION_MISSING"), FString::Printf(TEXT("Destination folder does not exist: %s"), *DestinationPath));
		}

		Operation->TryGetStringField(TEXT("destination_name"), DestinationName);
		if (DestinationName.TrimStartAndEnd().IsEmpty())
		{
			DestinationName = AssetData.AssetName.ToString();
		}
		DestinationName = DestinationName.TrimStartAndEnd();

		const FString TargetObjectPath = BuildAssetObjectPath(DestinationPath, DestinationName);
		if (!FPackageName::IsValidObjectPath(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_INVALID_TARGET"), FString::Printf(TEXT("Invalid target path: %s"), *TargetObjectPath));
		}
		if (!SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase) && UEditorAssetLibrary::DoesAssetExist(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_TARGET_EXISTS"), FString::Printf(TEXT("Target asset already exists: %s"), *TargetObjectPath));
		}
		if (TargetPaths.Contains(TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_DUPLICATE_TARGET"), FString::Printf(TEXT("Multiple operations target the same path: %s"), *TargetObjectPath));
		}
		TargetPaths.Add(TargetObjectPath);
		Plans.Add({ AssetData, SourceObjectPath, TargetObjectPath, DestinationPath, DestinationName });

		TSharedPtr<FJsonObject> OperationObj = MakeShareable(new FJsonObject());
		OperationObj->SetStringField(TEXT("before_asset_path"), SourceObjectPath);
		OperationObj->SetStringField(TEXT("after_asset_path"), TargetObjectPath);
		OperationObj->SetStringField(TEXT("destination_path"), DestinationPath);
		OperationObj->SetStringField(TEXT("destination_name"), DestinationName);
		OperationObj->SetBoolField(TEXT("will_move"), !SourceObjectPath.Equals(TargetObjectPath, ESearchCase::IgnoreCase));
		OperationArray.Add(MakeShareable(new FJsonValueObject(OperationObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("batch_move_assets"));
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	ResultObj->SetNumberField(TEXT("operation_count"), Plans.Num());
	ResultObj->SetArrayField(TEXT("operations"), OperationArray);

	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run: batch move preflight passed."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	TArray<TSharedPtr<FJsonValue>> AppliedArray;
	for (const FMovePlan& Plan : Plans)
	{
		if (Plan.SourceObjectPath.Equals(Plan.TargetObjectPath, ESearchCase::IgnoreCase))
		{
			continue;
		}
		if (!UEditorAssetLibrary::RenameAsset(Plan.SourceObjectPath, Plan.TargetObjectPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_APPLY_FAILED"), FString::Printf(TEXT("Failed to move asset '%s'."), *Plan.SourceObjectPath));
		}

		FAssetData MovedAssetData;
		FString VerifiedTargetPath;
		if (!ResolveAssetDataFromPath(Plan.TargetObjectPath, MovedAssetData, VerifiedTargetPath))
		{
			return BuildSceneToolError(OutResult, TEXT("BATCH_MOVE_VERIFY_FAILED"), FString::Printf(TEXT("Moved asset could not be resolved at '%s'."), *Plan.TargetObjectPath));
		}

		TSharedPtr<FJsonObject> AppliedObj = MakeShareable(new FJsonObject());
		AppliedObj->SetStringField(TEXT("before_asset_path"), Plan.SourceObjectPath);
		AppliedObj->SetStringField(TEXT("after_asset_path"), VerifiedTargetPath);
		AppliedArray.Add(MakeShareable(new FJsonValueObject(AppliedObj)));

		TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
		AuditDetails->SetStringField(TEXT("source_asset_path"), Plan.SourceObjectPath);
		AuditDetails->SetStringField(TEXT("target_asset_path"), VerifiedTargetPath);
		AppendSceneMutationAudit(ResultObj, TEXT("batch_move_assets"), VerifiedTargetPath, MovedAssetData.AssetClassPath.GetAssetName().ToString(), TEXT("batch_move_asset"), TEXT("Batch asset move applied."), AuditDetails);
	}

	ResultObj->SetArrayField(TEXT("applied_operations"), AppliedArray);
	ResultObj->SetNumberField(TEXT("applied_count"), AppliedArray.Num());
	ResultObj->SetStringField(TEXT("message"), TEXT("Batch move completed."));
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_FindLargeAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString FolderPath = TEXT("/Game");
	FString ClassNamesCsv;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("folder_path"), FolderPath);
		Input->TryGetStringField(TEXT("class_names"), ClassNamesCsv);
	}

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("FIND_LARGE_ASSETS_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	double ThresholdMb = 50.0;
	ParseOptionalDoubleField(Input, TEXT("threshold_mb"), ThresholdMb, ThresholdMb);
	const int64 ThresholdBytes = static_cast<int64>(ThresholdMb * 1024.0 * 1024.0);
	int32 MaxResults = 100;
	ParseOptionalIntField(Input, TEXT("max_results"), MaxResults, MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 5000);
	const TArray<FString> ClassFilters = ParseCsvTokens(ClassNamesCsv);

	TArray<FAssetData> Assets;
	GetScopedAssets(FolderPath, bRecursive, Assets);

	struct FLargeAsset
	{
		FAssetData AssetData;
		int64 SizeBytes;
	};

	TArray<FLargeAsset> Matches;
	for (const FAssetData& AssetData : Assets)
	{
		if (!AssetMatchesClassFilters(AssetData, ClassFilters))
		{
			continue;
		}
		const int64 SizeBytes = GetAssetFileSizeBytes(AssetData);
		if (SizeBytes >= ThresholdBytes)
		{
			Matches.Add({ AssetData, SizeBytes });
		}
	}

	Matches.Sort([](const FLargeAsset& A, const FLargeAsset& B)
	{
		return A.SizeBytes > B.SizeBytes;
	});

	TArray<TSharedPtr<FJsonValue>> AssetArray;
	for (const FLargeAsset& Match : Matches)
	{
		AssetArray.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(Match.AssetData, true))));
		if (AssetArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("find_large_assets"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetStringField(TEXT("class_names"), ClassNamesCsv);
	ResultObj->SetNumberField(TEXT("threshold_mb"), ThresholdMb);
	ResultObj->SetNumberField(TEXT("scanned_count"), Assets.Num());
	ResultObj->SetNumberField(TEXT("matched_count"), Matches.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), AssetArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), AssetArray);
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_FindAssetsModifiedSince(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("FIND_MODIFIED_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString FolderPath = TEXT("/Game");
	FString ClassNamesCsv;
	FString ModifiedSinceString;
	Input->TryGetStringField(TEXT("folder_path"), FolderPath);
	Input->TryGetStringField(TEXT("class_names"), ClassNamesCsv);
	Input->TryGetStringField(TEXT("modified_since"), ModifiedSinceString);

	FolderPath = NormalizeFolderPath(FolderPath);
	if (!IsGameFolderPath(FolderPath))
	{
		return BuildSceneToolError(OutResult, TEXT("FIND_MODIFIED_NOT_GAME"), TEXT("folder_path must stay under /Game/."));
	}

	FDateTime SinceTime = FDateTime::MinValue();
	if (!ModifiedSinceString.TrimStartAndEnd().IsEmpty())
	{
		if (!ParseDateTimeInput(ModifiedSinceString, SinceTime))
		{
			return BuildSceneToolError(OutResult, TEXT("FIND_MODIFIED_INVALID_DATE"), TEXT("modified_since must be an ISO-like date/time, e.g. 2026-04-10 or 2026-04-10T13:30:00."));
		}
	}
	else
	{
		double DaysAgo = 0.0;
		ParseOptionalDoubleField(Input, TEXT("days_ago"), DaysAgo, DaysAgo);
		if (DaysAgo <= 0.0)
		{
			return BuildSceneToolError(OutResult, TEXT("FIND_MODIFIED_SINCE_REQUIRED"), TEXT("Provide modified_since or days_ago."));
		}
		SinceTime = FDateTime::Now() - FTimespan::FromDays(DaysAgo);
	}

	const bool bRecursive = ParseOptionalBoolField(Input, TEXT("recursive"), true);
	int32 MaxResults = 200;
	ParseOptionalIntField(Input, TEXT("max_results"), MaxResults, MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 5000);
	const TArray<FString> ClassFilters = ParseCsvTokens(ClassNamesCsv);

	TArray<FAssetData> Assets;
	GetScopedAssets(FolderPath, bRecursive, Assets);

	struct FModifiedAsset
	{
		FAssetData AssetData;
		FDateTime ModifiedTime;
	};

	TArray<FModifiedAsset> Matches;
	for (const FAssetData& AssetData : Assets)
	{
		if (!AssetMatchesClassFilters(AssetData, ClassFilters))
		{
			continue;
		}
		const FDateTime ModifiedTime = GetAssetFileTimestamp(AssetData);
		if (ModifiedTime != FDateTime::MinValue() && ModifiedTime >= SinceTime)
		{
			Matches.Add({ AssetData, ModifiedTime });
		}
	}

	Matches.Sort([](const FModifiedAsset& A, const FModifiedAsset& B)
	{
		return A.ModifiedTime > B.ModifiedTime;
	});

	TArray<TSharedPtr<FJsonValue>> AssetArray;
	for (const FModifiedAsset& Match : Matches)
	{
		AssetArray.Add(MakeShareable(new FJsonValueObject(MakeAssetSummaryJson(Match.AssetData, true))));
		if (AssetArray.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("find_assets_modified_since"));
	ResultObj->SetStringField(TEXT("folder_path"), FolderPath);
	ResultObj->SetStringField(TEXT("class_names"), ClassNamesCsv);
	ResultObj->SetStringField(TEXT("modified_since"), DateTimeToIsoString(SinceTime));
	ResultObj->SetNumberField(TEXT("scanned_count"), Assets.Num());
	ResultObj->SetNumberField(TEXT("matched_count"), Matches.Num());
	ResultObj->SetNumberField(TEXT("returned_count"), AssetArray.Num());
	ResultObj->SetArrayField(TEXT("assets"), AssetArray);
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_BrowseToAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), AssetPath))
	{
		OutResult = TEXT("Missing required field: asset_path");
		return false;
	}

	bool bBrowsed = false;
	FString ResolvedAssetPath;
	FString ErrorMessage;
	FEvent* CompletionEvent = FPlatformProcess::GetSynchEventFromPool(true);
	auto BrowseAssetTask = [&]()
	{
		FAssetRegistryModule& AssetRegistryModule =
			FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(AssetPath));

		if (!AssetData.IsValid())
		{
			if (UObject* LoadedAsset = UEditorAssetLibrary::LoadAsset(AssetPath))
			{
				AssetData = FAssetData(LoadedAsset);
			}
		}

		if (!AssetData.IsValid())
		{
			ErrorMessage = FString::Printf(TEXT("Asset not found: %s"), *AssetPath);
		}
		else
		{
			SyncContentBrowserToAsset(AssetData);
			ResolvedAssetPath = AssetData.GetObjectPathString();
			bBrowsed = true;
		}

		CompletionEvent->Trigger();
	};

	if (IsInGameThread())
	{
		BrowseAssetTask();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, MoveTemp(BrowseAssetTask));
		CompletionEvent->Wait();
	}
	FPlatformProcess::ReturnSynchEventToPool(CompletionEvent);

	if (!bBrowsed)
	{
		OutResult = ErrorMessage.IsEmpty() ? TEXT("Failed to browse to asset.") : ErrorMessage;
		return false;
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("asset_path"), ResolvedAssetPath);
	ResultObj->SetStringField(TEXT("message"), TEXT("Content Browser synced to asset."));
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_DeleteAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("DELETE_ASSET_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString RequestedAssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), RequestedAssetPath) || RequestedAssetPath.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("DELETE_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FAssetData AssetData;
	FString AssetPath;
	if (!ResolveAssetDataFromPath(RequestedAssetPath, AssetData, AssetPath))
	{
		return BuildSceneToolError(OutResult, TEXT("DELETE_ASSET_NOT_FOUND"), FString::Printf(TEXT("Asset not found: %s"), *RequestedAssetPath));
	}

	if (!AssetPath.StartsWith(TEXT("/Game/")))
	{
		return BuildSceneToolError(OutResult, TEXT("DELETE_ASSET_NON_GAME_BLOCKED"), TEXT("Only /Game/ assets can be deleted."));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("action"), TEXT("delete_asset"));
	ResultObj->SetStringField(TEXT("asset_path"), AssetPath);
	ResultObj->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	if (!bApplyChanges)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("Dry-run only. Set apply_changes=true to delete asset."));
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	UObject* AssetObj = AssetData.GetAsset();
	if (!AssetObj)
	{
		return BuildSceneToolError(OutResult, TEXT("DELETE_ASSET_LOAD_FAILED"), FString::Printf(TEXT("Failed to load asset: %s"), *AssetPath));
	}

	TArray<UObject*> ObjectsToDelete;
	ObjectsToDelete.Add(AssetObj);

	int32 DeletedCount = ObjectTools::ForceDeleteObjects(ObjectsToDelete, false);

	if (DeletedCount > 0)
	{
		ResultObj->SetStringField(TEXT("message"), FString::Printf(TEXT("Successfully deleted asset: %s"), *AssetPath));
		TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
		AuditDetails->SetStringField(TEXT("deleted_asset_path"), AssetPath);
		AppendSceneMutationAudit(
			ResultObj,
			TEXT("delete_asset"),
			AssetPath,
			AssetData.AssetClassPath.GetAssetName().ToString(),
			TEXT("delete_asset"),
			ResultObj->GetStringField(TEXT("message")),
			AuditDetails);
		OutResult = JsonObjToString(ResultObj);
		return true;
	}

	return BuildSceneToolError(
		OutResult,
		TEXT("DELETE_ASSET_FAILED"),
		FString::Printf(TEXT("Failed to delete asset: %s. It may be referenced by other assets."), *AssetPath));
}

bool FVFXToolRegistry::Tool_ImportAssetFromFile(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString SourceFile;
	if (!Input->TryGetStringField(TEXT("source_file"), SourceFile) || SourceFile.IsEmpty())
	{
		OutResult = TEXT("Missing required field: source_file");
		return false;
	}

	FString DestinationPath;
	if (!Input->TryGetStringField(TEXT("destination_path"), DestinationPath) || DestinationPath.IsEmpty())
	{
		OutResult = TEXT("Missing required field: destination_path");
		return false;
	}

	if (!FPaths::FileExists(SourceFile))
	{
		OutResult = FString::Printf(TEXT("Source file does not exist: %s"), *SourceFile);
		return false;
	}

	if (!DestinationPath.StartsWith(TEXT("/Game/")))
	{
		OutResult = FString::Printf(TEXT("Destination path must be under /Game/: %s"), *DestinationPath);
		return false;
	}

	FString DestinationName;
	Input->TryGetStringField(TEXT("destination_name"), DestinationName);
	if (DestinationName.IsEmpty())
	{
		DestinationName = FPaths::GetBaseFilename(SourceFile);
	}

	const bool bReplaceExisting = ParseOptionalBoolField(Input, TEXT("replace_existing"), true);
	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), true);
	const bool bSyncBrowser = ParseOptionalBoolField(Input, TEXT("sync_browser"), true);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	const FString ExpectedObjectPath = FString::Printf(TEXT("%s/%s.%s"), *DestinationPath, *DestinationName, *DestinationName);

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("source_file"), SourceFile);
		DryRunResult->SetStringField(TEXT("destination_path"), DestinationPath);
		DryRunResult->SetStringField(TEXT("destination_name"), DestinationName);
		DryRunResult->SetStringField(TEXT("expected_object_path"), ExpectedObjectPath);
		DryRunResult->SetStringField(TEXT("message"), TEXT("Dry-run only. Set apply_changes=true to import asset."));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	if (!UEditorAssetLibrary::DoesDirectoryExist(DestinationPath))
	{
		UEditorAssetLibrary::MakeDirectory(DestinationPath);
	}

	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->Filename = SourceFile;
	ImportTask->DestinationPath = DestinationPath;
	ImportTask->DestinationName = DestinationName;
	ImportTask->bAutomated = true;
	ImportTask->bReplaceExisting = bReplaceExisting;
	ImportTask->bReplaceExistingSettings = bReplaceExisting;
	ImportTask->bSave = bSaveAsset;

	TArray<UAssetImportTask*> Tasks;
	Tasks.Add(ImportTask);

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks(Tasks);

	TArray<TSharedPtr<FJsonValue>> ImportedArray;
	for (const FString& ImportedPath : ImportTask->ImportedObjectPaths)
	{
		if (!ImportedPath.IsEmpty())
		{
			ImportedArray.Add(MakeShareable(new FJsonValueString(ImportedPath)));
		}
	}

	if (ImportedArray.Num() == 0)
	{
		UObject* ImportedAsset = UEditorAssetLibrary::LoadAsset(ExpectedObjectPath);
		if (ImportedAsset)
		{
			ImportedArray.Add(MakeShareable(new FJsonValueString(ImportedAsset->GetPathName())));
		}
	}

	if (ImportedArray.Num() == 0)
	{
		OutResult = FString::Printf(TEXT("Import completed without any imported objects for source file: %s"), *SourceFile);
		return false;
	}

	const FString PrimaryObjectPath = ImportedArray[0]->AsString();

	if (bSyncBrowser)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(PrimaryObjectPath));
		if (!AssetData.IsValid())
		{
			if (UObject* ImportedAsset = UEditorAssetLibrary::LoadAsset(PrimaryObjectPath))
			{
				AssetData = FAssetData(ImportedAsset);
			}
		}

		if (AssetData.IsValid())
		{
			FContentBrowserModule& ContentBrowserModule =
				FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
			TArray<FAssetData> AssetsToSync;
			AssetsToSync.Add(AssetData);
			ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync);
		}
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("source_file"), SourceFile);
	ResultObj->SetStringField(TEXT("destination_path"), DestinationPath);
	ResultObj->SetStringField(TEXT("destination_name"), DestinationName);
	ResultObj->SetStringField(TEXT("primary_object_path"), PrimaryObjectPath);
	ResultObj->SetArrayField(TEXT("imported_object_paths"), ImportedArray);
	ResultObj->SetStringField(TEXT("message"), TEXT("Asset imported successfully."));
	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_VerifyViewportCaptureState(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const bool bEnsurePerspective = ParseOptionalBoolField(Input, TEXT("ensure_perspective"), true);
	const bool bEnsureRealtime = ParseOptionalBoolField(Input, TEXT("ensure_realtime"), true);
	const bool bFailIfUnready = ParseOptionalBoolField(Input, TEXT("fail_if_unready"), false);
	int32 WaitMs = 100;
	TryReadOptionalIntField(Input, TEXT("wait_ms"), WaitMs);
	WaitMs = FMath::Clamp(WaitMs, 0, 5000);

	FLevelEditorViewportClient* ViewportClient = nullptr;
	FViewport* Viewport = nullptr;
	TSharedPtr<FJsonObject> State = BuildViewportCaptureStateObject(
		bEnsurePerspective,
		bEnsureRealtime,
		WaitMs,
		ViewportClient,
		Viewport);
	State->SetStringField(TEXT("action"), TEXT("verify_viewport_capture_state"));
	State->SetBoolField(TEXT("dry_run"), false);
	State->SetBoolField(TEXT("fail_if_unready"), bFailIfUnready);
	if (bFailIfUnready && !State->GetBoolField(TEXT("ready_for_visual_capture")))
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VIEWPORT_NOT_READY"),
			State->GetStringField(TEXT("readiness_reason")),
			State);
	}
	OutResult = JsonObjToString(State);
	return true;
}

bool FVFXToolRegistry::Tool_CaptureViewport(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const bool bEnsurePerspective = ParseOptionalBoolField(Input, TEXT("ensure_perspective"), true);
	const bool bEnsureRealtime = ParseOptionalBoolField(Input, TEXT("ensure_realtime"), true);
	const bool bRequireReady = ParseOptionalBoolField(Input, TEXT("require_ready"), true);
	int32 WaitMs = 100;
	TryReadOptionalIntField(Input, TEXT("wait_ms"), WaitMs);
	WaitMs = FMath::Clamp(WaitMs, 0, 5000);

	FLevelEditorViewportClient* ViewportClient = nullptr;
	FViewport* Viewport = nullptr;
	TSharedPtr<FJsonObject> CaptureState = BuildViewportCaptureStateObject(
		bEnsurePerspective,
		bEnsureRealtime,
		WaitMs,
		ViewportClient,
		Viewport);
	if (!Viewport || !ViewportClient)
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VIEWPORT_NOT_READY"),
			CaptureState->GetStringField(TEXT("readiness_reason")),
			CaptureState);
	}
	if (bRequireReady && !CaptureState->GetBoolField(TEXT("ready_for_visual_capture")))
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VIEWPORT_NOT_READY"),
			CaptureState->GetStringField(TEXT("readiness_reason")),
			CaptureState);
	}

	// Read pixels from viewport
	TArray<FColor> Bitmap;
	int32 Width = Viewport->GetSizeXY().X;
	int32 Height = Viewport->GetSizeXY().Y;

	if (Width == 0 || Height == 0)
	{
		OutResult = TEXT("Viewport has zero size.");
		return false;
	}

	bool bReadSuccess = Viewport->ReadPixels(Bitmap);
	if (!bReadSuccess || Bitmap.Num() == 0)
	{
		OutResult = TEXT("Failed to read viewport pixels.");
		return false;
	}

	// Compress to PNG using ImageWrapper
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

	if (!ImageWrapper.IsValid())
	{
		OutResult = TEXT("Failed to create image wrapper.");
		return false;
	}

	// Convert FColor array to raw byte array (BGRA)
	TArray<uint8> RawData;
	RawData.SetNum(Bitmap.Num() * 4);
	for (int32 i = 0; i < Bitmap.Num(); i++)
	{
		RawData[i * 4 + 0] = Bitmap[i].B;
		RawData[i * 4 + 1] = Bitmap[i].G;
		RawData[i * 4 + 2] = Bitmap[i].R;
		RawData[i * 4 + 3] = Bitmap[i].A;
	}

	if (!ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), Width, Height, ERGBFormat::BGRA, 8))
	{
		OutResult = TEXT("Failed to set raw image data.");
		return false;
	}

	TArray<uint8> CompressedData;
	CompressedData = ImageWrapper->GetCompressed(85);

	if (CompressedData.Num() == 0)
	{
		OutResult = TEXT("Failed to compress image.");
		return false;
	}

	// Encode to base64
	FString Base64String = FBase64::Encode(CompressedData);

	// Return as JSON with base64 data (AI will use this for vision analysis)
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("message"), FString::Printf(TEXT("Viewport captured (%dx%d)"), Width, Height));
	Result->SetStringField(TEXT("image_base64"), Base64String);
	Result->SetStringField(TEXT("media_type"), TEXT("image/jpeg"));
	Result->SetObjectField(TEXT("capture_state"), CaptureState);
	OutResult = JsonObjToString(Result);

	return true;
}

bool FVFXToolRegistry::Tool_CaptureAssetThumbnail(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("THUMB_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("THUMB_ASSET_PATH_REQUIRED"), TEXT("Missing required field: asset_path."));
	}

	int32 Width = 512;
	int32 Height = 512;
	TryReadOptionalIntField(Input, TEXT("width"), Width);
	TryReadOptionalIntField(Input, TEXT("height"), Height);
	Width = FMath::Clamp(Width, 64, 2048);
	Height = FMath::Clamp(Height, 64, 2048);

	FString FileStem;
	Input->TryGetStringField(TEXT("filename"), FileStem);
	if (FileStem.IsEmpty())
	{
		const FString AssetName = FPackageName::ObjectPathToObjectName(AssetPath);
		FileStem = FString::Printf(TEXT("%s_thumbnail"), *AssetName);
	}
	FileStem = FileStem.Replace(TEXT("/"), TEXT("_")).Replace(TEXT("\\"), TEXT("_"));

	UObject* Asset = UEditorAssetLibrary::LoadAsset(AssetPath);
	if (!Asset)
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("THUMB_ASSET_LOAD_FAILED"),
			FString::Printf(TEXT("Could not load asset at path: %s"), *AssetPath));
	}

	FObjectThumbnail Thumbnail;
	ThumbnailTools::RenderThumbnail(
		Asset,
		static_cast<uint32>(Width),
		static_cast<uint32>(Height),
		ThumbnailTools::EThumbnailTextureFlushMode::NeverFlush,
		nullptr,
		&Thumbnail);

	const TArray<uint8>& ImageData = Thumbnail.GetUncompressedImageData();
	if (ImageData.Num() == 0 || Thumbnail.GetImageWidth() <= 0 || Thumbnail.GetImageHeight() <= 0)
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("THUMB_RENDER_FAILED"),
			FString::Printf(TEXT("Thumbnail render produced no image data for asset: %s"), *AssetPath));
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	if (!ImageWrapper.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("THUMB_IMAGE_WRAPPER_FAILED"), TEXT("Failed to create image wrapper."));
	}

	if (!ImageWrapper->SetRaw(
			ImageData.GetData(),
			ImageData.Num(),
			Thumbnail.GetImageWidth(),
			Thumbnail.GetImageHeight(),
			ERGBFormat::BGRA,
			8))
	{
		return BuildSceneToolError(OutResult, TEXT("THUMB_SET_RAW_FAILED"), TEXT("Failed to prepare thumbnail image data."));
	}

	const TArray64<uint8> CompressedData = ImageWrapper->GetCompressed(90);
	if (CompressedData.Num() == 0)
	{
		return BuildSceneToolError(OutResult, TEXT("THUMB_COMPRESS_FAILED"), TEXT("Failed to compress thumbnail image."));
	}

	const FString OutputDir = FPaths::Combine(
		FPaths::ProjectSavedDir(),
		TEXT("ForgePilot"),
		TEXT("AssetThumbnails"));
	IFileManager::Get().MakeDirectory(*OutputDir, true);
	const FString OutputPath = FPaths::Combine(OutputDir, FileStem + TEXT(".jpg"));
	if (!FFileHelper::SaveArrayToFile(CompressedData, *OutputPath))
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("THUMB_SAVE_FAILED"),
			FString::Printf(TEXT("Failed to save thumbnail to disk: %s"), *OutputPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("capture_asset_thumbnail"));
	Result->SetStringField(TEXT("asset_path"), AssetPath);
	Result->SetStringField(TEXT("output_path"), OutputPath);
	Result->SetNumberField(TEXT("width"), Thumbnail.GetImageWidth());
	Result->SetNumberField(TEXT("height"), Thumbnail.GetImageHeight());
	Result->SetStringField(TEXT("media_type"), TEXT("image/jpeg"));
	TArray<uint8> Base64Input;
	Base64Input.Append(CompressedData.GetData(), static_cast<int32>(CompressedData.Num()));
	Result->SetStringField(TEXT("image_base64"), FBase64::Encode(Base64Input));
	Result->SetStringField(TEXT("message"), TEXT("Asset thumbnail captured."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_StartPlayInViewport(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!GUnrealEd)
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Unreal editor is not available.\"}");
		return false;
	}

	TSharedPtr<FJsonObject> Result = BuildPIEStateObject();
	if (GUnrealEd->IsPlayingSessionInEditor())
	{
		Result->SetStringField(TEXT("message"), TEXT("A PIE session is already running."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	TSharedPtr<IAssetViewport> ActiveViewport = GetFirstActiveAssetViewport();
	if (!ActiveViewport.IsValid())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"No active level viewport available for PIE.\"}");
		return false;
	}

	bool bSimulate = false;
	bool bPlayFromCamera = false;
	if (Input.IsValid())
	{
		Input->TryGetBoolField(TEXT("simulate"), bSimulate);
		Input->TryGetBoolField(TEXT("play_from_camera"), bPlayFromCamera);
	}

	FRequestPlaySessionParams Params;
	Params.WorldType = bSimulate ? EPlaySessionWorldType::SimulateInEditor : EPlaySessionWorldType::PlayInEditor;
	Params.DestinationSlateViewport = ActiveViewport;

	if (bPlayFromCamera && !bSimulate)
	{
		Params.StartLocation = ActiveViewport->GetAssetViewportClient().GetViewLocation();
		Params.StartRotation = ActiveViewport->GetAssetViewportClient().GetViewRotation();
	}

	GUnrealEd->RequestPlaySession(Params);

	Result = BuildPIEStateObject();
	Result->SetStringField(TEXT("message"), TEXT("Requested Play In Editor in the active viewport."));
	Result->SetBoolField(TEXT("requested_start"), true);
	Result->SetBoolField(TEXT("simulate"), bSimulate);
	Result->SetBoolField(TEXT("play_from_camera"), bPlayFromCamera);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_StopPlaySession(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!GUnrealEd)
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Unreal editor is not available.\"}");
		return false;
	}

	TSharedPtr<FJsonObject> Result = BuildPIEStateObject();
	if (!GUnrealEd->IsPlayingSessionInEditor())
	{
		Result->SetStringField(TEXT("message"), TEXT("No PIE session is currently running."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	GUnrealEd->RequestEndPlayMap();
	Result = BuildPIEStateObject();
	Result->SetStringField(TEXT("message"), TEXT("Requested end play map."));
	Result->SetBoolField(TEXT("requested_stop"), true);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetPIESessionState(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	TSharedPtr<FJsonObject> Result = BuildPIEStateObject();
	Result->SetStringField(TEXT("message"), TEXT("PIE state snapshot."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SendPIEKeyInput(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Input payload is required.\"}");
		return false;
	}

	if (!GUnrealEd || !GUnrealEd->IsPlayingSessionInEditor())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"PIE is not currently running.\"}");
		return false;
	}

	FString RequestedKeyName;
	if (!Input->TryGetStringField(TEXT("key"), RequestedKeyName) || RequestedKeyName.IsEmpty())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Field 'key' is required.\"}");
		return false;
	}

	FString EventName = TEXT("tap");
	Input->TryGetStringField(TEXT("event"), EventName);
	EventName = EventName.TrimStartAndEnd().ToLower();

	double AmountValue = 1.0;
	Input->TryGetNumberField(TEXT("amount"), AmountValue);
	const float Amount = static_cast<float>(AmountValue);

	FKey Key;
	FString ResolvedKeyName;
	if (!TryResolveKeyByName(RequestedKeyName, Key, ResolvedKeyName))
	{
		OutResult = FString::Printf(
			TEXT("{\"success\":false,\"error\":\"Unable to resolve key '%s'.\"}"),
			*RequestedKeyName);
		return false;
	}

	TArray<EInputEvent> EventsToSend;
	if (EventName.IsEmpty() || EventName == TEXT("tap"))
	{
		EventsToSend = { IE_Pressed, IE_Released };
	}
	else if (EventName == TEXT("pressed") || EventName == TEXT("press"))
	{
		EventsToSend = { IE_Pressed };
	}
	else if (EventName == TEXT("released") || EventName == TEXT("release"))
	{
		EventsToSend = { IE_Released };
	}
	else if (EventName == TEXT("repeat"))
	{
		EventsToSend = { IE_Repeat };
	}
	else
	{
		OutResult = FString::Printf(
			TEXT("{\"success\":false,\"error\":\"Unsupported event '%s'. Use tap, pressed, released, or repeat.\"}"),
			*EventName);
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> SentEvents;
	for (const EInputEvent EventType : EventsToSend)
	{
		FString EventError;
		const bool bHandled = SendPIEKeyEventToActiveViewport(Key, EventType, Amount, EventError);

		TSharedPtr<FJsonObject> EventObject = MakeShareable(new FJsonObject());
		EventObject->SetStringField(TEXT("event"), InputEventToString(EventType));
		EventObject->SetBoolField(TEXT("handled"), bHandled);
		if (!EventError.IsEmpty())
		{
			EventObject->SetStringField(TEXT("detail"), EventError);
		}
		SentEvents.Add(MakeShareable(new FJsonValueObject(EventObject)));
	}

	TSharedPtr<FJsonObject> Result = BuildPIEStateObject();
	Result->SetStringField(TEXT("message"), TEXT("Key input dispatched to the active PIE viewport."));
	Result->SetStringField(TEXT("key"), ResolvedKeyName);
	Result->SetStringField(TEXT("requested_key"), RequestedKeyName);
	Result->SetStringField(TEXT("requested_event"), EventName);
	Result->SetNumberField(TEXT("amount"), Amount);
	Result->SetArrayField(TEXT("events"), SentEvents);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RunPIEProbeScenario(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Input payload is required.\"}");
		return false;
	}

	FString ScenarioName;
	if (!Input->TryGetStringField(TEXT("scenario_name"), ScenarioName) || ScenarioName.IsEmpty())
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Field 'scenario_name' is required.\"}");
		return false;
	}

	auto ReadOptionalBool = [&Input](const TCHAR* FieldName, bool DefaultValue) -> bool
	{
		bool BoolValue = false;
		if (Input->TryGetBoolField(FieldName, BoolValue))
		{
			return BoolValue;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue))
		{
			StringValue = StringValue.TrimStartAndEnd().ToLower();
			if (StringValue == TEXT("true") || StringValue == TEXT("1") || StringValue == TEXT("yes"))
			{
				return true;
			}
			if (StringValue == TEXT("false") || StringValue == TEXT("0") || StringValue == TEXT("no"))
			{
				return false;
			}
		}

		return DefaultValue;
	};

	const bool bIncludeLogScan = ReadOptionalBool(TEXT("include_log_scan"), true);
	const bool bAbortOnFailure = ReadOptionalBool(TEXT("abort_on_failure"), true);

	TSharedPtr<FJsonObject> ScenarioDefinition;
	FString ScenarioError;
	if (!LoadPIEProbeScenarioDefinition(ScenarioName, ScenarioDefinition, ScenarioError))
	{
		OutResult = FString::Printf(TEXT("{\"success\":false,\"error\":\"%s\"}"), *ScenarioError.ReplaceCharWithEscapedChar());
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* StepsArray = nullptr;
	if (!ScenarioDefinition->TryGetArrayField(TEXT("steps"), StepsArray) || StepsArray == nullptr || StepsArray->Num() == 0)
	{
		OutResult = FString::Printf(TEXT("{\"success\":false,\"error\":\"Scenario '%s' has no steps.\"}"), *ScenarioName);
		return false;
	}

	FString OutputDir;
	Input->TryGetStringField(TEXT("output_dir"), OutputDir);
	if (OutputDir.IsEmpty())
	{
		OutputDir = GetDefaultProbeOutputDir(ScenarioName);
	}
	IFileManager::Get().MakeDirectory(*OutputDir, true);

	TSharedPtr<FJsonObject> Summary = MakeShareable(new FJsonObject());
	Summary->SetBoolField(TEXT("success"), true);
	Summary->SetStringField(TEXT("scenario_name"), ScenarioName);
	Summary->SetStringField(TEXT("scenario_config_path"), GetForgePilotScenarioConfigPath());
	Summary->SetStringField(TEXT("output_dir"), OutputDir);

	FString ScenarioDescription;
	ScenarioDefinition->TryGetStringField(TEXT("description"), ScenarioDescription);
	if (!ScenarioDescription.IsEmpty())
	{
		Summary->SetStringField(TEXT("description"), ScenarioDescription);
	}

	TArray<TSharedPtr<FJsonValue>> StepResults;
	TArray<TSharedPtr<FJsonValue>> CaptureResults;
	bool bAllStepsSucceeded = true;

	for (int32 StepIndex = 0; StepIndex < StepsArray->Num(); ++StepIndex)
	{
		const TSharedPtr<FJsonValue>& StepValue = (*StepsArray)[StepIndex];
		if (!StepValue.IsValid() || StepValue->Type != EJson::Object)
		{
			continue;
		}

		const TSharedPtr<FJsonObject> StepObject = StepValue->AsObject();
		TSharedPtr<FJsonObject> StepResult = MakeShareable(new FJsonObject());
		StepResult->SetNumberField(TEXT("index"), StepIndex);

		FString StepType;
		StepObject->TryGetStringField(TEXT("type"), StepType);
		StepResult->SetStringField(TEXT("type"), StepType);

		bool bStepSucceeded = true;
		FString StepError;

		if (StepType == TEXT("wait"))
		{
			int32 WaitMs = 0;
			TryReadOptionalIntField(StepObject, TEXT("ms"), WaitMs);
			WaitMs = FMath::Max(0, WaitMs);
			StepResult->SetNumberField(TEXT("wait_ms"), WaitMs);
			FPlatformProcess::Sleep(static_cast<float>(WaitMs) / 1000.0f);
			StepResult->SetStringField(TEXT("message"), TEXT("Completed wait step."));
		}
		else if (StepType == TEXT("tool"))
		{
			FString ToolName;
			StepObject->TryGetStringField(TEXT("name"), ToolName);
			StepResult->SetStringField(TEXT("tool_name"), ToolName);

			if (ToolName.IsEmpty() || ToolName == TEXT("run_pie_probe_scenario"))
			{
				bStepSucceeded = false;
				StepError = TEXT("Scenario step references an invalid or recursive tool name.");
			}
			else
			{
				TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
				const TSharedPtr<FJsonObject>* InlineInput = nullptr;
				if (StepObject->TryGetObjectField(TEXT("input"), InlineInput) && InlineInput != nullptr && InlineInput->IsValid())
				{
					ToolInput = *InlineInput;
				}

				FString ToolResultString;
				bStepSucceeded = ExecuteTool(ToolName, ToolInput, ToolResultString);

				TSharedPtr<FJsonObject> ParsedToolResult;
				const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ToolResultString);
				if (FJsonSerializer::Deserialize(Reader, ParsedToolResult) && ParsedToolResult.IsValid())
				{
					if (ToolName == TEXT("start_play_in_viewport"))
					{
						int32 WaitedMs = 0;
						for (; WaitedMs < 10000; WaitedMs += 100)
						{
							FPlatformProcess::Sleep(0.1f);
							TSharedPtr<FJsonObject> PIEState = BuildPIEStateObject();
							bool bIsPlaying = false;
							PIEState->TryGetBoolField(TEXT("is_playing_in_editor"), bIsPlaying);
							if (bIsPlaying)
							{
								ParsedToolResult->SetObjectField(TEXT("post_wait_pie_state"), PIEState);
								break;
							}
						}

						ParsedToolResult->SetNumberField(TEXT("waited_for_pie_ms"), WaitedMs);
					}
					else if (ToolName == TEXT("stop_play_session"))
					{
						int32 WaitedMs = 0;
						for (; WaitedMs < 5000; WaitedMs += 100)
						{
							FPlatformProcess::Sleep(0.1f);
							TSharedPtr<FJsonObject> PIEState = BuildPIEStateObject();
							bool bIsPlaying = false;
							PIEState->TryGetBoolField(TEXT("is_playing_in_editor"), bIsPlaying);
							if (!bIsPlaying)
							{
								ParsedToolResult->SetObjectField(TEXT("post_wait_pie_state"), PIEState);
								break;
							}
						}

						ParsedToolResult->SetNumberField(TEXT("waited_for_stop_ms"), WaitedMs);
					}

					FString CaptureLabel;
					StepObject->TryGetStringField(TEXT("capture_label"), CaptureLabel);

					FString Base64Payload;
					if (!CaptureLabel.IsEmpty() && ParsedToolResult->TryGetStringField(TEXT("image_base64"), Base64Payload) && !Base64Payload.IsEmpty())
					{
						FString MediaType = TEXT("image/jpeg");
						ParsedToolResult->TryGetStringField(TEXT("media_type"), MediaType);
						const FString Extension = MediaType.Contains(TEXT("png")) ? TEXT("png") : TEXT("jpg");
						const FString OutputPath = FPaths::Combine(OutputDir, FString::Printf(TEXT("%s.%s"), *SanitizeScenarioToken(CaptureLabel), *Extension));

						FString CaptureError;
						if (SaveCapturePayloadToFile(Base64Payload, OutputPath, CaptureError))
						{
							ParsedToolResult->RemoveField(TEXT("image_base64"));
							ParsedToolResult->SetStringField(TEXT("saved_capture_path"), OutputPath);

							TSharedPtr<FJsonObject> CaptureObject = MakeShareable(new FJsonObject());
							CaptureObject->SetStringField(TEXT("label"), CaptureLabel);
							CaptureObject->SetStringField(TEXT("path"), OutputPath);
							CaptureObject->SetStringField(TEXT("media_type"), MediaType);
							CaptureResults.Add(MakeShareable(new FJsonValueObject(CaptureObject)));
						}
						else
						{
							bStepSucceeded = false;
							StepError = CaptureError;
						}
					}

					StepResult->SetObjectField(TEXT("result"), ParsedToolResult);
				}
				else
				{
					StepResult->SetStringField(TEXT("raw_result"), ToolResultString);
				}
			}
		}
		else
		{
			bStepSucceeded = false;
			StepError = FString::Printf(TEXT("Unsupported scenario step type '%s'."), *StepType);
		}

		StepResult->SetBoolField(TEXT("success"), bStepSucceeded);
		if (!StepError.IsEmpty())
		{
			StepResult->SetStringField(TEXT("error"), StepError);
		}

		StepResults.Add(MakeShareable(new FJsonValueObject(StepResult)));
		bAllStepsSucceeded &= bStepSucceeded;

		if (!bStepSucceeded && bAbortOnFailure)
		{
			break;
		}
	}

	Summary->SetBoolField(TEXT("success"), bAllStepsSucceeded);
	Summary->SetArrayField(TEXT("steps"), StepResults);
	Summary->SetArrayField(TEXT("captures"), CaptureResults);
	Summary->SetObjectField(TEXT("final_pie_state"), BuildPIEStateObject());

	if (bIncludeLogScan)
	{
		Summary->SetObjectField(TEXT("latest_log_summary"), BuildLatestPIELogSummary());
	}

	const FString SummaryPath = FPaths::Combine(OutputDir, TEXT("summary.json"));
	FFileHelper::SaveStringToFile(JsonObjToString(Summary), *SummaryPath);
	Summary->SetStringField(TEXT("summary_path"), SummaryPath);

	OutResult = JsonObjToString(Summary);
	return bAllStepsSucceeded;
}

bool FVFXToolRegistry::Tool_InspectSkeletalAttachmentContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	FString ActorLabel;
	FString ComponentName;
	FString SocketName;
	FString BoneName;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("actor_label"), ActorLabel);
		Input->TryGetStringField(TEXT("component_name"), ComponentName);
		Input->TryGetStringField(TEXT("socket_name"), SocketName);
		Input->TryGetStringField(TEXT("bone_name"), BoneName);
	}

	const bool bUseSelectedActor = ParseOptionalBoolField(Input, TEXT("use_selected_actor"), true);
	const bool bIncludeAllSocketNames = ParseOptionalBoolField(Input, TEXT("include_all_socket_names"), true);
	const bool bIncludeAllBoneNames = ParseOptionalBoolField(Input, TEXT("include_all_bone_names"), false);

	AActor* TargetActor = nullptr;
	if (!ActorLabel.IsEmpty())
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (AActor* Actor = *It; Actor && Actor->GetActorLabel() == ActorLabel)
			{
				TargetActor = Actor;
				break;
			}
		}
	}

	if (!TargetActor && bUseSelectedActor && GEditor)
	{
		if (USelection* Selection = GEditor->GetSelectedActors())
		{
			TargetActor = Cast<AActor>(Selection->GetSelectedObject(0));
		}
	}

	if (!TargetActor)
	{
		OutResult = ActorLabel.IsEmpty()
			? TEXT("No target actor found. Provide actor_label or select an actor in the level.")
			: FString::Printf(TEXT("Actor '%s' was not found in the current level."), *ActorLabel);
		return false;
	}

	auto MakeVectorObject = [](const FVector& Vector)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetNumberField(TEXT("x"), Vector.X);
		Obj->SetNumberField(TEXT("y"), Vector.Y);
		Obj->SetNumberField(TEXT("z"), Vector.Z);
		return Obj;
	};
	auto MakeRotatorObject = [](const FRotator& Rotator)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetNumberField(TEXT("pitch"), Rotator.Pitch);
		Obj->SetNumberField(TEXT("yaw"), Rotator.Yaw);
		Obj->SetNumberField(TEXT("roll"), Rotator.Roll);
		return Obj;
	};
	auto MakeTransformObject = [&](const FTransform& Transform)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetObjectField(TEXT("location"), MakeVectorObject(Transform.GetLocation()));
		Obj->SetObjectField(TEXT("rotation"), MakeRotatorObject(Transform.Rotator()));
		Obj->SetObjectField(TEXT("scale"), MakeVectorObject(Transform.GetScale3D()));
		return Obj;
	};

	TArray<USkeletalMeshComponent*> SkeletalComponents;
	TargetActor->GetComponents(SkeletalComponents);

	TArray<TSharedPtr<FJsonValue>> ComponentArray;
	bool bFocusedComponentFound = false;
	for (USkeletalMeshComponent* SkeletalComponent : SkeletalComponents)
	{
		if (!SkeletalComponent)
		{
			continue;
		}
		if (!ComponentName.IsEmpty() && !SkeletalComponent->GetName().Equals(ComponentName, ESearchCase::IgnoreCase))
		{
			continue;
		}

		bFocusedComponentFound = true;
		TSharedPtr<FJsonObject> CompObj = MakeShareable(new FJsonObject());
		CompObj->SetStringField(TEXT("component_name"), SkeletalComponent->GetName());
		CompObj->SetStringField(TEXT("component_class"), SkeletalComponent->GetClass()->GetName());
		CompObj->SetStringField(TEXT("skeletal_mesh_path"), SkeletalComponent->GetSkeletalMeshAsset() ? SkeletalComponent->GetSkeletalMeshAsset()->GetPathName() : TEXT(""));
		CompObj->SetObjectField(TEXT("component_transform"), MakeTransformObject(SkeletalComponent->GetComponentTransform()));

		TArray<FName> SocketNames = SkeletalComponent->GetAllSocketNames();
		if (bIncludeAllSocketNames)
		{
			TArray<TSharedPtr<FJsonValue>> SocketArray;
			for (const FName& Name : SocketNames)
			{
				SocketArray.Add(MakeShareable(new FJsonValueString(Name.ToString())));
			}
			CompObj->SetArrayField(TEXT("socket_names"), SocketArray);
		}
		CompObj->SetNumberField(TEXT("socket_count"), SocketNames.Num());

		if (bIncludeAllBoneNames)
		{
			TArray<TSharedPtr<FJsonValue>> BoneArray;
			const int32 BoneCount = SkeletalComponent->GetNumBones();
			for (int32 BoneIndex = 0; BoneIndex < BoneCount; ++BoneIndex)
			{
				BoneArray.Add(MakeShareable(new FJsonValueString(SkeletalComponent->GetBoneName(BoneIndex).ToString())));
			}
			CompObj->SetArrayField(TEXT("bone_names"), BoneArray);
		}
		CompObj->SetNumberField(TEXT("bone_count"), SkeletalComponent->GetNumBones());

		if (!SocketName.IsEmpty() && SkeletalComponent->DoesSocketExist(FName(*SocketName)))
		{
			TSharedPtr<FJsonObject> FocusObj = MakeShareable(new FJsonObject());
			FocusObj->SetStringField(TEXT("socket_name"), SocketName);
			FocusObj->SetObjectField(TEXT("component_transform"), MakeTransformObject(SkeletalComponent->GetSocketTransform(FName(*SocketName), RTS_Component)));
			FocusObj->SetObjectField(TEXT("world_transform"), MakeTransformObject(SkeletalComponent->GetSocketTransform(FName(*SocketName), RTS_World)));
			CompObj->SetObjectField(TEXT("focused_socket"), FocusObj);
		}

		if (!BoneName.IsEmpty())
		{
			const int32 BoneIndex = SkeletalComponent->GetBoneIndex(FName(*BoneName));
			if (BoneIndex != INDEX_NONE)
			{
				TSharedPtr<FJsonObject> FocusObj = MakeShareable(new FJsonObject());
				FocusObj->SetStringField(TEXT("bone_name"), BoneName);
				FocusObj->SetNumberField(TEXT("bone_index"), BoneIndex);
				FocusObj->SetObjectField(TEXT("component_transform"), MakeTransformObject(SkeletalComponent->GetBoneTransform(BoneIndex)));
				CompObj->SetObjectField(TEXT("focused_bone"), FocusObj);
			}
		}

		ComponentArray.Add(MakeShareable(new FJsonValueObject(CompObj)));
	}

	if (!bFocusedComponentFound)
	{
		OutResult = ComponentName.IsEmpty()
			? FString::Printf(TEXT("Actor '%s' has no skeletal mesh components."), *TargetActor->GetActorLabel())
			: FString::Printf(TEXT("Skeletal component '%s' was not found on actor '%s'."), *ComponentName, *TargetActor->GetActorLabel());
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("inspect_skeletal_attachment_context"));
	Result->SetStringField(TEXT("actor_label"), TargetActor->GetActorLabel());
	Result->SetStringField(TEXT("actor_name"), TargetActor->GetName());
	Result->SetStringField(TEXT("actor_class"), TargetActor->GetClass()->GetName());
	Result->SetArrayField(TEXT("skeletal_components"), ComponentArray);
	Result->SetNumberField(TEXT("skeletal_component_count"), ComponentArray.Num());
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_LoadVFXRecipe(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString RequestedEffectType;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("effect_type"), RequestedEffectType) || RequestedEffectType.TrimStartAndEnd().IsEmpty())
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_RECIPE_EFFECT_TYPE_REQUIRED"));
		ErrorObj->SetStringField(TEXT("error_message"), TEXT("Field 'effect_type' is required."));
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	TSharedPtr<FJsonObject> Bundle;
	FString CanonicalEffectType;
	FString ErrorMessage;
	TArray<FString> AvailableRecipes;
	if (!FVFXKnowledgeLoader::LoadRecipeBundle(RequestedEffectType, Bundle, CanonicalEffectType, ErrorMessage, &AvailableRecipes))
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_RECIPE_NOT_FOUND"));
		ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
		TArray<TSharedPtr<FJsonValue>> AvailableValues;
		for (const FString& RecipeName : AvailableRecipes)
		{
			AvailableValues.Add(MakeShared<FJsonValueString>(RecipeName));
		}
		ErrorObj->SetArrayField(TEXT("available_recipes"), AvailableValues);
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> AvailableValues;
	for (const FString& RecipeName : AvailableRecipes)
	{
		AvailableValues.Add(MakeShared<FJsonValueString>(RecipeName));
	}
	Bundle->SetArrayField(TEXT("available_recipes"), AvailableValues);

	OutResult = JsonObjToString(Bundle);
	return true;
}

bool FVFXToolRegistry::Tool_ResolveEffectPlan(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString Description;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("description"), Description) || Description.TrimStartAndEnd().IsEmpty())
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_EFFECT_DESCRIPTION_REQUIRED"));
		ErrorObj->SetStringField(TEXT("error_message"), TEXT("Field 'description' is required."));
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	TSharedPtr<FJsonObject> Plan;
	FString ErrorMessage;
	TArray<FString> AvailableRecipes;
	if (!FVFXKnowledgeLoader::ResolveEffectPlan(Description, Plan, ErrorMessage, &AvailableRecipes))
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_EFFECT_PLAN_UNRESOLVED"));
		ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
		TArray<TSharedPtr<FJsonValue>> AvailableValues;
		for (const FString& RecipeName : AvailableRecipes)
		{
			AvailableValues.Add(MakeShared<FJsonValueString>(RecipeName));
		}
		ErrorObj->SetArrayField(TEXT("available_recipes"), AvailableValues);
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	OutResult = JsonObjToString(Plan);
	return true;
}

bool FVFXToolRegistry::Tool_ResolveEffectPlanV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString Description;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("description"), Description) || Description.TrimStartAndEnd().IsEmpty())
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_EFFECT_DESCRIPTION_REQUIRED"));
		ErrorObj->SetStringField(TEXT("error_message"), TEXT("Field 'description' is required."));
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	TSharedPtr<FJsonObject> Plan;
	FString ErrorMessage;
	TArray<FString> AvailableRecipes;
	if (!FVFXKnowledgeLoader::ResolveEffectPlanV2(Description, Plan, ErrorMessage, &AvailableRecipes))
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("VFX_EFFECT_PLAN_V2_UNRESOLVED"));
		ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
		TArray<TSharedPtr<FJsonValue>> AvailableValues;
		for (const FString& RecipeName : AvailableRecipes)
		{
			AvailableValues.Add(MakeShared<FJsonValueString>(RecipeName));
		}
		ErrorObj->SetArrayField(TEXT("available_recipes"), AvailableValues);
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	OutResult = JsonObjToString(Plan);
	return true;
}

bool FVFXToolRegistry::Tool_GenerateVisualContractDataAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_VISUAL_CONTRACT_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString ArchetypeName;
	if (!Input->TryGetStringField(TEXT("archetype"), ArchetypeName) || ArchetypeName.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_VISUAL_CONTRACT_ARCHETYPE_REQUIRED"), TEXT("Field 'archetype' is required."));
	}

	FString AssetName;
	Input->TryGetStringField(TEXT("asset_name"), AssetName);
	if (AssetName.IsEmpty())
	{
		AssetName = MakeVisualContractAssetName(ArchetypeName);
	}

	FString SavePath = TEXT("/Game/ForgePilot_Generated/VisualContracts");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	SavePath = SavePath.TrimStartAndEnd();
	if (!SavePath.StartsWith(TEXT("/")))
	{
		SavePath = TEXT("/") + SavePath;
	}
	if (SavePath.EndsWith(TEXT("/")))
	{
		SavePath.LeftChopInline(1, false);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FString PackagePath = SavePath / AssetName;

	TSharedPtr<FJsonObject> VisualContract;
	FString ErrorMessage;
	if (!FVFXKnowledgeLoader::LoadV2VisualContract(ArchetypeName, VisualContract, ErrorMessage))
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_VISUAL_CONTRACT_LOAD_FAILED"), ErrorMessage);
	}

	int32 TotalBindingCount = 0;
	const TSharedPtr<FJsonObject>* OpBindingsPtr = nullptr;
	if (VisualContract->TryGetObjectField(TEXT("op_bindings"), OpBindingsPtr) && OpBindingsPtr && (*OpBindingsPtr).IsValid())
	{
		for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : (*OpBindingsPtr)->Values)
		{
			if (!Pair.Value.IsValid() || Pair.Value->Type != EJson::Object)
			{
				continue;
			}

			const TArray<TSharedPtr<FJsonValue>>* Bindings = nullptr;
			if (Pair.Value->AsObject()->TryGetArrayField(TEXT("bindings"), Bindings) && Bindings)
			{
				TotalBindingCount += Bindings->Num();
			}
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("generate_visual_contract_data_asset"));
	Result->SetStringField(TEXT("archetype"), ArchetypeName);
	Result->SetStringField(TEXT("save_path"), SavePath);
	Result->SetStringField(TEXT("asset_name"), AssetName);
	Result->SetStringField(TEXT("planned_asset_path"), PackagePath);
	Result->SetNumberField(TEXT("op_binding_count"), OpBindingsPtr && (*OpBindingsPtr).IsValid() ? (*OpBindingsPtr)->Values.Num() : 0);
	Result->SetNumberField(TEXT("binding_count"), TotalBindingCount);
	Result->SetObjectField(TEXT("visual_contract"), VisualContract);

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetBoolField(TEXT("asset_already_exists"), FPackageName::DoesPackageExist(PackagePath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VFX_VISUAL_CONTRACT_ASSET_EXISTS"),
			FString::Printf(TEXT("Asset already exists at '%s'."), *PackagePath));
	}

	UPackage* Package = CreatePackage(*PackagePath);
	if (!Package)
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VFX_VISUAL_CONTRACT_PACKAGE_CREATE_FAILED"),
			FString::Printf(TEXT("Could not create package '%s'."), *PackagePath));
	}

	UVFXVisualContractDataAsset* DataAsset = NewObject<UVFXVisualContractDataAsset>(
		Package,
		UVFXVisualContractDataAsset::StaticClass(),
		FName(*AssetName),
		RF_Public | RF_Standalone);
	if (!DataAsset)
	{
		return BuildSceneToolError(
			OutResult,
			TEXT("VFX_VISUAL_CONTRACT_ASSET_CREATE_FAILED"),
			FString::Printf(TEXT("Could not create data asset '%s'."), *AssetName));
	}

	PopulateVisualContractDataAsset(DataAsset, ArchetypeName, VisualContract);
	FAssetRegistryModule::AssetCreated(DataAsset);
	DataAsset->MarkPackageDirty();
	Package->MarkPackageDirty();

	const bool bSaveSucceeded = UEditorAssetLibrary::SaveLoadedAsset(DataAsset, true);
	if (!bSaveSucceeded)
	{
		TArray<UObject*> ObjectsToDelete;
		ObjectsToDelete.Add(DataAsset);
		ObjectTools::DeleteObjectsUnchecked(ObjectsToDelete);
		ResetLoaders(Package);
		Package->SetDirtyFlag(false);
		Package->MarkAsGarbage();
	}

	Result->SetBoolField(TEXT("success"), bSaveSucceeded);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("asset_path"), DataAsset->GetPathName());
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	Result->SetStringField(
		TEXT("message"),
		bSaveSucceeded
			? FString::Printf(TEXT("Generated visual contract data asset '%s' for archetype '%s'."), *DataAsset->GetPathName(), *ArchetypeName)
			: FString::Printf(TEXT("Generated visual contract data asset '%s' but save failed."), *PackagePath));

	OutResult = JsonObjToString(Result);
	return bSaveSucceeded;
}

bool FVFXToolRegistry::Tool_ReviewEffectFailureTaxonomyV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	return Tool_ReviewSuctionCoreFailureTaxonomy(Input, OutResult);
}

bool FVFXToolRegistry::Tool_ReviewSuctionCoreFailureTaxonomy(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_REVIEW_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString Archetype = TEXT("suction_core_energy");
	Input->TryGetStringField(TEXT("archetype"), Archetype);

	FV2ArchetypeSceneConfig ArchetypeConfig;
	if (!GetV2ArchetypeSceneConfig(Archetype, ArchetypeConfig))
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_EFFECT_V2_REVIEW_ARCHETYPE_UNSUPPORTED"),
			FString::Printf(TEXT("Unsupported V2 archetype '%s'."), *Archetype));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_REVIEW_SYSTEM_REQUIRED"), TEXT("Field 'system_path' is required."));
	}

	FString MainEmitter = ArchetypeConfig.HeroEmitter;
	FString SecondaryEmitter = ArchetypeConfig.SecondaryEmitter;
	FString CoreEmitter = ArchetypeConfig.CoreEmitter;
	FString SparksEmitter = ArchetypeConfig.SparksEmitter;
	FString ShockGlowEmitter = ArchetypeConfig.GlowEmitter;
	Input->TryGetStringField(TEXT("main_layer_emitter_name"), MainEmitter);
	Input->TryGetStringField(TEXT("secondary_layer_emitter_name"), SecondaryEmitter);
	Input->TryGetStringField(TEXT("core_layer_emitter_name"), CoreEmitter);
	Input->TryGetStringField(TEXT("sparks_layer_emitter_name"), SparksEmitter);
	Input->TryGetStringField(TEXT("shock_glow_emitter_name"), ShockGlowEmitter);

	int32 ConsecutiveMotionFailures = 0;
	TryReadOptionalIntField(Input, TEXT("consecutive_layer_a_motion_failures"), ConsecutiveMotionFailures);

	TSharedPtr<FJsonObject> GetSystemInput = MakeShareable(new FJsonObject());
	GetSystemInput->SetStringField(TEXT("system_path"), SystemPath);
	FString SystemInfoJson;
	if (!ExecuteTool(TEXT("get_niagara_system_info"), GetSystemInput, SystemInfoJson))
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_REVIEW_SYSTEM_INFO_FAILED"), TEXT("Failed to inspect Niagara System info."));
	}

	TSharedPtr<FJsonObject> SystemInfo;
	if (!ParseToolJsonResult(SystemInfoJson, SystemInfo))
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_REVIEW_SYSTEM_INFO_PARSE_FAILED"), TEXT("Could not parse get_niagara_system_info result."));
	}

	TMap<FString, TSharedPtr<FJsonObject>> EmitterByName;
	const TArray<TSharedPtr<FJsonValue>>* Emitters = nullptr;
	if (SystemInfo->TryGetArrayField(TEXT("emitters"), Emitters) && Emitters)
	{
		for (const TSharedPtr<FJsonValue>& EmitterValue : *Emitters)
		{
			const TSharedPtr<FJsonObject> EmitterObject = EmitterValue.IsValid() ? EmitterValue->AsObject() : nullptr;
			FString EmitterName;
			if (EmitterObject.IsValid() && EmitterObject->TryGetStringField(TEXT("name"), EmitterName))
			{
				EmitterByName.Add(EmitterName, EmitterObject);
			}
		}
	}

	TMap<FString, TSharedPtr<FJsonObject>> FindingsByLabel;
	auto HasEmitterNamed = [&EmitterByName](const FString& Name) -> bool
	{
		return EmitterByName.Contains(Name);
	};

	if (!HasEmitterNamed(MainEmitter))
	{
		AddFailureLabel(FindingsByLabel, TEXT("hero_sweep_missing"), FString::Printf(TEXT("%s is missing."), *MainEmitter), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
		AddFailureLabel(FindingsByLabel, TEXT("no_outside_to_core_read"), TEXT("Without the hero layer there is no readable outer-to-core flow."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
	}
	if (!HasEmitterNamed(SecondaryEmitter) || !HasEmitterNamed(CoreEmitter) || !HasEmitterNamed(SparksEmitter) || !HasEmitterNamed(ShockGlowEmitter))
	{
		AddFailureLabel(FindingsByLabel, TEXT("support_fill_missing"), TEXT("One or more support layers are missing from the V2 orb stack."), TEXT("medium"), false, TEXT("build_core_turbulence_layer"));
	}
	if (!HasEmitterNamed(CoreEmitter) || !HasEmitterNamed(ShockGlowEmitter))
	{
		AddFailureLabel(FindingsByLabel, TEXT("inner_hot_core_missing"), TEXT("Core mass or bind glow is missing, so the orb center cannot bind correctly."), TEXT("high"), false, TEXT("build_core_merge_glow_material"));
	}

	auto ReviewEmitterStack = [this, &SystemPath](const FString& EmitterName, TSharedPtr<FJsonObject>& OutStackObject) -> bool
	{
		TSharedPtr<FJsonObject> StackInput = MakeShareable(new FJsonObject());
		StackInput->SetStringField(TEXT("system_path"), SystemPath);
		StackInput->SetStringField(TEXT("emitter_name"), EmitterName);
		StackInput->SetBoolField(TEXT("include_inputs"), true);
		FString StackJson;
		if (!ExecuteTool(TEXT("get_niagara_emitter_stack"), StackInput, StackJson))
		{
			return false;
		}
		return ParseToolJsonResult(StackJson, OutStackObject);
	};

	auto HasStandardModule = [](const TSharedPtr<FJsonObject>& StackObject, const FString& FieldName) -> bool
	{
		if (!StackObject.IsValid())
		{
			return false;
		}
		const TSharedPtr<FJsonObject>* PresencePtr = nullptr;
		if (!StackObject->TryGetObjectField(TEXT("standard_module_presence"), PresencePtr) || !PresencePtr || !(*PresencePtr).IsValid())
		{
			return false;
		}
		bool bEnabled = false;
		return TryGetBoolFromValue((*PresencePtr)->TryGetField(FieldName), bEnabled) && bEnabled;
	};

	auto StackContainsNodeClass = [](const TSharedPtr<FJsonObject>& StackObject, const FString& NodeClass) -> bool
	{
		if (!StackObject.IsValid())
		{
			return false;
		}

		const TArray<TSharedPtr<FJsonValue>>* Stages = nullptr;
		if (!StackObject->TryGetArrayField(TEXT("stages"), Stages) || !Stages)
		{
			return false;
		}

		for (const TSharedPtr<FJsonValue>& StageValue : *Stages)
		{
			const TSharedPtr<FJsonObject> StageObject = StageValue.IsValid() ? StageValue->AsObject() : nullptr;
			if (!StageObject.IsValid())
			{
				continue;
			}

			const TArray<TSharedPtr<FJsonValue>>* Modules = nullptr;
			if (!StageObject->TryGetArrayField(TEXT("modules"), Modules) || !Modules)
			{
				continue;
			}

			for (const TSharedPtr<FJsonValue>& ModuleValue : *Modules)
			{
				const TSharedPtr<FJsonObject> ModuleObject = ModuleValue.IsValid() ? ModuleValue->AsObject() : nullptr;
				FString CurrentNodeClass;
				if (ModuleObject.IsValid() &&
					ModuleObject->TryGetStringField(TEXT("node_class"), CurrentNodeClass) &&
					CurrentNodeClass.Equals(NodeClass, ESearchCase::IgnoreCase))
				{
					return true;
				}
			}
		}

		return false;
	};

	TSharedPtr<FJsonObject> MainStack;
	if (HasEmitterNamed(MainEmitter) && ReviewEmitterStack(MainEmitter, MainStack))
	{
		const bool bHasAddVelocity = HasStandardModule(MainStack, TEXT("add_velocity"));
		const bool bHasPointAttraction = HasStandardModule(MainStack, TEXT("point_attraction_force"));
		const bool bHasVortexForce = HasStandardModule(MainStack, TEXT("vortex_force"));
		const bool bHasVortexVelocity = HasStandardModule(MainStack, TEXT("vortex_velocity"));
		const bool bHasConeVelocity = HasStandardModule(MainStack, TEXT("add_velocity_in_cone"));
		const bool bHasSphereLocation = HasStandardModule(MainStack, TEXT("sphere_location"));

		if (ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			if (bHasPointAttraction || bHasVortexForce || bHasVortexVelocity)
			{
				AddFailureLabel(FindingsByLabel, TEXT("orbit_ring_shell"), TEXT("Explosion hero layer is leaning on attraction or angular motion instead of a readable outward burst."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
			}
			if (!bHasAddVelocity || !bHasSphereLocation)
			{
				AddFailureLabel(FindingsByLabel, TEXT("no_outside_to_core_read"), TEXT("Explosion hero layer is missing outer-zone spawn or a readable outward burst velocity."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
			}
			if (bHasConeVelocity)
			{
				AddFailureLabel(FindingsByLabel, TEXT("smoke_plume_silhouette"), TEXT("Explosion hero layer is using cone-style motion, which tends to read like a plume instead of a compact blast."), TEXT("medium"), false, ArchetypeConfig.HeroRetuneOp);
			}
		}
		else
		{
			if (!bHasPointAttraction && (bHasVortexForce || bHasVortexVelocity))
			{
				const FString Label = ArchetypeConfig.Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase)
					? TEXT("smoke_plume_silhouette")
					: (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase)
						? TEXT("orbit_ring_shell")
						: TEXT("orbit_dominant"));
				AddFailureLabel(FindingsByLabel, Label, TEXT("Hero layer uses angular motion without enough inward attraction."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
			}
			if (!bHasPointAttraction || !bHasSphereLocation)
			{
				AddFailureLabel(FindingsByLabel, TEXT("no_outside_to_core_read"), TEXT("Hero layer is missing outer-zone spawn or inward attraction."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
			}
			if (bHasConeVelocity)
			{
				const FString Label = ArchetypeConfig.Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase)
					? TEXT("electrical_zigzag_dominance")
					: TEXT("angular_motion_too_high");
				AddFailureLabel(FindingsByLabel, Label, TEXT("Hero layer still uses cone-velocity style motion, which weakens the intended orb-centric flow."), TEXT("medium"), false, ArchetypeConfig.HeroRetuneOp);
			}
		}

		if (const TSharedPtr<FJsonObject>* MainEmitterObj = EmitterByName.Find(MainEmitter))
		{
			const TArray<TSharedPtr<FJsonValue>>* Renderers = nullptr;
			if ((*MainEmitterObj).IsValid() && (*MainEmitterObj)->TryGetArrayField(TEXT("renderers"), Renderers) && Renderers)
			{
				for (const TSharedPtr<FJsonValue>& RendererValue : *Renderers)
				{
					const TSharedPtr<FJsonObject> RendererObject = RendererValue.IsValid() ? RendererValue->AsObject() : nullptr;
					FString RendererClass;
					if (RendererObject.IsValid() && RendererObject->TryGetStringField(TEXT("renderer_class"), RendererClass) && RendererClass.Contains(TEXT("Ribbon")))
					{
						FString RibbonLabel = TEXT("cage_silhouette_risk");
						if (ArchetypeConfig.Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
						{
							RibbonLabel = TEXT("thin_wireframe_ribbons");
						}
						else if (ArchetypeConfig.Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
						{
							RibbonLabel = TEXT("closed_ring_orb");
						}
						else if (ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
						{
							RibbonLabel = TEXT("wire_arc_cage");
						}
						else if (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ||
								 ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
						{
							RibbonLabel = TEXT("thin_wireframe_ribbons");
						}

						FString LineworkLabel = TEXT("linework_not_flame_body");
						if (ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
						{
							LineworkLabel = TEXT("wire_arc_cage");
						}
						else if (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ||
								 ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
						{
							LineworkLabel = TEXT("thin_wireframe_ribbons");
						}
						AddFailureLabel(FindingsByLabel, RibbonLabel, TEXT("Hero layer is still authored as a ribbon-first silhouette."), TEXT("high"), true, ArchetypeConfig.HeroRebuildOp);
						AddFailureLabel(FindingsByLabel, LineworkLabel, TEXT("Ribbon-first hero rendering tends to read as linework instead of a family-appropriate mass read."), TEXT("high"), false, TEXT("simplify_hero_ribbon_material"));
					}
				}
			}
		}
	}

	TSharedPtr<FJsonObject> SecondaryStack;
	if (HasEmitterNamed(SecondaryEmitter) && ReviewEmitterStack(SecondaryEmitter, SecondaryStack))
	{
		const bool bSecondaryRibbon = [&EmitterByName, &SecondaryEmitter]() -> bool
		{
			const TSharedPtr<FJsonObject>* SecondaryEmitterObj = EmitterByName.Find(SecondaryEmitter);
			if (!SecondaryEmitterObj || !(*SecondaryEmitterObj).IsValid())
			{
				return false;
			}

			const TArray<TSharedPtr<FJsonValue>>* Renderers = nullptr;
			if (!(*SecondaryEmitterObj)->TryGetArrayField(TEXT("renderers"), Renderers) || !Renderers)
			{
				return false;
			}

			for (const TSharedPtr<FJsonValue>& RendererValue : *Renderers)
			{
				const TSharedPtr<FJsonObject> RendererObject = RendererValue.IsValid() ? RendererValue->AsObject() : nullptr;
				FString RendererClass;
				if (RendererObject.IsValid() &&
					RendererObject->TryGetStringField(TEXT("renderer_class"), RendererClass) &&
					RendererClass.Contains(TEXT("Ribbon")))
				{
					return true;
				}
			}
			return false;
		}();

		const bool bSecondaryTorus = HasStandardModule(SecondaryStack, TEXT("torus_location"));
		const bool bSecondarySphere = HasStandardModule(SecondaryStack, TEXT("sphere_location"));
		const bool bSecondaryVortex = HasStandardModule(SecondaryStack, TEXT("vortex_force")) || HasStandardModule(SecondaryStack, TEXT("vortex_velocity"));
		const bool bSecondaryPointAttraction = HasStandardModule(SecondaryStack, TEXT("point_attraction_force"));
		const bool bSecondaryCustomHlsl = StackContainsNodeClass(SecondaryStack, TEXT("NiagaraNodeCustomHlsl"));

		if (bSecondaryTorus)
		{
			FString Label = TEXT("shell_ring_dominant");
			if (ArchetypeConfig.Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ||
				ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("closed_ring_orb");
			}
			else if (ArchetypeConfig.Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase) ||
					 ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ||
					 ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("orbit_ring_shell");
			}
			AddFailureLabel(FindingsByLabel, Label, TEXT("Secondary support still uses ring-shell topology."), TEXT("medium"), false, TEXT("build_secondary_filament_layer"));
		}
		if (bSecondaryRibbon && bSecondarySphere && bSecondaryVortex && !bSecondaryPointAttraction)
		{
			FString Label = TEXT("silhouette_too_closed");
			if (ArchetypeConfig.Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ||
				ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("closed_ring_orb");
			}
			else if (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("lingering_orb_shell");
			}
			else if (ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("orbit_ring_shell");
			}
			AddFailureLabel(FindingsByLabel, Label, TEXT("Secondary support is orbiting from a sphere shell without inward pull, which tends to close the silhouette around the core."), TEXT("medium"), false, TEXT("build_secondary_filament_layer"));
		}
		if (bSecondaryRibbon && bSecondaryCustomHlsl)
		{
			FString Label = TEXT("linework_not_flame_body");
			if (ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("wire_arc_cage");
			}
			else if (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ||
					 ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
			{
				Label = TEXT("thin_wireframe_ribbons");
			}
			AddFailureLabel(FindingsByLabel, Label, TEXT("Secondary support still contains non-native or ribbon-heavy authoring residue that risks reading as graphic linework instead of a family-appropriate energy body."), TEXT("high"), false, TEXT("simplify_hero_ribbon_material"));
		}
	}

	FString HeroMaterialPath;
	Input->TryGetStringField(TEXT("hero_material_path"), HeroMaterialPath);
	if (!HeroMaterialPath.IsEmpty())
	{
		TSharedPtr<FJsonObject> MaterialReviewInput = MakeShareable(new FJsonObject());
		MaterialReviewInput->SetStringField(TEXT("material_path"), HeroMaterialPath);
		FString HygieneJson;
		if (ExecuteTool(TEXT("review_material_graph_hygiene"), MaterialReviewInput, HygieneJson))
		{
			TSharedPtr<FJsonObject> HygieneObject;
			if (ParseToolJsonResult(HygieneJson, HygieneObject) && HygieneObject.IsValid())
			{
				double CustomNodeCount = 0.0;
				HygieneObject->TryGetNumberField(TEXT("custom_node_count"), CustomNodeCount);
				if (CustomNodeCount > 0.0)
				{
					FString Label = TEXT("linework_not_flame_body");
					if (ArchetypeConfig.Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
					{
						Label = TEXT("wire_arc_cage");
					}
					else if (ArchetypeConfig.Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ||
							 ArchetypeConfig.Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
					{
						Label = TEXT("thin_wireframe_ribbons");
					}
					AddFailureLabel(FindingsByLabel, Label, TEXT("Hero material still depends on Custom-node-heavy linework authoring."), TEXT("high"), false, TEXT("simplify_hero_ribbon_material"));
				}
			}
		}
	}

	TArray<TSharedPtr<FJsonValue>> CurrentLabels;
	bool bCurrentHardGate = false;
	for (const auto& Pair : FindingsByLabel)
	{
		CurrentLabels.Add(MakeShared<FJsonValueString>(Pair.Key));
		bCurrentHardGate |= Pair.Value->GetBoolField(TEXT("hard_gate"));
	}
	CurrentLabels.Sort([](const TSharedPtr<FJsonValue>& A, const TSharedPtr<FJsonValue>& B)
	{
		return A->AsString() < B->AsString();
	});

	const int32 UpdatedConsecutiveMotionFailures = bCurrentHardGate ? (ConsecutiveMotionFailures + 1) : 0;
	const bool bRebuildGateActive = bCurrentHardGate && UpdatedConsecutiveMotionFailures >= 2;

	TArray<TSharedPtr<FJsonValue>> FindingsArray;
	for (const auto& Pair : FindingsByLabel)
	{
		FindingsArray.Add(MakeShared<FJsonValueObject>(Pair.Value));
	}

	TSharedPtr<FJsonObject> RepairDecision = MakeShareable(new FJsonObject());
	RepairDecision->SetBoolField(TEXT("hard_rebuild_gate_active"), bRebuildGateActive);
	RepairDecision->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), UpdatedConsecutiveMotionFailures);
	RepairDecision->SetBoolField(TEXT("block_scalar_only_layer_a_polish"), bRebuildGateActive);
	RepairDecision->SetStringField(TEXT("scheduled_op"), bRebuildGateActive ? ArchetypeConfig.HeroRebuildOp : TEXT(""));

	TSharedPtr<FJsonObject> HardGateState = MakeShareable(new FJsonObject());
	HardGateState->SetBoolField(TEXT("active"), bRebuildGateActive);
	HardGateState->SetNumberField(TEXT("consecutive_motion_failures"), UpdatedConsecutiveMotionFailures);
	HardGateState->SetStringField(TEXT("hero_rebuild_op"), ArchetypeConfig.HeroRebuildOp);

	TSharedPtr<FJsonObject> LayerProxyMetrics = MakeShareable(new FJsonObject());
	LayerProxyMetrics->SetBoolField(TEXT("hero_present"), HasEmitterNamed(MainEmitter));
	LayerProxyMetrics->SetBoolField(TEXT("secondary_present"), HasEmitterNamed(SecondaryEmitter));
	LayerProxyMetrics->SetBoolField(TEXT("core_present"), HasEmitterNamed(CoreEmitter));
	LayerProxyMetrics->SetBoolField(TEXT("sparks_present"), HasEmitterNamed(SparksEmitter));
	LayerProxyMetrics->SetBoolField(TEXT("glow_present"), HasEmitterNamed(ShockGlowEmitter));
	LayerProxyMetrics->SetNumberField(TEXT("finding_count_proxy"), FindingsArray.Num());

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"),
		ArchetypeConfig.Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
			? TEXT("review_suction_core_failure_taxonomy")
			: TEXT("review_effect_failure_taxonomy_v2"));
	Result->SetStringField(TEXT("archetype"), ArchetypeConfig.Archetype);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetNumberField(TEXT("finding_count"), FindingsArray.Num());
	Result->SetArrayField(TEXT("failure_labels"), CurrentLabels);
	Result->SetArrayField(TEXT("findings"), FindingsArray);
	Result->SetObjectField(TEXT("repair_decision"), RepairDecision);
	Result->SetObjectField(TEXT("layer_proxy_metrics"), LayerProxyMetrics);
	Result->SetObjectField(TEXT("hard_gate_state"), HardGateState);
	Result->SetStringField(TEXT("message"), FindingsArray.Num() > 0
		? TEXT("V2 taxonomy review completed with actionable findings.")
		: TEXT("V2 taxonomy review found no currently mapped failures."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RunEffectWorkflowV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	TSharedPtr<FJsonObject> ForwardInput = Input.IsValid() ? MakeShared<FJsonObject>(*Input) : MakeShared<FJsonObject>();
	ForwardInput->SetBoolField(TEXT("compatibility_wrapper"), false);
	return Tool_RunSuctionCoreV2Workflow(ForwardInput, OutResult);
}

bool FVFXToolRegistry::Tool_RunSuctionCoreV2Workflow(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_INPUT_MISSING"), TEXT("Missing input."));
	}

	const bool bCompatibilityWrapper = ParseOptionalBoolField(Input, TEXT("compatibility_wrapper"), true);
	FString ForcedArchetype;
	Input->TryGetStringField(TEXT("archetype"), ForcedArchetype);
	if (ForcedArchetype.IsEmpty() && bCompatibilityWrapper)
	{
		ForcedArchetype = TEXT("suction_core_energy");
	}

	FString Description;
	FString HeroParentMaterialPath;
	FString CoreParentMaterialPath;
	if (!Input->TryGetStringField(TEXT("description"), Description) || Description.TrimStartAndEnd().IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_DESCRIPTION_REQUIRED"), TEXT("Field 'description' is required."));
	}
	if (!Input->TryGetStringField(TEXT("hero_parent_material_path"), HeroParentMaterialPath) || HeroParentMaterialPath.IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_HERO_PARENT_REQUIRED"), TEXT("Field 'hero_parent_material_path' is required."));
	}
	if (!Input->TryGetStringField(TEXT("core_parent_material_path"), CoreParentMaterialPath) || CoreParentMaterialPath.IsEmpty())
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_CORE_PARENT_REQUIRED"), TEXT("Field 'core_parent_material_path' is required."));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bCaptureThumbnail = ParseOptionalBoolField(Input, TEXT("capture_thumbnail"), true);
	const bool bKeepAssets = ParseOptionalBoolField(Input, TEXT("keep_assets"), false);

	int32 MaxRepairIterations = 1;
	TryReadOptionalIntField(Input, TEXT("max_repair_iterations"), MaxRepairIterations);
	MaxRepairIterations = FMath::Clamp(MaxRepairIterations, 0, 3);

	FString SavePath = TEXT("/Game/ForgePilot_Generated/V2");
	Input->TryGetStringField(TEXT("save_path"), SavePath);

	FString SystemName;
	Input->TryGetStringField(TEXT("system_name"), SystemName);
	if (SystemName.IsEmpty())
	{
		FString DescriptionToken = Description.Left(48);
		SystemName = FString::Printf(TEXT("NS_SuctionCoreV2_%s_%s"), *SanitizeScenarioToken(DescriptionToken), *FDateTime::Now().ToString(TEXT("%H%M%S")));
	}

	auto NormalizeObjectPath = [](const FString& AssetPath) -> FString
	{
		if (AssetPath.Contains(TEXT(".")))
		{
			return AssetPath;
		}

		const FString AssetName = FPackageName::GetLongPackageAssetName(AssetPath);
		return AssetName.IsEmpty() ? AssetPath : FString::Printf(TEXT("%s.%s"), *AssetPath, *AssetName);
	};

	const FString SystemObjectPath = NormalizeObjectPath(FString::Printf(TEXT("%s/%s"), *SavePath, *SystemName));
	const FString HeroInstanceName = FString::Printf(TEXT("MI_%s_HeroStreak"), *SanitizeScenarioToken(SystemName));
	const FString CoreInstanceName = FString::Printf(TEXT("MI_%s_CoreMerge"), *SanitizeScenarioToken(SystemName));

	TSharedPtr<FJsonObject> Plan;
	FString ResolveError;
	TArray<FString> AvailableRecipes;
	if (!FVFXKnowledgeLoader::ResolveEffectPlanV2(Description, Plan, ResolveError, &AvailableRecipes, ForcedArchetype) || !Plan.IsValid())
	{
		TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonValue>> AvailableValues;
		for (const FString& RecipeName : AvailableRecipes)
		{
			AvailableValues.Add(MakeShared<FJsonValueString>(RecipeName));
		}
		DebugContext->SetArrayField(TEXT("available_recipes"), AvailableValues);
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_PLAN_UNRESOLVED"), ResolveError, DebugContext);
	}

	const TArray<TSharedPtr<FJsonValue>>* RawExecutionPlan = nullptr;
	if (!Plan->TryGetArrayField(TEXT("execution_plan"), RawExecutionPlan) || !RawExecutionPlan)
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_PLAN_MISSING"), TEXT("Resolved V2 plan is missing execution_plan."));
	}

	FString ResolvedEffectType = TEXT("CoreEnergy");
	Plan->TryGetStringField(TEXT("resolved_effect_type"), ResolvedEffectType);
	FString ResolvedArchetype = ForcedArchetype;
	Plan->TryGetStringField(TEXT("resolved_archetype"), ResolvedArchetype);

	FV2ArchetypeSceneConfig ArchetypeConfig;
	if (!GetV2ArchetypeSceneConfig(ResolvedArchetype, ArchetypeConfig))
	{
		return BuildSceneToolError(OutResult, TEXT("VFX_EFFECT_V2_WORKFLOW_ARCHETYPE_UNSUPPORTED"),
			FString::Printf(TEXT("Unsupported V2 archetype '%s'."), *ResolvedArchetype));
	}

	auto ExecuteToolJson = [this](const FString& ToolName, const TSharedPtr<FJsonObject>& ToolInput, TSharedPtr<FJsonObject>& OutObject, FString& OutError) -> bool
	{
		FString ToolResult;
		if (!ExecuteTool(ToolName, ToolInput, ToolResult))
		{
			OutError = ToolResult;
			ParseToolJsonResult(ToolResult, OutObject);
			return false;
		}
		if (!ParseToolJsonResult(ToolResult, OutObject))
		{
			OutError = FString::Printf(TEXT("Tool '%s' returned non-JSON output."), *ToolName);
			return false;
		}
		return true;
	};

	auto CloneBoundInput = [&](const TSharedPtr<FJsonObject>& Source, const FString& CurrentSystemPath, const FString& CurrentHeroMaterialPath, const FString& CurrentCoreMaterialPath) -> TSharedPtr<FJsonObject>
	{
		FString SourceMaterialPath;
		const FString CurrentMaterialPath = Source.IsValid() && Source->TryGetStringField(TEXT("material_path"), SourceMaterialPath)
			? BindVisualContractPlaceholders(
				SourceMaterialPath,
				CurrentSystemPath,
				HeroParentMaterialPath,
				CoreParentMaterialPath,
				CurrentHeroMaterialPath,
				CurrentHeroMaterialPath,
				CurrentCoreMaterialPath)
			: CurrentHeroMaterialPath;
		return CloneJsonObjectWithBoundPlaceholders(
			Source,
			CurrentSystemPath,
			HeroParentMaterialPath,
			CoreParentMaterialPath,
			CurrentMaterialPath,
			CurrentHeroMaterialPath,
			CurrentCoreMaterialPath);
	};

	TArray<TSharedPtr<FJsonValue>> BoundExecutionPlan;
	TArray<TSharedPtr<FJsonValue>> ExecutedSteps;
	TArray<TSharedPtr<FJsonValue>> RepairTrace;
	TArray<TSharedPtr<FJsonValue>> CreatedAssets;
	TArray<TSharedPtr<FJsonValue>> DeletedAssets;
	TArray<TSharedPtr<FJsonValue>> CleanupFailedAssets;
	TSharedPtr<FJsonObject> FinalCritic;
	TSharedPtr<FJsonObject> FinalRepairDecision;
	FString FinalThumbnailPath;
	int32 RebuildCount = 0;
	int32 ConsecutiveLayerAMotionFailures = 0;
	bool bHardRebuildGateActive = false;
	FString CreatedSystemPath = SystemObjectPath;
	FString CreatedHeroMaterialPath = NormalizeObjectPath(FString::Printf(TEXT("%s/%s"), *SavePath, *HeroInstanceName));
	FString CreatedCoreMaterialPath = NormalizeObjectPath(FString::Printf(TEXT("%s/%s"), *SavePath, *CoreInstanceName));

	auto DeleteGeneratedAsset = [](const FString& AssetPath) -> bool
	{
		if (AssetPath.IsEmpty())
		{
			return true;
		}

		const FString PackagePath = FPackageName::ObjectPathToPackageName(AssetPath);
		if (!PackagePath.IsEmpty() && UEditorAssetLibrary::DeleteAsset(PackagePath))
		{
			return true;
		}

		return UEditorAssetLibrary::DeleteAsset(AssetPath);
	};

	for (const TSharedPtr<FJsonValue>& StepValue : *RawExecutionPlan)
	{
		const TSharedPtr<FJsonObject> StepObject = StepValue.IsValid() ? StepValue->AsObject() : nullptr;
		if (!StepObject.IsValid())
		{
			continue;
		}

		const TSharedPtr<FJsonObject>* StepInputPtr = nullptr;
		TSharedPtr<FJsonObject> StepInput = MakeShareable(new FJsonObject());
		if (StepObject->TryGetObjectField(TEXT("input"), StepInputPtr) && StepInputPtr && (*StepInputPtr).IsValid())
		{
			StepInput = CloneBoundInput(*StepInputPtr, CreatedSystemPath, CreatedHeroMaterialPath, CreatedCoreMaterialPath);
		}

		FString ToolName;
		StepObject->TryGetStringField(TEXT("tool_name"), ToolName);
		if (ToolName.Equals(TEXT("create_empty_niagara_system"), ESearchCase::IgnoreCase))
		{
			StepInput->SetStringField(TEXT("system_name"), SystemName);
			StepInput->SetStringField(TEXT("save_path"), SavePath);
			StepInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
		}
		else if (ToolName.Equals(TEXT("run_v2_material_op"), ESearchCase::IgnoreCase) ||
			ToolName.Equals(TEXT("run_suction_core_material_op"), ESearchCase::IgnoreCase))
		{
			FString OpName;
			StepInput->TryGetStringField(TEXT("op_name"), OpName);
			StepInput->SetStringField(TEXT("save_path"), SavePath);
			StepInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
			StepInput->SetStringField(TEXT("archetype"), ResolvedArchetype);
			if (OpName.Equals(TEXT("build_hero_streak_material"), ESearchCase::IgnoreCase))
			{
				StepInput->SetStringField(TEXT("instance_name"), HeroInstanceName);
			}
			else if (OpName.Equals(TEXT("build_core_merge_glow_material"), ESearchCase::IgnoreCase))
			{
				StepInput->SetStringField(TEXT("instance_name"), CoreInstanceName);
			}
		}
		else if (ToolName.Equals(TEXT("run_v2_niagara_op"), ESearchCase::IgnoreCase) ||
			ToolName.Equals(TEXT("run_suction_core_niagara_op"), ESearchCase::IgnoreCase))
		{
			StepInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
			StepInput->SetBoolField(TEXT("hard_rebuild_gate_active"), bHardRebuildGateActive);
			StepInput->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), ConsecutiveLayerAMotionFailures);
			StepInput->SetStringField(TEXT("archetype"), ResolvedArchetype);
		}
		else if (ToolName.Equals(TEXT("review_effect_failure_taxonomy_v2"), ESearchCase::IgnoreCase) ||
			ToolName.Equals(TEXT("review_suction_core_failure_taxonomy"), ESearchCase::IgnoreCase))
		{
			StepInput->SetStringField(TEXT("archetype"), ResolvedArchetype);
			StepInput->SetStringField(TEXT("hero_material_path"), CreatedHeroMaterialPath);
			StepInput->SetStringField(TEXT("core_material_path"), CreatedCoreMaterialPath);
			StepInput->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), ConsecutiveLayerAMotionFailures);
		}

		TSharedPtr<FJsonObject> BoundStep = MakeShareable(new FJsonObject(*StepObject));
		BoundStep->SetObjectField(TEXT("input"), StepInput);
		BoundExecutionPlan.Add(MakeShared<FJsonValueObject>(BoundStep));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRun = MakeShareable(new FJsonObject());
		DryRun->SetBoolField(TEXT("success"), true);
		DryRun->SetBoolField(TEXT("dry_run"), true);
		DryRun->SetStringField(TEXT("action"),
			ResolvedArchetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
				? TEXT("run_suction_core_v2_workflow")
				: TEXT("run_effect_workflow_v2"));
		DryRun->SetStringField(TEXT("description"), Description);
		DryRun->SetStringField(TEXT("resolved_effect_type"), ResolvedEffectType);
		DryRun->SetStringField(TEXT("resolved_archetype"), ResolvedArchetype);
		DryRun->SetStringField(TEXT("system_path"), CreatedSystemPath);
		DryRun->SetStringField(TEXT("hero_material_path"), CreatedHeroMaterialPath);
		DryRun->SetStringField(TEXT("core_material_path"), CreatedCoreMaterialPath);
		DryRun->SetArrayField(TEXT("bound_execution_plan"), BoundExecutionPlan);
		OutResult = JsonObjToString(DryRun);
		return true;
	}

	for (const TSharedPtr<FJsonValue>& StepValue : BoundExecutionPlan)
	{
		const TSharedPtr<FJsonObject> StepObject = StepValue.IsValid() ? StepValue->AsObject() : nullptr;
		if (!StepObject.IsValid())
		{
			continue;
		}

		FString ToolName;
		StepObject->TryGetStringField(TEXT("tool_name"), ToolName);
		const TSharedPtr<FJsonObject>* StepInputPtr = nullptr;
		TSharedPtr<FJsonObject> StepInput = MakeShareable(new FJsonObject());
		if (StepObject->TryGetObjectField(TEXT("input"), StepInputPtr) && StepInputPtr && (*StepInputPtr).IsValid())
		{
			StepInput = *StepInputPtr;
		}

		TSharedPtr<FJsonObject> StepResult;
		FString StepError;
		const bool bStepSuccess = ExecuteToolJson(ToolName, StepInput, StepResult, StepError);

		TSharedPtr<FJsonObject> ExecutedStep = MakeShareable(new FJsonObject());
		ExecutedStep->SetStringField(TEXT("tool_name"), ToolName);
		ExecutedStep->SetObjectField(TEXT("input"), StepInput);
		ExecutedStep->SetBoolField(TEXT("success"), bStepSuccess);
		if (StepResult.IsValid())
		{
			ExecutedStep->SetObjectField(TEXT("result"), StepResult);
		}
		if (!bStepSuccess)
		{
			ExecutedStep->SetStringField(TEXT("error"), StepError);
			ExecutedSteps.Add(MakeShared<FJsonValueObject>(ExecutedStep));
			TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
			DebugContext->SetArrayField(TEXT("executed_steps"), ExecutedSteps);
			DebugContext->SetObjectField(TEXT("failed_step"), ExecutedStep);
			DebugContext->SetStringField(TEXT("system_path"), CreatedSystemPath);
			DebugContext->SetStringField(TEXT("hero_material_path"), CreatedHeroMaterialPath);
			DebugContext->SetStringField(TEXT("core_material_path"), CreatedCoreMaterialPath);
			return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_STEP_FAILED"), StepError.IsEmpty() ? TEXT("Workflow step failed.") : StepError, DebugContext);
		}

		if (ToolName.Equals(TEXT("create_empty_niagara_system"), ESearchCase::IgnoreCase) && StepResult.IsValid())
		{
			FString AssetPath;
			if (StepResult->TryGetStringField(TEXT("asset_path"), AssetPath) && !AssetPath.IsEmpty())
			{
				CreatedSystemPath = NormalizeObjectPath(AssetPath);
				CreatedAssets.Add(MakeShared<FJsonValueString>(CreatedSystemPath));
			}
		}
		else if ((ToolName.Equals(TEXT("run_v2_material_op"), ESearchCase::IgnoreCase) ||
			ToolName.Equals(TEXT("run_suction_core_material_op"), ESearchCase::IgnoreCase)) && StepResult.IsValid())
		{
			FString MaterialPath;
			if (StepResult->TryGetStringField(TEXT("material_path"), MaterialPath) && !MaterialPath.IsEmpty())
			{
				FString OpName;
				StepInput->TryGetStringField(TEXT("op_name"), OpName);
				if (OpName.Equals(TEXT("build_hero_streak_material"), ESearchCase::IgnoreCase))
				{
					CreatedHeroMaterialPath = MaterialPath;
				}
				else if (OpName.Equals(TEXT("build_core_merge_glow_material"), ESearchCase::IgnoreCase))
				{
					CreatedCoreMaterialPath = MaterialPath;
				}
				CreatedAssets.Add(MakeShared<FJsonValueString>(MaterialPath));
			}
		}
		else if ((ToolName.Equals(TEXT("review_effect_failure_taxonomy_v2"), ESearchCase::IgnoreCase) ||
			ToolName.Equals(TEXT("review_suction_core_failure_taxonomy"), ESearchCase::IgnoreCase)) && StepResult.IsValid())
		{
			FinalCritic = StepResult;
			const TSharedPtr<FJsonObject>* RepairDecisionPtr = nullptr;
			if (StepResult->TryGetObjectField(TEXT("repair_decision"), RepairDecisionPtr) && RepairDecisionPtr && (*RepairDecisionPtr).IsValid())
			{
				FinalRepairDecision = *RepairDecisionPtr;
				FinalRepairDecision->TryGetBoolField(TEXT("hard_rebuild_gate_active"), bHardRebuildGateActive);
				double FailureCount = 0.0;
				if (FinalRepairDecision->TryGetNumberField(TEXT("consecutive_layer_a_motion_failures"), FailureCount))
				{
					ConsecutiveLayerAMotionFailures = FMath::RoundToInt(FailureCount);
				}
			}
		}

		ExecutedSteps.Add(MakeShared<FJsonValueObject>(ExecutedStep));
	}

	for (int32 RepairIteration = 0; RepairIteration < MaxRepairIterations; ++RepairIteration)
	{
		if (!FinalRepairDecision.IsValid())
		{
			break;
		}

		FString ScheduledOp;
		FinalRepairDecision->TryGetStringField(TEXT("scheduled_op"), ScheduledOp);
		bool bShouldRebuild = false;
		FinalRepairDecision->TryGetBoolField(TEXT("hard_rebuild_gate_active"), bShouldRebuild);
		bShouldRebuild = bShouldRebuild && ScheduledOp.Equals(ArchetypeConfig.HeroRebuildOp, ESearchCase::IgnoreCase);
		if (!bShouldRebuild)
		{
			break;
		}

		TSharedPtr<FJsonObject> RebuildInput = MakeShareable(new FJsonObject());
		RebuildInput->SetStringField(TEXT("archetype"), ResolvedArchetype);
		RebuildInput->SetStringField(TEXT("op_name"), ArchetypeConfig.HeroRebuildOp);
		RebuildInput->SetStringField(TEXT("system_path"), CreatedSystemPath);
		RebuildInput->SetStringField(TEXT("material_path"), CreatedHeroMaterialPath);
		RebuildInput->SetBoolField(TEXT("hard_rebuild_gate_active"), bHardRebuildGateActive);
		RebuildInput->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), ConsecutiveLayerAMotionFailures);
		RebuildInput->SetBoolField(TEXT("apply_changes"), true);

		TSharedPtr<FJsonObject> RebuildResult;
		FString RebuildError;
		if (!ExecuteToolJson(TEXT("run_v2_niagara_op"), RebuildInput, RebuildResult, RebuildError))
		{
			TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
			DebugContext->SetArrayField(TEXT("executed_steps"), ExecutedSteps);
			DebugContext->SetArrayField(TEXT("repair_trace"), RepairTrace);
			if (RebuildResult.IsValid())
			{
				DebugContext->SetObjectField(TEXT("rebuild_result"), RebuildResult);
			}
			return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_REBUILD_FAILED"), RebuildError.IsEmpty() ? TEXT("Automatic Layer A rebuild failed.") : RebuildError, DebugContext);
		}

		TSharedPtr<FJsonObject> RepairStep = MakeShareable(new FJsonObject());
		RepairStep->SetNumberField(TEXT("iteration"), RepairIteration + 1);
		RepairStep->SetStringField(TEXT("action"), ArchetypeConfig.HeroRebuildOp);
		RepairStep->SetObjectField(TEXT("result"), RebuildResult);
		RepairTrace.Add(MakeShared<FJsonValueObject>(RepairStep));
		RebuildCount++;

		TSharedPtr<FJsonObject> CriticInput = MakeShareable(new FJsonObject());
		CriticInput->SetStringField(TEXT("archetype"), ResolvedArchetype);
		CriticInput->SetStringField(TEXT("system_path"), CreatedSystemPath);
		CriticInput->SetStringField(TEXT("hero_material_path"), CreatedHeroMaterialPath);
		CriticInput->SetStringField(TEXT("core_material_path"), CreatedCoreMaterialPath);
		CriticInput->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), ConsecutiveLayerAMotionFailures);

		TSharedPtr<FJsonObject> CriticResult;
		FString CriticError;
		if (!ExecuteToolJson(TEXT("review_effect_failure_taxonomy_v2"), CriticInput, CriticResult, CriticError))
		{
			TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
			DebugContext->SetArrayField(TEXT("executed_steps"), ExecutedSteps);
			DebugContext->SetArrayField(TEXT("repair_trace"), RepairTrace);
			return BuildSceneToolError(OutResult, TEXT("VFX_SUCTION_V2_WORKFLOW_CRITIC_RERUN_FAILED"), CriticError.IsEmpty() ? TEXT("Post-rebuild critic rerun failed.") : CriticError, DebugContext);
		}

		FinalCritic = CriticResult;
		const TSharedPtr<FJsonObject>* RepairDecisionPtr = nullptr;
		if (CriticResult->TryGetObjectField(TEXT("repair_decision"), RepairDecisionPtr) && RepairDecisionPtr && (*RepairDecisionPtr).IsValid())
		{
			FinalRepairDecision = *RepairDecisionPtr;
			FinalRepairDecision->TryGetBoolField(TEXT("hard_rebuild_gate_active"), bHardRebuildGateActive);
			double FailureCount = 0.0;
			if (FinalRepairDecision->TryGetNumberField(TEXT("consecutive_layer_a_motion_failures"), FailureCount))
			{
				ConsecutiveLayerAMotionFailures = FMath::RoundToInt(FailureCount);
			}
		}
		else
		{
			FinalRepairDecision.Reset();
			bHardRebuildGateActive = false;
			ConsecutiveLayerAMotionFailures = 0;
		}
	}

	if (bCaptureThumbnail)
	{
		TSharedPtr<FJsonObject> ThumbnailInput = MakeShareable(new FJsonObject());
		ThumbnailInput->SetStringField(TEXT("asset_path"), CreatedSystemPath);
		FString ThumbnailFilename;
		if (!Input->TryGetStringField(TEXT("thumbnail_filename"), ThumbnailFilename) || ThumbnailFilename.IsEmpty())
		{
			ThumbnailFilename = FString::Printf(TEXT("%s_workflow_thumb"), *SanitizeScenarioToken(SystemName));
		}
		ThumbnailInput->SetStringField(TEXT("filename"), ThumbnailFilename);

		TSharedPtr<FJsonObject> ThumbnailResult;
		FString ThumbnailError;
		if (ExecuteToolJson(TEXT("capture_asset_thumbnail"), ThumbnailInput, ThumbnailResult, ThumbnailError) &&
			ThumbnailResult.IsValid())
		{
			ThumbnailResult->TryGetStringField(TEXT("output_path"), FinalThumbnailPath);
		}
	}

	if (!bKeepAssets)
	{
		TSet<FString> SeenDeletedAssets;
		for (const TSharedPtr<FJsonValue>& CreatedValue : CreatedAssets)
		{
			if (!CreatedValue.IsValid())
			{
				continue;
			}

			const FString AssetPath = CreatedValue->AsString();
			if (AssetPath.IsEmpty() || SeenDeletedAssets.Contains(AssetPath))
			{
				continue;
			}
			SeenDeletedAssets.Add(AssetPath);

			if (DeleteGeneratedAsset(AssetPath))
			{
				DeletedAssets.Add(MakeShared<FJsonValueString>(AssetPath));
			}
			else
			{
				CleanupFailedAssets.Add(MakeShared<FJsonValueString>(AssetPath));
			}
		}
	}

	bool bWorkflowPassed = true;
	if (FinalCritic.IsValid())
	{
		double FindingCount = 0.0;
		if (FinalCritic->TryGetNumberField(TEXT("finding_count"), FindingCount))
		{
			bWorkflowPassed &= (FindingCount <= 0.0);
		}
	}
	if (FinalRepairDecision.IsValid())
	{
		bool bRepairGateStillActive = false;
		FinalRepairDecision->TryGetBoolField(TEXT("hard_rebuild_gate_active"), bRepairGateStillActive);
		bWorkflowPassed &= !bRepairGateStillActive;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"),
		ResolvedArchetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
			? TEXT("run_suction_core_v2_workflow")
			: TEXT("run_effect_workflow_v2"));
	Result->SetStringField(TEXT("description"), Description);
	Result->SetStringField(TEXT("resolved_effect_type"), ResolvedEffectType);
	Result->SetStringField(TEXT("resolved_archetype"), ResolvedArchetype);
	Result->SetStringField(TEXT("system_path"), CreatedSystemPath);
	Result->SetStringField(TEXT("hero_material_path"), CreatedHeroMaterialPath);
	Result->SetStringField(TEXT("core_material_path"), CreatedCoreMaterialPath);
	Result->SetBoolField(TEXT("workflow_passed"), bWorkflowPassed);
	Result->SetBoolField(TEXT("keep_assets"), bKeepAssets);
	Result->SetBoolField(TEXT("assets_retained"), bKeepAssets);
	Result->SetNumberField(TEXT("rebuild_count"), RebuildCount);
	Result->SetNumberField(TEXT("consecutive_layer_a_motion_failures"), ConsecutiveLayerAMotionFailures);
	Result->SetArrayField(TEXT("created_assets"), CreatedAssets);
	Result->SetArrayField(TEXT("deleted_assets"), DeletedAssets);
	Result->SetArrayField(TEXT("cleanup_failed_assets"), CleanupFailedAssets);
	Result->SetArrayField(TEXT("bound_execution_plan"), BoundExecutionPlan);
	Result->SetArrayField(TEXT("executed_steps"), ExecutedSteps);
	Result->SetArrayField(TEXT("repair_trace"), RepairTrace);
	if (FinalCritic.IsValid())
	{
		Result->SetObjectField(TEXT("critic_output"), FinalCritic);
	}
	if (FinalRepairDecision.IsValid())
	{
		Result->SetObjectField(TEXT("repair_decision"), FinalRepairDecision);
	}
	if (!FinalThumbnailPath.IsEmpty())
	{
		Result->SetStringField(TEXT("thumbnail_path"), FinalThumbnailPath);
	}

	OutResult = JsonObjToString(Result);
	return true;
}
