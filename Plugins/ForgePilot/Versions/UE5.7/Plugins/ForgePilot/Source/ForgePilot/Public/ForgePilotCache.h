// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Containers/LruCache.h"

/**
 * Selective response cache for ForgePilot
 * Only caches safe, idempotent requests:
 * - Tool list/schema requests
 * - Asset scanning (read-only)
 * - Stable read-only registry/search queries
 *
 * DOES NOT cache:
 * - Anything with editor state (selections, viewport)
 * - Tool execution results
 * - Asset modification operations
 */
class FForgePilotCache
{
public:
	// Singleton
	static FForgePilotCache& Get();

	// Configuration
	void SetEnabled(bool bInEnabled) { bEnabled = bInEnabled; }
	bool IsEnabled() const { return bEnabled; }
	void SetMaxSize(int32 InMaxSize) { MaxCacheSize = InMaxSize; }

	// Cache operations
	/**
	 * Get cached response if exists and valid
	 * @param CacheKey - Hash of the request
	 * @param OutResponse - Output cached response
	 * @return true if cache hit
	 */
	bool GetCached(const FString& CacheKey, FString& OutResponse) const;

	/**
	 * Cache a response
	 * @param CacheKey - Hash of the request
	 * @param Response - Response to cache
	 * @param TTLSeconds - Time to live in seconds (default 5 minutes)
	 */
	void Put(const FString& CacheKey, const FString& Response, float TTLSeconds = 300.0f);

	/**
	 * Invalidate a specific cache entry
	 */
	void Invalidate(const FString& CacheKey);

	/**
	 * Clear all cache
	 */
	void Clear();

	/**
	 * Get cache statistics
	 */
	int32 GetCacheSize() const { return Cache.Num(); }
	int32 GetCacheHits() const { return HitCount; }
	int32 GetCacheMisses() const { return MissCount; }

	/**
	 * Generate cache key for tool list request
	 */
	static FString GenerateToolListKey();

	/**
	 * Generate cache key for asset scan request
	 */
	static FString GenerateAssetScanKey(const FString& FolderPath, const FString& AssetType, int32 MaxResults);

	/**
	 * Generate cache key for context query
	 */
	static FString GenerateContextKey();

	/**
	 * Check if request is cacheable
	 * @param ToolName - Name of tool being called
	 * @param InputJson - Input parameters (optional)
	 * @return true if safe to cache
	 */
	static bool IsCacheable(const FString& ToolName, const TSharedPtr<FJsonObject>* InputJson = nullptr);

private:
	FForgePilotCache() = default;

	struct FCachedEntry
	{
		FString Response;
		FDateTime ExpiryTime;
	};

	bool bEnabled = true;
	int32 MaxCacheSize = 100;
	mutable TMap<FString, FCachedEntry> Cache;
	mutable int32 HitCount = 0;
	mutable int32 MissCount = 0;

	mutable FCriticalSection CacheLock;
};

// Inline implementation for IsCacheable
inline bool FForgePilotCache::IsCacheable(const FString& ToolName, const TSharedPtr<FJsonObject>* InputJson)
{
	// Always cacheable: tool list/schema
	if (ToolName == TEXT("get_tool_list") ||
		ToolName == TEXT("get_tools") ||
		ToolName == TEXT("list_tools"))
	{
		return true;
	}

	// Cacheable with no modification intent
	if (ToolName == TEXT("scan_assets") ||
		ToolName == TEXT("search_assets"))
	{
		if (InputJson && InputJson->IsValid())
		{
			bool bApplyChanges = false;
			if ((*InputJson)->TryGetBoolField(TEXT("apply_changes"), bApplyChanges) && bApplyChanges)
			{
				return false;
			}
		}
		return true;
	}

	// Everything else: don't cache (modifications, tool execution)
	return false;
}
