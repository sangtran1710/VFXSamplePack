// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotRateLimiter.h"
#include "ForgePilotLogging.h"

FForgePilotRateLimiter& FForgePilotRateLimiter::Get()
{
	static FForgePilotRateLimiter Instance;
	return Instance;
}

void FForgePilotRateLimiter::ConfigureProvider(const FString& ProviderName, int32 MaxRequestsPerMinute, int32 MaxTokensPerMinute)
{
	FScopeLock Lock(&LimitsLock);

	FProviderLimits& Limits = ProviderLimits.FindOrAdd(ProviderName);
	Limits.MaxRequestsPerMinute = MaxRequestsPerMinute;
	Limits.MaxTokensPerMinute = MaxTokensPerMinute;
	Limits.WindowStart = FDateTime::Now();
	Limits.CurrentRequests = 0;
	Limits.CurrentTokens = 0;
}

bool FForgePilotRateLimiter::CanMakeRequest(const FString& ProviderName, int32 EstimatedTokens) const
{
	if (!bEnabled) return true;

	FScopeLock Lock(&LimitsLock);

	const FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	if (!Limits) return true; // No limits configured for this provider

	// Check if window has expired
	FDateTime Now = FDateTime::Now();
	FTimespan Elapsed = Now - Limits->WindowStart;

	if (Elapsed.GetTotalMinutes() >= 1.0)
	{
		// Window expired, reset
		return true;
	}

	// Check request limit
	if (Limits->CurrentRequests >= Limits->MaxRequestsPerMinute)
	{
		return false;
	}

	// Check token limit (if applicable)
	if (Limits->MaxTokensPerMinute > 0 && EstimatedTokens > 0)
	{
		if (Limits->CurrentTokens + EstimatedTokens > Limits->MaxTokensPerMinute)
		{
			return false;
		}
	}

	return true;
}

void FForgePilotRateLimiter::RecordRequest(const FString& ProviderName, int32 TokensUsed)
{
	if (!bEnabled) return;

	FScopeLock Lock(&LimitsLock);

	FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	if (!Limits) return;

	// Check if window has expired
	FDateTime Now = FDateTime::Now();
	FTimespan Elapsed = Now - Limits->WindowStart;

	if (Elapsed.GetTotalMinutes() >= 1.0)
	{
		// Window expired, reset
		Limits->CurrentRequests = 0;
		Limits->CurrentTokens = 0;
		Limits->WindowStart = Now;
	}

	Limits->CurrentRequests++;
	Limits->CurrentTokens += TokensUsed;
}

float FForgePilotRateLimiter::GetWaitTime(const FString& ProviderName) const
{
	if (!bEnabled) return 0.0f;

	FScopeLock Lock(&LimitsLock);

	const FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	if (!Limits) return 0.0f;

	FDateTime Now = FDateTime::Now();
	FTimespan Elapsed = Now - Limits->WindowStart;

	if (Elapsed.GetTotalMinutes() >= 1.0)
	{
		return 0.0f;
	}

	// Calculate remaining time in window
	float RemainingSeconds = 60.0f - (float)Elapsed.GetTotalSeconds();
	return FMath::Max(0.0f, RemainingSeconds);
}

bool FForgePilotRateLimiter::WaitForRequest(const FString& ProviderName, int32 EstimatedTokens, float MaxWaitSeconds)
{
	if (!bEnabled) return true;

	float TotalWaited = 0.0f;
	const float CheckInterval = 0.5f; // Check every 500ms

	while (true)
	{
		if (CanMakeRequest(ProviderName, EstimatedTokens))
		{
			return true;
		}

		float WaitTime = GetWaitTime(ProviderName);
		if (WaitTime <= 0.0f)
		{
			// Try again immediately
			continue;
		}

		float SleepTime = FMath::Min(WaitTime, CheckInterval);
		FPlatformProcess::Sleep(SleepTime);
		TotalWaited += SleepTime;

		// Check timeout
		if (MaxWaitSeconds > 0.0f && TotalWaited >= MaxWaitSeconds)
		{
			return false;
		}
	}
}

int32 FForgePilotRateLimiter::GetCurrentRequestCount(const FString& ProviderName) const
{
	FScopeLock Lock(&LimitsLock);

	const FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	return Limits ? Limits->CurrentRequests : 0;
}

int32 FForgePilotRateLimiter::GetCurrentTokenUsage(const FString& ProviderName) const
{
	FScopeLock Lock(&LimitsLock);

	const FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	return Limits ? Limits->CurrentTokens : 0;
}

void FForgePilotRateLimiter::ResetProvider(const FString& ProviderName)
{
	FScopeLock Lock(&LimitsLock);

	FProviderLimits* Limits = ProviderLimits.Find(ProviderName);
	if (Limits)
	{
		Limits->CurrentRequests = 0;
		Limits->CurrentTokens = 0;
		Limits->WindowStart = FDateTime::Now();
	}
}

void FForgePilotRateLimiter::ResetAll()
{
	FScopeLock Lock(&LimitsLock);

	for (auto& Pair : ProviderLimits)
	{
		Pair.Value.CurrentRequests = 0;
		Pair.Value.CurrentTokens = 0;
		Pair.Value.WindowStart = FDateTime::Now();
	}
}
