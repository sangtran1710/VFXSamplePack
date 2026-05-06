// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "ForgePilotStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Brushes/SlateImageBrush.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FForgePilotStyle::StyleInstance = nullptr;

void FForgePilotStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FForgePilotStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FForgePilotStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ForgePilotStyle"));
	return StyleSetName;
}

void FForgePilotStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FForgePilotStyle::Get()
{
	return *StyleInstance;
}

TSharedRef<FSlateStyleSet> FForgePilotStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("ForgePilotStyle"));

	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("ForgePilot");
	if (Plugin.IsValid())
	{
		Style->SetContentRoot(Plugin->GetBaseDir() / TEXT("Resources"));
	}

	Style->Set("ForgePilot.OpenPanel", new FSlateImageBrush(
		RootToContentDir(TEXT("ForgePilot_Icon_40"), TEXT(".png")),
		FVector2D(40.0f, 40.0f)));
	Style->Set("ForgePilot.OpenPanel.Small", new FSlateImageBrush(
		RootToContentDir(TEXT("ForgePilot_Icon_20"), TEXT(".png")),
		FVector2D(24.0f, 24.0f)));
	Style->Set("ForgePilot.OpenPanel.Large", new FSlateImageBrush(
		RootToContentDir(TEXT("ForgePilot_Icon_64"), TEXT(".png")),
		FVector2D(64.0f, 64.0f)));

	return Style;
}

#undef RootToContentDir
