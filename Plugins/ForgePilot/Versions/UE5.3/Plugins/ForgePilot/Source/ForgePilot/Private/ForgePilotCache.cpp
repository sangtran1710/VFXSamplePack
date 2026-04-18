// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotCache.h"
#include "ForgePilotLogging.h"
#include "Misc/SecureHash.h"

FForgePilotCache& FForgePilotCache::Get()
{
	static FForgePilotCache Instance;
	return Instance;
}

bool FForgePilotCache::GetCached(const FString& CacheKey, FString& OutResponse) const
{
	if (!bEnabled) return false;

	FScopeLock Lock(&CacheLock);

	const FCachedEntry* Entry = Cache.Find(CacheKey);
	if (!Entry)
	{
		MissCount++;
		return false;
	}

	// Check expiry
	if (FDateTime::Now() > Entry->ExpiryTime)
	{
		// Expired - remove
		Cache.Remove(CacheKey);
		MissCount++;
		return false;
	}

	// Cache hit
	HitCount++;
	OutResponse = Entry->Response;
	return true;
}

void FForgePilotCache::Put(const FString& CacheKey, const FString& Response, float TTLSeconds)
{
	if (!bEnabled) return;
	if (CacheKey.IsEmpty() || Response.IsEmpty()) return;

	FScopeLock Lock(&CacheLock);

	// Evict old entries if at capacity
	if (Cache.Num() >= MaxCacheSize)
	{
		// Remove the entry with the earliest expiry time.
		FString OldestKey;
		FDateTime OldestTime;
		bool bHasOldest = false;

		for (const auto& Pair : Cache)
		{
			if (!bHasOldest || Pair.Value.ExpiryTime < OldestTime)
			{
				OldestTime = Pair.Value.ExpiryTime;
				OldestKey = Pair.Key;
				bHasOldest = true;
			}
		}

		if (!OldestKey.IsEmpty())
		{
			Cache.Remove(OldestKey);
		}
	}

	// Add new entry
	FCachedEntry Entry;
	Entry.Response = Response;
	Entry.ExpiryTime = FDateTime::Now() + FTimespan::FromSeconds(TTLSeconds);

	Cache.Add(CacheKey, Entry);
}

void FForgePilotCache::Invalidate(const FString& CacheKey)
{
	FScopeLock Lock(&CacheLock);
	Cache.Remove(CacheKey);
}

void FForgePilotCache::Clear()
{
	FScopeLock Lock(&CacheLock);
	Cache.Empty();
	HitCount = 0;
	MissCount = 0;
}

FString FForgePilotCache::GenerateToolListKey()
{
	// Tool list is always the same for a given registry state
	return TEXT("tool_list_v1");
}

FString FForgePilotCache::GenerateAssetScanKey(const FString& FolderPath, const FString& AssetType, int32 MaxResults)
{
	// Create deterministic key from parameters
	FString Input = FString::Printf(TEXT("scan:%s:%s:%d"), *FolderPath, *AssetType, MaxResults);

	// Simple hash for cache key
	uint32 Hash = GetTypeHash(Input);
	return FString::Printf(TEXT("asset_scan_%u"), Hash);
}

FString FForgePilotCache::GenerateContextKey()
{
	// Context depends on current editor state - use timestamp rounded to 10s
	// This gives some caching but not too aggressive
	int64 Seconds = FDateTime::Now().ToUnixTimestamp() / 10;
	return FString::Printf(TEXT("context_%lld"), Seconds);
}
