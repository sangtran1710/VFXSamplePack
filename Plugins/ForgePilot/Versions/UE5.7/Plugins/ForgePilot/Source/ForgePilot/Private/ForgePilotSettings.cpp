// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotSettings.h"

#include "HAL/PlatformMisc.h"
#include "Misc/App.h"
#include "Misc/Paths.h"
#include "Misc/SecureHash.h"

namespace
{
static bool TryParseBridgePort(const FString& RawValue, int32& OutPort)
{
	if (RawValue.TrimStartAndEnd().IsEmpty())
	{
		return false;
	}

	int32 ParsedPort = 0;
	if (!LexTryParseString(ParsedPort, *RawValue) || ParsedPort <= 0 || ParsedPort > 65535)
	{
		return false;
	}

	OutPort = ParsedPort;
	return true;
}

static FString NormalizeBridgePath(FString Path)
{
	Path = FPaths::ConvertRelativePathToFull(Path);
	FPaths::NormalizeFilename(Path);
	Path.ReplaceInline(TEXT("\\"), TEXT("/"));
	while (Path.EndsWith(TEXT("/")))
	{
		Path.LeftChopInline(1, EAllowShrinking::No);
	}
	return Path;
}
}

UForgePilotSettings::UForgePilotSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadConfig();
}

FString UForgePilotSettings::GetNormalizedProjectDirectory()
{
	return NormalizeBridgePath(FPaths::ProjectDir());
}

FString UForgePilotSettings::GetBridgeProjectName()
{
	const FString ProjectName = FApp::GetProjectName();
	return ProjectName.IsEmpty() ? TEXT("UnknownProject") : ProjectName;
}

FString UForgePilotSettings::GetBridgeWorkspaceId()
{
	const FString NormalizedProjectDir = GetNormalizedProjectDirectory().ToLower();
	const FString Hash = FMD5::HashAnsiString(*NormalizedProjectDir);
	return FString::Printf(TEXT("fpw_%s"), *Hash.Left(12));
}

int32 UForgePilotSettings::ResolveMCPServerPort()
{
	int32 PortOverride = 0;
	if (TryParseBridgePort(FPlatformMisc::GetEnvironmentVariable(TEXT("FORGEPILOT_BRIDGE_PORT")), PortOverride))
	{
		return PortOverride;
	}

	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && Settings->MCPServerPort > 0)
	{
		return Settings->MCPServerPort;
	}

	const FString NormalizedProjectDir = GetNormalizedProjectDirectory().ToLower();
	const FString Hash = FMD5::HashAnsiString(*NormalizedProjectDir).Left(8);
	const uint32 HashValue = static_cast<uint32>(FCString::Strtoui64(*Hash, nullptr, 16));
	return 19080 + static_cast<int32>(HashValue % 1000u);
}

void UForgePilotSettings::ResetToDefaults()
{
	MaxToolIterations = 25;
	RequestTimeoutSeconds = 120.0f;

	MaxRetries = 3;
	InitialBackoffSeconds = 1.0f;
	MaxBackoffSeconds = 30.0f;
	bRetryOnRateLimit = true;
	bRetryOnServerError = true;

	bEnableLogging = true;
	bLogToolExecution = true;
	bLogBridgeOperations = true;
	LogVerbosity = 2;

	bEnableMetrics = true;
	MaxMetricsDataPoints = 10000;
	bExportMetricsOnShutdown = true;
	MetricsExportPath.Empty();

	ToolTimeoutSeconds = 30.0f;
	bDefaultDryRun = true;
	bAllowAssetModification = true;
	MaxAssetScanResults = 500;

	bEnableMCPBridge = true;
	MCPServerPort = 0;
	bStartMCPServerOnStartup = true;

	bShowPanelOnStartup = true;
	PanelWidth = 400.0f;
}

bool UForgePilotSettings::ValidateSettings(FString& OutErrorMessage) const
{
	if (RequestTimeoutSeconds <= 0)
	{
		OutErrorMessage = TEXT("Request timeout must be positive");
		return false;
	}

	if (ToolTimeoutSeconds <= 0)
	{
		OutErrorMessage = TEXT("Tool timeout must be positive");
		return false;
	}

	if (MaxRetries < 0)
	{
		OutErrorMessage = TEXT("Max retries cannot be negative");
		return false;
	}

	if (MaxToolIterations <= 0)
	{
		OutErrorMessage = TEXT("Max tool iterations must be positive");
		return false;
	}

	if (LogVerbosity < 0 || LogVerbosity > 5)
	{
		OutErrorMessage = TEXT("Log verbosity must be between 0-5");
		return false;
	}

	if (MCPServerPort < 0 || MCPServerPort > 65535)
	{
		OutErrorMessage = TEXT("MCP server port must be between 0 and 65535");
		return false;
	}

	return true;
}
