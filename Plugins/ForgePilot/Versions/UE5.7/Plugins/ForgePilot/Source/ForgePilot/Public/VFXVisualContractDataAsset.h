// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "VFXVisualContractDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FORGEPILOT_API FVFXVisualContractBinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString ParameterType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bSkipIfEmpty = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bHasScalarValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	float ScalarValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bHasVectorValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FVector VectorValue = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bHasStringValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bHasAssetValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	TSoftObjectPtr<UObject> AssetValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString RawValueJson;
};

USTRUCT(BlueprintType)
struct FORGEPILOT_API FVFXVisualContractOpEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString OpName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString Layer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString Strategy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bAllowLegacyStackFallback = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bAutoCreateMissingParameters = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	TArray<FVFXVisualContractBinding> Bindings;
};

UCLASS(BlueprintType)
class FORGEPILOT_API UVFXVisualContractDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString ArchetypeName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString Strategy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bAllowLegacyStackFallback = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	bool bAutoCreateMissingParameters = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	FString RawVisualContractJson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForgePilot|VisualContract")
	TArray<FVFXVisualContractOpEntry> OpBindings;

	UFUNCTION(BlueprintPure, Category = "ForgePilot|VisualContract")
	bool GetOpBinding(const FString& OpName, FVFXVisualContractOpEntry& OutBinding) const;

	UFUNCTION(BlueprintPure, Category = "ForgePilot|VisualContract")
	bool GetParameterBinding(const FString& OpName, const FString& ParameterName, FVFXVisualContractBinding& OutBinding) const;
};
