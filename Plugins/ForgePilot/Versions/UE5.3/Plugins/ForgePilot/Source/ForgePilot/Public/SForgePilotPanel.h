// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SButton;
class STextBlock;

class SForgePilotPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SForgePilotPanel) {}
	SLATE_END_ARGS()

	enum class EBridgeVisualState : uint8
	{
		Idle,
		Starting,
		Connected,
		Error,
		Disabled
	};

	void Construct(const FArguments& InArgs);
	virtual ~SForgePilotPanel() override = default;

private:
	FReply OnReconnectClicked();
	EActiveTimerReturnType HandleStatusRefresh(double InCurrentTime, float InDeltaTime);

	void RefreshBridgeStatus();
	void UpdateStatusPresentation(
		EBridgeVisualState NewState,
		const FString& InHeadline,
		const FString& InDetail);
	void RequestBridgeBootstrap();

	bool QueryBridgeSocket(FString& OutError) const;
	bool QueryBridgeSubsystemStatus(bool& bOutInitialized, bool& bOutHasToolRegistry, FString& OutError) const;
	FString GetBridgeHost() const;
	int32 GetBridgePort() const;
	FString GetEndpointLabel() const;

	TSharedRef<SWidget> CreateMainCard();

	TSharedPtr<STextBlock> StatusPillText;
	TSharedPtr<STextBlock> StatusHeadlineText;
	TSharedPtr<STextBlock> StatusDetailText;
	TSharedPtr<STextBlock> EndpointText;
	TSharedPtr<SButton> ReconnectButton;

	EBridgeVisualState CurrentVisualState = EBridgeVisualState::Starting;
	double LastConnectRequestTime = 0.0;
};
