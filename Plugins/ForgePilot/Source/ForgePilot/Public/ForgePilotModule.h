// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Containers/Ticker.h"

class FForgePilotModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void EnsureBridgeBootstrapped();

private:
	void OpenBridgeWindow();
	void RequestBridgeBootstrap();
	bool TickBridgeBootstrap(float DeltaTime);
	bool TryBootstrapBridgeNow();
	void ClearBridgeBootstrapTicker();
	void RegisterMenus();

	TSharedPtr<class FUICommandList> PluginCommands;
	TWeakPtr<class SWindow> BridgeWindow;
	FTSTicker::FDelegateHandle BridgeBootstrapTickerHandle;
	int32 BridgeBootstrapAttempts = 0;
	bool bBridgeBootstrapComplete = false;
};
