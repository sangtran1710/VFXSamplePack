// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "SForgePilotPanel.h"

#include "Editor.h"
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
		return FLinearColor(0.45f, 0.50f, 0.58f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Connected:
		return FLinearColor(0.17f, 0.74f, 0.48f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Error:
		return FLinearColor(0.95f, 0.52f, 0.22f, 1.0f);
	case SForgePilotPanel::EBridgeVisualState::Disabled:
		return FLinearColor(0.30f, 0.32f, 0.36f, 1.0f);
	default:
		return FLinearColor(0.25f, 0.70f, 0.98f, 1.0f);
	}
}

static FText GetPillLabel(SForgePilotPanel::EBridgeVisualState State)
{
	switch (State)
	{
	case SForgePilotPanel::EBridgeVisualState::Idle:
		return LOCTEXT("BridgeStateIdle", "MANUAL");
	case SForgePilotPanel::EBridgeVisualState::Connected:
		return LOCTEXT("BridgeStateConnected", "CONNECTED");
	case SForgePilotPanel::EBridgeVisualState::Error:
		return LOCTEXT("BridgeStateError", "WAITING");
	case SForgePilotPanel::EBridgeVisualState::Disabled:
		return LOCTEXT("BridgeStateDisabled", "DISABLED");
	default:
		return LOCTEXT("BridgeStateStarting", "AUTO CONNECT");
	}
}
}

void SForgePilotPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor(0.10f, 0.11f, 0.14f, 1.0f))
		.Padding(14.0f)
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
			TEXT("Connected"),
			TEXT("Ready for external AI"));
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

TSharedRef<SWidget> SForgePilotPanel::CreateMainCard()
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor(0.14f, 0.15f, 0.19f, 1.0f))
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
				.Padding(0.0f, 0.0f, 12.0f, 0.0f)
				[
					SNew(SBox)
					.WidthOverride(38.0f)
					.HeightOverride(38.0f)
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
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 17))
						.ColorAndOpacity(FSlateColor(FLinearColor::White))
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 4.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PanelMiniCopy", "External MCP bridge"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.73f, 0.78f, 0.86f, 1.0f)))
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 16.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 10.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
					.BorderBackgroundColor_Lambda([this]() { return FSlateColor(GetPillColor(CurrentVisualState)); })
					.Padding(FMargin(10.0f, 5.0f))
					[
						SAssignNew(StatusPillText, STextBlock)
						.Text(LOCTEXT("DefaultState", "AUTO CONNECT"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
						.ColorAndOpacity(FSlateColor(FLinearColor::White))
					]
				]

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SAssignNew(StatusHeadlineText, STextBlock)
					.Text(LOCTEXT("DefaultHeadline", "Connecting..."))
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
					.ColorAndOpacity(FSlateColor(FLinearColor::White))
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 10.0f, 0.0f, 0.0f)
			[
				SAssignNew(StatusDetailText, STextBlock)
				.Text(LOCTEXT("DefaultDetail", "Starting local MCP bridge"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
				.ColorAndOpacity(FSlateColor(FLinearColor(0.74f, 0.78f, 0.84f, 1.0f)))
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 16.0f, 0.0f, 0.0f)
			[
				SAssignNew(EndpointText, STextBlock)
				.Text(FText::FromString(GetEndpointLabel()))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
				.ColorAndOpacity(FSlateColor(FLinearColor(0.98f, 0.79f, 0.39f, 1.0f)))
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SSpacer)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FooterHint", "Bridge startup follows ForgePilot settings"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.ColorAndOpacity(FSlateColor(FLinearColor(0.63f, 0.68f, 0.76f, 1.0f)))
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(ReconnectButton, SButton)
					.ButtonStyle(FAppStyle::Get(), "SimpleButton")
					.OnClicked(this, &SForgePilotPanel::OnReconnectClicked)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ReconnectLabel", "Connect"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.92f, 0.95f, 1.0f, 1.0f)))
					]
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
