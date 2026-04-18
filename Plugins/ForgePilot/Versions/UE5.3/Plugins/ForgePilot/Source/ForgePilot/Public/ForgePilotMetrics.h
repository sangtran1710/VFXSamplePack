// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"

// ============================================================
// Metrics Types
// ============================================================

enum class EMagicMetricType
{
	Counter,
	Gauge,
	Timer,
	Histogram
};

struct FMetricValue
{
	union
	{
		int64 CounterValue;
		double GaugeValue;
		double TimerValueMs;
		double HistogramValue;
	};

	FMetricValue() : CounterValue(0) {}
};

// Single metric data point
struct FMetricDataPoint
{
	FString Name;
	EMagicMetricType Type;
	FMetricValue Value;
	FDateTime Timestamp;
	TMap<FString, FString> Tags; // e.g., "provider": "Anthropic", "tool": "spawn_actor"
};

// Aggregated metric for reporting
struct FAggregatedMetric
{
	FString Name;
	EMagicMetricType Type;
	int64 Count = 0;
	double Sum = 0.0;
	double Min = 0.0;
	double Max = 0.0;
	TMap<FString, FString> Tags;
};

// ============================================================
// Metrics Collector
// ============================================================

class FForgePilotMetrics
{
public:
	// Singleton access
	static FForgePilotMetrics& Get();

	// Configuration
	void SetEnabled(bool bInEnabled) { bEnabled = bInEnabled; }
	bool IsEnabled() const { return bEnabled; }

	// Metrics recording
	void RecordCounter(const FString& Name, int64 Delta = 1, const TMap<FString, FString>& Tags = {});
	void RecordGauge(const FString& Name, double Value, const TMap<FString, FString>& Tags = {});
	void RecordTimer(const FString& Name, double DurationMs, const TMap<FString, FString>& Tags = {});
	void RecordHistogram(const FString& Name, double Value, const TMap<FString, FString>& Tags = {});

	// Convenience helpers
	void RecordAIRequest(const FString& Provider, const FString& Model);
	void RecordAIResponse(const FString& Provider, const FString& Model, bool bSuccess, int32 ToolCallsCount = 0);
	void RecordToolExecution(const FString& ToolName, double DurationMs, bool bSuccess);
	void RecordToolError(const FString& ToolName, const FString& ErrorCode);

	// Reporting - get aggregated metrics as JSON
	TArray<FAggregatedMetric> GetAggregatedMetrics() const;
	FString GetMetricsJson() const;
	FString GetMetricsJsonSince(FDateTime Since) const;

	// Reset
	void Reset();
	void ResetMetric(const FString& Name);

private:
	FForgePilotMetrics() = default;

	bool bEnabled = true;
	mutable FCriticalSection MetricsLock;
	TArray<FMetricDataPoint> DataPoints;

	// Aggregated cache (rebuilt on request)
	mutable TArray<FAggregatedMetric> CachedAggregates;
	mutable FDateTime CacheTimestamp;
	mutable bool bCacheDirty = true;
};

// ============================================================
// Scoped Timer (RAII)
// ============================================================

class FScopedTimer
{
public:
	FScopedTimer(const FString& MetricName, const TMap<FString, FString>& Tags = {})
		: MetricName(MetricName), Tags(Tags), StartTime(FDateTime::Now())
	{
	}

	~FScopedTimer()
	{
		double DurationMs = (FDateTime::Now() - StartTime).GetTotalMilliseconds();
		FForgePilotMetrics::Get().RecordTimer(MetricName, DurationMs, Tags);
	}

private:
	FString MetricName;
	TMap<FString, FString> Tags;
	FDateTime StartTime;
};

// ============================================================
// Common Metrics Names
// ============================================================

namespace VFXMetrics
{
	// AI metrics
	inline const FString AI_REQUEST_TOTAL = TEXT("forgepilot.ai.request.total");
	inline const FString AI_REQUEST_SUCCESS = TEXT("forgepilot.ai.request.success");
	inline const FString AI_REQUEST_FAILURE = TEXT("forgepilot.ai.request.failure");
	inline const FString AI_REQUEST_DURATION_MS = TEXT("forgepilot.ai.request.duration_ms");
	inline const FString AI_TOOL_CALLS_COUNT = TEXT("forgepilot.ai.tool_calls.count");

	// Tool metrics
	inline const FString TOOL_EXECUTION_TOTAL = TEXT("forgepilot.tool.execution.total");
	inline const FString TOOL_EXECUTION_SUCCESS = TEXT("forgepilot.tool.execution.success");
	inline const FString TOOL_EXECUTION_FAILURE = TEXT("forgepilot.tool.execution.failure");
	inline const FString TOOL_EXECUTION_DURATION_MS = TEXT("forgepilot.tool.execution.duration_ms");

	// Provider-specific
	inline const FString PROVIDER_RETRY_COUNT = TEXT("forgepilot.provider.retry.count");
	inline const FString PROVIDER_ERROR_RATE = TEXT("forgepilot.provider.error.rate");

	// MCP Bridge
	inline const FString BRIDGE_REQUEST_TOTAL = TEXT("forgepilot.bridge.request.total");
	inline const FString BRIDGE_REQUEST_DURATION_MS = TEXT("forgepilot.bridge.request.duration_ms");
}
