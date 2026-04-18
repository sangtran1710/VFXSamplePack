// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotCommands.h"

#define LOCTEXT_NAMESPACE "FForgePilotModule"

void FForgePilotCommands::RegisterCommands()
{
	UI_COMMAND(OpenPanel, "ForgePilot Bridge", "Open the external AI bridge panel", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
