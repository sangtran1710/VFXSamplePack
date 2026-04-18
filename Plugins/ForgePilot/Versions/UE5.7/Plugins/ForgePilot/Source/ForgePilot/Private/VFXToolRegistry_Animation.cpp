// Copyright 2026, HenryFX. ForgePilot Plugin.
// Phase 5: Animation-adjacent inspection tools
#include "VFXToolRegistry.h"
#include "VFXToolRegistryHelpers.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimCurveCompressionSettings.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimTypes.h"
#include "Animation/Skeleton.h"
#include "AnimationBlueprintLibrary.h"
#include "ContentBrowserModule.h"
#include "Editor.h"
#include "EditorAssetLibrary.h"
#include "IContentBrowserSingleton.h"
#include "Modules/ModuleManager.h"
#include "Subsystems/AssetEditorSubsystem.h"

namespace
{
	struct FAnimationInspectionSummary
	{
		bool bIsSequence = false;
		bool bIsMontage = false;
		float LengthSeconds = 0.0f;
		float RateScale = 1.0f;
		int32 NumFrames = 0;
		int32 NumKeys = 0;
		int32 TrackCount = 0;
		int32 NotifyTrackCount = 0;
		int32 NotifyCount = 0;
		int32 FloatCurveCount = 0;
		int32 VectorCurveCount = 0;
		int32 TransformCurveCount = 0;
		int32 MetaDataCount = 0;
		int32 SectionCount = 0;
		int32 SlotCount = 0;
		bool bRootMotionEnabled = false;
	};

	static bool ParseOptionalBoolField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, bool DefaultValue)
	{
		if (!Input.IsValid())
		{
			return DefaultValue;
		}

		bool BoolValue = DefaultValue;
		if (Input->TryGetBoolField(FieldName, BoolValue))
		{
			return BoolValue;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue))
		{
			return StringValue.Equals(TEXT("true"), ESearchCase::IgnoreCase) || StringValue == TEXT("1");
		}
		return DefaultValue;
	}

	static int32 ParseOptionalIntField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, int32 DefaultValue)
	{
		if (!Input.IsValid())
		{
			return DefaultValue;
		}

		double NumberValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumberValue))
		{
			return FMath::RoundToInt(NumberValue);
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue) && !StringValue.IsEmpty())
		{
			return FCString::Atoi(*StringValue);
		}

		return DefaultValue;
	}

	static bool BuildAnimationToolError(
		FString& OutResult,
		const FString& ErrorCode,
		const FString& ErrorMessage,
		const TSharedPtr<FJsonObject>& DebugContext = nullptr)
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetBoolField(TEXT("success"), false);
		ErrorObj->SetStringField(TEXT("error_code"), ErrorCode);
		ErrorObj->SetStringField(TEXT("error_message"), ErrorMessage);
		if (DebugContext.IsValid())
		{
			ErrorObj->SetObjectField(TEXT("debug_context"), DebugContext);
		}
		OutResult = JsonObjToString(ErrorObj);
		return false;
	}

	static FString CurveTypeToString(const ERawCurveTrackTypes CurveType)
	{
		switch (CurveType)
		{
		case ERawCurveTrackTypes::RCT_Float:
			return TEXT("Float");
		case ERawCurveTrackTypes::RCT_Vector:
			return TEXT("Vector");
		case ERawCurveTrackTypes::RCT_Transform:
			return TEXT("Transform");
		default:
			return TEXT("Unknown");
		}
	}

	static FString VectorToCompactString(const FVector& Value)
	{
		return FString::Printf(TEXT("(X=%.3f,Y=%.3f,Z=%.3f)"), Value.X, Value.Y, Value.Z);
	}

	static FString TransformToCompactString(const FTransform& Value)
	{
		const FVector Location = Value.GetLocation();
		const FRotator Rotation = Value.Rotator();
		const FVector Scale = Value.GetScale3D();
		return FString::Printf(
			TEXT("Loc(X=%.3f,Y=%.3f,Z=%.3f) Rot(P=%.3f,Y=%.3f,R=%.3f) Scale(X=%.3f,Y=%.3f,Z=%.3f)"),
			Location.X,
			Location.Y,
			Location.Z,
			Rotation.Pitch,
			Rotation.Yaw,
			Rotation.Roll,
			Scale.X,
			Scale.Y,
			Scale.Z);
	}

	static FString EnumToString(const UEnum* Enum, int64 Value)
	{
		return Enum ? Enum->GetNameStringByValue(Value) : FString::FromInt(static_cast<int32>(Value));
	}

	static TSharedPtr<FJsonObject> BuildNotifyEventObject(const FAnimNotifyEvent& NotifyEvent, const TArray<FName>& TrackNames)
	{
		TSharedPtr<FJsonObject> NotifyObj = MakeShareable(new FJsonObject());
		NotifyObj->SetStringField(TEXT("event_name"), NotifyEvent.GetNotifyEventName().ToString());
		NotifyObj->SetStringField(TEXT("notify_name"), NotifyEvent.NotifyName.ToString());
		NotifyObj->SetNumberField(TEXT("track_index"), NotifyEvent.TrackIndex);
		if (TrackNames.IsValidIndex(NotifyEvent.TrackIndex))
		{
			NotifyObj->SetStringField(TEXT("track_name"), TrackNames[NotifyEvent.TrackIndex].ToString());
		}
		NotifyObj->SetNumberField(TEXT("trigger_time"), NotifyEvent.GetTriggerTime());
		NotifyObj->SetNumberField(TEXT("end_trigger_time"), NotifyEvent.GetEndTriggerTime());
		NotifyObj->SetNumberField(TEXT("duration"), NotifyEvent.GetDuration());
		NotifyObj->SetBoolField(TEXT("is_state"), NotifyEvent.NotifyStateClass != nullptr);
		NotifyObj->SetBoolField(TEXT("is_branching_point"), NotifyEvent.IsBranchingPoint());
		NotifyObj->SetNumberField(TEXT("trigger_weight_threshold"), NotifyEvent.TriggerWeightThreshold);
		NotifyObj->SetNumberField(TEXT("trigger_chance"), NotifyEvent.NotifyTriggerChance);
		NotifyObj->SetBoolField(TEXT("trigger_on_dedicated_server"), NotifyEvent.bTriggerOnDedicatedServer);
		NotifyObj->SetBoolField(TEXT("trigger_on_follower"), NotifyEvent.bTriggerOnFollower);

		if (NotifyEvent.Notify)
		{
			NotifyObj->SetStringField(TEXT("notify_class"), NotifyEvent.Notify->GetClass()->GetName());
			NotifyObj->SetStringField(TEXT("notify_path"), NotifyEvent.Notify->GetPathName());
			NotifyObj->SetStringField(TEXT("notify_display_name"), NotifyEvent.Notify->GetNotifyName());
		}

		if (NotifyEvent.NotifyStateClass)
		{
			NotifyObj->SetStringField(TEXT("notify_state_class"), NotifyEvent.NotifyStateClass->GetClass()->GetName());
			NotifyObj->SetStringField(TEXT("notify_state_path"), NotifyEvent.NotifyStateClass->GetPathName());
			NotifyObj->SetStringField(TEXT("notify_state_display_name"), NotifyEvent.NotifyStateClass->GetNotifyName());
		}

		return NotifyObj;
	}

	static void AddFloatCurveObjects(UAnimSequence* AnimSequence, int32 MaxCurveKeysPreview, TArray<TSharedPtr<FJsonValue>>& OutCurves)
	{
		TArray<FName> CurveNames;
		UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Float, CurveNames);
		for (const FName& CurveName : CurveNames)
		{
			TArray<float> Times;
			TArray<float> Values;
			UAnimationBlueprintLibrary::GetFloatKeys(AnimSequence, CurveName, Times, Values);

			TArray<TSharedPtr<FJsonValue>> PreviewArray;
			const int32 PreviewCount = FMath::Min3(MaxCurveKeysPreview, Times.Num(), Values.Num());
			for (int32 Index = 0; Index < PreviewCount; ++Index)
			{
				TSharedPtr<FJsonObject> PreviewObj = MakeShareable(new FJsonObject());
				PreviewObj->SetNumberField(TEXT("time"), Times[Index]);
				PreviewObj->SetNumberField(TEXT("value"), Values[Index]);
				PreviewArray.Add(MakeShareable(new FJsonValueObject(PreviewObj)));
			}

			TSharedPtr<FJsonObject> CurveObj = MakeShareable(new FJsonObject());
			CurveObj->SetStringField(TEXT("name"), CurveName.ToString());
			CurveObj->SetStringField(TEXT("type"), CurveTypeToString(ERawCurveTrackTypes::RCT_Float));
			CurveObj->SetNumberField(TEXT("key_count"), Times.Num());
			CurveObj->SetArrayField(TEXT("preview_keys"), PreviewArray);
			OutCurves.Add(MakeShareable(new FJsonValueObject(CurveObj)));
		}
	}

	static void AddVectorCurveObjects(UAnimSequence* AnimSequence, int32 MaxCurveKeysPreview, TArray<TSharedPtr<FJsonValue>>& OutCurves)
	{
		TArray<FName> CurveNames;
		UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Vector, CurveNames);
		for (const FName& CurveName : CurveNames)
		{
			TArray<float> Times;
			TArray<FVector> Values;
			UAnimationBlueprintLibrary::GetVectorKeys(AnimSequence, CurveName, Times, Values);

			TArray<TSharedPtr<FJsonValue>> PreviewArray;
			const int32 PreviewCount = FMath::Min3(MaxCurveKeysPreview, Times.Num(), Values.Num());
			for (int32 Index = 0; Index < PreviewCount; ++Index)
			{
				TSharedPtr<FJsonObject> PreviewObj = MakeShareable(new FJsonObject());
				PreviewObj->SetNumberField(TEXT("time"), Times[Index]);
				PreviewObj->SetStringField(TEXT("value"), VectorToCompactString(Values[Index]));
				PreviewArray.Add(MakeShareable(new FJsonValueObject(PreviewObj)));
			}

			TSharedPtr<FJsonObject> CurveObj = MakeShareable(new FJsonObject());
			CurveObj->SetStringField(TEXT("name"), CurveName.ToString());
			CurveObj->SetStringField(TEXT("type"), CurveTypeToString(ERawCurveTrackTypes::RCT_Vector));
			CurveObj->SetNumberField(TEXT("key_count"), Times.Num());
			CurveObj->SetArrayField(TEXT("preview_keys"), PreviewArray);
			OutCurves.Add(MakeShareable(new FJsonValueObject(CurveObj)));
		}
	}

static void AddTransformCurveObjects(UAnimSequence* AnimSequence, int32 MaxCurveKeysPreview, TArray<TSharedPtr<FJsonValue>>& OutCurves)
	{
		TArray<FName> CurveNames;
		UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Transform, CurveNames);
		for (const FName& CurveName : CurveNames)
		{
			TArray<float> Times;
			TArray<FTransform> Values;
			UAnimationBlueprintLibrary::GetTransformationKeys(AnimSequence, CurveName, Times, Values);

			TArray<TSharedPtr<FJsonValue>> PreviewArray;
			const int32 PreviewCount = FMath::Min3(MaxCurveKeysPreview, Times.Num(), Values.Num());
			for (int32 Index = 0; Index < PreviewCount; ++Index)
			{
				TSharedPtr<FJsonObject> PreviewObj = MakeShareable(new FJsonObject());
				PreviewObj->SetNumberField(TEXT("time"), Times[Index]);
				PreviewObj->SetStringField(TEXT("value"), TransformToCompactString(Values[Index]));
				PreviewArray.Add(MakeShareable(new FJsonValueObject(PreviewObj)));
			}

			TSharedPtr<FJsonObject> CurveObj = MakeShareable(new FJsonObject());
			CurveObj->SetStringField(TEXT("name"), CurveName.ToString());
			CurveObj->SetStringField(TEXT("type"), CurveTypeToString(ERawCurveTrackTypes::RCT_Transform));
			CurveObj->SetNumberField(TEXT("key_count"), Times.Num());
			CurveObj->SetArrayField(TEXT("preview_keys"), PreviewArray);
			OutCurves.Add(MakeShareable(new FJsonValueObject(CurveObj)));
		}
	}

	static TSharedPtr<FJsonObject> BuildAnimationAssetInfoObject(
		UAnimationAsset* AnimationAsset,
		const bool bIncludeNotifies,
		const bool bIncludeCurves,
		const int32 MaxCurveKeysPreview)
	{
		TSharedPtr<FJsonObject> AssetObj = MakeShareable(new FJsonObject());
		if (!AnimationAsset)
		{
			AssetObj->SetBoolField(TEXT("valid"), false);
			return AssetObj;
		}

		UAnimSequenceBase* SequenceBase = Cast<UAnimSequenceBase>(AnimationAsset);
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AnimationAsset);
		UAnimMontage* AnimMontage = Cast<UAnimMontage>(AnimationAsset);
		USkeleton* Skeleton = AnimationAsset->GetSkeleton();

		FAnimationInspectionSummary Summary;
		Summary.bIsSequence = (AnimSequence != nullptr);
		Summary.bIsMontage = (AnimMontage != nullptr);

		TArray<FName> TrackNames;
		TArray<FName> NotifyTrackNames;
		TArray<FAnimNotifyEvent> NotifyEvents;

		if (SequenceBase)
		{
			UAnimationBlueprintLibrary::GetSequenceLength(SequenceBase, Summary.LengthSeconds);
			UAnimationBlueprintLibrary::GetRateScale(SequenceBase, Summary.RateScale);
			UAnimationBlueprintLibrary::GetNumFrames(SequenceBase, Summary.NumFrames);
			UAnimationBlueprintLibrary::GetNumKeys(SequenceBase, Summary.NumKeys);
			UAnimationBlueprintLibrary::GetAnimationTrackNames(SequenceBase, TrackNames);
			UAnimationBlueprintLibrary::GetAnimationNotifyTrackNames(SequenceBase, NotifyTrackNames);
			UAnimationBlueprintLibrary::GetAnimationNotifyEvents(SequenceBase, NotifyEvents);

			Summary.TrackCount = TrackNames.Num();
			Summary.NotifyTrackCount = NotifyTrackNames.Num();
			Summary.NotifyCount = NotifyEvents.Num();
		}

		TArray<UAnimMetaData*> MetaData;
		UAnimationBlueprintLibrary::GetMetaData(AnimationAsset, MetaData);
		Summary.MetaDataCount = MetaData.Num();

		TArray<TSharedPtr<FJsonValue>> MetaDataArray;
		for (UAnimMetaData* MetaDataEntry : MetaData)
		{
			if (!MetaDataEntry)
			{
				continue;
			}

			TSharedPtr<FJsonObject> MetaObj = MakeShareable(new FJsonObject());
			MetaObj->SetStringField(TEXT("class"), MetaDataEntry->GetClass()->GetName());
			MetaObj->SetStringField(TEXT("path"), MetaDataEntry->GetPathName());
			MetaDataArray.Add(MakeShareable(new FJsonValueObject(MetaObj)));
		}

		if (AnimSequence)
		{
			TArray<FName> FloatCurveNames;
			TArray<FName> VectorCurveNames;
			TArray<FName> TransformCurveNames;
			UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Float, FloatCurveNames);
			UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Vector, VectorCurveNames);
			UAnimationBlueprintLibrary::GetAnimationCurveNames(AnimSequence, ERawCurveTrackTypes::RCT_Transform, TransformCurveNames);

			Summary.FloatCurveCount = FloatCurveNames.Num();
			Summary.VectorCurveCount = VectorCurveNames.Num();
			Summary.TransformCurveCount = TransformCurveNames.Num();
			Summary.bRootMotionEnabled = UAnimationBlueprintLibrary::IsRootMotionEnabled(AnimSequence);
		}

		TArray<TSharedPtr<FJsonValue>> SectionArray;
		TArray<TSharedPtr<FJsonValue>> SlotArray;
		if (AnimMontage)
		{
			Summary.SectionCount = AnimMontage->CompositeSections.Num();
			for (const FCompositeSection& Section : AnimMontage->CompositeSections)
			{
				TSharedPtr<FJsonObject> SectionObj = MakeShareable(new FJsonObject());
				SectionObj->SetStringField(TEXT("name"), Section.SectionName.ToString());
				SectionObj->SetStringField(TEXT("next_section"), Section.NextSectionName.ToString());
				SectionObj->SetNumberField(TEXT("time"), Section.GetTime());
				SectionArray.Add(MakeShareable(new FJsonValueObject(SectionObj)));
			}

			TArray<FName> SlotNames;
			UAnimationBlueprintLibrary::GetMontageSlotNames(AnimMontage, SlotNames);
			Summary.SlotCount = SlotNames.Num();
			for (const FName& SlotName : SlotNames)
			{
				SlotArray.Add(MakeShareable(new FJsonValueString(SlotName.ToString())));
			}
		}

		TSharedPtr<FJsonObject> SummaryObj = MakeShareable(new FJsonObject());
		SummaryObj->SetBoolField(TEXT("is_sequence"), Summary.bIsSequence);
		SummaryObj->SetBoolField(TEXT("is_montage"), Summary.bIsMontage);
		SummaryObj->SetNumberField(TEXT("length_seconds"), Summary.LengthSeconds);
		SummaryObj->SetNumberField(TEXT("rate_scale"), Summary.RateScale);
		SummaryObj->SetNumberField(TEXT("num_frames"), Summary.NumFrames);
		SummaryObj->SetNumberField(TEXT("num_keys"), Summary.NumKeys);
		SummaryObj->SetNumberField(TEXT("track_count"), Summary.TrackCount);
		SummaryObj->SetNumberField(TEXT("notify_track_count"), Summary.NotifyTrackCount);
		SummaryObj->SetNumberField(TEXT("notify_count"), Summary.NotifyCount);
		SummaryObj->SetNumberField(TEXT("float_curve_count"), Summary.FloatCurveCount);
		SummaryObj->SetNumberField(TEXT("vector_curve_count"), Summary.VectorCurveCount);
		SummaryObj->SetNumberField(TEXT("transform_curve_count"), Summary.TransformCurveCount);
		SummaryObj->SetNumberField(TEXT("metadata_count"), Summary.MetaDataCount);
		SummaryObj->SetNumberField(TEXT("section_count"), Summary.SectionCount);
		SummaryObj->SetNumberField(TEXT("slot_count"), Summary.SlotCount);
		SummaryObj->SetBoolField(TEXT("root_motion_enabled"), Summary.bRootMotionEnabled);

		AssetObj->SetBoolField(TEXT("valid"), true);
		AssetObj->SetStringField(TEXT("asset_path"), AnimationAsset->GetPathName());
		AssetObj->SetStringField(TEXT("asset_name"), AnimationAsset->GetName());
		AssetObj->SetStringField(TEXT("asset_class"), AnimationAsset->GetClass()->GetName());
		AssetObj->SetStringField(TEXT("skeleton_path"), Skeleton ? Skeleton->GetPathName() : TEXT(""));
		AssetObj->SetObjectField(TEXT("summary"), SummaryObj);

		TArray<TSharedPtr<FJsonValue>> TrackArray;
		for (const FName& TrackName : TrackNames)
		{
			TrackArray.Add(MakeShareable(new FJsonValueString(TrackName.ToString())));
		}
		AssetObj->SetArrayField(TEXT("track_names"), TrackArray);

		TArray<TSharedPtr<FJsonValue>> NotifyTrackArray;
		for (const FName& TrackName : NotifyTrackNames)
		{
			NotifyTrackArray.Add(MakeShareable(new FJsonValueString(TrackName.ToString())));
		}
		AssetObj->SetArrayField(TEXT("notify_track_names"), NotifyTrackArray);
		AssetObj->SetArrayField(TEXT("metadata"), MetaDataArray);

		if (AnimMontage)
		{
			AssetObj->SetArrayField(TEXT("montage_sections"), SectionArray);
			AssetObj->SetArrayField(TEXT("montage_slots"), SlotArray);
		}

		if (AnimSequence)
		{
			TEnumAsByte<EAdditiveAnimationType> AdditiveType;
			UAnimationBlueprintLibrary::GetAdditiveAnimationType(AnimSequence, AdditiveType);
			SummaryObj->SetStringField(TEXT("additive_type"), EnumToString(StaticEnum<EAdditiveAnimationType>(), AdditiveType.GetValue()));

			EAnimInterpolationType InterpolationType = EAnimInterpolationType::Linear;
			UAnimationBlueprintLibrary::GetAnimationInterpolationType(AnimSequence, InterpolationType);
			SummaryObj->SetStringField(TEXT("interpolation_type"), EnumToString(StaticEnum<EAnimInterpolationType>(), static_cast<int64>(InterpolationType)));

			UAnimCurveCompressionSettings* CurveCompression = nullptr;
			UAnimationBlueprintLibrary::GetCurveCompressionSettings(AnimSequence, CurveCompression);
			SummaryObj->SetStringField(TEXT("curve_compression_settings"), CurveCompression ? CurveCompression->GetPathName() : TEXT(""));

			TArray<TSharedPtr<FJsonValue>> CurveArray;
			if (bIncludeCurves)
			{
				AddFloatCurveObjects(AnimSequence, MaxCurveKeysPreview, CurveArray);
				AddVectorCurveObjects(AnimSequence, MaxCurveKeysPreview, CurveArray);
				AddTransformCurveObjects(AnimSequence, MaxCurveKeysPreview, CurveArray);
			}
			AssetObj->SetArrayField(TEXT("curves"), CurveArray);
		}

		TArray<TSharedPtr<FJsonValue>> NotifyArray;
		if (bIncludeNotifies)
		{
			for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
			{
				NotifyArray.Add(MakeShareable(new FJsonValueObject(BuildNotifyEventObject(NotifyEvent, NotifyTrackNames))));
			}
		}
		AssetObj->SetArrayField(TEXT("notify_events"), NotifyArray);

		return AssetObj;
	}
}

// ============================================================
// Registration
// ============================================================

void FVFXToolRegistry::RegisterAnimationTools()
{
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_animation_editor_context");
		Def.Description = TEXT(
			"Return animation assets currently selected in the Content Browser or open in asset editors, "
			"plus a preferred animation path for follow-up inspect/review tools.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("include_asset_info"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include shallow asset info for returned animation assets.")));
		Props->SetObjectField(TEXT("include_notifies"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include notify details in returned asset info.")));
		Props->SetObjectField(TEXT("include_curves"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default false. Include curve previews in returned asset info.")));
		Props->SetObjectField(TEXT("max_curve_keys_preview"),
			MakeStringProperty(TEXT("Optional integer. Default 5. Max preview keys per curve when include_curves is true.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetAnimationEditorContext);
		RegisterTool(Def);
	}

	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_animation_asset_info");
		Def.Description = TEXT(
			"Inspect an animation asset (AnimSequence, AnimMontage, or other UAnimationAsset) and return notifies, tracks, "
			"curves, sections, slots, metadata, skeleton, and summary stats.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset path of the animation asset to inspect.")));
		Props->SetObjectField(TEXT("include_notifies"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true. Include notify details.")));
		Props->SetObjectField(TEXT("include_curves"),
			MakeStringProperty(TEXT("Optional bool-like flag. Default true for sequences. Include curve previews.")));
		Props->SetObjectField(TEXT("max_curve_keys_preview"),
			MakeStringProperty(TEXT("Optional integer. Default 5. Max preview keys per curve.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetAnimationAssetInfo);
		RegisterTool(Def);
	}

	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_animation_asset");
		Def.Description = TEXT(
			"Review an animation asset and return structured findings about notify coverage, montage setup, curves, root motion, "
			"and potential gameplay/VFX integration risks.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("asset_path"),
			MakeStringProperty(TEXT("Full asset path of the animation asset to review.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("asset_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewAnimationAsset);
		RegisterTool(Def);
	}
}

// ============================================================
// Implementations
// ============================================================

bool FVFXToolRegistry::Tool_GetAnimationEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const bool bIncludeAssetInfo = ParseOptionalBoolField(Input, TEXT("include_asset_info"), true);
	const bool bIncludeNotifies = ParseOptionalBoolField(Input, TEXT("include_notifies"), false);
	const bool bIncludeCurves = ParseOptionalBoolField(Input, TEXT("include_curves"), false);
	const int32 MaxCurveKeysPreview = FMath::Clamp(ParseOptionalIntField(Input, TEXT("max_curve_keys_preview"), 5), 1, 20);

	TArray<UAnimationAsset*> SelectedAnimationAssets;
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FAssetData> SelectedAssets;
		ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);
		for (const FAssetData& AssetData : SelectedAssets)
		{
			if (UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(AssetData.GetAsset()))
			{
				SelectedAnimationAssets.Add(AnimationAsset);
			}
		}
	}

	TArray<UAnimationAsset*> OpenedAnimationAssets;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			TArray<UObject*> EditedAssets = AssetEditorSubsystem->GetAllEditedAssets();
			for (UObject* EditedAsset : EditedAssets)
			{
				if (UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(EditedAsset))
				{
					OpenedAnimationAssets.Add(AnimationAsset);
				}
			}
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_animation_editor_context"));
	Result->SetNumberField(TEXT("selected_count"), SelectedAnimationAssets.Num());
	Result->SetNumberField(TEXT("opened_count"), OpenedAnimationAssets.Num());

	TArray<TSharedPtr<FJsonValue>> SelectedArray;
	for (UAnimationAsset* AnimationAsset : SelectedAnimationAssets)
	{
		if (!AnimationAsset)
		{
			continue;
		}

		TSharedPtr<FJsonObject> EntryObj = MakeShareable(new FJsonObject());
		EntryObj->SetStringField(TEXT("asset_path"), AnimationAsset->GetPathName());
		EntryObj->SetStringField(TEXT("asset_name"), AnimationAsset->GetName());
		EntryObj->SetStringField(TEXT("asset_class"), AnimationAsset->GetClass()->GetName());
		if (bIncludeAssetInfo)
		{
			EntryObj->SetObjectField(TEXT("asset_info"), BuildAnimationAssetInfoObject(AnimationAsset, bIncludeNotifies, bIncludeCurves, MaxCurveKeysPreview));
		}
		SelectedArray.Add(MakeShareable(new FJsonValueObject(EntryObj)));
	}

	TArray<TSharedPtr<FJsonValue>> OpenedArray;
	for (UAnimationAsset* AnimationAsset : OpenedAnimationAssets)
	{
		if (!AnimationAsset)
		{
			continue;
		}

		TSharedPtr<FJsonObject> EntryObj = MakeShareable(new FJsonObject());
		EntryObj->SetStringField(TEXT("asset_path"), AnimationAsset->GetPathName());
		EntryObj->SetStringField(TEXT("asset_name"), AnimationAsset->GetName());
		EntryObj->SetStringField(TEXT("asset_class"), AnimationAsset->GetClass()->GetName());
		if (bIncludeAssetInfo)
		{
			EntryObj->SetObjectField(TEXT("asset_info"), BuildAnimationAssetInfoObject(AnimationAsset, bIncludeNotifies, bIncludeCurves, MaxCurveKeysPreview));
		}
		OpenedArray.Add(MakeShareable(new FJsonValueObject(EntryObj)));
	}

	Result->SetArrayField(TEXT("selected_assets"), SelectedArray);
	Result->SetArrayField(TEXT("opened_assets"), OpenedArray);

	UAnimationAsset* PreferredAsset = SelectedAnimationAssets.Num() > 0 ? SelectedAnimationAssets[0]
		: (OpenedAnimationAssets.Num() > 0 ? OpenedAnimationAssets[0] : nullptr);
	Result->SetStringField(TEXT("preferred_asset_path"), PreferredAsset ? PreferredAsset->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("preferred_source"),
		SelectedAnimationAssets.Num() > 0 ? TEXT("content_browser_selection")
		: (OpenedAnimationAssets.Num() > 0 ? TEXT("open_editor") : TEXT("none")));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetAnimationAssetInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildAnimationToolError(OutResult, TEXT("AN_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		return BuildAnimationToolError(OutResult, TEXT("AN_MISSING_ASSET_PATH"), TEXT("Missing required field: asset_path"));
	}

	const bool bIncludeNotifies = ParseOptionalBoolField(Input, TEXT("include_notifies"), true);
	const bool bIncludeCurves = ParseOptionalBoolField(Input, TEXT("include_curves"), true);
	const int32 MaxCurveKeysPreview = FMath::Clamp(ParseOptionalIntField(Input, TEXT("max_curve_keys_preview"), 5), 1, 20);

	UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(UEditorAssetLibrary::LoadAsset(AssetPath));
	if (!AnimationAsset)
	{
		return BuildAnimationToolError(
			OutResult,
			TEXT("AN_ASSET_NOT_FOUND"),
			FString::Printf(TEXT("Animation asset not found at '%s'."), *AssetPath));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_animation_asset_info"));
	Result->SetObjectField(TEXT("animation_asset"), BuildAnimationAssetInfoObject(AnimationAsset, bIncludeNotifies, bIncludeCurves, MaxCurveKeysPreview));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewAnimationAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildAnimationToolError(OutResult, TEXT("AN_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		return BuildAnimationToolError(OutResult, TEXT("AN_MISSING_ASSET_PATH"), TEXT("Missing required field: asset_path"));
	}

	UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(UEditorAssetLibrary::LoadAsset(AssetPath));
	if (!AnimationAsset)
	{
		return BuildAnimationToolError(
			OutResult,
			TEXT("AN_ASSET_NOT_FOUND"),
			FString::Printf(TEXT("Animation asset not found at '%s'."), *AssetPath));
	}

	TSharedPtr<FJsonObject> AssetInfo = BuildAnimationAssetInfoObject(AnimationAsset, true, true, 3);
	const TSharedPtr<FJsonObject>* SummaryPtr = nullptr;
	const TArray<TSharedPtr<FJsonValue>>* NotifyEventsPtr = nullptr;
	const TArray<TSharedPtr<FJsonValue>>* CurvesPtr = nullptr;
	const TArray<TSharedPtr<FJsonValue>>* SectionsPtr = nullptr;
	const TArray<TSharedPtr<FJsonValue>>* SlotsPtr = nullptr;
	AssetInfo->TryGetObjectField(TEXT("summary"), SummaryPtr);
	AssetInfo->TryGetArrayField(TEXT("notify_events"), NotifyEventsPtr);
	AssetInfo->TryGetArrayField(TEXT("curves"), CurvesPtr);
	AssetInfo->TryGetArrayField(TEXT("montage_sections"), SectionsPtr);
	AssetInfo->TryGetArrayField(TEXT("montage_slots"), SlotsPtr);

	TArray<TSharedPtr<FJsonValue>> Findings;
	auto AddFinding = [&Findings](const FString& Severity, const FString& Finding, const FString& Reason, const FString& SuggestedFix)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("finding"), Finding);
		FindingObj->SetStringField(TEXT("reason"), Reason);
		FindingObj->SetStringField(TEXT("suggested_fix"), SuggestedFix);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	};

	const TSharedPtr<FJsonObject> SummaryObj = SummaryPtr ? *SummaryPtr : MakeShareable(new FJsonObject());
	const bool bIsSequence = SummaryObj->GetBoolField(TEXT("is_sequence"));
	const bool bIsMontage = SummaryObj->GetBoolField(TEXT("is_montage"));
	const float LengthSeconds = SummaryObj->GetNumberField(TEXT("length_seconds"));
	const float RateScale = SummaryObj->GetNumberField(TEXT("rate_scale"));
	const int32 NotifyCount = static_cast<int32>(SummaryObj->GetNumberField(TEXT("notify_count")));
	const int32 TrackCount = static_cast<int32>(SummaryObj->GetNumberField(TEXT("track_count")));
	const int32 FloatCurveCount = static_cast<int32>(SummaryObj->GetNumberField(TEXT("float_curve_count")));
	const int32 VectorCurveCount = static_cast<int32>(SummaryObj->GetNumberField(TEXT("vector_curve_count")));
	const int32 TransformCurveCount = static_cast<int32>(SummaryObj->GetNumberField(TEXT("transform_curve_count")));
	const bool bRootMotionEnabled = SummaryObj->GetBoolField(TEXT("root_motion_enabled"));

	if (AnimationAsset->GetSkeleton() == nullptr)
	{
		AddFinding(
			TEXT("error"),
			TEXT("Missing skeleton reference"),
			TEXT("Animation asset has no skeleton, which breaks retargeting and downstream animation usage."),
			TEXT("Reassign a valid skeleton or repair the source asset import."));
	}

	if (LengthSeconds <= 0.0f)
	{
		AddFinding(
			TEXT("error"),
			TEXT("Zero-length animation"),
			TEXT("Animation reports zero or negative length."),
			TEXT("Reimport or rebuild the asset; verify compression/data model integrity."));
	}

	if (TrackCount == 0 && bIsSequence)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("No animation tracks"),
			TEXT("Sequence has no animation tracks, so it may not visibly move any bones."),
			TEXT("Verify the source animation import and track data."));
	}

	if (bIsMontage)
	{
		const int32 SectionCount = SectionsPtr ? SectionsPtr->Num() : 0;
		const int32 SlotCount = SlotsPtr ? SlotsPtr->Num() : 0;
		if (SectionCount == 0)
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Montage has no sections"),
				TEXT("Montage sections are often needed for gameplay routing and jumps."),
				TEXT("Add at least one named montage section if this asset drives gameplay transitions."));
		}
		if (SlotCount == 0)
		{
			AddFinding(
				TEXT("warning"),
				TEXT("Montage has no slots"),
				TEXT("Montage has no slot mapping, which can prevent expected playback routing."),
				TEXT("Add or verify montage slot tracks."));
		}
	}

	if (NotifyCount == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No notify events"),
			TEXT("Animation has no notify events. That may be fine, but gameplay/VFX sync cannot originate from this asset."),
			TEXT("Add anim notifies if gameplay, sound, or VFX timing should come from the animation itself."));
	}
	else if (NotifyCount > 20)
	{
		AddFinding(
			TEXT("info"),
			TEXT("High notify density"),
			FString::Printf(TEXT("Animation contains %d notify events."), NotifyCount),
			TEXT("Review notify density to keep timing readable and maintainable."));
	}

	if (bIsSequence && (FloatCurveCount + VectorCurveCount + TransformCurveCount) == 0 && NotifyCount == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No curves or notifies"),
			TEXT("Sequence has neither curves nor notify events, so external systems must drive all timing."),
			TEXT("If this animation should coordinate gameplay/VFX, consider adding curves or notifies."));
	}

	if (!FMath::IsNearlyEqual(RateScale, 1.0f))
	{
		AddFinding(
			TEXT("info"),
			TEXT("Non-default rate scale"),
			FString::Printf(TEXT("Animation rate scale is %.3f."), RateScale),
			TEXT("Verify downstream timing assumptions if gameplay or VFX are keyed to this asset."));
	}

	if (bRootMotionEnabled)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Root motion enabled"),
			TEXT("Sequence uses root motion, which affects how gameplay code and montage logic should consume it."),
			TEXT("Confirm the consuming character or ability flow expects root motion from this asset."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious issues"),
			TEXT("No high-signal automated issues were detected in this animation asset."),
			TEXT("Do a runtime timing pass in PIE if this asset is tied to gameplay, VFX, or montage state changes."));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_animation_asset"));
	Result->SetStringField(TEXT("asset_path"), AssetPath);
	Result->SetStringField(TEXT("asset_class"), AnimationAsset->GetClass()->GetName());
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetObjectField(TEXT("animation_asset"), AssetInfo);
	OutResult = JsonObjToString(Result);
	return true;
}
