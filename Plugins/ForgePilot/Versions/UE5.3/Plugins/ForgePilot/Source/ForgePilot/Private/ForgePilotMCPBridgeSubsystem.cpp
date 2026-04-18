// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotMCPBridgeSubsystem.h"
#include "ForgePilotCache.h"
#include "ForgePilotSettings.h"
#include "VFXToolRegistry.h"

#include "Dom/JsonValue.h"
#include "HAL/PlatformMisc.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"

namespace
{
static FString JsonObjectToString(const TSharedPtr<FJsonObject>& Obj)
{
	FString Out;
	if (!Obj.IsValid())
	{
		return TEXT("{}");
	}

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);
	return Out;
}

static TSharedPtr<FJsonObject> ParseJsonObject(const FString& Input)
{
	if (Input.TrimStartAndEnd().IsEmpty())
	{
		return MakeShareable(new FJsonObject());
	}

	TSharedPtr<FJsonObject> Parsed;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Input);
	if (!FJsonSerializer::Deserialize(Reader, Parsed) || !Parsed.IsValid())
	{
		return nullptr;
	}

	return Parsed;
}

static FString GetBridgeSessionId()
{
	return FPlatformMisc::GetEnvironmentVariable(TEXT("FORGEPILOT_BRIDGE_SESSION_ID")).TrimStartAndEnd();
}

static FString GetBridgeProjectDir()
{
	return UForgePilotSettings::GetNormalizedProjectDirectory();
}

static FString GetBridgeProjectName()
{
	return UForgePilotSettings::GetBridgeProjectName();
}

static FString GetBridgeWorkspaceId()
{
	return UForgePilotSettings::GetBridgeWorkspaceId();
}

static FString NormalizeBridgePathForComparison(FString Path)
{
	Path = FPaths::ConvertRelativePathToFull(Path);
	FPaths::NormalizeFilename(Path);
	Path.ReplaceInline(TEXT("\\"), TEXT("/"));
	while (Path.EndsWith(TEXT("/")))
	{
		Path.LeftChopInline(1, false);
	}
	return Path;
}
}

void UForgePilotMCPBridgeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ToolRegistry = MakeShared<FVFXToolRegistry>();
	bInitialized = true;
}

void UForgePilotMCPBridgeSubsystem::Deinitialize()
{
	bInitialized = false;
	ToolRegistry.Reset();
	Super::Deinitialize();
}

FString UForgePilotMCPBridgeSubsystem::BuildErrorResponse(
	const FString& Action,
	const FString& ErrorCode,
	const FString& Message,
	const TSharedPtr<FJsonObject>& DebugContext) const
{
	TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
	Response->SetBoolField(TEXT("ok"), false);
	Response->SetBoolField(TEXT("success"), false);
	Response->SetStringField(TEXT("action"), Action);
	Response->SetStringField(TEXT("error_code"), ErrorCode);
	Response->SetStringField(TEXT("error_message"), Message);
	Response->SetStringField(TEXT("error"), Message);
	if (DebugContext.IsValid())
	{
		Response->SetObjectField(TEXT("debug_context"), DebugContext);
	}
	return JsonObjectToString(Response);
}

FString UForgePilotMCPBridgeSubsystem::GetBridgeStatusJson() const
{
	TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
	Response->SetBoolField(TEXT("ok"), true);
	Response->SetStringField(TEXT("status"), bInitialized ? TEXT("ready") : TEXT("not_ready"));
	Response->SetBoolField(TEXT("initialized"), bInitialized);
	Response->SetBoolField(TEXT("has_tool_registry"), ToolRegistry.IsValid());
	AppendBridgeIdentity(Response);
	return JsonObjectToString(Response);
}

FString UForgePilotMCPBridgeSubsystem::HandleBridgeRequest(const FString& RequestJson)
{
	TSharedPtr<FJsonObject> RequestObj = ParseJsonObject(RequestJson);
	if (!RequestObj.IsValid())
	{
		return BuildErrorResponse(
			TEXT("unknown"),
			TEXT("BRIDGE_INVALID_JSON"),
			TEXT("Request is not valid JSON object."));
	}

	FString Action;
	if (!RequestObj->TryGetStringField(TEXT("action"), Action))
	{
		return BuildErrorResponse(
			TEXT("unknown"),
			TEXT("BRIDGE_MISSING_ACTION"),
			TEXT("Missing required field: action"));
	}

	if (!bInitialized || !ToolRegistry.IsValid())
	{
		return BuildErrorResponse(
			Action,
			TEXT("BRIDGE_NOT_INITIALIZED"),
			TEXT("Bridge is not initialized."));
	}

	FString ExpectedWorkspaceId;
	if (RequestObj->TryGetStringField(TEXT("expected_workspace_id"), ExpectedWorkspaceId) &&
		!ExpectedWorkspaceId.TrimStartAndEnd().IsEmpty() &&
		!ExpectedWorkspaceId.Equals(GetBridgeWorkspaceId(), ESearchCase::IgnoreCase))
	{
		TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
		DebugContext->SetStringField(TEXT("expected_workspace_id"), ExpectedWorkspaceId);
		DebugContext->SetStringField(TEXT("actual_workspace_id"), GetBridgeWorkspaceId());
		DebugContext->SetStringField(TEXT("actual_project_dir"), GetBridgeProjectDir());
		DebugContext->SetStringField(TEXT("actual_project_name"), GetBridgeProjectName());
		DebugContext->SetStringField(TEXT("actual_bridge_session_id"), GetBridgeSessionId());
		return BuildErrorResponse(
			Action,
			TEXT("BRIDGE_WORKSPACE_MISMATCH"),
			TEXT("Bridge request targeted a different workspace."),
			DebugContext);
	}

	FString ExpectedProjectDir;
	if (RequestObj->TryGetStringField(TEXT("expected_project_dir"), ExpectedProjectDir) &&
		!ExpectedProjectDir.TrimStartAndEnd().IsEmpty())
	{
		const FString NormalizedExpectedProjectDir = NormalizeBridgePathForComparison(ExpectedProjectDir);
		if (!NormalizedExpectedProjectDir.Equals(GetBridgeProjectDir(), ESearchCase::IgnoreCase))
		{
			TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
			DebugContext->SetStringField(TEXT("expected_project_dir"), NormalizedExpectedProjectDir);
			DebugContext->SetStringField(TEXT("actual_project_dir"), GetBridgeProjectDir());
			DebugContext->SetStringField(TEXT("actual_workspace_id"), GetBridgeWorkspaceId());
			DebugContext->SetStringField(TEXT("actual_project_name"), GetBridgeProjectName());
			DebugContext->SetStringField(TEXT("actual_bridge_session_id"), GetBridgeSessionId());
			return BuildErrorResponse(
				Action,
				TEXT("BRIDGE_PROJECT_MISMATCH"),
				TEXT("Bridge request targeted a different project directory."),
				DebugContext);
		}
	}

	FString ExpectedSessionId;
	if (RequestObj->TryGetStringField(TEXT("expected_bridge_session_id"), ExpectedSessionId) &&
		!ExpectedSessionId.TrimStartAndEnd().IsEmpty() &&
		!ExpectedSessionId.Equals(GetBridgeSessionId(), ESearchCase::IgnoreCase))
	{
		TSharedPtr<FJsonObject> DebugContext = MakeShareable(new FJsonObject());
		DebugContext->SetStringField(TEXT("expected_bridge_session_id"), ExpectedSessionId);
		DebugContext->SetStringField(TEXT("actual_bridge_session_id"), GetBridgeSessionId());
		DebugContext->SetStringField(TEXT("actual_workspace_id"), GetBridgeWorkspaceId());
		DebugContext->SetStringField(TEXT("actual_project_dir"), GetBridgeProjectDir());
		return BuildErrorResponse(
			Action,
			TEXT("BRIDGE_SESSION_MISMATCH"),
			TEXT("Bridge request targeted a different editor session."),
			DebugContext);
	}

	if (Action.Equals(TEXT("ping"), ESearchCase::IgnoreCase))
	{
		TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
		Response->SetBoolField(TEXT("ok"), true);
		Response->SetStringField(TEXT("action"), TEXT("ping"));
		Response->SetStringField(TEXT("message"), TEXT("pong"));
		AppendBridgeIdentity(Response);
		return JsonObjectToString(Response);
	}

	if (Action.Equals(TEXT("list_tools"), ESearchCase::IgnoreCase))
	{
		if (!ToolRegistry->AreDefaultToolsRegistered())
		{
			ToolRegistry->RegisterDefaultTools();
		}

		const FString CacheKey = FForgePilotCache::GenerateToolListKey();
		FString CachedResponse;
		if (FForgePilotCache::Get().GetCached(CacheKey, CachedResponse))
		{
			return CachedResponse;
		}

		TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
		Response->SetBoolField(TEXT("ok"), true);
		Response->SetStringField(TEXT("action"), TEXT("list_tools"));
		Response->SetArrayField(TEXT("tools"), ToolRegistry->GetToolDefinitionsJson());
		AppendBridgeIdentity(Response);

		const FString Serialized = JsonObjectToString(Response);
		FForgePilotCache::Get().Put(CacheKey, Serialized, 300.0f);
		return Serialized;
	}

	if (Action.Equals(TEXT("execute_tool"), ESearchCase::IgnoreCase))
	{
		FString ToolName;
		if (!RequestObj->TryGetStringField(TEXT("tool_name"), ToolName) || ToolName.IsEmpty())
		{
			return BuildErrorResponse(
				TEXT("execute_tool"),
				TEXT("BRIDGE_MISSING_TOOL_NAME"),
				TEXT("Missing required field: tool_name"));
		}

		TSharedPtr<FJsonObject> ToolInput = MakeShareable(new FJsonObject());
		const TSharedPtr<FJsonObject>* InputObj = nullptr;
		if (RequestObj->TryGetObjectField(TEXT("input"), InputObj) && InputObj && InputObj->IsValid())
		{
			ToolInput = *InputObj;
		}

		FString ToolResult;
		const bool bSuccess = ToolRegistry->ExecuteTool(ToolName, ToolInput, ToolResult);

		TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
		Response->SetBoolField(TEXT("ok"), bSuccess);
		Response->SetStringField(TEXT("action"), TEXT("execute_tool"));
		Response->SetStringField(TEXT("tool_name"), ToolName);
		AppendBridgeIdentity(Response);

		TSharedPtr<FJsonObject> ResultObj = ParseJsonObject(ToolResult);
		if (ResultObj.IsValid())
		{
			Response->SetObjectField(TEXT("result"), ResultObj);
			if (!bSuccess)
			{
				FString ErrorCode;
				FString ErrorMessage;
				ResultObj->TryGetStringField(TEXT("error_code"), ErrorCode);
				ResultObj->TryGetStringField(TEXT("error_message"), ErrorMessage);
				if (!ErrorCode.IsEmpty())
				{
					Response->SetStringField(TEXT("error_code"), ErrorCode);
				}
				if (!ErrorMessage.IsEmpty())
				{
					Response->SetStringField(TEXT("error"), ErrorMessage);
				}
			}
		}
		else
		{
			if (!bSuccess)
			{
				Response->SetStringField(TEXT("error_code"), TEXT("TOOL_FAILED_TEXT_ONLY"));
				Response->SetStringField(TEXT("error"), ToolResult);
			}
			Response->SetStringField(TEXT("result_text"), ToolResult);
		}

		return JsonObjectToString(Response);
	}

	return BuildErrorResponse(
		Action,
		TEXT("BRIDGE_UNSUPPORTED_ACTION"),
		TEXT("Unsupported action. Valid: ping, list_tools, execute_tool"));
}

void UForgePilotMCPBridgeSubsystem::AppendBridgeIdentity(const TSharedPtr<FJsonObject>& Response) const
{
	if (!Response.IsValid())
	{
		return;
	}

	Response->SetStringField(TEXT("bridge_host"), FPlatformMisc::GetEnvironmentVariable(TEXT("FORGEPILOT_BRIDGE_HOST")));
	Response->SetNumberField(TEXT("bridge_port"), UForgePilotSettings::ResolveMCPServerPort());
	Response->SetStringField(TEXT("project_name"), GetBridgeProjectName());
	Response->SetStringField(TEXT("project_dir"), GetBridgeProjectDir());
	Response->SetStringField(TEXT("workspace_id"), GetBridgeWorkspaceId());
	Response->SetStringField(TEXT("bridge_session_id"), GetBridgeSessionId());
}
