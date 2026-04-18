// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotMetrics.h"
#include "ForgePilotLogging.h"
#include "Serialization/JsonSerializer.h"

// ============================================================
// FForgePilotMetrics Implementation
// ============================================================

FForgePilotMetrics& FForgePilotMetrics::Get()
{
	static FForgePilotMetrics Instance;
	return Instance;
}

void FForgePilotMetrics::RecordCounter(const FString& Name, int64 Delta, const TMap<FString, FString>& Tags)
{
	if (!bEnabled) return;

	FScopeLock Lock(&MetricsLock);

	FMetricDataPoint DataPoint;
	DataPoint.Name = Name;
	DataPoint.Type = EMagicMetricType::Counter;
	DataPoint.Value.CounterValue = Delta;
	DataPoint.Timestamp = FDateTime::Now();
	DataPoint.Tags = Tags;

	DataPoints.Add(DataPoint);
	bCacheDirty = true;
}

void FForgePilotMetrics::RecordGauge(const FString& Name, double Value, const TMap<FString, FString>& Tags)
{
	if (!bEnabled) return;

	FScopeLock Lock(&MetricsLock);

	FMetricDataPoint DataPoint;
	DataPoint.Name = Name;
	DataPoint.Type = EMagicMetricType::Gauge;
	DataPoint.Value.GaugeValue = Value;
	DataPoint.Timestamp = FDateTime::Now();
	DataPoint.Tags = Tags;

	DataPoints.Add(DataPoint);
	bCacheDirty = true;
}

void FForgePilotMetrics::RecordTimer(const FString& Name, double DurationMs, const TMap<FString, FString>& Tags)
{
	if (!bEnabled) return;

	FScopeLock Lock(&MetricsLock);

	FMetricDataPoint DataPoint;
	DataPoint.Name = Name;
	DataPoint.Type = EMagicMetricType::Timer;
	DataPoint.Value.TimerValueMs = DurationMs;
	DataPoint.Timestamp = FDateTime::Now();
	DataPoint.Tags = Tags;

	DataPoints.Add(DataPoint);
	bCacheDirty = true;
}

void FForgePilotMetrics::RecordHistogram(const FString& Name, double Value, const TMap<FString, FString>& Tags)
{
	if (!bEnabled) return;

	FScopeLock Lock(&MetricsLock);

	FMetricDataPoint DataPoint;
	DataPoint.Name = Name;
	DataPoint.Type = EMagicMetricType::Histogram;
	DataPoint.Value.HistogramValue = Value;
	DataPoint.Timestamp = FDateTime::Now();
	DataPoint.Tags = Tags;

	DataPoints.Add(DataPoint);
	bCacheDirty = true;
}

void FForgePilotMetrics::RecordAIRequest(const FString& Provider, const FString& Model)
{
	TMap<FString, FString> Tags;
	Tags.Add(TEXT("provider"), Provider);
	Tags.Add(TEXT("model"), Model);

	RecordCounter(VFXMetrics::AI_REQUEST_TOTAL, 1, Tags);
}

void FForgePilotMetrics::RecordAIResponse(const FString& Provider, const FString& Model, bool bSuccess, int32 ToolCallsCount)
{
	TMap<FString, FString> Tags;
	Tags.Add(TEXT("provider"), Provider);
	Tags.Add(TEXT("model"), Model);

	if (bSuccess)
	{
		RecordCounter(VFXMetrics::AI_REQUEST_SUCCESS, 1, Tags);
	}
	else
	{
		RecordCounter(VFXMetrics::AI_REQUEST_FAILURE, 1, Tags);
	}

	if (ToolCallsCount > 0)
	{
		RecordCounter(VFXMetrics::AI_TOOL_CALLS_COUNT, ToolCallsCount, Tags);
	}
}

void FForgePilotMetrics::RecordToolExecution(const FString& ToolName, double DurationMs, bool bSuccess)
{
	TMap<FString, FString> Tags;
	Tags.Add(TEXT("tool"), ToolName);

	RecordCounter(VFXMetrics::TOOL_EXECUTION_TOTAL, 1, Tags);
	RecordTimer(VFXMetrics::TOOL_EXECUTION_DURATION_MS, DurationMs, Tags);

	if (bSuccess)
	{
		RecordCounter(VFXMetrics::TOOL_EXECUTION_SUCCESS, 1, Tags);
	}
	else
	{
		RecordCounter(VFXMetrics::TOOL_EXECUTION_FAILURE, 1, Tags);
	}
}

void FForgePilotMetrics::RecordToolError(const FString& ToolName, const FString& ErrorCode)
{
	TMap<FString, FString> Tags;
	Tags.Add(TEXT("tool"), ToolName);
	Tags.Add(TEXT("error_code"), ErrorCode);

	RecordCounter(VFXMetrics::TOOL_EXECUTION_FAILURE, 1, Tags);
}

TArray<FAggregatedMetric> FForgePilotMetrics::GetAggregatedMetrics() const
{
	FScopeLock Lock(&MetricsLock);

	// Rebuild cache if dirty
	if (bCacheDirty || CachedAggregates.Num() == 0)
	{
		CachedAggregates.Empty();
		CacheTimestamp = FDateTime::Now();

		// Group by name + tags
		TMap<FString, FAggregatedMetric> AggregationMap;

		for (const FMetricDataPoint& Point : DataPoints)
		{
			FString Key = Point.Name;
			for (const auto& Tag : Point.Tags)
			{
				Key += TEXT("|") + Tag.Key + TEXT("=") + Tag.Value;
			}

			FAggregatedMetric* Existing = AggregationMap.Find(Key);
			if (!Existing)
			{
				FAggregatedMetric NewMetric;
				NewMetric.Name = Point.Name;
				NewMetric.Type = Point.Type;
				NewMetric.Tags = Point.Tags;
				AggregationMap.Add(Key, NewMetric);
				Existing = &AggregationMap[Key];
			}

			Existing->Count++;

			switch (Point.Type)
			{
			case EMagicMetricType::Counter:
				Existing->Sum += Point.Value.CounterValue;
				break;
			case EMagicMetricType::Gauge:
				Existing->Sum = Point.Value.GaugeValue; // Last value
				break;
			case EMagicMetricType::Timer:
			case EMagicMetricType::Histogram:
				Existing->Sum += Point.Value.TimerValueMs;
				if (Existing->Count == 1)
				{
					Existing->Min = Point.Value.TimerValueMs;
					Existing->Max = Point.Value.TimerValueMs;
				}
				else
				{
					Existing->Min = FMath::Min(Existing->Min, Point.Value.TimerValueMs);
					Existing->Max = FMath::Max(Existing->Max, Point.Value.TimerValueMs);
				}
				break;
			}
		}

		// Convert to array
		AggregationMap.GenerateValueArray(CachedAggregates);
		bCacheDirty = false;
	}

	return CachedAggregates;
}

FString FForgePilotMetrics::GetMetricsJson() const
{
	TArray<FAggregatedMetric> Metrics = GetAggregatedMetrics();

	TSharedPtr<FJsonObject> RootObj = MakeShareable(new FJsonObject());
	RootObj->SetStringField(TEXT("timestamp"), CacheTimestamp.ToString());

	TArray<TSharedPtr<FJsonValue>> MetricsArray;

	for (const FAggregatedMetric& Metric : Metrics)
	{
		TSharedPtr<FJsonObject> MetricObj = MakeShareable(new FJsonObject());
		MetricObj->SetStringField(TEXT("name"), Metric.Name);

		FString TypeStr;
		switch (Metric.Type)
		{
		case EMagicMetricType::Counter: TypeStr = TEXT("counter"); break;
		case EMagicMetricType::Gauge: TypeStr = TEXT("gauge"); break;
		case EMagicMetricType::Timer: TypeStr = TEXT("timer"); break;
		case EMagicMetricType::Histogram: TypeStr = TEXT("histogram"); break;
		}
		MetricObj->SetStringField(TEXT("type"), TypeStr);

		MetricObj->SetNumberField(TEXT("count"), Metric.Count);

		switch (Metric.Type)
		{
		case EMagicMetricType::Counter:
			MetricObj->SetNumberField(TEXT("value"), (int64)Metric.Sum);
			break;
		case EMagicMetricType::Gauge:
			MetricObj->SetNumberField(TEXT("value"), Metric.Sum);
			break;
		case EMagicMetricType::Timer:
		case EMagicMetricType::Histogram:
			MetricObj->SetNumberField(TEXT("sum"), Metric.Sum);
			MetricObj->SetNumberField(TEXT("min"), Metric.Min);
			MetricObj->SetNumberField(TEXT("max"), Metric.Max);
			MetricObj->SetNumberField(TEXT("avg"), Metric.Sum / Metric.Count);
			break;
		}

		// Tags
		TSharedPtr<FJsonObject> TagsObj = MakeShareable(new FJsonObject());
		for (const auto& Tag : Metric.Tags)
		{
			TagsObj->SetStringField(Tag.Key, Tag.Value);
		}
		MetricObj->SetObjectField(TEXT("tags"), TagsObj);

		MetricsArray.Add(MakeShareable(new FJsonValueObject(MetricObj)));
	}

	RootObj->SetArrayField(TEXT("metrics"), MetricsArray);

	// Serialize
	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(RootObj.ToSharedRef(), Writer);

	return Out;
}

FString FForgePilotMetrics::GetMetricsJsonSince(FDateTime Since) const
{
	FScopeLock Lock(&MetricsLock);

	TArray<FAggregatedMetric> Aggregates;
	TMap<FString, FAggregatedMetric> AggregationMap;

	// Filter and aggregate
	for (const FMetricDataPoint& Point : DataPoints)
	{
		if (Point.Timestamp < Since)
			continue;

		FString Key = Point.Name;
		for (const auto& Tag : Point.Tags)
		{
			Key += TEXT("|") + Tag.Key + TEXT("=") + Tag.Value;
		}

		FAggregatedMetric* Existing = AggregationMap.Find(Key);
		if (!Existing)
		{
			FAggregatedMetric NewMetric;
			NewMetric.Name = Point.Name;
			NewMetric.Type = Point.Type;
			NewMetric.Tags = Point.Tags;
			AggregationMap.Add(Key, NewMetric);
			Existing = &AggregationMap[Key];
		}

		Existing->Count++;
		switch (Point.Type)
		{
		case EMagicMetricType::Counter:
			Existing->Sum += Point.Value.CounterValue;
			break;
		case EMagicMetricType::Gauge:
			Existing->Sum = Point.Value.GaugeValue;
			break;
		case EMagicMetricType::Timer:
		case EMagicMetricType::Histogram:
			Existing->Sum += Point.Value.TimerValueMs;
			if (Existing->Count == 1)
			{
				Existing->Min = Point.Value.TimerValueMs;
				Existing->Max = Point.Value.TimerValueMs;
			}
			else
			{
				Existing->Min = FMath::Min(Existing->Min, Point.Value.TimerValueMs);
				Existing->Max = FMath::Max(Existing->Max, Point.Value.TimerValueMs);
			}
			break;
		}
	}

	AggregationMap.GenerateValueArray(Aggregates);

	// Build JSON (same as GetMetricsJson but with filtered data)
	TSharedPtr<FJsonObject> RootObj = MakeShareable(new FJsonObject());
	RootObj->SetStringField(TEXT("timestamp"), FDateTime::Now().ToString());
	RootObj->SetStringField(TEXT("since"), Since.ToString());

	TArray<TSharedPtr<FJsonValue>> MetricsArray;

	for (const FAggregatedMetric& Metric : Aggregates)
	{
		TSharedPtr<FJsonObject> MetricObj = MakeShareable(new FJsonObject());
		MetricObj->SetStringField(TEXT("name"), Metric.Name);

		FString TypeStr;
		switch (Metric.Type)
		{
		case EMagicMetricType::Counter: TypeStr = TEXT("counter"); break;
		case EMagicMetricType::Gauge: TypeStr = TEXT("gauge"); break;
		case EMagicMetricType::Timer: TypeStr = TEXT("timer"); break;
		case EMagicMetricType::Histogram: TypeStr = TEXT("histogram"); break;
		}
		MetricObj->SetStringField(TEXT("type"), TypeStr);
		MetricObj->SetNumberField(TEXT("count"), Metric.Count);

		switch (Metric.Type)
		{
		case EMagicMetricType::Counter:
			MetricObj->SetNumberField(TEXT("value"), (int64)Metric.Sum);
			break;
		case EMagicMetricType::Gauge:
			MetricObj->SetNumberField(TEXT("value"), Metric.Sum);
			break;
		case EMagicMetricType::Timer:
		case EMagicMetricType::Histogram:
			MetricObj->SetNumberField(TEXT("sum"), Metric.Sum);
			MetricObj->SetNumberField(TEXT("min"), Metric.Min);
			MetricObj->SetNumberField(TEXT("max"), Metric.Max);
			MetricObj->SetNumberField(TEXT("avg"), Metric.Sum / Metric.Count);
			break;
		}

		TSharedPtr<FJsonObject> TagsObj = MakeShareable(new FJsonObject());
		for (const auto& Tag : Metric.Tags)
		{
			TagsObj->SetStringField(Tag.Key, Tag.Value);
		}
		MetricObj->SetObjectField(TEXT("tags"), TagsObj);

		MetricsArray.Add(MakeShareable(new FJsonValueObject(MetricObj)));
	}

	RootObj->SetArrayField(TEXT("metrics"), MetricsArray);

	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(RootObj.ToSharedRef(), Writer);

	return Out;
}

void FForgePilotMetrics::Reset()
{
	FScopeLock Lock(&MetricsLock);
	DataPoints.Empty();
	CachedAggregates.Empty();
	bCacheDirty = true;
}

void FForgePilotMetrics::ResetMetric(const FString& Name)
{
	FScopeLock Lock(&MetricsLock);

	for (int32 i = DataPoints.Num() - 1; i >= 0; i--)
	{
		if (DataPoints[i].Name == Name)
		{
			DataPoints.RemoveAt(i);
		}
	}

	bCacheDirty = true;
}
