// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "VFXMutationAudit.h"
#include "VFXToolRegistryHelpers.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"

namespace
{
	static FString SanitizeAuditToken(FString Value)
	{
		Value = Value.TrimStartAndEnd();
		for (TCHAR& Character : Value)
		{
			if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
			{
				Character = TEXT('_');
			}
		}

		return Value.IsEmpty() ? TEXT("entry") : Value;
	}
}

FString FVFXMutationAudit::GetAuditRoot()
{
	return FPaths::Combine(GetForgePilotDownloadsRoot(), TEXT("MutationAudit"));
}

bool FVFXMutationAudit::RecordAssetMutation(
	const FString& ToolName,
	const FString& AssetPath,
	const FString& AssetClass,
	const FString& MutationAction,
	const FString& SnapshotPath,
	const FString& PackageFilename,
	const bool bSucceeded,
	const FString& Message,
	FString& OutAuditEntryPath,
	const TSharedPtr<FJsonObject>& Details)
{
	const FString AuditRoot = GetAuditRoot();
	if (AuditRoot.IsEmpty())
	{
		return false;
	}

	IFileManager::Get().MakeDirectory(*AuditRoot, true);

	const FDateTime Now = FDateTime::Now();
	const FString Timestamp = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
	const FString UniqueSuffix = FString::Printf(TEXT("%lld"), Now.GetTicks());
	const FString ToolToken = SanitizeAuditToken(ToolName);
	const FString AssetToken = SanitizeAuditToken(FPaths::GetBaseFilename(AssetPath));
	OutAuditEntryPath = FPaths::Combine(
		AuditRoot,
		FString::Printf(TEXT("%s_%s_%s_%s.json"), *Timestamp, *UniqueSuffix, *ToolToken, *AssetToken));

	TSharedPtr<FJsonObject> AuditObj = MakeShareable(new FJsonObject());
	AuditObj->SetStringField(TEXT("timestamp"), Now.ToIso8601());
	AuditObj->SetStringField(TEXT("tool_name"), ToolName);
	AuditObj->SetStringField(TEXT("asset_path"), AssetPath);
	AuditObj->SetStringField(TEXT("asset_class"), AssetClass);
	AuditObj->SetStringField(TEXT("mutation_action"), MutationAction);
	AuditObj->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	AuditObj->SetStringField(TEXT("package_filename"), PackageFilename);
	AuditObj->SetBoolField(TEXT("succeeded"), bSucceeded);
	AuditObj->SetStringField(TEXT("message"), Message);
	if (Details.IsValid())
	{
		AuditObj->SetObjectField(TEXT("details"), Details);
	}

	return FFileHelper::SaveStringToFile(JsonObjToString(AuditObj), *OutAuditEntryPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

void FVFXMutationAudit::GetRecentAuditEntryPaths(const int32 MaxResults, TArray<FString>& OutEntryPaths)
{
	OutEntryPaths.Reset();

	const FString AuditRoot = GetAuditRoot();
	if (AuditRoot.IsEmpty() || !IFileManager::Get().DirectoryExists(*AuditRoot))
	{
		return;
	}

	TArray<FString> FoundFiles;
	IFileManager::Get().FindFiles(FoundFiles, *FPaths::Combine(AuditRoot, TEXT("*.json")), true, false);
	for (FString& FileName : FoundFiles)
	{
		FileName = FPaths::Combine(AuditRoot, FileName);
	}

	FoundFiles.Sort([](const FString& Left, const FString& Right)
	{
		return IFileManager::Get().GetTimeStamp(*Left) > IFileManager::Get().GetTimeStamp(*Right);
	});

	const int32 Limit = FMath::Clamp(MaxResults, 1, 200);
	for (const FString& EntryPath : FoundFiles)
	{
		OutEntryPaths.Add(EntryPath);
		if (OutEntryPaths.Num() >= Limit)
		{
			break;
		}
	}
}

bool FVFXMutationAudit::ReadAuditEntry(const FString& EntryPath, TSharedPtr<FJsonObject>& OutEntryObject)
{
	OutEntryObject.Reset();
	if (!FPaths::FileExists(EntryPath))
	{
		return false;
	}

	FString JsonText;
	if (!FFileHelper::LoadFileToString(JsonText, *EntryPath))
	{
		return false;
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonText);
	return FJsonSerializer::Deserialize(Reader, OutEntryObject) && OutEntryObject.IsValid();
}
