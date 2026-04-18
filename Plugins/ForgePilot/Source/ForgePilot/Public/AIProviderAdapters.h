// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "AnthropicClient.h"

/**
 * Anthropic (Claude) Adapter
 */
class FAnthropicAdapter : public IAIProviderAdapter
{
public:
	FAnthropicAdapter();

	// IAIProviderAdapter
	virtual void SetApiKey(const FString& InApiKey) override;
	virtual void SetModel(const FString& InModel) override;
	virtual void SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry) override;
	virtual FString GetDefaultModel() const override;
	virtual FString GetProviderName() const override;

	virtual void BuildRequest(
		const TArray<FAgentMessage>& ConversationHistory,
		const FString& SystemPrompt,
		FString& OutUrl,
		TMap<FString, FString>& OutHeaders,
		FString& OutBody) override;

	virtual bool ParseResponse(
		TSharedPtr<FJsonObject> Json,
		TArray<FAgentContentBlock>& OutContent,
		FString& OutTextOutput,
		bool& bOutHasToolCalls) override;

	virtual FString ParseError(int32 ResponseCode, const FString& ErrorBody) const override;

private:
	TArray<TSharedPtr<FJsonValue>> BuildTools() const;
};

/**
 * OpenAI Adapter
 */
class FOpenAIAdapter : public IAIProviderAdapter
{
public:
	FOpenAIAdapter();

	// IAIProviderAdapter
	virtual void SetApiKey(const FString& InApiKey) override;
	virtual void SetModel(const FString& InModel) override;
	virtual void SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry) override;
	virtual FString GetDefaultModel() const override;
	virtual FString GetProviderName() const override;

	virtual void BuildRequest(
		const TArray<FAgentMessage>& ConversationHistory,
		const FString& SystemPrompt,
		FString& OutUrl,
		TMap<FString, FString>& OutHeaders,
		FString& OutBody) override;

	virtual bool ParseResponse(
		TSharedPtr<FJsonObject> Json,
		TArray<FAgentContentBlock>& OutContent,
		FString& OutTextOutput,
		bool& bOutHasToolCalls) override;

	virtual FString ParseError(int32 ResponseCode, const FString& ErrorBody) const override;

private:
	TArray<TSharedPtr<FJsonValue>> BuildTools() const;
};

/**
 * OpenRouter Adapter (uses OpenAI-compatible API)
 */
class FOpenRouterAdapter : public FOpenAIAdapter
{
public:
	FOpenRouterAdapter();

	virtual FString GetDefaultModel() const override;
	virtual FString GetProviderName() const override;

	virtual void BuildRequest(
		const TArray<FAgentMessage>& ConversationHistory,
		const FString& SystemPrompt,
		FString& OutUrl,
		TMap<FString, FString>& OutHeaders,
		FString& OutBody) override;

	virtual FString ParseError(int32 ResponseCode, const FString& ErrorBody) const override;

	virtual int32 GetMaxRetries() const override { return 5; } // More retries for routing
	virtual float GetInitialBackoffSeconds() const override { return 2.0f; }
};

/**
 * Gemini Adapter
 */
class FGeminiAdapter : public IAIProviderAdapter
{
public:
	FGeminiAdapter();

	// IAIProviderAdapter
	virtual void SetApiKey(const FString& InApiKey) override;
	virtual void SetModel(const FString& InModel) override;
	virtual void SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry) override;
	virtual FString GetDefaultModel() const override;
	virtual FString GetProviderName() const override;

	virtual void BuildRequest(
		const TArray<FAgentMessage>& ConversationHistory,
		const FString& SystemPrompt,
		FString& OutUrl,
		TMap<FString, FString>& OutHeaders,
		FString& OutBody) override;

	virtual bool ParseResponse(
		TSharedPtr<FJsonObject> Json,
		TArray<FAgentContentBlock>& OutContent,
		FString& OutTextOutput,
		bool& bOutHasToolCalls) override;

	virtual FString ParseError(int32 ResponseCode, const FString& ErrorBody) const override;

private:
	TArray<TSharedPtr<FJsonValue>> BuildTools() const;
	FString GenerateToolCallId() const;
};
