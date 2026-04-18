// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"
#include "EdGraphSchema_K2.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#if WITH_EDITOR
#include "Editor.h"
#endif

static inline TSharedPtr<FJsonObject> MakeEmptyObjectSchema()
{
	TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	Schema->SetObjectField(TEXT("properties"), MakeShareable(new FJsonObject()));
	return Schema;
}

static inline TSharedPtr<FJsonObject> MakeStringProperty(const FString& Description)
{
	TSharedPtr<FJsonObject> Prop = MakeShareable(new FJsonObject());
	Prop->SetStringField(TEXT("type"), TEXT("string"));
	Prop->SetStringField(TEXT("description"), Description);
	return Prop;
}

static inline TSharedPtr<FJsonObject> MakeArrayProperty(const FString& Description, const FString& ItemType)
{
	TSharedPtr<FJsonObject> Prop = MakeShareable(new FJsonObject());
	Prop->SetStringField(TEXT("type"), TEXT("array"));
	Prop->SetStringField(TEXT("description"), Description);
	TSharedPtr<FJsonObject> Items = MakeShareable(new FJsonObject());
	Items->SetStringField(TEXT("type"), ItemType);
	Prop->SetObjectField(TEXT("items"), Items);
	return Prop;
}

// JSON serialization helper
static inline FString JsonObjToString(TSharedPtr<FJsonObject> Obj)
{
	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);
	return Out;
}

static inline FString GetForgePilotUserDownloadsDir()
{
	FString UserProfileDir = FPlatformMisc::GetEnvironmentVariable(TEXT("USERPROFILE"));
	if (UserProfileDir.IsEmpty())
	{
		UserProfileDir = FPlatformProcess::UserHomeDir();
	}
	if (UserProfileDir.IsEmpty())
	{
		UserProfileDir = FPlatformProcess::UserDir();
	}

	return FPaths::ConvertRelativePathToFull(FPaths::Combine(UserProfileDir, TEXT("Downloads")));
}

static inline FString GetForgePilotDownloadsRoot()
{
	return FPaths::Combine(GetForgePilotUserDownloadsDir(), TEXT("ForgePilot"));
}

static inline UWorld* GetForgePilotEditorWorld(const bool bPreferPlayWorld = false)
{
#if WITH_EDITOR
	if (!GEditor)
	{
		return nullptr;
	}

	if (bPreferPlayWorld && GEditor->PlayWorld)
	{
		return GEditor->PlayWorld;
	}

	if (UWorld* EditorWorld = GEditor->GetEditorWorldContext().World())
	{
		return EditorWorld;
	}

	if (GEditor->PlayWorld)
	{
		return GEditor->PlayWorld;
	}
#endif

	return nullptr;
}

// Helper to get PinType category display name
static inline FString PinTypeToDisplayString(const FEdGraphPinType& PinType)
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	if (PinType.PinCategory == K2Schema->PC_Boolean) return TEXT("bool");
	if (PinType.PinCategory == K2Schema->PC_Byte)
	{
		if (UEnum* E = Cast<UEnum>(PinType.PinSubCategoryObject.Get()))
			return E->GetName();
		return TEXT("byte");
	}
	if (PinType.PinCategory == K2Schema->PC_Int) return TEXT("int");
	if (PinType.PinCategory == K2Schema->PC_Int64) return TEXT("int64");
	if (PinType.PinCategory == K2Schema->PC_Float) return TEXT("float");
	if (PinType.PinCategory == K2Schema->PC_Double) return TEXT("double");
	if (PinType.PinCategory == K2Schema->PC_Real) return TEXT("real");
	if (PinType.PinCategory == K2Schema->PC_String) return TEXT("string");
	if (PinType.PinCategory == K2Schema->PC_Text) return TEXT("text");
	if (PinType.PinCategory == K2Schema->PC_Name) return TEXT("name");
	if (PinType.PinCategory == K2Schema->PC_Struct)
	{
		if (UScriptStruct* S = Cast<UScriptStruct>(PinType.PinSubCategoryObject.Get()))
			return S->GetName();
		return TEXT("struct");
	}
	if (PinType.PinCategory == K2Schema->PC_Object || PinType.PinCategory == K2Schema->PC_SoftObject)
	{
		if (UClass* C = Cast<UClass>(PinType.PinSubCategoryObject.Get()))
			return C->GetName();
		return TEXT("object");
	}
	if (PinType.PinCategory == K2Schema->PC_Class) return TEXT("class");
	if (PinType.PinCategory == K2Schema->PC_Interface) return TEXT("interface");

	return PinType.PinCategory.ToString();
}
