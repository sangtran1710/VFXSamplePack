// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

// ============================================================
// JSON Helpers (Standardized)
// ============================================================

// Helper to create a simple success response with data
static TSharedPtr<FJsonObject> MakeSuccessJson()
{
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetBoolField(TEXT("success"), true);
	return Obj;
}

// Helper to create a success response with message
static TSharedPtr<FJsonObject> MakeSuccessJson(const FString& Message)
{
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetBoolField(TEXT("success"), true);
	Obj->SetStringField(TEXT("message"), Message);
	return Obj;
}

// Helper to create a success response with message and data
static TSharedPtr<FJsonObject> MakeSuccessJson(const FString& Message, const FString& DataKey, const FString& DataValue)
{
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetBoolField(TEXT("success"), true);
	Obj->SetStringField(TEXT("message"), Message);
	Obj->SetStringField(DataKey, DataValue);
	return Obj;
}

// Helper to create an error response with error_code and error_message
static TSharedPtr<FJsonObject> MakeErrorJson(const FString& ErrorCode, const FString& ErrorMessage)
{
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetBoolField(TEXT("success"), false);
	Obj->SetStringField(TEXT("error_code"), ErrorCode);
	Obj->SetStringField(TEXT("error_message"), ErrorMessage);
	return Obj;
}

// Helper to create an error response with debug context
static TSharedPtr<FJsonObject> MakeErrorJson(const FString& ErrorCode, const FString& ErrorMessage, const FString& DebugKey, const FString& DebugValue)
{
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetBoolField(TEXT("success"), false);
	Obj->SetStringField(TEXT("error_code"), ErrorCode);
	Obj->SetStringField(TEXT("error_message"), ErrorMessage);

	TSharedPtr<FJsonObject> DebugCtx = MakeShareable(new FJsonObject());
	DebugCtx->SetStringField(DebugKey, DebugValue);
	Obj->SetObjectField(TEXT("debug_context"), DebugCtx);

	return Obj;
}

// Convert FJsonObject to string
static FString JsonObjToString(TSharedPtr<FJsonObject> Obj)
{
	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);
	return Out;
}

// Helper to make empty object schema for tools
static TSharedPtr<FJsonObject> MakeEmptyObjectSchema()
{
	TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	Schema->SetObjectField(TEXT("properties"), MakeShareable(new FJsonObject()));
	return Schema;
}

// Helper to make string property schema
static TSharedPtr<FJsonObject> MakeStringProperty(const FString& Description)
{
	TSharedPtr<FJsonObject> Prop = MakeShareable(new FJsonObject());
	Prop->SetStringField(TEXT("type"), TEXT("string"));
	Prop->SetStringField(TEXT("description"), Description);
	return Prop;
}

// Helper to make array property schema
static TSharedPtr<FJsonObject> MakeArrayProperty(const FString& Description, const FString& ItemType)
{
	TSharedPtr<FJsonObject> Prop = MakeShareable(new FJsonObject());
	Prop->SetStringField(TEXT("type"), TEXT("array"));
	Prop->SetStringField(TEXT("description"), Description);

	TSharedPtr<FJsonObject> Items = MakeShareable(new FJsonObject());
	Items->SetStringField(TEXT("type"), ItemType);
	Prop->SetObjectField(TEXT("items"), Items);

	return Prop;
}
