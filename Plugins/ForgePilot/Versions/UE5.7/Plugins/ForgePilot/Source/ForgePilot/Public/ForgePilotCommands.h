// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ForgePilotStyle.h"

class FForgePilotCommands : public TCommands<FForgePilotCommands>
{
public:
	FForgePilotCommands()
		: TCommands<FForgePilotCommands>(
			TEXT("ForgePilot"),
			NSLOCTEXT("Contexts", "ForgePilot", "ForgePilot Plugin"),
			NAME_None,
			FForgePilotStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> OpenPanel;
};
