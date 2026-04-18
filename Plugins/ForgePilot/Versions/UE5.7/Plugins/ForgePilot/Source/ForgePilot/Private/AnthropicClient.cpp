// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "AnthropicClient.h"
#include "AIProviderAdapters.h"
#include "ForgePilotRateLimiter.h"
#include "VFXToolRegistry.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonValue.h"
#include "Containers/Ticker.h"
#include "Misc/Base64.h"

// ============================================================
// FAIClient
// ============================================================

FAIClient::FAIClient()
{
	FForgePilotRateLimiter::Get().ConfigureProvider(TEXT("Anthropic"), 50);
	FForgePilotRateLimiter::Get().ConfigureProvider(TEXT("OpenAI"), 60);
	FForgePilotRateLimiter::Get().ConfigureProvider(TEXT("OpenRouter"), 40);
	FForgePilotRateLimiter::Get().ConfigureProvider(TEXT("Gemini"), 60);

	SetProvider(EAIProvider::Anthropic);
}

FAIClient::~FAIClient()
{
	CancelRequest();
}

void FAIClient::SetProvider(EAIProvider InProvider)
{
	Provider = InProvider;

	// Create appropriate adapter
	switch (Provider)
	{
	case EAIProvider::Anthropic:
		CurrentAdapter = MakeUnique<FAnthropicAdapter>();
		break;
	case EAIProvider::OpenAI:
		CurrentAdapter = MakeUnique<FOpenAIAdapter>();
		break;
	case EAIProvider::OpenRouter:
		CurrentAdapter = MakeUnique<FOpenRouterAdapter>();
		break;
	case EAIProvider::Gemini:
		CurrentAdapter = MakeUnique<FGeminiAdapter>();
		break;
	default:
		CurrentAdapter = MakeUnique<FAnthropicAdapter>();
		break;
	}

	// Transfer existing config to new adapter
	if (!ApiKey.IsEmpty())
		CurrentAdapter->SetApiKey(ApiKey);
	if (!Model.IsEmpty())
		CurrentAdapter->SetModel(Model);
	if (ToolRegistry.IsValid())
		CurrentAdapter->SetToolRegistry(ToolRegistry);

	// Reset model to provider default
	Model = CurrentAdapter->GetDefaultModel();
}

FString FAIClient::ProviderToString(EAIProvider P)
{
	switch (P)
	{
	case EAIProvider::Anthropic: return TEXT("Anthropic");
	case EAIProvider::OpenAI:    return TEXT("OpenAI");
	case EAIProvider::OpenRouter:return TEXT("OpenRouter");
	case EAIProvider::Gemini:    return TEXT("Gemini");
	default:                     return TEXT("Anthropic");
	}
}

EAIProvider FAIClient::StringToProvider(const FString& S)
{
	if (S == TEXT("OpenAI"))    return EAIProvider::OpenAI;
	if (S == TEXT("OpenRouter"))return EAIProvider::OpenRouter;
	if (S == TEXT("Gemini"))    return EAIProvider::Gemini;
	return EAIProvider::Anthropic;
}

void FAIClient::SetApiKey(const FString& InApiKey)
{
	ApiKey = InApiKey;
	if (CurrentAdapter.IsValid())
		CurrentAdapter->SetApiKey(InApiKey);
}

void FAIClient::SetModel(const FString& InModel)
{
	Model = InModel;
	if (CurrentAdapter.IsValid())
		CurrentAdapter->SetModel(InModel);
}

void FAIClient::SetSystemPrompt(const FString& InSystemPrompt)
{
	SystemPrompt = InSystemPrompt;
}

void FAIClient::SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry)
{
	ToolRegistry = InToolRegistry;
	if (CurrentAdapter.IsValid())
		CurrentAdapter->SetToolRegistry(InToolRegistry);
}

FString FAIClient::GetDefaultModel() const
{
	if (CurrentAdapter.IsValid())
		return CurrentAdapter->GetDefaultModel();
	return TEXT("claude-sonnet-4-20250514");
}

void FAIClient::StartConversation()
{
	ConversationHistory.Empty();
	CurrentIteration = 0;
	CurrentRetryCount = 0;
	bIsBusy = false;
}

void FAIClient::SendMessage(const FString& UserMessage)
{
	if (bIsBusy) return;
	if (ApiKey.IsEmpty())
	{
		OnError.ExecuteIfBound(TEXT("API key is not set. Open Settings to configure."));
		return;
	}

	bIsBusy = true;
	CurrentIteration = 0;
	CurrentRetryCount = 0;

	FAgentMessage UserMsg;
	UserMsg.Role = TEXT("user");
	FAgentContentBlock TextBlock;
	TextBlock.Type = TEXT("text");
	TextBlock.Text = UserMessage;
	UserMsg.Content.Add(TextBlock);
	ConversationHistory.Add(UserMsg);

	SendToApi();
}

void FAIClient::SendMessageWithImage(const FString& UserMessage, const TArray<uint8>& ImageData, const FString& MediaType)
{
	if (bIsBusy) return;
	if (ApiKey.IsEmpty())
	{
		OnError.ExecuteIfBound(TEXT("API key is not set. Open Settings to configure."));
		return;
	}

	bIsBusy = true;
	CurrentIteration = 0;
	CurrentRetryCount = 0;

	FAgentMessage UserMsg;
	UserMsg.Role = TEXT("user");

	// Add image block
	FAgentContentBlock ImageBlock;
	ImageBlock.Type = TEXT("image");
	ImageBlock.ImageBase64 = FBase64::Encode(ImageData);
	ImageBlock.ImageMediaType = MediaType;
	UserMsg.Content.Add(ImageBlock);

	// Add text block
	if (!UserMessage.IsEmpty())
	{
		FAgentContentBlock TextBlock;
		TextBlock.Type = TEXT("text");
		TextBlock.Text = UserMessage;
		UserMsg.Content.Add(TextBlock);
	}

	ConversationHistory.Add(UserMsg);
	SendToApi();
}

void FAIClient::CancelRequest()
{
	if (ActiveRequest.IsValid())
	{
		ActiveRequest->CancelRequest();
		ActiveRequest.Reset();
	}
	if (RetryTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RetryTickerHandle);
		RetryTickerHandle.Reset();
	}
	bIsBusy = false;
}

// ============================================================
// Core Loop
// ============================================================

void FAIClient::SendToApi()
{
	if (!CurrentAdapter.IsValid())
	{
		bIsBusy = false;
		OnError.ExecuteIfBound(TEXT("No provider adapter configured."));
		return;
	}

	FString Url;
	TMap<FString, FString> Headers;
	FString Body;

	// Delegate to adapter
	CurrentAdapter->BuildRequest(ConversationHistory, SystemPrompt, Url, Headers, Body);
	const FString ProviderName = CurrentAdapter->GetProviderName();

	if (!FForgePilotRateLimiter::Get().CanMakeRequest(ProviderName))
	{
		const float WaitSeconds = FMath::Max(FForgePilotRateLimiter::Get().GetWaitTime(ProviderName), 0.1f);

		if (RetryTickerHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(RetryTickerHandle);
			RetryTickerHandle.Reset();
		}

		RetryTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateLambda([this](float)
			{
				RetryTickerHandle.Reset();
				SendToApi();
				return false;
			}),
			WaitSeconds);
		return;
	}

	ActiveRequest = FHttpModule::Get().CreateRequest();
	ActiveRequest->SetTimeout(CurrentAdapter->GetTimeoutSeconds());
	ActiveRequest->SetVerb(TEXT("POST"));
	ActiveRequest->SetURL(Url);
	for (const auto& H : Headers)
	{
		ActiveRequest->SetHeader(H.Key, H.Value);
	}
	ActiveRequest->SetContentAsString(Body);
	ActiveRequest->OnProcessRequestComplete().BindRaw(this, &FAIClient::OnHttpResponseReceived);
	ActiveRequest->ProcessRequest();
	FForgePilotRateLimiter::Get().RecordRequest(ProviderName);

	LastRequestTime = FDateTime::Now();
}

void FAIClient::OnHttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ActiveRequest.Reset();

	if (!bWasSuccessful || !Response.IsValid())
	{
		// Network failure - try retry
		if (ShouldRetry(0))
		{
			HandleRetry();
			return;
		}

		bIsBusy = false;
		OnError.ExecuteIfBound(TEXT("Connection failed. Check your network."));
		return;
	}

	int32 Code = Response->GetResponseCode();
	FString ErrorBody = Response->GetContentAsString();

	// Check for retryable errors
	if (ShouldRetry(Code))
	{
		HandleRetry();
		return;
	}

	if (Code != 200)
	{
		bIsBusy = false;
		FString ErrorMsg = CurrentAdapter.IsValid()
			? CurrentAdapter->ParseError(Code, ErrorBody)
			: FString::Printf(TEXT("API Error %d: %s"), Code, *ErrorBody.Left(500));
		OnError.ExecuteIfBound(ErrorMsg);
		return;
	}

	// Parse successful response
	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (!FJsonSerializer::Deserialize(Reader, Json) || !Json.IsValid())
	{
		bIsBusy = false;
		OnError.ExecuteIfBound(TEXT("Failed to parse API response."));
		return;
	}

	// Delegate to adapter for parsing
	TArray<FAgentContentBlock> Content;
	FString TextOutput;
	bool bHasToolCalls = false;

	if (!CurrentAdapter->ParseResponse(Json, Content, TextOutput, bHasToolCalls))
	{
		bIsBusy = false;
		OnError.ExecuteIfBound(TEXT("Failed to parse provider response."));
		return;
	}

	AppendAssistantMessage(Content);

	// Emit text output
	if (!TextOutput.IsEmpty())
	{
		OnTextOutput.ExecuteIfBound(TextOutput);
	}

	// Process tool calls
	if (bHasToolCalls && Content.Num() > 0)
	{
		// Extract tool calls
		TArray<FAgentContentBlock> ToolCalls;
		for (const auto& C : Content)
		{
			if (C.Type == TEXT("tool_use"))
			{
				ToolCalls.Add(C);
			}
		}

		if (ToolCalls.Num() > 0)
		{
			CurrentIteration++;
			if (CurrentIteration >= MaxIterations)
			{
				bIsBusy = false;
				OnError.ExecuteIfBound(TEXT("Safety limit: max 25 tool iterations reached."));
				return;
			}
			ExecuteToolCalls(ToolCalls);
			return;
		}
	}

	// No tool calls - complete
	bIsBusy = false;
	OnLoopComplete.ExecuteIfBound();
}

void FAIClient::ExecuteToolCalls(const TArray<FAgentContentBlock>& ToolUseBlocks)
{
	FAgentMessage ResultMsg;
	ResultMsg.Role = TEXT("user");

	for (const FAgentContentBlock& ToolCall : ToolUseBlocks)
	{
		OnToolExecution.ExecuteIfBound(ToolCall.ToolName,
			FString::Printf(TEXT("Executing: %s"), *ToolCall.ToolName));

		FAgentContentBlock ResultBlock;
		ResultBlock.Type = TEXT("tool_result");
		ResultBlock.ToolResultId = ToolCall.ToolUseId;
		ResultBlock.ToolName = ToolCall.ToolName; // needed for Gemini

		if (ToolRegistry.IsValid())
		{
			FString OutResult;
			bool bSuccess = ToolRegistry->ExecuteTool(ToolCall.ToolName, ToolCall.ToolInput, OutResult);
			ResultBlock.ResultContent = OutResult;
			ResultBlock.bIsError = !bSuccess;
		}
		else
		{
			ResultBlock.ResultContent = TEXT("Error: Tool registry not available.");
			ResultBlock.bIsError = true;
		}

		ResultMsg.Content.Add(ResultBlock);
	}

	ConversationHistory.Add(ResultMsg);
	SendToApi();
}

void FAIClient::AppendAssistantMessage(const TArray<FAgentContentBlock>& Content)
{
	if (Content.Num() == 0)
	{
		return;
	}

	FAgentMessage AssistantMsg;
	AssistantMsg.Role = TEXT("assistant");
	AssistantMsg.Content = Content;
	ConversationHistory.Add(AssistantMsg);
}

// ============================================================
// Retry Logic
// ============================================================

bool FAIClient::ShouldRetry(int32 ResponseCode) const
{
	if (!CurrentAdapter.IsValid()) return false;

	int32 MaxRetries = CurrentAdapter->GetMaxRetries();
	if (CurrentRetryCount >= MaxRetries)
		return false;

	// Retry on network errors (ResponseCode = 0)
	if (ResponseCode == 0)
		return true;

	// Retry on rate limiting (429) and server errors (5xx)
	if (ResponseCode == 429 || (ResponseCode >= 500 && ResponseCode < 600))
		return true;

	return false;
}

float FAIClient::GetBackoffDelay() const
{
	if (!CurrentAdapter.IsValid())
		return 1.0f;

	float Initial = CurrentAdapter->GetInitialBackoffSeconds();
	float Max = CurrentAdapter->GetMaxBackoffSeconds();

	// Exponential backoff: initial * 2^retry
	float Delay = Initial * FMath::Pow(2.0f, CurrentRetryCount);
	return FMath::Min(Delay, Max);
}


void FAIClient::HandleRetry()
{
	float Delay = GetBackoffDelay();

	++CurrentRetryCount;

	if (RetryTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RetryTickerHandle);
		RetryTickerHandle.Reset();
	}

	RetryTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([this](float)
		{
			RetryTickerHandle.Reset();
			SendToApi();
			return false;
		}),
		Delay);
}
