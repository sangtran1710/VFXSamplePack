// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class FVFXKnowledgeLoader
{
public:
	static FString GetKnowledgeRootDir();
	static TArray<FString> GetAvailableRecipeTypes();

	static bool LoadRecipeBundle(
		const FString& RequestedEffectType,
		TSharedPtr<FJsonObject>& OutBundle,
		FString& OutCanonicalEffectType,
		FString& OutErrorMessage,
		TArray<FString>* OutAvailableRecipes = nullptr);

	static bool ResolveEffectPlan(
		const FString& Description,
		TSharedPtr<FJsonObject>& OutPlan,
		FString& OutErrorMessage,
		TArray<FString>* OutAvailableRecipes = nullptr);

	static bool ResolveEffectPlanV2(
		const FString& Description,
		TSharedPtr<FJsonObject>& OutPlan,
		FString& OutErrorMessage,
		TArray<FString>* OutAvailableRecipes = nullptr,
		const FString& ExplicitArchetype = FString());

	static bool LoadV2VisualContract(
		const FString& ArchetypeName,
		TSharedPtr<FJsonObject>& OutVisualContract,
		FString& OutErrorMessage);
};
