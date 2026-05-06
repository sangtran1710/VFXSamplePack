// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "SForgePilotPanel.h"

#include "Editor.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Misc/DefaultValueHelper.h"
#include "Misc/ScopeExit.h"
#include "Modules/ModuleManager.h"
#include "Serialization/JsonSerializer.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Styling/AppStyle.h"
#include "ForgePilotMCPBridgeSubsystem.h"
#include "ForgePilotModule.h"
#include "ForgePilotSettings.h"
#include "ForgePilotStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SForgePilotPanel"

namespace
{
static FString GetBridgeHostFromConfig()
{
	FString Host = FPlatformMisc::GetEnvironmentVariable(TEXT("FORGEPILOT_BRIDGE_HOST"));
	if (Host.TrimStartAndEnd().IsEmpty())
	{
		Host = TEXT("127.0.0.1");
	}

	return Host;
}

static int32 GetBridgePortFromConfig()
{
	const FString EnvPort = FPlatformMisc::GetEnvironmentVariable(TEXT("FORGEPILOT_BRIDGE_PORT"));
	int32 ParsedPort = 0;
	if (!EnvPort.TrimStartAndEnd().IsEmpty() &&
		FDefaultValueHelper::ParseInt(EnvPort, ParsedPort) &&
		ParsedPort > 0)
	{
		return ParsedPort;
	}

	return UForgePilotSettings::ResolveMCPServerPort();
}

static bool ParseBridgeStatusJson(
	const FString& JsonText,
	bool& bOutInitialized,
	bool& bOutHasToolRegistry,
	FString& OutError)
{
	bOutInitialized = false;
	bOutHasToolRegistry = false;
	OutError.Reset();

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonText);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		OutError = TEXT("Bridge status returned invalid JSON.");
		return false;
	}

	JsonObject->TryGetBoolField(TEXT("initialized"), bOutInitialized);
	JsonObject->TryGetBoolField(TEXT("has_tool_registry"), bOutHasToolRegistry);
	return true;
}

static FLinearColor GetPillColor(SForgePilotPanel::EBridgeVisualState State)
{
	switch (State)
	{
	case SForgePilotPanel::EBridgeVisualState::Idle:
		return FLinearColor(0.47f, 0.53f, 0.62f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Connected:
		return FLinearColor(0.12f, 0.82f, 0.56f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Error:
		return FLinearColor(1.00f, 0.58f, 0.24f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Disabled:
		return FLinearColor(0.33f, 0.35f, 0.40f, 1.0f);
	default:
		return FLinearColor(0.08f, 0.76f, 0.96f, 1.0f);
	}
}

static FText GetPillLabel(SForgePilotPanel::EBridgeVisualState State)
{
	switch (State)
	{
	case SForgePilotPanel::EBridgeVisualState::Idle:
		return LOCTEXT("BridgeStateIdle", "MANUAL");
	case SForgePilotPanel::EBridgeVisualState::Connected:
		return LOCTEXT("BridgeStateConnected", "ONLINE");
	case SForgePilotPanel::EBridgeVisualState::Error:
		return LOCTEXT("BridgeStateError", "WAITING");
	case SForgePilotPanel::EBridgeVisualState::Disabled:
		return LOCTEXT("BridgeStateDisabled", "DISABLED");
	default:
		return LOCTEXT("BridgeStateStarting", "STARTING");
	}
}

static FText GetActionLabel(SForgePilotPanel::EBridgeVisualState State)
{
	switch (State)
	{
	case SForgePilotPanel::EBridgeVisualState::Connected:
		return LOCTEXT("ReconnectAction", "Reconnect");
	case SForgePilotPanel::EBridgeVisualState::Disabled:
		return LOCTEXT("DisabledAction", "Disabled");
	default:
		return LOCTEXT("ConnectAction", "Connect");
	}
}
}

void SForgePilotPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor(0.055f, 0.058f, 0.068f, 1.0f))
		.Padding(12.0f)
		[
			CreateMainCard()
		]
	];

	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		UpdateStatusPresentation(
			EBridgeVisualState::Disabled,
			TEXT("Bridge disabled"),
			TEXT("Enable the MCP bridge in ForgePilot settings to connect"));
	}
	else if (!Settings || Settings->bStartMCPServerOnStartup)
	{
		RequestBridgeBootstrap();
	}
	else
	{
		LastConnectRequestTime = 0.0;
		UpdateStatusPresentation(
			EBridgeVisualState::Idle,
			TEXT("Not connected"),
			TEXT("Bridge startup is manual; use Connect when needed"));
	}

	RefreshBridgeStatus();
	RegisterActiveTimer(0.8f, FWidgetActiveTimerDelegate::CreateSP(this, &SForgePilotPanel::HandleStatusRefresh));
}

FReply SForgePilotPanel::OnReconnectClicked()
{
	RequestBridgeBootstrap();
	return FReply::Handled();
}

FReply SForgePilotPanel::OnCopyEndpointClicked()
{
	FPlatformApplicationMisc::ClipboardCopy(*GetEndpointLabel());
	return FReply::Handled();
}

EActiveTimerReturnType SForgePilotPanel::HandleStatusRefresh(double InCurrentTime, float InDeltaTime)
{
	RefreshBridgeStatus();
	return EActiveTimerReturnType::Continue;
}

void SForgePilotPanel::RefreshBridgeStatus()
{
	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		UpdateStatusPresentation(
			EBridgeVisualState::Disabled,
			TEXT("Bridge disabled"),
			TEXT("Enable the MCP bridge in ForgePilot settings to connect"));
		return;
	}

	bool bSubsystemInitialized = false;
	bool bHasToolRegistry = false;
	FString SubsystemError;
	const bool bSubsystemOk = QueryBridgeSubsystemStatus(bSubsystemInitialized, bHasToolRegistry, SubsystemError);

	FString SocketError;
	const bool bSocketReachable = QueryBridgeSocket(SocketError);

	if (bSocketReachable)
	{
		UpdateStatusPresentation(
			EBridgeVisualState::Connected,
			TEXT("Bridge ready"),
			TEXT("External agents can connect to this editor session"));
		return;
	}

	if (LastConnectRequestTime <= 0.0)
	{
		UpdateStatusPresentation(
			EBridgeVisualState::Idle,
			TEXT("Not connected"),
			TEXT("Bridge startup is manual; use Connect when needed"));
		return;
	}

	const double ElapsedSeconds = FPlatformTime::Seconds() - LastConnectRequestTime;
	if (ElapsedSeconds < 12.0)
	{
		UpdateStatusPresentation(
			EBridgeVisualState::Starting,
			TEXT("Connecting..."),
			TEXT("Starting local MCP bridge"));
		return;
	}

	const FString Detail = bSubsystemOk && bSubsystemInitialized && bHasToolRegistry
		? TEXT("Bridge socket is still warming up")
		: (SubsystemError.IsEmpty() ? TEXT("Waiting for Unreal bridge") : SubsystemError);
	UpdateStatusPresentation(EBridgeVisualState::Error, TEXT("Still connecting"), Detail);
}

void SForgePilotPanel::UpdateStatusPresentation(
	EBridgeVisualState NewState,
	const FString& InHeadline,
	const FString& InDetail)
{
	CurrentVisualState = NewState;

	if (StatusPillText.IsValid())
	{
		StatusPillText->SetText(GetPillLabel(NewState));
	}

	if (StatusHeadlineText.IsValid())
	{
		StatusHeadlineText->SetText(FText::FromString(InHeadline));
	}

	if (StatusDetailText.IsValid())
	{
		StatusDetailText->SetText(FText::FromString(InDetail));
	}

	if (EndpointText.IsValid())
	{
		EndpointText->SetText(FText::FromString(GetEndpointLabel()));
	}

	if (ProjectText.IsValid())
	{
		ProjectText->SetText(FText::FromString(GetProjectLabel()));
	}

	if (StartupModeText.IsValid())
	{
		StartupModeText->SetText(FText::FromString(GetStartupModeLabel()));
	}

	if (SafetyText.IsValid())
	{
		SafetyText->SetText(FText::FromString(GetSafetyLabel()));
	}

	if (ReconnectButtonText.IsValid())
	{
		ReconnectButtonText->SetText(GetActionLabel(NewState));
	}
}

void SForgePilotPanel::RequestBridgeBootstrap()
{
	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		LastConnectRequestTime = 0.0;
		UpdateStatusPresentation(
			EBridgeVisualState::Disabled,
			TEXT("Bridge disabled"),
			TEXT("Enable the MCP bridge in ForgePilot settings to connect"));
		return;
	}

	FForgePilotModule& ForgePilotModule = FModuleManager::LoadModuleChecked<FForgePilotModule>(TEXT("ForgePilot"));
	ForgePilotModule.EnsureBridgeBootstrapped();
	LastConnectRequestTime = FPlatformTime::Seconds();
	UpdateStatusPresentation(EBridgeVisualState::Starting, TEXT("Connecting..."), TEXT("Starting local MCP bridge"));
}

bool SForgePilotPanel::QueryBridgeSocket(FString& OutError) const
{
	OutError.Reset();

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		OutError = TEXT("Socket subsystem unavailable");
		return false;
	}

	TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
	bool bIsValidIp = false;
	Address->SetIp(*GetBridgeHost(), bIsValidIp);
	Address->SetPort(GetBridgePort());
	if (!bIsValidIp)
	{
		OutError = TEXT("Invalid bridge host");
		return false;
	}

	FSocket* Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("ForgePilotBridgeProbe"), false);
	if (!Socket)
	{
		OutError = TEXT("Could not create socket");
		return false;
	}

	ON_SCOPE_EXIT
	{
		SocketSubsystem->DestroySocket(Socket);
	};

	Socket->SetNonBlocking(false);
	Socket->SetRecvErr(true);

	if (!Socket->Connect(*Address))
	{
		OutError = SocketSubsystem->GetSocketError(SocketSubsystem->GetLastErrorCode());
		return false;
	}

	return true;
}

bool SForgePilotPanel::QueryBridgeSubsystemStatus(
	bool& bOutInitialized,
	bool& bOutHasToolRegistry,
	FString& OutError) const
{
	bOutInitialized = false;
	bOutHasToolRegistry = false;
	OutError.Reset();

	if (!GEditor)
	{
		OutError = TEXT("GEditor unavailable");
		return false;
	}

	UForgePilotMCPBridgeSubsystem* BridgeSubsystem = GEditor->GetEditorSubsystem<UForgePilotMCPBridgeSubsystem>();
	if (!BridgeSubsystem)
	{
		OutError = TEXT("Bridge subsystem unavailable");
		return false;
	}

	const FString StatusJson = BridgeSubsystem->GetBridgeStatusJson();
	return ParseBridgeStatusJson(StatusJson, bOutInitialized, bOutHasToolRegistry, OutError);
}

FString SForgePilotPanel::GetBridgeHost() const
{
	return GetBridgeHostFromConfig();
}

int32 SForgePilotPanel::GetBridgePort() const
{
	return GetBridgePortFromConfig();
}

FString SForgePilotPanel::GetEndpointLabel() const
{
	return FString::Printf(TEXT("%s:%d"), *GetBridgeHost(), GetBridgePort());
}

FString SForgePilotPanel::GetProjectLabel() const
{
	return UForgePilotSettings::GetBridgeProjectName();
}

FString SForgePilotPanel::GetStartupModeLabel() const
{
	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (Settings && !Settings->bEnableMCPBridge)
	{
		return TEXT("Bridge disabled");
	}

	return Settings && Settings->bStartMCPServerOnStartup
		? TEXT("Auto start")
		: TEXT("Manual start");
}

FString SForgePilotPanel::GetSafetyLabel() const
{
	const UForgePilotSettings* Settings = GetDefault<UForgePilotSettings>();
	if (!Settings)
	{
		return TEXT("Settings unavailable");
	}

	const FString MutationLabel = Settings->bAllowAssetModification
		? TEXT("Mutation allowed")
		: TEXT("Mutation blocked");
	const FString DryRunLabel = Settings->bDefaultDryRun
		? TEXT("dry-run default")
		: TEXT("live default");
	return FString::Printf(TEXT("%s, %s"), *MutationLabel, *DryRunLabel);
}

TSharedRef<SWidget> SForgePilotPanel::CreateInfoRow(const FText& Label, const TSharedRef<SWidget>& ValueWidget) const
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 14.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(82.0f)
			[
				SNew(STextBlock)
				.Text(Label)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				.ColorAndOpacity(FSlateColor(FLinearColor(0.48f, 0.54f, 0.62f, 1.0f)))
			]
		]

		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.VAlign(VAlign_Center)
		[
			ValueWidget
		];
}

TSharedRef<SWidget> SForgePilotPanel::CreateMainCard()
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor(0.095f, 0.102f, 0.118f, 1.0f))
		.Padding(18.0f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 14.0f, 0.0f)
				[
					SNew(SBox)
					.WidthOverride(36.0f)
					.HeightOverride(36.0f)
					[
						SNew(SImage)
						.Image(FForgePilotStyle::Get().GetBrush(TEXT("ForgePilot.OpenPanel.Large")))
					]
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PanelTitle", "ForgePilot"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
						.ColorAndOpacity(FSlateColor(FLinearColor::White))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 4.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PanelMiniCopy", "AI Bridge - Unreal Editor Automation"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.73f, 0.78f, 0.86f, 1.0f)))
					]
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
					.BorderBackgroundColor_Lambda([this]() { return FSlateColor(GetPillColor(CurrentVisualState)); })
					.Padding(FMargin(10.0f, 5.0f))
					[
						SAssignNew(StatusPillText, STextBlock)
						.Text(LOCTEXT("DefaultState", "STARTING"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
						.ColorAndOpacity(FSlateColor(FLinearColor::White))
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 16.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.115f, 0.125f, 0.145f, 1.0f))
				.Padding(FMargin(14.0f, 12.0f))
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(StatusHeadlineText, STextBlock)
						.Text(LOCTEXT("DefaultHeadline", "Connecting..."))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
						.ColorAndOpacity(FSlateColor(FLinearColor::White))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 5.0f, 0.0f, 0.0f)
					[
						SAssignNew(StatusDetailText, STextBlock)
						.Text(LOCTEXT("DefaultDetail", "Starting local MCP bridge"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.73f, 0.78f, 0.86f, 1.0f)))
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 12.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.082f, 0.088f, 0.102f, 1.0f))
				.Padding(FMargin(12.0f, 11.0f))
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						CreateInfoRow(
							LOCTEXT("ProjectLabel", "Project"),
							SAssignNew(ProjectText, STextBlock)
								.Text(FText::FromString(GetProjectLabel()))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
								.ColorAndOpacity(FSlateColor(FLinearColor(0.86f, 0.90f, 0.96f, 1.0f))))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						CreateInfoRow(
							LOCTEXT("EndpointLabel", "Endpoint"),
							SAssignNew(EndpointText, STextBlock)
								.Text(FText::FromString(GetEndpointLabel()))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
								.ColorAndOpacity(FSlateColor(FLinearColor(0.23f, 0.88f, 1.0f, 1.0f))))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						CreateInfoRow(
							LOCTEXT("StartupLabel", "Startup"),
							SAssignNew(StartupModeText, STextBlock)
								.Text(FText::FromString(GetStartupModeLabel()))
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
								.ColorAndOpacity(FSlateColor(FLinearColor(0.74f, 0.80f, 0.88f, 1.0f))))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						CreateInfoRow(
							LOCTEXT("SafetyLabel", "Mode"),
							SAssignNew(SafetyText, STextBlock)
								.Text(FText::FromString(GetSafetyLabel()))
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
								.ColorAndOpacity(FSlateColor(FLinearColor(0.74f, 0.80f, 0.88f, 1.0f))))
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 14.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FooterHint", "Local MCP bridge endpoint"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.ColorAndOpacity(FSlateColor(FLinearColor(0.63f, 0.68f, 0.76f, 1.0f)))
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonStyle(FAppStyle::Get(), "SimpleButton")
					.OnClicked(this, &SForgePilotPanel::OnCopyEndpointClicked)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
						.BorderBackgroundColor(FLinearColor(0.22f, 0.25f, 0.30f, 1.0f))
						.Padding(FMargin(12.0f, 7.0f))
						[
							SNew(STextBlock)
							.Text(LOCTEXT("CopyEndpointLabel", "Copy Endpoint"))
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.92f, 0.95f, 1.0f, 1.0f)))
						]
					]
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(ReconnectButton, SButton)
					.ButtonStyle(FAppStyle::Get(), "SimpleButton")
					.OnClicked(this, &SForgePilotPanel::OnReconnectClicked)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
						.BorderBackgroundColor(FLinearColor(0.02f, 0.62f, 0.78f, 1.0f))
						.Padding(FMargin(14.0f, 7.0f))
						[
							SAssignNew(ReconnectButtonText, STextBlock)
							.Text(LOCTEXT("ReconnectLabel", "Connect"))
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
							.ColorAndOpacity(FSlateColor(FLinearColor::White))
						]
					]
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
