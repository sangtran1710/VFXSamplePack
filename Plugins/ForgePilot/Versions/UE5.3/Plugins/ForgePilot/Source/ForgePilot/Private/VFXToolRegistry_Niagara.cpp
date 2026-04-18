// Copyright 2026, HenryFX. ForgePilot Plugin.
// Phase 4: Niagara VFX Tools - Registration + Implementation
#include "VFXToolRegistry.h"
#include "VFXKnowledgeLoader.h"
#include "VFXMutationAudit.h"
#include "VFXNiagaraStackToolRegistration.h"
#include "VFXNiagaraStackTools.h"
#include "VFXToolRegistryHelpers.h"

#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"
#include "NiagaraRendererProperties.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraTypes.h"
#include "NiagaraParameterStore.h"
#include "NiagaraCommon.h"
#include "NiagaraConstants.h"
#include "NiagaraSystemFactoryNew.h"
#include "NiagaraEmitterFactoryNew.h"
#include "NiagaraScript.h"
#include "NiagaraSpriteRendererProperties.h"
#include "NiagaraRibbonRendererProperties.h"
#include "NiagaraMeshRendererProperties.h"
#include "NiagaraScriptSource.h"
#include "NiagaraGraph.h"
#include "NiagaraNodeFunctionCall.h"
#include "NiagaraNodeAssignment.h"
#include "NiagaraNodeOutput.h"
#include "NiagaraNodeInput.h"
#include "NiagaraNodeCustomHlsl.h"
#include "NiagaraSimulationStageBase.h"
#include "NiagaraEditorUtilities.h"
#include "NiagaraSystemEditorData.h"
#include "ViewModels/Stack/NiagaraStackFunctionInput.h"
#include "ViewModels/Stack/NiagaraStackGraphUtilities.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "EditorAssetLibrary.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Interfaces/IPluginManager.h"
#include "PackageTools.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture.h"
#include "HAL/PlatformProcess.h"

namespace
{
	static FString GetAssetPackageFilename(UObject* Asset)
	{
		if (!Asset)
		{
			return FString();
		}

		return FPackageName::LongPackageNameToFilename(
			Asset->GetOutermost()->GetName(),
			FPackageName::GetAssetPackageExtension());
	}

	static void AppendNiagaraMutationAudit(
		TSharedPtr<FJsonObject> Result,
		const FString& ToolName,
		UObject* Asset,
		const FString& Message,
		const FString& SnapshotPath = TEXT(""),
		const TSharedPtr<FJsonObject>& Details = nullptr)
	{
		if (!Result.IsValid() || !Asset)
		{
			return;
		}

		FString AuditEntryPath;
		const bool bRecorded = FVFXMutationAudit::RecordAssetMutation(
			ToolName,
			Asset->GetPathName(),
			Asset->GetClass()->GetName(),
			ToolName,
			SnapshotPath,
			GetAssetPackageFilename(Asset),
			true,
			Message,
			AuditEntryPath,
			Details);

		Result->SetBoolField(TEXT("audit_recorded"), bRecorded);
		Result->SetStringField(TEXT("audit_entry_path"), AuditEntryPath);
	}

	static void AppendNiagaraMutationAudit(
		TSharedPtr<FJsonObject> Result,
		const FString& ToolName,
		UObject* Asset,
		const FString& Message,
		const TSharedPtr<FJsonObject>& Details)
	{
		AppendNiagaraMutationAudit(Result, ToolName, Asset, Message, TEXT(""), Details);
	}

	static FString SanitizeNiagaraMutationToken(FString Value)
	{
		Value = Value.TrimStartAndEnd();
		for (TCHAR& Character : Value)
		{
			if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
			{
				Character = TEXT('_');
			}
		}

		return Value.IsEmpty() ? TEXT("niagara") : Value;
	}

	struct FV2NiagaraArchetypeConfig
	{
		FString Archetype;
		FString HeroEmitter;
		FString SecondaryEmitter;
		FString CoreEmitter;
		FString SparksEmitter;
		FString GlowEmitter;
		FString HeroMaterialFamily;
		FString CoreMaterialFamily;
		FString HeroRebuildOp;
	};

	static bool GetV2NiagaraArchetypeConfig(const FString& Archetype, FV2NiagaraArchetypeConfig& OutConfig)
	{
		if (Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("electric_arc_orb"),
				TEXT("HeroElectricArc"),
				TEXT("SecondaryElectricFilaments"),
				TEXT("CoreElectricMass"),
				TEXT("SparkBursts"),
				TEXT("BindGlow"),
				TEXT("electric_hero_streak_material"),
				TEXT("electric_core_mass_material"),
				TEXT("rebuild_hero_electric_arc_layer")
			};
			return true;
		}
		if (Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("impact_pulse_core"),
				TEXT("HeroImpactBurst"),
				TEXT("SecondaryImpactStreaks"),
				TEXT("CoreImpactFlash"),
				TEXT("DebrisSparks"),
				TEXT("BindGlow"),
				TEXT("impact_hero_streak_material"),
				TEXT("impact_core_mass_material"),
				TEXT("rebuild_hero_impact_burst_layer")
			};
			return true;
		}
		if (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("explosion_flash_core"),
				TEXT("HeroExplosionBurst"),
				TEXT("SecondaryBlastStreaks"),
				TEXT("CoreFlashMass"),
				TEXT("DebrisSparks"),
				TEXT("ShockGlow"),
				TEXT("explosion_hero_streak_material"),
				TEXT("explosion_core_mass_material"),
				TEXT("rebuild_hero_explosion_burst_layer")
			};
			return true;
		}
		if (Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("hero_fire_orb_sweep"),
				TEXT("HeroFireSweep"),
				TEXT("SecondaryFireFilaments"),
				TEXT("CoreFlameMass"),
				TEXT("Embers"),
				TEXT("BindGlow"),
				TEXT("fire_hero_streak_material"),
				TEXT("fire_core_mass_material"),
				TEXT("rebuild_hero_fire_sweep_layer")
			};
			return true;
		}
		if (Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("arcane_pull_orb"),
				TEXT("HeroArcaneSweep"),
				TEXT("SecondaryArcaneFilaments"),
				TEXT("CoreArcaneMass"),
				TEXT("SparkMotes"),
				TEXT("BindGlow"),
				TEXT("arcane_hero_streak_material"),
				TEXT("arcane_core_mass_material"),
				TEXT("rebuild_hero_arcane_sweep_layer")
			};
			return true;
		}
		if (Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase))
		{
			OutConfig = {
				TEXT("suction_core_energy"),
				TEXT("MainAttractionStreaks"),
				TEXT("SecondaryFilaments"),
				TEXT("CoreTurbulence"),
				TEXT("Sparks"),
				TEXT("ShockGlow"),
				TEXT("hero_streak_material"),
				TEXT("core_merge_glow_material"),
				TEXT("rebuild_main_attraction_layer")
			};
			return true;
		}
		return false;
	}

	static bool TryReadOptionalIntField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, int32& OutValue)
	{
		if (!Input.IsValid())
		{
			return false;
		}

		double NumericValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumericValue))
		{
			OutValue = static_cast<int32>(NumericValue);
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

	static FString GetNiagaraMutationSnapshotRoot()
	{
		return FPaths::Combine(GetForgePilotDownloadsRoot(), TEXT("NiagaraMutationSnapshots"));
	}

	static bool CreateNiagaraMutationSnapshot(UNiagaraSystem* NiagaraSystem, const FString& MutationAction, FString& OutSnapshotPath, FString& OutError)
	{
		if (!NiagaraSystem)
		{
			OutError = TEXT("Niagara System is null.");
			return false;
		}

		const FString SnapshotRoot = GetNiagaraMutationSnapshotRoot();
		if (SnapshotRoot.IsEmpty())
		{
			OutError = TEXT("Could not resolve ForgePilot Niagara snapshot directory.");
			return false;
		}

		const FString PackageFilename = GetAssetPackageFilename(NiagaraSystem);
		if (!FPaths::FileExists(PackageFilename))
		{
			OutError = FString::Printf(TEXT("Niagara package file does not exist on disk: %s"), *PackageFilename);
			return false;
		}

		const FString SystemToken = SanitizeNiagaraMutationToken(NiagaraSystem->GetName());
		const FString ActionToken = SanitizeNiagaraMutationToken(MutationAction);
		const FString SnapshotDir = FPaths::Combine(SnapshotRoot, SystemToken);
		IFileManager::Get().MakeDirectory(*SnapshotDir, true);

		const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
		OutSnapshotPath = FPaths::Combine(
			SnapshotDir,
			FString::Printf(TEXT("%s_%s_%s%s"),
				*SystemToken,
				*ActionToken,
				*Timestamp,
				*FPackageName::GetAssetPackageExtension()));

		const uint32 CopyResult = IFileManager::Get().Copy(*OutSnapshotPath, *PackageFilename, true, true);
		if (CopyResult != COPY_OK)
		{
			OutError = FString::Printf(TEXT("Failed to create Niagara snapshot (copy code %u)."), CopyResult);
			OutSnapshotPath.Reset();
			return false;
		}

		return true;
	}

	static void GetNiagaraSnapshotFiles(UNiagaraSystem* NiagaraSystem, TArray<FString>& OutSnapshotFiles)
	{
		OutSnapshotFiles.Reset();
		if (!NiagaraSystem)
		{
			return;
		}

		const FString SnapshotRoot = GetNiagaraMutationSnapshotRoot();
		if (SnapshotRoot.IsEmpty())
		{
			return;
		}

		const FString SnapshotDir = FPaths::Combine(SnapshotRoot, SanitizeNiagaraMutationToken(NiagaraSystem->GetName()));
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

	static bool IsNiagaraSnapshotPathUnderRoot(const FString& SnapshotPath)
	{
		const FString SnapshotRoot = FPaths::ConvertRelativePathToFull(GetNiagaraMutationSnapshotRoot());
		const FString FullSnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
		return !SnapshotRoot.IsEmpty() && FullSnapshotPath.StartsWith(SnapshotRoot, ESearchCase::IgnoreCase);
	}

	static TSharedPtr<FJsonObject> BuildNiagaraMutationPatchReport(
		const FString& Action,
		UNiagaraSystem* NiagaraSystem,
		const FString& SnapshotPath,
		const bool bSaveSucceeded,
		const TSharedPtr<FJsonObject>& Details = nullptr)
	{
		TSharedPtr<FJsonObject> PatchReport = MakeShareable(new FJsonObject());
		PatchReport->SetStringField(TEXT("action"), Action);
		PatchReport->SetStringField(TEXT("system_path"), NiagaraSystem ? NiagaraSystem->GetPathName() : TEXT(""));
		PatchReport->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		PatchReport->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
		if (Details.IsValid())
		{
			PatchReport->SetObjectField(TEXT("details"), Details);
		}
		return PatchReport;
	}

	static bool FinalizeNiagaraAssetMutation(
		FString& OutResult,
		const TSharedPtr<FJsonObject>& Result,
		const FString& ToolName,
		UNiagaraSystem* NiagaraSystem,
		const FString& SnapshotPath,
		const bool bSaveSucceeded,
		const FString& Message,
		const TSharedPtr<FJsonObject>& Details = nullptr,
		const FString& CompileSaveError = FString(),
		const TSharedPtr<FJsonObject>& CompileDiagnostics = TSharedPtr<FJsonObject>())
	{
		if (!Result.IsValid())
		{
			TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
			ErrorObj->SetBoolField(TEXT("success"), false);
			ErrorObj->SetStringField(TEXT("error_code"), TEXT("NI_INTERNAL_RESULT_MISSING"));
			ErrorObj->SetStringField(TEXT("error_message"), TEXT("Niagara mutation result object was not initialized."));
			OutResult = JsonObjToString(ErrorObj);
			return false;
		}

		Result->SetBoolField(TEXT("success"), bSaveSucceeded);
		Result->SetStringField(TEXT("message"), Message);
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
		Result->SetBoolField(TEXT("saved"), bSaveSucceeded);
		if (CompileDiagnostics.IsValid())
		{
			Result->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
		}
		if (!bSaveSucceeded && !CompileSaveError.IsEmpty())
		{
			Result->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
			Result->SetStringField(TEXT("error_message"), CompileSaveError);
		}
		Result->SetObjectField(
			TEXT("patch_report"),
			BuildNiagaraMutationPatchReport(ToolName, NiagaraSystem, SnapshotPath, bSaveSucceeded, Details));
		AppendNiagaraMutationAudit(Result, ToolName, NiagaraSystem, Message, SnapshotPath, Details);
		OutResult = JsonObjToString(Result);
		return bSaveSucceeded;
	}

	static bool CompileAndSaveNiagaraAssetWithLogGuard(
		UNiagaraSystem* NiagaraSystem,
		FString& OutErrorMessage,
		TSharedPtr<FJsonObject>& OutDiagnostics)
	{
		VFXNiagaraStackTools::FEmitterContext Context;
		Context.System = NiagaraSystem;

		const bool bSucceeded = VFXNiagaraStackTools::CompileAndSaveSystem(Context, OutErrorMessage, OutDiagnostics);
		if (OutDiagnostics.IsValid())
		{
			OutDiagnostics->SetBoolField(TEXT("save_succeeded"), bSucceeded);
		}
		if (!bSucceeded && OutErrorMessage.IsEmpty())
		{
			OutErrorMessage = NiagaraSystem
				? FString::Printf(TEXT("Niagara compile/log/save guard failed for '%s'."), *NiagaraSystem->GetPathName())
				: TEXT("Niagara compile/log/save guard failed: system is null.");
		}
		return bSucceeded;
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

	static FString NormalizeNiagaraTemplateToken(const FString& Value);

	static void ParseOptionalStringArrayField(
		const TSharedPtr<FJsonObject>& Input,
		const TCHAR* FieldName,
		TArray<FString>& OutValues)
	{
		OutValues.Reset();
		if (!Input.IsValid())
		{
			return;
		}

		const TArray<TSharedPtr<FJsonValue>>* JsonArray = nullptr;
		if (Input->TryGetArrayField(FieldName, JsonArray) && JsonArray)
		{
			for (const TSharedPtr<FJsonValue>& Entry : *JsonArray)
			{
				if (!Entry.IsValid())
				{
					continue;
				}

				const FString Value = Entry->AsString().TrimStartAndEnd();
				if (!Value.IsEmpty())
				{
					OutValues.AddUnique(Value);
				}
			}
			return;
		}

		FString CsvValue;
		if (Input->TryGetStringField(FieldName, CsvValue))
		{
			TArray<FString> Tokens;
			CsvValue.ParseIntoArray(Tokens, TEXT(","), true);
			for (FString& Token : Tokens)
			{
				Token = Token.TrimStartAndEnd();
				if (!Token.IsEmpty())
				{
					OutValues.AddUnique(Token);
				}
			}
		}
	}

	static bool BuildNiagaraToolError(
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

	static bool ParseToolJsonResult(const FString& Json, TSharedPtr<FJsonObject>& OutObject)
	{
		OutObject.Reset();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
		return FJsonSerializer::Deserialize(Reader, OutObject) && OutObject.IsValid();
	}

	static bool SystemHasEmitterNamed(UNiagaraSystem* NiagaraSystem, const FString& EmitterName)
	{
		if (!NiagaraSystem || EmitterName.IsEmpty())
		{
			return false;
		}

		for (const FNiagaraEmitterHandle& Handle : NiagaraSystem->GetEmitterHandles())
		{
			if (Handle.GetName().ToString().Equals(EmitterName, ESearchCase::CaseSensitive))
			{
				return true;
			}
		}
		return false;
	}

	static bool ResolveNiagaraModuleScriptPathByName(
		const FString& ModuleDisplayName,
		FString& OutModuleScriptPath,
		FString& OutErrorMessage)
	{
		OutModuleScriptPath.Reset();
		OutErrorMessage.Reset();
		if (ModuleDisplayName.IsEmpty())
		{
			OutErrorMessage = TEXT("Module display name is empty.");
			return false;
		}

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		FARFilter Filter;
		Filter.ClassPaths.Add(UNiagaraScript::StaticClass()->GetClassPathName());
		Filter.bRecursiveClasses = true;
		Filter.bRecursivePaths = true;
		Filter.PackagePaths.Add(FName(TEXT("/Niagara/Modules")));

		TArray<FAssetData> AssetDataList;
		AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

		const FString Wanted = NormalizeNiagaraTemplateToken(ModuleDisplayName);
		int32 BestScore = MIN_int32;
		for (const FAssetData& AssetData : AssetDataList)
		{
			const FString AssetNameNorm = NormalizeNiagaraTemplateToken(AssetData.AssetName.ToString());
			const FString ObjectPath = AssetData.GetObjectPathString();
			const FString ObjectPathNorm = NormalizeNiagaraTemplateToken(ObjectPath);
			int32 Score = MIN_int32;
			if (AssetNameNorm == Wanted)
			{
				Score = 1000;
			}
			else if (AssetNameNorm.Contains(Wanted) || ObjectPathNorm.Contains(Wanted))
			{
				Score = 500;
			}
			if (Score > BestScore)
			{
				BestScore = Score;
				OutModuleScriptPath = ObjectPath;
			}
		}

		if (OutModuleScriptPath.IsEmpty())
		{
			OutErrorMessage = FString::Printf(
				TEXT("Could not resolve native Niagara module script for '%s'. The V2 semantic op refuses to fall back to non-native authoring."),
				*ModuleDisplayName);
			return false;
		}
		return true;
	}

	static FString NormalizeNiagaraTemplateToken(const FString& Value)
	{
		FString Result;
		Result.Reserve(Value.Len());
		for (TCHAR Char : Value)
		{
			if (FChar::IsAlnum(Char))
			{
				Result.AppendChar(FChar::ToLower(Char));
			}
		}
		return Result;
	}

	static TArray<FString> ParseSearchTerms(const TArray<TSharedPtr<FJsonValue>>& Values)
	{
		TArray<FString> Terms;
		for (const TSharedPtr<FJsonValue>& Value : Values)
		{
			FString StringValue;
			if (Value.IsValid() && Value->TryGetString(StringValue) && !StringValue.IsEmpty())
			{
				Terms.Add(StringValue);
			}
		}
		return Terms;
	}

	static int32 ScoreTemplateEmitterCandidate(
		const FString& EffectType,
		const FString& VariantName,
		const FString& LayerName,
		const FString& PreferredArchetype,
		const TArray<FString>& ArchetypeSearchTerms,
		const FString& SystemPath,
		const FString& EmitterName,
		TArray<FString>& OutMatchedTerms)
	{
		const FString LayerNorm = NormalizeNiagaraTemplateToken(LayerName);
		const FString EffectNorm = NormalizeNiagaraTemplateToken(EffectType);
		const FString VariantNorm = NormalizeNiagaraTemplateToken(VariantName);
		const FString ArchetypeNorm = NormalizeNiagaraTemplateToken(PreferredArchetype);
		const FString SystemNorm = NormalizeNiagaraTemplateToken(SystemPath);
		const FString EmitterNorm = NormalizeNiagaraTemplateToken(EmitterName);
		const FString Combined = SystemNorm + TEXT(" ") + EmitterNorm;

		int32 Score = 0;
		if (!LayerNorm.IsEmpty() && (EmitterNorm.Contains(LayerNorm) || SystemNorm.Contains(LayerNorm)))
		{
			Score += 80;
			OutMatchedTerms.AddUnique(TEXT("layer_name"));
		}

		if (!EffectNorm.IsEmpty() && Combined.Contains(EffectNorm))
		{
			Score += 50;
			OutMatchedTerms.AddUnique(TEXT("effect_type"));
		}

		if (!VariantNorm.IsEmpty() && Combined.Contains(VariantNorm))
		{
			Score += 35;
			OutMatchedTerms.AddUnique(TEXT("variant"));
		}

		if (!ArchetypeNorm.IsEmpty() && Combined.Contains(ArchetypeNorm))
		{
			Score += 20;
			OutMatchedTerms.AddUnique(TEXT("archetype"));
		}

		for (const FString& SearchTerm : ArchetypeSearchTerms)
		{
			const FString TermNorm = NormalizeNiagaraTemplateToken(SearchTerm);
			if (!TermNorm.IsEmpty() && Combined.Contains(TermNorm))
			{
				Score += 20;
				OutMatchedTerms.AddUnique(SearchTerm);
			}
		}

		if (EmitterNorm.Contains(TEXT("template")) || SystemNorm.Contains(TEXT("template")))
		{
			Score += 10;
			OutMatchedTerms.AddUnique(TEXT("template"));
		}

		if (EmitterNorm.Contains(TEXT("fountain")) || SystemNorm.Contains(TEXT("fountain")))
		{
			Score += 8;
			OutMatchedTerms.AddUnique(TEXT("fountain"));
		}

		if (SystemNorm.Contains(TEXT("generated")) || SystemNorm.Contains(TEXT("audit")))
		{
			Score -= 25;
		}

		return Score;
	}

	static bool ResolveNiagaraTypeDefinition(const FString& ParamType, FNiagaraTypeDefinition& OutTypeDef)
	{
		if (ParamType.Equals(TEXT("float"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetFloatDef();
			return true;
		}
		if (ParamType.Equals(TEXT("int"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("int32"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetIntDef();
			return true;
		}
		if (ParamType.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetBoolDef();
			return true;
		}
		if (ParamType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("Vec3"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetVec3Def();
			return true;
		}
		if (ParamType.Equals(TEXT("LinearColor"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("Color"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetColorDef();
			return true;
		}
		if (ParamType.Equals(TEXT("MaterialInterface"), ESearchCase::IgnoreCase) ||
			ParamType.Equals(TEXT("Material"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition(UMaterialInterface::StaticClass());
			return true;
		}
		if (ParamType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition(UTexture::StaticClass());
			return true;
		}
		if (ParamType.Equals(TEXT("UObject"), ESearchCase::IgnoreCase) ||
			ParamType.Equals(TEXT("Object"), ESearchCase::IgnoreCase))
		{
			OutTypeDef = FNiagaraTypeDefinition::GetUObjectDef();
			return true;
		}
		return false;
	}

	static FString NiagaraEnumValueToString(const UEnum* Enum, int64 Value)
	{
		if (!Enum)
		{
			return TEXT("unknown");
		}

		return Enum->GetNameStringByValue(Value);
	}

	static TSharedPtr<FJsonObject> BoxToJsonObject(const FBox& Box)
	{
		TSharedPtr<FJsonObject> BoxObj = MakeShareable(new FJsonObject());
		const bool bBoxIsValid = Box.IsValid != 0;
		BoxObj->SetBoolField(TEXT("is_valid"), bBoxIsValid);
		if (bBoxIsValid)
		{
			BoxObj->SetStringField(
				TEXT("min"),
				FString::Printf(TEXT("(X=%.3f,Y=%.3f,Z=%.3f)"), Box.Min.X, Box.Min.Y, Box.Min.Z));
			BoxObj->SetStringField(
				TEXT("max"),
				FString::Printf(TEXT("(X=%.3f,Y=%.3f,Z=%.3f)"), Box.Max.X, Box.Max.Y, Box.Max.Z));
		}
		return BoxObj;
	}

	static bool TryParseVectorField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, FVector& OutVector, FString& OutError)
	{
		const TArray<TSharedPtr<FJsonValue>>* ArrayValue = nullptr;
		if (!Input.IsValid() || !Input->TryGetArrayField(FieldName, ArrayValue) || ArrayValue == nullptr || ArrayValue->Num() < 3)
		{
			OutError = FString::Printf(TEXT("Field '%s' must be an array with at least 3 numeric values."), FieldName);
			return false;
		}

		OutVector.X = static_cast<float>((*ArrayValue)[0]->AsNumber());
		OutVector.Y = static_cast<float>((*ArrayValue)[1]->AsNumber());
		OutVector.Z = static_cast<float>((*ArrayValue)[2]->AsNumber());
		return true;
	}

	static bool TryParseBoundsMode(const FString& BoundsModeString, ENiagaraEmitterCalculateBoundMode& OutBoundsMode)
	{
		if (BoundsModeString.Equals(TEXT("fixed"), ESearchCase::IgnoreCase))
		{
			OutBoundsMode = ENiagaraEmitterCalculateBoundMode::Fixed;
			return true;
		}
		if (BoundsModeString.Equals(TEXT("dynamic"), ESearchCase::IgnoreCase))
		{
			OutBoundsMode = ENiagaraEmitterCalculateBoundMode::Dynamic;
			return true;
		}
		if (BoundsModeString.Equals(TEXT("programmable"), ESearchCase::IgnoreCase))
		{
			OutBoundsMode = ENiagaraEmitterCalculateBoundMode::Programmable;
			return true;
		}
		return false;
	}

	static bool ResolveNiagaraEmitterTarget(
		UNiagaraSystem* NiagaraSystem,
		const TSharedPtr<FJsonObject>& Input,
		int32& OutEmitterIndex,
		FNiagaraEmitterHandle*& OutEmitterHandle,
		FVersionedNiagaraEmitterData*& OutEmitterData,
		FString& OutErrorCode,
		FString& OutErrorMessage)
	{
		OutEmitterIndex = INDEX_NONE;
		OutEmitterHandle = nullptr;
		OutEmitterData = nullptr;

		if (!NiagaraSystem)
		{
			OutErrorCode = TEXT("NI_SYSTEM_NOT_FOUND");
			OutErrorMessage = TEXT("Niagara System is null.");
			return false;
		}

		FString EmitterName;
		if (Input.IsValid())
		{
			Input->TryGetStringField(TEXT("emitter_name"), EmitterName);
			if (Input->HasTypedField<EJson::Number>(TEXT("emitter_index")))
			{
				OutEmitterIndex = static_cast<int32>(Input->GetNumberField(TEXT("emitter_index")));
			}
			else
			{
				FString EmitterIndexString;
				if (Input->TryGetStringField(TEXT("emitter_index"), EmitterIndexString) && !EmitterIndexString.IsEmpty())
				{
					OutEmitterIndex = FCString::Atoi(*EmitterIndexString);
				}
			}
		}

		TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
		if (EmitterHandles.Num() == 0)
		{
			OutErrorCode = TEXT("NI_NO_EMITTERS");
			OutErrorMessage = FString::Printf(TEXT("Niagara System '%s' has no emitters."), *NiagaraSystem->GetName());
			return false;
		}

		if (OutEmitterIndex != INDEX_NONE)
		{
			if (!EmitterHandles.IsValidIndex(OutEmitterIndex))
			{
				OutErrorCode = TEXT("NI_EMITTER_INDEX_OUT_OF_RANGE");
				OutErrorMessage = FString::Printf(TEXT("Emitter index %d is out of range for system '%s'."), OutEmitterIndex, *NiagaraSystem->GetName());
				return false;
			}
		}
		else if (!EmitterName.IsEmpty())
		{
			for (int32 Index = 0; Index < EmitterHandles.Num(); ++Index)
			{
				if (EmitterHandles[Index].GetName().ToString().Equals(EmitterName, ESearchCase::IgnoreCase))
				{
					OutEmitterIndex = Index;
					break;
				}
			}
			if (OutEmitterIndex == INDEX_NONE)
			{
				OutErrorCode = TEXT("NI_EMITTER_NOT_FOUND");
				OutErrorMessage = FString::Printf(TEXT("Emitter '%s' not found in Niagara System '%s'."), *EmitterName, *NiagaraSystem->GetName());
				return false;
			}
		}
		else
		{
			OutEmitterIndex = 0;
		}

		OutEmitterHandle = &EmitterHandles[OutEmitterIndex];
		OutEmitterData = OutEmitterHandle->GetEmitterData();
		if (!OutEmitterData)
		{
			OutErrorCode = TEXT("NI_EMITTER_DATA_MISSING");
			OutErrorMessage = FString::Printf(TEXT("Emitter '%s' has no emitter data."), *OutEmitterHandle->GetName().ToString());
			return false;
		}

		return true;
	}

	static FString MakeUniqueEmitterNameForSystem(UNiagaraSystem* NiagaraSystem, const FString& DesiredName)
	{
		const FString BaseName = DesiredName.TrimStartAndEnd().IsEmpty() ? TEXT("Emitter") : DesiredName.TrimStartAndEnd();
		if (!NiagaraSystem)
		{
			return BaseName;
		}

		TSet<FString> ExistingNames;
		for (const FNiagaraEmitterHandle& Handle : NiagaraSystem->GetEmitterHandles())
		{
			ExistingNames.Add(Handle.GetName().ToString());
		}

		if (!ExistingNames.Contains(BaseName))
		{
			return BaseName;
		}

		for (int32 Suffix = 1; Suffix < 10000; ++Suffix)
		{
			const FString Candidate = FString::Printf(TEXT("%s_%d"), *BaseName, Suffix);
			if (!ExistingNames.Contains(Candidate))
			{
				return Candidate;
			}
		}

		return FString::Printf(TEXT("%s_%s"), *BaseName, *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	}

	static bool ResolveNiagaraRendererTarget(
		FVersionedNiagaraEmitterData* EmitterData,
		const TSharedPtr<FJsonObject>& Input,
		int32& OutRendererIndex,
		UNiagaraRendererProperties*& OutRenderer,
		FString& OutErrorCode,
		FString& OutErrorMessage)
	{
		OutRendererIndex = INDEX_NONE;
		OutRenderer = nullptr;

		if (!EmitterData)
		{
			OutErrorCode = TEXT("NI_EMITTER_DATA_MISSING");
			OutErrorMessage = TEXT("Emitter data is null.");
			return false;
		}

		FString RendererName;
		if (Input.IsValid())
		{
			Input->TryGetStringField(TEXT("renderer_name"), RendererName);
			if (Input->HasTypedField<EJson::Number>(TEXT("renderer_index")))
			{
				OutRendererIndex = static_cast<int32>(Input->GetNumberField(TEXT("renderer_index")));
			}
			else
			{
				FString RendererIndexString;
				if (Input->TryGetStringField(TEXT("renderer_index"), RendererIndexString) && !RendererIndexString.IsEmpty())
				{
					OutRendererIndex = FCString::Atoi(*RendererIndexString);
				}
			}
		}

		const TArray<UNiagaraRendererProperties*>& Renderers = EmitterData->GetRenderers();
		if (Renderers.Num() == 0)
		{
			OutErrorCode = TEXT("NI_NO_RENDERERS");
			OutErrorMessage = TEXT("Emitter has no renderers.");
			return false;
		}

		if (OutRendererIndex != INDEX_NONE)
		{
			if (!Renderers.IsValidIndex(OutRendererIndex))
			{
				OutErrorCode = TEXT("NI_RENDERER_INDEX_OUT_OF_RANGE");
				OutErrorMessage = FString::Printf(TEXT("Renderer index %d is out of range."), OutRendererIndex);
				return false;
			}
		}
		else if (!RendererName.IsEmpty())
		{
			for (int32 Index = 0; Index < Renderers.Num(); ++Index)
			{
				const UNiagaraRendererProperties* Renderer = Renderers[Index];
				if (Renderer && Renderer->GetFName().ToString().Equals(RendererName, ESearchCase::IgnoreCase))
				{
					OutRendererIndex = Index;
					break;
				}
			}
			if (OutRendererIndex == INDEX_NONE)
			{
				OutErrorCode = TEXT("NI_RENDERER_NOT_FOUND");
				OutErrorMessage = FString::Printf(TEXT("Renderer '%s' not found on emitter."), *RendererName);
				return false;
			}
		}
		else
		{
			OutRendererIndex = 0;
		}

		OutRenderer = Renderers[OutRendererIndex];
		if (!OutRenderer)
		{
			OutErrorCode = TEXT("NI_RENDERER_MISSING");
			OutErrorMessage = FString::Printf(TEXT("Renderer at index %d is null."), OutRendererIndex);
			return false;
		}

		return true;
	}

	static UNiagaraNodeOutput* FindOutputNodeForScript(UNiagaraScript* Script)
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

	static bool ResolveParticleSpawnOutputNode(
		FVersionedNiagaraEmitterData* EmitterData,
		UNiagaraScript*& OutSpawnScript,
		UNiagaraNodeOutput*& OutOutputNode,
		FString& OutErrorMessage)
	{
		OutSpawnScript = nullptr;
		OutOutputNode = nullptr;

		if (!EmitterData)
		{
			OutErrorMessage = TEXT("Emitter data is null.");
			return false;
		}

		OutSpawnScript = EmitterData->SpawnScriptProps.Script;
		if (!OutSpawnScript)
		{
			OutErrorMessage = TEXT("Emitter does not have a particle spawn script.");
			return false;
		}

		OutOutputNode = FindOutputNodeForScript(OutSpawnScript);
		if (!OutOutputNode)
		{
			OutErrorMessage = TEXT("Could not resolve the particle spawn output node.");
			return false;
		}

		return true;
	}

	static bool AssignmentNodeContainsTargets(
		const UNiagaraNodeAssignment* AssignmentNode,
		const TArray<FNiagaraVariable>& RequiredTargets)
	{
		if (!AssignmentNode)
		{
			return false;
		}

		const TArray<FNiagaraVariable>& ExistingTargets = AssignmentNode->GetAssignmentTargets();
		for (const FNiagaraVariable& RequiredTarget : RequiredTargets)
		{
			bool bFoundMatch = false;
			for (const FNiagaraVariable& ExistingTarget : ExistingTargets)
			{
				if (ExistingTarget.GetName() == RequiredTarget.GetName() && ExistingTarget.GetType() == RequiredTarget.GetType())
				{
					bFoundMatch = true;
					break;
				}
			}

			if (!bFoundMatch)
			{
				return false;
			}
		}

		return true;
	}

	static UNiagaraNodeAssignment* FindAssignmentNodeByTargets(
		UNiagaraNodeOutput& OutputNode,
		const TArray<FNiagaraVariable>& RequiredTargets)
	{
		UEdGraph* Graph = OutputNode.GetGraph();
		if (!Graph)
		{
			return nullptr;
		}

		TArray<UNiagaraNodeAssignment*> AssignmentNodes;
		Graph->GetNodesOfClass<UNiagaraNodeAssignment>(AssignmentNodes);
		for (UNiagaraNodeAssignment* AssignmentNode : AssignmentNodes)
		{
			if (AssignmentNodeContainsTargets(AssignmentNode, RequiredTargets))
			{
				return AssignmentNode;
			}
		}

		return nullptr;
	}

	static bool EnsureAssignmentNodeTargets(
		UNiagaraNodeAssignment& AssignmentNode,
		const TArray<FNiagaraVariable>& TargetVariables,
		const TArray<FString>& DefaultExpressions,
		bool& bOutChanged,
		FString& OutErrorMessage)
	{
		bOutChanged = false;

		if (TargetVariables.Num() != DefaultExpressions.Num())
		{
			OutErrorMessage = TEXT("Target variable count did not match default expression count.");
			return false;
		}

		for (int32 Index = 0; Index < TargetVariables.Num(); ++Index)
		{
			const FNiagaraVariable& TargetVariable = TargetVariables[Index];
			const FString& DefaultExpression = DefaultExpressions[Index];
			int32 ExistingTargetIndex = AssignmentNode.FindAssignmentTarget(TargetVariable.GetName(), TargetVariable.GetType());
			if (ExistingTargetIndex == INDEX_NONE)
			{
				AssignmentNode.AddAssignmentTarget(TargetVariable, &DefaultExpression);
				bOutChanged = true;
				continue;
			}

			if (AssignmentNode.SetAssignmentTarget(ExistingTargetIndex, TargetVariable, &DefaultExpression))
			{
				bOutChanged = true;
			}
		}

		if (bOutChanged)
		{
			AssignmentNode.RefreshFromExternalChanges();
		}

		return true;
	}

	static void AppendMaterialParameterCounts(TSharedPtr<FJsonObject> Target, const FNiagaraRendererMaterialParameters& MaterialParameters)
	{
		if (!Target.IsValid())
		{
			return;
		}

		Target->SetNumberField(TEXT("material_attribute_binding_count"), MaterialParameters.AttributeBindings.Num());
		Target->SetNumberField(TEXT("material_scalar_parameter_count"), MaterialParameters.ScalarParameters.Num());
		Target->SetNumberField(TEXT("material_vector_parameter_count"), MaterialParameters.VectorParameters.Num());
		Target->SetNumberField(TEXT("material_texture_parameter_count"), MaterialParameters.TextureParameters.Num());
		Target->SetNumberField(TEXT("material_static_bool_parameter_count"), MaterialParameters.StaticBoolParameters.Num());
	}

	static FString NormalizeNiagaraUserParameterName(const FString& RawName)
	{
		FString Trimmed = RawName;
		Trimmed.TrimStartAndEndInline();
		if (Trimmed.IsEmpty())
		{
			return Trimmed;
		}
		if (!Trimmed.StartsWith(TEXT("User."), ESearchCase::IgnoreCase))
		{
			Trimmed = FString::Printf(TEXT("User.%s"), *Trimmed);
		}
		return Trimmed;
	}

	static void ParseNiagaraFloatList(const FString& Str, TArray<float>& OutValues)
	{
		FString Trimmed = Str;
		Trimmed = Trimmed.Replace(TEXT("["), TEXT("")).Replace(TEXT("]"), TEXT(""));
		TArray<FString> Parts;
		Trimmed.ParseIntoArray(Parts, TEXT(","));
		for (const FString& Part : Parts)
		{
			OutValues.Add(FCString::Atof(*Part.TrimStartAndEnd()));
		}
	}

	static bool SetNiagaraParameterStoreValue(
		FNiagaraParameterStore& ParameterStore,
		const FNiagaraVariable& Parameter,
		const FString& ParamType,
		const FString& ValueStr,
		TSharedPtr<FJsonObject> Result,
		FString& OutErrorCode,
		FString& OutErrorMessage)
	{
		if (ParamType.Equals(TEXT("float"), ESearchCase::IgnoreCase))
		{
			FNiagaraVariable ValueVar(Parameter.GetType(), Parameter.GetName());
			ValueVar.SetValue<float>(FCString::Atof(*ValueStr));
			ParameterStore.SetParameterData(ValueVar.GetData(), Parameter);
			return true;
		}

		if (ParamType.Equals(TEXT("int"), ESearchCase::IgnoreCase))
		{
			FNiagaraVariable ValueVar(Parameter.GetType(), Parameter.GetName());
			ValueVar.SetValue<int32>(FCString::Atoi(*ValueStr));
			ParameterStore.SetParameterData(ValueVar.GetData(), Parameter);
			return true;
		}

		if (ParamType.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
		{
			FNiagaraVariable ValueVar(Parameter.GetType(), Parameter.GetName());
			ValueVar.SetValue<FNiagaraBool>(FNiagaraBool(ValueStr.Equals(TEXT("true"), ESearchCase::IgnoreCase) || ValueStr == TEXT("1")));
			ParameterStore.SetParameterData(ValueVar.GetData(), Parameter);
			return true;
		}

		if (ParamType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase))
		{
			TArray<float> Values;
			ParseNiagaraFloatList(ValueStr, Values);
			if (Values.Num() < 3)
			{
				OutErrorCode = TEXT("NI_INVALID_VECTOR");
				OutErrorMessage = TEXT("Vector value must have at least 3 components: [X,Y,Z]");
				return false;
			}

			FNiagaraVariable ValueVar(Parameter.GetType(), Parameter.GetName());
			ValueVar.SetValue<FVector3f>(FVector3f(Values[0], Values[1], Values[2]));
			ParameterStore.SetParameterData(ValueVar.GetData(), Parameter);
			return true;
		}

		if (ParamType.Equals(TEXT("LinearColor"), ESearchCase::IgnoreCase))
		{
			TArray<float> Values;
			ParseNiagaraFloatList(ValueStr, Values);
			if (Values.Num() < 3)
			{
				OutErrorCode = TEXT("NI_INVALID_COLOR");
				OutErrorMessage = TEXT("LinearColor value must have at least 3 components: [R,G,B] or [R,G,B,A]");
				return false;
			}

			FNiagaraVariable ValueVar(Parameter.GetType(), Parameter.GetName());
			ValueVar.SetValue<FLinearColor>(FLinearColor(Values[0], Values[1], Values[2], Values.Num() >= 4 ? Values[3] : 1.0f));
			ParameterStore.SetParameterData(ValueVar.GetData(), Parameter);
			return true;
		}

		if (ParamType.Equals(TEXT("MaterialInterface"), ESearchCase::IgnoreCase) ||
			ParamType.Equals(TEXT("Material"), ESearchCase::IgnoreCase))
		{
			UMaterialInterface* Material = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(ValueStr));
			if (!Material)
			{
				OutErrorCode = TEXT("NI_INVALID_MATERIAL");
				OutErrorMessage = FString::Printf(TEXT("MaterialInterface asset not found at '%s'."), *ValueStr);
				return false;
			}

			ParameterStore.SetUObject(Material, Parameter);
			if (Result.IsValid())
			{
				Result->SetStringField(TEXT("resolved_asset_path"), Material->GetPathName());
				Result->SetStringField(TEXT("resolved_asset_class"), Material->GetClass()->GetName());
			}
			return true;
		}

		if (ParamType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
		{
			UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(ValueStr));
			if (!Texture)
			{
				OutErrorCode = TEXT("NI_INVALID_TEXTURE");
				OutErrorMessage = FString::Printf(TEXT("Texture asset not found at '%s'."), *ValueStr);
				return false;
			}

			ParameterStore.SetUObject(Texture, Parameter);
			if (Result.IsValid())
			{
				Result->SetStringField(TEXT("resolved_asset_path"), Texture->GetPathName());
				Result->SetStringField(TEXT("resolved_asset_class"), Texture->GetClass()->GetName());
			}
			return true;
		}

		if (ParamType.Equals(TEXT("UObject"), ESearchCase::IgnoreCase) ||
			ParamType.Equals(TEXT("Object"), ESearchCase::IgnoreCase))
		{
			UObject* ObjectValue = UEditorAssetLibrary::LoadAsset(ValueStr);
			if (!ObjectValue)
			{
				OutErrorCode = TEXT("NI_INVALID_OBJECT");
				OutErrorMessage = FString::Printf(TEXT("UObject asset not found at '%s'."), *ValueStr);
				return false;
			}

			ParameterStore.SetUObject(ObjectValue, Parameter);
			if (Result.IsValid())
			{
				Result->SetStringField(TEXT("resolved_asset_path"), ObjectValue->GetPathName());
				Result->SetStringField(TEXT("resolved_asset_class"), ObjectValue->GetClass()->GetName());
			}
			return true;
		}

		OutErrorCode = TEXT("NI_UNSUPPORTED_PARAM_TYPE");
		OutErrorMessage = FString::Printf(TEXT("Unsupported param_type '%s'. Use float, int, bool, Vector, LinearColor, MaterialInterface, Texture, or UObject."), *ParamType);
		return false;
	}

	static FString DescribeNiagaraType(const FNiagaraTypeDefinition& TypeDef)
	{
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetFloatDef()))
		{
			return TEXT("float");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetIntDef()))
		{
			return TEXT("int");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetBoolDef()))
		{
			return TEXT("bool");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetVec3Def()))
		{
			return TEXT("Vector");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetColorDef()))
		{
			return TEXT("LinearColor");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition(UMaterialInterface::StaticClass())))
		{
			return TEXT("MaterialInterface");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition(UTexture::StaticClass())))
		{
			return TEXT("Texture");
		}
		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetUObjectDef()))
		{
			return TEXT("UObject");
		}
		return TypeDef.GetName();
	}

	static TSharedPtr<FJsonObject> BuildNiagaraParameterValueObject(
		const FNiagaraParameterStore& ParameterStore,
		const FNiagaraVariableWithOffset& Parameter)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		const FNiagaraTypeDefinition& TypeDef = Parameter.GetType();
		const FString FriendlyType = DescribeNiagaraType(TypeDef);
		const FNiagaraVariable ParameterVar(TypeDef, Parameter.GetName());

		Result->SetStringField(TEXT("name"), Parameter.GetName().ToString());
		Result->SetStringField(TEXT("type"), FriendlyType);
		Result->SetStringField(TEXT("type_namespace"), TypeDef.GetNameText().ToString());
		Result->SetNumberField(TEXT("size_bytes"), Parameter.GetSizeInBytes());
		Result->SetBoolField(TEXT("is_object_backed"), TypeDef.GetClass() != nullptr);

		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetFloatDef()))
		{
			Result->SetStringField(TEXT("value_kind"), TEXT("float"));
			Result->SetNumberField(TEXT("value"), ParameterStore.GetParameterValue<float>(ParameterVar));
			return Result;
		}

		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetIntDef()))
		{
			Result->SetStringField(TEXT("value_kind"), TEXT("int"));
			Result->SetNumberField(TEXT("value"), ParameterStore.GetParameterValue<int32>(ParameterVar));
			return Result;
		}

		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetBoolDef()))
		{
			const FNiagaraBool BoolValue = ParameterStore.GetParameterValue<FNiagaraBool>(ParameterVar);
			Result->SetStringField(TEXT("value_kind"), TEXT("bool"));
			Result->SetBoolField(TEXT("value"), BoolValue.GetValue());
			Result->SetNumberField(TEXT("raw_value"), BoolValue.GetRawValue());
			return Result;
		}

		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetVec3Def()))
		{
			const FVector3f VecValue = ParameterStore.GetParameterValue<FVector3f>(ParameterVar);
			TArray<TSharedPtr<FJsonValue>> ValueArray;
			ValueArray.Add(MakeShareable(new FJsonValueNumber(VecValue.X)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(VecValue.Y)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(VecValue.Z)));
			Result->SetStringField(TEXT("value_kind"), TEXT("Vector"));
			Result->SetArrayField(TEXT("value"), ValueArray);
			return Result;
		}

		if (TypeDef.IsSameBaseDefinition(FNiagaraTypeDefinition::GetColorDef()))
		{
			const FLinearColor ColorValue = ParameterStore.GetParameterValue<FLinearColor>(ParameterVar);
			TArray<TSharedPtr<FJsonValue>> ValueArray;
			ValueArray.Add(MakeShareable(new FJsonValueNumber(ColorValue.R)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(ColorValue.G)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(ColorValue.B)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(ColorValue.A)));
			Result->SetStringField(TEXT("value_kind"), TEXT("LinearColor"));
			Result->SetArrayField(TEXT("value"), ValueArray);
			return Result;
		}

		if (TypeDef.GetClass() != nullptr)
		{
			UObject* ObjectValue = ParameterStore.GetUObject(ParameterVar);
			Result->SetStringField(TEXT("value_kind"), TEXT("object"));
			Result->SetStringField(TEXT("value_object_path"), ObjectValue ? ObjectValue->GetPathName() : TEXT(""));
			Result->SetStringField(TEXT("value_object_class"), ObjectValue ? ObjectValue->GetClass()->GetName() : TEXT(""));
			Result->SetBoolField(TEXT("value_is_null"), ObjectValue == nullptr);
			return Result;
		}

		const uint8* RawData = ParameterStore.GetParameterData(ParameterVar);
		Result->SetStringField(TEXT("value_kind"), TEXT("raw_bytes"));
		if (RawData && Parameter.GetSizeInBytes() > 0)
		{
			TArray<TSharedPtr<FJsonValue>> ByteValues;
			const int32 ByteCount = FMath::Min(Parameter.GetSizeInBytes(), 32);
			for (int32 Index = 0; Index < ByteCount; ++Index)
			{
				ByteValues.Add(MakeShareable(new FJsonValueNumber(static_cast<int32>(RawData[Index]))));
			}
			Result->SetArrayField(TEXT("value_bytes"), ByteValues);
			Result->SetNumberField(TEXT("reported_byte_count"), ByteCount);
		}

		return Result;
	}

	static TArray<TSharedPtr<FJsonValue>> BuildNiagaraParameterValueArray(
		const FNiagaraParameterStore& ParameterStore,
		const FString& ParamNameFilter = FString())
	{
		const FString NormalizedFilter = ParamNameFilter.IsEmpty() ? FString() : NormalizeNiagaraUserParameterName(ParamNameFilter);
		TArray<TSharedPtr<FJsonValue>> ParamArray;
		for (const FNiagaraVariableWithOffset& Parameter : ParameterStore.ReadParameterVariables())
		{
			if (!NormalizedFilter.IsEmpty() &&
				!Parameter.GetName().ToString().Equals(NormalizedFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}

			ParamArray.Add(MakeShareable(new FJsonValueObject(BuildNiagaraParameterValueObject(ParameterStore, Parameter))));
		}
		return ParamArray;
	}

	static TSharedPtr<FJsonObject> BuildNiagaraRendererDetailsObject(
		UNiagaraSystem* NiagaraSystem,
		FNiagaraEmitterHandle* EmitterHandle,
		FVersionedNiagaraEmitterData* EmitterData,
		int32 EmitterIndex,
		UNiagaraRendererProperties* Renderer,
		int32 RendererIndex)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		const UEnum* SimTargetEnum = StaticEnum<ENiagaraSimTarget>();
		const UEnum* SourceModeEnum = StaticEnum<ENiagaraRendererSourceDataMode>();

		TArray<UMaterialInterface*> UsedMaterials;
		Renderer->GetUsedMaterials(nullptr, UsedMaterials);
		TArray<TSharedPtr<FJsonValue>> UsedMaterialArray;
		for (UMaterialInterface* UsedMaterial : UsedMaterials)
		{
			if (UsedMaterial)
			{
				UsedMaterialArray.Add(MakeShareable(new FJsonValueString(UsedMaterial->GetPathName())));
			}
		}

		Result->SetBoolField(TEXT("success"), true);
		Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
		Result->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
		Result->SetNumberField(TEXT("emitter_index"), EmitterIndex);
		Result->SetStringField(TEXT("emitter_name"), EmitterHandle ? EmitterHandle->GetName().ToString() : TEXT(""));
		Result->SetNumberField(TEXT("renderer_index"), RendererIndex);
		Result->SetStringField(TEXT("renderer_class"), Renderer->GetClass()->GetName());
		Result->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
		Result->SetBoolField(TEXT("enabled"), Renderer->GetIsEnabled());
		Result->SetBoolField(TEXT("supports_emitter_sim_target"), EmitterData ? Renderer->IsSimTargetSupported(EmitterData->SimTarget) : false);
		Result->SetBoolField(TEXT("needs_mids_for_materials"), Renderer->NeedsMIDsForMaterials());
		Result->SetStringField(TEXT("source_mode"), NiagaraEnumValueToString(SourceModeEnum, static_cast<int64>(Renderer->GetCurrentSourceMode())));
		Result->SetNumberField(TEXT("sort_order_hint"), Renderer->SortOrderHint);
		Result->SetStringField(TEXT("sim_target"), EmitterData ? NiagaraEnumValueToString(SimTargetEnum, static_cast<int64>(EmitterData->SimTarget)) : TEXT("unknown"));
		Result->SetArrayField(TEXT("used_materials"), UsedMaterialArray);
		Result->SetNumberField(TEXT("used_material_count"), UsedMaterialArray.Num());
		Result->SetStringField(TEXT("renderer_enabled_binding"), Renderer->RendererEnabledBinding.GetParamMapBindableVariable().GetName().ToString());

		if (const UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer))
		{
			Result->SetStringField(TEXT("renderer_type"), TEXT("sprite"));
			Result->SetStringField(TEXT("material_path"), SpriteRenderer->Material ? SpriteRenderer->Material->GetPathName() : TEXT(""));
			Result->SetStringField(TEXT("material_user_param"), SpriteRenderer->MaterialUserParamBinding.Parameter.GetName().ToString());
			AppendMaterialParameterCounts(Result, SpriteRenderer->MaterialParameters);
		}
		else if (const UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer))
		{
			Result->SetStringField(TEXT("renderer_type"), TEXT("ribbon"));
			Result->SetStringField(TEXT("material_path"), RibbonRenderer->Material ? RibbonRenderer->Material->GetPathName() : TEXT(""));
			Result->SetStringField(TEXT("material_user_param"), RibbonRenderer->MaterialUserParamBinding.Parameter.GetName().ToString());
			Result->SetStringField(TEXT("ribbon_id_binding"), RibbonRenderer->RibbonIdBinding.GetParamMapBindableVariable().GetName().ToString());
			Result->SetStringField(TEXT("ribbon_link_order_binding"), RibbonRenderer->RibbonLinkOrderBinding.GetParamMapBindableVariable().GetName().ToString());
			Result->SetBoolField(TEXT("ribbon_link_order_use_unique_id_fallback"), RibbonRenderer->bLinkOrderUseUniqueID != 0);
			AppendMaterialParameterCounts(Result, RibbonRenderer->MaterialParameters);
		}
		else if (const UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
		{
			Result->SetStringField(TEXT("renderer_type"), TEXT("mesh"));
			Result->SetNumberField(TEXT("mesh_count"), MeshRenderer->Meshes.Num());
			Result->SetBoolField(TEXT("override_materials_enabled"), MeshRenderer->bOverrideMaterials);
			Result->SetNumberField(TEXT("override_material_count"), MeshRenderer->OverrideMaterials.Num());
			AppendMaterialParameterCounts(Result, MeshRenderer->MaterialParameters);

			TArray<TSharedPtr<FJsonValue>> MeshArray;
			for (int32 MeshIndex = 0; MeshIndex < MeshRenderer->Meshes.Num(); ++MeshIndex)
			{
				const FNiagaraMeshRendererMeshProperties& MeshProps = MeshRenderer->Meshes[MeshIndex];
				TSharedPtr<FJsonObject> MeshObj = MakeShareable(new FJsonObject());
				MeshObj->SetNumberField(TEXT("index"), MeshIndex);
				MeshObj->SetStringField(TEXT("mesh_path"), MeshProps.Mesh ? MeshProps.Mesh->GetPathName() : TEXT(""));
				MeshArray.Add(MakeShareable(new FJsonValueObject(MeshObj)));
			}
			Result->SetArrayField(TEXT("meshes"), MeshArray);

			TArray<TSharedPtr<FJsonValue>> OverrideArray;
			for (int32 OverrideIndex = 0; OverrideIndex < MeshRenderer->OverrideMaterials.Num(); ++OverrideIndex)
			{
				const FNiagaraMeshMaterialOverride& Override = MeshRenderer->OverrideMaterials[OverrideIndex];
				TSharedPtr<FJsonObject> OverrideObj = MakeShareable(new FJsonObject());
				OverrideObj->SetNumberField(TEXT("index"), OverrideIndex);
				OverrideObj->SetStringField(TEXT("explicit_material_path"), Override.ExplicitMat ? Override.ExplicitMat->GetPathName() : TEXT(""));
				OverrideObj->SetStringField(TEXT("user_param_binding"), Override.UserParamBinding.Parameter.GetName().ToString());
				OverrideArray.Add(MakeShareable(new FJsonValueObject(OverrideObj)));
			}
			Result->SetArrayField(TEXT("override_materials"), OverrideArray);
		}
		else
		{
			Result->SetStringField(TEXT("renderer_type"), TEXT("generic"));
		}

		return Result;
	}

	struct FNiagaraRendererCleanupPlan
	{
		FGuid EmitterId;
		FString EmitterName;
		int32 EmitterIndex = INDEX_NONE;
		int32 RendererIndex = INDEX_NONE;
		FString RendererName;
		FString RendererClass;
		FString Reason;
		TObjectPtr<UNiagaraRendererProperties> Renderer = nullptr;
	};

	struct FNiagaraEmitterCleanupPlan
	{
		FGuid EmitterId;
		FString EmitterName;
		int32 EmitterIndex = INDEX_NONE;
		FString Reason;
	};

	static TSharedPtr<FJsonObject> BuildNiagaraRendererCleanupObject(const FNiagaraRendererCleanupPlan& Plan)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetStringField(TEXT("emitter_id"), Plan.EmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));
		Result->SetStringField(TEXT("emitter_name"), Plan.EmitterName);
		Result->SetNumberField(TEXT("emitter_index"), Plan.EmitterIndex);
		Result->SetNumberField(TEXT("renderer_index"), Plan.RendererIndex);
		Result->SetStringField(TEXT("renderer_name"), Plan.RendererName);
		Result->SetStringField(TEXT("renderer_class"), Plan.RendererClass);
		Result->SetStringField(TEXT("reason"), Plan.Reason);
		return Result;
	}

	static TSharedPtr<FJsonObject> BuildNiagaraEmitterCleanupObject(const FNiagaraEmitterCleanupPlan& Plan)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetStringField(TEXT("emitter_id"), Plan.EmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));
		Result->SetStringField(TEXT("emitter_name"), Plan.EmitterName);
		Result->SetNumberField(TEXT("emitter_index"), Plan.EmitterIndex);
		Result->SetStringField(TEXT("reason"), Plan.Reason);
		return Result;
	}

	static TSharedPtr<FJsonObject> BuildNiagaraSystemContextObject(UNiagaraSystem* NiagaraSystem)
	{
		if (!NiagaraSystem)
		{
			return MakeShareable(new FJsonObject());
		}

		TSharedPtr<FJsonObject> SystemObj = MakeShareable(new FJsonObject());
		SystemObj->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
		SystemObj->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
		SystemObj->SetStringField(TEXT("asset_class"), NiagaraSystem->GetClass()->GetName());

		const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
		int32 EnabledEmitterCount = 0;
		TArray<TSharedPtr<FJsonValue>> EmitterNameArray;
		for (const FNiagaraEmitterHandle& Handle : EmitterHandles)
		{
			if (Handle.GetIsEnabled())
			{
				++EnabledEmitterCount;
			}
			EmitterNameArray.Add(MakeShareable(new FJsonValueString(Handle.GetName().ToString())));
		}

		const TArrayView<const FNiagaraVariableWithOffset> UserParams = NiagaraSystem->GetExposedParameters().ReadParameterVariables();
		TArray<TSharedPtr<FJsonValue>> UserParamNameArray;
		for (const FNiagaraVariableWithOffset& Param : UserParams)
		{
			UserParamNameArray.Add(MakeShareable(new FJsonValueString(Param.GetName().ToString())));
		}

		SystemObj->SetNumberField(TEXT("emitter_count"), EmitterHandles.Num());
		SystemObj->SetNumberField(TEXT("enabled_emitter_count"), EnabledEmitterCount);
		SystemObj->SetNumberField(TEXT("user_parameter_count"), UserParams.Num());
		SystemObj->SetArrayField(TEXT("emitter_names"), EmitterNameArray);
		SystemObj->SetArrayField(TEXT("user_parameter_names"), UserParamNameArray);
		return SystemObj;
	}

	static FString NiagaraStackStageLabel(ENiagaraScriptUsage Usage)
	{
		switch (Usage)
		{
		case ENiagaraScriptUsage::EmitterSpawnScript:
			return TEXT("emitter_spawn");
		case ENiagaraScriptUsage::EmitterUpdateScript:
			return TEXT("emitter_update");
		case ENiagaraScriptUsage::ParticleSpawnScript:
			return TEXT("particle_spawn");
		case ENiagaraScriptUsage::ParticleUpdateScript:
			return TEXT("particle_update");
		case ENiagaraScriptUsage::ParticleSimulationStageScript:
			return TEXT("simulation_stage");
		default:
			return NiagaraEnumValueToString(StaticEnum<ENiagaraScriptUsage>(), static_cast<int64>(Usage));
		}
	}

	static void AppendNiagaraStackFinding(
		TArray<TSharedPtr<FJsonValue>>& Findings,
		const FString& Severity,
		const FString& Stage,
		const FString& Finding,
		const FString& Reason)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("stage"), Stage);
		FindingObj->SetStringField(TEXT("finding"), Finding);
		FindingObj->SetStringField(TEXT("reason"), Reason);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	}

	static TSharedPtr<FJsonObject> BuildNiagaraStackStageObject(
		UNiagaraScript* Script,
		const FString& StageLabel,
		const bool bIncludeNodes,
		TArray<TSharedPtr<FJsonValue>>& Findings)
	{
		TSharedPtr<FJsonObject> StageObj = MakeShareable(new FJsonObject());
		StageObj->SetStringField(TEXT("stage"), StageLabel);
		StageObj->SetStringField(TEXT("script_path"), Script ? Script->GetPathName() : TEXT(""));
		StageObj->SetStringField(TEXT("script_name"), Script ? Script->GetName() : TEXT(""));

		if (!Script)
		{
			StageObj->SetBoolField(TEXT("available"), false);
			StageObj->SetStringField(TEXT("message"), TEXT("Script is missing for this stage."));
			AppendNiagaraStackFinding(Findings, TEXT("warning"), StageLabel, TEXT("missing_script"), TEXT("Stage has no script object."));
			return StageObj;
		}

		UNiagaraScriptSource* ScriptSource = Cast<UNiagaraScriptSource>(Script->GetLatestSource());
		if (!ScriptSource || !ScriptSource->NodeGraph)
		{
			StageObj->SetBoolField(TEXT("available"), false);
			StageObj->SetStringField(TEXT("message"), TEXT("Script source graph is unavailable."));
			AppendNiagaraStackFinding(Findings, TEXT("warning"), StageLabel, TEXT("missing_graph"), TEXT("Script exists but no Niagara graph source was available."));
			return StageObj;
		}

		TArray<UNiagaraNode*> Traversal;
		for (UEdGraphNode* GraphNode : ScriptSource->NodeGraph->Nodes)
		{
			if (UNiagaraNode* NiagaraNode = Cast<UNiagaraNode>(GraphNode))
			{
				Traversal.Add(NiagaraNode);
			}
		}

		TArray<TSharedPtr<FJsonValue>> ModuleArray;
		TArray<TSharedPtr<FJsonValue>> NodeArray;
		int32 FunctionCallCount = 0;
		int32 CustomHlslCount = 0;
		int32 InputCount = 0;

		for (UNiagaraNode* Node : Traversal)
		{
			if (!Node)
			{
				continue;
			}

			if (UNiagaraNodeFunctionCall* FunctionCallNode = Cast<UNiagaraNodeFunctionCall>(Node))
			{
				++FunctionCallCount;
				TSharedPtr<FJsonObject> ModuleObj = MakeShareable(new FJsonObject());
				ModuleObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
				ModuleObj->SetStringField(TEXT("title"), Node->GetNodeTitle(ENodeTitleType::ListView).ToString());
				ModuleObj->SetStringField(TEXT("function_name"), FunctionCallNode->GetFunctionName());
				ModuleObj->SetStringField(TEXT("function_script_path"),
					FunctionCallNode->FunctionScript ? FunctionCallNode->FunctionScript->GetPathName() : TEXT(""));
				ModuleArray.Add(MakeShareable(new FJsonValueObject(ModuleObj)));
			}
			else if (Cast<UNiagaraNodeCustomHlsl>(Node))
			{
				++CustomHlslCount;
			}
			else if (Cast<UNiagaraNodeInput>(Node))
			{
				++InputCount;
			}

			if (bIncludeNodes)
			{
				TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
				NodeObj->SetStringField(TEXT("node_class"), Node->GetClass()->GetName());
				NodeObj->SetStringField(TEXT("title"), Node->GetNodeTitle(ENodeTitleType::ListView).ToString());
				NodeArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));
			}
		}

		StageObj->SetBoolField(TEXT("available"), true);
		StageObj->SetNumberField(TEXT("traversal_node_count"), Traversal.Num());
		StageObj->SetNumberField(TEXT("function_call_count"), FunctionCallCount);
		StageObj->SetNumberField(TEXT("custom_hlsl_count"), CustomHlslCount);
		StageObj->SetNumberField(TEXT("input_count"), InputCount);
		StageObj->SetArrayField(TEXT("modules"), ModuleArray);
		if (bIncludeNodes)
		{
			StageObj->SetArrayField(TEXT("nodes"), NodeArray);
		}

		if (FunctionCallCount == 0)
		{
			AppendNiagaraStackFinding(
				Findings,
				TEXT("warning"),
				StageLabel,
				TEXT("no_function_calls"),
				TEXT("Traversal contained no Niagara function call nodes. This stage may be effectively empty or built from only inputs/outputs."));
		}

		return StageObj;
	}
}

// ============================================================
// Registration
// ============================================================

void FVFXToolRegistry::RegisterNiagaraTools()
{
	// ---- Tool: create_empty_niagara_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_empty_niagara_system");
		Def.Description = TEXT(
			"Create a brand new empty Niagara System asset. "
			"Use this as the preferred starting point for authored Niagara work instead of duplicating an existing system.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_name"),
			MakeStringProperty(TEXT("Name for the empty Niagara System asset.")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Content Browser folder path (default: /Game/).")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to create the asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateEmptyNiagaraSystem);
		RegisterTool(Def);
	}

	// ---- Tool: create_niagara_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_niagara_system");
		Def.Description = TEXT(
			"Create a new empty Niagara System asset with optional user-exposed parameters. "
			"The user should then open the system in the Niagara Editor to add emitters and configure modules.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_name"),
			MakeStringProperty(TEXT("Name for the Niagara System asset")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Content Browser folder path (default: /Game/)")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to create asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateNiagaraSystem);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_system_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("find_niagara_template_emitters");
		Def.Description = TEXT(
			"Find reusable Niagara template emitters for each recipe layer inferred from a freeform effect description. "
			"This is the preferred template-discovery step before add_niagara_emitter_to_system.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("description"),
			MakeStringProperty(TEXT("Freeform effect description such as 'campfire with embers and smoke'.")));
		Props->SetObjectField(TEXT("search_root"),
			MakeStringProperty(TEXT("Optional content root to search. Default /Game/.")));
		Props->SetObjectField(TEXT("max_candidates_per_layer"),
			MakeStringProperty(TEXT("Optional max candidate emitters to return per layer. Default 5.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("description"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FindNiagaraTemplateEmitters);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_system_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_system_info");
		Def.Description = TEXT(
			"Get information about an existing Niagara System: emitter names, user parameters, and their types.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraSystemInfo);
		RegisterTool(Def);
	}

	// ---- Tool: spawn_niagara_at_location ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("spawn_niagara_at_location");
		Def.Description = TEXT(
			"Spawn a Niagara System at a specific world location in the editor level. "
			"Creates an Actor with a NiagaraComponent set to the specified system.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System to spawn")));
		Props->SetObjectField(TEXT("location"),
			MakeStringProperty(TEXT("[X, Y, Z] world coordinates as array (default: [0,0,0])")));
		Props->SetObjectField(TEXT("rotation"),
			MakeStringProperty(TEXT("[Pitch, Yaw, Roll] in degrees as array (default: [0,0,0])")));
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Optional label for the spawned actor")));
		Props->SetObjectField(TEXT("auto_activate"),
			MakeStringProperty(TEXT("true or false (default: true)")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to spawn actor.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SpawnNiagaraAtLocation);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_emitter_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_emitter_details");
		Def.Description = TEXT(
			"Get deep details for a single emitter inside a Niagara System: sim target, bounds mode, fixed bounds, "
			"event/simulation stage counts, renderer compatibility, and used materials.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index. Use this when names are duplicated or omitted.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraEmitterDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_renderer_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_renderer_details");
		Def.Description = TEXT(
			"Get deep details for a single renderer inside a Niagara emitter: renderer type, materials, "
			"renderer enable state, sim-target compatibility, and renderer-specific material bindings.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraRendererDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_editor_context ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_editor_context");
		Def.Description = TEXT(
			"Return Niagara Systems currently selected in the Content Browser or open in asset editors, "
			"plus a preferred system path for follow-up inspection tools.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("include_system_info"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include emitter/user parameter counts for returned systems.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraEditorContext);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_user_parameter_defaults ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_user_parameter_defaults");
		Def.Description = TEXT(
			"Read current exposed-user-parameter default values from a Niagara System asset. "
			"Supports float, int, bool, Vector, LinearColor, MaterialInterface, Texture, and UObject values.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Optional single user parameter name to filter to. 'User.' prefix is optional.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraUserParameterDefaults);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_actor_user_parameter_values ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_actor_user_parameter_values");
		Def.Description = TEXT(
			"Read current Niagara user-parameter values from a spawned actor's Niagara Component override store. "
			"Useful for verifying runtime overrides after set_niagara_user_parameter.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Exact World Outliner label of the actor holding the Niagara Component.")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Optional single user parameter name to filter to. 'User.' prefix is optional.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraActorUserParameterValues);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_mutation_snapshots ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_mutation_snapshots");
		Def.Description = TEXT(
			"List saved mutation snapshots for a Niagara System, newest first. Use this before restore_niagara_mutation_snapshot.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max number of snapshots to return. Default 20.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraMutationSnapshots);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_user_parameter ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_user_parameter");
		Def.Description = TEXT(
			"Set a user-exposed parameter value on a Niagara Component that exists on an actor in the level. "
			"Supports float, int, bool, Vector, LinearColor, MaterialInterface, Texture, and UObject parameter types.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Label of the actor that has the Niagara Component")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Name of the user parameter to set")));
		Props->SetObjectField(TEXT("param_type"),
			MakeStringProperty(TEXT("Parameter type: float, int, bool, Vector, LinearColor, MaterialInterface, Texture, UObject")));
		Props->SetObjectField(TEXT("value"),
			MakeStringProperty(TEXT("Value as string. For float/int: number, for bool: true/false, for Vector: '[X,Y,Z]', for LinearColor: '[R,G,B,A]', for MaterialInterface/Texture/UObject: full asset path.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to set parameter on component.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_type"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraUserParameter);
		RegisterTool(Def);
	}

	// ---- Tool: add_niagara_user_parameter ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_niagara_user_parameter");
		Def.Description = TEXT(
			"Add an exposed user parameter to a Niagara System asset. "
			"Supports float, int, bool, Vector, LinearColor, MaterialInterface, Texture, and UObject.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("User parameter name. 'User.' prefix is optional.")));
		Props->SetObjectField(TEXT("param_type"),
			MakeStringProperty(TEXT("Type: float, int, bool, Vector, LinearColor, MaterialInterface, Texture, UObject")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to modify system asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_type"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddNiagaraUserParameter);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_user_parameter_default ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_user_parameter_default");
		Def.Description = TEXT(
			"Set the default value of an exposed Niagara user parameter on a Niagara System asset. "
			"Supports float, int, bool, Vector, LinearColor, MaterialInterface, Texture, and UObject.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("User parameter name. 'User.' prefix is optional.")));
		Props->SetObjectField(TEXT("param_type"),
			MakeStringProperty(TEXT("Parameter type: float, int, bool, Vector, LinearColor, MaterialInterface, Texture, UObject")));
		Props->SetObjectField(TEXT("value"),
			MakeStringProperty(TEXT("Default value as string. For float/int: number, for bool: true/false, for Vector: '[X,Y,Z]', for LinearColor: '[R,G,B,A]', for MaterialInterface/Texture/UObject: full asset path.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to modify the Niagara System asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_type"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraUserParameterDefault);
		RegisterTool(Def);
	}

	// ---- Tool: remove_niagara_user_parameter ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("remove_niagara_user_parameter");
		Def.Description = TEXT(
			"Remove an exposed user parameter from a Niagara System asset by name.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("User parameter name. 'User.' prefix is optional.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to modify the Niagara System asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RemoveNiagaraUserParameter);
		RegisterTool(Def);
	}

	// ---- Tool: duplicate_niagara_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("duplicate_niagara_system");
		Def.Description = TEXT("Duplicate a Niagara System to a new asset path.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("source_system_path"),
			MakeStringProperty(TEXT("Full source Niagara System asset path")));
		Props->SetObjectField(TEXT("new_system_name"),
			MakeStringProperty(TEXT("Name for duplicated Niagara System")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Destination folder path. Default /Game/")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to duplicate asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("source_system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("new_system_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DuplicateNiagaraSystem);
		RegisterTool(Def);
	}

	// ---- Tool: add_niagara_emitter_to_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_niagara_emitter_to_system");
		Def.Description = TEXT(
			"Add a single emitter from a source Niagara System into a target Niagara System. "
			"This is useful for building a clean new system around only the emitter you want, without duplicating the full source system.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("target_system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System that will receive the emitter.")));
		Props->SetObjectField(TEXT("source_system_path"),
			MakeStringProperty(TEXT("Full asset path of the source Niagara System that already contains the desired emitter.")));
		Props->SetObjectField(TEXT("source_emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the source Niagara System. Optional if source_emitter_index is provided.")));
		Props->SetObjectField(TEXT("source_emitter_index"),
			MakeStringProperty(TEXT("Optional source emitter index.")));
		Props->SetObjectField(TEXT("copy_emitter"),
			MakeStringProperty(TEXT("Optional bool string. Default true. If true, the emitter is copied into the target system instead of referenced directly.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("target_system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("source_system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddNiagaraEmitterToSystem);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_emitter_enabled ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_emitter_enabled");
		Def.Description = TEXT(
			"Enable/disable a Niagara emitter by name on a Niagara System asset or a spawned Niagara actor component.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name to toggle")));
		Props->SetObjectField(TEXT("enabled"),
			MakeStringProperty(TEXT("true/false")));
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Optional Niagara System asset path (asset edit mode)")));
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("Optional actor label (runtime/component mode)")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("emitter_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("enabled"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraEmitterEnabled);
		RegisterTool(Def);
	}

	// ---- Tool: remove_niagara_emitters_from_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("remove_niagara_emitters_from_system");
		Def.Description = TEXT(
			"Remove emitters from a Niagara System by emitter name list, or keep only a named subset and delete the rest. "
			"Useful for stripping duplicated template systems down to a clean minimal runtime system.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_names"),
			MakeStringProperty(TEXT("Optional array or comma-separated list of emitter names to remove.")));
		Props->SetObjectField(TEXT("keep_only_emitter_names"),
			MakeStringProperty(TEXT("Optional array or comma-separated list of emitter names to keep; all other emitters will be removed.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to modify the Niagara System asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RemoveNiagaraEmittersFromSystem);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_renderer_enabled ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_renderer_enabled");
		Def.Description = TEXT(
			"Enable or disable a specific renderer inside a Niagara emitter on a Niagara System asset.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Props->SetObjectField(TEXT("enabled"),
			MakeStringProperty(TEXT("true/false")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("enabled"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraRendererEnabled);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_renderer_material ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_renderer_material");
		Def.Description = TEXT(
			"Set a renderer material on a Niagara renderer. Supports direct material assignment for sprite/ribbon renderers "
			"and explicit override materials for mesh renderers.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material or Material Instance to assign.")));
		Props->SetObjectField(TEXT("override_material_index"),
			MakeStringProperty(TEXT("Mesh renderer only. Optional override material slot index. Defaults to 0.")));
		Props->SetObjectField(TEXT("enable_override_materials"),
			MakeStringProperty(TEXT("Mesh renderer only. Optional bool string. Default true when assigning an override material.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraRendererMaterial);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_ribbon_deterministic_streams ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_ribbon_deterministic_streams");
		Def.Description = TEXT(
			"Create or update a particle-spawn Set Variables module that assigns deterministic ribbon stream ids and link order, "
			"then bind a ribbon renderer to those attributes.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Ribbon renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. When omitted, the tool now prefers an enabled ribbon renderer automatically.")));
		Props->SetObjectField(TEXT("stream_count"),
			MakeStringProperty(TEXT("Required integer stream count. Ribbon id will be Particles.UniqueID % stream_count.")));
		Props->SetObjectField(TEXT("ribbon_id_attribute"),
			MakeStringProperty(TEXT("Optional ribbon id attribute name. Default: Particles.RibbonStreamID")));
		Props->SetObjectField(TEXT("ribbon_link_order_attribute"),
			MakeStringProperty(TEXT("Optional ribbon link order attribute name. Default: Particles.RibbonStreamLinkOrder")));
		Props->SetObjectField(TEXT("target_index"),
			MakeStringProperty(TEXT("Optional Particle Spawn stack index for the Set Variables module. Defaults to 0.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("stream_count"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraRibbonDeterministicStreams);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_sprite_renderer_subuv ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_sprite_renderer_subuv");
		Def.Description = TEXT(
			"Set SubUV frame layout on a Niagara sprite renderer. Useful for flipbooks such as 8x8 fire sheets.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Props->SetObjectField(TEXT("columns"),
			MakeStringProperty(TEXT("SubUV column count (X).")));
		Props->SetObjectField(TEXT("rows"),
			MakeStringProperty(TEXT("SubUV row count (Y).")));
		Props->SetObjectField(TEXT("enable_blend"),
			MakeStringProperty(TEXT("Optional bool string. Default true. Enables SubUV blending between adjacent frames.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("columns"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("rows"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraSpriteRendererSubUV);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_renderer_material_user_param ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_renderer_material_user_param");
		Def.Description = TEXT(
			"Set a material user-parameter binding on a Niagara renderer. Supports sprite/ribbon material bindings "
			"and mesh renderer override material user-param bindings.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Props->SetObjectField(TEXT("user_param_name"),
			MakeStringProperty(TEXT("Niagara user parameter name. 'User.' prefix is optional.")));
		Props->SetObjectField(TEXT("override_material_index"),
			MakeStringProperty(TEXT("Mesh renderer only. Optional override material slot index. Defaults to 0.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("user_param_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraRendererMaterialUserParam);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_system_fixed_bounds ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_system_fixed_bounds");
		Def.Description = TEXT(
			"Enable or disable fixed bounds on a Niagara System asset and optionally set the fixed bounds box.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System to update.")));
		Props->SetObjectField(TEXT("enabled"),
			MakeStringProperty(TEXT("true/false. If true, fixed bounds are enabled on the system.")));
		Props->SetObjectField(TEXT("min"),
			MakeStringProperty(TEXT("Optional [X,Y,Z] minimum bounds. Required when enabling and setting a new box.")));
		Props->SetObjectField(TEXT("max"),
			MakeStringProperty(TEXT("Optional [X,Y,Z] maximum bounds. Required when enabling and setting a new box.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("enabled"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraSystemFixedBounds);
		RegisterTool(Def);
	}

	// ---- Tool: set_niagara_emitter_fixed_bounds ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_niagara_emitter_fixed_bounds");
		Def.Description = TEXT(
			"Set bounds mode and optional fixed bounds for a Niagara emitter inside a Niagara System asset.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("bounds_mode"),
			MakeStringProperty(TEXT("fixed, dynamic, or programmable. Default fixed.")));
		Props->SetObjectField(TEXT("min"),
			MakeStringProperty(TEXT("Optional [X,Y,Z] minimum bounds. Required for fixed mode when setting a new box.")));
		Props->SetObjectField(TEXT("max"),
			MakeStringProperty(TEXT("Optional [X,Y,Z] maximum bounds. Required for fixed mode when setting a new box.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to apply.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetNiagaraEmitterFixedBounds);
		RegisterTool(Def);
	}

	// ---- Tool: restore_niagara_mutation_snapshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("restore_niagara_mutation_snapshot");
		Def.Description = TEXT("Restore a Niagara System package from a saved ForgePilot snapshot, then reload the package in editor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("snapshot_path"),
			MakeStringProperty(TEXT("Optional absolute snapshot file path. If omitted, the latest snapshot is used.")));
		Props->SetObjectField(TEXT("use_latest"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Use the newest snapshot when snapshot_path is omitted.")));
		Props->SetObjectField(TEXT("reopen_editor"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Re-open the Niagara asset editor after restore.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually restore the snapshot. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RestoreNiagaraMutationSnapshot);
		RegisterTool(Def);
	}

	// ---- Tool: cleanup_niagara_audit_assets ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("cleanup_niagara_audit_assets");
		Def.Description = TEXT("Delete temporary Niagara Systems under /Game/ForgePilot_Audit. Useful after capability probes or disposable mutation tests.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("folder_path"),
			MakeStringProperty(TEXT("Optional content folder to scan. Default /Game/ForgePilot_Audit.")));
		Props->SetObjectField(TEXT("name_prefix"),
			MakeStringProperty(TEXT("Optional asset-name prefix filter. Example: NS_FireHand_RuntimeUserParamProbe_")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Set true to actually delete matching assets. Default false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CleanupNiagaraAuditAssets);
		RegisterTool(Def);
	}

	// ---- Tool: cleanup_niagara_authoring_residue ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("cleanup_niagara_authoring_residue");
		Def.Description = TEXT("Inspect one Niagara emitter or all emitters in a system for production-hostile residue and optionally remove disabled renderers or empty/disabled parked emitters.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Optional emitter name. Limits cleanup to one emitter when provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index. Overrides emitter_name when set.")));
		Props->SetObjectField(TEXT("remove_disabled_renderers"),
			MakeStringProperty(TEXT("Optional bool string. Default true. Remove disabled renderers from the scoped emitter(s).")));
		Props->SetObjectField(TEXT("remove_disabled_sprite_renderers_when_ribbon_present"),
			MakeStringProperty(TEXT("Optional bool string. Default true. Removes disabled sprite renderers when the same emitter has an enabled ribbon renderer.")));
		Props->SetObjectField(TEXT("remove_disabled_emitters"),
			MakeStringProperty(TEXT("Optional bool string. Default false. Removes disabled emitters from the system.")));
		Props->SetObjectField(TEXT("remove_empty_emitters"),
			MakeStringProperty(TEXT("Optional bool string. Default false. Removes emitters that have no renderer objects.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false. Set true to mutate and save the Niagara asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CleanupNiagaraAuthoringResidue);
		RegisterTool(Def);
	}

	// ---- Tool: get_niagara_emitter_stack ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_niagara_emitter_stack");
		Def.Description = TEXT(
			"List Niagara stack/module structure for one emitter, grouped by script stage so the agent can describe how the emitter is built.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("include_nodes"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include non-function-call traversal nodes.")));
		Props->SetObjectField(TEXT("include_inputs"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include stack-aware module input summaries and standard-module presence map.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetNiagaraEmitterStack);
		RegisterTool(Def);
	}

	// ---- Tool: review_niagara_system ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_niagara_system");
		Def.Description = TEXT(
			"Analyze a Niagara System and return structured findings for setup quality and optimization risks.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System to review")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewNiagaraSystem);
		RegisterTool(Def);
	}

	// ---- Tool: review_niagara_emitter ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_niagara_emitter");
		Def.Description = TEXT(
			"Analyze a single Niagara emitter and return focused findings for renderer setup, bounds risks, "
			"material bindings, and likely runtime issues.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index. Use this when names are duplicated or omitted.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewNiagaraEmitter);
		RegisterTool(Def);
	}

	// ---- Tool: review_niagara_renderer ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_niagara_renderer");
		Def.Description = TEXT(
			"Analyze a single Niagara renderer and return focused findings for material bindings, renderer state, "
			"sim-target compatibility, and renderer-specific setup risks.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Emitter name inside the Niagara System. Optional if emitter_index is provided.")));
		Props->SetObjectField(TEXT("emitter_index"),
			MakeStringProperty(TEXT("Optional emitter index.")));
		Props->SetObjectField(TEXT("renderer_name"),
			MakeStringProperty(TEXT("Renderer name inside the emitter. Optional if renderer_index is provided.")));
		Props->SetObjectField(TEXT("renderer_index"),
			MakeStringProperty(TEXT("Optional renderer index. Defaults to 0 when omitted.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewNiagaraRenderer);
		RegisterTool(Def);
	}

	// ---- Tool: add_empty_niagara_emitter ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_empty_niagara_emitter");
		Def.Description = TEXT(
			"Add a clean empty emitter to an existing Niagara System. "
			"Optionally add a bare sprite renderer, but do not inherit template modules by default.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("system_path"),
			MakeStringProperty(TEXT("Full asset path of the Niagara System that will receive the new empty emitter.")));
		Props->SetObjectField(TEXT("emitter_name"),
			MakeStringProperty(TEXT("Optional emitter name. Default: Emitter")));
		Props->SetObjectField(TEXT("simulation_target"),
			MakeStringProperty(TEXT("Optional simulation target: cpu or gpu. Default: cpu.")));
		Props->SetObjectField(TEXT("add_sprite_renderer"),
			MakeStringProperty(TEXT("Optional bool string. Default false. Adds a bare sprite renderer to the new emitter.")));
		Props->SetObjectField(TEXT("add_ribbon_renderer"),
			MakeStringProperty(TEXT("Optional bool string. Default false. Adds a bare ribbon renderer to the new emitter. Use this for clean ribbon emitters instead of copying template emitters.")));
		Props->SetObjectField(TEXT("add_default_modules_and_renderer"),
			MakeStringProperty(TEXT("Optional bool string. Default false. If true, initialize the empty emitter with Niagara's default starter modules and renderer.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to modify the Niagara System asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("system_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddEmptyNiagaraEmitter);
		RegisterTool(Def);
	}

	// ---- Tool: run_suction_core_niagara_op ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_suction_core_niagara_op");
		Def.Description = TEXT(
			"Run a guarded V2 suction-core semantic Niagara op. These ops wrap native emitter/module authoring and compile/log verification into one production-oriented batch.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("op_name"), MakeStringProperty(TEXT("Semantic op name: build_main_attraction_layer, build_secondary_filament_layer, build_core_turbulence_layer, build_sparks_layer, build_shock_glow_layer, retune_motion_family, rebuild_main_attraction_layer.")));
		Props->SetObjectField(TEXT("system_path"), MakeStringProperty(TEXT("Niagara System asset path.")));
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Optional material to bind after authoring the target layer.")));
		Props->SetObjectField(TEXT("hard_rebuild_gate_active"), MakeStringProperty(TEXT("Optional bool string. When true, scalar-only Layer A polish ops are blocked.")));
		Props->SetObjectField(TEXT("consecutive_layer_a_motion_failures"), MakeStringProperty(TEXT("Optional consecutive motion-family failures count. Used with the rebuild gate.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default false.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("op_name"))),
			MakeShareable(new FJsonValueString(TEXT("system_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunSuctionCoreNiagaraOp);
		RegisterTool(Def);
	}

	// ---- Tool: run_v2_niagara_op ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_v2_niagara_op");
		Def.Description = TEXT(
			"Run a guarded V2 semantic Niagara op for supported orb-centric archetypes."
			" Wraps native emitter/module authoring, compile/log verification, and semantic review contracts.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("archetype"), MakeStringProperty(TEXT("Resolved V2 archetype: suction_core_energy, electric_arc_orb, hero_fire_orb_sweep, arcane_pull_orb, impact_pulse_core, or explosion_flash_core.")));
		Props->SetObjectField(TEXT("op_name"), MakeStringProperty(TEXT("Semantic op name.")));
		Props->SetObjectField(TEXT("system_path"), MakeStringProperty(TEXT("Niagara System asset path.")));
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Optional material to bind after authoring the target layer.")));
		Props->SetObjectField(TEXT("hard_rebuild_gate_active"), MakeStringProperty(TEXT("Optional bool string.")));
		Props->SetObjectField(TEXT("consecutive_layer_a_motion_failures"), MakeStringProperty(TEXT("Optional consecutive hero-layer motion-family failures count.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default false.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("archetype"))),
			MakeShareable(new FJsonValueString(TEXT("op_name"))),
			MakeShareable(new FJsonValueString(TEXT("system_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunV2NiagaraOp);
		RegisterTool(Def);
	}

	RegisterNiagaraStackEditingTools(*this);
}

// ============================================================
// Implementations
// ============================================================

bool FVFXToolRegistry::Tool_CreateEmptyNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemName;
	if (!Input->TryGetStringField(TEXT("system_name"), SystemName))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_NAME"), TEXT("Missing required field: system_name"));
	}

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	FString PackagePath = SavePath;
	if (!PackagePath.EndsWith(TEXT("/")))
	{
		PackagePath += TEXT("/");
	}
	PackagePath += SystemName;

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ASSET_EXISTS"),
			FString::Printf(TEXT("Asset already exists at path '%s'."), *PackagePath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_empty_niagara_system"));
	Result->SetStringField(TEXT("asset_path"), PackagePath);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create an empty Niagara System '%s' at '%s'."), *SystemName, *PackagePath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	UPackage* Package = CreatePackage(*PackagePath);
	UNiagaraSystem* NewSystem = NewObject<UNiagaraSystem>(
		Package,
		FName(*SystemName),
		RF_Public | RF_Standalone | RF_Transactional);

	if (!NewSystem)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_CREATE_FAILED"), TEXT("Failed to create empty Niagara System asset."));
	}

	UNiagaraSystemFactoryNew::InitializeSystem(NewSystem, true);

	FAssetRegistryModule::AssetCreated(NewSystem);
	NewSystem->MarkPackageDirty();

	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NewSystem, CompileSaveError, CompileDiagnostics);
	if (!bSaveSucceeded)
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
		ErrorObj->SetStringField(
			TEXT("error_message"),
			CompileSaveError.IsEmpty()
				? FString::Printf(TEXT("Created empty Niagara System '%s' but compile/log/save guard failed for '%s'."), *SystemName, *PackagePath)
				: CompileSaveError);
		ErrorObj->SetStringField(TEXT("asset_path"), PackagePath);
		ErrorObj->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
		if (CompileDiagnostics.IsValid())
		{
			ErrorObj->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
		}
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	const FString Message = FString::Printf(
		TEXT("Created empty Niagara System '%s' at '%s'. This is the preferred clean starting point for authored Niagara work."),
		*SystemName,
		*PackagePath);
	Result->SetStringField(TEXT("message"), Message);
	Result->SetBoolField(TEXT("saved"), true);
	Result->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
	if (CompileDiagnostics.IsValid())
	{
		Result->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
	}
	AppendNiagaraMutationAudit(Result, TEXT("create_empty_niagara_system"), NewSystem, Message);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemName;
	if (!Input->TryGetStringField(TEXT("system_name"), SystemName))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_NAME"), TEXT("Missing required field: system_name"));
	}

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	// Build package path
	FString PackagePath = SavePath;
	if (!PackagePath.EndsWith(TEXT("/")))
	{
		PackagePath += TEXT("/");
	}
	PackagePath += SystemName;

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ASSET_EXISTS"),
			FString::Printf(TEXT("Asset already exists at path '%s'."), *PackagePath));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("create_niagara_system"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("planned_asset_path"), PackagePath);
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create Niagara System '%s' at '%s'"), *SystemName, *PackagePath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	// Create the package and Niagara System
	UPackage* Package = CreatePackage(*PackagePath);
	UNiagaraSystem* NewSystem = NewObject<UNiagaraSystem>(
		Package,
		FName(*SystemName),
		RF_Public | RF_Standalone | RF_Transactional);

	if (!NewSystem)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_CREATE_FAILED"), TEXT("Failed to create Niagara System asset."));
	}

	// Niagara systems need their script source and default graph initialized before save/registry work.
	UNiagaraSystemFactoryNew::InitializeSystem(NewSystem, true);

	// Register with asset registry and save
	FAssetRegistryModule::AssetCreated(NewSystem);
	NewSystem->MarkPackageDirty();

	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NewSystem, CompileSaveError, CompileDiagnostics);
	if (!bSaveSucceeded)
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
		ErrorObj->SetStringField(
			TEXT("error_message"),
			CompileSaveError.IsEmpty()
				? FString::Printf(TEXT("Created Niagara System '%s' but compile/log/save guard failed for '%s'."), *SystemName, *PackagePath)
				: CompileSaveError);
		ErrorObj->SetStringField(TEXT("asset_path"), PackagePath);
		ErrorObj->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
		if (CompileDiagnostics.IsValid())
		{
			ErrorObj->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
		}
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("create_niagara_system"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("asset_path"), PackagePath);
	Result->SetBoolField(TEXT("saved"), true);
	Result->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
	if (CompileDiagnostics.IsValid())
	{
		Result->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
	}
	const FString CreateNiagaraMessage = FString::Printf(TEXT("Created Niagara System '%s' at '%s'. Open it in the Niagara Editor to add emitters and configure modules."),
		*SystemName, *PackagePath);
	Result->SetStringField(TEXT("message"), CreateNiagaraMessage);
	AppendNiagaraMutationAudit(Result, TEXT("create_niagara_system"), NewSystem, CreateNiagaraMessage);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_FindNiagaraTemplateEmitters(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString Description;
	if (!Input->TryGetStringField(TEXT("description"), Description) || Description.TrimStartAndEnd().IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_DESCRIPTION"), TEXT("Missing required field: description"));
	}

	FString SearchRoot = TEXT("/Game/");
	Input->TryGetStringField(TEXT("search_root"), SearchRoot);
	if (SearchRoot.IsEmpty())
	{
		SearchRoot = TEXT("/Game/");
	}

	int32 MaxCandidatesPerLayer = 5;
	if (Input->HasField(TEXT("max_candidates_per_layer")))
	{
		MaxCandidatesPerLayer = FMath::Clamp(static_cast<int32>(Input->GetNumberField(TEXT("max_candidates_per_layer"))), 1, 20);
	}

	TSharedPtr<FJsonObject> Plan;
	FString ResolveError;
	if (!FVFXKnowledgeLoader::ResolveEffectPlan(Description, Plan, ResolveError, nullptr) || !Plan.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_RESOLVE_EFFECT_PLAN_FAILED"), ResolveError);
	}

	FString EffectType;
	Plan->TryGetStringField(TEXT("resolved_effect_type"), EffectType);
	FString VariantName;
	Plan->TryGetStringField(TEXT("resolved_variant"), VariantName);

	const TSharedPtr<FJsonObject>* StrategyPtr = nullptr;
	if (!Plan->TryGetObjectField(TEXT("authoring_strategy"), StrategyPtr) || !StrategyPtr || !(*StrategyPtr).IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_AUTHORING_STRATEGY_MISSING"), TEXT("resolve_effect_plan did not return a valid authoring_strategy."));
	}

	const TSharedPtr<FJsonObject>* SelectedArchetypesPtr = nullptr;
	if (!(*StrategyPtr)->TryGetObjectField(TEXT("selected_template_archetypes"), SelectedArchetypesPtr) || !SelectedArchetypesPtr || !(*SelectedArchetypesPtr).IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_TEMPLATE_ARCHETYPES_MISSING"), TEXT("authoring_strategy did not include selected_template_archetypes."));
	}

	const TArray<TSharedPtr<FJsonValue>>* SelectedLayersPtr = nullptr;
	if (!Plan->TryGetArrayField(TEXT("selected_layers"), SelectedLayersPtr) || !SelectedLayersPtr)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SELECTED_LAYERS_MISSING"), TEXT("resolve_effect_plan did not return selected_layers."));
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FARFilter Filter;
	Filter.ClassPaths.Add(UNiagaraSystem::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(FName(*SearchRoot));

	TArray<FAssetData> NiagaraAssets;
	AssetRegistryModule.Get().GetAssets(Filter, NiagaraAssets);

	TArray<TSharedPtr<FJsonValue>> LayerMatchesArray;
	TSharedPtr<FJsonObject> LayerBestMatches = MakeShareable(new FJsonObject());
	int32 TotalEmitterCandidatesScanned = 0;

	for (const TSharedPtr<FJsonValue>& LayerValue : *SelectedLayersPtr)
	{
		FString LayerName;
		if (!LayerValue.IsValid() || !LayerValue->TryGetString(LayerName) || LayerName.IsEmpty())
		{
			continue;
		}

		const TSharedPtr<FJsonObject>* ArchetypePtr = nullptr;
		if (!(*SelectedArchetypesPtr)->TryGetObjectField(LayerName, ArchetypePtr) || !ArchetypePtr || !(*ArchetypePtr).IsValid())
		{
			continue;
		}

		FString PreferredArchetypeDescription = LayerName;
		TArray<FString> ArchetypeSearchTerms;
		(*ArchetypePtr)->TryGetStringField(TEXT("description"), PreferredArchetypeDescription);
		const TArray<TSharedPtr<FJsonValue>>* SearchTermsPtr = nullptr;
		if ((*ArchetypePtr)->TryGetArrayField(TEXT("template_search_terms"), SearchTermsPtr) && SearchTermsPtr)
		{
			ArchetypeSearchTerms = ParseSearchTerms(*SearchTermsPtr);
		}

		TArray<FString> SystemPathPrefilterTerms;
		SystemPathPrefilterTerms.Add(EffectType);
		if (!VariantName.IsEmpty())
		{
			SystemPathPrefilterTerms.Add(VariantName);
		}
		SystemPathPrefilterTerms.Add(LayerName);
		SystemPathPrefilterTerms.Append(ArchetypeSearchTerms);

		struct FTemplateCandidate
		{
			int32 Score = 0;
			FString SystemPath;
			FString SystemName;
			FString EmitterName;
			int32 EmitterIndex = INDEX_NONE;
			TArray<FString> MatchedTerms;
		};

		TArray<FTemplateCandidate> Candidates;

		for (const FAssetData& AssetData : NiagaraAssets)
		{
			const FString SystemPath = AssetData.GetObjectPathString();
			const FString SystemPathNorm = NormalizeNiagaraTemplateToken(SystemPath);
			bool bSystemPathLooksRelevant = false;
			for (const FString& PrefilterTerm : SystemPathPrefilterTerms)
			{
				const FString TermNorm = NormalizeNiagaraTemplateToken(PrefilterTerm);
				if (!TermNorm.IsEmpty() && SystemPathNorm.Contains(TermNorm))
				{
					bSystemPathLooksRelevant = true;
					break;
				}
			}

			if (!bSystemPathLooksRelevant)
			{
				continue;
			}

			UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(AssetData.GetAsset());
			if (!NiagaraSystem)
			{
				continue;
			}

			const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
			for (int32 EmitterIndex = 0; EmitterIndex < EmitterHandles.Num(); ++EmitterIndex)
			{
				const FNiagaraEmitterHandle& EmitterHandle = EmitterHandles[EmitterIndex];
				const FString EmitterName = EmitterHandle.GetName().ToString();
				TArray<FString> MatchedTerms;
				const int32 Score = ScoreTemplateEmitterCandidate(
					EffectType,
					VariantName,
					LayerName,
					PreferredArchetypeDescription,
					ArchetypeSearchTerms,
					SystemPath,
					EmitterName,
					MatchedTerms);

				++TotalEmitterCandidatesScanned;
				if (Score <= 0)
				{
					continue;
				}

				FTemplateCandidate Candidate;
				Candidate.Score = Score;
				Candidate.SystemPath = SystemPath;
				Candidate.SystemName = NiagaraSystem->GetName();
				Candidate.EmitterName = EmitterName;
				Candidate.EmitterIndex = EmitterIndex;
				Candidate.MatchedTerms = MatchedTerms;
				Candidates.Add(Candidate);
			}
		}

		Candidates.Sort([](const FTemplateCandidate& A, const FTemplateCandidate& B)
		{
			if (A.Score != B.Score)
			{
				return A.Score > B.Score;
			}
			if (A.SystemPath != B.SystemPath)
			{
				return A.SystemPath < B.SystemPath;
			}
			return A.EmitterName < B.EmitterName;
		});

		TArray<TSharedPtr<FJsonValue>> CandidateArray;
		const int32 CandidateCount = FMath::Min(MaxCandidatesPerLayer, Candidates.Num());
		for (int32 Index = 0; Index < CandidateCount; ++Index)
		{
			const FTemplateCandidate& Candidate = Candidates[Index];
			TSharedPtr<FJsonObject> CandidateObj = MakeShareable(new FJsonObject());
			CandidateObj->SetStringField(TEXT("source_system_path"), Candidate.SystemPath);
			CandidateObj->SetStringField(TEXT("source_system_name"), Candidate.SystemName);
			CandidateObj->SetStringField(TEXT("source_emitter_name"), Candidate.EmitterName);
			CandidateObj->SetNumberField(TEXT("source_emitter_index"), Candidate.EmitterIndex);
			CandidateObj->SetNumberField(TEXT("score"), Candidate.Score);

			TArray<TSharedPtr<FJsonValue>> MatchedTermValues;
			for (const FString& MatchedTerm : Candidate.MatchedTerms)
			{
				MatchedTermValues.Add(MakeShareable(new FJsonValueString(MatchedTerm)));
			}
			CandidateObj->SetArrayField(TEXT("matched_terms"), MatchedTermValues);
			CandidateArray.Add(MakeShareable(new FJsonValueObject(CandidateObj)));
		}

		TSharedPtr<FJsonObject> LayerMatchObj = MakeShareable(new FJsonObject());
		LayerMatchObj->SetStringField(TEXT("layer_name"), LayerName);
		LayerMatchObj->SetStringField(TEXT("preferred_template_archetype_description"), PreferredArchetypeDescription);
		LayerMatchObj->SetNumberField(TEXT("candidate_count"), Candidates.Num());
		LayerMatchObj->SetArrayField(TEXT("candidates"), CandidateArray);

		TArray<TSharedPtr<FJsonValue>> SearchTermValues;
		for (const FString& SearchTerm : ArchetypeSearchTerms)
		{
			SearchTermValues.Add(MakeShareable(new FJsonValueString(SearchTerm)));
		}
		LayerMatchObj->SetArrayField(TEXT("template_search_terms"), SearchTermValues);

		if (Candidates.Num() > 0)
		{
			TSharedPtr<FJsonObject> BestMatchObj = MakeShareable(new FJsonObject());
			BestMatchObj->SetStringField(TEXT("source_system_path"), Candidates[0].SystemPath);
			BestMatchObj->SetStringField(TEXT("source_system_name"), Candidates[0].SystemName);
			BestMatchObj->SetStringField(TEXT("source_emitter_name"), Candidates[0].EmitterName);
			BestMatchObj->SetNumberField(TEXT("source_emitter_index"), Candidates[0].EmitterIndex);
			BestMatchObj->SetNumberField(TEXT("score"), Candidates[0].Score);
			LayerMatchObj->SetObjectField(TEXT("best_candidate"), BestMatchObj);
			LayerBestMatches->SetObjectField(LayerName, BestMatchObj);
		}

		LayerMatchesArray.Add(MakeShareable(new FJsonValueObject(LayerMatchObj)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("find_niagara_template_emitters"));
	Result->SetStringField(TEXT("description"), Description);
	Result->SetStringField(TEXT("resolved_effect_type"), EffectType);
	if (!VariantName.IsEmpty())
	{
		Result->SetStringField(TEXT("resolved_variant"), VariantName);
	}
	Result->SetStringField(TEXT("search_root"), SearchRoot);
	Result->SetNumberField(TEXT("max_candidates_per_layer"), MaxCandidatesPerLayer);
	Result->SetNumberField(TEXT("niagara_system_count"), NiagaraAssets.Num());
	Result->SetNumberField(TEXT("emitter_candidates_scanned"), TotalEmitterCandidatesScanned);
	Result->SetObjectField(TEXT("authoring_strategy"), *StrategyPtr);
	Result->SetObjectField(TEXT("layer_best_matches"), LayerBestMatches);
	Result->SetArrayField(TEXT("layer_matches"), LayerMatchesArray);
	Result->SetStringField(TEXT("message"), TEXT("Template emitter candidates resolved for the selected recipe layers."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_AddEmptyNiagaraEmitter(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	FString RequestedEmitterName = TEXT("Emitter");
	Input->TryGetStringField(TEXT("emitter_name"), RequestedEmitterName);
	const FString UniqueEmitterName = MakeUniqueEmitterNameForSystem(NiagaraSystem, RequestedEmitterName);

	FString SimulationTarget = TEXT("cpu");
	Input->TryGetStringField(TEXT("simulation_target"), SimulationTarget);
	const bool bUseGpuSim = SimulationTarget.Equals(TEXT("gpu"), ESearchCase::IgnoreCase);

	const bool bAddSpriteRenderer = ParseOptionalBoolField(Input, TEXT("add_sprite_renderer"), false);
	const bool bAddRibbonRenderer = ParseOptionalBoolField(Input, TEXT("add_ribbon_renderer"), false);
	const bool bAddDefaultModulesAndRenderer = ParseOptionalBoolField(Input, TEXT("add_default_modules_and_renderer"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	if (bAddSpriteRenderer && bAddRibbonRenderer)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_EMITTER_RENDERER_CONFLICT"),
			TEXT("add_empty_niagara_emitter supports one explicit renderer type at a time. Set either add_sprite_renderer or add_ribbon_renderer, not both."));
	}

	if (bAddRibbonRenderer && bAddDefaultModulesAndRenderer)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RIBBON_DEFAULT_RENDERER_BLOCKED"),
			TEXT("add_empty_niagara_emitter no longer allows add_ribbon_renderer together with add_default_modules_and_renderer because that leaves disabled sprite-renderer residue. Create a clean ribbon emitter with add_ribbon_renderer=true and add_default_modules_and_renderer=false."));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("add_empty_niagara_emitter"));
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("emitter_name"), UniqueEmitterName);
	Result->SetStringField(TEXT("simulation_target"), bUseGpuSim ? TEXT("gpu") : TEXT("cpu"));
	Result->SetBoolField(TEXT("add_sprite_renderer"), bAddSpriteRenderer);
	Result->SetBoolField(TEXT("add_ribbon_renderer"), bAddRibbonRenderer);
	Result->SetBoolField(TEXT("add_default_modules_and_renderer"), bAddDefaultModulesAndRenderer);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would add empty emitter '%s' to '%s'%s."),
				*UniqueEmitterName,
				*SystemPath,
				bAddSpriteRenderer ? TEXT(" with a bare sprite renderer") : (bAddRibbonRenderer ? TEXT(" with a bare ribbon renderer") : TEXT(""))));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("add_empty_niagara_emitter"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	UNiagaraEmitter* EmptyEmitter = NewObject<UNiagaraEmitter>(GetTransientPackage());
	UNiagaraEmitterFactoryNew::InitializeEmitter(EmptyEmitter, bAddDefaultModulesAndRenderer);
	EmptyEmitter->SetUniqueEmitterName(UniqueEmitterName);
	EmptyEmitter->SetFlags(RF_Transactional);

	const FGuid EmitterVersion;
	const FGuid AddedEmitterId = FNiagaraEditorUtilities::AddEmitterToSystem(*NiagaraSystem, *EmptyEmitter, EmitterVersion, true);

	FNiagaraEmitterHandle* AddedHandle = nullptr;
	for (FNiagaraEmitterHandle& Handle : NiagaraSystem->GetEmitterHandles())
	{
		if (Handle.GetId() == AddedEmitterId)
		{
			AddedHandle = &Handle;
			break;
		}
	}

	if (!AddedHandle)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ADDED_EMITTER_NOT_FOUND"),
			FString::Printf(TEXT("Emitter '%s' was added but could not be resolved afterwards."), *UniqueEmitterName));
	}

	FVersionedNiagaraEmitter AddedEmitterInstance = AddedHandle->GetInstance();
	if (FVersionedNiagaraEmitterData* AddedEmitterData = AddedHandle->GetEmitterData())
	{
		AddedEmitterData->SimTarget = bUseGpuSim ? ENiagaraSimTarget::GPUComputeSim : ENiagaraSimTarget::CPUSim;
		if (bAddSpriteRenderer && !bAddDefaultModulesAndRenderer && AddedEmitterInstance.Emitter)
		{
			UNiagaraSpriteRendererProperties* SpriteRenderer = NewObject<UNiagaraSpriteRendererProperties>(AddedEmitterInstance.Emitter, TEXT("SpriteRenderer"));
			AddedEmitterInstance.Emitter->AddRenderer(SpriteRenderer, AddedEmitterInstance.Version);
		}
		if (bAddRibbonRenderer && AddedEmitterInstance.Emitter)
		{
			UNiagaraRibbonRendererProperties* RibbonRenderer = NewObject<UNiagaraRibbonRendererProperties>(AddedEmitterInstance.Emitter, TEXT("RibbonRenderer"));
			AddedEmitterInstance.Emitter->AddRenderer(RibbonRenderer, AddedEmitterInstance.Version);
		}
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	Result->SetStringField(TEXT("added_emitter_id"), AddedEmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetNumberField(TEXT("emitter_count"), NiagaraSystem->GetEmitterHandles().Num());
	Result->SetBoolField(TEXT("sprite_renderer_added"), bAddSpriteRenderer || bAddDefaultModulesAndRenderer);
	Result->SetBoolField(TEXT("ribbon_renderer_added"), bAddRibbonRenderer);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), UniqueEmitterName);
	AuditDetails->SetStringField(TEXT("simulation_target"), bUseGpuSim ? TEXT("gpu") : TEXT("cpu"));
	AuditDetails->SetBoolField(TEXT("add_sprite_renderer"), bAddSpriteRenderer);
	AuditDetails->SetBoolField(TEXT("add_ribbon_renderer"), bAddRibbonRenderer);
	AuditDetails->SetBoolField(TEXT("add_default_modules_and_renderer"), bAddDefaultModulesAndRenderer);
	AuditDetails->SetStringField(TEXT("added_emitter_id"), AddedEmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));

	const FString Message = FString::Printf(
		TEXT("Added empty emitter '%s' to '%s'%s."),
		*UniqueEmitterName,
		*SystemPath,
		bAddDefaultModulesAndRenderer ? TEXT(" with default rendering support") : (bAddSpriteRenderer ? TEXT(" with sprite rendering support") : (bAddRibbonRenderer ? TEXT(" with ribbon rendering support") : TEXT(""))));

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("add_empty_niagara_emitter"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		Message,
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_GetNiagaraSystemInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(
		UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
	Result->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
	Result->SetStringField(TEXT("asset_class"), NiagaraSystem->GetClass()->GetName());

	// Emitter handles
	TArray<TSharedPtr<FJsonValue>> EmitterArray;
	const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	for (int32 i = 0; i < EmitterHandles.Num(); i++)
	{
		TSharedPtr<FJsonObject> EmitterObj = MakeShareable(new FJsonObject());
		EmitterObj->SetNumberField(TEXT("index"), i);
		EmitterObj->SetStringField(TEXT("name"), EmitterHandles[i].GetName().ToString());
		EmitterObj->SetBoolField(TEXT("enabled"), EmitterHandles[i].GetIsEnabled());
		EmitterObj->SetStringField(TEXT("id"), EmitterHandles[i].GetId().ToString(EGuidFormats::DigitsWithHyphensLower));

		if (FVersionedNiagaraEmitterData* EmitterData = EmitterHandles[i].GetEmitterData())
		{
			const FVersionedNiagaraEmitter VersionedEmitter = EmitterHandles[i].GetInstance();
			if (UNiagaraEmitter* EmitterAsset = VersionedEmitter.Emitter)
			{
				EmitterObj->SetStringField(TEXT("emitter_asset_path"), EmitterAsset->GetPathName());
				EmitterObj->SetStringField(TEXT("emitter_asset_name"), EmitterAsset->GetName());
			}
			const TArray<UNiagaraRendererProperties*>& Renderers = EmitterData->GetRenderers();
			EmitterObj->SetNumberField(TEXT("renderer_count"), Renderers.Num());
			TArray<TSharedPtr<FJsonValue>> RendererArray;
			for (int32 RendererIndex = 0; RendererIndex < Renderers.Num(); ++RendererIndex)
			{
				const UNiagaraRendererProperties* Renderer = Renderers[RendererIndex];
				if (!Renderer) continue;
				TSharedPtr<FJsonObject> RendererObj = MakeShareable(new FJsonObject());
				RendererObj->SetNumberField(TEXT("index"), RendererIndex);
				RendererObj->SetStringField(TEXT("renderer_class"), Renderer->GetClass()->GetName());
				RendererObj->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
				RendererArray.Add(MakeShareable(new FJsonValueObject(RendererObj)));
			}
			EmitterObj->SetArrayField(TEXT("renderers"), RendererArray);
		}
		EmitterArray.Add(MakeShareable(new FJsonValueObject(EmitterObj)));
	}
	Result->SetArrayField(TEXT("emitters"), EmitterArray);
	Result->SetNumberField(TEXT("emitter_count"), EmitterHandles.Num());

	// User parameters
	TArray<TSharedPtr<FJsonValue>> ParamArray;
	const auto& UserParams = NiagaraSystem->GetExposedParameters().ReadParameterVariables();
	for (const auto& Param : UserParams)
	{
		TSharedPtr<FJsonObject> ParamObj = MakeShareable(new FJsonObject());
		ParamObj->SetStringField(TEXT("name"), Param.GetName().ToString());
		ParamObj->SetStringField(TEXT("type"), Param.GetType().GetName());
		ParamObj->SetStringField(TEXT("type_namespace"), Param.GetType().GetNameText().ToString());
		ParamArray.Add(MakeShareable(new FJsonValueObject(ParamObj)));
	}
	Result->SetArrayField(TEXT("user_parameters"), ParamArray);
	Result->SetNumberField(TEXT("user_parameter_count"), ParamArray.Num());

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const bool bIncludeSystemInfo = ParseOptionalBoolField(Input, TEXT("include_system_info"), true);

	TArray<FAssetData> SelectedAssets;
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	TSet<FString> SelectedPaths;
	TArray<TSharedPtr<FJsonValue>> SelectedSystemsArray;
	for (const FAssetData& AssetData : SelectedAssets)
	{
		UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(AssetData.GetAsset());
		if (!NiagaraSystem)
		{
			continue;
		}

		const FString SystemPath = NiagaraSystem->GetPathName();
		SelectedPaths.Add(SystemPath);
		TSharedPtr<FJsonObject> SystemObj = bIncludeSystemInfo
			? BuildNiagaraSystemContextObject(NiagaraSystem)
			: MakeShareable(new FJsonObject());
		if (!bIncludeSystemInfo)
		{
			SystemObj->SetStringField(TEXT("system_path"), SystemPath);
			SystemObj->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
		}
		SelectedSystemsArray.Add(MakeShareable(new FJsonValueObject(SystemObj)));
	}

	TSet<FString> OpenedPaths;
	TArray<TSharedPtr<FJsonValue>> OpenedSystemsArray;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			const TArray<UObject*> OpenedAssets = AssetEditorSubsystem->GetAllEditedAssets();
			for (UObject* AssetObj : OpenedAssets)
			{
				UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(AssetObj);
				if (!NiagaraSystem)
				{
					continue;
				}

				const FString SystemPath = NiagaraSystem->GetPathName();
				OpenedPaths.Add(SystemPath);
				TSharedPtr<FJsonObject> SystemObj = bIncludeSystemInfo
					? BuildNiagaraSystemContextObject(NiagaraSystem)
					: MakeShareable(new FJsonObject());
				if (!bIncludeSystemInfo)
				{
					SystemObj->SetStringField(TEXT("system_path"), SystemPath);
					SystemObj->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
				}
				SystemObj->SetBoolField(TEXT("is_selected"), SelectedPaths.Contains(SystemPath));
				OpenedSystemsArray.Add(MakeShareable(new FJsonValueObject(SystemObj)));
			}
		}
	}

	FString PreferredSystemPath;
	FString PreferredSource = TEXT("none");
	if (SelectedSystemsArray.Num() > 0)
	{
		const TSharedPtr<FJsonObject>* PreferredObj = nullptr;
		if (SelectedSystemsArray[0]->TryGetObject(PreferredObj) && PreferredObj && PreferredObj->IsValid())
		{
			(*PreferredObj)->TryGetStringField(TEXT("system_path"), PreferredSystemPath);
			PreferredSource = TEXT("selected");
		}
	}
	else if (OpenedSystemsArray.Num() > 0)
	{
		const TSharedPtr<FJsonObject>* PreferredObj = nullptr;
		if (OpenedSystemsArray[0]->TryGetObject(PreferredObj) && PreferredObj && PreferredObj->IsValid())
		{
			(*PreferredObj)->TryGetStringField(TEXT("system_path"), PreferredSystemPath);
			PreferredSource = TEXT("opened");
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("include_system_info"), bIncludeSystemInfo);
	Result->SetStringField(TEXT("preferred_source"), PreferredSource);
	Result->SetStringField(TEXT("preferred_system_path"), PreferredSystemPath);
	Result->SetNumberField(TEXT("selected_niagara_count"), SelectedSystemsArray.Num());
	Result->SetNumberField(TEXT("opened_niagara_count"), OpenedSystemsArray.Num());
	Result->SetArrayField(TEXT("selected_systems"), SelectedSystemsArray);
	Result->SetArrayField(TEXT("opened_systems"), OpenedSystemsArray);
	Result->SetBoolField(TEXT("preferred_system_is_open"), !PreferredSystemPath.IsEmpty() && OpenedPaths.Contains(PreferredSystemPath));
	Result->SetBoolField(TEXT("preferred_system_is_selected"), !PreferredSystemPath.IsEmpty() && SelectedPaths.Contains(PreferredSystemPath));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraEmitterDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	FString EmitterName;
	Input->TryGetStringField(TEXT("emitter_name"), EmitterName);

	int32 EmitterIndex = INDEX_NONE;
	if (Input->HasTypedField<EJson::Number>(TEXT("emitter_index")))
	{
		EmitterIndex = static_cast<int32>(Input->GetNumberField(TEXT("emitter_index")));
	}
	else
	{
		FString EmitterIndexStr;
		if (Input->TryGetStringField(TEXT("emitter_index"), EmitterIndexStr) && !EmitterIndexStr.IsEmpty())
		{
			EmitterIndex = FCString::Atoi(*EmitterIndexStr);
		}
	}

	const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	if (EmitterHandles.Num() == 0)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_NO_EMITTERS"),
			FString::Printf(TEXT("Niagara System '%s' has no emitters."), *NiagaraSystem->GetName()));
	}

	int32 MatchedIndex = INDEX_NONE;
	if (EmitterIndex != INDEX_NONE)
	{
		if (!EmitterHandles.IsValidIndex(EmitterIndex))
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_EMITTER_INDEX_OUT_OF_RANGE"),
				FString::Printf(TEXT("Emitter index %d is out of range for system '%s'."), EmitterIndex, *NiagaraSystem->GetName()));
		}
		MatchedIndex = EmitterIndex;
	}
	else if (!EmitterName.IsEmpty())
	{
		for (int32 Index = 0; Index < EmitterHandles.Num(); ++Index)
		{
			if (EmitterHandles[Index].GetName().ToString().Equals(EmitterName, ESearchCase::IgnoreCase))
			{
				MatchedIndex = Index;
				break;
			}
		}
		if (MatchedIndex == INDEX_NONE)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_EMITTER_NOT_FOUND"),
				FString::Printf(TEXT("Emitter '%s' not found in Niagara System '%s'."), *EmitterName, *NiagaraSystem->GetName()));
		}
	}
	else
	{
		MatchedIndex = 0;
	}

	const FNiagaraEmitterHandle& EmitterHandle = EmitterHandles[MatchedIndex];
	FVersionedNiagaraEmitterData* EmitterData = EmitterHandle.GetEmitterData();
	if (!EmitterData)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_EMITTER_DATA_MISSING"),
			FString::Printf(TEXT("Emitter '%s' has no emitter data."), *EmitterHandle.GetName().ToString()));
	}

	const FVersionedNiagaraEmitter VersionedEmitter = EmitterHandle.GetInstance();
	UNiagaraEmitter* EmitterAsset = VersionedEmitter.Emitter;
	const UEnum* SimTargetEnum = StaticEnum<ENiagaraSimTarget>();
	const UEnum* BoundsModeEnum = StaticEnum<ENiagaraEmitterCalculateBoundMode>();

	TArray<TSharedPtr<FJsonValue>> RendererArray;
	TArray<FString> SummaryLines;
	int32 DisabledRendererCount = 0;
	int32 IncompatibleRendererCount = 0;
	const TArray<UNiagaraRendererProperties*>& Renderers = EmitterData->GetRenderers();
	for (int32 RendererIndex = 0; RendererIndex < Renderers.Num(); ++RendererIndex)
	{
		const UNiagaraRendererProperties* Renderer = Renderers[RendererIndex];
		if (!Renderer)
		{
			continue;
		}

		TArray<UMaterialInterface*> UsedMaterials;
		Renderer->GetUsedMaterials(nullptr, UsedMaterials);
		TArray<TSharedPtr<FJsonValue>> UsedMaterialArray;
		for (UMaterialInterface* UsedMaterial : UsedMaterials)
		{
			if (!UsedMaterial)
			{
				continue;
			}
			UsedMaterialArray.Add(MakeShareable(new FJsonValueString(UsedMaterial->GetPathName())));
		}

		const bool bRendererEnabled = Renderer->GetIsEnabled();
		const bool bRendererSupportsSimTarget = Renderer->IsSimTargetSupported(EmitterData->SimTarget);
		if (!bRendererEnabled)
		{
			++DisabledRendererCount;
		}
		if (bRendererEnabled && !bRendererSupportsSimTarget)
		{
			++IncompatibleRendererCount;
		}

		TSharedPtr<FJsonObject> RendererObj = MakeShareable(new FJsonObject());
		RendererObj->SetNumberField(TEXT("index"), RendererIndex);
		RendererObj->SetStringField(TEXT("renderer_class"), Renderer->GetClass()->GetName());
		RendererObj->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
		RendererObj->SetBoolField(TEXT("enabled"), bRendererEnabled);
		RendererObj->SetBoolField(TEXT("supports_emitter_sim_target"), bRendererSupportsSimTarget);
		RendererObj->SetArrayField(TEXT("used_materials"), UsedMaterialArray);
		RendererArray.Add(MakeShareable(new FJsonValueObject(RendererObj)));
	}

	TArray<TSharedPtr<FJsonValue>> UserParamsArray;
	const TArrayView<const FNiagaraVariableWithOffset> UserParams = NiagaraSystem->GetExposedParameters().ReadParameterVariables();
	for (const FNiagaraVariableWithOffset& Param : UserParams)
	{
		TSharedPtr<FJsonObject> ParamObj = MakeShareable(new FJsonObject());
		ParamObj->SetStringField(TEXT("name"), Param.GetName().ToString());
		ParamObj->SetStringField(TEXT("type"), Param.GetType().GetName());
		UserParamsArray.Add(MakeShareable(new FJsonValueObject(ParamObj)));
	}

	const FString SimTargetString = NiagaraEnumValueToString(SimTargetEnum, static_cast<int64>(EmitterData->SimTarget));
	const FString BoundsModeString = NiagaraEnumValueToString(BoundsModeEnum, static_cast<int64>(EmitterData->CalculateBoundsMode));

	SummaryLines.Add(FString::Printf(
		TEXT("Emitter '%s' is %s, sim target '%s', local space %s, bounds mode '%s'."),
		*EmitterHandle.GetName().ToString(),
		EmitterHandle.GetIsEnabled() ? TEXT("enabled") : TEXT("disabled"),
		*SimTargetString,
		EmitterData->bLocalSpace ? TEXT("true") : TEXT("false"),
		*BoundsModeString));
	SummaryLines.Add(FString::Printf(
		TEXT("Emitter has %d renderer(s), %d event handler(s), %d simulation stage(s)."),
		Renderers.Num(),
		EmitterData->GetEventHandlers().Num(),
		EmitterData->GetSimulationStages().Num()));
	if (EmitterData->CalculateBoundsMode == ENiagaraEmitterCalculateBoundMode::Fixed)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("Emitter uses fixed bounds min %s max %s."),
			*EmitterData->FixedBounds.Min.ToString(),
			*EmitterData->FixedBounds.Max.ToString()));
	}
	if (IncompatibleRendererCount > 0)
	{
		SummaryLines.Add(FString::Printf(
			TEXT("%d renderer(s) are enabled but incompatible with sim target '%s'."),
			IncompatibleRendererCount,
			*SimTargetString));
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

	if (EmitterData->SimTarget == ENiagaraSimTarget::GPUComputeSim &&
		EmitterData->CalculateBoundsMode == ENiagaraEmitterCalculateBoundMode::Dynamic &&
		!NiagaraSystem->bFixedBounds)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("GPU emitter using dynamic bounds"),
			TEXT("GPU emitters with dynamic bounds can cull incorrectly when neither emitter nor system has fixed bounds."),
			TEXT("Set emitter fixed bounds or enable fixed bounds on the Niagara System."));
	}

	if (Renderers.Num() == 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Emitter has no renderers"),
			TEXT("Emitter exists but no renderer properties were found."),
			TEXT("Add a sprite/mesh/ribbon renderer or verify this emitter is intended to be logic-only."));
	}

	if (IncompatibleRendererCount > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Renderer/sim target mismatch"),
			FString::Printf(TEXT("%d enabled renderer(s) do not support sim target '%s'."), IncompatibleRendererCount, *SimTargetString),
			TEXT("Switch emitter sim target or update renderer types to supported ones."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious emitter issues"),
			TEXT("No high-signal emitter-level issues detected by automated checks."),
			TEXT("Validate runtime spawn density, timing, and material bindings in target scenes."));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
	Result->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
	Result->SetBoolField(TEXT("system_fixed_bounds_enabled"), NiagaraSystem->bFixedBounds);
	Result->SetObjectField(TEXT("system_fixed_bounds"), BoxToJsonObject(NiagaraSystem->GetFixedBounds()));
	Result->SetNumberField(TEXT("matched_emitter_index"), MatchedIndex);
	Result->SetStringField(TEXT("emitter_name"), EmitterHandle.GetName().ToString());
	Result->SetBoolField(TEXT("enabled"), EmitterHandle.GetIsEnabled());
	Result->SetStringField(TEXT("emitter_id"), EmitterHandle.GetId().ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetStringField(TEXT("emitter_asset_path"), EmitterAsset ? EmitterAsset->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("emitter_asset_name"), EmitterAsset ? EmitterAsset->GetName() : TEXT(""));
	Result->SetStringField(TEXT("sim_target"), SimTargetString);
	Result->SetBoolField(TEXT("local_space"), EmitterData->bLocalSpace);
	Result->SetStringField(TEXT("calculate_bounds_mode"), BoundsModeString);
	Result->SetObjectField(TEXT("fixed_bounds"), BoxToJsonObject(EmitterData->FixedBounds));
	Result->SetNumberField(TEXT("renderer_count"), Renderers.Num());
	Result->SetNumberField(TEXT("disabled_renderer_count"), DisabledRendererCount);
	Result->SetNumberField(TEXT("incompatible_renderer_count"), IncompatibleRendererCount);
	Result->SetNumberField(TEXT("event_handler_count"), EmitterData->GetEventHandlers().Num());
	Result->SetNumberField(TEXT("simulation_stage_count"), EmitterData->GetSimulationStages().Num());
	Result->SetArrayField(TEXT("renderers"), RendererArray);
	Result->SetArrayField(TEXT("system_user_parameters"), UserParamsArray);
	Result->SetArrayField(TEXT("findings"), Findings);
	TArray<TSharedPtr<FJsonValue>> SummaryLineValues;
	for (const FString& SummaryLine : SummaryLines)
	{
		SummaryLineValues.Add(MakeShareable(new FJsonValueString(SummaryLine)));
	}
	Result->SetArrayField(TEXT("summary_lines"), SummaryLineValues);
	Result->SetStringField(TEXT("human_summary"), FString::Join(SummaryLines, TEXT("\n")));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraRendererDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraUserParameterDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString ParamName;
	Input->TryGetStringField(TEXT("param_name"), ParamName);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	const TArray<TSharedPtr<FJsonValue>> ParamArray = BuildNiagaraParameterValueArray(
		NiagaraSystem->GetExposedParameters(),
		ParamName);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
	Result->SetStringField(TEXT("system_name"), NiagaraSystem->GetName());
	Result->SetStringField(TEXT("asset_class"), NiagaraSystem->GetClass()->GetName());
	Result->SetStringField(TEXT("param_name_filter"), ParamName.IsEmpty() ? TEXT("") : NormalizeNiagaraUserParameterName(ParamName));
	Result->SetArrayField(TEXT("parameters"), ParamArray);
	Result->SetNumberField(TEXT("parameter_count"), ParamArray.Num());
	Result->SetStringField(TEXT("message"),
		ParamName.IsEmpty()
			? FString::Printf(TEXT("Read %d Niagara user parameter default value(s) from '%s'."), ParamArray.Num(), *NiagaraSystem->GetName())
			: FString::Printf(TEXT("Read Niagara user parameter default(s) matching '%s' from '%s'."), *NormalizeNiagaraUserParameterName(ParamName), *NiagaraSystem->GetName()));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraActorUserParameterValues(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString ActorLabel;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel) || ActorLabel.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ACTOR_LABEL"), TEXT("Missing required field: actor_label"));
	}

	FString ParamName;
	Input->TryGetStringField(TEXT("param_name"), ParamName);

	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_NO_EDITOR_WORLD"), TEXT("No editor world available."));
	}

	AActor* FoundActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel() == ActorLabel)
		{
			FoundActor = *It;
			break;
		}
	}

	if (!FoundActor)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ACTOR_NOT_FOUND"),
			FString::Printf(TEXT("Actor with label '%s' not found."), *ActorLabel));
	}

	UNiagaraComponent* NiagaraComp = FoundActor->FindComponentByClass<UNiagaraComponent>();
	if (!NiagaraComp)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Actor '%s' has no Niagara Component."), *ActorLabel));
	}

	const TArray<TSharedPtr<FJsonValue>> ParamArray = BuildNiagaraParameterValueArray(
		NiagaraComp->GetOverrideParameters(),
		ParamName);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetStringField(TEXT("actor_name"), FoundActor->GetName());
	Result->SetStringField(TEXT("component_name"), NiagaraComp->GetName());
	Result->SetStringField(TEXT("system_path"), NiagaraComp->GetAsset() ? NiagaraComp->GetAsset()->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("param_name_filter"), ParamName.IsEmpty() ? TEXT("") : NormalizeNiagaraUserParameterName(ParamName));
	Result->SetArrayField(TEXT("parameters"), ParamArray);
	Result->SetNumberField(TEXT("parameter_count"), ParamArray.Num());
	Result->SetStringField(TEXT("message"),
		ParamName.IsEmpty()
			? FString::Printf(TEXT("Read %d Niagara actor user parameter value(s) from '%s'."), ParamArray.Num(), *ActorLabel)
			: FString::Printf(TEXT("Read Niagara actor user parameter value(s) matching '%s' from '%s'."), *NormalizeNiagaraUserParameterName(ParamName), *ActorLabel));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	int32 MaxResults = 20;
	TryReadOptionalIntField(Input, TEXT("max_results"), MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 200);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	TArray<FString> SnapshotFiles;
	GetNiagaraSnapshotFiles(NiagaraSystem, SnapshotFiles);

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

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_niagara_mutation_snapshots"));
	Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
	Result->SetStringField(TEXT("snapshot_root"), GetNiagaraMutationSnapshotRoot());
	Result->SetNumberField(TEXT("count"), SnapshotArray.Num());
	Result->SetArrayField(TEXT("snapshots"), SnapshotArray);
	Result->SetStringField(TEXT("message"), SnapshotArray.Num() > 0
		? TEXT("Niagara mutation snapshots found.")
		: TEXT("No Niagara mutation snapshots found."));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetNiagaraEmitterStack(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	const bool bIncludeNodes = ParseOptionalBoolField(Input, TEXT("include_nodes"), false);
	const bool bIncludeInputs = ParseOptionalBoolField(Input, TEXT("include_inputs"), false);
	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	TArray<TSharedPtr<FJsonValue>> StageArray;
	TArray<TSharedPtr<FJsonValue>> Findings;
	auto AppendStage = [&](UNiagaraScript* Script)
	{
		if (!Script)
		{
			return;
		}
		const FString StageLabel = NiagaraStackStageLabel(Script->GetUsage());
		StageArray.Add(MakeShareable(new FJsonValueObject(
			BuildNiagaraStackStageObject(Script, StageLabel, bIncludeNodes, Findings))));
	};

	AppendStage(EmitterData->EmitterSpawnScriptProps.Script);
	AppendStage(EmitterData->EmitterUpdateScriptProps.Script);
	AppendStage(EmitterData->SpawnScriptProps.Script);
	AppendStage(EmitterData->UpdateScriptProps.Script);
	for (UNiagaraSimulationStageBase* SimulationStage : EmitterData->GetSimulationStages())
	{
		if (SimulationStage && SimulationStage->Script)
		{
			StageArray.Add(MakeShareable(new FJsonValueObject(
				BuildNiagaraStackStageObject(SimulationStage->Script, TEXT("simulation_stage"), bIncludeNodes, Findings))));
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_niagara_emitter_stack"));
	Result->SetStringField(TEXT("system_path"), NiagaraSystem->GetPathName());
	Result->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	Result->SetStringField(TEXT("emitter_name"), EmitterHandle ? EmitterHandle->GetName().ToString() : TEXT(""));
	Result->SetArrayField(TEXT("stages"), StageArray);
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetNumberField(TEXT("stage_count"), StageArray.Num());
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetBoolField(TEXT("include_inputs"), bIncludeInputs);

	if (bIncludeInputs)
	{
		VFXNiagaraStackTools::FEmitterContext StackContext;
		FString StackErrorCode;
		FString StackErrorMessage;
		if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, Input, StackContext, StackErrorCode, StackErrorMessage))
		{
			Result->SetStringField(TEXT("stack_inputs_error_code"), StackErrorCode);
			Result->SetStringField(TEXT("stack_inputs_error_message"), StackErrorMessage);
		}
		else
		{
			TArray<VFXNiagaraStackTools::FStackModuleSummary> Modules;
			VFXNiagaraStackTools::CollectModuleSummaries(StackContext, Modules);

			TArray<TSharedPtr<FJsonValue>> ModuleArray;
			for (const VFXNiagaraStackTools::FStackModuleSummary& ModuleSummary : Modules)
			{
				ModuleArray.Add(MakeShared<FJsonValueObject>(
					VFXNiagaraStackTools::BuildModuleJsonObject(ModuleSummary, true)));
			}

			Result->SetArrayField(TEXT("module_inputs"), ModuleArray);
			Result->SetObjectField(TEXT("standard_module_presence"),
				VFXNiagaraStackTools::BuildStandardModulePresenceObject(Modules));
		}
	}
	Result->SetStringField(TEXT("message"), TEXT("Niagara emitter stack extracted."));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SpawnNiagaraAtLocation(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(
		UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	// Parse location
	FVector Location = FVector::ZeroVector;
	const TArray<TSharedPtr<FJsonValue>>* LocArray;
	if (Input->TryGetArrayField(TEXT("location"), LocArray) && LocArray->Num() >= 3)
	{
		Location.X = (*LocArray)[0]->AsNumber();
		Location.Y = (*LocArray)[1]->AsNumber();
		Location.Z = (*LocArray)[2]->AsNumber();
	}

	// Parse rotation
	FRotator Rotation = FRotator::ZeroRotator;
	const TArray<TSharedPtr<FJsonValue>>* RotArray;
	if (Input->TryGetArrayField(TEXT("rotation"), RotArray) && RotArray->Num() >= 3)
	{
		Rotation.Pitch = (*RotArray)[0]->AsNumber();
		Rotation.Yaw = (*RotArray)[1]->AsNumber();
		Rotation.Roll = (*RotArray)[2]->AsNumber();
	}

	// Auto activate
	bool bAutoActivate = true;
	FString AutoActivateStr;
	if (Input->TryGetStringField(TEXT("auto_activate"), AutoActivateStr))
	{
		bAutoActivate = AutoActivateStr.Equals(TEXT("true"), ESearchCase::IgnoreCase);
	}

	FString ActorLabel;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel) || ActorLabel.IsEmpty())
	{
		ActorLabel = FString::Printf(TEXT("NS_%s"), *NiagaraSystem->GetName());
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("spawn_niagara_at_location"));
		Result->SetBoolField(TEXT("will_mutate_level"), false);
		Result->SetStringField(TEXT("system_path"), SystemPath);
		Result->SetStringField(TEXT("planned_actor_label"), ActorLabel);
		Result->SetStringField(TEXT("planned_location"),
			FString::Printf(TEXT("(X=%.2f,Y=%.2f,Z=%.2f)"), Location.X, Location.Y, Location.Z));
		Result->SetStringField(TEXT("planned_rotation"),
			FString::Printf(TEXT("(Pitch=%.2f,Yaw=%.2f,Roll=%.2f)"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
		Result->SetBoolField(TEXT("planned_auto_activate"), bAutoActivate);
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would spawn Niagara '%s' as actor '%s'"), *NiagaraSystem->GetName(), *ActorLabel));
		OutResult = JsonObjToString(Result);
		return true;
	}

	// Get the editor world
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_NO_EDITOR_WORLD"), TEXT("No editor world available."));
	}

	// Spawn an empty actor
	AActor* NewActor = World->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation);
	if (!NewActor)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_ACTOR_SPAWN_FAILED"), TEXT("Failed to spawn actor for Niagara System."));
	}

	// Add root scene component
	USceneComponent* RootComp = NewObject<USceneComponent>(NewActor, TEXT("RootComponent"));
	RootComp->RegisterComponent();
	NewActor->SetRootComponent(RootComp);
	NewActor->SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);

	// Create and attach Niagara Component
	UNiagaraComponent* NiagaraComp = NewObject<UNiagaraComponent>(NewActor, TEXT("NiagaraComponent"));
	NiagaraComp->SetAsset(NiagaraSystem);
	NiagaraComp->SetAutoActivate(bAutoActivate);
	NiagaraComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
	NiagaraComp->RegisterComponent();
	NewActor->AddInstanceComponent(NiagaraComp);

	if (bAutoActivate)
	{
		NiagaraComp->Activate(true);
	}

	// Set label
	NewActor->SetActorLabel(ActorLabel);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("spawn_niagara_at_location"));
	Result->SetBoolField(TEXT("will_mutate_level"), true);
	Result->SetStringField(TEXT("actor_label"), NewActor->GetActorLabel());
	Result->SetStringField(TEXT("message"),
		FString::Printf(TEXT("Spawned Niagara System '%s' at (%0.1f, %0.1f, %0.1f) as actor '%s'"),
			*NiagaraSystem->GetName(), Location.X, Location.Y, Location.Z, *NewActor->GetActorLabel()));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString ActorLabel;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ACTOR_LABEL"), TEXT("Missing required field: actor_label"));
	}

	FString ParamName;
	if (!Input->TryGetStringField(TEXT("param_name"), ParamName))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_NAME"), TEXT("Missing required field: param_name"));
	}

	FString ParamType;
	if (!Input->TryGetStringField(TEXT("param_type"), ParamType))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_TYPE"), TEXT("Missing required field: param_type"));
	}

	FString ValueStr;
	if (!Input->TryGetStringField(TEXT("value"), ValueStr))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_VALUE"), TEXT("Missing required field: value"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	// Find actor by label.
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_NO_EDITOR_WORLD"), TEXT("No editor world available."));
	}

	AActor* FoundActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel() == ActorLabel)
		{
			FoundActor = *It;
			break;
		}
	}
	if (!FoundActor)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ACTOR_NOT_FOUND"),
			FString::Printf(TEXT("Actor with label '%s' not found."), *ActorLabel));
	}

	// Find Niagara Component.
	UNiagaraComponent* NiagaraComp = FoundActor->FindComponentByClass<UNiagaraComponent>();
	if (!NiagaraComp)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Actor '%s' has no Niagara Component."), *ActorLabel));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_user_parameter"));
	Result->SetBoolField(TEXT("will_mutate_component"), bApplyChanges);
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetStringField(TEXT("param_name"), ParamName);
	Result->SetStringField(TEXT("param_type"), ParamType);
	Result->SetStringField(TEXT("value"), ValueStr);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would set Niagara parameter '%s' (type: %s) = %s on actor '%s'"),
				*ParamName,
				*ParamType,
				*ValueStr,
				*ActorLabel));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FName ParamFName(*ParamName);
	bool bSuccess = false;

	if (ParamType.Equals(TEXT("float"), ESearchCase::IgnoreCase))
	{
		float Val = FCString::Atof(*ValueStr);
		NiagaraComp->SetVariableFloat(ParamFName, Val);
		bSuccess = true;
	}
	else if (ParamType.Equals(TEXT("int"), ESearchCase::IgnoreCase))
	{
		int32 Val = FCString::Atoi(*ValueStr);
		NiagaraComp->SetVariableInt(ParamFName, Val);
		bSuccess = true;
	}
	else if (ParamType.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
	{
		bool Val = ValueStr.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		NiagaraComp->SetVariableBool(ParamFName, Val);
		bSuccess = true;
	}
	else if (ParamType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase))
	{
		TArray<float> Values;
		ParseNiagaraFloatList(ValueStr, Values);
		if (Values.Num() >= 3)
		{
			FVector Vec(Values[0], Values[1], Values[2]);
			NiagaraComp->SetVariableVec3(ParamFName, Vec);
			bSuccess = true;
		}
		else
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_INVALID_VECTOR"),
				TEXT("Vector value must have at least 3 components: [X,Y,Z]"));
		}
	}
	else if (ParamType.Equals(TEXT("LinearColor"), ESearchCase::IgnoreCase))
	{
		TArray<float> Values;
		ParseNiagaraFloatList(ValueStr, Values);
		if (Values.Num() >= 3)
		{
			FLinearColor Color(Values[0], Values[1], Values[2],
				Values.Num() >= 4 ? Values[3] : 1.0f);
			NiagaraComp->SetVariableLinearColor(ParamFName, Color);
			bSuccess = true;
		}
		else
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_INVALID_COLOR"),
				TEXT("LinearColor value must have at least 3 components: [R,G,B] or [R,G,B,A]"));
		}
	}
	else if (ParamType.Equals(TEXT("MaterialInterface"), ESearchCase::IgnoreCase) ||
		ParamType.Equals(TEXT("Material"), ESearchCase::IgnoreCase))
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(ValueStr));
		if (!Material)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_INVALID_MATERIAL"),
				FString::Printf(TEXT("MaterialInterface asset not found at '%s'."), *ValueStr));
		}
		NiagaraComp->SetVariableMaterial(ParamFName, Material);
		Result->SetStringField(TEXT("resolved_asset_path"), Material->GetPathName());
		Result->SetStringField(TEXT("resolved_asset_class"), Material->GetClass()->GetName());
		bSuccess = true;
	}
	else if (ParamType.Equals(TEXT("Texture"), ESearchCase::IgnoreCase))
	{
		UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(ValueStr));
		if (!Texture)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_INVALID_TEXTURE"),
				FString::Printf(TEXT("Texture asset not found at '%s'."), *ValueStr));
		}
		NiagaraComp->SetVariableTexture(ParamFName, Texture);
		Result->SetStringField(TEXT("resolved_asset_path"), Texture->GetPathName());
		Result->SetStringField(TEXT("resolved_asset_class"), Texture->GetClass()->GetName());
		bSuccess = true;
	}
	else if (ParamType.Equals(TEXT("UObject"), ESearchCase::IgnoreCase) ||
		ParamType.Equals(TEXT("Object"), ESearchCase::IgnoreCase))
	{
		UObject* ObjectValue = UEditorAssetLibrary::LoadAsset(ValueStr);
		if (!ObjectValue)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_INVALID_OBJECT"),
				FString::Printf(TEXT("UObject asset not found at '%s'."), *ValueStr));
		}
		NiagaraComp->SetVariableObject(ParamFName, ObjectValue);
		Result->SetStringField(TEXT("resolved_asset_path"), ObjectValue->GetPathName());
		Result->SetStringField(TEXT("resolved_asset_class"), ObjectValue->GetClass()->GetName());
		bSuccess = true;
	}
	else
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_UNSUPPORTED_PARAM_TYPE"),
			FString::Printf(TEXT("Unsupported param_type: '%s'. Use float, int, bool, Vector, LinearColor, MaterialInterface, Texture, or UObject."), *ParamType));
	}

	Result->SetBoolField(TEXT("success"), bSuccess);
	Result->SetStringField(TEXT("message"),
		FString::Printf(TEXT("Set Niagara parameter '%s' (type: %s) = %s on actor '%s'"),
			*ParamName, *ParamType, *ValueStr, *ActorLabel));

	OutResult = JsonObjToString(Result);
	return bSuccess;
}

bool FVFXToolRegistry::Tool_AddNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString ParamName;
	if (!Input->TryGetStringField(TEXT("param_name"), ParamName) || ParamName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_NAME"), TEXT("Missing required field: param_name"));
	}

	FString ParamType;
	if (!Input->TryGetStringField(TEXT("param_type"), ParamType) || ParamType.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_TYPE"), TEXT("Missing required field: param_type"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FString FinalParamName = ParamName.StartsWith(TEXT("User.")) ? ParamName : FString::Printf(TEXT("User.%s"), *ParamName);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	FNiagaraTypeDefinition ParamTypeDef;
	if (!ResolveNiagaraTypeDefinition(ParamType, ParamTypeDef))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_UNSUPPORTED_PARAM_TYPE"),
			FString::Printf(TEXT("Unsupported param_type '%s'. Use float, int, bool, Vector, LinearColor, MaterialInterface, Texture, or UObject."), *ParamType));
	}

	const TArrayView<const FNiagaraVariableWithOffset> ExistingParams = NiagaraSystem->GetExposedParameters().ReadParameterVariables();
	for (const FNiagaraVariableWithOffset& ExistingVar : ExistingParams)
	{
		if (ExistingVar.GetName().ToString().Equals(FinalParamName, ESearchCase::IgnoreCase))
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_PARAM_EXISTS"),
				FString::Printf(TEXT("User parameter '%s' already exists on '%s'."), *FinalParamName, *SystemPath));
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("add_niagara_user_parameter"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("param_name"), FinalParamName);
	Result->SetStringField(TEXT("param_type"), ParamType);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would add user parameter '%s' (%s) to '%s'."),
				*FinalParamName,
				*ParamType,
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FNiagaraVariable NewVar(ParamTypeDef, FName(*FinalParamName));
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("add_niagara_user_parameter"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	NiagaraSystem->GetExposedParameters().AddParameter(NewVar);
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("param_name"), FinalParamName);
	AuditDetails->SetStringField(TEXT("param_type"), ParamType);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("add_niagara_user_parameter"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Added user parameter '%s' (%s) to '%s'."),
			*FinalParamName,
			*ParamType,
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraUserParameterDefault(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString ParamName;
	if (!Input->TryGetStringField(TEXT("param_name"), ParamName) || ParamName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_NAME"), TEXT("Missing required field: param_name"));
	}

	FString ParamType;
	if (!Input->TryGetStringField(TEXT("param_type"), ParamType) || ParamType.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_TYPE"), TEXT("Missing required field: param_type"));
	}

	FString ValueStr;
	if (!Input->TryGetStringField(TEXT("value"), ValueStr))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_VALUE"), TEXT("Missing required field: value"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FString FinalParamName = NormalizeNiagaraUserParameterName(ParamName);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	FNiagaraTypeDefinition RequestedTypeDef;
	if (!ResolveNiagaraTypeDefinition(ParamType, RequestedTypeDef))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_UNSUPPORTED_PARAM_TYPE"),
			FString::Printf(TEXT("Unsupported param_type '%s'. Use float, int, bool, Vector, LinearColor, MaterialInterface, Texture, or UObject."), *ParamType));
	}

	const FNiagaraVariableWithOffset* FoundParam = nullptr;
	for (const FNiagaraVariableWithOffset& ExistingVar : NiagaraSystem->GetExposedParameters().ReadParameterVariables())
	{
		if (ExistingVar.GetName().ToString().Equals(FinalParamName, ESearchCase::IgnoreCase))
		{
			FoundParam = &ExistingVar;
			break;
		}
	}

	if (!FoundParam)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_PARAM_NOT_FOUND"),
			FString::Printf(TEXT("User parameter '%s' was not found on '%s'."), *FinalParamName, *SystemPath));
	}

	if (!FoundParam->GetType().IsSameBaseDefinition(RequestedTypeDef))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_PARAM_TYPE_MISMATCH"),
			FString::Printf(TEXT("User parameter '%s' exists, but its type does not match requested type '%s'."), *FinalParamName, *ParamType));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_user_parameter_default"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("param_name"), FinalParamName);
	Result->SetStringField(TEXT("param_type"), ParamType);
	Result->SetStringField(TEXT("value"), ValueStr);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would set default Niagara user parameter '%s' (type: %s) = %s on '%s'."),
				*FinalParamName,
				*ParamType,
				*ValueStr,
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FNiagaraVariable TargetParameter(FoundParam->GetType(), FoundParam->GetName());
	FString ErrorCode;
	FString ErrorMessage;
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_user_parameter_default"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	if (!SetNiagaraParameterStoreValue(NiagaraSystem->GetExposedParameters(), TargetParameter, ParamType, ValueStr, Result, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("param_name"), FinalParamName);
	AuditDetails->SetStringField(TEXT("param_type"), ParamType);
	AuditDetails->SetStringField(TEXT("value"), ValueStr);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_user_parameter_default"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Set default Niagara user parameter '%s' (type: %s) = %s on '%s'."),
			*FinalParamName, *ParamType, *ValueStr, *SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_RemoveNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString ParamName;
	if (!Input->TryGetStringField(TEXT("param_name"), ParamName) || ParamName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_PARAM_NAME"), TEXT("Missing required field: param_name"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FString FinalParamName = NormalizeNiagaraUserParameterName(ParamName);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	const FNiagaraVariableWithOffset* FoundParam = nullptr;
	for (const FNiagaraVariableWithOffset& ExistingVar : NiagaraSystem->GetExposedParameters().ReadParameterVariables())
	{
		if (ExistingVar.GetName().ToString().Equals(FinalParamName, ESearchCase::IgnoreCase))
		{
			FoundParam = &ExistingVar;
			break;
		}
	}

	if (!FoundParam)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_PARAM_NOT_FOUND"),
			FString::Printf(TEXT("User parameter '%s' was not found on '%s'."), *FinalParamName, *SystemPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("remove_niagara_user_parameter"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("param_name"), FinalParamName);
	Result->SetStringField(TEXT("param_type"), FoundParam->GetType().GetName());

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would remove Niagara user parameter '%s' from '%s'."),
				*FinalParamName,
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	const FNiagaraVariable TargetParameter(FoundParam->GetType(), FoundParam->GetName());
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("remove_niagara_user_parameter"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	if (!NiagaraSystem->GetExposedParameters().RemoveParameter(TargetParameter))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_REMOVE_PARAM_FAILED"),
			FString::Printf(TEXT("Failed to remove user parameter '%s' from '%s'."), *FinalParamName, *SystemPath));
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("param_name"), FinalParamName);
	AuditDetails->SetStringField(TEXT("param_type"), FoundParam->GetType().GetName());

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("remove_niagara_user_parameter"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Removed Niagara user parameter '%s' from '%s'."),
			*FinalParamName, *SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_DuplicateNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SourceSystemPath;
	if (!Input->TryGetStringField(TEXT("source_system_path"), SourceSystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SOURCE_SYSTEM_PATH"), TEXT("Missing required field: source_system_path"));
	}

	FString NewSystemName;
	if (!Input->TryGetStringField(TEXT("new_system_name"), NewSystemName) || NewSystemName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_NEW_SYSTEM_NAME"), TEXT("Missing required field: new_system_name"));
	}

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	if (!SavePath.EndsWith(TEXT("/")))
	{
		SavePath += TEXT("/");
	}

	const FString DestinationPath = SavePath + NewSystemName;
	if (UEditorAssetLibrary::DoesAssetExist(DestinationPath))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ASSET_EXISTS"),
			FString::Printf(TEXT("Destination asset already exists at '%s'."), *DestinationPath));
	}

	UNiagaraSystem* SourceSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SourceSystemPath));
	if (!SourceSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Source Niagara System not found at '%s'."), *SourceSystemPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("duplicate_niagara_system"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("source_system_path"), SourceSystemPath);
	Result->SetStringField(TEXT("planned_destination_path"), DestinationPath);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would duplicate '%s' to '%s'."), *SourceSystemPath, *DestinationPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	UObject* DuplicatedAsset = UEditorAssetLibrary::DuplicateAsset(SourceSystemPath, DestinationPath);
	if (DuplicatedAsset == nullptr)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_DUPLICATE_FAILED"),
			FString::Printf(TEXT("Failed to duplicate Niagara System from '%s' to '%s'."), *SourceSystemPath, *DestinationPath));
	}

	UNiagaraSystem* DuplicatedSystem = Cast<UNiagaraSystem>(DuplicatedAsset);
	if (!DuplicatedSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_DUPLICATE_TYPE_MISMATCH"),
			FString::Printf(TEXT("Duplicated asset '%s' is not a Niagara System."), *DestinationPath));
	}

	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(DuplicatedSystem, CompileSaveError, CompileDiagnostics);
	if (!bSaveSucceeded)
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), TEXT("NI_COMPILE_LOG_OR_SAVE_FAILED"));
		ErrorObj->SetStringField(
			TEXT("error_message"),
			CompileSaveError.IsEmpty()
				? FString::Printf(TEXT("Duplicated Niagara System '%s' but compile/log/save guard failed for '%s'."), *SourceSystemPath, *DestinationPath)
				: CompileSaveError);
		ErrorObj->SetStringField(TEXT("source_system_path"), SourceSystemPath);
		ErrorObj->SetStringField(TEXT("destination_path"), DestinationPath);
		ErrorObj->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
		if (CompileDiagnostics.IsValid())
		{
			ErrorObj->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
		}
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	Result->SetStringField(TEXT("destination_path"), DestinationPath);
	Result->SetBoolField(TEXT("saved"), true);
	Result->SetBoolField(TEXT("compile_log_checked"), CompileDiagnostics.IsValid());
	if (CompileDiagnostics.IsValid())
	{
		Result->SetObjectField(TEXT("compile_diagnostics"), CompileDiagnostics);
	}
	Result->SetStringField(
		TEXT("message"),
		FString::Printf(TEXT("Duplicated Niagara System '%s' to '%s'."), *SourceSystemPath, *DestinationPath));
	AppendNiagaraMutationAudit(Result, TEXT("duplicate_niagara_system"), DuplicatedAsset, Result->GetStringField(TEXT("message")));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_AddNiagaraEmitterToSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString TargetSystemPath;
	if (!Input->TryGetStringField(TEXT("target_system_path"), TargetSystemPath) || TargetSystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_TARGET_SYSTEM_PATH"), TEXT("Missing required field: target_system_path"));
	}

	FString SourceSystemPath;
	if (!Input->TryGetStringField(TEXT("source_system_path"), SourceSystemPath) || SourceSystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SOURCE_SYSTEM_PATH"), TEXT("Missing required field: source_system_path"));
	}

	UNiagaraSystem* TargetSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(TargetSystemPath));
	if (!TargetSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_TARGET_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Target Niagara System not found at '%s'."), *TargetSystemPath));
	}

	UNiagaraSystem* SourceSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SourceSystemPath));
	if (!SourceSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SOURCE_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Source Niagara System not found at '%s'."), *SourceSystemPath));
	}

	int32 SourceEmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* SourceEmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* SourceEmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	TSharedPtr<FJsonObject> SourceResolveInput = MakeShareable(new FJsonObject(*Input));
	if (!SourceResolveInput->HasField(TEXT("emitter_name")) && SourceResolveInput->HasField(TEXT("source_emitter_name")))
	{
		SourceResolveInput->SetStringField(TEXT("emitter_name"), SourceResolveInput->GetStringField(TEXT("source_emitter_name")));
	}
	if (!SourceResolveInput->HasField(TEXT("emitter_index")) && SourceResolveInput->HasField(TEXT("source_emitter_index")))
	{
		if (SourceResolveInput->HasTypedField<EJson::Number>(TEXT("source_emitter_index")))
		{
			SourceResolveInput->SetNumberField(TEXT("emitter_index"), SourceResolveInput->GetNumberField(TEXT("source_emitter_index")));
		}
		else
		{
			SourceResolveInput->SetStringField(TEXT("emitter_index"), SourceResolveInput->GetStringField(TEXT("source_emitter_index")));
		}
	}
	if (!ResolveNiagaraEmitterTarget(SourceSystem, SourceResolveInput, SourceEmitterIndex, SourceEmitterHandle, SourceEmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	const FVersionedNiagaraEmitter VersionedSourceEmitter = SourceEmitterHandle->GetInstance();
	UNiagaraEmitter* SourceEmitter = VersionedSourceEmitter.Emitter;
	if (!SourceEmitter)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SOURCE_EMITTER_NOT_FOUND"),
			FString::Printf(TEXT("Emitter '%s' on '%s' has no source emitter object."), *SourceEmitterHandle->GetName().ToString(), *SourceSystemPath));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bCreateCopy = ParseOptionalBoolField(Input, TEXT("copy_emitter"), true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("add_niagara_emitter_to_system"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("target_system_path"), TargetSystemPath);
	Result->SetStringField(TEXT("source_system_path"), SourceSystemPath);
	Result->SetStringField(TEXT("source_emitter_name"), SourceEmitterHandle->GetName().ToString());
	Result->SetBoolField(TEXT("copy_emitter"), bCreateCopy);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would add emitter '%s' from '%s' into '%s'."),
				*SourceEmitterHandle->GetName().ToString(),
				*SourceSystemPath,
				*TargetSystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	TargetSystem->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(TargetSystem, TEXT("add_niagara_emitter_to_system"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	const FGuid AddedEmitterId = FNiagaraEditorUtilities::AddEmitterToSystem(*TargetSystem, *SourceEmitter, VersionedSourceEmitter.Version, bCreateCopy);
	TargetSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(TargetSystem, CompileSaveError, CompileDiagnostics);

	Result->SetStringField(TEXT("added_emitter_id"), AddedEmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetNumberField(TEXT("emitter_count"), TargetSystem->GetEmitterHandles().Num());

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("target_system_path"), TargetSystemPath);
	AuditDetails->SetStringField(TEXT("source_system_path"), SourceSystemPath);
	AuditDetails->SetStringField(TEXT("source_emitter_name"), SourceEmitterHandle->GetName().ToString());
	AuditDetails->SetStringField(TEXT("added_emitter_id"), AddedEmitterId.ToString(EGuidFormats::DigitsWithHyphensLower));
	AuditDetails->SetBoolField(TEXT("copy_emitter"), bCreateCopy);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("add_niagara_emitter_to_system"),
		TargetSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Added emitter '%s' from '%s' into '%s'."),
			*SourceEmitterHandle->GetName().ToString(),
			*SourceSystemPath,
			*TargetSystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_RemoveNiagaraEmittersFromSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), false);
	Result->SetStringField(TEXT("action"), TEXT("remove_niagara_emitters_from_system"));

	FString SystemPath;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	TArray<FString> EmitterNamesToRemove;
	TArray<FString> KeepOnlyEmitterNames;
	ParseOptionalStringArrayField(Input, TEXT("emitter_names"), EmitterNamesToRemove);
	ParseOptionalStringArrayField(Input, TEXT("keep_only_emitter_names"), KeepOnlyEmitterNames);

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("system_path"), SystemPath);

	if (EmitterNamesToRemove.Num() == 0 && KeepOnlyEmitterNames.Num() == 0)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_REMOVE_EMITTERS_MISSING_TARGETS"),
			TEXT("Provide emitter_names or keep_only_emitter_names."));
	}

	UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *SystemPath);
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Niagara System '%s'."), *SystemPath));
	}

	const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	if (EmitterHandles.Num() == 0)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_NO_EMITTERS"),
			FString::Printf(TEXT("Niagara System '%s' has no emitters."), *NiagaraSystem->GetName()));
	}

	TSet<FString> RemoveNameSet;
	for (const FString& Name : EmitterNamesToRemove)
	{
		RemoveNameSet.Add(Name);
	}

	TSet<FString> KeepNameSet;
	for (const FString& Name : KeepOnlyEmitterNames)
	{
		KeepNameSet.Add(Name);
	}

	TSet<FGuid> EmitterIdsToDelete;
	TArray<TSharedPtr<FJsonValue>> RequestedEmitterNames;
	TArray<TSharedPtr<FJsonValue>> RemovedEmitters;

	for (const FString& Name : EmitterNamesToRemove)
	{
		RequestedEmitterNames.Add(MakeShareable(new FJsonValueString(Name)));
	}

	for (const FString& Name : KeepOnlyEmitterNames)
	{
		RequestedEmitterNames.Add(MakeShareable(new FJsonValueString(FString::Printf(TEXT("keep:%s"), *Name))));
	}

	for (const FNiagaraEmitterHandle& EmitterHandle : EmitterHandles)
	{
		const FString EmitterName = EmitterHandle.GetName().ToString();
		const bool bShouldDelete =
			(RemoveNameSet.Num() > 0 && RemoveNameSet.Contains(EmitterName)) ||
			(KeepNameSet.Num() > 0 && !KeepNameSet.Contains(EmitterName));
		if (!bShouldDelete)
		{
			continue;
		}

		EmitterIdsToDelete.Add(EmitterHandle.GetId());

		TSharedPtr<FJsonObject> RemovedEmitterObj = MakeShareable(new FJsonObject());
		RemovedEmitterObj->SetStringField(TEXT("name"), EmitterName);
		RemovedEmitterObj->SetStringField(TEXT("id"), EmitterHandle.GetId().ToString(EGuidFormats::DigitsWithHyphensLower));
		RemovedEmitters.Add(MakeShareable(new FJsonValueObject(RemovedEmitterObj)));
	}

	Result->SetArrayField(TEXT("requested_targets"), RequestedEmitterNames);
	Result->SetArrayField(TEXT("removed_emitters"), RemovedEmitters);
	Result->SetNumberField(TEXT("remove_count"), EmitterIdsToDelete.Num());
	Result->SetNumberField(TEXT("initial_emitter_count"), EmitterHandles.Num());

	if (EmitterIdsToDelete.Num() == 0)
	{
		Result->SetBoolField(TEXT("success"), true);
		Result->SetStringField(TEXT("message"), TEXT("No emitters matched the requested remove/keep filters."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!bApplyChanges)
	{
		Result->SetBoolField(TEXT("success"), true);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: emitters would be removed from the Niagara System."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("remove_niagara_emitters_from_system"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	NiagaraSystem->RemoveEmitterHandlesById(EmitterIdsToDelete);
	if (UNiagaraSystemEditorData* SystemEditorData = Cast<UNiagaraSystemEditorData>(NiagaraSystem->GetEditorData()))
	{
		SystemEditorData->SynchronizeOverviewGraphWithSystem(*NiagaraSystem);
	}
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	Result->SetNumberField(TEXT("final_emitter_count"), NiagaraSystem->GetEmitterHandles().Num());

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetNumberField(TEXT("removed_count"), EmitterIdsToDelete.Num());
	AuditDetails->SetNumberField(TEXT("initial_emitter_count"), EmitterHandles.Num());
	AuditDetails->SetNumberField(TEXT("final_emitter_count"), NiagaraSystem->GetEmitterHandles().Num());

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("remove_niagara_emitters_from_system"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Removed %d emitter(s) from '%s'."), EmitterIdsToDelete.Num(), *SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraEmitterEnabled(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString EmitterName;
	if (!Input->TryGetStringField(TEXT("emitter_name"), EmitterName) || EmitterName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_EMITTER_NAME"), TEXT("Missing required field: emitter_name"));
	}

	FString EnabledStr;
	if (!Input->TryGetStringField(TEXT("enabled"), EnabledStr))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ENABLED"), TEXT("Missing required field: enabled"));
	}

	bool bEnabled = EnabledStr.Equals(TEXT("true"), ESearchCase::IgnoreCase) || EnabledStr == TEXT("1");
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	FString SystemPath;
	Input->TryGetStringField(TEXT("system_path"), SystemPath);
	FString ActorLabel;
	Input->TryGetStringField(TEXT("actor_label"), ActorLabel);

	if (SystemPath.IsEmpty() && ActorLabel.IsEmpty())
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_TARGET_MISSING"),
			TEXT("Provide either system_path (asset mode) or actor_label (component mode)."));
	}

	// Asset mode
	if (!SystemPath.IsEmpty())
	{
		UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
		if (!NiagaraSystem)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SYSTEM_NOT_FOUND"),
				FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
		}

		TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
		int32 MatchedIndex = INDEX_NONE;
		bool bCurrentEnabled = false;
		for (int32 Index = 0; Index < EmitterHandles.Num(); ++Index)
		{
			if (EmitterHandles[Index].GetName().ToString().Equals(EmitterName, ESearchCase::IgnoreCase))
			{
				MatchedIndex = Index;
				bCurrentEnabled = EmitterHandles[Index].GetIsEnabled();
				break;
			}
		}

		if (MatchedIndex == INDEX_NONE)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_EMITTER_NOT_FOUND"),
				FString::Printf(TEXT("Emitter '%s' not found in Niagara System '%s'."), *EmitterName, *SystemPath));
		}

		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
		Result->SetStringField(TEXT("action"), TEXT("set_niagara_emitter_enabled"));
		Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
		Result->SetStringField(TEXT("target_mode"), TEXT("asset"));
		Result->SetStringField(TEXT("system_path"), SystemPath);
		Result->SetStringField(TEXT("emitter_name"), EmitterName);
		Result->SetBoolField(TEXT("current_enabled"), bCurrentEnabled);
		Result->SetBoolField(TEXT("target_enabled"), bEnabled);

		if (!bApplyChanges)
		{
			Result->SetStringField(
				TEXT("message"),
				FString::Printf(TEXT("Dry-run: would set emitter '%s' enabled=%s on '%s'."),
					*EmitterName,
					bEnabled ? TEXT("true") : TEXT("false"),
					*SystemPath));
			OutResult = JsonObjToString(Result);
			return true;
		}

		FString SnapshotPath;
		FString SnapshotError;
		if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_emitter_enabled"), SnapshotPath, SnapshotError))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
		}

		EmitterHandles[MatchedIndex].SetIsEnabled(bEnabled, *NiagaraSystem, true);
		NiagaraSystem->MarkPackageDirty();
		FString CompileSaveError;
		TSharedPtr<FJsonObject> CompileDiagnostics;
		const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

		TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
		AuditDetails->SetStringField(TEXT("target_mode"), TEXT("asset"));
		AuditDetails->SetStringField(TEXT("emitter_name"), EmitterName);
		AuditDetails->SetBoolField(TEXT("target_enabled"), bEnabled);

		return FinalizeNiagaraAssetMutation(
			OutResult,
			Result,
			TEXT("set_niagara_emitter_enabled"),
			NiagaraSystem,
			SnapshotPath,
			bSaveSucceeded,
			FString::Printf(TEXT("Set emitter '%s' enabled=%s on '%s'."),
				*EmitterName,
				bEnabled ? TEXT("true") : TEXT("false"),
				*SystemPath),
			AuditDetails,
			CompileSaveError,
			CompileDiagnostics);
	}

	// Component mode
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_NO_EDITOR_WORLD"), TEXT("No editor world available."));
	}

	AActor* FoundActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel().Equals(ActorLabel, ESearchCase::CaseSensitive))
		{
			FoundActor = *It;
			break;
		}
	}

	if (!FoundActor)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ACTOR_NOT_FOUND"),
			FString::Printf(TEXT("Actor with label '%s' not found."), *ActorLabel));
	}

	UNiagaraComponent* NiagaraComp = FoundActor->FindComponentByClass<UNiagaraComponent>();
	if (!NiagaraComp)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_COMPONENT_NOT_FOUND"),
			FString::Printf(TEXT("Actor '%s' has no Niagara Component."), *ActorLabel));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_emitter_enabled"));
	Result->SetBoolField(TEXT("will_mutate_component"), bApplyChanges);
	Result->SetStringField(TEXT("target_mode"), TEXT("component"));
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetStringField(TEXT("emitter_name"), EmitterName);
	Result->SetBoolField(TEXT("target_enabled"), bEnabled);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would set emitter '%s' enabled=%s on actor '%s'."),
				*EmitterName,
				bEnabled ? TEXT("true") : TEXT("false"),
				*ActorLabel));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraComp->SetEmitterEnable(FName(*EmitterName), bEnabled);

	Result->SetStringField(
		TEXT("message"),
		FString::Printf(TEXT("Set emitter '%s' enabled=%s on actor '%s'."),
			*EmitterName,
			bEnabled ? TEXT("true") : TEXT("false"),
			*ActorLabel));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetNiagaraRendererEnabled(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString EnabledString;
	if (!Input->TryGetStringField(TEXT("enabled"), EnabledString))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ENABLED"), TEXT("Missing required field: enabled"));
	}

	const bool bEnabled = EnabledString.Equals(TEXT("true"), ESearchCase::IgnoreCase) || EnabledString == TEXT("1");
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_renderer_enabled"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetBoolField(TEXT("target_enabled"), bEnabled);
	Result->SetBoolField(TEXT("current_enabled"), Renderer->GetIsEnabled());

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would set renderer '%s' enabled=%s on emitter '%s' in '%s'."),
				*Renderer->GetFName().ToString(),
				bEnabled ? TEXT("true") : TEXT("false"),
				*EmitterHandle->GetName().ToString(),
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	Renderer->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_renderer_enabled"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	Renderer->SetIsEnabled(bEnabled);
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandle->GetName().ToString());
	AuditDetails->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	AuditDetails->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
	AuditDetails->SetNumberField(TEXT("renderer_index"), RendererIndex);
	AuditDetails->SetBoolField(TEXT("target_enabled"), bEnabled);

	Result->SetBoolField(TEXT("enabled"), Renderer->GetIsEnabled());
	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_renderer_enabled"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Set renderer '%s' enabled=%s on emitter '%s' in '%s'."),
			*Renderer->GetFName().ToString(),
			bEnabled ? TEXT("true") : TEXT("false"),
			*EmitterHandle->GetName().ToString(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraRendererMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) || MaterialPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_MATERIAL_PATH"), TEXT("Missing required field: material_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	UMaterialInterface* TargetMaterial = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!TargetMaterial)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_MATERIAL_NOT_FOUND"),
			FString::Printf(TEXT("Material not found at '%s'."), *MaterialPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	int32 OverrideMaterialIndex = 0;
	if (Input->HasTypedField<EJson::Number>(TEXT("override_material_index")))
	{
		OverrideMaterialIndex = static_cast<int32>(Input->GetNumberField(TEXT("override_material_index")));
	}
	else
	{
		FString OverrideIndexString;
		if (Input->TryGetStringField(TEXT("override_material_index"), OverrideIndexString) && !OverrideIndexString.IsEmpty())
		{
			OverrideMaterialIndex = FCString::Atoi(*OverrideIndexString);
		}
	}
	OverrideMaterialIndex = FMath::Max(0, OverrideMaterialIndex);

	const bool bEnableOverrideMaterials = ParseOptionalBoolField(Input, TEXT("enable_override_materials"), true);

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_renderer_material"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("target_material_path"), TargetMaterial->GetPathName());

	FString MaterialActionDescription;
	if (UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer))
	{
		Result->SetStringField(TEXT("current_material_path"), SpriteRenderer->Material ? SpriteRenderer->Material->GetPathName() : TEXT(""));
		MaterialActionDescription = FString::Printf(
			TEXT("set sprite renderer material to '%s'"),
			*TargetMaterial->GetPathName());
	}
	else if (UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer))
	{
		Result->SetStringField(TEXT("current_material_path"), RibbonRenderer->Material ? RibbonRenderer->Material->GetPathName() : TEXT(""));
		MaterialActionDescription = FString::Printf(
			TEXT("set ribbon renderer material to '%s'"),
			*TargetMaterial->GetPathName());
	}
	else if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		Result->SetBoolField(TEXT("target_enable_override_materials"), bEnableOverrideMaterials);
		Result->SetNumberField(TEXT("target_override_material_index"), OverrideMaterialIndex);
		FString CurrentExplicitPath;
		if (MeshRenderer->OverrideMaterials.IsValidIndex(OverrideMaterialIndex))
		{
			CurrentExplicitPath = MeshRenderer->OverrideMaterials[OverrideMaterialIndex].ExplicitMat
				? MeshRenderer->OverrideMaterials[OverrideMaterialIndex].ExplicitMat->GetPathName()
				: TEXT("");
		}
		Result->SetStringField(TEXT("current_material_path"), CurrentExplicitPath);
		MaterialActionDescription = FString::Printf(
			TEXT("set mesh renderer override material slot %d to '%s'"),
			OverrideMaterialIndex,
			*TargetMaterial->GetPathName());
	}
	else
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RENDERER_MATERIAL_UNSUPPORTED"),
			FString::Printf(TEXT("Renderer class '%s' is not currently supported by set_niagara_renderer_material."), *Renderer->GetClass()->GetName()));
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would %s on emitter '%s' in '%s'."),
				*MaterialActionDescription,
				*EmitterHandle->GetName().ToString(),
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	Renderer->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_renderer_material"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	if (UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer))
	{
		SpriteRenderer->Material = TargetMaterial;
	}
	else if (UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer))
	{
		RibbonRenderer->Material = TargetMaterial;
	}
	else if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		MeshRenderer->bOverrideMaterials = bEnableOverrideMaterials;
		if (MeshRenderer->OverrideMaterials.Num() <= OverrideMaterialIndex)
		{
			MeshRenderer->OverrideMaterials.SetNum(OverrideMaterialIndex + 1);
		}
		MeshRenderer->OverrideMaterials[OverrideMaterialIndex].ExplicitMat = TargetMaterial;
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> UpdatedDetails = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);

	for (const auto& Pair : UpdatedDetails->Values)
	{
		Result->SetField(Pair.Key, Pair.Value);
	}

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandle->GetName().ToString());
	AuditDetails->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	AuditDetails->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
	AuditDetails->SetNumberField(TEXT("renderer_index"), RendererIndex);
	AuditDetails->SetStringField(TEXT("target_material_path"), TargetMaterial->GetPathName());
	if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		AuditDetails->SetBoolField(TEXT("target_enable_override_materials"), MeshRenderer->bOverrideMaterials);
		AuditDetails->SetNumberField(TEXT("target_override_material_index"), OverrideMaterialIndex);
	}

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_renderer_material"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Updated renderer '%s' material on emitter '%s' in '%s'."),
			*Renderer->GetFName().ToString(),
			*EmitterHandle->GetName().ToString(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraRibbonDeterministicStreams(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	int32 StreamCount = 0;
	if (!TryReadOptionalIntField(Input, TEXT("stream_count"), StreamCount) || StreamCount <= 0)
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_STREAM_COUNT"), TEXT("Missing or invalid required field: stream_count"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	FString RibbonIdAttributeName = TEXT("Particles.RibbonStreamID");
	Input->TryGetStringField(TEXT("ribbon_id_attribute"), RibbonIdAttributeName);
	RibbonIdAttributeName = RibbonIdAttributeName.TrimStartAndEnd();
	if (RibbonIdAttributeName.IsEmpty())
	{
		RibbonIdAttributeName = TEXT("Particles.RibbonStreamID");
	}

	FString RibbonLinkOrderAttributeName = TEXT("Particles.RibbonStreamLinkOrder");
	Input->TryGetStringField(TEXT("ribbon_link_order_attribute"), RibbonLinkOrderAttributeName);
	RibbonLinkOrderAttributeName = RibbonLinkOrderAttributeName.TrimStartAndEnd();
	if (RibbonLinkOrderAttributeName.IsEmpty())
	{
		RibbonLinkOrderAttributeName = TEXT("Particles.RibbonStreamLinkOrder");
	}

	int32 TargetIndex = 0;
	TryReadOptionalIntField(Input, TEXT("target_index"), TargetIndex);
	TargetIndex = FMath::Max(0, TargetIndex);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	const bool bRendererSpecified =
		(Input->HasTypedField<EJson::Number>(TEXT("renderer_index")) ||
		 Input->HasField(TEXT("renderer_index")) ||
		 Input->HasField(TEXT("renderer_name")));
	if (!bRendererSpecified && EmitterData)
	{
		const TArray<UNiagaraRendererProperties*>& Renderers = EmitterData->GetRenderers();
		int32 PreferredEnabledRibbonIndex = INDEX_NONE;
		int32 PreferredAnyRibbonIndex = INDEX_NONE;
		for (int32 Index = 0; Index < Renderers.Num(); ++Index)
		{
			UNiagaraRendererProperties* Candidate = Renderers[Index];
			if (!Candidate || !Cast<UNiagaraRibbonRendererProperties>(Candidate))
			{
				continue;
			}

			if (PreferredAnyRibbonIndex == INDEX_NONE)
			{
				PreferredAnyRibbonIndex = Index;
			}
			if (Candidate->GetIsEnabled())
			{
				PreferredEnabledRibbonIndex = Index;
				break;
			}
		}

		if (PreferredEnabledRibbonIndex != INDEX_NONE)
		{
			Input->SetNumberField(TEXT("renderer_index"), PreferredEnabledRibbonIndex);
		}
		else if (PreferredAnyRibbonIndex != INDEX_NONE)
		{
			Input->SetNumberField(TEXT("renderer_index"), PreferredAnyRibbonIndex);
		}
	}

	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer);
	if (!RibbonRenderer)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RENDERER_NOT_RIBBON"),
			FString::Printf(TEXT("Renderer '%s' on emitter '%s' is not a ribbon renderer."), *Renderer->GetFName().ToString(), *EmitterHandle->GetName().ToString()));
	}

	UNiagaraScript* ParticleSpawnScript = nullptr;
	UNiagaraNodeOutput* ParticleSpawnOutputNode = nullptr;
	if (!ResolveParticleSpawnOutputNode(EmitterData, ParticleSpawnScript, ParticleSpawnOutputNode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_PARTICLE_SPAWN_STAGE_NOT_FOUND"), ErrorMessage);
	}

	const FNiagaraVariable RibbonIdVariable(FNiagaraTypeDefinition::GetIntDef(), *RibbonIdAttributeName);
	const FNiagaraVariable RibbonLinkOrderVariable(FNiagaraTypeDefinition::GetFloatDef(), *RibbonLinkOrderAttributeName);
	const TArray<FNiagaraVariable> AssignmentTargets{ RibbonIdVariable, RibbonLinkOrderVariable };
	const TArray<FString> AssignmentDefaultExpressions
	{
		FString::Printf(TEXT("(Particles.UniqueID - ((Particles.UniqueID / %d) * %d))"), StreamCount, StreamCount),
		TEXT("(Particles.UniqueID * 1.0f)")
	};

	UNiagaraNodeAssignment* ExistingAssignmentNode = FindAssignmentNodeByTargets(*ParticleSpawnOutputNode, AssignmentTargets);

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_ribbon_deterministic_streams"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetNumberField(TEXT("stream_count"), StreamCount);
	Result->SetStringField(TEXT("ribbon_id_attribute"), RibbonIdAttributeName);
	Result->SetStringField(TEXT("ribbon_link_order_attribute"), RibbonLinkOrderAttributeName);
	Result->SetStringField(TEXT("ribbon_id_expression"), AssignmentDefaultExpressions[0]);
	Result->SetStringField(TEXT("ribbon_link_order_expression"), AssignmentDefaultExpressions[1]);
	Result->SetStringField(TEXT("particle_spawn_script_path"), ParticleSpawnScript ? ParticleSpawnScript->GetPathName() : TEXT(""));
	Result->SetNumberField(TEXT("target_index"), TargetIndex);
	Result->SetStringField(TEXT("assignment_action"), ExistingAssignmentNode ? TEXT("reuse_existing_assignment_module") : TEXT("add_assignment_module"));
	if (ExistingAssignmentNode)
	{
		Result->SetStringField(TEXT("assignment_module_key"), ExistingAssignmentNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	}

	TArray<TSharedPtr<FJsonValue>> AssignmentTargetArray;
	for (const FNiagaraVariable& AssignmentTarget : AssignmentTargets)
	{
		AssignmentTargetArray.Add(MakeShareable(new FJsonValueString(AssignmentTarget.GetName().ToString())));
	}
	Result->SetArrayField(TEXT("assignment_targets"), AssignmentTargetArray);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would %s and bind ribbon renderer '%s' on emitter '%s' in '%s' to deterministic stream attributes."),
				ExistingAssignmentNode ? TEXT("reuse the existing Set Variables module") : TEXT("add a new Set Variables module"),
				*Renderer->GetFName().ToString(),
				*EmitterHandle->GetName().ToString(),
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	Renderer->Modify();
	ParticleSpawnScript->Modify();
	ParticleSpawnOutputNode->Modify();

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_ribbon_deterministic_streams"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	bool bAssignmentNodeChanged = false;
	UNiagaraNodeAssignment* AssignmentNode = ExistingAssignmentNode;
	if (!AssignmentNode)
	{
		AssignmentNode = FNiagaraStackGraphUtilities::AddParameterModuleToStack(
			AssignmentTargets,
			*ParticleSpawnOutputNode,
			TargetIndex,
			AssignmentDefaultExpressions);
		if (!AssignmentNode)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_ADD_ASSIGNMENT_MODULE_FAILED"),
				TEXT("Failed to add the deterministic ribbon Set Variables module to Particle Spawn."));
		}
		bAssignmentNodeChanged = true;
	}

	bool bAssignmentTargetsChanged = false;
	if (!EnsureAssignmentNodeTargets(*AssignmentNode, AssignmentTargets, AssignmentDefaultExpressions, bAssignmentTargetsChanged, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_ASSIGNMENT_TARGET_UPDATE_FAILED"), ErrorMessage);
	}

	VFXNiagaraStackTools::FEmitterContext StackContext;
	TSharedPtr<FJsonObject> StackContextInput = MakeShareable(new FJsonObject());
	StackContextInput->SetStringField(TEXT("system_path"), SystemPath);
	StackContextInput->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	FString StackErrorCode;
	if (!VFXNiagaraStackTools::ResolveEmitterContext(SystemPath, StackContextInput, StackContext, StackErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_STACK_CONTEXT_REFRESH_FAILED"),
			FString::Printf(TEXT("Deterministic ribbon assignment module was created, but the Niagara stack context could not be refreshed (%s: %s)."), *StackErrorCode, *ErrorMessage));
	}

	const FString AssignmentModuleKey = AssignmentNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower);
	const FString RibbonIdInputKey = FString::Printf(TEXT("Module.%s"), *RibbonIdAttributeName);
	const FString RibbonLinkOrderInputKey = FString::Printf(TEXT("Module.%s"), *RibbonLinkOrderAttributeName);

	UNiagaraStackFunctionInput* RibbonIdInput = VFXNiagaraStackTools::FindInputByKey(StackContext, AssignmentModuleKey, RibbonIdInputKey);
	if (!RibbonIdInput)
	{
		FString MatchedInputKey;
		RibbonIdInput = VFXNiagaraStackTools::FindInputByHeuristics(
			StackContext,
			AssignmentModuleKey,
			{ TEXT("RibbonStreamID"), TEXT("Particles.RibbonStreamID") },
			MatchedInputKey);
	}

	UNiagaraStackFunctionInput* RibbonLinkOrderInput = VFXNiagaraStackTools::FindInputByKey(StackContext, AssignmentModuleKey, RibbonLinkOrderInputKey);
	if (!RibbonLinkOrderInput)
	{
		FString MatchedInputKey;
		RibbonLinkOrderInput = VFXNiagaraStackTools::FindInputByHeuristics(
			StackContext,
			AssignmentModuleKey,
			{ TEXT("RibbonStreamLinkOrder"), TEXT("Particles.RibbonStreamLinkOrder") },
			MatchedInputKey);
	}

	if (!RibbonIdInput || !RibbonLinkOrderInput)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ASSIGNMENT_INPUTS_NOT_FOUND"),
			TEXT("Deterministic ribbon assignment module was created, but the generated stack inputs for ribbon stream id/link order could not be resolved."));
	}

	RibbonIdInput->SetCustomExpression(AssignmentDefaultExpressions[0]);
	RibbonLinkOrderInput->SetCustomExpression(AssignmentDefaultExpressions[1]);

	const bool bRibbonIdExpressionApplied = RibbonIdInput->GetValueMode() == UNiagaraStackFunctionInput::EValueMode::Expression;
	const bool bRibbonLinkOrderExpressionApplied = RibbonLinkOrderInput->GetValueMode() == UNiagaraStackFunctionInput::EValueMode::Expression;
	if (!bRibbonIdExpressionApplied || !bRibbonLinkOrderExpressionApplied)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_ASSIGNMENT_EXPRESSION_APPLY_FAILED"),
			TEXT("Failed to switch the deterministic ribbon assignment inputs into expression mode."));
	}

	const FNiagaraVariableAttributeBinding PreviousRibbonIdBinding = RibbonRenderer->RibbonIdBinding;
	const FNiagaraVariableAttributeBinding PreviousRibbonLinkOrderBinding = RibbonRenderer->RibbonLinkOrderBinding;
	const bool bPreviousUniqueIdFallback = RibbonRenderer->bLinkOrderUseUniqueID != 0;

	RibbonRenderer->RibbonIdBinding = FNiagaraConstants::GetAttributeDefaultBinding(RibbonIdVariable);
	RibbonRenderer->RibbonLinkOrderBinding = FNiagaraConstants::GetAttributeDefaultBinding(RibbonLinkOrderVariable);
	RibbonRenderer->bLinkOrderUseUniqueID = false;

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> UpdatedDetails = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	for (const auto& Pair : UpdatedDetails->Values)
	{
		Result->SetField(Pair.Key, Pair.Value);
	}

	Result->SetStringField(TEXT("assignment_module_key"), AssignmentNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	Result->SetBoolField(TEXT("assignment_node_created"), bAssignmentNodeChanged);
	Result->SetBoolField(TEXT("assignment_targets_updated"), bAssignmentTargetsChanged);
	Result->SetBoolField(TEXT("ribbon_id_expression_applied"), bRibbonIdExpressionApplied);
	Result->SetBoolField(TEXT("ribbon_link_order_expression_applied"), bRibbonLinkOrderExpressionApplied);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandle->GetName().ToString());
	AuditDetails->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	AuditDetails->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
	AuditDetails->SetNumberField(TEXT("renderer_index"), RendererIndex);
	AuditDetails->SetNumberField(TEXT("stream_count"), StreamCount);
	AuditDetails->SetStringField(TEXT("ribbon_id_attribute"), RibbonIdAttributeName);
	AuditDetails->SetStringField(TEXT("ribbon_link_order_attribute"), RibbonLinkOrderAttributeName);
	AuditDetails->SetStringField(TEXT("assignment_module_key"), AssignmentNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphensLower));
	AuditDetails->SetBoolField(TEXT("assignment_node_created"), bAssignmentNodeChanged);
	AuditDetails->SetBoolField(TEXT("assignment_targets_updated"), bAssignmentTargetsChanged);
	AuditDetails->SetBoolField(TEXT("ribbon_id_expression_applied"), bRibbonIdExpressionApplied);
	AuditDetails->SetBoolField(TEXT("ribbon_link_order_expression_applied"), bRibbonLinkOrderExpressionApplied);
	AuditDetails->SetStringField(TEXT("previous_ribbon_id_binding"), PreviousRibbonIdBinding.GetParamMapBindableVariable().GetName().ToString());
	AuditDetails->SetStringField(TEXT("previous_ribbon_link_order_binding"), PreviousRibbonLinkOrderBinding.GetParamMapBindableVariable().GetName().ToString());
	AuditDetails->SetBoolField(TEXT("previous_ribbon_link_order_use_unique_id_fallback"), bPreviousUniqueIdFallback);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_ribbon_deterministic_streams"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Bound ribbon renderer '%s' on emitter '%s' in '%s' to deterministic ribbon stream attributes."),
			*Renderer->GetFName().ToString(),
			*EmitterHandle->GetName().ToString(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraSpriteRendererSubUV(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	double ColumnsValue = 0.0;
	if (!Input->TryGetNumberField(TEXT("columns"), ColumnsValue))
	{
		FString ColumnsString;
		if (!Input->TryGetStringField(TEXT("columns"), ColumnsString) || ColumnsString.IsEmpty())
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_COLUMNS"), TEXT("Missing required field: columns"));
		}
		ColumnsValue = FCString::Atod(*ColumnsString);
	}

	double RowsValue = 0.0;
	if (!Input->TryGetNumberField(TEXT("rows"), RowsValue))
	{
		FString RowsString;
		if (!Input->TryGetStringField(TEXT("rows"), RowsString) || RowsString.IsEmpty())
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ROWS"), TEXT("Missing required field: rows"));
		}
		RowsValue = FCString::Atod(*RowsString);
	}

	const float Columns = FMath::Max(1.0f, static_cast<float>(ColumnsValue));
	const float Rows = FMath::Max(1.0f, static_cast<float>(RowsValue));
	const bool bEnableBlend = ParseOptionalBoolField(Input, TEXT("enable_blend"), true);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer);
	if (!SpriteRenderer)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RENDERER_NOT_SPRITE"),
			FString::Printf(TEXT("Renderer '%s' is not a Niagara sprite renderer."), *Renderer->GetFName().ToString()));
	}

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_sprite_renderer_subuv"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("current_sub_image_size"), FString::Printf(TEXT("(X=%.3f,Y=%.3f)"), SpriteRenderer->SubImageSize.X, SpriteRenderer->SubImageSize.Y));
	Result->SetBoolField(TEXT("current_sub_image_blend"), SpriteRenderer->bSubImageBlend != 0);
	Result->SetStringField(TEXT("target_sub_image_size"), FString::Printf(TEXT("(X=%.3f,Y=%.3f)"), Columns, Rows));
	Result->SetBoolField(TEXT("target_sub_image_blend"), bEnableBlend);

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would set sprite renderer SubUV to %.0fx%.0f on emitter '%s' in '%s'."),
				Columns,
				Rows,
				*EmitterHandle->GetName().ToString(),
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	Renderer->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_sprite_renderer_subuv"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	SpriteRenderer->SubImageSize = FVector2D(Columns, Rows);
	SpriteRenderer->bSubImageBlend = bEnableBlend ? 1 : 0;
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandle->GetName().ToString());
	AuditDetails->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	AuditDetails->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
	AuditDetails->SetNumberField(TEXT("renderer_index"), RendererIndex);
	AuditDetails->SetStringField(TEXT("target_sub_image_size"), FString::Printf(TEXT("(X=%.3f,Y=%.3f)"), Columns, Rows));
	AuditDetails->SetBoolField(TEXT("target_sub_image_blend"), bEnableBlend);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_sprite_renderer_subuv"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Updated sprite renderer SubUV to %.0fx%.0f on emitter '%s' in '%s'."),
			Columns,
			Rows,
			*EmitterHandle->GetName().ToString(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraRendererMaterialUserParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString UserParamName;
	if (!Input->TryGetStringField(TEXT("user_param_name"), UserParamName) || UserParamName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_USER_PARAM_NAME"), TEXT("Missing required field: user_param_name"));
	}
	UserParamName = NormalizeNiagaraUserParameterName(UserParamName);
	if (UserParamName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_USER_PARAM_NAME"), TEXT("user_param_name resolved to an empty value."));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	int32 EmitterIndex = INDEX_NONE;
	FNiagaraEmitterHandle* EmitterHandle = nullptr;
	FVersionedNiagaraEmitterData* EmitterData = nullptr;
	FString ErrorCode;
	FString ErrorMessage;
	if (!ResolveNiagaraEmitterTarget(NiagaraSystem, Input, EmitterIndex, EmitterHandle, EmitterData, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 RendererIndex = INDEX_NONE;
	UNiagaraRendererProperties* Renderer = nullptr;
	if (!ResolveNiagaraRendererTarget(EmitterData, Input, RendererIndex, Renderer, ErrorCode, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, ErrorCode, ErrorMessage);
	}

	int32 OverrideMaterialIndex = 0;
	if (Input->HasTypedField<EJson::Number>(TEXT("override_material_index")))
	{
		OverrideMaterialIndex = static_cast<int32>(Input->GetNumberField(TEXT("override_material_index")));
	}
	else
	{
		FString OverrideIndexString;
		if (Input->TryGetStringField(TEXT("override_material_index"), OverrideIndexString) && !OverrideIndexString.IsEmpty())
		{
			OverrideMaterialIndex = FCString::Atoi(*OverrideIndexString);
		}
	}
	OverrideMaterialIndex = FMath::Max(0, OverrideMaterialIndex);

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FNiagaraTypeDefinition MaterialTypeDef(UMaterialInterface::StaticClass());
	const FNiagaraVariable BindingVariable(MaterialTypeDef, FName(*UserParamName));

	TSharedPtr<FJsonObject> Result = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_renderer_material_user_param"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("target_user_param_name"), UserParamName);

	FString ActionDescription;
	if (UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer))
	{
		Result->SetStringField(TEXT("current_user_param_name"), SpriteRenderer->MaterialUserParamBinding.Parameter.GetName().ToString());
		ActionDescription = FString::Printf(TEXT("set sprite renderer material user param to '%s'"), *UserParamName);
	}
	else if (UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer))
	{
		Result->SetStringField(TEXT("current_user_param_name"), RibbonRenderer->MaterialUserParamBinding.Parameter.GetName().ToString());
		ActionDescription = FString::Printf(TEXT("set ribbon renderer material user param to '%s'"), *UserParamName);
	}
	else if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		FString CurrentUserParamName;
		if (MeshRenderer->OverrideMaterials.IsValidIndex(OverrideMaterialIndex))
		{
			CurrentUserParamName = MeshRenderer->OverrideMaterials[OverrideMaterialIndex].UserParamBinding.Parameter.GetName().ToString();
		}
		Result->SetStringField(TEXT("current_user_param_name"), CurrentUserParamName);
		Result->SetNumberField(TEXT("target_override_material_index"), OverrideMaterialIndex);
		ActionDescription = FString::Printf(TEXT("set mesh renderer override material user param slot %d to '%s'"), OverrideMaterialIndex, *UserParamName);
	}
	else
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RENDERER_USER_PARAM_UNSUPPORTED"),
			FString::Printf(TEXT("Renderer class '%s' is not currently supported by set_niagara_renderer_material_user_param."), *Renderer->GetClass()->GetName()));
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(
				TEXT("Dry-run: would %s on emitter '%s' in '%s'."),
				*ActionDescription,
				*EmitterHandle->GetName().ToString(),
				*SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	Renderer->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_renderer_material_user_param"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	if (UNiagaraSpriteRendererProperties* SpriteRenderer = Cast<UNiagaraSpriteRendererProperties>(Renderer))
	{
		SpriteRenderer->MaterialUserParamBinding.Parameter = BindingVariable;
	}
	else if (UNiagaraRibbonRendererProperties* RibbonRenderer = Cast<UNiagaraRibbonRendererProperties>(Renderer))
	{
		RibbonRenderer->MaterialUserParamBinding.Parameter = BindingVariable;
	}
	else if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		if (MeshRenderer->OverrideMaterials.Num() <= OverrideMaterialIndex)
		{
			MeshRenderer->OverrideMaterials.SetNum(OverrideMaterialIndex + 1);
		}
		MeshRenderer->OverrideMaterials[OverrideMaterialIndex].UserParamBinding.Parameter = BindingVariable;
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> UpdatedDetails = BuildNiagaraRendererDetailsObject(
		NiagaraSystem,
		EmitterHandle,
		EmitterData,
		EmitterIndex,
		Renderer,
		RendererIndex);
	for (const auto& Pair : UpdatedDetails->Values)
	{
		Result->SetField(Pair.Key, Pair.Value);
	}
	Result->SetStringField(TEXT("target_user_param_name"), UserParamName);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandle->GetName().ToString());
	AuditDetails->SetNumberField(TEXT("emitter_index"), EmitterIndex);
	AuditDetails->SetStringField(TEXT("renderer_name"), Renderer->GetFName().ToString());
	AuditDetails->SetNumberField(TEXT("renderer_index"), RendererIndex);
	AuditDetails->SetStringField(TEXT("target_user_param_name"), UserParamName);
	if (UNiagaraMeshRendererProperties* MeshRenderer = Cast<UNiagaraMeshRendererProperties>(Renderer))
	{
		AuditDetails->SetNumberField(TEXT("target_override_material_index"), OverrideMaterialIndex);
	}

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_renderer_material_user_param"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Updated renderer '%s' material user param binding on emitter '%s' in '%s'."),
			*Renderer->GetFName().ToString(),
			*EmitterHandle->GetName().ToString(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraSystemFixedBounds(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	FString EnabledString;
	if (!Input->TryGetStringField(TEXT("enabled"), EnabledString))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_ENABLED"), TEXT("Missing required field: enabled"));
	}

	const bool bEnableFixedBounds = EnabledString.Equals(TEXT("true"), ESearchCase::IgnoreCase) || EnabledString == TEXT("1");
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	FVector MinVector = FVector::ZeroVector;
	FVector MaxVector = FVector::ZeroVector;
	const bool bHasMin = Input->HasField(TEXT("min"));
	const bool bHasMax = Input->HasField(TEXT("max"));
	FString ParseError;
	if ((bHasMin || bHasMax) && !(bHasMin && bHasMax))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_BOUNDS_INCOMPLETE"), TEXT("Both 'min' and 'max' must be provided together."));
	}
	if (bHasMin && !TryParseVectorField(Input, TEXT("min"), MinVector, ParseError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_MIN_BOUNDS"), ParseError);
	}
	if (bHasMax && !TryParseVectorField(Input, TEXT("max"), MaxVector, ParseError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_MAX_BOUNDS"), ParseError);
	}
	if (bEnableFixedBounds && bHasMin && bHasMax &&
		(MinVector.X > MaxVector.X || MinVector.Y > MaxVector.Y || MinVector.Z > MaxVector.Z))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_BOUNDS_RANGE"), TEXT("Bounds min must be less than or equal to max on every axis."));
	}

	const FBox CurrentBounds = NiagaraSystem->GetFixedBounds();
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_system_fixed_bounds"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetBoolField(TEXT("current_enabled"), NiagaraSystem->bFixedBounds);
	Result->SetObjectField(TEXT("current_bounds"), BoxToJsonObject(CurrentBounds));
	Result->SetBoolField(TEXT("target_enabled"), bEnableFixedBounds);

	if (bHasMin && bHasMax)
	{
		FBox PlannedBounds(MinVector, MaxVector);
		Result->SetObjectField(TEXT("target_bounds"), BoxToJsonObject(PlannedBounds));
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would set system fixed bounds enabled=%s on '%s'."), bEnableFixedBounds ? TEXT("true") : TEXT("false"), *SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_system_fixed_bounds"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	NiagaraSystem->bFixedBounds = bEnableFixedBounds;
	if (bHasMin && bHasMax)
	{
		NiagaraSystem->SetFixedBounds(FBox(MinVector, MaxVector));
	}
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetBoolField(TEXT("target_enabled"), bEnableFixedBounds);
	if (bHasMin && bHasMax)
	{
		AuditDetails->SetObjectField(TEXT("target_bounds"), BoxToJsonObject(FBox(MinVector, MaxVector)));
	}

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_system_fixed_bounds"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Updated system fixed bounds enabled=%s on '%s'."), bEnableFixedBounds ? TEXT("true") : TEXT("false"), *SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_SetNiagaraEmitterFixedBounds(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	FString EmitterName;
	Input->TryGetStringField(TEXT("emitter_name"), EmitterName);

	int32 EmitterIndex = INDEX_NONE;
	if (Input->HasTypedField<EJson::Number>(TEXT("emitter_index")))
	{
		EmitterIndex = static_cast<int32>(Input->GetNumberField(TEXT("emitter_index")));
	}
	else
	{
		FString EmitterIndexString;
		if (Input->TryGetStringField(TEXT("emitter_index"), EmitterIndexString) && !EmitterIndexString.IsEmpty())
		{
			EmitterIndex = FCString::Atoi(*EmitterIndexString);
		}
	}

	TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	if (EmitterHandles.Num() == 0)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_NO_EMITTERS"),
			FString::Printf(TEXT("Niagara System '%s' has no emitters."), *SystemPath));
	}

	int32 MatchedIndex = INDEX_NONE;
	if (EmitterIndex != INDEX_NONE)
	{
		if (!EmitterHandles.IsValidIndex(EmitterIndex))
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_EMITTER_INDEX_OUT_OF_RANGE"),
				FString::Printf(TEXT("Emitter index %d is out of range for system '%s'."), EmitterIndex, *SystemPath));
		}
		MatchedIndex = EmitterIndex;
	}
	else if (!EmitterName.IsEmpty())
	{
		for (int32 Index = 0; Index < EmitterHandles.Num(); ++Index)
		{
			if (EmitterHandles[Index].GetName().ToString().Equals(EmitterName, ESearchCase::IgnoreCase))
			{
				MatchedIndex = Index;
				break;
			}
		}
		if (MatchedIndex == INDEX_NONE)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_EMITTER_NOT_FOUND"),
				FString::Printf(TEXT("Emitter '%s' not found in Niagara System '%s'."), *EmitterName, *SystemPath));
		}
	}
	else
	{
		MatchedIndex = 0;
	}

	FVersionedNiagaraEmitterData* EmitterData = EmitterHandles[MatchedIndex].GetEmitterData();
	if (!EmitterData)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_EMITTER_DATA_MISSING"),
			FString::Printf(TEXT("Emitter '%s' has no emitter data."), *EmitterHandles[MatchedIndex].GetName().ToString()));
	}

	FString BoundsModeString = TEXT("fixed");
	Input->TryGetStringField(TEXT("bounds_mode"), BoundsModeString);
	ENiagaraEmitterCalculateBoundMode TargetBoundsMode = ENiagaraEmitterCalculateBoundMode::Fixed;
	if (!TryParseBoundsMode(BoundsModeString, TargetBoundsMode))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_INVALID_BOUNDS_MODE"),
			TEXT("bounds_mode must be one of: fixed, dynamic, programmable."));
	}

	FVector MinVector = FVector::ZeroVector;
	FVector MaxVector = FVector::ZeroVector;
	const bool bHasMin = Input->HasField(TEXT("min"));
	const bool bHasMax = Input->HasField(TEXT("max"));
	FString ParseError;
	if ((bHasMin || bHasMax) && !(bHasMin && bHasMax))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_BOUNDS_INCOMPLETE"), TEXT("Both 'min' and 'max' must be provided together."));
	}
	if (bHasMin && !TryParseVectorField(Input, TEXT("min"), MinVector, ParseError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_MIN_BOUNDS"), ParseError);
	}
	if (bHasMax && !TryParseVectorField(Input, TEXT("max"), MaxVector, ParseError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_MAX_BOUNDS"), ParseError);
	}
	if (TargetBoundsMode == ENiagaraEmitterCalculateBoundMode::Fixed && bHasMin && bHasMax &&
		(MinVector.X > MaxVector.X || MinVector.Y > MaxVector.Y || MinVector.Z > MaxVector.Z))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INVALID_BOUNDS_RANGE"), TEXT("Bounds min must be less than or equal to max on every axis."));
	}
	if (TargetBoundsMode == ENiagaraEmitterCalculateBoundMode::Fixed && !(bHasMin && bHasMax))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_FIXED_BOUNDS_REQUIRED"), TEXT("Fixed bounds mode requires both 'min' and 'max' arrays."));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const FBox CurrentBounds = EmitterData->FixedBounds;
	const FString CurrentBoundsModeString = NiagaraEnumValueToString(StaticEnum<ENiagaraEmitterCalculateBoundMode>(), static_cast<int64>(EmitterData->CalculateBoundsMode));

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("set_niagara_emitter_fixed_bounds"));
	Result->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetNumberField(TEXT("matched_emitter_index"), MatchedIndex);
	Result->SetStringField(TEXT("emitter_name"), EmitterHandles[MatchedIndex].GetName().ToString());
	Result->SetStringField(TEXT("current_bounds_mode"), CurrentBoundsModeString);
	Result->SetObjectField(TEXT("current_fixed_bounds"), BoxToJsonObject(CurrentBounds));
	Result->SetStringField(TEXT("target_bounds_mode"), BoundsModeString.ToLower());
	if (bHasMin && bHasMax)
	{
		Result->SetObjectField(TEXT("target_fixed_bounds"), BoxToJsonObject(FBox(MinVector, MaxVector)));
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would set emitter '%s' bounds_mode=%s on '%s'."), *EmitterHandles[MatchedIndex].GetName().ToString(), *BoundsModeString.ToLower(), *SystemPath));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("set_niagara_emitter_fixed_bounds"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	EmitterData->CalculateBoundsMode = TargetBoundsMode;
	if (TargetBoundsMode == ENiagaraEmitterCalculateBoundMode::Fixed)
	{
		EmitterData->FixedBounds = FBox(MinVector, MaxVector);
	}
	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("emitter_name"), EmitterHandles[MatchedIndex].GetName().ToString());
	AuditDetails->SetStringField(TEXT("target_bounds_mode"), BoundsModeString.ToLower());
	if (bHasMin && bHasMax)
	{
		AuditDetails->SetObjectField(TEXT("target_fixed_bounds"), BoxToJsonObject(FBox(MinVector, MaxVector)));
	}

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("set_niagara_emitter_fixed_bounds"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(TEXT("Updated emitter '%s' bounds_mode=%s on '%s'."), *EmitterHandles[MatchedIndex].GetName().ToString(), *BoundsModeString.ToLower(), *SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_RestoreNiagaraMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	FString SnapshotPath;
	Input->TryGetStringField(TEXT("snapshot_path"), SnapshotPath);
	const bool bUseLatest = ParseOptionalBoolField(Input, TEXT("use_latest"), true);
	const bool bReopenEditor = ParseOptionalBoolField(Input, TEXT("reopen_editor"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	if (SnapshotPath.IsEmpty() && bUseLatest)
	{
		TArray<FString> SnapshotFiles;
		GetNiagaraSnapshotFiles(NiagaraSystem, SnapshotFiles);
		if (SnapshotFiles.Num() > 0)
		{
			SnapshotPath = SnapshotFiles[0];
		}
	}

	if (SnapshotPath.IsEmpty())
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SNAPSHOT_NOT_SPECIFIED"),
			TEXT("No snapshot_path was provided and no latest snapshot could be resolved."));
	}

	SnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
	if (!FPaths::FileExists(SnapshotPath))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SNAPSHOT_NOT_FOUND"),
			FString::Printf(TEXT("Snapshot file does not exist: %s"), *SnapshotPath));
	}

	if (!IsNiagaraSnapshotPathUnderRoot(SnapshotPath))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SNAPSHOT_OUT_OF_BOUNDS"),
			TEXT("Snapshot path must live under the ForgePilot NiagaraMutationSnapshots directory."));
	}

	const FString PackageFilename = GetAssetPackageFilename(NiagaraSystem);
	if (PackageFilename.IsEmpty() || !FPaths::FileExists(PackageFilename))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_PACKAGE_FILE_MISSING"),
			FString::Printf(TEXT("Niagara package file does not exist on disk: %s"), *PackageFilename));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("restore_niagara_mutation_snapshot"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("system_path"), SystemPath);
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetStringField(TEXT("package_filename"), PackageFilename);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would restore the Niagara System package from the selected snapshot."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString PreRestoreSnapshotPath;
	FString PreRestoreSnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("pre_restore"), PreRestoreSnapshotPath, PreRestoreSnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), PreRestoreSnapshotError);
	}

	int32 ClosedEditorCount = 0;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			ClosedEditorCount = AssetEditorSubsystem->CloseAllEditorsForAsset(NiagaraSystem);
		}
	}

	const uint32 CopyResult = IFileManager::Get().Copy(*PackageFilename, *SnapshotPath, true, true);
	if (CopyResult != COPY_OK)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RESTORE_COPY_FAILED"),
			FString::Printf(TEXT("Failed to copy snapshot into package file (copy code %u)."), CopyResult));
	}

	FText ReloadError;
	const bool bReloadSucceeded = UPackageTools::ReloadPackages(
		{ NiagaraSystem->GetOutermost() },
		ReloadError,
		EReloadPackagesInteractionMode::AssumePositive);

	UObject* ReloadedAsset = UEditorAssetLibrary::LoadAsset(SystemPath);
	UNiagaraSystem* ReloadedSystem = Cast<UNiagaraSystem>(ReloadedAsset);
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
	Result->SetStringField(TEXT("action"), TEXT("restore_niagara_mutation_snapshot"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildNiagaraMutationPatchReport(
		TEXT("restore_niagara_mutation_snapshot"),
		ReloadedSystem ? ReloadedSystem : NiagaraSystem,
		PreRestoreSnapshotPath,
		bReloadSucceeded,
		PatchDetails));

	const FString RestoreMessage = bReloadSucceeded
		? TEXT("Niagara snapshot restored and package reloaded.")
		: TEXT("Snapshot file was copied, but package reload reported a problem. See patch_report details.");
	Result->SetStringField(TEXT("message"), RestoreMessage);
	AppendNiagaraMutationAudit(
		Result,
		TEXT("restore_niagara_mutation_snapshot"),
		ReloadedSystem ? ReloadedSystem : NiagaraSystem,
		RestoreMessage,
		PreRestoreSnapshotPath,
		PatchDetails);

	OutResult = JsonObjToString(Result);
	return bReloadSucceeded;
}

bool FVFXToolRegistry::Tool_CleanupNiagaraAuditAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const FString FolderPath = Input.IsValid() && Input->HasField(TEXT("folder_path"))
		? Input->GetStringField(TEXT("folder_path"))
		: TEXT("/Game/ForgePilot_Audit");
	const FString NamePrefix = Input.IsValid() && Input->HasField(TEXT("name_prefix"))
		? Input->GetStringField(TEXT("name_prefix"))
		: FString();
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	FARFilter Filter;
	Filter.PackagePaths.Add(*FolderPath);
	Filter.ClassPaths.Add(UNiagaraSystem::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	TArray<TSharedPtr<FJsonValue>> MatchArray;
	int32 DeletedCount = 0;
	for (const FAssetData& AssetData : AssetDataList)
	{
		const FString AssetName = AssetData.AssetName.ToString();
		if (!NamePrefix.IsEmpty() && !AssetName.StartsWith(NamePrefix))
		{
			continue;
		}

		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		AssetObj->SetStringField(TEXT("asset_name"), AssetName);
		AssetObj->SetStringField(TEXT("asset_path"), AssetData.GetObjectPathString());
		MatchArray.Add(MakeShareable(new FJsonValueObject(AssetObj)));

		if (bApplyChanges)
		{
			if (UEditorAssetLibrary::DeleteAsset(AssetData.GetObjectPathString()))
			{
				++DeletedCount;
			}
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("cleanup_niagara_audit_assets"));
	Result->SetStringField(TEXT("folder_path"), FolderPath);
	Result->SetStringField(TEXT("name_prefix"), NamePrefix);
	Result->SetNumberField(TEXT("matched_count"), MatchArray.Num());
	Result->SetNumberField(TEXT("deleted_count"), DeletedCount);
	Result->SetArrayField(TEXT("matched_assets"), MatchArray);
	Result->SetStringField(TEXT("message"), bApplyChanges
		? FString::Printf(TEXT("Deleted %d Niagara audit asset(s)."), DeletedCount)
		: FString::Printf(TEXT("Dry-run: found %d Niagara audit asset(s) matching the filter."), MatchArray.Num()));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CleanupNiagaraAuthoringResidue(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	int32 ScopedEmitterIndex = INDEX_NONE;
	if (Input->HasTypedField<EJson::Number>(TEXT("emitter_index")))
	{
		ScopedEmitterIndex = static_cast<int32>(Input->GetNumberField(TEXT("emitter_index")));
	}
	else
	{
		FString EmitterIndexString;
		if (Input->TryGetStringField(TEXT("emitter_index"), EmitterIndexString) && !EmitterIndexString.IsEmpty())
		{
			ScopedEmitterIndex = FCString::Atoi(*EmitterIndexString);
		}
	}

	FString ScopedEmitterName;
	Input->TryGetStringField(TEXT("emitter_name"), ScopedEmitterName);

	const bool bRemoveDisabledRenderers = ParseOptionalBoolField(Input, TEXT("remove_disabled_renderers"), true);
	const bool bRemoveDisabledSpriteWhenRibbonPresent = ParseOptionalBoolField(Input, TEXT("remove_disabled_sprite_renderers_when_ribbon_present"), true);
	const bool bRemoveDisabledEmitters = ParseOptionalBoolField(Input, TEXT("remove_disabled_emitters"), false);
	const bool bRemoveEmptyEmitters = ParseOptionalBoolField(Input, TEXT("remove_empty_emitters"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	if (EmitterHandles.Num() == 0)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_NO_EMITTERS"),
			FString::Printf(TEXT("Niagara System '%s' has no emitters."), *SystemPath));
	}

	TArray<FNiagaraRendererCleanupPlan> PlannedRendererRemovals;
	TArray<FNiagaraEmitterCleanupPlan> PlannedEmitterRemovals;
	bool bAnyEmitterMatchedScope = false;

	for (int32 EmitterIndex = 0; EmitterIndex < EmitterHandles.Num(); ++EmitterIndex)
	{
		const FNiagaraEmitterHandle& EmitterHandle = EmitterHandles[EmitterIndex];
		const FString EmitterName = EmitterHandle.GetName().ToString();

		const bool bMatchesScope =
			(ScopedEmitterIndex != INDEX_NONE && EmitterIndex == ScopedEmitterIndex) ||
			(ScopedEmitterIndex == INDEX_NONE && !ScopedEmitterName.IsEmpty() && EmitterName.Equals(ScopedEmitterName, ESearchCase::IgnoreCase)) ||
			(ScopedEmitterIndex == INDEX_NONE && ScopedEmitterName.IsEmpty());
		if (!bMatchesScope)
		{
			continue;
		}

		bAnyEmitterMatchedScope = true;

		FVersionedNiagaraEmitterData* EmitterData = EmitterHandle.GetEmitterData();
		const TArray<UNiagaraRendererProperties*>* RenderersPtr = EmitterData ? &EmitterData->GetRenderers() : nullptr;

		bool bHasEnabledRibbonRenderer = false;
		if (RenderersPtr)
		{
			for (UNiagaraRendererProperties* Renderer : *RenderersPtr)
			{
				if (Renderer && Renderer->GetIsEnabled() && Cast<UNiagaraRibbonRendererProperties>(Renderer))
				{
					bHasEnabledRibbonRenderer = true;
					break;
				}
			}
		}

		if (bRemoveDisabledRenderers && RenderersPtr)
		{
			for (int32 RendererIndex = 0; RendererIndex < RenderersPtr->Num(); ++RendererIndex)
			{
				UNiagaraRendererProperties* Renderer = (*RenderersPtr)[RendererIndex];
				if (!Renderer || Renderer->GetIsEnabled())
				{
					continue;
				}

				FString Reason = TEXT("Disabled renderer.");
				if (Cast<UNiagaraSpriteRendererProperties>(Renderer) && bHasEnabledRibbonRenderer)
				{
					if (!bRemoveDisabledSpriteWhenRibbonPresent)
					{
						continue;
					}
					Reason = TEXT("Disabled sprite renderer left behind while an enabled ribbon renderer is present.");
				}

				FNiagaraRendererCleanupPlan Plan;
				Plan.EmitterId = EmitterHandle.GetId();
				Plan.EmitterName = EmitterName;
				Plan.EmitterIndex = EmitterIndex;
				Plan.RendererIndex = RendererIndex;
				Plan.RendererName = Renderer->GetFName().ToString();
				Plan.RendererClass = Renderer->GetClass()->GetName();
				Plan.Reason = Reason;
				Plan.Renderer = Renderer;
				PlannedRendererRemovals.Add(Plan);
			}
		}

		if (bRemoveEmptyEmitters && (!EmitterData || !RenderersPtr || RenderersPtr->Num() == 0))
		{
			FNiagaraEmitterCleanupPlan Plan;
			Plan.EmitterId = EmitterHandle.GetId();
			Plan.EmitterName = EmitterName;
			Plan.EmitterIndex = EmitterIndex;
			Plan.Reason = TEXT("Emitter has no renderer objects.");
			PlannedEmitterRemovals.Add(Plan);
			continue;
		}

		if (bRemoveDisabledEmitters && !EmitterHandle.GetIsEnabled())
		{
			FNiagaraEmitterCleanupPlan Plan;
			Plan.EmitterId = EmitterHandle.GetId();
			Plan.EmitterName = EmitterName;
			Plan.EmitterIndex = EmitterIndex;
			Plan.Reason = TEXT("Emitter is disabled and marked for cleanup.");
			PlannedEmitterRemovals.Add(Plan);
		}
	}

	if (!bAnyEmitterMatchedScope)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_EMITTER_NOT_FOUND"),
			ScopedEmitterIndex != INDEX_NONE
				? FString::Printf(TEXT("Emitter index %d was not found in '%s'."), ScopedEmitterIndex, *SystemPath)
				: FString::Printf(TEXT("Emitter '%s' was not found in '%s'."), *ScopedEmitterName, *SystemPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("cleanup_niagara_authoring_residue"));
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	if (ScopedEmitterIndex != INDEX_NONE)
	{
		Result->SetNumberField(TEXT("scoped_emitter_index"), ScopedEmitterIndex);
	}
	if (!ScopedEmitterName.IsEmpty())
	{
		Result->SetStringField(TEXT("scoped_emitter_name"), ScopedEmitterName);
	}
	Result->SetBoolField(TEXT("remove_disabled_renderers"), bRemoveDisabledRenderers);
	Result->SetBoolField(TEXT("remove_disabled_sprite_renderers_when_ribbon_present"), bRemoveDisabledSpriteWhenRibbonPresent);
	Result->SetBoolField(TEXT("remove_disabled_emitters"), bRemoveDisabledEmitters);
	Result->SetBoolField(TEXT("remove_empty_emitters"), bRemoveEmptyEmitters);

	TArray<TSharedPtr<FJsonValue>> RendererRemovalArray;
	for (const FNiagaraRendererCleanupPlan& Plan : PlannedRendererRemovals)
	{
		RendererRemovalArray.Add(MakeShareable(new FJsonValueObject(BuildNiagaraRendererCleanupObject(Plan))));
	}
	Result->SetArrayField(TEXT("planned_renderer_removals"), RendererRemovalArray);
	Result->SetNumberField(TEXT("planned_renderer_removal_count"), PlannedRendererRemovals.Num());

	TArray<TSharedPtr<FJsonValue>> EmitterRemovalArray;
	for (const FNiagaraEmitterCleanupPlan& Plan : PlannedEmitterRemovals)
	{
		EmitterRemovalArray.Add(MakeShareable(new FJsonValueObject(BuildNiagaraEmitterCleanupObject(Plan))));
	}
	Result->SetArrayField(TEXT("planned_emitter_removals"), EmitterRemovalArray);
	Result->SetNumberField(TEXT("planned_emitter_removal_count"), PlannedEmitterRemovals.Num());

	if (PlannedRendererRemovals.Num() == 0 && PlannedEmitterRemovals.Num() == 0)
	{
		Result->SetStringField(TEXT("message"), TEXT("No deterministic authoring residue matched the requested cleanup rules."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: deterministic Niagara authoring residue was identified and can be removed safely."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	NiagaraSystem->Modify();
	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("cleanup_niagara_authoring_residue"), SnapshotPath, SnapshotError))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SNAPSHOT_FAILED"), SnapshotError);
	}

	int32 RemovedRendererCount = 0;
	TSet<FGuid> EmittersScheduledForRemoval;
	for (const FNiagaraEmitterCleanupPlan& Plan : PlannedEmitterRemovals)
	{
		EmittersScheduledForRemoval.Add(Plan.EmitterId);
	}

	for (const FNiagaraRendererCleanupPlan& Plan : PlannedRendererRemovals)
	{
		if (EmittersScheduledForRemoval.Contains(Plan.EmitterId) || !Plan.Renderer)
		{
			continue;
		}

		for (FNiagaraEmitterHandle& EmitterHandle : NiagaraSystem->GetEmitterHandles())
		{
			if (EmitterHandle.GetId() != Plan.EmitterId)
			{
				continue;
			}

			FVersionedNiagaraEmitter Instance = EmitterHandle.GetInstance();
			if (Instance.Emitter)
			{
				Instance.Emitter->Modify();
				Instance.Emitter->RemoveRenderer(Plan.Renderer, Instance.Version);
				++RemovedRendererCount;
			}
			break;
		}
	}

	if (EmittersScheduledForRemoval.Num() > 0)
	{
		NiagaraSystem->RemoveEmitterHandlesById(EmittersScheduledForRemoval);
		if (UNiagaraSystemEditorData* SystemEditorData = Cast<UNiagaraSystemEditorData>(NiagaraSystem->GetEditorData()))
		{
			SystemEditorData->SynchronizeOverviewGraphWithSystem(*NiagaraSystem);
		}
	}

	NiagaraSystem->MarkPackageDirty();
	FString CompileSaveError;
	TSharedPtr<FJsonObject> CompileDiagnostics;
	const bool bSaveSucceeded = CompileAndSaveNiagaraAssetWithLogGuard(NiagaraSystem, CompileSaveError, CompileDiagnostics);

	Result->SetNumberField(TEXT("removed_renderer_count"), RemovedRendererCount);
	Result->SetNumberField(TEXT("removed_emitter_count"), EmittersScheduledForRemoval.Num());
	Result->SetNumberField(TEXT("final_emitter_count"), NiagaraSystem->GetEmitterHandles().Num());

	TSharedPtr<FJsonObject> AuditDetails = MakeShareable(new FJsonObject());
	AuditDetails->SetStringField(TEXT("system_path"), SystemPath);
	AuditDetails->SetNumberField(TEXT("removed_renderer_count"), RemovedRendererCount);
	AuditDetails->SetNumberField(TEXT("removed_emitter_count"), EmittersScheduledForRemoval.Num());
	AuditDetails->SetNumberField(TEXT("final_emitter_count"), NiagaraSystem->GetEmitterHandles().Num());
	AuditDetails->SetBoolField(TEXT("remove_disabled_renderers"), bRemoveDisabledRenderers);
	AuditDetails->SetBoolField(TEXT("remove_disabled_sprite_renderers_when_ribbon_present"), bRemoveDisabledSpriteWhenRibbonPresent);
	AuditDetails->SetBoolField(TEXT("remove_disabled_emitters"), bRemoveDisabledEmitters);
	AuditDetails->SetBoolField(TEXT("remove_empty_emitters"), bRemoveEmptyEmitters);

	return FinalizeNiagaraAssetMutation(
		OutResult,
		Result,
		TEXT("cleanup_niagara_authoring_residue"),
		NiagaraSystem,
		SnapshotPath,
		bSaveSucceeded,
		FString::Printf(
			TEXT("Removed %d renderer residue item(s) and %d emitter residue item(s) from '%s'."),
			RemovedRendererCount,
			EmittersScheduledForRemoval.Num(),
			*SystemPath),
		AuditDetails,
		CompileSaveError,
		CompileDiagnostics);
}

bool FVFXToolRegistry::Tool_ReviewNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_MISSING_SYSTEM_PATH"), TEXT("Missing required field: system_path"));
	}

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'"), *SystemPath));
	}

	const TArray<FNiagaraEmitterHandle>& EmitterHandles = NiagaraSystem->GetEmitterHandles();
	int32 DisabledEmitterCount = 0;
	int32 TotalRendererCount = 0;
	for (const FNiagaraEmitterHandle& Handle : EmitterHandles)
	{
		if (!Handle.GetIsEnabled())
		{
			++DisabledEmitterCount;
		}
		if (const FVersionedNiagaraEmitterData* EmitterData = Handle.GetEmitterData())
		{
			TotalRendererCount += EmitterData->GetRenderers().Num();
		}
	}

	const TArrayView<const FNiagaraVariableWithOffset> UserParams = NiagaraSystem->GetExposedParameters().ReadParameterVariables();

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

	if (EmitterHandles.Num() == 0)
	{
		AddFinding(
			TEXT("error"),
			TEXT("No emitters in system"),
			TEXT("Niagara system has zero emitters and will not render any effect."),
			TEXT("Add at least one emitter to the system."));
	}

	if (EmitterHandles.Num() > 0 && DisabledEmitterCount == EmitterHandles.Num())
	{
		AddFinding(
			TEXT("warning"),
			TEXT("All emitters disabled"),
			TEXT("Every emitter in the system is disabled."),
			TEXT("Enable at least one emitter or verify this is intended for template storage."));
	}

	if (TotalRendererCount == 0 && EmitterHandles.Num() > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("No renderers configured"),
			TEXT("Emitters exist but no renderer properties were found."),
			TEXT("Add sprite/mesh/ribbon renderer modules to visible emitters."));
	}

	if (UserParams.Num() > 20)
	{
		AddFinding(
			TEXT("info"),
			TEXT("High user parameter count"),
			FString::Printf(TEXT("System exposes %d user parameters."), UserParams.Num()),
			TEXT("Group or prune parameters to simplify usage from Blueprint/Sequencer."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious issues"),
			TEXT("No high-signal issues detected by automated review rules."),
			TEXT("Run runtime profiling and visual validation in target gameplay scenes."));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_niagara_system"));
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetNumberField(TEXT("emitter_count"), EmitterHandles.Num());
	Result->SetNumberField(TEXT("disabled_emitter_count"), DisabledEmitterCount);
	Result->SetNumberField(TEXT("renderer_count"), TotalRendererCount);
	Result->SetNumberField(TEXT("user_parameter_count"), UserParams.Num());
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewNiagaraEmitter(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString EmitterDetailsJson;
	if (!Tool_GetNiagaraEmitterDetails(Input, EmitterDetailsJson))
	{
		OutResult = EmitterDetailsJson;
		return false;
	}

	TSharedPtr<FJsonObject> Details;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(EmitterDetailsJson);
	if (!FJsonSerializer::Deserialize(Reader, Details) || !Details.IsValid())
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_REVIEW_PARSE_FAILED"),
			TEXT("Failed to parse emitter details prior to review."));
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

	const FString EmitterName = Details->GetStringField(TEXT("emitter_name"));
	const FString SystemPath = Details->GetStringField(TEXT("system_path"));
	const FString SimTarget = Details->GetStringField(TEXT("sim_target"));
	const FString BoundsMode = Details->GetStringField(TEXT("calculate_bounds_mode"));
	const bool bEmitterEnabled = Details->GetBoolField(TEXT("enabled"));
	const bool bLocalSpace = Details->GetBoolField(TEXT("local_space"));
	const bool bSystemFixedBoundsEnabled = Details->GetBoolField(TEXT("system_fixed_bounds_enabled"));
	const int32 RendererCount = static_cast<int32>(Details->GetNumberField(TEXT("renderer_count")));
	const int32 DisabledRendererCount = static_cast<int32>(Details->GetNumberField(TEXT("disabled_renderer_count")));
	const int32 IncompatibleRendererCount = static_cast<int32>(Details->GetNumberField(TEXT("incompatible_renderer_count")));
	const int32 EventHandlerCount = static_cast<int32>(Details->GetNumberField(TEXT("event_handler_count")));
	const int32 SimulationStageCount = static_cast<int32>(Details->GetNumberField(TEXT("simulation_stage_count")));

	if (!bEmitterEnabled)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Emitter disabled"),
			TEXT("The emitter is disabled in the Niagara System and will not contribute at runtime."),
			TEXT("Enable the emitter or keep it disabled only if it is serving as a template or parked variation."));
	}

	if (RendererCount == 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("No render output"),
			TEXT("The emitter has no renderers, so it will only be useful for logic/event generation."),
			TEXT("Add a renderer or confirm this emitter is intentionally logic-only."));
	}
	else if (DisabledRendererCount == RendererCount)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("All renderers disabled"),
			TEXT("Renderer objects exist, but they are all disabled, which makes the emitter visually silent."),
			TEXT("Enable at least one renderer or remove unused renderer entries to reduce clutter."));
	}

	if (IncompatibleRendererCount > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Renderer incompatibility"),
			FString::Printf(TEXT("%d enabled renderer(s) are incompatible with sim target '%s'."), IncompatibleRendererCount, *SimTarget),
			TEXT("Swap renderer types or switch the emitter sim target so enabled renderers can draw correctly."));
	}

	if (SimTarget.Equals(TEXT("GPUComputeSim"), ESearchCase::IgnoreCase) &&
		BoundsMode.Equals(TEXT("Dynamic"), ESearchCase::IgnoreCase) &&
		!bSystemFixedBoundsEnabled)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Risky GPU bounds setup"),
			TEXT("GPU emitters using dynamic bounds without fixed system bounds can cull incorrectly in gameplay."),
			TEXT("Set emitter fixed bounds or enable fixed bounds on the parent Niagara System."));
	}

	if (SimulationStageCount > 0 && RendererCount == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Simulation-only emitter"),
			TEXT("The emitter uses simulation stages but has no renderer output, which is valid but often harder to debug."),
			TEXT("Document the emitter purpose clearly or pair it with a debug renderer while iterating."));
	}

	if (EventHandlerCount > 0 && !bLocalSpace)
	{
		AddFinding(
			TEXT("info"),
			TEXT("World-space event emitter"),
			TEXT("Event-driven emitters in world space can be correct, but they are more likely to drift away from expected attachments."),
			TEXT("Validate attachment behavior in runtime and switch to local space if the effect should stay locked to a source."));
	}

	const TArray<TSharedPtr<FJsonValue>>* Renderers = nullptr;
	if (Details->TryGetArrayField(TEXT("renderers"), Renderers) && Renderers != nullptr)
	{
		int32 RendererWithoutMaterialCount = 0;
		for (const TSharedPtr<FJsonValue>& RendererValue : *Renderers)
		{
			const TSharedPtr<FJsonObject>* RendererObj = nullptr;
			if (!RendererValue.IsValid() || !RendererValue->TryGetObject(RendererObj) || RendererObj == nullptr || !RendererObj->IsValid())
			{
				continue;
			}

			const TArray<TSharedPtr<FJsonValue>>* UsedMaterials = nullptr;
			if (!(*RendererObj)->TryGetArrayField(TEXT("used_materials"), UsedMaterials) || UsedMaterials == nullptr || UsedMaterials->Num() == 0)
			{
				++RendererWithoutMaterialCount;
			}
		}

		if (RendererWithoutMaterialCount > 0)
		{
			AddFinding(
				TEXT("info"),
				TEXT("Renderer material bindings worth checking"),
				FString::Printf(TEXT("%d renderer(s) reported no explicit used materials."), RendererWithoutMaterialCount),
				TEXT("Verify renderer defaults and material assignments in the Niagara editor, especially for sprite and mesh renderers."));
		}
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious emitter issues"),
			TEXT("No high-signal emitter-level issues were detected by the current automated rules."),
			TEXT("Validate runtime spawn timing, attachment stability, and material response in the target gameplay scene."));
	}

	TArray<TSharedPtr<FJsonValue>> Recommendations;
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("Check this emitter in runtime using the PIE probe tools if the effect is timing-sensitive."))));
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("If this emitter drives a hand-attached effect, validate local/world space and fixed bounds in the gameplay camera."))));
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("Use get_niagara_emitter_details first when you need raw renderer/material/bounds data for manual inspection."))));

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_niagara_emitter"));
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("emitter_name"), EmitterName);
	Result->SetStringField(TEXT("sim_target"), SimTarget);
	Result->SetStringField(TEXT("calculate_bounds_mode"), BoundsMode);
	Result->SetBoolField(TEXT("enabled"), bEmitterEnabled);
	Result->SetBoolField(TEXT("local_space"), bLocalSpace);
	Result->SetNumberField(TEXT("renderer_count"), RendererCount);
	Result->SetNumberField(TEXT("event_handler_count"), EventHandlerCount);
	Result->SetNumberField(TEXT("simulation_stage_count"), SimulationStageCount);
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetArrayField(TEXT("recommendations"), Recommendations);

	const TArray<TSharedPtr<FJsonValue>>* SummaryLines = nullptr;
	if (Details->TryGetArrayField(TEXT("summary_lines"), SummaryLines) && SummaryLines != nullptr)
	{
		Result->SetArrayField(TEXT("summary_lines"), *SummaryLines);
	}
	Result->SetObjectField(TEXT("details"), Details);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewNiagaraRenderer(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString RendererDetailsJson;
	if (!Tool_GetNiagaraRendererDetails(Input, RendererDetailsJson))
	{
		OutResult = RendererDetailsJson;
		return false;
	}

	TSharedPtr<FJsonObject> Details;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RendererDetailsJson);
	if (!FJsonSerializer::Deserialize(Reader, Details) || !Details.IsValid())
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_RENDERER_REVIEW_PARSE_FAILED"),
			TEXT("Failed to parse renderer details prior to review."));
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

	const FString SystemPath = Details->GetStringField(TEXT("system_path"));
	const FString EmitterName = Details->GetStringField(TEXT("emitter_name"));
	const FString RendererName = Details->GetStringField(TEXT("renderer_name"));
	const FString RendererType = Details->GetStringField(TEXT("renderer_type"));
	const FString SimTarget = Details->GetStringField(TEXT("sim_target"));
	const bool bEnabled = Details->GetBoolField(TEXT("enabled"));
	const bool bSupportsSimTarget = Details->GetBoolField(TEXT("supports_emitter_sim_target"));
	const bool bNeedsMids = Details->GetBoolField(TEXT("needs_mids_for_materials"));
	const int32 UsedMaterialCount = static_cast<int32>(Details->GetNumberField(TEXT("used_material_count")));
	TSet<FString> ExposedUserParams;
	TMap<FString, FString> ExposedUserParamTypes;
	TMap<FString, bool> ExposedUserParamObjectBacked;

	if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath)))
	{
		for (const FNiagaraVariableWithOffset& Parameter : NiagaraSystem->GetExposedParameters().ReadParameterVariables())
		{
			const FString ParamName = Parameter.GetName().ToString();
			const FString NormalizedParamName = NormalizeNiagaraUserParameterName(ParamName);
			ExposedUserParams.Add(NormalizedParamName);
			ExposedUserParamTypes.Add(NormalizedParamName, Parameter.GetType().GetName());
			ExposedUserParamObjectBacked.Add(NormalizedParamName, Parameter.GetType().GetClass() != nullptr);
		}
	}

	auto AddMissingUserParamFinding = [&](const FString& BindingName, const FString& BindingContext)
	{
		if (BindingName.IsEmpty())
		{
			return;
		}

		const FString NormalizedBinding = NormalizeNiagaraUserParameterName(BindingName);
		if (!ExposedUserParams.Contains(NormalizedBinding))
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Renderer user param binding has no matching system param"),
				FString::Printf(
				TEXT("%s references '%s', but the Niagara System does not expose that user parameter."),
					*BindingContext,
					*NormalizedBinding),
				TEXT("Add the missing Niagara user parameter on the system, or change the renderer binding to match an existing exposed param."));
			return;
		}

		const bool* bObjectBacked = ExposedUserParamObjectBacked.Find(NormalizedBinding);
		const FString* TypeName = ExposedUserParamTypes.Find(NormalizedBinding);
		if (bObjectBacked != nullptr && !*bObjectBacked)
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Renderer user param binding targets non-object param"),
				FString::Printf(
					TEXT("%s references '%s', but that exposed user parameter is typed as '%s' rather than an object-backed material-compatible type."),
					*BindingContext,
					*NormalizedBinding,
					TypeName ? **TypeName : TEXT("unknown")),
				TEXT("Bind the renderer to a MaterialInterface/UObject-compatible Niagara user parameter instead of a scalar/vector-style param."));
		}
	};

	if (!bEnabled)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Renderer disabled"),
			TEXT("The renderer is disabled, so this renderer path contributes no visible output."),
			TEXT("Enable the renderer if it is intended to be part of the live effect, or remove it if it is obsolete."));	
	}

	if (!bSupportsSimTarget)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Renderer incompatible with sim target"),
			FString::Printf(TEXT("Renderer type '%s' does not support emitter sim target '%s'."), *RendererType, *SimTarget),
			TEXT("Change the renderer type or switch emitter sim target so the renderer can draw correctly."));
	}

	if ((RendererType.Equals(TEXT("sprite"), ESearchCase::IgnoreCase) || RendererType.Equals(TEXT("ribbon"), ESearchCase::IgnoreCase)) &&
		UsedMaterialCount == 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("No explicit material binding"),
			TEXT("Sprite/ribbon renderer reported no explicit used materials."),
			TEXT("Check the renderer material assignment and user material binding in the Niagara editor."));
	}

	if (RendererType.Equals(TEXT("sprite"), ESearchCase::IgnoreCase) || RendererType.Equals(TEXT("ribbon"), ESearchCase::IgnoreCase))
	{
		const FString MaterialPath = Details->GetStringField(TEXT("material_path"));
		const FString MaterialUserParam = Details->GetStringField(TEXT("material_user_param"));

		if (MaterialPath.IsEmpty() && MaterialUserParam.IsEmpty())
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Renderer has no material source"),
				TEXT("Sprite/ribbon renderer has neither an explicit material nor a material user-parameter binding."),
				TEXT("Assign a material directly or bind the renderer to a valid Niagara user material parameter."));
		}
		else if (!MaterialPath.IsEmpty() && !MaterialUserParam.IsEmpty())
		{
			AddFinding(
				TEXT("info"),
				TEXT("Renderer has both explicit material and user param binding"),
				FString::Printf(
					TEXT("Sprite/ribbon renderer references explicit material '%s' and user param '%s'. This can make the active material source less obvious during debugging."),
					*MaterialPath,
					*NormalizeNiagaraUserParameterName(MaterialUserParam)),
				TEXT("Document which source is intended to drive the material at runtime, or simplify the renderer to one clear material source if possible."));
		}

		AddMissingUserParamFinding(MaterialUserParam, TEXT("Renderer material user param binding"));
	}

	if (RendererType.Equals(TEXT("mesh"), ESearchCase::IgnoreCase))
	{
		const int32 MeshCount = static_cast<int32>(Details->GetNumberField(TEXT("mesh_count")));
		const bool bOverrideMaterialsEnabled = Details->GetBoolField(TEXT("override_materials_enabled"));
		const int32 OverrideMaterialCount = static_cast<int32>(Details->GetNumberField(TEXT("override_material_count")));

		if (MeshCount == 0)
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Mesh renderer has no meshes"),
				TEXT("Mesh renderer is present but does not reference any meshes."),
				TEXT("Assign at least one mesh or remove the renderer if it is unused."));
		}

		if (bOverrideMaterialsEnabled && OverrideMaterialCount == 0)
		{
			AddFinding(
				TEXT("info"),
				TEXT("Override materials enabled with no overrides"),
				TEXT("Mesh renderer has override materials enabled but no override entries."),
				TEXT("Add override materials or disable the override path to reduce confusion."));
		}
		else if (!bOverrideMaterialsEnabled && OverrideMaterialCount > 0)
		{
			AddFinding(
				TEXT("info"),
				TEXT("Mesh override materials configured but disabled"),
				TEXT("Mesh renderer has override material entries configured, but override materials are disabled."),
				TEXT("Either enable override materials so the slots are used, or clear the stale override data to reduce ambiguity."));
		}

		const TArray<TSharedPtr<FJsonValue>>* OverrideMaterials = nullptr;
		if (Details->TryGetArrayField(TEXT("override_materials"), OverrideMaterials) && OverrideMaterials != nullptr)
		{
			for (const TSharedPtr<FJsonValue>& OverrideValue : *OverrideMaterials)
			{
				if (!OverrideValue.IsValid() || OverrideValue->Type != EJson::Object)
				{
					continue;
				}

				const TSharedPtr<FJsonObject> OverrideObject = OverrideValue->AsObject();
				if (!OverrideObject.IsValid())
				{
					continue;
				}

				const int32 OverrideIndex = static_cast<int32>(OverrideObject->GetNumberField(TEXT("index")));
				const FString ExplicitMaterialPath = OverrideObject->GetStringField(TEXT("explicit_material_path"));
				const FString UserParamBinding = OverrideObject->GetStringField(TEXT("user_param_binding"));

				if (bOverrideMaterialsEnabled && ExplicitMaterialPath.IsEmpty() && UserParamBinding.IsEmpty())
				{
					AddFinding(
						TEXT("info"),
						TEXT("Empty mesh override slot"),
						FString::Printf(TEXT("Mesh override slot %d is enabled but has neither an explicit material nor a user-param binding."), OverrideIndex),
						TEXT("Remove the empty slot, assign an explicit material, or bind it to a valid Niagara user material parameter."));
				}
				else if (!ExplicitMaterialPath.IsEmpty() && !UserParamBinding.IsEmpty())
				{
					AddFinding(
						TEXT("info"),
						TEXT("Mesh override slot has both explicit material and user param binding"),
						FString::Printf(
							TEXT("Mesh override slot %d references explicit material '%s' and user param '%s'."),
							OverrideIndex,
							*ExplicitMaterialPath,
							*NormalizeNiagaraUserParameterName(UserParamBinding)),
						TEXT("Prefer one clearly intended material source per override slot unless the dual setup is deliberate and documented."));
				}

				AddMissingUserParamFinding(
					UserParamBinding,
					FString::Printf(TEXT("Mesh override slot %d user param binding"), OverrideIndex));
			}
		}
	}

	const int32 ScalarParameterCount = static_cast<int32>(Details->GetNumberField(TEXT("material_scalar_parameter_count")));
	const int32 VectorParameterCount = static_cast<int32>(Details->GetNumberField(TEXT("material_vector_parameter_count")));
	const int32 TextureParameterCount = static_cast<int32>(Details->GetNumberField(TEXT("material_texture_parameter_count")));
	const int32 AttributeBindingCount = static_cast<int32>(Details->GetNumberField(TEXT("material_attribute_binding_count")));
	if (bNeedsMids && (ScalarParameterCount + VectorParameterCount + TextureParameterCount + AttributeBindingCount) == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("MID-capable renderer with no parameter bindings"),
			TEXT("Renderer reports it needs MIDs for materials but currently shows no material parameter bindings."),
			TEXT("Validate whether renderer material overrides are actually driven at runtime, or simplify if not needed."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious renderer issues"),
			TEXT("No high-signal renderer-level issues were detected by the current automated rules."),
			TEXT("Validate renderer appearance in the gameplay camera and inspect material-facing bindings if the visual still looks off."));
	}

	TArray<TSharedPtr<FJsonValue>> Recommendations;
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("Use get_niagara_renderer_details when you need raw renderer/material information for manual debugging."))));
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("If this renderer drives a gameplay-critical visual, compare it against runtime captures from the PIE probe tools."))));
	Recommendations.Add(MakeShareable(new FJsonValueString(TEXT("When renderer/material issues are suspected, inspect material bindings before changing emitter timing."))));

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_niagara_renderer"));
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("emitter_name"), EmitterName);
	Result->SetStringField(TEXT("renderer_name"), RendererName);
	Result->SetStringField(TEXT("renderer_type"), RendererType);
	Result->SetStringField(TEXT("sim_target"), SimTarget);
	Result->SetBoolField(TEXT("enabled"), bEnabled);
	Result->SetBoolField(TEXT("supports_emitter_sim_target"), bSupportsSimTarget);
	Result->SetNumberField(TEXT("used_material_count"), UsedMaterialCount);
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetArrayField(TEXT("recommendations"), Recommendations);
	Result->SetObjectField(TEXT("details"), Details);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RunV2NiagaraOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	return Tool_RunSuctionCoreNiagaraOp(Input, OutResult);
}

bool FVFXToolRegistry::Tool_RunSuctionCoreNiagaraOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_OP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString OpName;
	FString SystemPath;
	if (!Input->TryGetStringField(TEXT("op_name"), OpName) || OpName.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_OP_NAME_MISSING"), TEXT("Missing required field: op_name"));
	}
	if (!Input->TryGetStringField(TEXT("system_path"), SystemPath) || SystemPath.IsEmpty())
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_SYSTEM_PATH_MISSING"), TEXT("Missing required field: system_path"));
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bHardRebuildGateActive = ParseOptionalBoolField(Input, TEXT("hard_rebuild_gate_active"), false);
	FString Archetype = TEXT("suction_core_energy");
	Input->TryGetStringField(TEXT("archetype"), Archetype);
	FV2NiagaraArchetypeConfig ArchetypeConfig;
	if (!GetV2NiagaraArchetypeConfig(Archetype, ArchetypeConfig))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_V2_ARCHETYPE_UNSUPPORTED"),
			FString::Printf(TEXT("Unsupported V2 archetype '%s'."), *Archetype));
	}
	if (OpName.Equals(TEXT("rebuild_hero_fire_sweep_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("rebuild_hero_arcane_sweep_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("rebuild_hero_electric_arc_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("rebuild_hero_impact_burst_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("rebuild_hero_explosion_burst_layer"), ESearchCase::IgnoreCase))
	{
		OpName = TEXT("rebuild_main_attraction_layer");
	}
	int32 ConsecutiveMotionFailures = 0;
	TryReadOptionalIntField(Input, TEXT("consecutive_layer_a_motion_failures"), ConsecutiveMotionFailures);

	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	if (!NiagaraSystem)
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SUCTION_SYSTEM_NOT_FOUND"),
			FString::Printf(TEXT("Niagara System not found at '%s'."), *SystemPath));
	}

	const bool bLayerAScalarOnlyRetune = OpName.Equals(TEXT("retune_motion_family"), ESearchCase::IgnoreCase);
	if (bLayerAScalarOnlyRetune && (bHardRebuildGateActive || ConsecutiveMotionFailures >= 2))
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_LAYER_A_REBUILD_REQUIRED"),
			TEXT("Layer A motion-family failures have already crossed the rebuild gate. Scalar-only polish is blocked; schedule rebuild_main_attraction_layer instead."));
	}

	auto ExecuteToolJson = [this](const FString& ToolName, const TSharedPtr<FJsonObject>& ToolInput, TSharedPtr<FJsonObject>& OutObject, FString& OutError) -> bool
	{
		FString ToolResult;
		if (!ExecuteTool(ToolName, ToolInput, ToolResult))
		{
			OutError = ToolResult;
			return false;
		}
		if (!ParseToolJsonResult(ToolResult, OutObject))
		{
			OutError = FString::Printf(TEXT("Tool '%s' returned non-JSON output."), *ToolName);
			return false;
		}
		return true;
	};

	auto EnsureEmitter = [&](const FString& EmitterName, const bool bRibbon, TArray<TSharedPtr<FJsonValue>>& MutationLog, FString& OutError) -> bool
	{
		if (SystemHasEmitterNamed(NiagaraSystem, EmitterName))
		{
			TSharedPtr<FJsonObject> Info = MakeShareable(new FJsonObject());
			Info->SetStringField(TEXT("tool_name"), TEXT("add_empty_niagara_emitter"));
			Info->SetStringField(TEXT("message"), TEXT("Emitter already exists; reusing existing emitter."));
			Info->SetStringField(TEXT("emitter_name"), EmitterName);
			MutationLog.Add(MakeShared<FJsonValueObject>(Info));
			return true;
		}

		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetStringField(TEXT("emitter_name"), EmitterName);
		ToolInput->SetStringField(TEXT("simulation_target"), TEXT("cpu"));
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
		if (bRibbon)
		{
			ToolInput->SetBoolField(TEXT("add_ribbon_renderer"), true);
		}
		else
		{
			ToolInput->SetBoolField(TEXT("add_sprite_renderer"), true);
		}

		TSharedPtr<FJsonObject> ToolResult;
		if (!ExecuteToolJson(TEXT("add_empty_niagara_emitter"), ToolInput, ToolResult, OutError))
		{
			return false;
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(ToolResult));
		return true;
	};

	auto AddNativeModule = [&](const FString& EmitterName, const FString& StageUsage, const FString& ModuleDisplayName, TArray<TSharedPtr<FJsonValue>>& MutationLog, FString& OutError) -> bool
	{
		FString ModulePath;
		if (!ResolveNiagaraModuleScriptPathByName(ModuleDisplayName, ModulePath, OutError))
		{
			return false;
		}

		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetStringField(TEXT("emitter_name"), EmitterName);
		ToolInput->SetStringField(TEXT("stage_usage"), StageUsage);
		ToolInput->SetStringField(TEXT("module_script_path"), ModulePath);
		ToolInput->SetStringField(TEXT("suggested_name"), ModuleDisplayName);
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);

		TSharedPtr<FJsonObject> ToolResult;
		if (!ExecuteToolJson(TEXT("add_niagara_module_to_stage"), ToolInput, ToolResult, OutError))
		{
			return false;
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(ToolResult));
		return true;
	};

	auto ApplyScalarEdit = [&](const FString& ToolName, const FString& EmitterName, const TSharedPtr<FJsonValue>& ValueField, TArray<TSharedPtr<FJsonValue>>& MutationLog, FString& OutError) -> bool
	{
		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetStringField(TEXT("emitter_name"), EmitterName);
		ToolInput->SetField(TEXT("value"), ValueField);
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);

		TSharedPtr<FJsonObject> ToolResult;
		if (!ExecuteToolJson(ToolName, ToolInput, ToolResult, OutError))
		{
			return false;
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(ToolResult));
		return true;
	};

	auto ApplyRangeEdit = [&](const FString& ToolName, const FString& EmitterName, const double MinValue, const double MaxValue, TArray<TSharedPtr<FJsonValue>>& MutationLog, FString& OutError) -> bool
	{
		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetStringField(TEXT("emitter_name"), EmitterName);
		ToolInput->SetNumberField(TEXT("min_value"), MinValue);
		ToolInput->SetNumberField(TEXT("max_value"), MaxValue);
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);

		TSharedPtr<FJsonObject> ToolResult;
		if (!ExecuteToolJson(ToolName, ToolInput, ToolResult, OutError))
		{
			return false;
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(ToolResult));
		return true;
	};

	auto BindMaterialIfPresent = [&](const FString& EmitterName, TArray<TSharedPtr<FJsonValue>>& MutationLog, FString& OutError) -> bool
	{
		FString MaterialPath;
		Input->TryGetStringField(TEXT("material_path"), MaterialPath);
		if (MaterialPath.IsEmpty())
		{
			return true;
		}

		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetStringField(TEXT("emitter_name"), EmitterName);
		ToolInput->SetStringField(TEXT("material_path"), MaterialPath);
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);

		TSharedPtr<FJsonObject> ToolResult;
		if (!ExecuteToolJson(TEXT("set_niagara_renderer_material"), ToolInput, ToolResult, OutError))
		{
			return false;
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(ToolResult));
		return true;
	};

	auto CompileSystem = [&](TSharedPtr<FJsonObject>& OutCompile, FString& OutError) -> bool
	{
		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		ToolInput->SetStringField(TEXT("system_path"), SystemPath);
		ToolInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
		return ExecuteToolJson(TEXT("compile_niagara_system"), ToolInput, OutCompile, OutError);
	};

	auto ResolveVisualContractString = [&](const FString& RawValue) -> FString
	{
		FString Resolved = RawValue;
		FString MaterialPath;
		Input->TryGetStringField(TEXT("material_path"), MaterialPath);
		Resolved.ReplaceInline(TEXT("{{material_path}}"), *MaterialPath);
		Resolved.ReplaceInline(TEXT("{{system_path}}"), *SystemPath);
		Resolved.ReplaceInline(TEXT("{{op_name}}"), *OpName);
		Resolved.ReplaceInline(TEXT("{{archetype}}"), *Archetype);
		return Resolved;
	};

	auto SerializeVisualContractValue = [&](const TSharedPtr<FJsonValue>& Value, FString& OutValue, FString& OutError) -> bool
	{
		OutValue.Reset();
		OutError.Reset();
		if (!Value.IsValid())
		{
			OutError = TEXT("Visual-contract binding is missing a value field.");
			return false;
		}

		switch (Value->Type)
		{
		case EJson::String:
			OutValue = ResolveVisualContractString(Value->AsString());
			return true;
		case EJson::Number:
			OutValue = FString::SanitizeFloat(Value->AsNumber());
			return true;
		case EJson::Boolean:
			OutValue = Value->AsBool() ? TEXT("true") : TEXT("false");
			return true;
		case EJson::Array:
		{
			const TArray<TSharedPtr<FJsonValue>>& ArrayValues = Value->AsArray();
			TArray<FString> Parts;
			for (const TSharedPtr<FJsonValue>& Entry : ArrayValues)
			{
				if (!Entry.IsValid())
				{
					continue;
				}
				if (Entry->Type == EJson::String)
				{
					Parts.Add(ResolveVisualContractString(Entry->AsString()));
				}
				else if (Entry->Type == EJson::Number)
				{
					Parts.Add(FString::SanitizeFloat(Entry->AsNumber()));
				}
				else if (Entry->Type == EJson::Boolean)
				{
					Parts.Add(Entry->AsBool() ? TEXT("true") : TEXT("false"));
				}
				else
				{
					OutError = TEXT("Visual-contract arrays currently support only string, number, or bool values.");
					return false;
				}
			}
			OutValue = FString::Printf(TEXT("[%s]"), *FString::Join(Parts, TEXT(",")));
			return true;
		}
		default:
			OutError = TEXT("Visual-contract binding uses an unsupported JSON value type.");
			return false;
		}
	};

	auto ResolveOpVisualContract = [&](TSharedPtr<FJsonObject>& OutContract, bool& OutAllowLegacyStackFallback, FString& OutStrategy, FString& OutError) -> bool
	{
		OutContract.Reset();
		OutAllowLegacyStackFallback = true;
		OutStrategy = TEXT("legacy_stack_authoring");
		OutError.Reset();

		const TSharedPtr<FJsonObject>* InlineContractPtr = nullptr;
		if (Input->TryGetObjectField(TEXT("visual_contract"), InlineContractPtr) && InlineContractPtr && (*InlineContractPtr).IsValid())
		{
			OutContract = MakeShared<FJsonObject>(**InlineContractPtr);
		}
		else
		{
			TSharedPtr<FJsonObject> LoadedContract;
			if (!FVFXKnowledgeLoader::LoadV2VisualContract(Archetype, LoadedContract, OutError))
			{
				return false;
			}

			const TSharedPtr<FJsonObject>* OpBindingsPtr = nullptr;
			if (LoadedContract.IsValid() &&
				LoadedContract->TryGetObjectField(TEXT("op_bindings"), OpBindingsPtr) &&
				OpBindingsPtr &&
				(*OpBindingsPtr).IsValid())
			{
				const TSharedPtr<FJsonObject>* BindingPtr = nullptr;
				if ((*OpBindingsPtr)->TryGetObjectField(OpName, BindingPtr) && BindingPtr && (*BindingPtr).IsValid())
				{
					OutContract = MakeShared<FJsonObject>(**BindingPtr);
					if (!OutContract->HasField(TEXT("strategy")) && LoadedContract->HasField(TEXT("strategy")))
					{
						OutContract->SetStringField(TEXT("strategy"), LoadedContract->GetStringField(TEXT("strategy")));
					}
					if (!OutContract->HasField(TEXT("allow_legacy_stack_fallback")))
					{
						bool bLoadedAllowLegacy = true;
						if (LoadedContract->TryGetBoolField(TEXT("allow_legacy_stack_fallback"), bLoadedAllowLegacy))
						{
							OutContract->SetBoolField(TEXT("allow_legacy_stack_fallback"), bLoadedAllowLegacy);
						}
					}
					if (!OutContract->HasField(TEXT("auto_create_missing_parameters")))
					{
						bool bAutoCreateMissingParameters = false;
						if (LoadedContract->TryGetBoolField(TEXT("auto_create_missing_parameters"), bAutoCreateMissingParameters))
						{
							OutContract->SetBoolField(TEXT("auto_create_missing_parameters"), bAutoCreateMissingParameters);
						}
					}
				}
			}
		}

		if (!OutContract.IsValid())
		{
			return true;
		}

		OutContract->TryGetStringField(TEXT("strategy"), OutStrategy);
		OutContract->TryGetBoolField(TEXT("allow_legacy_stack_fallback"), OutAllowLegacyStackFallback);
		return true;
	};

	auto FindExistingUserParam = [&](const FString& ParamName, FNiagaraVariableWithOffset& OutParam) -> bool
	{
		const FString FinalParamName = NormalizeNiagaraUserParameterName(ParamName);
		for (const FNiagaraVariableWithOffset& ExistingVar : NiagaraSystem->GetExposedParameters().ReadParameterVariables())
		{
			if (ExistingVar.GetName().ToString().Equals(FinalParamName, ESearchCase::IgnoreCase))
			{
				OutParam = ExistingVar;
				return true;
			}
		}
		return false;
	};

	auto CountExistingVisualContractBindings = [&](const TSharedPtr<FJsonObject>& Contract) -> int32
	{
		if (!Contract.IsValid())
		{
			return 0;
		}

		const TArray<TSharedPtr<FJsonValue>>* Bindings = nullptr;
		if (!Contract->TryGetArrayField(TEXT("bindings"), Bindings) || !Bindings)
		{
			return 0;
		}

		int32 MatchCount = 0;
		for (const TSharedPtr<FJsonValue>& BindingValue : *Bindings)
		{
			const TSharedPtr<FJsonObject> BindingObject = BindingValue.IsValid() ? BindingValue->AsObject() : nullptr;
			FString ParamName;
			if (!BindingObject.IsValid() || !BindingObject->TryGetStringField(TEXT("param_name"), ParamName) || ParamName.IsEmpty())
			{
				continue;
			}

			FNiagaraVariableWithOffset ExistingParam;
			if (FindExistingUserParam(ParamName, ExistingParam))
			{
				++MatchCount;
			}
		}
		return MatchCount;
	};

	auto ContractCanAutoCreateBindings = [&](const TSharedPtr<FJsonObject>& Contract) -> bool
	{
		if (!Contract.IsValid())
		{
			return false;
		}

		bool bAutoCreateMissingParameters = false;
		if (Contract->TryGetBoolField(TEXT("auto_create_missing_parameters"), bAutoCreateMissingParameters) &&
			bAutoCreateMissingParameters)
		{
			return true;
		}

		const TArray<TSharedPtr<FJsonValue>>* Bindings = nullptr;
		if (!Contract->TryGetArrayField(TEXT("bindings"), Bindings) || !Bindings)
		{
			return false;
		}

		for (const TSharedPtr<FJsonValue>& BindingValue : *Bindings)
		{
			const TSharedPtr<FJsonObject> BindingObject = BindingValue.IsValid() ? BindingValue->AsObject() : nullptr;
			if (!BindingObject.IsValid())
			{
				continue;
			}

			bool bCreateIfMissing = false;
			if (BindingObject->TryGetBoolField(TEXT("create_if_missing"), bCreateIfMissing) && bCreateIfMissing)
			{
				return true;
			}
		}

		return false;
	};

	bool bCreatedDirectUserParamSnapshot = false;
	FString DirectUserParamSnapshotPath;
	auto ApplyVisualContractBindings = [&](const TSharedPtr<FJsonObject>& Contract, TArray<TSharedPtr<FJsonValue>>& MutationLog, TArray<TSharedPtr<FJsonValue>>& AppliedBindings, FString& OutError) -> bool
	{
		if (!Contract.IsValid())
		{
			return true;
		}

		const TArray<TSharedPtr<FJsonValue>>* Bindings = nullptr;
		if (!Contract->TryGetArrayField(TEXT("bindings"), Bindings) || !Bindings)
		{
			return true;
		}

		bool bAutoCreateMissingParameters = false;
		Contract->TryGetBoolField(TEXT("auto_create_missing_parameters"), bAutoCreateMissingParameters);

		for (const TSharedPtr<FJsonValue>& BindingValue : *Bindings)
		{
			const TSharedPtr<FJsonObject> BindingObject = BindingValue.IsValid() ? BindingValue->AsObject() : nullptr;
			if (!BindingObject.IsValid())
			{
				continue;
			}

			FString ParamName;
			FString ParamType;
			if (!BindingObject->TryGetStringField(TEXT("param_name"), ParamName) || ParamName.IsEmpty() ||
				!BindingObject->TryGetStringField(TEXT("param_type"), ParamType) || ParamType.IsEmpty())
			{
				OutError = TEXT("Visual-contract binding requires both param_name and param_type.");
				return false;
			}

			const FString FinalParamName = NormalizeNiagaraUserParameterName(ParamName);
			const TSharedPtr<FJsonValue> RawValue = BindingObject->TryGetField(TEXT("value"));
			FString SerializedValue;
			if (!SerializeVisualContractValue(RawValue, SerializedValue, OutError))
			{
				return false;
			}

			bool bSkipIfEmpty = false;
			BindingObject->TryGetBoolField(TEXT("skip_if_empty"), bSkipIfEmpty);
			if (bSkipIfEmpty && SerializedValue.IsEmpty())
			{
				continue;
			}

			bool bCreateIfMissing = bAutoCreateMissingParameters;
			BindingObject->TryGetBoolField(TEXT("create_if_missing"), bCreateIfMissing);

			FNiagaraTypeDefinition RequestedTypeDef;
			if (!ResolveNiagaraTypeDefinition(ParamType, RequestedTypeDef))
			{
				OutError = FString::Printf(TEXT("Unsupported visual-contract param_type '%s' for '%s'."), *ParamType, *FinalParamName);
				return false;
			}

			FNiagaraVariableWithOffset ExistingParam;
			const bool bParamExists = FindExistingUserParam(FinalParamName, ExistingParam);
			if (!bParamExists)
			{
				if (!bCreateIfMissing)
				{
					continue;
				}

				if (!bCreatedDirectUserParamSnapshot && bApplyChanges)
				{
					FString SnapshotError;
					if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("run_v2_niagara_op_user_params"), DirectUserParamSnapshotPath, SnapshotError))
					{
						OutError = SnapshotError;
						return false;
					}
					bCreatedDirectUserParamSnapshot = true;
				}

				FNiagaraVariable NewVar(RequestedTypeDef, FName(*FinalParamName));
				NiagaraSystem->GetExposedParameters().AddParameter(NewVar);

				TSharedPtr<FJsonObject> AddedObj = MakeShareable(new FJsonObject());
				AddedObj->SetStringField(TEXT("tool_name"), TEXT("add_niagara_user_parameter_direct"));
				AddedObj->SetStringField(TEXT("param_name"), FinalParamName);
				AddedObj->SetStringField(TEXT("param_type"), ParamType);
				AddedObj->SetStringField(TEXT("message"), TEXT("Added missing Niagara user parameter from visual contract."));
				MutationLog.Add(MakeShared<FJsonValueObject>(AddedObj));

				if (!FindExistingUserParam(FinalParamName, ExistingParam))
				{
					OutError = FString::Printf(TEXT("Failed to resolve newly created visual-contract user parameter '%s'."), *FinalParamName);
					return false;
				}
			}
			else if (!ExistingParam.GetType().IsSameBaseDefinition(RequestedTypeDef))
			{
				OutError = FString::Printf(TEXT("Visual-contract parameter '%s' exists, but its type does not match requested type '%s'."), *FinalParamName, *ParamType);
				return false;
			}

			if (!bCreatedDirectUserParamSnapshot && bApplyChanges)
			{
				FString SnapshotError;
				if (!CreateNiagaraMutationSnapshot(NiagaraSystem, TEXT("run_v2_niagara_op_user_params"), DirectUserParamSnapshotPath, SnapshotError))
				{
					OutError = SnapshotError;
					return false;
				}
				bCreatedDirectUserParamSnapshot = true;
			}

			TSharedPtr<FJsonObject> BindingResult = MakeShareable(new FJsonObject());
			FString ParamErrorCode;
			if (!SetNiagaraParameterStoreValue(
				NiagaraSystem->GetExposedParameters(),
				FNiagaraVariable(ExistingParam.GetType(), ExistingParam.GetName()),
				ParamType,
				SerializedValue,
				BindingResult,
				ParamErrorCode,
				OutError))
			{
				return false;
			}

			NiagaraSystem->MarkPackageDirty();

			BindingResult->SetStringField(TEXT("tool_name"), TEXT("set_niagara_user_parameter_default_direct"));
			BindingResult->SetStringField(TEXT("param_name"), FinalParamName);
			BindingResult->SetStringField(TEXT("param_type"), ParamType);
			BindingResult->SetStringField(TEXT("value"), SerializedValue);
			MutationLog.Add(MakeShared<FJsonValueObject>(BindingResult));

			TSharedPtr<FJsonObject> AppliedObj = MakeShareable(new FJsonObject());
			AppliedObj->SetStringField(TEXT("param_name"), FinalParamName);
			AppliedObj->SetStringField(TEXT("param_type"), ParamType);
			AppliedObj->SetStringField(TEXT("value"), SerializedValue);
			AppliedBindings.Add(MakeShared<FJsonValueObject>(AppliedObj));
		}

		return true;
	};

	FString TargetLayer = ArchetypeConfig.HeroEmitter;
	FString RendererFamily = TEXT("sprite");
	FString MaterialFamily = ArchetypeConfig.HeroMaterialFamily;
	TArray<TSharedPtr<FJsonValue>> ForbiddenMutations;
	TArray<TSharedPtr<FJsonValue>> ExpectedReviewDeltas;
	TArray<TSharedPtr<FJsonValue>> PredictedResolvedFailureLabels;
	TArray<TSharedPtr<FJsonValue>> PredictedRemainingFailureLabels;
	TArray<TSharedPtr<FJsonValue>> MutationLog;
	TArray<TSharedPtr<FJsonValue>> AppliedUserParameterBindings;
	FString ErrorMessage;

	auto AddForbidden = [&ForbiddenMutations](const FString& Text)
	{
		ForbiddenMutations.Add(MakeShared<FJsonValueString>(Text));
	};
	auto AddReviewDelta = [&ExpectedReviewDeltas](const FString& Text)
	{
		ExpectedReviewDeltas.Add(MakeShared<FJsonValueString>(Text));
	};
	auto AddPredictedResolved = [&PredictedResolvedFailureLabels](const FString& Text)
	{
		PredictedResolvedFailureLabels.Add(MakeShared<FJsonValueString>(Text));
	};
	auto AddPredictedRemaining = [&PredictedRemainingFailureLabels](const FString& Text)
	{
		PredictedRemainingFailureLabels.Add(MakeShared<FJsonValueString>(Text));
	};
	auto MakeVectorValue = [](double X, double Y, double Z) -> TSharedPtr<FJsonValue>
	{
		TArray<TSharedPtr<FJsonValue>> VectorArray;
		VectorArray.Add(MakeShared<FJsonValueNumber>(X));
		VectorArray.Add(MakeShared<FJsonValueNumber>(Y));
		VectorArray.Add(MakeShared<FJsonValueNumber>(Z));
		return MakeShared<FJsonValueArray>(VectorArray);
	};

	if (OpName.Equals(TEXT("build_main_attraction_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("rebuild_main_attraction_layer"), ESearchCase::IgnoreCase) ||
		OpName.Equals(TEXT("retune_motion_family"), ESearchCase::IgnoreCase))
	{
		TargetLayer = ArchetypeConfig.HeroEmitter;
		RendererFamily = TEXT("sprite_or_stretched_sprite");
		MaterialFamily = ArchetypeConfig.HeroMaterialFamily;
		AddForbidden(TEXT("No ribbon-first Layer A authoring."));
		AddForbidden(TEXT("No custom curve-expression authoring."));
		AddReviewDelta(
			Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase)
				? TEXT("Outward blast read should improve.")
				: TEXT("Outside-to-core read should improve."));
		AddReviewDelta(
			Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase)
				? TEXT("Plume-like or ring-shell motion should reduce.")
				: TEXT("Orbit-dominant motion should reduce."));
		AddPredictedResolved(TEXT("hero_sweep_missing"));
		AddPredictedResolved(TEXT("no_outside_to_core_read"));
		AddPredictedRemaining(TEXT("requires_visual_review"));
	}
	else if (OpName.Equals(TEXT("build_secondary_filament_layer"), ESearchCase::IgnoreCase))
	{
		TargetLayer = ArchetypeConfig.SecondaryEmitter;
		RendererFamily = TEXT("sprite_or_stretched_sprite");
		MaterialFamily = ArchetypeConfig.HeroMaterialFamily;
		AddForbidden(TEXT("Do not use deterministic ribbon stream authoring for Layer B."));
		AddForbidden(TEXT("Do not build sphere-shell vortex support without inward pull."));
		AddForbidden(TEXT("No custom HLSL residue in SecondaryFilaments authoring."));
		AddForbidden(TEXT("Do not overtake hero silhouette dominance."));
		AddReviewDelta(TEXT("Support filaments should stay secondary."));
		AddReviewDelta(TEXT("Support layer should avoid closed cage silhouettes."));
		AddReviewDelta(TEXT("Support read should stay plasma/flame-like instead of ribbon linework."));
		AddPredictedResolved(TEXT("silhouette_too_closed"));
		AddPredictedResolved(TEXT("linework_not_flame_body"));
		AddPredictedRemaining(TEXT("requires_visual_review"));
	}
	else if (OpName.Equals(TEXT("build_core_turbulence_layer"), ESearchCase::IgnoreCase))
	{
		TargetLayer = ArchetypeConfig.CoreEmitter;
		RendererFamily = TEXT("sprite");
		MaterialFamily = ArchetypeConfig.CoreMaterialFamily;
		AddReviewDelta(TEXT("Inner hot core detail should improve."));
		AddPredictedResolved(TEXT("inner_hot_core_missing"));
		AddPredictedRemaining(TEXT("requires_visual_review"));
	}
	else if (OpName.Equals(TEXT("build_sparks_layer"), ESearchCase::IgnoreCase))
	{
		TargetLayer = ArchetypeConfig.SparksEmitter;
		RendererFamily = TEXT("sprite");
		MaterialFamily = ArchetypeConfig.HeroMaterialFamily;
		AddReviewDelta(TEXT("High-frequency support motion should improve."));
		AddPredictedResolved(TEXT("support_fill_missing"));
		AddPredictedRemaining(TEXT("requires_visual_review"));
	}
	else if (OpName.Equals(TEXT("build_shock_glow_layer"), ESearchCase::IgnoreCase))
	{
		TargetLayer = ArchetypeConfig.GlowEmitter;
		RendererFamily = TEXT("sprite");
		MaterialFamily = ArchetypeConfig.CoreMaterialFamily;
		AddReviewDelta(TEXT("Core bind glow should improve."));
		AddPredictedResolved(TEXT("support_fill_missing"));
		AddPredictedResolved(TEXT("inner_hot_core_missing"));
		AddPredictedRemaining(TEXT("requires_visual_review"));
	}
	else
	{
		return BuildNiagaraToolError(
			OutResult,
			TEXT("NI_SUCTION_OP_UNSUPPORTED"),
			FString::Printf(TEXT("Unsupported suction-core Niagara op '%s'."), *OpName));
	}

	TSharedPtr<FJsonObject> ActiveVisualContract;
	bool bAllowLegacyStackFallback = true;
	FString VisualContractStrategy;
	if (!ResolveOpVisualContract(ActiveVisualContract, bAllowLegacyStackFallback, VisualContractStrategy, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_V2_VISUAL_CONTRACT_LOAD_FAILED"), ErrorMessage);
	}
	const int32 ExistingVisualContractParamCount = CountExistingVisualContractBindings(ActiveVisualContract);
	const bool bContractCanAutoCreateBindings = ContractCanAutoCreateBindings(ActiveVisualContract);
	const bool bPrefersUserParameterTweak =
		ActiveVisualContract.IsValid() &&
		VisualContractStrategy.Contains(TEXT("user_parameter"), ESearchCase::IgnoreCase) &&
		(ExistingVisualContractParamCount > 0 || bContractCanAutoCreateBindings);
	FString AuthoringMode = bPrefersUserParameterTweak ? TEXT("user_parameter_tweak") : TEXT("legacy_stack_authoring");

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRun = MakeShareable(new FJsonObject());
		DryRun->SetBoolField(TEXT("success"), true);
		DryRun->SetBoolField(TEXT("dry_run"), true);
		DryRun->SetStringField(TEXT("action"),
			Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
				? TEXT("run_suction_core_niagara_op")
				: TEXT("run_v2_niagara_op"));
		DryRun->SetStringField(TEXT("archetype"), Archetype);
		DryRun->SetStringField(TEXT("op_name"), OpName);
		DryRun->SetStringField(TEXT("system_path"), SystemPath);
		DryRun->SetStringField(TEXT("target_layer"), TargetLayer);
		DryRun->SetStringField(TEXT("required_renderer_family"), RendererFamily);
		DryRun->SetStringField(TEXT("required_material_family"), MaterialFamily);
		DryRun->SetStringField(TEXT("authoring_mode_preference"), AuthoringMode);
		DryRun->SetNumberField(TEXT("matched_user_parameter_count"), ExistingVisualContractParamCount);
		DryRun->SetBoolField(TEXT("allow_legacy_stack_fallback"), bAllowLegacyStackFallback);
		DryRun->SetArrayField(TEXT("forbidden_mutations"), ForbiddenMutations);
		DryRun->SetArrayField(TEXT("expected_review_deltas"), ExpectedReviewDeltas);
		DryRun->SetArrayField(TEXT("mutated_assets"), {
			MakeShared<FJsonValueString>(SystemPath)
		});
		if (ActiveVisualContract.IsValid())
		{
			DryRun->SetObjectField(TEXT("visual_contract"), ActiveVisualContract);
		}
		DryRun->SetArrayField(TEXT("predicted_resolved_failure_labels"), PredictedResolvedFailureLabels);
		DryRun->SetArrayField(TEXT("predicted_remaining_failure_labels"), PredictedRemainingFailureLabels);
		OutResult = JsonObjToString(DryRun);
		return true;
	}

	bool bUseLegacyStackAuthoring = true;
	if (bPrefersUserParameterTweak)
	{
		if (!ApplyVisualContractBindings(ActiveVisualContract, MutationLog, AppliedUserParameterBindings, ErrorMessage))
		{
			if (!bAllowLegacyStackFallback)
			{
				return BuildNiagaraToolError(OutResult, TEXT("NI_V2_VISUAL_CONTRACT_APPLY_FAILED"), ErrorMessage);
			}
			AuthoringMode = TEXT("legacy_stack_authoring");
		}
		else if (AppliedUserParameterBindings.Num() > 0)
		{
			if (!SystemHasEmitterNamed(NiagaraSystem, TargetLayer))
			{
				if (!bAllowLegacyStackFallback)
				{
					return BuildNiagaraToolError(
						OutResult,
						TEXT("NI_V2_VISUAL_CONTRACT_EMITTER_MISSING"),
						FString::Printf(
							TEXT("Visual-contract user-parameter authoring requires target emitter '%s', and legacy fallback is disabled."),
							*TargetLayer));
				}

				AuthoringMode = TEXT("legacy_stack_authoring");
			}
			else
			{
				bUseLegacyStackAuthoring = false;
				AuthoringMode = TEXT("user_parameter_tweak");
			}
		}
		else if (!bAllowLegacyStackFallback)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_V2_VISUAL_CONTRACT_PARAM_NOT_FOUND"),
				TEXT("No exposed Niagara user parameters matched the visual contract, and legacy fallback is disabled."));
		}
		else
		{
			AuthoringMode = TEXT("legacy_stack_authoring");
		}
	}

	if (bUseLegacyStackAuthoring &&
		OpName.Equals(TEXT("rebuild_main_attraction_layer"), ESearchCase::IgnoreCase) &&
		SystemHasEmitterNamed(NiagaraSystem, TargetLayer))
	{
		TSharedPtr<FJsonObject> RemoveInput = MakeShareable(new FJsonObject());
		RemoveInput->SetStringField(TEXT("system_path"), SystemPath);
		RemoveInput->SetArrayField(TEXT("emitter_names"), { MakeShared<FJsonValueString>(TargetLayer) });
		RemoveInput->SetBoolField(TEXT("apply_changes"), bApplyChanges);
		TSharedPtr<FJsonObject> RemoveResult;
		if (!ExecuteToolJson(TEXT("remove_niagara_emitters_from_system"), RemoveInput, RemoveResult, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_LAYER_REBUILD_REMOVE_FAILED"), ErrorMessage);
		}
		MutationLog.Add(MakeShared<FJsonValueObject>(RemoveResult));
		NiagaraSystem = Cast<UNiagaraSystem>(UEditorAssetLibrary::LoadAsset(SystemPath));
	}

	if (bUseLegacyStackAuthoring && TargetLayer == ArchetypeConfig.HeroEmitter)
	{
		const double HeroSpawnRate =
			Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase) ? 34.0 :
			(Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ? 30.0 :
			(Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ? 38.0 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 48.0 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 56.0 : 42.0))));
		const double HeroLifeMin =
			Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase) ? 0.65 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 0.30 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.16 : 0.55));
		const double HeroLifeMax =
			Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ? 1.05 :
			(Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ? 0.80 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 0.58 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.36 : 0.95)));
		const double HeroDrag =
			Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase) ? 1.10 :
			(Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ? 1.55 :
			(Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ? 1.75 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 2.20 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.85 : 1.35))));
		const double HeroCurl =
			Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase) ? 0.26 :
			(Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase) ? 0.16 :
			(Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ? 0.10 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 0.08 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.12 : 0.22))));
		const bool bExplosionHero = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if (!EnsureEmitter(TargetLayer, false, MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_spawn_rate"), TargetLayer, MakeShared<FJsonValueNumber>(HeroSpawnRate), MutationLog, ErrorMessage) ||
			!ApplyRangeEdit(TEXT("set_niagara_lifetime"), TargetLayer, HeroLifeMin, HeroLifeMax, MutationLog, ErrorMessage) ||
			!AddNativeModule(TargetLayer, TEXT("particle_spawn"), TEXT("Sphere Location"), MutationLog, ErrorMessage) ||
			!(bExplosionHero
				? ApplyScalarEdit(TEXT("set_niagara_velocity"), TargetLayer, MakeVectorValue(0.0, 0.0, 760.0), MutationLog, ErrorMessage)
				: (AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Point Attraction Force"), MutationLog, ErrorMessage) &&
				   AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Vortex Force"), MutationLog, ErrorMessage))) ||
			!ApplyScalarEdit(TEXT("set_niagara_drag"), TargetLayer, MakeShared<FJsonValueNumber>(HeroDrag), MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_curl_noise_force"), TargetLayer, MakeShared<FJsonValueNumber>(HeroCurl), MutationLog, ErrorMessage) ||
			!BindMaterialIfPresent(TargetLayer, MutationLog, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_MAIN_LAYER_FAILED"), ErrorMessage);
		}
	}
	else if (bUseLegacyStackAuthoring && TargetLayer == ArchetypeConfig.SecondaryEmitter)
	{
		if (SystemHasEmitterNamed(NiagaraSystem, TargetLayer))
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SUCTION_SECONDARY_REBUILD_REQUIRES_FRESH_LAYER"),
				TEXT("SecondaryFilaments already exists. This semantic op now authors a clean sprite-based support layer and will not mutate an existing Layer B in place. Start from a fresh system or remove Layer B manually in a safe editor workflow first."));
		}

		const bool bExplosionSecondary = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if (!EnsureEmitter(TargetLayer, false, MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_spawn_rate"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionSecondary ? 22.0 : 16.0), MutationLog, ErrorMessage) ||
			!ApplyRangeEdit(TEXT("set_niagara_lifetime"), TargetLayer, bExplosionSecondary ? 0.18 : 0.35, bExplosionSecondary ? 0.48 : 0.7, MutationLog, ErrorMessage) ||
			!AddNativeModule(TargetLayer, TEXT("particle_spawn"), TEXT("Sphere Location"), MutationLog, ErrorMessage) ||
			!(bExplosionSecondary
				? ApplyScalarEdit(TEXT("set_niagara_velocity"), TargetLayer, MakeVectorValue(0.0, 0.0, 520.0), MutationLog, ErrorMessage)
				: (AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Point Attraction Force"), MutationLog, ErrorMessage) &&
				   AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Vortex Force"), MutationLog, ErrorMessage))) ||
			!ApplyScalarEdit(TEXT("set_niagara_drag"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionSecondary ? 0.95 : 1.45), MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_curl_noise_force"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionSecondary ? 0.16 : 0.18), MutationLog, ErrorMessage) ||
			!BindMaterialIfPresent(TargetLayer, MutationLog, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_SECONDARY_LAYER_FAILED"), ErrorMessage);
		}
	}
	else if (bUseLegacyStackAuthoring && TargetLayer == ArchetypeConfig.CoreEmitter)
	{
		const bool bExplosionCore = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if (!EnsureEmitter(TargetLayer, false, MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_spawn_rate"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionCore ? 12.0 : 16.0), MutationLog, ErrorMessage) ||
			!ApplyRangeEdit(TEXT("set_niagara_lifetime"), TargetLayer, bExplosionCore ? 0.16 : 0.45, bExplosionCore ? 0.34 : 0.9, MutationLog, ErrorMessage) ||
			!AddNativeModule(TargetLayer, TEXT("particle_spawn"), TEXT("Sphere Location"), MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_drag"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionCore ? 0.55 : 0.9), MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_curl_noise_force"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionCore ? 0.18 : 0.4), MutationLog, ErrorMessage) ||
			!BindMaterialIfPresent(TargetLayer, MutationLog, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_CORE_LAYER_FAILED"), ErrorMessage);
		}
	}
	else if (bUseLegacyStackAuthoring && TargetLayer == ArchetypeConfig.SparksEmitter)
	{
		const bool bExplosionSparks = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if (!EnsureEmitter(TargetLayer, false, MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_spawn_rate"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionSparks ? 160.0 : 120.0), MutationLog, ErrorMessage) ||
			!ApplyRangeEdit(TEXT("set_niagara_lifetime"), TargetLayer, bExplosionSparks ? 0.18 : 0.2, bExplosionSparks ? 0.55 : 0.5, MutationLog, ErrorMessage) ||
			!AddNativeModule(TargetLayer, TEXT("particle_spawn"), TEXT("Sphere Location"), MutationLog, ErrorMessage) ||
			!(bExplosionSparks
				? (ApplyScalarEdit(TEXT("set_niagara_velocity"), TargetLayer, MakeVectorValue(0.0, 0.0, 1100.0), MutationLog, ErrorMessage) &&
				   ApplyScalarEdit(TEXT("set_niagara_gravity_force"), TargetLayer, MakeVectorValue(0.0, 0.0, -980.0), MutationLog, ErrorMessage))
				: (AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Point Attraction Force"), MutationLog, ErrorMessage) &&
				   AddNativeModule(TargetLayer, TEXT("particle_update"), TEXT("Vortex Force"), MutationLog, ErrorMessage))) ||
			!ApplyScalarEdit(TEXT("set_niagara_drag"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionSparks ? 0.10 : 1.1), MutationLog, ErrorMessage) ||
			!BindMaterialIfPresent(TargetLayer, MutationLog, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_SPARKS_LAYER_FAILED"), ErrorMessage);
		}
	}
	else if (bUseLegacyStackAuthoring && TargetLayer == ArchetypeConfig.GlowEmitter)
	{
		const bool bExplosionGlow = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if (!EnsureEmitter(TargetLayer, false, MutationLog, ErrorMessage) ||
			!ApplyScalarEdit(TEXT("set_niagara_spawn_rate"), TargetLayer, MakeShared<FJsonValueNumber>(bExplosionGlow ? 2.0 : 4.0), MutationLog, ErrorMessage) ||
			!ApplyRangeEdit(TEXT("set_niagara_lifetime"), TargetLayer, bExplosionGlow ? 0.18 : 0.8, bExplosionGlow ? 0.36 : 1.4, MutationLog, ErrorMessage) ||
			!BindMaterialIfPresent(TargetLayer, MutationLog, ErrorMessage))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_SHOCK_GLOW_LAYER_FAILED"), ErrorMessage);
		}
	}

	TSharedPtr<FJsonObject> CompileResult;
	if (!CompileSystem(CompileResult, ErrorMessage))
	{
		return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_COMPILE_FAILED"), ErrorMessage);
	}

	struct FSemanticEmitterFacts
	{
		bool bEmitterFound = false;
		bool bHasEnabledSpriteRenderer = false;
		bool bHasEnabledRibbonRenderer = false;
		bool bHasSphereLocation = false;
		bool bHasAddVelocity = false;
		bool bHasPointAttraction = false;
		bool bHasConeVelocity = false;
		bool bHasCurlNoiseForce = false;
		bool bHasDrag = false;
		bool bHasGravityForce = false;
		FString PrimaryMaterialPath;
	};

	auto InspectEmitterFacts = [this, &ExecuteToolJson, &SystemPath, &ErrorMessage](const FString& EmitterName, FSemanticEmitterFacts& OutFacts) -> bool
	{
		OutFacts = FSemanticEmitterFacts();

		TSharedPtr<FJsonObject> EmitterReviewInput = MakeShareable(new FJsonObject());
		EmitterReviewInput->SetStringField(TEXT("system_path"), SystemPath);
		EmitterReviewInput->SetStringField(TEXT("emitter_name"), EmitterName);
		TSharedPtr<FJsonObject> EmitterReviewResult;
		if (ExecuteToolJson(TEXT("get_niagara_emitter_details"), EmitterReviewInput, EmitterReviewResult, ErrorMessage) &&
			EmitterReviewResult.IsValid())
		{
			OutFacts.bEmitterFound = EmitterReviewResult->GetBoolField(TEXT("success"));
			const TArray<TSharedPtr<FJsonValue>>* Renderers = nullptr;
			if (EmitterReviewResult->TryGetArrayField(TEXT("renderers"), Renderers) && Renderers)
			{
				for (const TSharedPtr<FJsonValue>& RendererValue : *Renderers)
				{
					const TSharedPtr<FJsonObject> RendererObject = RendererValue.IsValid() ? RendererValue->AsObject() : nullptr;
					if (!RendererObject.IsValid())
					{
						continue;
					}

					FString RendererClass;
					bool bRendererEnabled = false;
					RendererObject->TryGetBoolField(TEXT("enabled"), bRendererEnabled);
					if (!RendererObject->TryGetStringField(TEXT("renderer_class"), RendererClass) || !bRendererEnabled)
					{
						continue;
					}

					OutFacts.bHasEnabledSpriteRenderer |= RendererClass.Contains(TEXT("Sprite"));
					OutFacts.bHasEnabledRibbonRenderer |= RendererClass.Contains(TEXT("Ribbon"));
					if (OutFacts.PrimaryMaterialPath.IsEmpty())
					{
						const TArray<TSharedPtr<FJsonValue>>* UsedMaterials = nullptr;
						if (RendererObject->TryGetArrayField(TEXT("used_materials"), UsedMaterials) && UsedMaterials && UsedMaterials->Num() > 0)
						{
							OutFacts.PrimaryMaterialPath = (*UsedMaterials)[0].IsValid() ? (*UsedMaterials)[0]->AsString() : FString();
						}
					}
				}
			}
		}

		TSharedPtr<FJsonObject> StackReviewInput = MakeShareable(new FJsonObject());
		StackReviewInput->SetStringField(TEXT("system_path"), SystemPath);
		StackReviewInput->SetStringField(TEXT("emitter_name"), EmitterName);
		StackReviewInput->SetBoolField(TEXT("include_inputs"), false);
		TSharedPtr<FJsonObject> StackReviewResult;
		if (!ExecuteToolJson(TEXT("get_niagara_emitter_stack"), StackReviewInput, StackReviewResult, ErrorMessage) ||
			!StackReviewResult.IsValid())
		{
			return false;
		}

		auto StackContainsModule = [&StackReviewResult](const FString& ModuleTitleNeedle, const FString& FunctionNameNeedle) -> bool
		{
			const TArray<TSharedPtr<FJsonValue>>* Stages = nullptr;
			if (!StackReviewResult->TryGetArrayField(TEXT("stages"), Stages) || !Stages)
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
					if (!ModuleObject.IsValid())
					{
						continue;
					}

					FString Title;
					FString FunctionName;
					ModuleObject->TryGetStringField(TEXT("title"), Title);
					ModuleObject->TryGetStringField(TEXT("function_name"), FunctionName);
					if ((!ModuleTitleNeedle.IsEmpty() && Title.Contains(ModuleTitleNeedle, ESearchCase::IgnoreCase)) ||
						(!FunctionNameNeedle.IsEmpty() && FunctionName.Contains(FunctionNameNeedle, ESearchCase::IgnoreCase)))
					{
						return true;
					}
				}
			}

			return false;
		};

		OutFacts.bHasSphereLocation = StackContainsModule(TEXT("Sphere Location"), TEXT("SphereLocation"));
		OutFacts.bHasAddVelocity = StackContainsModule(TEXT("Add Velocity"), TEXT("AddVelocity"));
		OutFacts.bHasPointAttraction = StackContainsModule(TEXT("Point Attraction Force"), TEXT("PointAttractionForce"));
		OutFacts.bHasConeVelocity = StackContainsModule(TEXT("Add Velocity in Cone"), TEXT("AddVelocityInCone"));
		OutFacts.bHasCurlNoiseForce = StackContainsModule(TEXT("Curl Noise Force"), TEXT("CurlNoiseForce"));
		OutFacts.bHasDrag = StackContainsModule(TEXT("Drag"), TEXT("Drag"));
		OutFacts.bHasGravityForce = StackContainsModule(TEXT("Gravity Force"), TEXT("GravityForce"));
		return true;
	};

	auto TryParseInlineNumberValue = [](const FString& ValueString, double& OutValue) -> bool
	{
		OutValue = 0.0;
		if (ValueString.IsEmpty())
		{
			return false;
		}

		TSharedPtr<FJsonObject> InlineObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ValueString);
		if (FJsonSerializer::Deserialize(Reader, InlineObject) && InlineObject.IsValid())
		{
			return InlineObject->TryGetNumberField(TEXT("value"), OutValue);
		}

		if (ValueString.IsNumeric())
		{
			OutValue = FCString::Atod(*ValueString);
			return true;
		}

		return false;
	};

	auto FindModuleInputObject = [](const TSharedPtr<FJsonObject>& StackReviewResult, const FString& ModuleDisplayName, const FString& InputDisplayName) -> TSharedPtr<FJsonObject>
	{
		if (!StackReviewResult.IsValid())
		{
			return nullptr;
		}

		const TArray<TSharedPtr<FJsonValue>>* ModuleInputs = nullptr;
		if (!StackReviewResult->TryGetArrayField(TEXT("module_inputs"), ModuleInputs) || !ModuleInputs)
		{
			return nullptr;
		}

		for (const TSharedPtr<FJsonValue>& ModuleValue : *ModuleInputs)
		{
			const TSharedPtr<FJsonObject> ModuleObject = ModuleValue.IsValid() ? ModuleValue->AsObject() : nullptr;
			if (!ModuleObject.IsValid())
			{
				continue;
			}

			FString FoundDisplayName;
			ModuleObject->TryGetStringField(TEXT("display_name"), FoundDisplayName);
			if (!FoundDisplayName.Equals(ModuleDisplayName, ESearchCase::IgnoreCase))
			{
				continue;
			}

			const TArray<TSharedPtr<FJsonValue>>* Inputs = nullptr;
			if (!ModuleObject->TryGetArrayField(TEXT("inputs"), Inputs) || !Inputs)
			{
				continue;
			}

			for (const TSharedPtr<FJsonValue>& InputValue : *Inputs)
			{
				const TSharedPtr<FJsonObject> InputObject = InputValue.IsValid() ? InputValue->AsObject() : nullptr;
				if (!InputObject.IsValid())
				{
					continue;
				}

				FString FoundInputDisplayName;
				InputObject->TryGetStringField(TEXT("display_name"), FoundInputDisplayName);
				if (FoundInputDisplayName.Equals(InputDisplayName, ESearchCase::IgnoreCase))
				{
					return InputObject;
				}
			}
		}

		return nullptr;
	};

	auto TryReadModuleInputNumber = [&FindModuleInputObject, &TryParseInlineNumberValue](const TSharedPtr<FJsonObject>& StackReviewResult, const FString& ModuleDisplayName, const FString& InputDisplayName, double& OutValue) -> bool
	{
		const TSharedPtr<FJsonObject> InputObject = FindModuleInputObject(StackReviewResult, ModuleDisplayName, InputDisplayName);
		if (!InputObject.IsValid())
		{
			return false;
		}

		FString ValueString;
		if (!InputObject->TryGetStringField(TEXT("value_string"), ValueString))
		{
			return false;
		}

		return TryParseInlineNumberValue(ValueString, OutValue);
	};

	auto TryReadModuleInputStringField = [&FindModuleInputObject](const TSharedPtr<FJsonObject>& StackReviewResult, const FString& ModuleDisplayName, const FString& InputDisplayName, const TCHAR* FieldName, FString& OutValue) -> bool
	{
		const TSharedPtr<FJsonObject> InputObject = FindModuleInputObject(StackReviewResult, ModuleDisplayName, InputDisplayName);
		if (!InputObject.IsValid())
		{
			return false;
		}

		return InputObject->TryGetStringField(FieldName, OutValue);
	};

	TSharedPtr<FJsonObject> SemanticReviewContract;
	TSharedPtr<FJsonObject> VisualReviewContract;
	if (TargetLayer == ArchetypeConfig.HeroEmitter)
	{
		SemanticReviewContract = MakeShareable(new FJsonObject());
		SemanticReviewContract->SetStringField(TEXT("contract_name"), TEXT("main_attraction_layer_contract_v1"));
		SemanticReviewContract->SetStringField(TEXT("target_layer"), TargetLayer);

		TArray<TSharedPtr<FJsonValue>> ContractFailureLabels;
		TArray<TSharedPtr<FJsonValue>> ContractReasons;
		auto AddContractFailure = [&ContractFailureLabels, &ContractReasons](const FString& Label, const FString& Reason)
		{
			ContractFailureLabels.Add(MakeShared<FJsonValueString>(Label));
			ContractReasons.Add(MakeShared<FJsonValueString>(Reason));
		};

		FSemanticEmitterFacts EmitterFacts;
		if (!InspectEmitterFacts(TargetLayer, EmitterFacts))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_MAIN_CONTRACT_INSPECTION_FAILED"), ErrorMessage);
		}

		if (!EmitterFacts.bEmitterFound || !EmitterFacts.bHasEnabledSpriteRenderer || EmitterFacts.bHasEnabledRibbonRenderer)
		{
			AddContractFailure(
				TEXT("hero_sweep_missing"),
				!EmitterFacts.bEmitterFound
					? FString::Printf(TEXT("%s could not be inspected after authoring."), *TargetLayer)
					: TEXT("Layer A must end with an enabled sprite/stretched-sprite renderer and no enabled ribbon renderer."));
		}
		const bool bExplosionArchetype = Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase);
		if ((!bExplosionArchetype && (!EmitterFacts.bHasSphereLocation || !EmitterFacts.bHasPointAttraction || EmitterFacts.bHasConeVelocity)) ||
			(bExplosionArchetype && (!EmitterFacts.bHasSphereLocation || !EmitterFacts.bHasAddVelocity || EmitterFacts.bHasPointAttraction)))
		{
			AddContractFailure(
				TEXT("no_outside_to_core_read"),
				bExplosionArchetype
					? (!EmitterFacts.bHasSphereLocation
						? TEXT("Layer A is missing Sphere Location, so it cannot guarantee a readable burst shell around the flash core.")
						: (!EmitterFacts.bHasAddVelocity
							? TEXT("Layer A is missing Add Velocity, so it cannot guarantee a readable outward burst.")
							: TEXT("Layer A still contains inward-attraction style motion, which fights the explosion burst read.")))
					: (!EmitterFacts.bHasSphereLocation
						? TEXT("Layer A is missing Sphere Location, so it cannot guarantee an outer-zone spawn.")
						: (!EmitterFacts.bHasPointAttraction
							? TEXT("Layer A is missing Point Attraction Force, so it cannot guarantee inward pull to the core.")
							: TEXT("Layer A still contains cone-velocity style motion, which weakens the outside-to-core suction read."))));
		}

		SemanticReviewContract->SetBoolField(TEXT("emitter_found"), EmitterFacts.bEmitterFound);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_sprite_renderer"), EmitterFacts.bHasEnabledSpriteRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_ribbon_renderer"), EmitterFacts.bHasEnabledRibbonRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_sphere_location"), EmitterFacts.bHasSphereLocation);
		SemanticReviewContract->SetBoolField(TEXT("has_add_velocity"), EmitterFacts.bHasAddVelocity);
		SemanticReviewContract->SetBoolField(TEXT("has_point_attraction_force"), EmitterFacts.bHasPointAttraction);
		SemanticReviewContract->SetBoolField(TEXT("has_add_velocity_in_cone"), EmitterFacts.bHasConeVelocity);
		SemanticReviewContract->SetBoolField(TEXT("has_curl_noise_force"), EmitterFacts.bHasCurlNoiseForce);
		SemanticReviewContract->SetBoolField(TEXT("has_drag"), EmitterFacts.bHasDrag);
		SemanticReviewContract->SetBoolField(TEXT("has_gravity_force"), EmitterFacts.bHasGravityForce);
		SemanticReviewContract->SetArrayField(TEXT("failure_labels"), ContractFailureLabels);
		SemanticReviewContract->SetArrayField(TEXT("failure_reasons"), ContractReasons);
		SemanticReviewContract->SetBoolField(TEXT("contract_passed"), ContractFailureLabels.Num() == 0);

		if (ContractFailureLabels.Num() > 0)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SUCTION_MAIN_CONTRACT_FAILED"),
				TEXT("Hero layer semantic contract failed after compile."),
				SemanticReviewContract);
		}

		VisualReviewContract = MakeShareable(new FJsonObject());
		VisualReviewContract->SetStringField(TEXT("contract_name"), TEXT("main_attraction_visual_contract_v1"));
		VisualReviewContract->SetStringField(TEXT("target_layer"), TargetLayer);

		TSharedPtr<FJsonObject> StackInputWithValues = MakeShareable(new FJsonObject());
		StackInputWithValues->SetStringField(TEXT("system_path"), SystemPath);
		StackInputWithValues->SetStringField(TEXT("emitter_name"), TargetLayer);
		StackInputWithValues->SetBoolField(TEXT("include_inputs"), true);
		TSharedPtr<FJsonObject> StackWithValues;
		if (!ExecuteToolJson(TEXT("get_niagara_emitter_stack"), StackInputWithValues, StackWithValues, ErrorMessage) || !StackWithValues.IsValid())
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_MAIN_VISUAL_INSPECTION_FAILED"), ErrorMessage);
		}

		double SpawnRateValue = 0.0;
		double SphereRadiusValue = 0.0;
		double AttractionRadiusValue = 0.0;
		double DragValue = 0.0;
		double CurlNoiseValue = 0.0;
		double HeroIntensity = 0.0;
		double HeroWidth = 0.0;
		bool bHasSpawnRateValue = TryReadModuleInputNumber(StackWithValues, TEXT("Spawn Rate"), TEXT("SpawnRate"), SpawnRateValue);
		bool bHasSphereRadiusValue = TryReadModuleInputNumber(StackWithValues, TEXT("Sphere Location"), TEXT("Sphere Radius"), SphereRadiusValue);
		bool bHasAttractionRadiusValue = TryReadModuleInputNumber(StackWithValues, TEXT("Point Attraction Force"), TEXT("Attraction Radius"), AttractionRadiusValue);
		bool bHasDragValue = TryReadModuleInputNumber(StackWithValues, TEXT("Drag"), TEXT("Drag"), DragValue);
		bool bHasCurlNoiseValue = TryReadModuleInputNumber(StackWithValues, TEXT("Curl Noise Force"), TEXT("Noise Strength"), CurlNoiseValue);
		FString LifetimeValueSource;
		const bool bHasLifetimeValueSource = TryReadModuleInputStringField(StackWithValues, TEXT("Initialize Particle"), TEXT("Lifetime"), TEXT("value_source"), LifetimeValueSource);
		const bool bLifetimeIsDynamic = bHasLifetimeValueSource && LifetimeValueSource.Equals(TEXT("dynamic"), ESearchCase::IgnoreCase);

		bool bHeroMaterialFound = false;
		bool bHeroMaterialLooksRight = false;
		TSharedPtr<FJsonObject> HeroMaterialInfo;
		if (!EmitterFacts.PrimaryMaterialPath.IsEmpty())
		{
			TSharedPtr<FJsonObject> MaterialInfoInput = MakeShareable(new FJsonObject());
			MaterialInfoInput->SetStringField(TEXT("material_instance_path"), EmitterFacts.PrimaryMaterialPath);
			if (ExecuteToolJson(TEXT("get_material_instance_info"), MaterialInfoInput, HeroMaterialInfo, ErrorMessage) &&
				HeroMaterialInfo.IsValid())
			{
				bHeroMaterialFound = HeroMaterialInfo->GetBoolField(TEXT("success"));
			}
			ErrorMessage.Reset();
		}

		const double MinHeroWidth =
			Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ? 0.20 :
			(Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase) ? 0.32 :
			(Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.48 : 0.30));

		if (HeroMaterialInfo.IsValid())
		{
			const TSharedPtr<FJsonObject>* OverridesObjectPtr = nullptr;
			HeroMaterialInfo->TryGetObjectField(TEXT("overrides"), OverridesObjectPtr);
			TSharedPtr<FJsonObject> OverridesObject = OverridesObjectPtr ? *OverridesObjectPtr : nullptr;
			const TArray<TSharedPtr<FJsonValue>>* ScalarOverrides = nullptr;
			if (OverridesObject.IsValid() && OverridesObject->TryGetArrayField(TEXT("scalar"), ScalarOverrides) && ScalarOverrides)
			{
				for (const TSharedPtr<FJsonValue>& ScalarValue : *ScalarOverrides)
				{
					const TSharedPtr<FJsonObject> ScalarObject = ScalarValue.IsValid() ? ScalarValue->AsObject() : nullptr;
					if (!ScalarObject.IsValid())
					{
						continue;
					}

					FString ParamName;
					double ParamValue = 0.0;
					if (!ScalarObject->TryGetStringField(TEXT("name"), ParamName) ||
						!ScalarObject->TryGetNumberField(TEXT("value"), ParamValue))
					{
						continue;
					}

					if (ParamName.Equals(TEXT("SR_Intensity"), ESearchCase::IgnoreCase))
					{
						HeroIntensity = ParamValue;
					}
					else if (ParamName.Equals(TEXT("SR_Width"), ESearchCase::IgnoreCase))
					{
						HeroWidth = ParamValue;
					}
				}
			}

			bHeroMaterialLooksRight =
				EmitterFacts.PrimaryMaterialPath.Contains(TEXT("HeroStreak"), ESearchCase::IgnoreCase) ||
				EmitterFacts.PrimaryMaterialPath.Contains(TEXT("HeroElectric"), ESearchCase::IgnoreCase) ||
				EmitterFacts.PrimaryMaterialPath.Contains(TEXT("HeroImpact"), ESearchCase::IgnoreCase) ||
				EmitterFacts.PrimaryMaterialPath.Contains(TEXT("HeroExplosion"), ESearchCase::IgnoreCase) ||
				(HeroIntensity >= 12.0 && HeroWidth >= MinHeroWidth);
		}

		const double MinCurlNoise =
			(Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase) ||
			 Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
				? 0.08
				: (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase) ? 0.10 : 0.10);
		const double VisualTolerance = 0.001;

		TArray<TSharedPtr<FJsonValue>> VisualFailureLabels;
		TArray<TSharedPtr<FJsonValue>> VisualFailureReasons;
		auto AddVisualFailure = [&VisualFailureLabels, &VisualFailureReasons](const FString& Label, const FString& Reason)
		{
			VisualFailureLabels.Add(MakeShared<FJsonValueString>(Label));
			VisualFailureReasons.Add(MakeShared<FJsonValueString>(Reason));
		};

		if (!bHasSpawnRateValue || SpawnRateValue < 24.0 || !bLifetimeIsDynamic || !bHeroMaterialFound || !bHeroMaterialLooksRight ||
			(HeroIntensity > 0.0 && HeroIntensity + VisualTolerance < 12.0) || (HeroWidth > 0.0 && HeroWidth + VisualTolerance < MinHeroWidth))
		{
			FString Reason = TEXT("Layer A is missing one or more hero-read signals: readable spawn density, dynamic streak hold, or a tuned hero streak material body.");
			if (!bHasSpawnRateValue || SpawnRateValue < 24.0)
			{
				Reason = TEXT("Layer A spawn density is too low for a readable hero sweep.");
			}
			else if (!bLifetimeIsDynamic)
			{
				Reason = TEXT("Layer A lifetime is not driven dynamically, so the hero sweep is likely to read too static or too short.");
			}
			else if (!bHeroMaterialFound)
			{
				Reason = TEXT("Layer A does not have a readable hero material instance bound.");
			}
			else if (!bHeroMaterialLooksRight || (HeroIntensity > 0.0 && HeroIntensity + VisualTolerance < 12.0) || (HeroWidth > 0.0 && HeroWidth + VisualTolerance < MinHeroWidth))
			{
				Reason = TEXT("Layer A hero material body is too weak or too thin to read as a dominant sweep.");
			}
			AddVisualFailure(TEXT("hero_sweep_missing"), Reason);
		}

		if ((!bExplosionArchetype && (!bHasSphereRadiusValue || SphereRadiusValue + VisualTolerance < 60.0 || !bHasAttractionRadiusValue || AttractionRadiusValue + VisualTolerance < 60.0 ||
			!bHasDragValue || DragValue + VisualTolerance < 0.75 || !bHasCurlNoiseValue || CurlNoiseValue + VisualTolerance < MinCurlNoise)) ||
			(bExplosionArchetype && (!bHasSphereRadiusValue || SphereRadiusValue + VisualTolerance < 60.0 || !EmitterFacts.bHasAddVelocity ||
				!bHasDragValue || DragValue - VisualTolerance > 1.10 || !bHasCurlNoiseValue || CurlNoiseValue + VisualTolerance < MinCurlNoise)))
		{
			FString Reason = TEXT("Layer A outside-to-core read is under-tuned for visual readability.");
			if (!bHasSphereRadiusValue || SphereRadiusValue + VisualTolerance < 60.0)
			{
				Reason = bExplosionArchetype
					? TEXT("Layer A burst shell radius is too small to read as a convincing explosion around the flash core.")
					: TEXT("Layer A outer spawn radius is too small to read as energy being pulled from around the core.");
			}
			else if (!bExplosionArchetype && (!bHasAttractionRadiusValue || AttractionRadiusValue + VisualTolerance < 60.0))
			{
				Reason = TEXT("Layer A attraction radius is too small to preserve an outside-to-core suction read.");
			}
			else if (bExplosionArchetype && !EmitterFacts.bHasAddVelocity)
			{
				Reason = TEXT("Layer A is missing Add Velocity, so the explosion cannot read as a fast outward burst.");
			}
			else if (!bHasDragValue || (!bExplosionArchetype && DragValue + VisualTolerance < 0.75) || (bExplosionArchetype && DragValue - VisualTolerance > 1.10))
			{
				Reason = bExplosionArchetype
					? TEXT("Layer A drag is too high, so the explosion burst is likely to stall and lose punch.")
					: TEXT("Layer A drag is too low, so the hero sweep is likely to smear or overshoot instead of reading as a controlled pull.");
			}
			else if (!bHasCurlNoiseValue || CurlNoiseValue + VisualTolerance < MinCurlNoise)
			{
				Reason = Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase)
					? TEXT("Layer A curl noise is too weak, so the burst sweep may read too straight and lose its impact flare.")
					: (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase)
						? TEXT("Layer A curl noise is too weak, so the blast streaks read too uniform and lose their explosive breakup.")
					: (Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase)
						? TEXT("Layer A curl noise is too weak, so the charged arc sweep may read too straight and lose its electric snap.")
						: TEXT("Layer A curl noise is too weak, so the sweep may read too straight and lose the magical pull feel.")));
			}
			AddVisualFailure(TEXT("no_outside_to_core_read"), Reason);
		}

		VisualReviewContract->SetStringField(TEXT("hero_material_path"), EmitterFacts.PrimaryMaterialPath);
		VisualReviewContract->SetBoolField(TEXT("hero_material_found"), bHeroMaterialFound);
		VisualReviewContract->SetBoolField(TEXT("hero_material_looks_right"), bHeroMaterialLooksRight);
		VisualReviewContract->SetBoolField(TEXT("has_dynamic_lifetime"), bLifetimeIsDynamic);
		if (bHasSpawnRateValue) VisualReviewContract->SetNumberField(TEXT("spawn_rate_value"), SpawnRateValue);
		if (bHasSphereRadiusValue) VisualReviewContract->SetNumberField(TEXT("sphere_radius_value"), SphereRadiusValue);
		if (bHasAttractionRadiusValue) VisualReviewContract->SetNumberField(TEXT("attraction_radius_value"), AttractionRadiusValue);
		if (bHasDragValue) VisualReviewContract->SetNumberField(TEXT("drag_value"), DragValue);
		if (bHasCurlNoiseValue) VisualReviewContract->SetNumberField(TEXT("curl_noise_strength_value"), CurlNoiseValue);
		if (HeroIntensity > 0.0) VisualReviewContract->SetNumberField(TEXT("hero_material_intensity"), HeroIntensity);
		if (HeroWidth > 0.0) VisualReviewContract->SetNumberField(TEXT("hero_material_width"), HeroWidth);
		VisualReviewContract->SetArrayField(TEXT("failure_labels"), VisualFailureLabels);
		VisualReviewContract->SetArrayField(TEXT("failure_reasons"), VisualFailureReasons);
		VisualReviewContract->SetBoolField(TEXT("contract_passed"), VisualFailureLabels.Num() == 0);

		if (VisualFailureLabels.Num() > 0)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SUCTION_MAIN_VISUAL_CONTRACT_FAILED"),
				TEXT("Hero layer visual proxy contract failed after compile."),
				VisualReviewContract);
		}
	}
	else if (TargetLayer == ArchetypeConfig.CoreEmitter)
	{
		SemanticReviewContract = MakeShareable(new FJsonObject());
		SemanticReviewContract->SetStringField(TEXT("contract_name"), TEXT("core_turbulence_layer_contract_v1"));
		SemanticReviewContract->SetStringField(TEXT("target_layer"), TargetLayer);

		TArray<TSharedPtr<FJsonValue>> ContractFailureLabels;
		TArray<TSharedPtr<FJsonValue>> ContractReasons;
		auto AddContractFailure = [&ContractFailureLabels, &ContractReasons](const FString& Label, const FString& Reason)
		{
			ContractFailureLabels.Add(MakeShared<FJsonValueString>(Label));
			ContractReasons.Add(MakeShared<FJsonValueString>(Reason));
		};

		FSemanticEmitterFacts EmitterFacts;
		if (!InspectEmitterFacts(TargetLayer, EmitterFacts))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_CORE_CONTRACT_INSPECTION_FAILED"), ErrorMessage);
		}

		if (!EmitterFacts.bEmitterFound || !EmitterFacts.bHasEnabledSpriteRenderer || EmitterFacts.bHasEnabledRibbonRenderer || !EmitterFacts.bHasSphereLocation)
		{
			AddContractFailure(
				TEXT("support_fill_missing"),
				!EmitterFacts.bEmitterFound
					? FString::Printf(TEXT("%s could not be inspected after authoring."), *TargetLayer)
					: (!EmitterFacts.bHasSphereLocation
						? TEXT("Core layer must spawn inside a spherical volume so it can fill the orb interior.")
						: TEXT("Core layer must stay on an enabled sprite renderer and not fall back to ribbon-style bodies.")));
		}
		if (!EmitterFacts.bHasCurlNoiseForce || !EmitterFacts.bHasDrag || EmitterFacts.bHasConeVelocity)
		{
			AddContractFailure(
				TEXT("inner_hot_core_missing"),
				!EmitterFacts.bHasCurlNoiseForce
					? TEXT("Core layer is missing Curl Noise Force, so the inner mass will not flicker with enough internal turbulence.")
					: (!EmitterFacts.bHasDrag
						? TEXT("Core layer is missing Drag, so inner energy detail is likely to smear instead of holding near the core.")
						: TEXT("Core layer still contains cone-velocity style motion, which fights the contained orb-center read.")));
		}

		SemanticReviewContract->SetBoolField(TEXT("emitter_found"), EmitterFacts.bEmitterFound);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_sprite_renderer"), EmitterFacts.bHasEnabledSpriteRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_ribbon_renderer"), EmitterFacts.bHasEnabledRibbonRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_sphere_location"), EmitterFacts.bHasSphereLocation);
		SemanticReviewContract->SetBoolField(TEXT("has_point_attraction_force"), EmitterFacts.bHasPointAttraction);
		SemanticReviewContract->SetBoolField(TEXT("has_add_velocity_in_cone"), EmitterFacts.bHasConeVelocity);
		SemanticReviewContract->SetBoolField(TEXT("has_curl_noise_force"), EmitterFacts.bHasCurlNoiseForce);
		SemanticReviewContract->SetBoolField(TEXT("has_drag"), EmitterFacts.bHasDrag);
		SemanticReviewContract->SetArrayField(TEXT("failure_labels"), ContractFailureLabels);
		SemanticReviewContract->SetArrayField(TEXT("failure_reasons"), ContractReasons);
		SemanticReviewContract->SetBoolField(TEXT("contract_passed"), ContractFailureLabels.Num() == 0);

		if (ContractFailureLabels.Num() > 0)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SUCTION_CORE_CONTRACT_FAILED"),
				TEXT("Core layer semantic contract failed after compile."),
				SemanticReviewContract);
		}
	}
	else if (TargetLayer == ArchetypeConfig.GlowEmitter)
	{
		SemanticReviewContract = MakeShareable(new FJsonObject());
		SemanticReviewContract->SetStringField(TEXT("contract_name"), TEXT("shock_glow_layer_contract_v1"));
		SemanticReviewContract->SetStringField(TEXT("target_layer"), TargetLayer);

		TArray<TSharedPtr<FJsonValue>> ContractFailureLabels;
		TArray<TSharedPtr<FJsonValue>> ContractReasons;
		auto AddContractFailure = [&ContractFailureLabels, &ContractReasons](const FString& Label, const FString& Reason)
		{
			ContractFailureLabels.Add(MakeShared<FJsonValueString>(Label));
			ContractReasons.Add(MakeShared<FJsonValueString>(Reason));
		};

		FSemanticEmitterFacts EmitterFacts;
		if (!InspectEmitterFacts(TargetLayer, EmitterFacts))
		{
			return BuildNiagaraToolError(OutResult, TEXT("NI_SUCTION_SHOCK_GLOW_CONTRACT_INSPECTION_FAILED"), ErrorMessage);
		}

		if (!EmitterFacts.bEmitterFound || !EmitterFacts.bHasEnabledSpriteRenderer || EmitterFacts.bHasEnabledRibbonRenderer)
		{
			AddContractFailure(
				TEXT("support_fill_missing"),
				!EmitterFacts.bEmitterFound
					? FString::Printf(TEXT("%s could not be inspected after authoring."), *TargetLayer)
					: TEXT("Bind glow layer must stay on an enabled sprite renderer and must not introduce ribbon-style bodies."));
		}
		if (EmitterFacts.bHasSphereLocation || EmitterFacts.bHasPointAttraction || EmitterFacts.bHasConeVelocity)
		{
			AddContractFailure(
				TEXT("inner_hot_core_missing"),
				EmitterFacts.bHasSphereLocation
					? TEXT("Bind glow layer should stay bound near the core and must not spawn from a sphere shell.")
					: (EmitterFacts.bHasPointAttraction
						? TEXT("Bind glow layer should not rely on attraction-force motion; it is a bind glow, not a pulled streak layer.")
						: TEXT("Bind glow layer still contains cone-velocity style motion, which breaks the compact binding glow read.")));
		}

		SemanticReviewContract->SetBoolField(TEXT("emitter_found"), EmitterFacts.bEmitterFound);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_sprite_renderer"), EmitterFacts.bHasEnabledSpriteRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_enabled_ribbon_renderer"), EmitterFacts.bHasEnabledRibbonRenderer);
		SemanticReviewContract->SetBoolField(TEXT("has_sphere_location"), EmitterFacts.bHasSphereLocation);
		SemanticReviewContract->SetBoolField(TEXT("has_point_attraction_force"), EmitterFacts.bHasPointAttraction);
		SemanticReviewContract->SetBoolField(TEXT("has_add_velocity_in_cone"), EmitterFacts.bHasConeVelocity);
		SemanticReviewContract->SetBoolField(TEXT("has_curl_noise_force"), EmitterFacts.bHasCurlNoiseForce);
		SemanticReviewContract->SetBoolField(TEXT("has_drag"), EmitterFacts.bHasDrag);
		SemanticReviewContract->SetArrayField(TEXT("failure_labels"), ContractFailureLabels);
		SemanticReviewContract->SetArrayField(TEXT("failure_reasons"), ContractReasons);
		SemanticReviewContract->SetBoolField(TEXT("contract_passed"), ContractFailureLabels.Num() == 0);

		if (ContractFailureLabels.Num() > 0)
		{
			return BuildNiagaraToolError(
				OutResult,
				TEXT("NI_SUCTION_SHOCK_GLOW_CONTRACT_FAILED"),
				TEXT("Bind glow semantic contract failed after compile."),
				SemanticReviewContract);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"),
		Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
			? TEXT("run_suction_core_niagara_op")
			: TEXT("run_v2_niagara_op"));
	Result->SetStringField(TEXT("archetype"), Archetype);
	Result->SetStringField(TEXT("op_name"), OpName);
	Result->SetStringField(TEXT("system_path"), SystemPath);
	Result->SetStringField(TEXT("target_layer"), TargetLayer);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("authoring_mode"), AuthoringMode);
	Result->SetStringField(TEXT("required_renderer_family"), RendererFamily);
	Result->SetStringField(TEXT("required_material_family"), MaterialFamily);
	Result->SetNumberField(TEXT("matched_user_parameter_count"), ExistingVisualContractParamCount);
	Result->SetBoolField(TEXT("used_user_parameter_contract"), !bUseLegacyStackAuthoring);
	Result->SetArrayField(TEXT("forbidden_mutations"), ForbiddenMutations);
	Result->SetArrayField(TEXT("expected_review_deltas"), ExpectedReviewDeltas);
	Result->SetArrayField(TEXT("mutations"), MutationLog);
	Result->SetArrayField(TEXT("applied_user_parameter_bindings"), AppliedUserParameterBindings);
	Result->SetObjectField(TEXT("compile_result"), CompileResult);
	if (ActiveVisualContract.IsValid())
	{
		Result->SetObjectField(TEXT("visual_contract"), ActiveVisualContract);
	}
	if (!DirectUserParamSnapshotPath.IsEmpty())
	{
		Result->SetStringField(TEXT("snapshot_path"), DirectUserParamSnapshotPath);
	}
	if (SemanticReviewContract.IsValid())
	{
		Result->SetObjectField(TEXT("semantic_review_contract"), SemanticReviewContract);
	}
	if (VisualReviewContract.IsValid())
	{
		Result->SetObjectField(TEXT("visual_review_contract"), VisualReviewContract);
	}
	Result->SetBoolField(TEXT("compile_log_checked"), CompileResult.IsValid() ? CompileResult->GetBoolField(TEXT("compile_log_checked")) : false);
	Result->SetArrayField(TEXT("mutated_assets"), {
		MakeShared<FJsonValueString>(SystemPath)
	});
	Result->SetArrayField(TEXT("predicted_resolved_failure_labels"), PredictedResolvedFailureLabels);
	Result->SetArrayField(TEXT("predicted_remaining_failure_labels"), PredictedRemainingFailureLabels);
	OutResult = JsonObjToString(Result);
	return true;
}
