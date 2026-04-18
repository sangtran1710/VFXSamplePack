// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "ForgePilotSettings.generated.h"

/**
 * ForgePilot Configuration Settings
 * Stored in Project Settings -> Plugins -> ForgePilot
 */
UCLASS(config = EditorPerProjectUserSettings, defaultconfig)
class UForgePilotSettings : public UObject
{
	GENERATED_BODY()

public:
	UForgePilotSettings(const FObjectInitializer& ObjectInitializer);
	static FString GetNormalizedProjectDirectory();
	static FString GetBridgeProjectName();
	static FString GetBridgeWorkspaceId();
	static int32 ResolveMCPServerPort();

	// ============================================================
	// Runtime Configuration
	// ============================================================

	/** Maximum number of tool execution iterations per request */
	UPROPERTY(config, EditAnywhere, Category = "Runtime")
	int32 MaxToolIterations = 25;

	/** Request timeout in seconds */
	UPROPERTY(config, EditAnywhere, Category = "Runtime")
	float RequestTimeoutSeconds = 120.0f;

	// ============================================================
	// Retry Configuration
	// ============================================================

	/** Maximum retry attempts for failed requests */
	UPROPERTY(config, EditAnywhere, Category = "Retry")
	int32 MaxRetries = 3;

	/** Initial backoff delay in seconds */
	UPROPERTY(config, EditAnywhere, Category = "Retry")
	float InitialBackoffSeconds = 1.0f;

	/** Maximum backoff delay in seconds */
	UPROPERTY(config, EditAnywhere, Category = "Retry")
	float MaxBackoffSeconds = 30.0f;

	/** Retry on rate limit errors (429) */
	UPROPERTY(config, EditAnywhere, Category = "Retry")
	bool bRetryOnRateLimit = true;

	/** Retry on server errors (5xx) */
	UPROPERTY(config, EditAnywhere, Category = "Retry")
	bool bRetryOnServerError = true;

	// ============================================================
	// Logging Configuration
	// ============================================================

	/** Enable structured logging */
	UPROPERTY(config, EditAnywhere, Category = "Logging")
	bool bEnableLogging = true;

	/** Log tool execution (start/end/errors) */
	UPROPERTY(config, EditAnywhere, Category = "Logging")
	bool bLogToolExecution = true;

	/** Log MCP bridge operations */
	UPROPERTY(config, EditAnywhere, Category = "Logging")
	bool bLogBridgeOperations = true;

	/** Log verbosity level (0=Error, 1=Warning, 2=Log, 3=Verbose, 4=VeryVerbose) */
	UPROPERTY(config, EditAnywhere, Category = "Logging", AdvancedDisplay)
	int32 LogVerbosity = 2;

	// ============================================================
	// Metrics Configuration
	// ============================================================

	/** Enable metrics collection */
	UPROPERTY(config, EditAnywhere, Category = "Metrics")
	bool bEnableMetrics = true;

	/** Maximum metrics data points to keep in memory */
	UPROPERTY(config, EditAnywhere, Category = "Metrics")
	int32 MaxMetricsDataPoints = 10000;

	/** Export metrics on shutdown */
	UPROPERTY(config, EditAnywhere, Category = "Metrics")
	bool bExportMetricsOnShutdown = true;

	/** Metrics export path (empty = default) */
	UPROPERTY(config, EditAnywhere, Category = "Metrics", AdvancedDisplay)
	FString MetricsExportPath;

	// ============================================================
	// Tool Configuration
	// ============================================================

	/** Default timeout for individual tool execution (seconds) */
	UPROPERTY(config, EditAnywhere, Category = "Tools")
	float ToolTimeoutSeconds = 30.0f;

	/** Enable dry-run mode by default (requires apply_changes=true to execute) */
	UPROPERTY(config, EditAnywhere, Category = "Tools")
	bool bDefaultDryRun = true;

	/** Allow tool execution to modify assets */
	UPROPERTY(config, EditAnywhere, Category = "Tools")
	bool bAllowAssetModification = true;

	/** Maximum number of assets to return in scan/search operations */
	UPROPERTY(config, EditAnywhere, Category = "Tools")
	int32 MaxAssetScanResults = 500;

	// ============================================================
	// MCP Bridge Configuration
	// ============================================================

	/** Enable MCP bridge subsystem */
	UPROPERTY(config, EditAnywhere, Category = "MCP Bridge")
	bool bEnableMCPBridge = true;

	/** MCP server port (0 = auto-select) */
	UPROPERTY(config, EditAnywhere, Category = "MCP Bridge")
	int32 MCPServerPort = 0;

	/** Enable MCP server at startup */
	UPROPERTY(config, EditAnywhere, Category = "MCP Bridge")
	bool bStartMCPServerOnStartup = true;

	// ============================================================
	// UI Configuration
	// ============================================================

	/** Show ForgePilot panel on startup */
	UPROPERTY(config, EditAnywhere, Category = "UI")
	bool bShowPanelOnStartup = true;

	/** Panel width in pixels */
	UPROPERTY(config, EditAnywhere, Category = "UI", AdvancedDisplay)
	float PanelWidth = 400.0f;

	// ============================================================
	// Helper Methods
	// ============================================================

	/** Reset to defaults */
	void ResetToDefaults();

	/** Validate settings */
	bool ValidateSettings(FString& OutErrorMessage) const;
};
