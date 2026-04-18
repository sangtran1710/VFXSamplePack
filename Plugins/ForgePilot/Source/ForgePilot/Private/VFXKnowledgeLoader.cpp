// Copyright 2026, HenryFX. ForgePilot Plugin.

#include "VFXKnowledgeLoader.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"

namespace
{
	static FString NormalizeKnowledgeToken(const FString& Value)
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

	static TArray<FString> ExtractDescriptionTokens(const FString& Description)
	{
		FString Sanitized;
		Sanitized.Reserve(Description.Len());
		for (TCHAR Char : Description)
		{
			if (FChar::IsAlnum(Char))
			{
				Sanitized.AppendChar(FChar::ToLower(Char));
			}
			else
			{
				Sanitized.AppendChar(TEXT(' '));
			}
		}

		TArray<FString> Tokens;
		Sanitized.ParseIntoArrayWS(Tokens);
		Tokens.Sort();
		for (int32 Index = Tokens.Num() - 1; Index > 0; --Index)
		{
			if (Tokens[Index] == Tokens[Index - 1])
			{
				Tokens.RemoveAt(Index);
			}
		}
		return Tokens;
	}

	static bool LoadJsonFile(const FString& FilePath, TSharedPtr<FJsonObject>& OutObject, FString& OutError)
	{
		FString Contents;
		if (!FFileHelper::LoadFileToString(Contents, *FilePath))
		{
			OutError = FString::Printf(TEXT("Could not read JSON file: %s"), *FilePath);
			return false;
		}

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Contents);
		if (!FJsonSerializer::Deserialize(Reader, OutObject) || !OutObject.IsValid())
		{
			OutError = FString::Printf(TEXT("Could not parse JSON file: %s"), *FilePath);
			return false;
		}

		return true;
	}

	static FString GetEffectsRootDir()
	{
		return FVFXKnowledgeLoader::GetKnowledgeRootDir() / TEXT("Effects");
	}

	static FString GetPrinciplesRootDir()
	{
		return FVFXKnowledgeLoader::GetKnowledgeRootDir() / TEXT("Principles");
	}

	static FString GetNiagaraPatternsRootDir()
	{
		return FVFXKnowledgeLoader::GetKnowledgeRootDir() / TEXT("Niagara") / TEXT("Patterns");
	}

	static FString GetSynonymsFilePath()
	{
		return FVFXKnowledgeLoader::GetKnowledgeRootDir() / TEXT("effect_synonyms.json");
	}

	static bool LoadSynonymConfig(TSharedPtr<FJsonObject>& OutSynonyms, FString& OutError)
	{
		return LoadJsonFile(GetSynonymsFilePath(), OutSynonyms, OutError);
	}

	static bool LoadOptionalJsonFile(const FString& FilePath, TSharedPtr<FJsonObject>& OutObject)
	{
		OutObject.Reset();
		if (!FPaths::FileExists(FilePath))
		{
			return false;
		}

		FString IgnoredError;
		return LoadJsonFile(FilePath, OutObject, IgnoredError) && OutObject.IsValid();
	}

	static bool FindRecipeTypeDirectory(
		const FString& RequestedEffectType,
		FString& OutCanonicalEffectType,
		FString& OutRecipeFilePath,
		TArray<FString>* OutAvailableRecipes = nullptr)
	{
		const FString EffectsRoot = GetEffectsRootDir();
		TArray<FString> CandidateDirs;
		IFileManager::Get().FindFiles(CandidateDirs, *(EffectsRoot / TEXT("*")), false, true);

		const FString Wanted = NormalizeKnowledgeToken(RequestedEffectType);
		TArray<FString> AvailableRecipes;

		for (const FString& CandidateDir : CandidateDirs)
		{
			const FString RecipePath = EffectsRoot / CandidateDir / TEXT("recipe.json");
			if (!FPaths::FileExists(RecipePath))
			{
				continue;
			}

			AvailableRecipes.Add(CandidateDir);
			if (!Wanted.IsEmpty() && NormalizeKnowledgeToken(CandidateDir) == Wanted)
			{
				OutCanonicalEffectType = CandidateDir;
				OutRecipeFilePath = RecipePath;
			}
		}

		AvailableRecipes.Sort();
		if (OutAvailableRecipes)
		{
			*OutAvailableRecipes = AvailableRecipes;
		}

		return !OutCanonicalEffectType.IsEmpty() && !OutRecipeFilePath.IsEmpty();
	}

static TArray<FString> GetObjectKeys(const TSharedPtr<FJsonObject>& Object)
	{
		TArray<FString> Keys;
		if (Object.IsValid())
		{
			Object->Values.GetKeys(Keys);
			Keys.Sort();
		}
		return Keys;
	}

	static bool TryResolveFromSynonymMap(
		const TSharedPtr<FJsonObject>& SynonymObject,
		const FString& DescriptionNorm,
		FString& OutMatchedKey)
	{
		if (!SynonymObject.IsValid())
		{
			return false;
		}

		for (const auto& Pair : SynonymObject->Values)
		{
			const TArray<TSharedPtr<FJsonValue>>* SynonymsPtr = nullptr;
			if (!SynonymObject->TryGetArrayField(Pair.Key, SynonymsPtr) || !SynonymsPtr)
			{
				continue;
			}

			for (const TSharedPtr<FJsonValue>& SynonymValue : *SynonymsPtr)
			{
				FString SynonymText;
				if (!SynonymValue.IsValid() || !SynonymValue->TryGetString(SynonymText))
				{
					continue;
				}

				const FString SynonymNorm = NormalizeKnowledgeToken(SynonymText);
				if (!SynonymNorm.IsEmpty() && DescriptionNorm.Contains(SynonymNorm))
				{
					OutMatchedKey = Pair.Key;
					return true;
				}
			}
		}

		return false;
	}

	static bool TryResolveVariantFromSynonymConfig(
		const TSharedPtr<FJsonObject>& SynonymConfig,
		const FString& EffectType,
		const FString& DescriptionNorm,
		FString& OutVariant)
	{
		if (!SynonymConfig.IsValid())
		{
			return false;
		}

		const TSharedPtr<FJsonObject>* VariantMapsPtr = nullptr;
		if (!SynonymConfig->TryGetObjectField(TEXT("variant_synonyms"), VariantMapsPtr) || !VariantMapsPtr || !(*VariantMapsPtr).IsValid())
		{
			return false;
		}

		const TSharedPtr<FJsonObject>* EffectVariantsPtr = nullptr;
		if (!(*VariantMapsPtr)->TryGetObjectField(EffectType, EffectVariantsPtr) || !EffectVariantsPtr || !(*EffectVariantsPtr).IsValid())
		{
			return false;
		}

		return TryResolveFromSynonymMap(*EffectVariantsPtr, DescriptionNorm, OutVariant);
	}

	static TArray<FString> CollectRelevantPaletteNames(
		const FString& EffectType,
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& DescriptionTokens,
		const TSharedPtr<FJsonObject>& ColorPrinciples)
	{
		TArray<FString> Result;
		if (!ColorPrinciples.IsValid())
		{
			return Result;
		}

		const TSharedPtr<FJsonObject>* PalettesPtr = nullptr;
		if (!ColorPrinciples->TryGetObjectField(TEXT("palettes"), PalettesPtr) || !PalettesPtr || !(*PalettesPtr).IsValid())
		{
			return Result;
		}

		const FString EffectNorm = NormalizeKnowledgeToken(EffectType);
		const TSharedPtr<FJsonObject> Palettes = *PalettesPtr;

		for (const auto& Pair : Palettes->Values)
		{
			const FString PaletteName = Pair.Key;
			const FString PaletteNorm = NormalizeKnowledgeToken(PaletteName);
			if (!EffectNorm.IsEmpty() && PaletteNorm.Contains(EffectNorm))
			{
				Result.AddUnique(PaletteName);
			}
		}

		const TSharedPtr<FJsonObject>* BaseArchitecturePtr = nullptr;
		if (Recipe.IsValid() &&
			Recipe->TryGetObjectField(TEXT("base_architecture"), BaseArchitecturePtr) &&
			BaseArchitecturePtr && (*BaseArchitecturePtr).IsValid())
		{
			const TArray<TSharedPtr<FJsonValue>>* EmitterLayersPtr = nullptr;
			if ((*BaseArchitecturePtr)->TryGetArrayField(TEXT("emitter_layers"), EmitterLayersPtr) && EmitterLayersPtr)
			{
				for (const TSharedPtr<FJsonValue>& LayerValue : *EmitterLayersPtr)
				{
					const TSharedPtr<FJsonObject>* LayerObjPtr = nullptr;
					if (!LayerValue.IsValid() || !LayerValue->TryGetObject(LayerObjPtr) || !LayerObjPtr || !(*LayerObjPtr).IsValid())
					{
						continue;
					}

					FString LayerName;
					(*LayerObjPtr)->TryGetStringField(TEXT("name"), LayerName);
					const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
					if (LayerNorm.Contains(TEXT("smoke")))
					{
						Result.AddUnique(TEXT("smoke_dark"));
						Result.AddUnique(TEXT("smoke_light"));
					}
					if (LayerNorm.Contains(TEXT("ember")) && Palettes->HasField(TEXT("fire_natural")))
					{
						Result.AddUnique(TEXT("fire_natural"));
					}
				}
			}
		}

		for (const FString& Token : DescriptionTokens)
		{
			if (Token == TEXT("smoke"))
			{
				Result.AddUnique(TEXT("smoke_dark"));
				Result.AddUnique(TEXT("smoke_light"));
			}
			else if (Token == TEXT("ember") || Token == TEXT("embers"))
			{
				Result.AddUnique(TEXT("fire_natural"));
			}
		}

		if (Result.Num() == 0)
		{
			if (EffectNorm == TEXT("explosion"))
			{
				Result.AddUnique(TEXT("explosion"));
			}
			else if (EffectNorm == TEXT("smoke"))
			{
				Result.AddUnique(TEXT("smoke_dark"));
			}
			else if (EffectNorm == TEXT("fire"))
			{
				Result.AddUnique(TEXT("fire_natural"));
			}
		}

		return Result;
	}

	static TArray<FString> CollectRelevantScaleGuidelines(
		const FString& EffectType,
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& DescriptionTokens,
		const TSharedPtr<FJsonObject>& ScalePrinciples)
	{
		TArray<FString> Result;
		if (!ScalePrinciples.IsValid())
		{
			return Result;
		}

		const TSharedPtr<FJsonObject>* GuidelinesPtr = nullptr;
		if (!ScalePrinciples->TryGetObjectField(TEXT("effect_scale_guidelines"), GuidelinesPtr) || !GuidelinesPtr || !(*GuidelinesPtr).IsValid())
		{
			return Result;
		}

		const FString EffectNorm = NormalizeKnowledgeToken(EffectType);
		const TSharedPtr<FJsonObject> Guidelines = *GuidelinesPtr;

		TSet<FString> MatchKeys;
		MatchKeys.Add(EffectNorm);

		const TSharedPtr<FJsonObject>* VariantsPtr = nullptr;
		if (Recipe.IsValid() && Recipe->TryGetObjectField(TEXT("variants"), VariantsPtr) && VariantsPtr && (*VariantsPtr).IsValid())
		{
			for (const FString& VariantKey : GetObjectKeys(*VariantsPtr))
			{
				MatchKeys.Add(NormalizeKnowledgeToken(VariantKey));
			}
		}

		for (const FString& Token : DescriptionTokens)
		{
			MatchKeys.Add(NormalizeKnowledgeToken(Token));
		}

		for (const auto& Pair : Guidelines->Values)
		{
			const FString GuidelineName = Pair.Key;
			const FString GuidelineNorm = NormalizeKnowledgeToken(GuidelineName);

			for (const FString& MatchKey : MatchKeys)
			{
				if (MatchKey.IsEmpty())
				{
					continue;
				}

				if (GuidelineNorm.Contains(MatchKey) || MatchKey.Contains(GuidelineNorm))
				{
					Result.AddUnique(GuidelineName);
					break;
				}
			}
		}

		if (Result.Num() == 0)
		{
			if (EffectNorm == TEXT("fire"))
			{
				Result.AddUnique(TEXT("campfire"));
				Result.AddUnique(TEXT("hand_fire"));
			}
			else if (EffectNorm == TEXT("explosion"))
			{
				Result.AddUnique(TEXT("explosion_small"));
			}
		}

		return Result;
	}

	static TArray<FString> CollectTimingProfiles(
		const FString& EffectType,
		const TSharedPtr<FJsonObject>&,
		const TArray<FString>& DescriptionTokens,
		const TSharedPtr<FJsonObject>& TimingPrinciples)
	{
		TArray<FString> Result;
		if (!TimingPrinciples.IsValid())
		{
			return Result;
		}

		const TSharedPtr<FJsonObject>* ProfilesPtr = nullptr;
		if (!TimingPrinciples->TryGetObjectField(TEXT("effect_timing_profiles"), ProfilesPtr) || !ProfilesPtr || !(*ProfilesPtr).IsValid())
		{
			return Result;
		}

		const TSharedPtr<FJsonObject> Profiles = *ProfilesPtr;
		const FString EffectNorm = NormalizeKnowledgeToken(EffectType);

		if (Profiles->HasField(EffectNorm))
		{
			Result.AddUnique(EffectNorm);
		}

		if (EffectNorm == TEXT("fire") && Profiles->HasField(TEXT("fire_loop")))
		{
			Result.AddUnique(TEXT("fire_loop"));
		}
		else if (EffectNorm == TEXT("magic") && Profiles->HasField(TEXT("magic_cast")))
		{
			Result.AddUnique(TEXT("magic_cast"));
		}
		else if (EffectNorm == TEXT("water") && Profiles->HasField(TEXT("water_splash")))
		{
			Result.AddUnique(TEXT("water_splash"));
		}
		else if (EffectNorm == TEXT("impact") && Profiles->HasField(TEXT("impact_hit")))
		{
			Result.AddUnique(TEXT("impact_hit"));
		}

		for (const FString& Token : DescriptionTokens)
		{
			const FString TokenNorm = NormalizeKnowledgeToken(Token);
			if (Profiles->HasField(TokenNorm))
			{
				Result.AddUnique(TokenNorm);
			}
		}

		if (Result.Num() == 0 && Profiles->HasField(TEXT("fire_loop")))
		{
			Result.AddUnique(TEXT("fire_loop"));
		}

		return Result;
	}

	static TSharedPtr<FJsonObject> BuildRecipeBundleObject(
		const FString& CanonicalEffectType,
		const FString& RecipeFilePath,
		const TSharedPtr<FJsonObject>& Recipe,
		const TSharedPtr<FJsonObject>& ColorPrinciples,
		const TSharedPtr<FJsonObject>& TimingPrinciples,
		const TSharedPtr<FJsonObject>& ScalePrinciples,
		const TArray<FString>& DescriptionTokens)
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		Result->SetBoolField(TEXT("success"), true);
		Result->SetStringField(TEXT("effect_type"), CanonicalEffectType);
		Result->SetStringField(TEXT("recipe_path"), RecipeFilePath);
		Result->SetObjectField(TEXT("recipe"), Recipe);

		TSharedPtr<FJsonObject> RelevantPrinciples = MakeShared<FJsonObject>();

		TSharedPtr<FJsonObject> TimingSection = MakeShared<FJsonObject>();
		TimingSection->SetStringField(TEXT("source_path"), GetPrinciplesRootDir() / TEXT("timing_rules.json"));
		if (TimingPrinciples.IsValid())
		{
			const TArray<FString> TimingProfiles = CollectTimingProfiles(CanonicalEffectType, Recipe, DescriptionTokens, TimingPrinciples);
			const TSharedPtr<FJsonObject>* ProfilesPtr = nullptr;
			if (TimingPrinciples->TryGetObjectField(TEXT("effect_timing_profiles"), ProfilesPtr) && ProfilesPtr && (*ProfilesPtr).IsValid())
			{
				TSharedPtr<FJsonObject> ChosenProfiles = MakeShared<FJsonObject>();
				for (const FString& ProfileName : TimingProfiles)
				{
					const TSharedPtr<FJsonObject>* ProfileObj = nullptr;
					if ((*ProfilesPtr)->TryGetObjectField(ProfileName, ProfileObj) && ProfileObj && (*ProfileObj).IsValid())
					{
						ChosenProfiles->SetObjectField(ProfileName, *ProfileObj);
					}
				}
				TimingSection->SetObjectField(TEXT("profiles"), ChosenProfiles);
			}

			const TSharedPtr<FJsonObject>* CoreRules = nullptr;
			if (TimingPrinciples->TryGetObjectField(TEXT("core_rules"), CoreRules) && CoreRules && (*CoreRules).IsValid())
			{
				TimingSection->SetObjectField(TEXT("core_rules"), *CoreRules);
			}

			const TSharedPtr<FJsonObject>* Curves = nullptr;
			if (TimingPrinciples->TryGetObjectField(TEXT("curve_guidelines"), Curves) && Curves && (*Curves).IsValid())
			{
				TimingSection->SetObjectField(TEXT("curve_guidelines"), *Curves);
			}
		}
		RelevantPrinciples->SetObjectField(TEXT("timing"), TimingSection);

		TSharedPtr<FJsonObject> ColorSection = MakeShared<FJsonObject>();
		ColorSection->SetStringField(TEXT("source_path"), GetPrinciplesRootDir() / TEXT("color_palettes.json"));
		if (ColorPrinciples.IsValid())
		{
			const TArray<FString> PaletteNames = CollectRelevantPaletteNames(CanonicalEffectType, Recipe, DescriptionTokens, ColorPrinciples);
			const TSharedPtr<FJsonObject>* PalettesPtr = nullptr;
			if (ColorPrinciples->TryGetObjectField(TEXT("palettes"), PalettesPtr) && PalettesPtr && (*PalettesPtr).IsValid())
			{
				TSharedPtr<FJsonObject> ChosenPalettes = MakeShared<FJsonObject>();
				for (const FString& PaletteName : PaletteNames)
				{
					const TSharedPtr<FJsonObject>* PaletteObj = nullptr;
					if ((*PalettesPtr)->TryGetObjectField(PaletteName, PaletteObj) && PaletteObj && (*PaletteObj).IsValid())
					{
						ChosenPalettes->SetObjectField(PaletteName, *PaletteObj);
					}
				}
				ColorSection->SetObjectField(TEXT("palettes"), ChosenPalettes);
			}

			const TSharedPtr<FJsonObject>* CoreRules = nullptr;
			if (ColorPrinciples->TryGetObjectField(TEXT("core_rules"), CoreRules) && CoreRules && (*CoreRules).IsValid())
			{
				ColorSection->SetObjectField(TEXT("core_rules"), *CoreRules);
			}

			const TSharedPtr<FJsonObject>* Patterns = nullptr;
			if (ColorPrinciples->TryGetObjectField(TEXT("color_over_life_patterns"), Patterns) && Patterns && (*Patterns).IsValid())
			{
				ColorSection->SetObjectField(TEXT("color_over_life_patterns"), *Patterns);
			}
		}
		RelevantPrinciples->SetObjectField(TEXT("color"), ColorSection);

		TSharedPtr<FJsonObject> ScaleSection = MakeShared<FJsonObject>();
		ScaleSection->SetStringField(TEXT("source_path"), GetPrinciplesRootDir() / TEXT("scale_reference.json"));
		if (ScalePrinciples.IsValid())
		{
			const TArray<FString> GuidelineNames = CollectRelevantScaleGuidelines(CanonicalEffectType, Recipe, DescriptionTokens, ScalePrinciples);
			const TSharedPtr<FJsonObject>* GuidelinesPtr = nullptr;
			if (ScalePrinciples->TryGetObjectField(TEXT("effect_scale_guidelines"), GuidelinesPtr) && GuidelinesPtr && (*GuidelinesPtr).IsValid())
			{
				TSharedPtr<FJsonObject> ChosenGuidelines = MakeShared<FJsonObject>();
				for (const FString& GuidelineName : GuidelineNames)
				{
					const TSharedPtr<FJsonObject>* GuidelineObj = nullptr;
					if ((*GuidelinesPtr)->TryGetObjectField(GuidelineName, GuidelineObj) && GuidelineObj && (*GuidelineObj).IsValid())
					{
						ChosenGuidelines->SetObjectField(GuidelineName, *GuidelineObj);
					}
				}
				ScaleSection->SetObjectField(TEXT("guidelines"), ChosenGuidelines);
			}

			const TSharedPtr<FJsonObject>* ReferenceSizes = nullptr;
			if (ScalePrinciples->TryGetObjectField(TEXT("reference_sizes"), ReferenceSizes) && ReferenceSizes && (*ReferenceSizes).IsValid())
			{
				ScaleSection->SetObjectField(TEXT("reference_sizes"), *ReferenceSizes);
			}

			const TArray<TSharedPtr<FJsonValue>>* Mistakes = nullptr;
			if (ScalePrinciples->TryGetArrayField(TEXT("common_mistakes"), Mistakes) && Mistakes)
			{
				ScaleSection->SetArrayField(TEXT("common_mistakes"), *Mistakes);
			}
		}
		RelevantPrinciples->SetObjectField(TEXT("scale"), ScaleSection);

		Result->SetObjectField(TEXT("relevant_principles"), RelevantPrinciples);
		return Result;
	}

	static FString PickVariantForDescription(
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& DescriptionTokens)
	{
		const TSharedPtr<FJsonObject>* VariantsPtr = nullptr;
		if (!Recipe.IsValid() || !Recipe->TryGetObjectField(TEXT("variants"), VariantsPtr) || !VariantsPtr || !(*VariantsPtr).IsValid())
		{
			return TEXT("");
		}

		for (const FString& VariantKey : GetObjectKeys(*VariantsPtr))
		{
			const FString VariantNorm = NormalizeKnowledgeToken(VariantKey);
			for (const FString& Token : DescriptionTokens)
			{
				const FString TokenNorm = NormalizeKnowledgeToken(Token);
				if (VariantNorm.Contains(TokenNorm) || TokenNorm.Contains(VariantNorm))
				{
					return VariantKey;
				}
			}
		}

		return TEXT("");
	}

	static FString ResolveEffectTypeFromDescription(
		const FString& Description,
		FString& OutVariant)
	{
		const TArray<FString> AvailableRecipes = FVFXKnowledgeLoader::GetAvailableRecipeTypes();
		const FString DescriptionNorm = NormalizeKnowledgeToken(Description);
		const TArray<FString> Tokens = ExtractDescriptionTokens(Description);
		TSharedPtr<FJsonObject> SynonymConfig;
		FString SynonymError;
		const bool bHasSynonymConfig = LoadSynonymConfig(SynonymConfig, SynonymError);

		for (const FString& EffectType : AvailableRecipes)
		{
			if (DescriptionNorm.Contains(NormalizeKnowledgeToken(EffectType)))
			{
				if (bHasSynonymConfig)
				{
					TryResolveVariantFromSynonymConfig(SynonymConfig, EffectType, DescriptionNorm, OutVariant);
				}
				return EffectType;
			}
		}

		if (bHasSynonymConfig)
		{
			const TSharedPtr<FJsonObject>* EffectTypeSynonymsPtr = nullptr;
			if (SynonymConfig->TryGetObjectField(TEXT("effect_type_synonyms"), EffectTypeSynonymsPtr) &&
				EffectTypeSynonymsPtr && (*EffectTypeSynonymsPtr).IsValid())
			{
				FString MatchedEffectType;
				if (TryResolveFromSynonymMap(*EffectTypeSynonymsPtr, DescriptionNorm, MatchedEffectType))
				{
					TryResolveVariantFromSynonymConfig(SynonymConfig, MatchedEffectType, DescriptionNorm, OutVariant);
					return MatchedEffectType;
				}
			}
		}

		for (const FString& EffectType : AvailableRecipes)
		{
			FString RecipePath;
			FString CanonicalType;
			if (!FindRecipeTypeDirectory(EffectType, CanonicalType, RecipePath, nullptr))
			{
				continue;
			}

			TSharedPtr<FJsonObject> Recipe;
			FString LoadError;
			if (!LoadJsonFile(RecipePath, Recipe, LoadError))
			{
				continue;
			}

			const FString Variant = PickVariantForDescription(Recipe, Tokens);
			if (!Variant.IsEmpty())
			{
				OutVariant = Variant;
				return EffectType;
			}
		}

		return TEXT("");
	}

	static bool HasDescriptionToken(const TArray<FString>& DescriptionTokens, const TCHAR* Token)
	{
		const FString Wanted = NormalizeKnowledgeToken(Token);
		for (const FString& DescriptionToken : DescriptionTokens)
		{
			if (NormalizeKnowledgeToken(DescriptionToken) == Wanted)
			{
				return true;
			}
		}
		return false;
	}

	static int32 GetCoreEnergyLayerPriorityWeight(
		const FString& LayerName,
		const FString& VariantName,
		const TArray<FString>& DescriptionTokens)
	{
		const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
		const FString VariantNorm = NormalizeKnowledgeToken(VariantName);
		const bool bChargingIntent =
			VariantNorm == TEXT("chargingcore") ||
			HasDescriptionToken(DescriptionTokens, TEXT("charging")) ||
			HasDescriptionToken(DescriptionTokens, TEXT("charge")) ||
			HasDescriptionToken(DescriptionTokens, TEXT("chargingcore"));

		if (LayerNorm.Contains(TEXT("filament")) || LayerNorm.Contains(TEXT("arc")))
		{
			return bChargingIntent ? 300 : 240;
		}

		if (LayerNorm.Contains(TEXT("core")))
		{
			return bChargingIntent ? 290 : 260;
		}

		if (LayerNorm.Contains(TEXT("shell")) || LayerNorm.Contains(TEXT("plasma")) || LayerNorm.Contains(TEXT("body")))
		{
			return bChargingIntent ? 280 : 220;
		}

		if (LayerNorm.Contains(TEXT("spark")))
		{
			return bChargingIntent ? 190 : 120;
		}

		if (LayerNorm.Contains(TEXT("turbulence")) || LayerNorm.Contains(TEXT("depth")))
		{
			return bChargingIntent ? 170 : 140;
		}

		if (LayerNorm.Contains(TEXT("rim")))
		{
			return bChargingIntent ? 130 : 160;
		}

		return 100;
	}

	static TArray<FString> SelectEmitterLayersForDescription(
		const FString& EffectType,
		const FString& VariantName,
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& DescriptionTokens)
	{
		TArray<FString> Result;
		const TSharedPtr<FJsonObject>* BaseArchitecturePtr = nullptr;
		if (!Recipe.IsValid() ||
			!Recipe->TryGetObjectField(TEXT("base_architecture"), BaseArchitecturePtr) ||
			!BaseArchitecturePtr || !(*BaseArchitecturePtr).IsValid())
		{
			return Result;
		}

		const TArray<TSharedPtr<FJsonValue>>* LayersPtr = nullptr;
		if (!(*BaseArchitecturePtr)->TryGetArrayField(TEXT("emitter_layers"), LayersPtr) || !LayersPtr)
		{
			return Result;
		}

		for (const TSharedPtr<FJsonValue>& LayerValue : *LayersPtr)
		{
			const TSharedPtr<FJsonObject>* LayerPtr = nullptr;
			if (!LayerValue.IsValid() || !LayerValue->TryGetObject(LayerPtr) || !LayerPtr || !(*LayerPtr).IsValid())
			{
				continue;
			}

			FString LayerName;
			FString Priority;
			FString Purpose;
			(*LayerPtr)->TryGetStringField(TEXT("name"), LayerName);
			(*LayerPtr)->TryGetStringField(TEXT("priority"), Priority);
			(*LayerPtr)->TryGetStringField(TEXT("purpose"), Purpose);

			const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
			const FString PurposeNorm = NormalizeKnowledgeToken(Purpose);
			const FString PriorityNorm = NormalizeKnowledgeToken(Priority);

			bool bInclude = PriorityNorm == TEXT("required") || PriorityNorm == TEXT("recommended");
			for (const FString& Token : DescriptionTokens)
			{
				const FString TokenNorm = NormalizeKnowledgeToken(Token);
				if (LayerNorm.Contains(TokenNorm) || PurposeNorm.Contains(TokenNorm))
				{
					bInclude = true;
					break;
				}
			}

			if (bInclude && !LayerName.IsEmpty())
			{
				Result.Add(LayerName);
			}
		}

		if (NormalizeKnowledgeToken(EffectType) == TEXT("coreenergy"))
		{
			Result.Sort([&](const FString& A, const FString& B)
			{
				const int32 WeightA = GetCoreEnergyLayerPriorityWeight(A, VariantName, DescriptionTokens);
				const int32 WeightB = GetCoreEnergyLayerPriorityWeight(B, VariantName, DescriptionTokens);
				if (WeightA != WeightB)
				{
					return WeightA > WeightB;
				}
				return A < B;
			});
		}

		return Result;
	}

	static TArray<TSharedPtr<FJsonValue>> MakeNumberArray(const TArray<double>& Values)
	{
		TArray<TSharedPtr<FJsonValue>> Result;
		for (double Value : Values)
		{
			Result.Add(MakeShared<FJsonValueNumber>(Value));
		}
		return Result;
	}

	static bool TryReadColorTriple(
		const TSharedPtr<FJsonObject>& Source,
		const FString& FieldName,
		TArray<double>& OutColor)
	{
		const TSharedPtr<FJsonObject>* ColorPtr = nullptr;
		if (!Source.IsValid() || !Source->TryGetObjectField(FieldName, ColorPtr) || !ColorPtr || !(*ColorPtr).IsValid())
		{
			return false;
		}

		double R = 1.0;
		double G = 1.0;
		double B = 1.0;
		(*ColorPtr)->TryGetNumberField(TEXT("r"), R);
		(*ColorPtr)->TryGetNumberField(TEXT("g"), G);
		(*ColorPtr)->TryGetNumberField(TEXT("b"), B);
		OutColor = { R, G, B };
		return true;
	}

	static TArray<double> BuildFallbackLayerColor(
		const FString& EffectType,
		const FString& LayerName,
		const TSharedPtr<FJsonObject>& RecipeBundle)
	{
		const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
		TArray<double> DefaultColor = { 1.0, 0.5, 0.1 };

		const TSharedPtr<FJsonObject>* PrinciplesPtr = nullptr;
		if (!RecipeBundle.IsValid() ||
			!RecipeBundle->TryGetObjectField(TEXT("relevant_principles"), PrinciplesPtr) ||
			!PrinciplesPtr || !(*PrinciplesPtr).IsValid())
		{
			return DefaultColor;
		}

		const TSharedPtr<FJsonObject>* ColorPtr = nullptr;
		if (!(*PrinciplesPtr)->TryGetObjectField(TEXT("color"), ColorPtr) || !ColorPtr || !(*ColorPtr).IsValid())
		{
			return DefaultColor;
		}

		const TSharedPtr<FJsonObject>* PalettesPtr = nullptr;
		if (!(*ColorPtr)->TryGetObjectField(TEXT("palettes"), PalettesPtr) || !PalettesPtr || !(*PalettesPtr).IsValid())
		{
			return DefaultColor;
		}

		auto ResolveFromPalette = [&](const FString& PaletteName, const TArray<FString>& CandidateFields) -> bool
		{
			const TSharedPtr<FJsonObject>* PaletteObj = nullptr;
			if (!(*PalettesPtr)->TryGetObjectField(PaletteName, PaletteObj) || !PaletteObj || !(*PaletteObj).IsValid())
			{
				return false;
			}

			for (const FString& FieldName : CandidateFields)
			{
				if (TryReadColorTriple(*PaletteObj, FieldName, DefaultColor))
				{
					return true;
				}
			}
			return false;
		};

		if (LayerNorm.Contains(TEXT("smoke")))
		{
			ResolveFromPalette(TEXT("smoke_dark"), { TEXT("start"), TEXT("end") });
			return DefaultColor;
		}

		if (LayerNorm.Contains(TEXT("ember")))
		{
			if (ResolveFromPalette(TEXT("fire_natural"), { TEXT("embers"), TEXT("mid") }))
			{
				return DefaultColor;
			}
		}

		if (NormalizeKnowledgeToken(EffectType) == TEXT("coreenergy"))
		{
			if (LayerNorm.Contains(TEXT("spark")))
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("spark"), TEXT("filament"), TEXT("body") });
			}
			else if (LayerNorm.Contains(TEXT("filament")) || LayerNorm.Contains(TEXT("arc")))
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("filament"), TEXT("spark"), TEXT("core") });
			}
			else if (LayerNorm.Contains(TEXT("rim")))
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("rim"), TEXT("body") });
			}
			else if (LayerNorm.Contains(TEXT("shell")) || LayerNorm.Contains(TEXT("body")) || LayerNorm.Contains(TEXT("plasma")) || LayerNorm.Contains(TEXT("turbulence")))
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("body"), TEXT("rim"), TEXT("filament") });
			}
			else if (LayerNorm.Contains(TEXT("core")))
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("core"), TEXT("filament") });
			}
			else
			{
				ResolveFromPalette(TEXT("core_energy"), { TEXT("body"), TEXT("core") });
			}
			return DefaultColor;
		}

		if (NormalizeKnowledgeToken(EffectType) == TEXT("explosion"))
		{
			if (LayerNorm.Contains(TEXT("flash")))
			{
				ResolveFromPalette(TEXT("explosion"), { TEXT("flash") });
			}
			else if (LayerNorm.Contains(TEXT("smoke")))
			{
				ResolveFromPalette(TEXT("explosion"), { TEXT("smoke_hot"), TEXT("smoke_cool") });
			}
			else
			{
				ResolveFromPalette(TEXT("explosion"), { TEXT("fireball") });
			}
			return DefaultColor;
		}

		if (LayerNorm.Contains(TEXT("outer")))
		{
			ResolveFromPalette(TEXT("fire_natural"), { TEXT("outer"), TEXT("mid") });
			return DefaultColor;
		}

		if (LayerNorm.Contains(TEXT("core")))
		{
			ResolveFromPalette(TEXT("fire_natural"), { TEXT("core"), TEXT("mid") });
			return DefaultColor;
		}

		ResolveFromPalette(TEXT("fire_natural"), { TEXT("mid"), TEXT("core") });
		return DefaultColor;
	}

	static const TSharedPtr<FJsonObject>* FindRecipeLayerByName(
		const TSharedPtr<FJsonObject>& Recipe,
		const FString& LayerName)
	{
		const TSharedPtr<FJsonObject>* BaseArchitecturePtr = nullptr;
		if (!Recipe.IsValid() ||
			!Recipe->TryGetObjectField(TEXT("base_architecture"), BaseArchitecturePtr) ||
			!BaseArchitecturePtr || !(*BaseArchitecturePtr).IsValid())
		{
			return nullptr;
		}

		const TArray<TSharedPtr<FJsonValue>>* LayersPtr = nullptr;
		if (!(*BaseArchitecturePtr)->TryGetArrayField(TEXT("emitter_layers"), LayersPtr) || !LayersPtr)
		{
			return nullptr;
		}

		for (const TSharedPtr<FJsonValue>& LayerValue : *LayersPtr)
		{
			const TSharedPtr<FJsonObject>* LayerPtr = nullptr;
			if (!LayerValue.IsValid() || !LayerValue->TryGetObject(LayerPtr) || !LayerPtr || !(*LayerPtr).IsValid())
			{
				continue;
			}

			FString CandidateName;
			(*LayerPtr)->TryGetStringField(TEXT("name"), CandidateName);
			if (CandidateName.Equals(LayerName, ESearchCase::IgnoreCase))
			{
				return LayerPtr;
			}
		}

		return nullptr;
	}

	static TArray<TSharedPtr<FJsonValue>> MakeStringArray(const TArray<FString>& Values)
	{
		TArray<TSharedPtr<FJsonValue>> Result;
		for (const FString& Value : Values)
		{
			Result.Add(MakeShared<FJsonValueString>(Value));
		}
		return Result;
	}

	static FString InferTemplateArchetype(
		const FString& EffectType,
		const FString& LayerName,
		const TSharedPtr<FJsonObject>& LayerObject)
	{
		const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
		FString Renderer;
		if (LayerObject.IsValid())
		{
			LayerObject->TryGetStringField(TEXT("renderer"), Renderer);
		}
		const FString RendererNorm = NormalizeKnowledgeToken(Renderer);
		const FString EffectNorm = NormalizeKnowledgeToken(EffectType);

		if (RendererNorm.Contains(TEXT("ribbon")) || LayerNorm.Contains(TEXT("trail")) || LayerNorm.Contains(TEXT("ribbon")))
		{
			return TEXT("ribbon_trail");
		}

		if (RendererNorm.Contains(TEXT("mesh")) || LayerNorm.Contains(TEXT("debris")) || LayerNorm.Contains(TEXT("chunk")))
		{
			return TEXT("mesh_debris");
		}

		if (LayerNorm.Contains(TEXT("smoke")) || LayerNorm.Contains(TEXT("fog")) || LayerNorm.Contains(TEXT("steam")) || LayerNorm.Contains(TEXT("wisp")))
		{
			return TEXT("smoke_billow");
		}

		if (LayerNorm.Contains(TEXT("ember")) || LayerNorm.Contains(TEXT("spark")))
		{
			return TEXT("spark_burst");
		}

		if (LayerNorm.Contains(TEXT("burst")) || LayerNorm.Contains(TEXT("shockwave")) || EffectNorm == TEXT("impact") || EffectNorm == TEXT("explosion"))
		{
			return TEXT("sprite_burst");
		}

		return TEXT("sprite_fountain_loop");
	}

	static TSharedPtr<FJsonObject> BuildLayerAuthoringGuidance(
		const FString& EffectType,
		const FString& VariantName,
		const TArray<FString>& DescriptionTokens,
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& SelectedLayers)
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> LayerGuidanceArray;

		for (const FString& LayerName : SelectedLayers)
		{
			const TSharedPtr<FJsonObject>* LayerPtr = FindRecipeLayerByName(Recipe, LayerName);
			if (!LayerPtr || !(*LayerPtr).IsValid())
			{
				continue;
			}

			const TSharedPtr<FJsonObject>& Layer = *LayerPtr;
			TSharedPtr<FJsonObject> LayerGuidance = MakeShared<FJsonObject>();
			LayerGuidance->SetStringField(TEXT("layer_name"), LayerName);

			FString Purpose;
			FString Priority;
			FString Renderer;
			FString MaterialType;
			Layer->TryGetStringField(TEXT("purpose"), Purpose);
			Layer->TryGetStringField(TEXT("priority"), Priority);
			Layer->TryGetStringField(TEXT("renderer"), Renderer);
			Layer->TryGetStringField(TEXT("material_type"), MaterialType);

			if (!Purpose.IsEmpty())
			{
				LayerGuidance->SetStringField(TEXT("purpose"), Purpose);
			}
			if (!Priority.IsEmpty())
			{
				LayerGuidance->SetStringField(TEXT("priority"), Priority);
			}
			if (!Renderer.IsEmpty())
			{
				LayerGuidance->SetStringField(TEXT("renderer"), Renderer);
			}
			if (!MaterialType.IsEmpty())
			{
				LayerGuidance->SetStringField(TEXT("material_type"), MaterialType);
			}

			LayerGuidance->SetStringField(TEXT("preferred_template_archetype"), InferTemplateArchetype(EffectType, LayerName, Layer));
			LayerGuidance->SetNumberField(TEXT("selection_rank"), LayerGuidanceArray.Num() + 1);

			TArray<FString> ControlFields;
			const TArray<FString> CandidateFields = {
				TEXT("spawn_rate"),
				TEXT("particle_lifetime"),
				TEXT("initial_size"),
				TEXT("size_over_life"),
				TEXT("velocity"),
				TEXT("color_over_life"),
				TEXT("color"),
				TEXT("opacity_over_life"),
				TEXT("drag"),
				TEXT("curl_noise"),
				TEXT("rotation"),
				TEXT("notes")
			};

			TSharedPtr<FJsonObject> RecipeControls = MakeShared<FJsonObject>();
			for (const FString& FieldName : CandidateFields)
			{
				FString Value;
				if (Layer->TryGetStringField(FieldName, Value) && !Value.IsEmpty())
				{
					RecipeControls->SetStringField(FieldName, Value);
					ControlFields.Add(FieldName);
				}
			}

			if (Layer->HasTypedField<EJson::Number>(TEXT("sort_order")))
			{
				RecipeControls->SetNumberField(TEXT("sort_order"), Layer->GetNumberField(TEXT("sort_order")));
				ControlFields.Add(TEXT("sort_order"));
			}

			LayerGuidance->SetArrayField(TEXT("recipe_control_fields"), MakeStringArray(ControlFields));
			LayerGuidance->SetObjectField(TEXT("recipe_controls"), RecipeControls);

			TArray<FString> RecommendedTools = {
				TEXT("get_niagara_emitter_stack"),
				TEXT("get_niagara_module_inputs"),
				TEXT("set_niagara_spawn_rate"),
				TEXT("set_niagara_lifetime"),
				TEXT("set_niagara_sprite_size"),
				TEXT("set_niagara_color_over_life"),
				TEXT("set_niagara_opacity_over_life"),
				TEXT("set_niagara_velocity"),
				TEXT("compile_niagara_system")
			};

			const FString LayerNorm = NormalizeKnowledgeToken(LayerName);
			if (LayerNorm.Contains(TEXT("smoke")) || LayerNorm.Contains(TEXT("ember")) || LayerNorm.Contains(TEXT("spark")))
			{
				RecommendedTools.Add(TEXT("set_niagara_drag"));
				RecommendedTools.Add(TEXT("set_niagara_curl_noise_force"));
			}
			if (LayerNorm.Contains(TEXT("smoke")) || LayerNorm.Contains(TEXT("dust")) || LayerNorm.Contains(TEXT("fog")))
			{
				RecommendedTools.Add(TEXT("set_niagara_gravity_force"));
			}

			if (NormalizeKnowledgeToken(EffectType) == TEXT("coreenergy"))
			{
				const int32 CoreEnergyWeight = GetCoreEnergyLayerPriorityWeight(LayerName, VariantName, DescriptionTokens);
				const bool bHeroLayer = CoreEnergyWeight >= 220;
				LayerGuidance->SetStringField(TEXT("authoring_priority"), bHeroLayer ? TEXT("hero") : TEXT("support"));
				LayerGuidance->SetNumberField(TEXT("authoring_priority_weight"), CoreEnergyWeight);

				if (LayerNorm.Contains(TEXT("filament")) || LayerNorm.Contains(TEXT("arc")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Prioritize readable thin swirling filaments before polishing secondary support layers."));
					RecommendedTools.Insert(TEXT("set_niagara_curl_noise_force"), 0);
					RecommendedTools.Insert(TEXT("set_niagara_velocity"), 0);
					RecommendedTools.Insert(TEXT("set_niagara_lifetime"), 0);
				}
				else if (LayerNorm.Contains(TEXT("core")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Lock the white-hot center size and intensity early so later shell and filament work cannot flatten the read."));
					RecommendedTools.Insert(TEXT("set_niagara_sprite_size"), 0);
					RecommendedTools.Insert(TEXT("set_niagara_color_over_life"), 0);
				}
				else if (LayerNorm.Contains(TEXT("shell")) || LayerNorm.Contains(TEXT("plasma")) || LayerNorm.Contains(TEXT("body")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Use this layer to carry warm spherical volume without obscuring the core or filament structure."));
					RecommendedTools.Insert(TEXT("set_niagara_opacity_over_life"), 0);
					RecommendedTools.Insert(TEXT("set_niagara_sprite_size"), 0);
				}
				else if (LayerNorm.Contains(TEXT("spark")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Treat sparks as surge punctuation only; keep them sparse and subordinate to the hero orb read."));
				}
				else if (LayerNorm.Contains(TEXT("turbulence")) || LayerNorm.Contains(TEXT("depth")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Keep turbulence behind the hero layers and lower contrast than the filament arcs."));
				}
				else if (LayerNorm.Contains(TEXT("rim")))
				{
					LayerGuidance->SetStringField(TEXT("authoring_focus"), TEXT("Use the rim only to reinforce containment. Avoid turning it into a bright donut outline."));
				}

				if (NormalizeKnowledgeToken(VariantName) == TEXT("chargingcore") ||
					HasDescriptionToken(DescriptionTokens, TEXT("charging")) ||
					HasDescriptionToken(DescriptionTokens, TEXT("charge")))
				{
					if (LayerNorm.Contains(TEXT("filament")) || LayerNorm.Contains(TEXT("core")) || LayerNorm.Contains(TEXT("shell")) || LayerNorm.Contains(TEXT("plasma")))
					{
						LayerGuidance->SetStringField(TEXT("charging_priority"), TEXT("high"));
					}
					else
					{
						LayerGuidance->SetStringField(TEXT("charging_priority"), TEXT("medium"));
					}
				}
			}

			LayerGuidance->SetArrayField(TEXT("recommended_tools"), MakeStringArray(RecommendedTools));
			LayerGuidanceArray.Add(MakeShared<FJsonValueObject>(LayerGuidance));
		}

		Result->SetArrayField(TEXT("layers"), LayerGuidanceArray);
		return Result;
	}

	static TSharedPtr<FJsonObject> BuildAuthoringStrategy(
		const FString& EffectType,
		const FString& VariantName,
		const TArray<FString>& DescriptionTokens,
		const TSharedPtr<FJsonObject>& WorkflowPattern,
		const TSharedPtr<FJsonObject>& TemplateArchetypes,
		const TSharedPtr<FJsonObject>& Recipe,
		const TArray<FString>& SelectedLayers)
	{
		TSharedPtr<FJsonObject> Strategy = MakeShared<FJsonObject>();
		Strategy->SetStringField(TEXT("effect_type"), EffectType);
		if (!VariantName.IsEmpty())
		{
			Strategy->SetStringField(TEXT("variant"), VariantName);
		}

		Strategy->SetStringField(TEXT("preferred_bootstrap"), TEXT("empty_system_then_template_emitters"));
		Strategy->SetStringField(TEXT("system_creation_tool"), TEXT("create_empty_niagara_system"));
		Strategy->SetStringField(TEXT("template_emitter_tool"), TEXT("add_niagara_emitter_to_system"));
		Strategy->SetStringField(TEXT("empty_emitter_fallback_tool"), TEXT("add_empty_niagara_emitter"));
		Strategy->SetStringField(TEXT("inspection_tool"), TEXT("get_niagara_module_inputs"));
		Strategy->SetStringField(TEXT("stack_review_tool"), TEXT("get_niagara_emitter_stack"));
		Strategy->SetStringField(TEXT("compile_tool"), TEXT("compile_niagara_system"));
		Strategy->SetStringField(TEXT("preview_tool"), TEXT("spawn_niagara_at_location"));
		Strategy->SetObjectField(TEXT("layer_authoring_guidance"), BuildLayerAuthoringGuidance(EffectType, VariantName, DescriptionTokens, Recipe, SelectedLayers));

		if (WorkflowPattern.IsValid())
		{
			FString PreferredBootstrap;
			if (WorkflowPattern->TryGetStringField(TEXT("preferred_bootstrap"), PreferredBootstrap) && !PreferredBootstrap.IsEmpty())
			{
				Strategy->SetStringField(TEXT("preferred_bootstrap"), PreferredBootstrap);
			}

			const TArray<TSharedPtr<FJsonValue>>* BootstrapSequence = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("bootstrap_sequence"), BootstrapSequence) && BootstrapSequence)
			{
				Strategy->SetArrayField(TEXT("bootstrap_sequence"), *BootstrapSequence);
			}

			const TArray<TSharedPtr<FJsonValue>>* AuthoringLoop = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("authoring_loop"), AuthoringLoop) && AuthoringLoop)
			{
				Strategy->SetArrayField(TEXT("authoring_loop"), *AuthoringLoop);
			}

			const TArray<TSharedPtr<FJsonValue>>* VerificationLoop = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("verification_loop"), VerificationLoop) && VerificationLoop)
			{
				Strategy->SetArrayField(TEXT("verification_loop"), *VerificationLoop);
			}

			const TArray<TSharedPtr<FJsonValue>>* Guardrails = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("guardrails"), Guardrails) && Guardrails)
			{
				Strategy->SetArrayField(TEXT("guardrails"), *Guardrails);
			}

			const TArray<TSharedPtr<FJsonValue>>* ArtistControlOrder = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("artist_control_order"), ArtistControlOrder) && ArtistControlOrder)
			{
				Strategy->SetArrayField(TEXT("artist_control_order"), *ArtistControlOrder);
			}

			const TArray<TSharedPtr<FJsonValue>>* AdvancedCapabilities = nullptr;
			if (WorkflowPattern->TryGetArrayField(TEXT("advanced_capabilities"), AdvancedCapabilities) && AdvancedCapabilities)
			{
				Strategy->SetArrayField(TEXT("advanced_capabilities"), *AdvancedCapabilities);
			}
		}

		if (TemplateArchetypes.IsValid())
		{
			const TSharedPtr<FJsonObject>* ArchetypesPtr = nullptr;
			if (TemplateArchetypes->TryGetObjectField(TEXT("archetypes"), ArchetypesPtr) && ArchetypesPtr && (*ArchetypesPtr).IsValid())
			{
				TSharedPtr<FJsonObject> SelectedArchetypes = MakeShared<FJsonObject>();
				for (const FString& LayerName : SelectedLayers)
				{
					const TSharedPtr<FJsonObject>* LayerPtr = FindRecipeLayerByName(Recipe, LayerName);
					TSharedPtr<FJsonObject> LayerObject;
					if (LayerPtr)
					{
						LayerObject = *LayerPtr;
					}
					const FString ArchetypeKey = InferTemplateArchetype(EffectType, LayerName, LayerObject);
					const TSharedPtr<FJsonObject>* ArchetypeObj = nullptr;
					if ((*ArchetypesPtr)->TryGetObjectField(ArchetypeKey, ArchetypeObj) && ArchetypeObj && (*ArchetypeObj).IsValid())
					{
						SelectedArchetypes->SetObjectField(LayerName, *ArchetypeObj);
					}
				}
				Strategy->SetObjectField(TEXT("selected_template_archetypes"), SelectedArchetypes);
			}
		}

		return Strategy;
	}

	static TSharedPtr<FJsonObject> MakePlanInput(std::initializer_list<TPair<FString, FString>> Pairs)
	{
		TSharedPtr<FJsonObject> Input = MakeShared<FJsonObject>();
		for (const TPair<FString, FString>& Pair : Pairs)
		{
			Input->SetStringField(Pair.Key, Pair.Value);
		}
		return Input;
	}

	static TSharedPtr<FJsonObject> MakePlanStep(
		int32 Order,
		const FString& ToolName,
		const TSharedPtr<FJsonObject>& Input,
		const FString& Intent,
		const FString& When = TEXT(""))
	{
		TSharedPtr<FJsonObject> Step = MakeShared<FJsonObject>();
		Step->SetNumberField(TEXT("order"), Order);
		Step->SetStringField(TEXT("tool_name"), ToolName);
		Step->SetObjectField(TEXT("input"), Input.IsValid() ? Input : MakeShared<FJsonObject>());
		Step->SetStringField(TEXT("intent"), Intent);
		if (!When.IsEmpty())
		{
			Step->SetStringField(TEXT("when"), When);
		}
		return Step;
	}

	static FString SanitizeNameToken(const FString& Value)
	{
		FString Result;
		for (TCHAR Char : Value)
		{
			if (FChar::IsAlnum(Char))
			{
				Result.AppendChar(Char);
			}
		}
		return Result;
	}

	static FString GetEffectKnowledgeDir(const FString& EffectType)
	{
		return GetEffectsRootDir() / EffectType;
	}

	static bool LoadV2Sidecar(
		const FString& EffectType,
		const FString& FileName,
		TSharedPtr<FJsonObject>& OutObject,
		FString& OutErrorMessage)
	{
		const FString FilePath = GetEffectKnowledgeDir(EffectType) / FileName;
		if (!LoadJsonFile(FilePath, OutObject, OutErrorMessage))
		{
			OutErrorMessage = FString::Printf(TEXT("Failed to load V2 sidecar '%s/%s': %s"), *EffectType, *FileName, *OutErrorMessage);
			return false;
		}
		return true;
	}

	struct FV2ArchetypeSelection
	{
		FString EffectType;
		FString ArchetypeName;
		FString ArchetypeFileName;
		FString TaxonomyFileName;
	};

	static bool ResolveExplicitV2ArchetypeSelection(const FString& ExplicitArchetype, FV2ArchetypeSelection& OutSelection)
	{
		if (ExplicitArchetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("CoreEnergy"), TEXT("suction_core_energy"), TEXT("suction_core_energy_archetype.json"), TEXT("suction_core_failure_taxonomy.json") };
			return true;
		}
		if (ExplicitArchetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("Electricity"), TEXT("electric_arc_orb"), TEXT("electric_arc_orb_archetype.json"), TEXT("electric_arc_orb_failure_taxonomy.json") };
			return true;
		}
		if (ExplicitArchetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("Fire"), TEXT("hero_fire_orb_sweep"), TEXT("hero_fire_orb_sweep_archetype.json"), TEXT("hero_fire_orb_sweep_failure_taxonomy.json") };
			return true;
		}
		if (ExplicitArchetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("Magic"), TEXT("arcane_pull_orb"), TEXT("arcane_pull_orb_archetype.json"), TEXT("arcane_pull_orb_failure_taxonomy.json") };
			return true;
		}
		if (ExplicitArchetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("Impact"), TEXT("impact_pulse_core"), TEXT("impact_pulse_core_archetype.json"), TEXT("impact_pulse_core_failure_taxonomy.json") };
			return true;
		}
		if (ExplicitArchetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			OutSelection = { TEXT("Explosion"), TEXT("explosion_flash_core"), TEXT("explosion_flash_core_archetype.json"), TEXT("explosion_flash_core_failure_taxonomy.json") };
			return true;
		}
		return false;
	}

	static bool TokenSetContainsAny(const TSet<FString>& TokenSet, std::initializer_list<const TCHAR*> Tokens)
	{
		for (const TCHAR* Token : Tokens)
		{
			if (TokenSet.Contains(Token))
			{
				return true;
			}
		}
		return false;
	}

	static bool ResolveV2ArchetypeSelection(const FString& Description, FV2ArchetypeSelection& OutSelection)
	{
		const TArray<FString> Tokens = ExtractDescriptionTokens(Description);
		const TSet<FString> TokenSet(Tokens);

		const bool bHasCoreShape = TokenSetContainsAny(TokenSet, {
			TEXT("core"), TEXT("orb"), TEXT("sphere"), TEXT("ball"), TEXT("center"), TEXT("centre")
		});
		const bool bHasPull = TokenSetContainsAny(TokenSet, {
			TEXT("suction"), TEXT("suck"), TEXT("pull"), TEXT("pulled"), TEXT("pulling"),
			TEXT("gather"), TEXT("gathering"), TEXT("attract"), TEXT("attraction"),
			TEXT("draw"), TEXT("drawn"), TEXT("drawing"), TEXT("implosion")
		});
		const bool bHasMagic = TokenSetContainsAny(TokenSet, {
			TEXT("magic"), TEXT("magical"), TEXT("arcane"), TEXT("mystic"),
			TEXT("sorcery"), TEXT("sorcerous"), TEXT("eldritch"), TEXT("rune"), TEXT("runic")
		});
		const bool bHasElectricity = TokenSetContainsAny(TokenSet, {
			TEXT("electric"), TEXT("electricity"), TEXT("lightning"), TEXT("arc"), TEXT("arcs"),
			TEXT("tesla"), TEXT("shocked"), TEXT("static"), TEXT("ion"), TEXT("ionic")
		});
		const bool bHasFire = TokenSetContainsAny(TokenSet, {
			TEXT("fire"), TEXT("flame"), TEXT("fiery"), TEXT("infernal"), TEXT("ember"), TEXT("embers"), TEXT("molten")
		});
		const bool bHasExplosion = TokenSetContainsAny(TokenSet, {
			TEXT("explosion"), TEXT("explosive"), TEXT("blast"), TEXT("detonation"), TEXT("boom"), TEXT("grenade"), TEXT("fireball")
		});
		const bool bHasImpact = TokenSetContainsAny(TokenSet, {
			TEXT("impact"), TEXT("hit"), TEXT("strike"), TEXT("slam"), TEXT("burst"), TEXT("pulse"),
			TEXT("collision"), TEXT("shockwave"), TEXT("flash")
		});
		const bool bHasEnergy = TokenSetContainsAny(TokenSet, {
			TEXT("energy"), TEXT("plasma"), TEXT("charge"), TEXT("charged"), TEXT("charging"), TEXT("orb")
		});

		if (bHasMagic && (bHasCoreShape || bHasPull))
		{
			OutSelection = { TEXT("Magic"), TEXT("arcane_pull_orb"), TEXT("arcane_pull_orb_archetype.json"), TEXT("arcane_pull_orb_failure_taxonomy.json") };
			return true;
		}

		if (bHasElectricity && (bHasCoreShape || bHasPull || bHasEnergy))
		{
			OutSelection = { TEXT("Electricity"), TEXT("electric_arc_orb"), TEXT("electric_arc_orb_archetype.json"), TEXT("electric_arc_orb_failure_taxonomy.json") };
			return true;
		}

		if (bHasFire && bHasCoreShape && !bHasPull)
		{
			OutSelection = { TEXT("Fire"), TEXT("hero_fire_orb_sweep"), TEXT("hero_fire_orb_sweep_archetype.json"), TEXT("hero_fire_orb_sweep_failure_taxonomy.json") };
			return true;
		}

		if (bHasExplosion && (bHasCoreShape || bHasEnergy))
		{
			OutSelection = { TEXT("Explosion"), TEXT("explosion_flash_core"), TEXT("explosion_flash_core_archetype.json"), TEXT("explosion_flash_core_failure_taxonomy.json") };
			return true;
		}

		if (bHasImpact && (bHasCoreShape || bHasEnergy))
		{
			OutSelection = { TEXT("Impact"), TEXT("impact_pulse_core"), TEXT("impact_pulse_core_archetype.json"), TEXT("impact_pulse_core_failure_taxonomy.json") };
			return true;
		}

		if ((bHasPull && (bHasCoreShape || bHasEnergy)) || (bHasFire && bHasPull))
		{
			OutSelection = { TEXT("CoreEnergy"), TEXT("suction_core_energy"), TEXT("suction_core_energy_archetype.json"), TEXT("suction_core_failure_taxonomy.json") };
			return true;
		}

		return false;
	}

	static TArray<FString> ExtractRequiredLayerNames(const TSharedPtr<FJsonObject>& Archetype)
	{
		TArray<FString> LayerNames;
		if (!Archetype.IsValid())
		{
			return LayerNames;
		}

		const TArray<TSharedPtr<FJsonValue>>* RequiredLayers = nullptr;
		if (!Archetype->TryGetArrayField(TEXT("required_layers"), RequiredLayers) || !RequiredLayers)
		{
			return LayerNames;
		}

		for (const TSharedPtr<FJsonValue>& LayerValue : *RequiredLayers)
		{
			const TSharedPtr<FJsonObject> LayerObject = LayerValue.IsValid() ? LayerValue->AsObject() : nullptr;
			FString LayerName;
			if (LayerObject.IsValid() && LayerObject->TryGetStringField(TEXT("name"), LayerName) && !LayerName.IsEmpty())
			{
				LayerNames.Add(LayerName);
			}
		}

		return LayerNames;
	}

	static TSharedPtr<FJsonObject> BuildV2LayerPlan(const TSharedPtr<FJsonObject>& Archetype)
	{
		TSharedPtr<FJsonObject> LayerPlan = MakeShared<FJsonObject>();
		if (!Archetype.IsValid())
		{
			return LayerPlan;
		}

		const TArray<TSharedPtr<FJsonValue>>* RequiredLayers = nullptr;
		if (!Archetype->TryGetArrayField(TEXT("required_layers"), RequiredLayers) || !RequiredLayers)
		{
			return LayerPlan;
		}

		for (const TSharedPtr<FJsonValue>& LayerValue : *RequiredLayers)
		{
			const TSharedPtr<FJsonObject> LayerObject = LayerValue.IsValid() ? LayerValue->AsObject() : nullptr;
			if (!LayerObject.IsValid())
			{
				continue;
			}

			FString LayerName;
			if (LayerObject->TryGetStringField(TEXT("name"), LayerName) && !LayerName.IsEmpty())
			{
				LayerPlan->SetObjectField(LayerName, LayerObject);
			}
		}

		return LayerPlan;
	}

	static TArray<TSharedPtr<FJsonValue>> BuildV2SemanticOpsPlan(const TSharedPtr<FJsonObject>& Archetype)
	{
		TArray<TSharedPtr<FJsonValue>> Steps;
		if (!Archetype.IsValid())
		{
			return Steps;
		}

		const TArray<TSharedPtr<FJsonValue>>* Ops = nullptr;
		if (!Archetype->TryGetArrayField(TEXT("default_semantic_ops_plan"), Ops) || !Ops)
		{
			return Steps;
		}

		int32 Order = 1;
		for (const TSharedPtr<FJsonValue>& Value : *Ops)
		{
			FString OpName;
			if (!Value.IsValid() || !Value->TryGetString(OpName) || OpName.IsEmpty())
			{
				continue;
			}

			TSharedPtr<FJsonObject> Step = MakeShared<FJsonObject>();
			Step->SetNumberField(TEXT("order"), Order++);
			Step->SetStringField(TEXT("op_name"), OpName);
			Step->SetStringField(TEXT("family"),
				OpName.Contains(TEXT("material")) || OpName.Contains(TEXT("flow")) || OpName.Contains(TEXT("opacity"))
					? TEXT("material")
					: (OpName.Contains(TEXT("review")) ? TEXT("critic") : TEXT("niagara")));
			Steps.Add(MakeShared<FJsonValueObject>(Step));
		}

		return Steps;
	}

	static TSharedPtr<FJsonObject> BuildDefaultV2RepairPolicy(const FString& ArchetypeName)
	{
		TSharedPtr<FJsonObject> RepairPolicy = MakeShared<FJsonObject>();

		TSharedPtr<FJsonObject> HardGate = MakeShared<FJsonObject>();
		HardGate->SetArrayField(TEXT("layer_a_failure_labels"), {
			MakeShared<FJsonValueString>(TEXT("orbit_dominant")),
			MakeShared<FJsonValueString>(TEXT("no_outside_to_core_read")),
			MakeShared<FJsonValueString>(TEXT("hero_sweep_missing")),
			MakeShared<FJsonValueString>(ArchetypeName.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
				? TEXT("cage_silhouette_risk")
				: TEXT("thin_wireframe_ribbons"))
		});
		HardGate->SetNumberField(TEXT("consecutive_iterations_before_rebuild"), 2);
		HardGate->SetBoolField(TEXT("block_scalar_only_layer_a_polish"), true);
		HardGate->SetStringField(TEXT("scheduled_op"),
			ArchetypeName.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase)
				? TEXT("rebuild_hero_fire_sweep_layer")
				: (ArchetypeName.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase)
					? TEXT("rebuild_hero_electric_arc_layer")
					: (ArchetypeName.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase)
						? TEXT("rebuild_hero_impact_burst_layer")
						: (ArchetypeName.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase)
							? TEXT("rebuild_hero_explosion_burst_layer")
						: (ArchetypeName.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase)
							? TEXT("rebuild_hero_arcane_sweep_layer")
							: TEXT("rebuild_main_attraction_layer"))))));
		HardGate->SetArrayField(TEXT("preserve_layers"), {
			MakeShared<FJsonValueString>(TEXT("support_core_layer")),
			MakeShared<FJsonValueString>(TEXT("support_sparks_layer")),
			MakeShared<FJsonValueString>(TEXT("support_glow_layer"))
		});
		RepairPolicy->SetObjectField(TEXT("hard_rebuild_gate"), HardGate);

		TSharedPtr<FJsonObject> SoftMappings = MakeShared<FJsonObject>();
		SoftMappings->SetArrayField(TEXT("opacity_noise_dominates"), {
			MakeShared<FJsonValueString>(TEXT("reduce_opacity_noise_preserve_body"))
		});
		SoftMappings->SetArrayField(TEXT("linework_not_flame_body"), {
			MakeShared<FJsonValueString>(TEXT("simplify_hero_ribbon_material")),
			MakeShared<FJsonValueString>(TEXT("retune_longitudinal_flow"))
		});
		SoftMappings->SetArrayField(TEXT("shell_ring_dominant"), {
			MakeShared<FJsonValueString>(TEXT("build_secondary_filament_layer"))
		});
		SoftMappings->SetArrayField(TEXT("secondary_overwhelms_hero"), {
			MakeShared<FJsonValueString>(TEXT("build_secondary_filament_layer"))
		});
		SoftMappings->SetArrayField(TEXT("inner_hot_core_missing"), {
			MakeShared<FJsonValueString>(TEXT("build_core_merge_glow_material")),
			MakeShared<FJsonValueString>(TEXT("build_core_turbulence_layer"))
		});
		RepairPolicy->SetObjectField(TEXT("soft_repair_mappings"), SoftMappings);

		return RepairPolicy;
	}

	static TSharedPtr<FJsonObject> BuildV2RepairPolicy(const TSharedPtr<FJsonObject>& Archetype)
	{
		const TSharedPtr<FJsonObject>* PolicyPtr = nullptr;
		if (Archetype.IsValid() && Archetype->TryGetObjectField(TEXT("repair_policy"), PolicyPtr) && PolicyPtr && (*PolicyPtr).IsValid())
		{
			return MakeShared<FJsonObject>(**PolicyPtr);
		}

		FString ArchetypeName;
		if (Archetype.IsValid())
		{
			Archetype->TryGetStringField(TEXT("archetype_name"), ArchetypeName);
		}
		return BuildDefaultV2RepairPolicy(ArchetypeName);
	}

	static TSharedPtr<FJsonObject> BuildV2StopConditions(const TSharedPtr<FJsonObject>& Archetype)
	{
		TSharedPtr<FJsonObject> StopConditions = MakeShared<FJsonObject>();
		const TSharedPtr<FJsonObject>* StopPtr = nullptr;
		if (Archetype.IsValid() && Archetype->TryGetObjectField(TEXT("stop_conditions"), StopPtr) && StopPtr && (*StopPtr).IsValid())
		{
			StopConditions = MakeShared<FJsonObject>(**StopPtr);
		}
		StopConditions->SetBoolField(TEXT("compile_log_must_be_clean"), true);
		StopConditions->SetBoolField(TEXT("native_authoring_only"), true);
		return StopConditions;
	}

	static TSharedPtr<FJsonObject> BuildDefaultV2EvalProfile(const FString& ArchetypeName)
	{
		TSharedPtr<FJsonObject> EvalProfile = MakeShared<FJsonObject>();
		if (ArchetypeName.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("fire_orb_v2_eval"));
		}
		else if (ArchetypeName.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("magic_orb_v2_eval"));
		}
		else if (ArchetypeName.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("electric_orb_v2_eval"));
		}
		else if (ArchetypeName.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("impact_orb_v2_eval"));
		}
		else if (ArchetypeName.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("explosion_orb_v2_eval"));
		}
		else
		{
			EvalProfile->SetStringField(TEXT("suite_name"), TEXT("suction_core_v2_eval"));
		}
		EvalProfile->SetArrayField(TEXT("scoring_axes"), {
			MakeShared<FJsonValueString>(TEXT("motion_family_match")),
			MakeShared<FJsonValueString>(TEXT("silhouette_family_match")),
			MakeShared<FJsonValueString>(TEXT("hero_secondary_hierarchy")),
			MakeShared<FJsonValueString>(TEXT("material_family_read")),
			MakeShared<FJsonValueString>(TEXT("gameplay_readability"))
		});
		EvalProfile->SetNumberField(TEXT("passing_threshold"), 18);
		return EvalProfile;
	}

	static TSharedPtr<FJsonObject> BuildV2EvalProfile(const TSharedPtr<FJsonObject>& Archetype)
	{
		const TSharedPtr<FJsonObject>* EvalPtr = nullptr;
		if (Archetype.IsValid() && Archetype->TryGetObjectField(TEXT("eval_profile"), EvalPtr) && EvalPtr && (*EvalPtr).IsValid())
		{
			return MakeShared<FJsonObject>(**EvalPtr);
		}

		FString ArchetypeName;
		if (Archetype.IsValid())
		{
			Archetype->TryGetStringField(TEXT("archetype_name"), ArchetypeName);
		}
		return BuildDefaultV2EvalProfile(ArchetypeName);
	}

	static TSharedPtr<FJsonObject> CloneJsonObjectOrEmpty(const TSharedPtr<FJsonObject>& Source)
	{
		return Source.IsValid() ? MakeShared<FJsonObject>(*Source) : MakeShared<FJsonObject>();
	}

	static void InheritOptionalBoolField(
		const TSharedPtr<FJsonObject>& Source,
		const TSharedPtr<FJsonObject>& Destination,
		const TCHAR* FieldName)
	{
		if (!Source.IsValid() || !Destination.IsValid() || Destination->HasField(FieldName))
		{
			return;
		}

		bool bValue = false;
		if (Source->TryGetBoolField(FieldName, bValue))
		{
			Destination->SetBoolField(FieldName, bValue);
		}
	}

	static TSharedPtr<FJsonObject> BuildV2VisualContract(const TSharedPtr<FJsonObject>& Archetype)
	{
		const TSharedPtr<FJsonObject>* ContractPtr = nullptr;
		if (Archetype.IsValid() && Archetype->TryGetObjectField(TEXT("visual_contract"), ContractPtr) && ContractPtr && (*ContractPtr).IsValid())
		{
			return CloneJsonObjectOrEmpty(*ContractPtr);
		}

		TSharedPtr<FJsonObject> Fallback = MakeShared<FJsonObject>();
		Fallback->SetStringField(TEXT("strategy"), TEXT("legacy_stack_authoring"));
		Fallback->SetBoolField(TEXT("allow_legacy_stack_fallback"), true);
		return Fallback;
	}

	static TSharedPtr<FJsonObject> BuildV2OpVisualContract(const TSharedPtr<FJsonObject>& Archetype, const FString& OpName)
	{
		const TSharedPtr<FJsonObject> VisualContract = BuildV2VisualContract(Archetype);
		const TSharedPtr<FJsonObject>* OpBindingsPtr = nullptr;
		if (!VisualContract.IsValid() ||
			!VisualContract->TryGetObjectField(TEXT("op_bindings"), OpBindingsPtr) ||
			!OpBindingsPtr ||
			!(*OpBindingsPtr).IsValid())
		{
			return nullptr;
		}

		const TSharedPtr<FJsonObject>* BindingPtr = nullptr;
		if (!(*OpBindingsPtr)->TryGetObjectField(OpName, BindingPtr) || !BindingPtr || !(*BindingPtr).IsValid())
		{
			return nullptr;
		}

		TSharedPtr<FJsonObject> Binding = CloneJsonObjectOrEmpty(*BindingPtr);
		if (!Binding->HasField(TEXT("op_name")))
		{
			Binding->SetStringField(TEXT("op_name"), OpName);
		}
		if (!Binding->HasField(TEXT("strategy")) && VisualContract->HasField(TEXT("strategy")))
		{
			Binding->SetStringField(TEXT("strategy"), VisualContract->GetStringField(TEXT("strategy")));
		}
		InheritOptionalBoolField(VisualContract, Binding, TEXT("allow_legacy_stack_fallback"));
		InheritOptionalBoolField(VisualContract, Binding, TEXT("auto_create_missing_parameters"));
		return Binding;
	}

	static FString InferSemanticToolFamily(const FString& OpName)
	{
		if (OpName.Contains(TEXT("review")))
		{
			return TEXT("critic");
		}
		if (OpName.Contains(TEXT("material")) || OpName.Contains(TEXT("flow")) || OpName.Contains(TEXT("opacity")))
		{
			return TEXT("material");
		}
		return TEXT("niagara");
	}

	static TArray<TSharedPtr<FJsonValue>> BuildV2ExecutionPlan(const TSharedPtr<FJsonObject>& Archetype)
	{
		TArray<TSharedPtr<FJsonValue>> ExecutionPlan;
		if (!Archetype.IsValid())
		{
			return ExecutionPlan;
		}

		FString ArchetypeName = TEXT("suction_core_energy");
		Archetype->TryGetStringField(TEXT("archetype_name"), ArchetypeName);

		const TArray<FString> LayerNames = ExtractRequiredLayerNames(Archetype);
		const FString GeneratedSystemName = FString::Printf(TEXT("NS_%s_V2"), *ArchetypeName);

		int32 Order = 1;
		ExecutionPlan.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("create_empty_niagara_system"),
			MakePlanInput({
				{ TEXT("system_name"), GeneratedSystemName },
				{ TEXT("save_path"), TEXT("/Game/ForgePilot_Generated/V2") },
				{ TEXT("apply_changes"), TEXT("true") }
			}),
			TEXT("Bootstrap a clean V2 Niagara System with no copied emitters or parent emitters."))));

		const TArray<TSharedPtr<FJsonValue>>* Ops = nullptr;
		if (!Archetype->TryGetArrayField(TEXT("default_semantic_ops_plan"), Ops) || !Ops)
		{
			return ExecutionPlan;
		}

		for (const TSharedPtr<FJsonValue>& Value : *Ops)
		{
			FString OpName;
			if (!Value.IsValid() || !Value->TryGetString(OpName) || OpName.IsEmpty())
			{
				continue;
			}

			const FString ToolFamily = InferSemanticToolFamily(OpName);
			if (ToolFamily.Equals(TEXT("critic"), ESearchCase::IgnoreCase))
			{
				ExecutionPlan.Add(MakeShared<FJsonValueObject>(MakePlanStep(
					Order++,
					TEXT("review_effect_failure_taxonomy_v2"),
					MakePlanInput({
						{ TEXT("archetype"), ArchetypeName },
						{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") }
					}),
					TEXT("Run the V2 rules-based critic before deciding whether to patch or rebuild the hero layer."))));
				continue;
			}

			if (ToolFamily.Equals(TEXT("material"), ESearchCase::IgnoreCase))
			{
				const bool bCoreMaterial = OpName.Contains(TEXT("core"), ESearchCase::IgnoreCase);
				ExecutionPlan.Add(MakeShared<FJsonValueObject>(MakePlanStep(
					Order++,
					TEXT("run_v2_material_op"),
					MakePlanInput({
						{ TEXT("archetype"), ArchetypeName },
						{ TEXT("op_name"), OpName },
						{ TEXT("parent_material_path"), bCoreMaterial ? TEXT("{{core_parent_material_path}}") : TEXT("{{hero_parent_material_path}}") },
						{ TEXT("apply_changes"), TEXT("true") }
					}),
					bCoreMaterial
						? TEXT("Build or tune the core material family before authoring the inner mass and bind glow layers.")
						: TEXT("Build or tune the hero material family before authoring hero or support streak layers."))));
				continue;
			}

			const bool bCoreLayer = OpName.Contains(TEXT("core"), ESearchCase::IgnoreCase) || OpName.Contains(TEXT("glow"), ESearchCase::IgnoreCase);
			TSharedPtr<FJsonObject> NiagaraInput = MakePlanInput({
				{ TEXT("archetype"), ArchetypeName },
				{ TEXT("op_name"), OpName },
				{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") },
				{ TEXT("material_path"), bCoreLayer ? TEXT("{{core_material_instance_path}}") : TEXT("{{hero_material_instance_path}}") },
				{ TEXT("apply_changes"), TEXT("true") }
			});
			if (TSharedPtr<FJsonObject> OpVisualContract = BuildV2OpVisualContract(Archetype, OpName))
			{
				NiagaraInput->SetObjectField(TEXT("visual_contract"), OpVisualContract);
			}
			ExecutionPlan.Add(MakeShared<FJsonValueObject>(MakePlanStep(
				Order++,
				TEXT("run_v2_niagara_op"),
				NiagaraInput,
				FString::Printf(TEXT("Author '%s' for the %s archetype."), *OpName, *ArchetypeName))));
		}

		return ExecutionPlan;
	}
}

FString FVFXKnowledgeLoader::GetKnowledgeRootDir()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ForgePilot"));
	if (Plugin.IsValid())
	{
		return FPaths::Combine(Plugin->GetBaseDir(), TEXT("Knowledge"));
	}

	return FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("ForgePilot"), TEXT("Knowledge")));
}

TArray<FString> FVFXKnowledgeLoader::GetAvailableRecipeTypes()
{
	TArray<FString> AvailableRecipes;
	FString DummyType;
	FString DummyPath;
	FindRecipeTypeDirectory(TEXT(""), DummyType, DummyPath, &AvailableRecipes);
	return AvailableRecipes;
}

bool FVFXKnowledgeLoader::LoadRecipeBundle(
	const FString& RequestedEffectType,
	TSharedPtr<FJsonObject>& OutBundle,
	FString& OutCanonicalEffectType,
	FString& OutErrorMessage,
	TArray<FString>* OutAvailableRecipes)
{
	FString RecipeFilePath;
	if (!FindRecipeTypeDirectory(RequestedEffectType, OutCanonicalEffectType, RecipeFilePath, OutAvailableRecipes))
	{
		OutErrorMessage = FString::Printf(TEXT("No recipe found for effect type '%s'."), *RequestedEffectType);
		return false;
	}

	TSharedPtr<FJsonObject> Recipe;
	if (!LoadJsonFile(RecipeFilePath, Recipe, OutErrorMessage))
	{
		return false;
	}

	TSharedPtr<FJsonObject> ColorPrinciples;
	TSharedPtr<FJsonObject> TimingPrinciples;
	TSharedPtr<FJsonObject> ScalePrinciples;
	if (!LoadJsonFile(GetPrinciplesRootDir() / TEXT("color_palettes.json"), ColorPrinciples, OutErrorMessage) ||
		!LoadJsonFile(GetPrinciplesRootDir() / TEXT("timing_rules.json"), TimingPrinciples, OutErrorMessage) ||
		!LoadJsonFile(GetPrinciplesRootDir() / TEXT("scale_reference.json"), ScalePrinciples, OutErrorMessage))
	{
		return false;
	}

	OutBundle = BuildRecipeBundleObject(
		OutCanonicalEffectType,
		RecipeFilePath,
		Recipe,
		ColorPrinciples,
		TimingPrinciples,
		ScalePrinciples,
		TArray<FString>());
	return true;
}

bool FVFXKnowledgeLoader::ResolveEffectPlan(
	const FString& Description,
	TSharedPtr<FJsonObject>& OutPlan,
	FString& OutErrorMessage,
	TArray<FString>* OutAvailableRecipes)
{
	FString VariantName;
	const FString ResolvedEffectType = ResolveEffectTypeFromDescription(Description, VariantName);
	if (ResolvedEffectType.IsEmpty())
	{
		if (OutAvailableRecipes)
		{
			*OutAvailableRecipes = GetAvailableRecipeTypes();
		}
		OutErrorMessage = FString::Printf(TEXT("Could not infer an effect type from description '%s'."), *Description);
		return false;
	}

	TSharedPtr<FJsonObject> Bundle;
	FString CanonicalEffectType;
	if (!LoadRecipeBundle(ResolvedEffectType, Bundle, CanonicalEffectType, OutErrorMessage, OutAvailableRecipes))
	{
		return false;
	}

	const TSharedPtr<FJsonObject>* RecipePtr = nullptr;
	if (!Bundle->TryGetObjectField(TEXT("recipe"), RecipePtr) || !RecipePtr || !(*RecipePtr).IsValid())
	{
		OutErrorMessage = TEXT("Loaded recipe bundle did not contain a valid recipe object.");
		return false;
	}

	const TArray<FString> DescriptionTokens = ExtractDescriptionTokens(Description);
	if (VariantName.IsEmpty())
	{
		VariantName = PickVariantForDescription(*RecipePtr, DescriptionTokens);
	}

	const TArray<FString> SelectedLayers = SelectEmitterLayersForDescription(CanonicalEffectType, VariantName, *RecipePtr, DescriptionTokens);
	TSharedPtr<FJsonObject> WorkflowPattern;
	TSharedPtr<FJsonObject> TemplateArchetypes;
	LoadOptionalJsonFile(GetNiagaraPatternsRootDir() / TEXT("ai_authoring_workflow.json"), WorkflowPattern);
	LoadOptionalJsonFile(GetNiagaraPatternsRootDir() / TEXT("template_archetypes.json"), TemplateArchetypes);

	FString SystemConvention = TEXT("NS_{Context}Effect_{Variant}");
	const TSharedPtr<FJsonObject>* BaseArchitecturePtr = nullptr;
	if ((*RecipePtr)->TryGetObjectField(TEXT("base_architecture"), BaseArchitecturePtr) &&
		BaseArchitecturePtr && (*BaseArchitecturePtr).IsValid())
	{
		(*BaseArchitecturePtr)->TryGetStringField(TEXT("system_name_convention"), SystemConvention);
	}

	const FString VariantToken = VariantName.IsEmpty() ? CanonicalEffectType : VariantName;
	FString GeneratedSystemName = SystemConvention;
	GeneratedSystemName.ReplaceInline(TEXT("{Context}"), TEXT("Agent"));
	GeneratedSystemName.ReplaceInline(TEXT("{Variant}"), *SanitizeNameToken(VariantToken));
	GeneratedSystemName.ReplaceInline(TEXT("{Type}"), *SanitizeNameToken(CanonicalEffectType));
	GeneratedSystemName.ReplaceInline(TEXT("{Effect}"), *SanitizeNameToken(CanonicalEffectType));

	TArray<TSharedPtr<FJsonValue>> PlanSteps;
	int32 Order = 1;
	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("load_vfx_recipe"),
		MakePlanInput({ { TEXT("effect_type"), CanonicalEffectType } }),
		TEXT("Load the canonical recipe bundle and relevant principles from disk before authoring."))));

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("create_empty_niagara_system"),
		MakePlanInput({
			{ TEXT("system_name"), GeneratedSystemName },
			{ TEXT("save_path"), TEXT("/Game/ForgePilot_Generated") },
			{ TEXT("apply_changes"), TEXT("true") }
		}),
		TEXT("Preferred bootstrap: create a clean empty Niagara System so the effect can be assembled emitter-by-emitter."),
		TEXT("Run as the default starting point for AI-authored Niagara work."))));

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("find_niagara_template_emitters"),
		MakePlanInput({
			{ TEXT("description"), Description },
			{ TEXT("search_root"), TEXT("/Game/") },
			{ TEXT("max_candidates_per_layer"), TEXT("5") }
		}),
		TEXT("Find reusable Niagara template systems whose emitters can be copied into the clean target system layer-by-layer."))));

	for (const FString& LayerName : SelectedLayers)
	{
		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("add_niagara_emitter_to_system"),
			MakePlanInput({
				{ TEXT("target_system_path"), TEXT("{{created_or_duplicated_system_path}}") },
				{ TEXT("source_system_path"), FString::Printf(TEXT("{{find_niagara_template_emitters.layer_best_matches.%s.source_system_path}}"), *LayerName) },
				{ TEXT("source_emitter_name"), FString::Printf(TEXT("{{find_niagara_template_emitters.layer_best_matches.%s.source_emitter_name}}"), *LayerName) },
				{ TEXT("copy_emitter"), TEXT("true") },
				{ TEXT("apply_changes"), TEXT("true") }
			}),
			FString::Printf(TEXT("Preferred emitter bootstrap for layer '%s': copy a matching emitter from a reusable template system into the clean target system."), *LayerName),
			FString::Printf(TEXT("Run only if a suitable template emitter is found for layer '%s' in step 3."), *LayerName))));

		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("add_empty_niagara_emitter"),
			MakePlanInput({
				{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") },
				{ TEXT("emitter_name"), LayerName },
				{ TEXT("simulation_target"), TEXT("cpu") },
				{ TEXT("add_sprite_renderer"), TEXT("true") },
				{ TEXT("add_default_modules_and_renderer"), TEXT("true") },
				{ TEXT("apply_changes"), TEXT("true") }
			}),
			FString::Printf(TEXT("Fallback bootstrap for layer '%s': create a starter emitter only when no good template emitter exists."), *LayerName),
			FString::Printf(TEXT("Run only if the template-emitter add step for layer '%s' is skipped."), *LayerName))));
	}

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("get_niagara_system_info"),
		MakePlanInput({ { TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") } }),
		TEXT("Inspect emitter names, user parameters, and current structure after bootstrap so later material and stack edits target the correct emitters."))));

	for (const FString& LayerName : SelectedLayers)
	{
		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("get_niagara_module_inputs"),
			MakePlanInput({
				{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") },
				{ TEXT("emitter_name"), LayerName }
			}),
			FString::Printf(TEXT("Inspect the stack-aware module/input surface for layer '%s' before applying artist-facing edits."), *LayerName))));
	}

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("search_assets"),
		MakePlanInput({
			{ TEXT("query"), FString::Printf(TEXT("%s %s"), *CanonicalEffectType, *FString::Join(SelectedLayers, TEXT(" "))).TrimStartAndEnd() },
			{ TEXT("class_names"), TEXT("Material,MaterialInstance,Texture2D") },
			{ TEXT("max_results"), TEXT("40") }
		}),
		TEXT("Find flipbooks and material parents that match the selected recipe layers."))));

	for (const FString& LayerName : SelectedLayers)
	{
		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("get_texture_material_inspection"),
			MakePlanInput({
				{ TEXT("texture_path"), FString::Printf(TEXT("{{texture_lookup_or_search.%s}}"), *LayerName) },
				{ TEXT("include_referencers"), TEXT("false") },
				{ TEXT("include_image_base64"), TEXT("false") },
				{ TEXT("save_preview"), TEXT("true") }
			}),
			FString::Printf(TEXT("Inspect the resolved texture for layer '%s' before material authoring so the agent can read its visual, metadata, and flipbook/grid hints."), *LayerName),
			TEXT("Run only if step 6 finds a texture candidate for this layer."))));

		TSharedPtr<FJsonObject> CreateMaterialInput = MakeShared<FJsonObject>();
		CreateMaterialInput->SetStringField(
			TEXT("material_name"),
			FString::Printf(TEXT("M_%s_%s"), *SanitizeNameToken(GeneratedSystemName), *SanitizeNameToken(LayerName)));
		CreateMaterialInput->SetStringField(TEXT("save_path"), TEXT("/Game/ForgePilot_Generated/Materials"));
		CreateMaterialInput->SetArrayField(TEXT("base_color"), MakeNumberArray(BuildFallbackLayerColor(CanonicalEffectType, LayerName, Bundle)));

		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("create_material"),
			CreateMaterialInput,
			FString::Printf(TEXT("Fallback: create a simple starter material for layer '%s' using recipe-driven palette defaults plus any texture inspection guidance from the previous step."), *LayerName),
			TEXT("Run only if step 6 does not find a suitable existing material for this layer."))));

		PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
			Order++,
			TEXT("set_niagara_renderer_material"),
			MakePlanInput({
				{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") },
				{ TEXT("emitter_name"), LayerName },
				{ TEXT("material_path"), FString::Printf(TEXT("{{material_lookup_or_created.%s}}"), *LayerName) },
				{ TEXT("apply_changes"), TEXT("true") }
			}),
			FString::Printf(TEXT("Bind the resolved existing-or-created material to recipe layer '%s'."), *LayerName),
			TEXT("Run after step 5 confirms emitter naming or after remapping emitter names if the template differs."))));
	}

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("compile_niagara_system"),
		MakePlanInput({
			{ TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") },
			{ TEXT("apply_changes"), TEXT("true") }
		}),
		TEXT("Compile and save the Niagara System after emitter bootstrap and first-pass artist edits so diagnostics are available before review."))));

	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("review_niagara_system"),
		MakePlanInput({ { TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}") } }),
		TEXT("Review the final Niagara stack against recipe layers, timing, color, and scale guidance."))));

	TSharedPtr<FJsonObject> SpawnInput = MakeShared<FJsonObject>();
	SpawnInput->SetStringField(TEXT("system_path"), TEXT("{{created_or_duplicated_system_path}}"));
	SpawnInput->SetArrayField(TEXT("location"), MakeNumberArray({ 0.0, 0.0, 0.0 }));
	SpawnInput->SetArrayField(TEXT("rotation"), MakeNumberArray({ 0.0, 0.0, 0.0 }));
	SpawnInput->SetStringField(TEXT("actor_label"), FString::Printf(TEXT("%s_Preview"), *GeneratedSystemName));
	SpawnInput->SetStringField(TEXT("auto_activate"), TEXT("true"));
	SpawnInput->SetStringField(TEXT("apply_changes"), TEXT("true"));
	PlanSteps.Add(MakeShared<FJsonValueObject>(MakePlanStep(
		Order++,
		TEXT("spawn_niagara_at_location"),
		SpawnInput,
		TEXT("Spawn the configured Niagara System into the level for immediate visual validation."),
		TEXT("Run after review when you want a preview actor in the scene."))));

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("description"), Description);
	Result->SetStringField(TEXT("resolved_effect_type"), CanonicalEffectType);
	if (!VariantName.IsEmpty())
	{
		Result->SetStringField(TEXT("resolved_variant"), VariantName);
	}
	Result->SetObjectField(TEXT("recipe_bundle"), Bundle);
	Result->SetObjectField(
		TEXT("authoring_strategy"),
		BuildAuthoringStrategy(CanonicalEffectType, VariantName, DescriptionTokens, WorkflowPattern, TemplateArchetypes, *RecipePtr, SelectedLayers));

	TArray<TSharedPtr<FJsonValue>> LayerValues;
	for (const FString& LayerName : SelectedLayers)
	{
		LayerValues.Add(MakeShared<FJsonValueString>(LayerName));
	}
	Result->SetArrayField(TEXT("selected_layers"), LayerValues);
	Result->SetArrayField(TEXT("execution_plan"), PlanSteps);

	const TArray<TSharedPtr<FJsonValue>>* ModulesPtr = nullptr;
	if ((*RecipePtr)->TryGetArrayField(TEXT("niagara_modules_needed"), ModulesPtr) && ModulesPtr)
	{
		Result->SetArrayField(TEXT("niagara_modules_needed"), *ModulesPtr);
	}

	TArray<TSharedPtr<FJsonValue>> Notes;
	Notes.Add(MakeShared<FJsonValueString>(TEXT("The plan now prefers empty-system bootstrap plus template-emitter copying so the AI can assemble effects layer-by-layer instead of cloning whole systems.")));
	Notes.Add(MakeShared<FJsonValueString>(TEXT("Each selected recipe layer is mapped to a preferred emitter archetype so Claude/Codex can search for the right kind of template emitter before editing stack details.")));
	Notes.Add(MakeShared<FJsonValueString>(TEXT("Recipe content is loaded from Knowledge/ on disk at runtime; the tool does not hardcode recipe bodies.")));
	Notes.Add(MakeShared<FJsonValueString>(TEXT("After emitter bootstrap, the AI should inspect module inputs and then apply artist-facing controls in a stable order: quantity, lifetime, size, color, opacity, velocity, forces, renderer, material.")));
	Notes.Add(MakeShared<FJsonValueString>(TEXT("Material fallback steps create starter materials only; richer graph authoring can follow with material tools once the first pass is visible.")));
	Result->SetArrayField(TEXT("notes"), Notes);

	OutPlan = Result;
	return true;
}

bool FVFXKnowledgeLoader::ResolveEffectPlanV2(
	const FString& Description,
	TSharedPtr<FJsonObject>& OutPlan,
	FString& OutErrorMessage,
	TArray<FString>* OutAvailableRecipes,
	const FString& ExplicitArchetype)
{
	FV2ArchetypeSelection Selection;
	if (!ExplicitArchetype.IsEmpty())
	{
		if (!ResolveExplicitV2ArchetypeSelection(ExplicitArchetype, Selection))
		{
			if (OutAvailableRecipes)
			{
				*OutAvailableRecipes = GetAvailableRecipeTypes();
			}
			OutErrorMessage = FString::Printf(TEXT("Explicit V2 archetype '%s' is not supported."), *ExplicitArchetype);
			return false;
		}
	}
	else if (!ResolveV2ArchetypeSelection(Description, Selection))
	{
		if (OutAvailableRecipes)
		{
			*OutAvailableRecipes = GetAvailableRecipeTypes();
		}
		OutErrorMessage = FString::Printf(
			TEXT("Description '%s' did not match any V2 orb-centric archetype gate. Current V2 supports suction_core_energy, electric_arc_orb, hero_fire_orb_sweep, arcane_pull_orb, impact_pulse_core, and explosion_flash_core."),
			*Description);
		return false;
	}

	TSharedPtr<FJsonObject> Bundle;
	FString CanonicalEffectType;
	if (!LoadRecipeBundle(Selection.EffectType, Bundle, CanonicalEffectType, OutErrorMessage, OutAvailableRecipes))
	{
		return false;
	}

	TSharedPtr<FJsonObject> Archetype;
	if (!LoadV2Sidecar(Selection.EffectType, Selection.ArchetypeFileName, Archetype, OutErrorMessage))
	{
		return false;
	}

	TSharedPtr<FJsonObject> Taxonomy;
	if (!LoadV2Sidecar(Selection.EffectType, Selection.TaxonomyFileName, Taxonomy, OutErrorMessage))
	{
		return false;
	}

	const TArray<FString> DescriptionTokens = ExtractDescriptionTokens(Description);
	TSharedPtr<FJsonObject> WorkflowPattern;
	TSharedPtr<FJsonObject> TemplateArchetypes;
	LoadOptionalJsonFile(GetNiagaraPatternsRootDir() / TEXT("ai_authoring_workflow.json"), WorkflowPattern);
	LoadOptionalJsonFile(GetNiagaraPatternsRootDir() / TEXT("template_archetypes.json"), TemplateArchetypes);

	const TSharedPtr<FJsonObject>* RecipePtr = nullptr;
	if (!Bundle->TryGetObjectField(TEXT("recipe"), RecipePtr) || !RecipePtr || !(*RecipePtr).IsValid())
	{
		OutErrorMessage = FString::Printf(TEXT("Loaded %s recipe bundle did not contain a valid recipe object for V2 planning."), *Selection.EffectType);
		return false;
	}

	TArray<FString> SelectedLayers = ExtractRequiredLayerNames(Archetype);
	TArray<TSharedPtr<FJsonValue>> ExecutionPlan = BuildV2ExecutionPlan(Archetype);

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("description"), Description);
	Result->SetStringField(TEXT("resolved_effect_type"), CanonicalEffectType);
	Result->SetStringField(TEXT("resolved_archetype"), Selection.ArchetypeName);
	Result->SetStringField(TEXT("rollout_mode"), TEXT("v2_sidecar"));
	Result->SetObjectField(TEXT("recipe_bundle"), Bundle);
	Result->SetObjectField(
		TEXT("authoring_strategy"),
		BuildAuthoringStrategy(CanonicalEffectType, Selection.ArchetypeName, DescriptionTokens, WorkflowPattern, TemplateArchetypes,
			*RecipePtr, SelectedLayers));
	Result->SetObjectField(TEXT("layer_plan"), BuildV2LayerPlan(Archetype));
	Result->SetArrayField(TEXT("semantic_ops_plan"), BuildV2SemanticOpsPlan(Archetype));
	Result->SetObjectField(TEXT("visual_contract"), BuildV2VisualContract(Archetype));
	Result->SetObjectField(TEXT("failure_taxonomy"), Taxonomy);
	Result->SetObjectField(TEXT("repair_policy"), BuildV2RepairPolicy(Archetype));
	Result->SetObjectField(TEXT("stop_conditions"), BuildV2StopConditions(Archetype));
	Result->SetObjectField(TEXT("eval_profile"), BuildV2EvalProfile(Archetype));
	Result->SetArrayField(TEXT("execution_plan"), ExecutionPlan);
	TArray<TSharedPtr<FJsonValue>> SelectedLayerValues;
	for (const FString& LayerName : SelectedLayers)
	{
		SelectedLayerValues.Add(MakeShared<FJsonValueString>(LayerName));
	}
	Result->SetArrayField(TEXT("selected_layers"), SelectedLayerValues);
	Result->SetArrayField(TEXT("notes"), {
		MakeShared<FJsonValueString>(TEXT("V2 planning starts from a fixed archetype, not a blank policy.")),
		MakeShared<FJsonValueString>(FString::Printf(TEXT("Resolved archetype '%s' is expected to preserve a clear hero-secondary hierarchy."), *Selection.ArchetypeName)),
		MakeShared<FJsonValueString>(TEXT("Repair policy is rules-based: two consecutive hero-layer motion-family failures trigger a rebuild gate."))
	});

	OutPlan = Result;
	return true;
}

bool FVFXKnowledgeLoader::LoadV2VisualContract(
	const FString& ArchetypeName,
	TSharedPtr<FJsonObject>& OutVisualContract,
	FString& OutErrorMessage)
{
	FV2ArchetypeSelection Selection;
	if (!ResolveExplicitV2ArchetypeSelection(ArchetypeName, Selection))
	{
		OutErrorMessage = FString::Printf(TEXT("Explicit V2 archetype '%s' is not supported."), *ArchetypeName);
		return false;
	}

	TSharedPtr<FJsonObject> Archetype;
	if (!LoadV2Sidecar(Selection.EffectType, Selection.ArchetypeFileName, Archetype, OutErrorMessage))
	{
		return false;
	}

	OutVisualContract = BuildV2VisualContract(Archetype);
	return OutVisualContract.IsValid();
}
