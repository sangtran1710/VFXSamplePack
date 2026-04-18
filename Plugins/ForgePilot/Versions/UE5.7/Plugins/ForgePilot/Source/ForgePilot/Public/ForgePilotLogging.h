// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"

// ============================================================
// Log Categories
// ============================================================

// Main plugin category
DECLARE_LOG_CATEGORY_EXTERN(LogForgePilot, Log, All);

// Detailed AI/ML operations
DECLARE_LOG_CATEGORY_EXTERN(LogForgePilotAI, Log, All);

// Tool execution logging
DECLARE_LOG_CATEGORY_EXTERN(LogForgePilotTools, Log, All);

// MCP Bridge operations
DECLARE_LOG_CATEGORY_EXTERN(LogForgePilotBridge, Log, All);

// ============================================================
// Log Macros
// ============================================================

// AI Request/Response logging with structured data
#define VFX_LOG_AI_REQUEST(Provider, Model, MessagePreview) \
	UE_LOG(LogForgePilotAI, Verbose, TEXT("[%s] %s request: %s"), \
		*Provider, *Model, *MessagePreview)

#define VFX_LOG_AI_RESPONSE(Provider, Model, Success, ToolCallsCount) \
	UE_LOG(LogForgePilotAI, Verbose, TEXT("[%s] %s response: success=%d tool_calls=%d"), \
		*Provider, *Model, Success, ToolCallsCount)

#define VFX_LOG_AI_ERROR(Provider, Model, ErrorMsg) \
	UE_LOG(LogForgePilotAI, Error, TEXT("[%s] %s error: %s"), \
		*Provider, *Model, *ErrorMsg)

// Tool execution logging
#define VFX_LOG_TOOL_START(ToolName, InputHash) \
	UE_LOG(LogForgePilotTools, Verbose, TEXT("Tool START: %s input_hash=%s"), \
		*ToolName, *InputHash)

#define VFX_LOG_TOOL_COMPLETE(ToolName, DurationMs, Success) \
	UE_LOG(LogForgePilotTools, Verbose, TEXT("Tool END: %s duration_ms=%.2f success=%d"), \
		*ToolName, DurationMs, Success)

#define VFX_LOG_TOOL_ERROR(ToolName, ErrorMsg) \
	UE_LOG(LogForgePilotTools, Error, TEXT("Tool ERROR: %s error=%s"), \
		*ToolName, *ErrorMsg)

// MCP Bridge logging
#define VFX_LOG_BRIDGE_REQUEST(Action, RequestPreview) \
	UE_LOG(LogForgePilotBridge, Verbose, TEXT("[%s] %s"), *Action, *RequestPreview)

#define VFX_LOG_BRIDGE_RESPONSE(Action, Success, DurationMs) \
	UE_LOG(LogForgePilotBridge, Verbose, TEXT("[%s] success=%d duration_ms=%.2f"), \
		*Action, Success, DurationMs)

// Main plugin log
#define VFX_LOG_INIT(Module, Message) \
	UE_LOG(LogForgePilot, Log, TEXT("[%s] %s"), *Module, *Message)

#define VFX_LOG_WARN(Module, Message) \
	UE_LOG(LogForgePilot, Warning, TEXT("[%s] %s"), *Module, *Message)

#define VFX_LOG_ERROR(Module, Message) \
	UE_LOG(LogForgePilot, Error, TEXT("[%s] %s"), *Module, *Message)
