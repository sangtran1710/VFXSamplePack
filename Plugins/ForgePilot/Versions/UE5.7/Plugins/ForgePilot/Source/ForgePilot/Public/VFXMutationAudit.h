// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class FORGEPILOT_API FVFXMutationAudit
{
public:
	static FString GetAuditRoot();

	static bool RecordAssetMutation(
		const FString& ToolName,
		const FString& AssetPath,
		const FString& AssetClass,
		const FString& MutationAction,
		const FString& SnapshotPath,
		const FString& PackageFilename,
		bool bSucceeded,
		const FString& Message,
		FString& OutAuditEntryPath,
		const TSharedPtr<FJsonObject>& Details = nullptr);

	static void GetRecentAuditEntryPaths(int32 MaxResults, TArray<FString>& OutEntryPaths);
	static bool ReadAuditEntry(const FString& EntryPath, TSharedPtr<FJsonObject>& OutEntryObject);
};
