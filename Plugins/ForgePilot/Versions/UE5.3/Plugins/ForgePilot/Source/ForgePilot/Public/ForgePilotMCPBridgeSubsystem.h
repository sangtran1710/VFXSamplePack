// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "ForgePilotMCPBridgeSubsystem.generated.h"

class FVFXToolRegistry;
class FJsonObject;

UCLASS()
class FORGEPILOT_API UForgePilotMCPBridgeSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "ForgePilot|MCP")
	FString HandleBridgeRequest(const FString& RequestJson);

	UFUNCTION(BlueprintCallable, Category = "ForgePilot|MCP")
	FString GetBridgeStatusJson() const;

private:
	TSharedPtr<FVFXToolRegistry> ToolRegistry;
	bool bInitialized = false;

	FString BuildErrorResponse(
		const FString& Action,
		const FString& ErrorCode,
		const FString& Message,
		const TSharedPtr<FJsonObject>& DebugContext = nullptr) const;
	void AppendBridgeIdentity(const TSharedPtr<FJsonObject>& Response) const;
};
