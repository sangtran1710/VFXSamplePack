// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "AIProviderAdapters.h"
#include "VFXToolRegistry.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonValue.h"
#include "Misc/Base64.h"

// ============================================================
// Helpers
// ============================================================

static FString JsonToString(TSharedPtr<FJsonObject> Obj)
{
	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);
	return Out;
}

// ============================================================
// FAnthropicAdapter
// ============================================================

FAnthropicAdapter::FAnthropicAdapter()
{
}

void FAnthropicAdapter::SetApiKey(const FString& InApiKey)
{
	ApiKey = InApiKey;
}

void FAnthropicAdapter::SetModel(const FString& InModel)
{
	Model = InModel;
}

void FAnthropicAdapter::SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry)
{
	ToolRegistry = InToolRegistry;
}

FString FAnthropicAdapter::GetDefaultModel() const
{
	return TEXT("claude-sonnet-4-20250514");
}

FString FAnthropicAdapter::GetProviderName() const
{
	return TEXT("Anthropic");
}

TArray<TSharedPtr<FJsonValue>> FAnthropicAdapter::BuildTools() const
{
	return ToolRegistry.IsValid() ? ToolRegistry->GetToolDefinitionsJson() : TArray<TSharedPtr<FJsonValue>>();
}

void FAnthropicAdapter::BuildRequest(
	const TArray<FAgentMessage>& ConversationHistory,
	const FString& SystemPrompt,
	FString& OutUrl,
	TMap<FString, FString>& OutHeaders,
	FString& OutBody)
{
	OutUrl = TEXT("https://api.anthropic.com/v1/messages");
	OutHeaders.Add(TEXT("Content-Type"), TEXT("application/json"));
	OutHeaders.Add(TEXT("x-api-key"), ApiKey);
	OutHeaders.Add(TEXT("anthropic-version"), TEXT("2023-06-01"));

	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());
	Payload->SetStringField(TEXT("model"), Model.IsEmpty() ? GetDefaultModel() : Model);
	Payload->SetNumberField(TEXT("max_tokens"), 8192);

	if (!SystemPrompt.IsEmpty())
		Payload->SetStringField(TEXT("system"), SystemPrompt);

	auto Tools = BuildTools();
	if (Tools.Num() > 0)
		Payload->SetArrayField(TEXT("tools"), Tools);

	// Build messages
	TArray<TSharedPtr<FJsonValue>> Msgs;
	for (const FAgentMessage& M : ConversationHistory)
	{
		TSharedPtr<FJsonObject> MsgObj = MakeShareable(new FJsonObject());
		MsgObj->SetStringField(TEXT("role"), M.Role);

		TArray<TSharedPtr<FJsonValue>> ContentArr;
		for (const FAgentContentBlock& B : M.Content)
		{
			TSharedPtr<FJsonObject> Block = MakeShareable(new FJsonObject());
			if (B.Type == TEXT("text"))
			{
				Block->SetStringField(TEXT("type"), TEXT("text"));
				Block->SetStringField(TEXT("text"), B.Text);
			}
			else if (B.Type == TEXT("image"))
			{
				Block->SetStringField(TEXT("type"), TEXT("image"));
				TSharedPtr<FJsonObject> Source = MakeShareable(new FJsonObject());
				Source->SetStringField(TEXT("type"), TEXT("base64"));
				Source->SetStringField(TEXT("media_type"), B.ImageMediaType);
				Source->SetStringField(TEXT("data"), B.ImageBase64);
				Block->SetObjectField(TEXT("source"), Source);
			}
			else if (B.Type == TEXT("tool_use"))
			{
				Block->SetStringField(TEXT("type"), TEXT("tool_use"));
				Block->SetStringField(TEXT("id"), B.ToolUseId);
				Block->SetStringField(TEXT("name"), B.ToolName);
				Block->SetObjectField(TEXT("input"), B.ToolInput.IsValid() ? B.ToolInput : MakeShareable(new FJsonObject()));
			}
			else if (B.Type == TEXT("tool_result"))
			{
				Block->SetStringField(TEXT("type"), TEXT("tool_result"));
				Block->SetStringField(TEXT("tool_use_id"), B.ToolResultId);
				Block->SetStringField(TEXT("content"), B.ResultContent);
				if (B.bIsError) Block->SetBoolField(TEXT("is_error"), true);
			}
			ContentArr.Add(MakeShareable(new FJsonValueObject(Block)));
		}
		MsgObj->SetArrayField(TEXT("content"), ContentArr);
		Msgs.Add(MakeShareable(new FJsonValueObject(MsgObj)));
	}
	Payload->SetArrayField(TEXT("messages"), Msgs);

	OutBody = JsonToString(Payload);
}

bool FAnthropicAdapter::ParseResponse(
	TSharedPtr<FJsonObject> Json,
	TArray<FAgentContentBlock>& OutContent,
	FString& OutTextOutput,
	bool& bOutHasToolCalls)
{
	FString StopReason;
	Json->TryGetStringField(TEXT("stop_reason"), StopReason);

	const TArray<TSharedPtr<FJsonValue>>* ContentArr;
	if (!Json->TryGetArrayField(TEXT("content"), ContentArr))
	{
		return false;
	}

	for (const auto& V : *ContentArr)
	{
		auto BlockObj = V->AsObject();
		if (!BlockObj.IsValid()) continue;

		FString BlockType;
		BlockObj->TryGetStringField(TEXT("type"), BlockType);

		FAgentContentBlock Block;
		Block.Type = BlockType;

		if (BlockType == TEXT("text"))
		{
			BlockObj->TryGetStringField(TEXT("text"), Block.Text);
			OutTextOutput += Block.Text;
		}
		else if (BlockType == TEXT("tool_use"))
		{
			BlockObj->TryGetStringField(TEXT("id"), Block.ToolUseId);
			BlockObj->TryGetStringField(TEXT("name"), Block.ToolName);
			const TSharedPtr<FJsonObject>* Inp;
			Block.ToolInput = BlockObj->TryGetObjectField(TEXT("input"), Inp)
				? *Inp : MakeShareable(new FJsonObject());
		}
		OutContent.Add(Block);
	}

	bOutHasToolCalls = (StopReason == TEXT("tool_use"));
	return true;
}

FString FAnthropicAdapter::ParseError(int32 ResponseCode, const FString& ErrorBody) const
{
	TSharedPtr<FJsonObject> ErrJson;
	TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(ErrorBody);
	if (FJsonSerializer::Deserialize(R, ErrJson) && ErrJson.IsValid())
	{
		const TSharedPtr<FJsonObject>* ErrObj;
		if (ErrJson->TryGetObjectField(TEXT("error"), ErrObj))
		{
			FString Msg;
			if ((*ErrObj)->TryGetStringField(TEXT("message"), Msg))
			{
				return FString::Printf(TEXT("Anthropic %d: %s"), ResponseCode, *Msg);
			}
		}
	}
	return FString::Printf(TEXT("Anthropic Error %d: %s"), ResponseCode, *ErrorBody.Left(500));
}

// ============================================================
// FOpenAIAdapter
// ============================================================

FOpenAIAdapter::FOpenAIAdapter()
{
}

void FOpenAIAdapter::SetApiKey(const FString& InApiKey)
{
	ApiKey = InApiKey;
}

void FOpenAIAdapter::SetModel(const FString& InModel)
{
	Model = InModel;
}

void FOpenAIAdapter::SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry)
{
	ToolRegistry = InToolRegistry;
}

FString FOpenAIAdapter::GetDefaultModel() const
{
	return TEXT("gpt-4o");
}

FString FOpenAIAdapter::GetProviderName() const
{
	return TEXT("OpenAI");
}

TArray<TSharedPtr<FJsonValue>> FOpenAIAdapter::BuildTools() const
{
	if (!ToolRegistry.IsValid()) return {};

	TArray<TSharedPtr<FJsonValue>> Tools;
	auto RawTools = ToolRegistry->GetToolDefinitionsJson();

	for (const auto& T : RawTools)
	{
		auto ToolObj = T->AsObject();
		if (!ToolObj.IsValid()) continue;

		TSharedPtr<FJsonObject> FuncObj = MakeShareable(new FJsonObject());
		FuncObj->SetStringField(TEXT("name"), ToolObj->GetStringField(TEXT("name")));
		FuncObj->SetStringField(TEXT("description"), ToolObj->GetStringField(TEXT("description")));

		const TSharedPtr<FJsonObject>* Schema;
		if (ToolObj->TryGetObjectField(TEXT("input_schema"), Schema))
			FuncObj->SetObjectField(TEXT("parameters"), *Schema);

		TSharedPtr<FJsonObject> Wrapper = MakeShareable(new FJsonObject());
		Wrapper->SetStringField(TEXT("type"), TEXT("function"));
		Wrapper->SetObjectField(TEXT("function"), FuncObj);
		Tools.Add(MakeShareable(new FJsonValueObject(Wrapper)));
	}
	return Tools;
}

void FOpenAIAdapter::BuildRequest(
	const TArray<FAgentMessage>& ConversationHistory,
	const FString& SystemPrompt,
	FString& OutUrl,
	TMap<FString, FString>& OutHeaders,
	FString& OutBody)
{
	OutUrl = TEXT("https://api.openai.com/v1/chat/completions");
	OutHeaders.Add(TEXT("Content-Type"), TEXT("application/json"));
	OutHeaders.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));

	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());
	Payload->SetStringField(TEXT("model"), Model.IsEmpty() ? GetDefaultModel() : Model);
	Payload->SetNumberField(TEXT("max_tokens"), 8192);

	TArray<TSharedPtr<FJsonValue>> Msgs;

	// System message
	if (!SystemPrompt.IsEmpty())
	{
		TSharedPtr<FJsonObject> SysMsg = MakeShareable(new FJsonObject());
		SysMsg->SetStringField(TEXT("role"), TEXT("system"));
		SysMsg->SetStringField(TEXT("content"), SystemPrompt);
		Msgs.Add(MakeShareable(new FJsonValueObject(SysMsg)));
	}

	// Conversation history
	for (const FAgentMessage& M : ConversationHistory)
	{
		if (M.Role == TEXT("user"))
		{
			bool bHasToolResults = false;
			for (const auto& B : M.Content)
			{
				if (B.Type == TEXT("tool_result"))
				{
					bHasToolResults = true;
					TSharedPtr<FJsonObject> ToolMsg = MakeShareable(new FJsonObject());
					ToolMsg->SetStringField(TEXT("role"), TEXT("tool"));
					ToolMsg->SetStringField(TEXT("tool_call_id"), B.ToolResultId);
					ToolMsg->SetStringField(TEXT("content"), B.ResultContent);
					Msgs.Add(MakeShareable(new FJsonValueObject(ToolMsg)));
				}
			}

			if (!bHasToolResults)
			{
				bool bHasImages = false;
				for (const auto& B : M.Content)
				{
					if (B.Type == TEXT("image")) { bHasImages = true; break; }
				}

				TSharedPtr<FJsonObject> UserMsg = MakeShareable(new FJsonObject());
				UserMsg->SetStringField(TEXT("role"), TEXT("user"));

				if (bHasImages)
				{
					TArray<TSharedPtr<FJsonValue>> Parts;
					for (const auto& B : M.Content)
					{
						if (B.Type == TEXT("image"))
						{
							TSharedPtr<FJsonObject> ImgPart = MakeShareable(new FJsonObject());
							ImgPart->SetStringField(TEXT("type"), TEXT("image_url"));
							TSharedPtr<FJsonObject> ImgUrl = MakeShareable(new FJsonObject());
							ImgUrl->SetStringField(TEXT("url"),
								FString::Printf(TEXT("data:%s;base64,%s"), *B.ImageMediaType, *B.ImageBase64));
							ImgPart->SetObjectField(TEXT("image_url"), ImgUrl);
							Parts.Add(MakeShareable(new FJsonValueObject(ImgPart)));
						}
						else if (B.Type == TEXT("text"))
						{
							TSharedPtr<FJsonObject> TextPart = MakeShareable(new FJsonObject());
							TextPart->SetStringField(TEXT("type"), TEXT("text"));
							TextPart->SetStringField(TEXT("text"), B.Text);
							Parts.Add(MakeShareable(new FJsonValueObject(TextPart)));
						}
					}
					UserMsg->SetArrayField(TEXT("content"), Parts);
				}
				else
				{
					FString CombinedText;
					for (const auto& B : M.Content)
					{
						if (B.Type == TEXT("text")) CombinedText += B.Text;
					}
					UserMsg->SetStringField(TEXT("content"), CombinedText);
				}
				Msgs.Add(MakeShareable(new FJsonValueObject(UserMsg)));
			}
		}
		else if (M.Role == TEXT("assistant"))
		{
			TSharedPtr<FJsonObject> AsstMsg = MakeShareable(new FJsonObject());
			AsstMsg->SetStringField(TEXT("role"), TEXT("assistant"));

			TArray<TSharedPtr<FJsonValue>> ToolCallsArr;
			FString TextContent;

			for (const auto& B : M.Content)
			{
				if (B.Type == TEXT("text")) TextContent += B.Text;
				else if (B.Type == TEXT("tool_use"))
				{
					TSharedPtr<FJsonObject> TC = MakeShareable(new FJsonObject());
					TC->SetStringField(TEXT("id"), B.ToolUseId);
					TC->SetStringField(TEXT("type"), TEXT("function"));

					TSharedPtr<FJsonObject> Func = MakeShareable(new FJsonObject());
					Func->SetStringField(TEXT("name"), B.ToolName);
					Func->SetStringField(TEXT("arguments"), B.ToolInput.IsValid() ? JsonToString(B.ToolInput) : TEXT("{}"));
					TC->SetObjectField(TEXT("function"), Func);

					ToolCallsArr.Add(MakeShareable(new FJsonValueObject(TC)));
				}
			}

			if (!TextContent.IsEmpty())
				AsstMsg->SetStringField(TEXT("content"), TextContent);
			else
				AsstMsg->SetField(TEXT("content"), MakeShareable(new FJsonValueNull()));

			if (ToolCallsArr.Num() > 0)
				AsstMsg->SetArrayField(TEXT("tool_calls"), ToolCallsArr);

			Msgs.Add(MakeShareable(new FJsonValueObject(AsstMsg)));
		}
	}

	Payload->SetArrayField(TEXT("messages"), Msgs);

	auto Tools = BuildTools();
	if (Tools.Num() > 0)
		Payload->SetArrayField(TEXT("tools"), Tools);

	OutBody = JsonToString(Payload);
}

bool FOpenAIAdapter::ParseResponse(
	TSharedPtr<FJsonObject> Json,
	TArray<FAgentContentBlock>& OutContent,
	FString& OutTextOutput,
	bool& bOutHasToolCalls)
{
	const TArray<TSharedPtr<FJsonValue>>* Choices;
	if (!Json->TryGetArrayField(TEXT("choices"), Choices) || Choices->Num() == 0)
	{
		return false;
	}

	auto Choice = (*Choices)[0]->AsObject();
	FString FinishReason;
	Choice->TryGetStringField(TEXT("finish_reason"), FinishReason);

	const TSharedPtr<FJsonObject>* MsgObj;
	if (!Choice->TryGetObjectField(TEXT("message"), MsgObj))
	{
		return false;
	}

	// Text content
	FString TextContent;
	if ((*MsgObj)->TryGetStringField(TEXT("content"), TextContent) && !TextContent.IsEmpty())
	{
		FAgentContentBlock Block;
		Block.Type = TEXT("text");
		Block.Text = TextContent;
		OutContent.Add(Block);
		OutTextOutput = TextContent;
	}

	// Tool calls
	const TArray<TSharedPtr<FJsonValue>>* ToolCallsArr;
	if ((*MsgObj)->TryGetArrayField(TEXT("tool_calls"), ToolCallsArr))
	{
		for (const auto& TC : *ToolCallsArr)
		{
			auto TCObj = TC->AsObject();
			if (!TCObj.IsValid()) continue;

			FAgentContentBlock Block;
			Block.Type = TEXT("tool_use");
			TCObj->TryGetStringField(TEXT("id"), Block.ToolUseId);

			const TSharedPtr<FJsonObject>* FuncObj;
			if (TCObj->TryGetObjectField(TEXT("function"), FuncObj))
			{
				(*FuncObj)->TryGetStringField(TEXT("name"), Block.ToolName);

				FString ArgsStr;
				if ((*FuncObj)->TryGetStringField(TEXT("arguments"), ArgsStr))
				{
					TSharedPtr<FJsonObject> Args;
					TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(ArgsStr);
					if (FJsonSerializer::Deserialize(R, Args))
						Block.ToolInput = Args;
					else
						Block.ToolInput = MakeShareable(new FJsonObject());
				}
			}

			OutContent.Add(Block);
		}
	}

	bOutHasToolCalls = (FinishReason == TEXT("tool_calls"));
	return true;
}

FString FOpenAIAdapter::ParseError(int32 ResponseCode, const FString& ErrorBody) const
{
	TSharedPtr<FJsonObject> ErrJson;
	TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(ErrorBody);
	if (FJsonSerializer::Deserialize(R, ErrJson) && ErrJson.IsValid())
	{
		const TSharedPtr<FJsonObject>* ErrObj;
		if (ErrJson->TryGetObjectField(TEXT("error"), ErrObj))
		{
			FString Msg;
			if ((*ErrObj)->TryGetStringField(TEXT("message"), Msg))
			{
				return FString::Printf(TEXT("OpenAI %d: %s"), ResponseCode, *Msg);
			}
		}
	}
	return FString::Printf(TEXT("OpenAI Error %d: %s"), ResponseCode, *ErrorBody.Left(500));
}

// ============================================================
// FOpenRouterAdapter
// ============================================================

FOpenRouterAdapter::FOpenRouterAdapter()
{
}

FString FOpenRouterAdapter::GetDefaultModel() const
{
	return TEXT("deepseek/deepseek-v3.2-speciale");
}

FString FOpenRouterAdapter::GetProviderName() const
{
	return TEXT("OpenRouter");
}

void FOpenRouterAdapter::BuildRequest(
	const TArray<FAgentMessage>& ConversationHistory,
	const FString& SystemPrompt,
	FString& OutUrl,
	TMap<FString, FString>& OutHeaders,
	FString& OutBody)
{
	// Reuse OpenAI build logic
	FOpenAIAdapter::BuildRequest(ConversationHistory, SystemPrompt, OutUrl, OutHeaders, OutBody);

	// Override URL and add OpenRouter headers
	OutUrl = TEXT("https://openrouter.ai/api/v1/chat/completions");
	OutHeaders.Add(TEXT("HTTP-Referer"), TEXT("https://local-forgepilot.invalid"));
	OutHeaders.Add(TEXT("X-Title"), TEXT("ForgePilot"));
}

FString FOpenRouterAdapter::ParseError(int32 ResponseCode, const FString& ErrorBody) const
{
	// Try OpenRouter-specific error format first
	TSharedPtr<FJsonObject> ErrJson;
	TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(ErrorBody);
	if (FJsonSerializer::Deserialize(R, ErrJson) && ErrJson.IsValid())
	{
		FString Msg;
		if (ErrJson->TryGetStringField(TEXT("error"), Msg))
		{
			return FString::Printf(TEXT("OpenRouter %d: %s"), ResponseCode, *Msg);
		}
	}
	// Fall back to OpenAI format
	return FOpenAIAdapter::ParseError(ResponseCode, ErrorBody);
}

// ============================================================
// FGeminiAdapter
// ============================================================

FGeminiAdapter::FGeminiAdapter()
{
}

void FGeminiAdapter::SetApiKey(const FString& InApiKey)
{
	ApiKey = InApiKey;
}

void FGeminiAdapter::SetModel(const FString& InModel)
{
	Model = InModel;
}

void FGeminiAdapter::SetToolRegistry(TSharedPtr<FVFXToolRegistry> InToolRegistry)
{
	ToolRegistry = InToolRegistry;
}

FString FGeminiAdapter::GetDefaultModel() const
{
	return TEXT("gemini-2.0-flash");
}

FString FGeminiAdapter::GetProviderName() const
{
	return TEXT("Gemini");
}

FString FGeminiAdapter::GenerateToolCallId() const
{
	static int32 Counter = 0;
	return FString::Printf(TEXT("call_%d_%d"), FPlatformTime::Cycles(), ++Counter);
}

TArray<TSharedPtr<FJsonValue>> FGeminiAdapter::BuildTools() const
{
	if (!ToolRegistry.IsValid()) return {};

	TArray<TSharedPtr<FJsonValue>> FuncDecls;
	auto RawTools = ToolRegistry->GetToolDefinitionsJson();

	for (const auto& T : RawTools)
	{
		auto ToolObj = T->AsObject();
		if (!ToolObj.IsValid()) continue;

		TSharedPtr<FJsonObject> Decl = MakeShareable(new FJsonObject());
		Decl->SetStringField(TEXT("name"), ToolObj->GetStringField(TEXT("name")));
		Decl->SetStringField(TEXT("description"), ToolObj->GetStringField(TEXT("description")));

		const TSharedPtr<FJsonObject>* Schema;
		if (ToolObj->TryGetObjectField(TEXT("input_schema"), Schema))
			Decl->SetObjectField(TEXT("parameters"), *Schema);

		FuncDecls.Add(MakeShareable(new FJsonValueObject(Decl)));
	}
	return FuncDecls;
}

void FGeminiAdapter::BuildRequest(
	const TArray<FAgentMessage>& ConversationHistory,
	const FString& SystemPrompt,
	FString& OutUrl,
	TMap<FString, FString>& OutHeaders,
	FString& OutBody)
{
	OutUrl = FString::Printf(
		TEXT("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s"),
		*(Model.IsEmpty() ? GetDefaultModel() : Model), *ApiKey);
	OutHeaders.Add(TEXT("Content-Type"), TEXT("application/json"));

	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());

	// System instruction
	if (!SystemPrompt.IsEmpty())
	{
		TSharedPtr<FJsonObject> SysInstr = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonValue>> Parts;
		TSharedPtr<FJsonObject> TextPart = MakeShareable(new FJsonObject());
		TextPart->SetStringField(TEXT("text"), SystemPrompt);
		Parts.Add(MakeShareable(new FJsonValueObject(TextPart)));
		SysInstr->SetArrayField(TEXT("parts"), Parts);
		Payload->SetObjectField(TEXT("systemInstruction"), SysInstr);
	}

	// Contents (conversation)
	TArray<TSharedPtr<FJsonValue>> Contents;
	for (const FAgentMessage& M : ConversationHistory)
	{
		FString GeminiRole = (M.Role == TEXT("assistant")) ? TEXT("model") : TEXT("user");
		TSharedPtr<FJsonObject> Content = MakeShareable(new FJsonObject());
		Content->SetStringField(TEXT("role"), GeminiRole);

		TArray<TSharedPtr<FJsonValue>> Parts;
		for (const auto& B : M.Content)
		{
			TSharedPtr<FJsonObject> Part = MakeShareable(new FJsonObject());

			if (B.Type == TEXT("text"))
			{
				Part->SetStringField(TEXT("text"), B.Text);
			}
			else if (B.Type == TEXT("image"))
			{
				TSharedPtr<FJsonObject> InlineData = MakeShareable(new FJsonObject());
				InlineData->SetStringField(TEXT("mimeType"), B.ImageMediaType);
				InlineData->SetStringField(TEXT("data"), B.ImageBase64);
				Part->SetObjectField(TEXT("inlineData"), InlineData);
			}
			else if (B.Type == TEXT("tool_use"))
			{
				TSharedPtr<FJsonObject> FuncCall = MakeShareable(new FJsonObject());
				FuncCall->SetStringField(TEXT("name"), B.ToolName);
				FuncCall->SetObjectField(TEXT("args"), B.ToolInput.IsValid() ? B.ToolInput : MakeShareable(new FJsonObject()));
				Part->SetObjectField(TEXT("functionCall"), FuncCall);
			}
			else if (B.Type == TEXT("tool_result"))
			{
				TSharedPtr<FJsonObject> FuncResp = MakeShareable(new FJsonObject());
				FuncResp->SetStringField(TEXT("name"), B.ToolName);

				TSharedPtr<FJsonObject> RespObj;
				TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(B.ResultContent);
				if (FJsonSerializer::Deserialize(R, RespObj) && RespObj.IsValid())
				{
					FuncResp->SetObjectField(TEXT("response"), RespObj);
				}
				else
				{
					TSharedPtr<FJsonObject> Wrapper = MakeShareable(new FJsonObject());
					Wrapper->SetStringField(TEXT("result"), B.ResultContent);
					FuncResp->SetObjectField(TEXT("response"), Wrapper);
				}

				Part->SetObjectField(TEXT("functionResponse"), FuncResp);
			}

			Parts.Add(MakeShareable(new FJsonValueObject(Part)));
		}

		Content->SetArrayField(TEXT("parts"), Parts);
		Contents.Add(MakeShareable(new FJsonValueObject(Content)));
	}
	Payload->SetArrayField(TEXT("contents"), Contents);

	// Tools
	auto FuncDecls = BuildTools();
	if (FuncDecls.Num() > 0)
	{
		TSharedPtr<FJsonObject> ToolsDef = MakeShareable(new FJsonObject());
		ToolsDef->SetArrayField(TEXT("functionDeclarations"), FuncDecls);
		TArray<TSharedPtr<FJsonValue>> ToolsArr;
		ToolsArr.Add(MakeShareable(new FJsonValueObject(ToolsDef)));
		Payload->SetArrayField(TEXT("tools"), ToolsArr);
	}

	OutBody = JsonToString(Payload);
}

bool FGeminiAdapter::ParseResponse(
	TSharedPtr<FJsonObject> Json,
	TArray<FAgentContentBlock>& OutContent,
	FString& OutTextOutput,
	bool& bOutHasToolCalls)
{
	const TArray<TSharedPtr<FJsonValue>>* Candidates;
	if (!Json->TryGetArrayField(TEXT("candidates"), Candidates) || Candidates->Num() == 0)
	{
		return false;
	}

	auto Candidate = (*Candidates)[0]->AsObject();
	const TSharedPtr<FJsonObject>* ContentObj;
	if (!Candidate->TryGetObjectField(TEXT("content"), ContentObj))
	{
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* PartsArr;
	if ((*ContentObj)->TryGetArrayField(TEXT("parts"), PartsArr))
	{
		for (const auto& P : *PartsArr)
		{
			auto PartObj = P->AsObject();
			if (!PartObj.IsValid()) continue;

			FAgentContentBlock Block;

			FString TextVal;
			if (PartObj->TryGetStringField(TEXT("text"), TextVal))
			{
				Block.Type = TEXT("text");
				Block.Text = TextVal;
				OutTextOutput += TextVal;
			}
			else
			{
				const TSharedPtr<FJsonObject>* FuncCallObj;
				if (PartObj->TryGetObjectField(TEXT("functionCall"), FuncCallObj))
				{
					Block.Type = TEXT("tool_use");
					(*FuncCallObj)->TryGetStringField(TEXT("name"), Block.ToolName);
					Block.ToolUseId = GenerateToolCallId();

					const TSharedPtr<FJsonObject>* ArgsObj;
					Block.ToolInput = (*FuncCallObj)->TryGetObjectField(TEXT("args"), ArgsObj)
						? *ArgsObj : MakeShareable(new FJsonObject());
				}
			}

			OutContent.Add(Block);
		}
	}

	bOutHasToolCalls = false;
	for (const auto& C : OutContent)
	{
		if (C.Type == TEXT("tool_use"))
		{
			bOutHasToolCalls = true;
			break;
		}
	}

	return true;
}

FString FGeminiAdapter::ParseError(int32 ResponseCode, const FString& ErrorBody) const
{
	TSharedPtr<FJsonObject> ErrJson;
	TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(ErrorBody);
	if (FJsonSerializer::Deserialize(R, ErrJson) && ErrJson.IsValid())
	{
		const TSharedPtr<FJsonObject>* ErrObj;
		if (ErrJson->TryGetObjectField(TEXT("error"), ErrObj))
		{
			FString Msg;
			if ((*ErrObj)->TryGetStringField(TEXT("message"), Msg))
			{
				return FString::Printf(TEXT("Gemini %d: %s"), ResponseCode, *Msg);
			}
		}
	}
	return FString::Printf(TEXT("Gemini Error %d: %s"), ResponseCode, *ErrorBody.Left(500));
}
