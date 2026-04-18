// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotModule.h"

#include "HAL/PlatformMisc.h"
#include "Framework/Application/SlateApplication.h"
#include "IPythonScriptPlugin.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SForgePilotPanel.h"
#include "ToolMenus.h"
#include "ForgePilotCommands.h"
#include "ForgePilotSettings.h"
#include "ForgePilotStyle.h"
#include "Widgets/SWindow.h"

namespace
{
static FString GetOrCreateBridgeSessionId()
{
	static FString SessionId;
	if (SessionId.IsEmpty())
	{
		SessionId = FString::Printf(
			TEXT("fps_%d_%s"),
			FPlatformProcess::GetCurrentProcessId(),
			*FGuid::NewGuid().ToString(EGuidFormats::Digits));
	}

	return SessionId;
}
}

#define LOCTEXT_NAMESPACE "FForgePilotModule"

void FForgePilotModule::StartupModule()
{
	FForgePilotStyle::Initialize();
	FForgePilotStyle::ReloadTextures();
	FForgePilotCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FForgePilotCommands::Get().OpenPanel,
		FExecuteAction::CreateRaw(this, &FForgePilotModule::OpenBridgeWindow),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FForgePilotModule::RegisterMenus));

	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (!Settings || (Settings->bEnableMCPBridge && Settings->bStartMCPServerOnStartup))
	{
		RequestBridgeBootstrap();
	}
}

void FForgePilotModule::ShutdownModule()
{
	ClearBridgeBootstrapTicker();
	BridgeWindow.Reset();

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FForgePilotStyle::Shutdown();
	FForgePilotCommands::Unregister();
}

void FForgePilotModule::OpenBridgeWindow()
{
	if (BridgeWindow.IsValid())
	{
		const TSharedPtr<SWindow> ExistingWindow = BridgeWindow.Pin();
		if (ExistingWindow.IsValid())
		{
			ExistingWindow->BringToFront(true);
			return;
		}
	}

	const TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("BridgeWindowTitle", "ForgePilot Bridge"))
		.ClientSize(FVector2D(430.0f, 230.0f))
		.SizingRule(ESizingRule::FixedSize)
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		[
			SNew(SForgePilotPanel)
		];

	Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& ClosedWindow)
	{
		if (BridgeWindow.Pin() == ClosedWindow)
		{
			BridgeWindow.Reset();
		}
	}));

	BridgeWindow = Window;
	FSlateApplication::Get().AddWindow(Window);
}

void FForgePilotModule::RequestBridgeBootstrap()
{
	BridgeBootstrapAttempts = 0;
	bBridgeBootstrapComplete = false;
	ClearBridgeBootstrapTicker();

	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		bBridgeBootstrapComplete = true;
		return;
	}

	if (TryBootstrapBridgeNow())
	{
		bBridgeBootstrapComplete = true;
		return;
	}

	BridgeBootstrapTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateRaw(this, &FForgePilotModule::TickBridgeBootstrap),
		1.0f);
}

bool FForgePilotModule::TickBridgeBootstrap(float DeltaTime)
{
	if (bBridgeBootstrapComplete)
	{
		return false;
	}

	if (TryBootstrapBridgeNow())
	{
		bBridgeBootstrapComplete = true;
		ClearBridgeBootstrapTicker();
		return false;
	}

	++BridgeBootstrapAttempts;
	return BridgeBootstrapAttempts < 30;
}

bool FForgePilotModule::TryBootstrapBridgeNow()
{
	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		return false;
	}

	IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
	if (!PythonPlugin || !PythonPlugin->IsPythonAvailable())
	{
		return false;
	}

	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ForgePilot"));
	if (!Plugin.IsValid())
	{
		return false;
	}

	const int32 BridgePort = UForgePilotSettings::ResolveMCPServerPort();
	const FString ProjectDir = UForgePilotSettings::GetNormalizedProjectDirectory();
	const FString ProjectName = UForgePilotSettings::GetBridgeProjectName();
	const FString WorkspaceId = UForgePilotSettings::GetBridgeWorkspaceId();
	const FString SessionId = GetOrCreateBridgeSessionId();

	FString PythonToolsDir = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Tools"), TEXT("Python"));
	FPaths::NormalizeDirectoryName(PythonToolsDir);
	PythonToolsDir.ReplaceInline(TEXT("\\"), TEXT("/"));

	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_HOST"), TEXT("127.0.0.1"));
	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_PORT"), *LexToString(BridgePort));
	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_PROJECT_DIR"), *ProjectDir);
	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_PROJECT_NAME"), *ProjectName);
	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_WORKSPACE_ID"), *WorkspaceId);
	FPlatformMisc::SetEnvironmentVar(TEXT("FORGEPILOT_BRIDGE_SESSION_ID"), *SessionId);

	const FString PythonCommand = FString::Printf(
		TEXT("import sys\n")
		TEXT("import os\n")
		TEXT("bridge_dir = r'%s'\n")
		TEXT("if bridge_dir not in sys.path:\n")
		TEXT("    sys.path.insert(0, bridge_dir)\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_HOST'] = '127.0.0.1'\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_PORT'] = '%d'\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_PROJECT_DIR'] = r'%s'\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_PROJECT_NAME'] = r'%s'\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_WORKSPACE_ID'] = r'%s'\n")
		TEXT("os.environ['FORGEPILOT_BRIDGE_SESSION_ID'] = r'%s'\n")
		TEXT("import start_forgepilot_bridge\n")
		TEXT("start_forgepilot_bridge.start_bridge()\n"),
		*PythonToolsDir,
		BridgePort,
		*ProjectDir,
		*ProjectName,
		*WorkspaceId,
		*SessionId);

	return PythonPlugin->ExecPythonCommand(*PythonCommand);
}

void FForgePilotModule::EnsureBridgeBootstrapped()
{
	RequestBridgeBootstrap();
}

void FForgePilotModule::ClearBridgeBootstrapTicker()
{
	if (BridgeBootstrapTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(BridgeBootstrapTickerHandle);
		BridgeBootstrapTickerHandle.Reset();
	}
}

void FForgePilotModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
		Section.AddMenuEntryWithCommandList(FForgePilotCommands::Get().OpenPanel, PluginCommands);
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("ForgePilot_Tool");
		FToolMenuEntry& Entry = Section.AddEntry(
			FToolMenuEntry::InitToolBarButton(FForgePilotCommands::Get().OpenPanel));
		Entry.SetCommandList(PluginCommands);
		Entry.Icon = FSlateIcon(FForgePilotStyle::GetStyleSetName(), "ForgePilot.OpenPanel.Small");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FForgePilotModule, ForgePilot)
