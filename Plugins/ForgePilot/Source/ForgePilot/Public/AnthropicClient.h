// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Interfaces/IHttpRequest.h"
#include "Dom/JsonObject.h"

class FVFXToolRegistry;

// ============================================================
// Provider enum
// ============================================================
enum class EAIProvider : uint8
{
	Anthropic,
	OpenAI,
	OpenRouter,
	Gemini
};

// ============================================================
// Internal message format (provider-agnostic)
// ============================================================
struct FAgentContentBlock
{
	FString Type; // "text", "tool_use", "tool_result", "image"

	// text
	FString Text;

	// image (base64 encoded)
	FString ImageBase64;
	FString ImageMediaType; // "image/png", "image/jpeg"

	// tool_use
	FString ToolUseId;
	FString ToolName;
	TSharedPtr<FJsonObject> ToolInput;

	// tool_result
	FString ToolResultId;
	FString ResultContent;
	bool bIsError = false;
};

struct FAgentMessage
{
	FString Role; // "user" or "assistant"
	TArray<FAgentContentBlock> Content;
};

// ============================================================
// Delegates
// ============================================================
DECLARE_DELEGATE_OneParam(FOnAgentTextOutput, const FString&);
DECLARE_DELEGATE(FOnAgentLoopComplete);
DECLARE_DELEGATE_OneParam(FOnAgentError, const FString&);
DECLARE_DELEGATE_TwoParams(FOnToolExecution, const FString&, const FString&);

// ============================================================
// Provider Adapter Interface
// ============================================================
class IAIProviderAdapter
{
public:
	virtual ~IAIProviderAdapter() = default;

	// Configuration
	virtual void SetApiKey(const FString& InApiKey) = 0;
	virtual void SetModel(const FString& InModel) = 0;
	virtual void SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry) = 0;
	virtual FString GetDefaultModel() const = 0;
	virtual FString GetProviderName() const = 0;

	// Request building
	virtual void BuildRequest(
		const TArray<FAgentMessage>& ConversationHistory,
		const FString& SystemPrompt,
		FString& OutUrl,
		TMap<FString, FString>& OutHeaders,
		FString& OutBody) = 0;

	// Response parsing
	virtual bool ParseResponse(
		TSharedPtr<FJsonObject> Json,
		TArray<FAgentContentBlock>& OutContent,
		FString& OutTextOutput,
		bool& bOutHasToolCalls) = 0;

	// Error parsing
	virtual FString ParseError(int32 ResponseCode, const FString& ErrorBody) const = 0;

	// Retry configuration
	virtual int32 GetMaxRetries() const { return 3; }
	virtual float GetInitialBackoffSeconds() const { return 1.0f; }
	virtual float GetMaxBackoffSeconds() const { return 30.0f; }
	virtual float GetTimeoutSeconds() const { return 120.0f; }

protected:
	FString ApiKey;
	FString Model;
	TSharedPtr<FVFXToolRegistry> ToolRegistry;
};

// ============================================================
// AI Client (simplified, delegates to adapter)
// ============================================================
class FAIClient
{
public:
	FAIClient();
	~FAIClient();

	// Provider switching
	void SetProvider(EAIProvider InProvider);
	EAIProvider GetProvider() const { return Provider; }
	static FString ProviderToString(EAIProvider P);
	static EAIProvider StringToProvider(const FString& S);

	// Configuration
	void SetApiKey(const FString& InApiKey);
	void SetModel(const FString& InModel);
	void SetSystemPrompt(const FString& InSystemPrompt);
	void SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry);
	FString GetDefaultModel() const;

	// Conversation
	void StartConversation();
	void SendMessage(const FString& UserMessage);
	void SendMessageWithImage(const FString& UserMessage, const TArray<uint8>& ImageData, const FString& MediaType);
	void CancelRequest();
	bool IsBusy() const { return bIsBusy; }

	// Callbacks
	FOnAgentTextOutput OnTextOutput;
	FOnAgentLoopComplete OnLoopComplete;
	FOnAgentError OnError;
	FOnToolExecution OnToolExecution;

private:
	// HTTP handling
	void SendToApi();
	void OnHttpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ExecuteToolCalls(const TArray<FAgentContentBlock>& ToolUseBlocks);
	void AppendAssistantMessage(const TArray<FAgentContentBlock>& Content);

	// Retry logic
	void HandleRetry();
	bool ShouldRetry(int32 ResponseCode) const;
	float GetBackoffDelay() const;

	// State
	EAIProvider Provider = EAIProvider::Anthropic;
	TUniquePtr<IAIProviderAdapter> CurrentAdapter;

	FString ApiKey;
	FString Model;
	FString SystemPrompt;

	TArray<FAgentMessage> ConversationHistory;
	bool bIsBusy = false;
	int32 CurrentIteration = 0;
	int32 CurrentRetryCount = 0;
	static constexpr int32 MaxIterations = 25;

	TSharedPtr<FVFXToolRegistry> ToolRegistry;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> ActiveRequest;
	FTSTicker::FDelegateHandle RetryTickerHandle;

	// Retry state
	FDateTime LastRequestTime;
};
