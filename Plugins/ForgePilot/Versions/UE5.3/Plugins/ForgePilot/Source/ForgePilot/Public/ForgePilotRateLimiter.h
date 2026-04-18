// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformProcess.h"

/**
 * Rate limiter for API requests
 * Implements token bucket algorithm with per-provider limits
 */
class FForgePilotRateLimiter
{
public:
	// Singleton
	static FForgePilotRateLimiter& Get();

	/**
	 * Configure rate limits for a provider
	 * @param ProviderName - Provider identifier (Anthropic, OpenAI, etc.)
	 * @param MaxRequestsPerMinute - Maximum requests allowed per minute
	 * @param MaxTokensPerMinute - Maximum tokens allowed per minute (0 = unlimited)
	 */
	void ConfigureProvider(const FString& ProviderName, int32 MaxRequestsPerMinute, int32 MaxTokensPerMinute = 0);

	/**
	 * Check if request is allowed
	 * @param ProviderName - Provider identifier
	 * @param EstimatedTokens - Estimated tokens for this request (0 = don't check token limit)
	 * @return true if request can proceed
	 */
	bool CanMakeRequest(const FString& ProviderName, int32 EstimatedTokens = 0) const;

	/**
	 * Record a completed request (for rate limiting)
	 * @param ProviderName - Provider identifier
	 * @param TokensUsed - Actual tokens used in request
	 */
	void RecordRequest(const FString& ProviderName, int32 TokensUsed = 0);

	/**
	 * Get wait time until next request is allowed
	 * @param ProviderName - Provider identifier
	 * @return Wait time in seconds, or 0 if can request now
	 */
	float GetWaitTime(const FString& ProviderName) const;

	/**
	 * Block until request is allowed (with optional timeout)
	 * @param ProviderName - Provider identifier
	 * @param EstimatedTokens - Estimated tokens for this request
	 * @param MaxWaitSeconds - Maximum seconds to wait (0 = wait forever)
	 * @return true if request allowed, false if timeout
	 */
	bool WaitForRequest(const FString& ProviderName, int32 EstimatedTokens = 0, float MaxWaitSeconds = 0.0f);

	/**
	 * Get current request count for a provider
	 */
	int32 GetCurrentRequestCount(const FString& ProviderName) const;

	/**
	 * Get current token usage for a provider
	 */
	int32 GetCurrentTokenUsage(const FString& ProviderName) const;

	/**
	 * Reset rate limits for a provider (use after quota reset)
	 */
	void ResetProvider(const FString& ProviderName);

	/**
	 * Reset all rate limits
	 */
	void ResetAll();

	/**
	 * Set enabled state
	 */
	void SetEnabled(bool bInEnabled) { bEnabled = bInEnabled; }
	bool IsEnabled() const { return bEnabled; }

private:
	FForgePilotRateLimiter() = default;

	struct FProviderLimits
	{
		int32 MaxRequestsPerMinute = 60;
		int32 MaxTokensPerMinute = 0; // 0 = unlimited
		int32 CurrentRequests = 0;
		int32 CurrentTokens = 0;
		FDateTime WindowStart;
	};

	mutable FCriticalSection LimitsLock;
	TMap<FString, FProviderLimits> ProviderLimits;
	bool bEnabled = true;
};
