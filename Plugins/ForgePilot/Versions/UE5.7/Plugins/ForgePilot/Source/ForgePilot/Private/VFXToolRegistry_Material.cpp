// Copyright 2026, HenryFX. ForgePilot Plugin.
// Phase 3 + 3.5: Material Tools - Registration + Implementation (with connection validation)

#include "VFXToolRegistry.h"
#include "VFXMutationAudit.h"
#include "VFXToolRegistryHelpers.h"

#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionAdd.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialExpressionTextureObject.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionConstant2Vector.h"
#include "Materials/MaterialExpressionFresnel.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionFunctionInput.h"
#include "Materials/MaterialExpressionFunctionOutput.h"
#include "Materials/MaterialExpressionComment.h"
#include "Materials/MaterialExpressionPanner.h"
#include "Materials/MaterialExpressionTime.h"
#include "Materials/MaterialExpressionClamp.h"
#include "Materials/MaterialExpressionOneMinus.h"
#include "Materials/MaterialExpressionPower.h"
#include "Materials/MaterialExpressionSine.h"
#include "Materials/MaterialExpressionDivide.h"
#include "Materials/MaterialExpressionAbs.h"
#include "Materials/MaterialExpressionCeil.h"
#include "Materials/MaterialExpressionCosine.h"
#include "Materials/MaterialExpressionCrossProduct.h"
#include "Materials/MaterialExpressionFmod.h"
#include "Materials/MaterialExpressionFloor.h"
#include "Materials/MaterialExpressionFrac.h"
#include "Materials/MaterialExpressionIf.h"
#include "Materials/MaterialExpressionInverseLinearInterpolate.h"
#include "Materials/MaterialExpressionLength.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionDistance.h"
#include "Materials/MaterialExpressionMin.h"
#include "Materials/MaterialExpressionMax.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionReflectionVectorWS.h"
#include "Materials/MaterialExpressionRound.h"
#include "Materials/MaterialExpressionSaturate.h"
#include "Materials/MaterialExpressionSign.h"
#include "Materials/MaterialExpressionSmoothStep.h"
#include "Materials/MaterialExpressionSquareRoot.h"
#include "Materials/MaterialExpressionStep.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionArccosine.h"
#include "Materials/MaterialExpressionArcsine.h"
#include "Materials/MaterialExpressionArctangent.h"
#include "Materials/MaterialExpressionArctangent2.h"
#include "Materials/MaterialExpressionDotProduct.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include "Materials/MaterialExpressionObjectPositionWS.h"
#include "Materials/MaterialExpressionObjectRadius.h"
#include "Materials/MaterialExpressionCameraVectorWS.h"
#include "Materials/MaterialExpressionDynamicParameter.h"
#include "Materials/MaterialExpressionNoise.h"
#include "Materials/MaterialExpressionTangent.h"
#include "Materials/MaterialExpressionBlackBody.h"
#include "Materials/MaterialExpressionDesaturation.h"
#include "Materials/MaterialExpressionParticleSubUV.h"
#include "Materials/MaterialExpressionSceneDepth.h"
#include "Materials/MaterialExpressionPixelDepth.h"
#include "Materials/MaterialExpressionDistanceToNearestSurface.h"
#include "Materials/MaterialExpressionFeatureLevelSwitch.h"
#include "Materials/MaterialExpressionStaticSwitchParameter.h"
#include "Materials/MaterialExpressionPreSkinnedPosition.h"
#include "Materials/MaterialExpressionTransform.h"
#include "Materials/MaterialExpressionTransformPosition.h"
#include "Materials/MaterialExpressionVertexNormalWS.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialFunction.h"
#include "Materials/MaterialFunctionInterface.h"
#include "Materials/MaterialFunctionInstance.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
// Note: MaterialExpressionParticleColor is resolved dynamically via FindObject to avoid linker issues
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ObjectTools.h"
#include "Misc/ObjectThumbnail.h"
#include "Engine/World.h"
#include "Engine/Selection.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPluginManager.h"
#include "IAssetViewport.h"
#include "LevelEditor.h"
#include "PackageTools.h"
#include "SLevelViewport.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "UObject/UnrealType.h"
#include "AssetCompilingManager.h"
#include "HAL/PlatformProcess.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/Base64.h"
#include "ContentStreaming.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/SWindow.h"
#include "Rendering/DrawElements.h"
#include "Math/Float16.h"
#include "Styling/AppStyle.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "MaterialEditorUtilities.h"
#include "IMaterialEditor.h"
#include "CanvasTypes.h"
#include "SceneView.h"
#include "RHIUtilities.h"
#include "ThumbnailHelpers.h"
#include "ThumbnailRendering/SceneThumbnailInfoWithPrimitive.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MaterialEditingLibrary.h"
#include "MaterialShared.h"
#include "RHIStrings.h"

namespace
{
	class FForgePilotThumbnailRendererAccess : public UThumbnailRenderer
	{
	public:
		static FGameTime GetThumbnailTime()
		{
			return GetTime();
		}

		static void RenderThumbnailViewFamily(FCanvas* Canvas, FSceneViewFamily* ViewFamily, FSceneView* View)
		{
			RenderViewFamily(Canvas, ViewFamily, View);
		}
	};

	static FString GetAssetPackageFilename(UObject* Asset)
	{
		if (!Asset)
		{
			return FString();
		}

		return FPackageName::LongPackageNameToFilename(
			Asset->GetOutermost()->GetName(),
			FPackageName::GetAssetPackageExtension());
	}

	static void AppendMaterialMutationAudit(
		TSharedPtr<FJsonObject> Result,
		const FString& ToolName,
		UObject* Asset,
		const FString& Message,
		const TSharedPtr<FJsonObject>& Details = nullptr,
		const FString& SnapshotPath = TEXT(""),
		const bool bSucceeded = true)
	{
		if (!Result.IsValid() || !Asset)
		{
			return;
		}

		FString AuditEntryPath;
		const bool bRecorded = FVFXMutationAudit::RecordAssetMutation(
			ToolName,
			Asset->GetPathName(),
			Asset->GetClass()->GetName(),
			ToolName,
			SnapshotPath,
			GetAssetPackageFilename(Asset),
			bSucceeded,
			Message,
			AuditEntryPath,
			Details);

		Result->SetBoolField(TEXT("audit_recorded"), bRecorded);
		Result->SetStringField(TEXT("audit_entry_path"), AuditEntryPath);
	}

	static FString SanitizeMaterialMutationToken(FString Value)
	{
		Value = Value.TrimStartAndEnd();
		for (TCHAR& Character : Value)
		{
			if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
			{
				Character = TEXT('_');
			}
		}

		return Value.IsEmpty() ? TEXT("material") : Value;
	}

	static FString GetMaterialMutationSnapshotRoot()
	{
		return FPaths::Combine(GetForgePilotDownloadsRoot(), TEXT("MaterialMutationSnapshots"));
	}

	static bool IsMaterialMutationAsset(UObject* Asset)
	{
		return Asset &&
			(Asset->IsA<UMaterial>() ||
			 Asset->IsA<UMaterialInstanceConstant>() ||
			 Asset->IsA<UMaterialFunctionInterface>());
	}

	static bool CreateMaterialMutationSnapshot(UObject* Asset, const FString& MutationAction, FString& OutSnapshotPath, FString& OutError)
	{
		if (!IsMaterialMutationAsset(Asset))
		{
			OutError = TEXT("Asset is not a Material / Material Instance / Material Function asset.");
			return false;
		}

		const FString SnapshotRoot = GetMaterialMutationSnapshotRoot();
		if (SnapshotRoot.IsEmpty())
		{
			OutError = TEXT("Could not resolve ForgePilot Material snapshot directory.");
			return false;
		}

		const FString PackageFilename = GetAssetPackageFilename(Asset);
		if (!FPaths::FileExists(PackageFilename))
		{
			OutError = FString::Printf(TEXT("Material package file does not exist on disk: %s"), *PackageFilename);
			return false;
		}

		const FString AssetToken = SanitizeMaterialMutationToken(Asset->GetName());
		const FString ActionToken = SanitizeMaterialMutationToken(MutationAction);
		const FString SnapshotDir = FPaths::Combine(SnapshotRoot, AssetToken);
		IFileManager::Get().MakeDirectory(*SnapshotDir, true);

		const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
		OutSnapshotPath = FPaths::Combine(
			SnapshotDir,
			FString::Printf(TEXT("%s_%s_%s%s"),
				*AssetToken,
				*ActionToken,
				*Timestamp,
				*FPackageName::GetAssetPackageExtension()));

		const uint32 CopyResult = IFileManager::Get().Copy(*OutSnapshotPath, *PackageFilename, true, true);
		if (CopyResult != COPY_OK)
		{
			OutError = FString::Printf(TEXT("Failed to create Material snapshot (copy code %u)."), CopyResult);
			OutSnapshotPath.Reset();
			return false;
		}

		return true;
	}

	static void GetMaterialSnapshotFiles(UObject* Asset, TArray<FString>& OutSnapshotFiles)
	{
		OutSnapshotFiles.Reset();
		if (!IsMaterialMutationAsset(Asset))
		{
			return;
		}

		const FString SnapshotRoot = GetMaterialMutationSnapshotRoot();
		if (SnapshotRoot.IsEmpty())
		{
			return;
		}

		const FString SnapshotDir = FPaths::Combine(SnapshotRoot, SanitizeMaterialMutationToken(Asset->GetName()));
		if (!IFileManager::Get().DirectoryExists(*SnapshotDir))
		{
			return;
		}

		IFileManager::Get().FindFiles(OutSnapshotFiles, *FPaths::Combine(SnapshotDir, TEXT("*.uasset")), true, false);
		for (FString& FileName : OutSnapshotFiles)
		{
			FileName = FPaths::Combine(SnapshotDir, FileName);
		}

		OutSnapshotFiles.Sort([](const FString& Left, const FString& Right)
		{
			return IFileManager::Get().GetTimeStamp(*Left) > IFileManager::Get().GetTimeStamp(*Right);
		});
	}

	static bool IsMaterialSnapshotPathUnderRoot(const FString& SnapshotPath)
	{
		const FString SnapshotRoot = FPaths::ConvertRelativePathToFull(GetMaterialMutationSnapshotRoot());
		const FString FullSnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
		return !SnapshotRoot.IsEmpty() && FullSnapshotPath.StartsWith(SnapshotRoot, ESearchCase::IgnoreCase);
	}

	static TSharedPtr<FJsonObject> BuildMaterialMutationPatchReport(
		const FString& Action,
		UObject* Asset,
		const FString& SnapshotPath,
		const bool bSaveSucceeded,
		const TSharedPtr<FJsonObject>& Details = nullptr)
	{
		TSharedPtr<FJsonObject> PatchReport = MakeShareable(new FJsonObject());
		PatchReport->SetStringField(TEXT("action"), Action);
		PatchReport->SetStringField(TEXT("asset_path"), Asset ? Asset->GetPathName() : TEXT(""));
		PatchReport->SetStringField(TEXT("asset_class"), Asset ? Asset->GetClass()->GetName() : TEXT(""));
		PatchReport->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		PatchReport->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
		if (Details.IsValid())
		{
			PatchReport->SetObjectField(TEXT("details"), Details);
		}
		return PatchReport;
	}

	static bool SaveMaterialMutationAsset(UObject* Asset, FString& OutError)
	{
		if (!IsMaterialMutationAsset(Asset))
		{
			OutError = TEXT("Asset is not a Material / Material Instance / Material Function asset.");
			return false;
		}

		UPackage* Package = Asset->GetOutermost();
		if (!Package)
		{
			OutError = TEXT("Asset package is null.");
			return false;
		}

		const FString PackageName = Package->GetName();
		if (PackageName.IsEmpty())
		{
			OutError = TEXT("Asset package name is empty.");
			return false;
		}

		const FString PackageFileName = FPackageName::LongPackageNameToFilename(
			PackageName,
			FPackageName::GetAssetPackageExtension());
		if (PackageFileName.IsEmpty())
		{
			OutError = FString::Printf(TEXT("Could not resolve package filename for '%s'."), *PackageName);
			return false;
		}

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		if (!UPackage::SavePackage(Package, Asset, *PackageFileName, SaveArgs))
		{
			OutError = FString::Printf(TEXT("Failed to save package '%s'."), *PackageFileName);
			return false;
		}

		return true;
	}

	static bool BuildMaterialToolError(
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

	static bool ParseToolJsonResult(const FString& Json, TSharedPtr<FJsonObject>& OutObject)
	{
		OutObject.Reset();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
		return FJsonSerializer::Deserialize(Reader, OutObject) && OutObject.IsValid();
	}

	static bool IsMaterialCustomHlslExplicitlyAllowed(const TSharedPtr<FJsonObject>& Input)
	{
		if (!Input.IsValid())
		{
			return false;
		}

		bool bAllowed = false;
		if (Input->TryGetBoolField(TEXT("allow_custom_hlsl"), bAllowed))
		{
			return bAllowed;
		}

		FString BoolText;
		if (Input->TryGetStringField(TEXT("allow_custom_hlsl"), BoolText))
		{
			return BoolText.Equals(TEXT("true"), ESearchCase::IgnoreCase) ||
				BoolText.Equals(TEXT("1"), ESearchCase::IgnoreCase) ||
				BoolText.Equals(TEXT("yes"), ESearchCase::IgnoreCase);
		}

		return false;
	}

	static FString GetMaterialExpressionCaptionText(UMaterialExpression* Expression);

	static FString GetMaterialExpressionPracticalName(UMaterialExpression* Expression)
	{
		if (!Expression)
		{
			return TEXT("Unknown");
		}

		if (const UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expression))
		{
			return FString::Printf(TEXT("ScalarParameter(%s)"), *ScalarParam->ParameterName.ToString());
		}

		if (const UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expression))
		{
			return FString::Printf(TEXT("VectorParameter(%s)"), *VectorParam->ParameterName.ToString());
		}

		if (const UMaterialExpressionTextureSampleParameter2D* TextureParam = Cast<UMaterialExpressionTextureSampleParameter2D>(Expression))
		{
			return FString::Printf(TEXT("TextureSampleParameter2D(%s)"), *TextureParam->ParameterName.ToString());
		}

		if (const UMaterialExpressionTextureSample* TextureSample = Cast<UMaterialExpressionTextureSample>(Expression))
		{
			return TextureSample->Texture
				? FString::Printf(TEXT("TextureSample(%s)"), *TextureSample->Texture->GetName())
				: TEXT("TextureSample");
		}

		if (const UMaterialExpressionTextureCoordinate* TexCoord = Cast<UMaterialExpressionTextureCoordinate>(Expression))
		{
			return FString::Printf(TEXT("TexCoord[%d]"), TexCoord->CoordinateIndex);
		}

		if (const UMaterialExpressionPanner* Panner = Cast<UMaterialExpressionPanner>(Expression))
		{
			return FString::Printf(TEXT("Panner(%.3g, %.3g)"), Panner->SpeedX, Panner->SpeedY);
		}

		if (const UMaterialExpressionPower* Power = Cast<UMaterialExpressionPower>(Expression))
		{
			return FString::Printf(TEXT("Power(exp=%.3g)"), Power->ConstExponent);
		}

		if (Expression->IsA<UMaterialExpressionSine>())
		{
			return TEXT("Sine");
		}

		if (Expression->IsA<UMaterialExpressionFloor>())
		{
			return TEXT("Floor");
		}

		if (const UMaterialExpressionConstant* Constant = Cast<UMaterialExpressionConstant>(Expression))
		{
			return FString::Printf(TEXT("Constant(%.3g)"), Constant->R);
		}

		if (const UMaterialExpressionComponentMask* ComponentMask = Cast<UMaterialExpressionComponentMask>(Expression))
		{
			FString Mask;
			if (ComponentMask->R) Mask += TEXT("R");
			if (ComponentMask->G) Mask += TEXT("G");
			if (ComponentMask->B) Mask += TEXT("B");
			if (ComponentMask->A) Mask += TEXT("A");
			return FString::Printf(TEXT("ComponentMask(%s)"), Mask.IsEmpty() ? TEXT("None") : *Mask);
		}

		return GetMaterialExpressionCaptionText(Expression);
	}

	static FString GetMaterialMaskLabel(const uint32 Mask)
	{
		FString Label;
		if (Mask & 1) Label += TEXT("R");
		if (Mask & 2) Label += TEXT("G");
		if (Mask & 4) Label += TEXT("B");
		if (Mask & 8) Label += TEXT("A");
		return Label.IsEmpty() ? TEXT("RGBA") : Label;
	}

	static int32 CountMaterialMaskChannels(const uint32 Mask)
	{
		const int32 ChannelCount =
			(Mask & 1 ? 1 : 0) +
			(Mask & 2 ? 1 : 0) +
			(Mask & 4 ? 1 : 0) +
			(Mask & 8 ? 1 : 0);
		return ChannelCount > 0 ? ChannelCount : 4;
	}

	static FString BuildMaterialConnectionSummary(
		UMaterialExpression* SourceExpr,
		const FString& SourceOutputName,
		const FString& SourceMaskLabel,
		const FString& TargetLabel,
		const FString& TargetInputName)
	{
		const FString SourceLabel = GetMaterialExpressionPracticalName(SourceExpr);
		const FString OutputLabel = SourceOutputName.IsEmpty() ? TEXT("Output") : SourceOutputName;
		if (SourceMaskLabel.IsEmpty() || SourceMaskLabel.Equals(TEXT("RGBA"), ESearchCase::IgnoreCase))
		{
			return FString::Printf(TEXT("%s %s -> %s %s"), *SourceLabel, *OutputLabel, *TargetLabel, *TargetInputName);
		}

		return FString::Printf(TEXT("%s %s (%s) -> %s %s"), *SourceLabel, *OutputLabel, *SourceMaskLabel, *TargetLabel, *TargetInputName);
	}

	static FString EnumValueToStringSafe(const UEnum* EnumType, int64 RawValue)
	{
		if (!EnumType)
		{
			return FString::FromInt(static_cast<int32>(RawValue));
		}
		return EnumType->GetNameStringByValue(RawValue);
	}

	static TSharedPtr<FJsonObject> BuildTextureInfoJson(UTexture* Texture, bool bIncludeReferencers)
	{
		TSharedPtr<FJsonObject> TextureObj = MakeShareable(new FJsonObject());
		if (!Texture)
		{
			return TextureObj;
		}

		TextureObj->SetStringField(TEXT("name"), Texture->GetName());
		TextureObj->SetStringField(TEXT("path"), Texture->GetPathName());
		TextureObj->SetStringField(TEXT("class"), Texture->GetClass()->GetName());
		TextureObj->SetBoolField(TEXT("srgb"), Texture->SRGB);
		TextureObj->SetStringField(
			TEXT("compression_settings"),
			EnumValueToStringSafe(StaticEnum<TextureCompressionSettings>(), static_cast<int64>(Texture->CompressionSettings)));
		TextureObj->SetStringField(
			TEXT("lod_group"),
			EnumValueToStringSafe(StaticEnum<TextureGroup>(), static_cast<int64>(Texture->LODGroup)));
		TextureObj->SetStringField(
			TEXT("mip_gen_settings"),
			EnumValueToStringSafe(StaticEnum<TextureMipGenSettings>(), static_cast<int64>(Texture->MipGenSettings)));
		TextureObj->SetStringField(
			TEXT("filter"),
			EnumValueToStringSafe(StaticEnum<TextureFilter>(), static_cast<int64>(Texture->Filter)));
		TextureObj->SetBoolField(TEXT("never_stream"), Texture->NeverStream);
		TextureObj->SetBoolField(TEXT("virtual_texture_streaming"), Texture->VirtualTextureStreaming);

		if (UTexture2D* Texture2D = Cast<UTexture2D>(Texture))
		{
			TextureObj->SetNumberField(TEXT("size_x"), Texture2D->GetSizeX());
			TextureObj->SetNumberField(TEXT("size_y"), Texture2D->GetSizeY());
			TextureObj->SetNumberField(TEXT("num_mips"), Texture2D->GetNumMips());
			TextureObj->SetBoolField(TEXT("is_power_of_two"), FMath::IsPowerOfTwo(Texture2D->GetSizeX()) && FMath::IsPowerOfTwo(Texture2D->GetSizeY()));
			TextureObj->SetBoolField(TEXT("is_square"), Texture2D->GetSizeX() == Texture2D->GetSizeY());
			if (Texture2D->GetSizeY() > 0)
			{
				TextureObj->SetNumberField(TEXT("aspect_ratio"), static_cast<double>(Texture2D->GetSizeX()) / static_cast<double>(Texture2D->GetSizeY()));
			}

#if WITH_EDITORONLY_DATA
			TextureObj->SetNumberField(TEXT("source_size_x"), Texture2D->Source.GetSizeX());
			TextureObj->SetNumberField(TEXT("source_size_y"), Texture2D->Source.GetSizeY());
			TextureObj->SetNumberField(TEXT("source_num_layers"), Texture2D->Source.GetNumLayers());
			TextureObj->SetNumberField(TEXT("source_format"), static_cast<int32>(Texture2D->Source.GetFormat()));
#endif
		}

		if (bIncludeReferencers)
		{
			TArray<FString> Referencers = UEditorAssetLibrary::FindPackageReferencersForAsset(Texture->GetPathName(), false);
			TArray<TSharedPtr<FJsonValue>> RefArray;
			for (const FString& RefPath : Referencers)
			{
				RefArray.Add(MakeShareable(new FJsonValueString(RefPath)));
			}
			TextureObj->SetArrayField(TEXT("referenced_by"), RefArray);
			TextureObj->SetNumberField(TEXT("referencer_count"), RefArray.Num());
		}

		return TextureObj;
	}

	static FString GetDefaultTexturePreviewOutputDir(const FString& TextureName)
	{
		const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
		return FPaths::Combine(
			GetForgePilotUserDownloadsDir(),
			TEXT("ForgePilotTextureInspections"),
			FString::Printf(TEXT("texture_inspection_%s_%s"), *SanitizeMaterialMutationToken(TextureName), *Timestamp));
	}

	static bool EncodeColorBufferToPngBytes(const TArray<FColor>& Bitmap, const int32 Width, const int32 Height, TArray64<uint8>& OutPngBytes, FString& OutError)
	{
		OutPngBytes.Reset();
		if (Bitmap.Num() < Width * Height || Width <= 0 || Height <= 0)
		{
			OutError = TEXT("Invalid image buffer.");
			return false;
		}

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		if (!ImageWrapper.IsValid())
		{
			OutError = TEXT("Failed to create PNG image wrapper.");
			return false;
		}

		TArray<uint8> RawData;
		RawData.SetNum(Bitmap.Num() * 4);
		for (int32 PixelIndex = 0; PixelIndex < Bitmap.Num(); ++PixelIndex)
		{
			RawData[PixelIndex * 4 + 0] = Bitmap[PixelIndex].B;
			RawData[PixelIndex * 4 + 1] = Bitmap[PixelIndex].G;
			RawData[PixelIndex * 4 + 2] = Bitmap[PixelIndex].R;
			RawData[PixelIndex * 4 + 3] = Bitmap[PixelIndex].A;
		}

		if (!ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), Width, Height, ERGBFormat::BGRA, 8))
		{
			OutError = TEXT("Failed to convert raw image buffer to PNG.");
			return false;
		}

		OutPngBytes = ImageWrapper->GetCompressed(100);
		if (OutPngBytes.Num() == 0)
		{
			OutError = TEXT("Failed to compress PNG image.");
			return false;
		}

		return true;
	}

	static void ResampleColorBufferToFit(
		const TArray<FColor>& InBitmap,
		const int32 InWidth,
		const int32 InHeight,
		const int32 MaxDimension,
		TArray<FColor>& OutBitmap,
		int32& OutWidth,
		int32& OutHeight)
	{
		OutBitmap.Reset();
		OutWidth = 0;
		OutHeight = 0;
		if (InBitmap.Num() == 0 || InWidth <= 0 || InHeight <= 0 || MaxDimension <= 0)
		{
			return;
		}

		const int32 LargestDimension = FMath::Max(InWidth, InHeight);
		const double Scale = LargestDimension > MaxDimension ? static_cast<double>(MaxDimension) / static_cast<double>(LargestDimension) : 1.0;
		OutWidth = FMath::Max(1, FMath::RoundToInt(InWidth * Scale));
		OutHeight = FMath::Max(1, FMath::RoundToInt(InHeight * Scale));
		OutBitmap.SetNum(OutWidth * OutHeight);

		for (int32 Y = 0; Y < OutHeight; ++Y)
		{
			const int32 SourceY = FMath::Clamp(FMath::FloorToInt((static_cast<double>(Y) / static_cast<double>(OutHeight)) * InHeight), 0, InHeight - 1);
			for (int32 X = 0; X < OutWidth; ++X)
			{
				const int32 SourceX = FMath::Clamp(FMath::FloorToInt((static_cast<double>(X) / static_cast<double>(OutWidth)) * InWidth), 0, InWidth - 1);
				OutBitmap[Y * OutWidth + X] = InBitmap[SourceY * InWidth + SourceX];
			}
		}
	}

	static bool TryLoadTextureSourceBitmap(UTexture2D* Texture2D, TArray<FColor>& OutBitmap, int32& OutWidth, int32& OutHeight, FString& OutError)
	{
		OutBitmap.Reset();
		OutWidth = 0;
		OutHeight = 0;
		OutError.Reset();

#if WITH_EDITORONLY_DATA
		if (!Texture2D || !Texture2D->Source.IsValid())
		{
			OutError = TEXT("Texture source data is not available.");
			return false;
		}

		OutWidth = Texture2D->Source.GetSizeX();
		OutHeight = Texture2D->Source.GetSizeY();
		if (OutWidth <= 0 || OutHeight <= 0)
		{
			OutError = TEXT("Texture source dimensions are invalid.");
			return false;
		}

		TArray64<uint8> MipData;
		if (!Texture2D->Source.GetMipData(MipData, 0) || MipData.Num() == 0)
		{
			OutError = TEXT("Failed to read texture source mip data.");
			return false;
		}

		const ETextureSourceFormat SourceFormat = Texture2D->Source.GetFormat();
		const int32 PixelCount = OutWidth * OutHeight;
		OutBitmap.SetNum(PixelCount);

		auto ClampToByte = [](double Value) -> uint8
		{
			return static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Value), 0, 255));
		};

		switch (SourceFormat)
		{
		case TSF_BGRA8:
		case TSF_BGRE8:
		{
			const int64 RequiredBytes = static_cast<int64>(PixelCount) * 4;
			if (MipData.Num() < RequiredBytes)
			{
				OutError = TEXT("Texture source buffer is smaller than expected for BGRA8/BGRE8.");
				return false;
			}
			FMemory::Memcpy(OutBitmap.GetData(), MipData.GetData(), RequiredBytes);
			if (SourceFormat == TSF_BGRE8)
			{
				for (FColor& Pixel : OutBitmap)
				{
					Pixel.A = 255;
				}
			}
			return true;
		}
		case TSF_G8:
		{
			if (MipData.Num() < PixelCount)
			{
				OutError = TEXT("Texture source buffer is smaller than expected for G8.");
				return false;
			}
			for (int32 PixelIndex = 0; PixelIndex < PixelCount; ++PixelIndex)
			{
				const uint8 Gray = MipData[PixelIndex];
				OutBitmap[PixelIndex] = FColor(Gray, Gray, Gray, 255);
			}
			return true;
		}
		case TSF_G16:
		{
			const int64 RequiredBytes = static_cast<int64>(PixelCount) * 2;
			if (MipData.Num() < RequiredBytes)
			{
				OutError = TEXT("Texture source buffer is smaller than expected for G16.");
				return false;
			}
			const uint16* SourcePtr = reinterpret_cast<const uint16*>(MipData.GetData());
			for (int32 PixelIndex = 0; PixelIndex < PixelCount; ++PixelIndex)
			{
				const uint8 Gray = ClampToByte((static_cast<double>(SourcePtr[PixelIndex]) / 65535.0) * 255.0);
				OutBitmap[PixelIndex] = FColor(Gray, Gray, Gray, 255);
			}
			return true;
		}
		case TSF_RGBA16:
		{
			const int64 RequiredBytes = static_cast<int64>(PixelCount) * 8;
			if (MipData.Num() < RequiredBytes)
			{
				OutError = TEXT("Texture source buffer is smaller than expected for RGBA16.");
				return false;
			}
			const uint16* SourcePtr = reinterpret_cast<const uint16*>(MipData.GetData());
			for (int32 PixelIndex = 0; PixelIndex < PixelCount; ++PixelIndex)
			{
				const int32 BaseIndex = PixelIndex * 4;
				OutBitmap[PixelIndex] = FColor(
					ClampToByte((static_cast<double>(SourcePtr[BaseIndex + 2]) / 65535.0) * 255.0),
					ClampToByte((static_cast<double>(SourcePtr[BaseIndex + 1]) / 65535.0) * 255.0),
					ClampToByte((static_cast<double>(SourcePtr[BaseIndex + 0]) / 65535.0) * 255.0),
					ClampToByte((static_cast<double>(SourcePtr[BaseIndex + 3]) / 65535.0) * 255.0));
			}
			return true;
		}
		case TSF_RGBA16F:
		{
			const int64 RequiredBytes = static_cast<int64>(PixelCount) * 8;
			if (MipData.Num() < RequiredBytes)
			{
				OutError = TEXT("Texture source buffer is smaller than expected for RGBA16F.");
				return false;
			}
			const FFloat16* SourcePtr = reinterpret_cast<const FFloat16*>(MipData.GetData());
			for (int32 PixelIndex = 0; PixelIndex < PixelCount; ++PixelIndex)
			{
				const int32 BaseIndex = PixelIndex * 4;
				OutBitmap[PixelIndex] = FColor(
					ClampToByte(FMath::Clamp(SourcePtr[BaseIndex + 0].GetFloat(), 0.0f, 1.0f) * 255.0),
					ClampToByte(FMath::Clamp(SourcePtr[BaseIndex + 1].GetFloat(), 0.0f, 1.0f) * 255.0),
					ClampToByte(FMath::Clamp(SourcePtr[BaseIndex + 2].GetFloat(), 0.0f, 1.0f) * 255.0),
					ClampToByte(FMath::Clamp(SourcePtr[BaseIndex + 3].GetFloat(), 0.0f, 1.0f) * 255.0));
			}
			return true;
		}
		default:
			OutError = FString::Printf(TEXT("Unsupported texture source format for direct pixel preview: %d"), static_cast<int32>(SourceFormat));
			return false;
		}
#else
		OutError = TEXT("Texture source pixel access requires editor-only data.");
		return false;
#endif
	}

	static bool RenderTexturePreviewBitmap(UTexture* Texture, const int32 PreviewSize, TArray<FColor>& OutBitmap, int32& OutWidth, int32& OutHeight, FString& OutError, FString& OutCaptureSource)
	{
		OutBitmap.Reset();
		OutWidth = 0;
		OutHeight = 0;
		OutError.Reset();
		OutCaptureSource = TEXT("unknown");

		if (!Texture)
		{
			OutError = TEXT("Texture is null.");
			return false;
		}

		if (UTexture2D* Texture2D = Cast<UTexture2D>(Texture))
		{
			TArray<FColor> SourceBitmap;
			int32 SourceWidth = 0;
			int32 SourceHeight = 0;
			FString SourceError;
			if (TryLoadTextureSourceBitmap(Texture2D, SourceBitmap, SourceWidth, SourceHeight, SourceError))
			{
				ResampleColorBufferToFit(SourceBitmap, SourceWidth, SourceHeight, PreviewSize, OutBitmap, OutWidth, OutHeight);
				if (OutBitmap.Num() > 0)
				{
					OutCaptureSource = TEXT("texture_source_pixels");
					return true;
				}
			}
		}

		FObjectThumbnail Thumbnail;
		ThumbnailTools::RenderThumbnail(Texture, PreviewSize, PreviewSize, ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush, nullptr, &Thumbnail);
		const TArray<uint8>& ThumbnailData = Thumbnail.GetUncompressedImageData();
		if (ThumbnailData.Num() == 0 || Thumbnail.GetImageWidth() <= 0 || Thumbnail.GetImageHeight() <= 0)
		{
			OutError = TEXT("Texture thumbnail renderer produced no image data.");
			return false;
		}

		OutWidth = Thumbnail.GetImageWidth();
		OutHeight = Thumbnail.GetImageHeight();
		OutBitmap.SetNum(OutWidth * OutHeight);
		FMemory::Memcpy(OutBitmap.GetData(), ThumbnailData.GetData(), FMath::Min(ThumbnailData.Num(), OutBitmap.Num() * static_cast<int32>(sizeof(FColor))));
		OutCaptureSource = TEXT("asset_thumbnail_fallback");
		return true;
	}

	static bool RenderStaticMeshMaterialPreviewBitmap(
		UStaticMesh* StaticMesh,
		const TArray<UMaterialInterface*>& OverrideMaterials,
		const int32 PreviewSize,
		TArray<FColor>& OutBitmap,
		int32& OutWidth,
		int32& OutHeight,
		FString& OutError,
		FString& OutCaptureSource)
	{
		OutBitmap.Reset();
		OutWidth = 0;
		OutHeight = 0;
		OutError.Reset();
		OutCaptureSource = TEXT("unknown");

		if (!StaticMesh)
		{
			OutError = TEXT("Static mesh is null.");
			return false;
		}
		if (!GEditor)
		{
			OutError = TEXT("Editor context is unavailable for static mesh preview rendering.");
			return false;
		}

		const uint32 TargetSize = static_cast<uint32>(FMath::Max(PreviewSize, 64));
		UTextureRenderTarget2D* RenderTargetTexture = GEditor->GetScratchRenderTarget(TargetSize);
		if (!RenderTargetTexture)
		{
			OutError = TEXT("Failed to allocate scratch render target for static mesh preview.");
			return false;
		}

		FTextureRenderTargetResource* RenderTargetResource = RenderTargetTexture->GameThread_GetRenderTargetResource();
		if (!RenderTargetResource)
		{
			OutError = TEXT("Scratch render target has no render target resource.");
			return false;
		}

		FlushAsyncLoading();
		FAssetCompilingManager::Get().FinishAllCompilation();
		UTexture::ForceUpdateTextureStreaming();
		IStreamingManager::Get().StreamAllResources();

		FStaticMeshThumbnailScene ThumbnailScene;
		ThumbnailScene.SetStaticMesh(StaticMesh);
		ThumbnailScene.SetOverrideMaterials(OverrideMaterials);
		ThumbnailScene.GetScene()->UpdateSpeedTreeWind(0.0f);

		UWorld* PreviewWorld = GetForgePilotEditorWorld();
		const ERHIFeatureLevel::Type FeatureLevel = PreviewWorld ? PreviewWorld->GetFeatureLevel() : ERHIFeatureLevel::SM5;
		FCanvas Canvas(RenderTargetResource, nullptr, FForgePilotThumbnailRendererAccess::GetThumbnailTime(), FeatureLevel);
		Canvas.Clear(FLinearColor(0.20f, 0.22f, 0.25f, 1.0f));

		FSceneViewFamilyContext ViewFamily(
			FSceneViewFamily::ConstructionValues(RenderTargetResource, ThumbnailScene.GetScene(), FEngineShowFlags(ESFIM_Game))
			.SetTime(FForgePilotThumbnailRendererAccess::GetThumbnailTime())
			.SetAdditionalViewFamily(false));

		ViewFamily.EngineShowFlags.DisableAdvancedFeatures();
		ViewFamily.EngineShowFlags.SetSeparateTranslucency(true);
		ViewFamily.EngineShowFlags.MotionBlur = 0;
		ViewFamily.EngineShowFlags.AntiAliasing = 0;

		FForgePilotThumbnailRendererAccess::RenderThumbnailViewFamily(
			&Canvas,
			&ViewFamily,
			ThumbnailScene.CreateView(&ViewFamily, 0, 0, TargetSize, TargetSize));

		ThumbnailScene.SetOverrideMaterials(TArray<UMaterialInterface*>());
		ThumbnailScene.SetStaticMesh(nullptr);

		Canvas.Flush_GameThread();

		ENQUEUE_RENDER_COMMAND(UpdateProbePreviewRTCommand)(
			[RenderTargetResource](FRHICommandListImmediate& RHICmdList)
			{
				TransitionAndCopyTexture(RHICmdList, RenderTargetResource->GetRenderTargetTexture(), RenderTargetResource->TextureRHI, {});
			});

		FlushRenderingCommands();

		OutWidth = static_cast<int32>(TargetSize);
		OutHeight = static_cast<int32>(TargetSize);
		OutBitmap.SetNum(OutWidth * OutHeight);
		if (!RenderTargetResource->ReadPixelsPtr(OutBitmap.GetData(), FReadSurfaceDataFlags(), FIntRect(0, 0, OutWidth, OutHeight)))
		{
			OutBitmap.Reset();
			OutWidth = 0;
			OutHeight = 0;
			OutError = TEXT("Failed to read static mesh preview pixels.");
			return false;
		}

		OutCaptureSource = TEXT("static_mesh_thumbnail_scene");
		return true;
	}

	static bool DetectLikelyFlipbookGrid(
		const UTexture* Texture,
		const TArray<FColor>& PreviewBitmap,
		const int32 PreviewWidth,
		const int32 PreviewHeight,
		int32& OutColumns,
		int32& OutRows,
		double& OutConfidence)
	{
		OutColumns = 1;
		OutRows = 1;
		OutConfidence = 0.0;
		if (!Texture || PreviewBitmap.Num() == 0 || PreviewWidth <= 0 || PreviewHeight <= 0)
		{
			return false;
		}

		const FString TextureNameLower = Texture->GetName().ToLower();
		const bool bHasSheetKeyword =
			TextureNameLower.Contains(TEXT("subuv")) ||
			TextureNameLower.Contains(TEXT("flipbook")) ||
			TextureNameLower.Contains(TEXT("sheet")) ||
			TextureNameLower.Contains(TEXT("atlas"));

		const TArray<int32> CandidateDivisors = { 2, 4, 5, 6, 8, 10, 12, 16 };
		const auto SampleBrightness = [&PreviewBitmap, PreviewWidth, PreviewHeight](int32 X0, int32 Y0, int32 X1, int32 Y1) -> double
		{
			double Sum = 0.0;
			int32 Count = 0;
			for (int32 Y = Y0; Y < Y1; ++Y)
			{
				if (Y < 0 || Y >= PreviewHeight) continue;
				for (int32 X = X0; X < X1; ++X)
				{
					if (X < 0 || X >= PreviewWidth) continue;
					const FColor& Pixel = PreviewBitmap[Y * PreviewWidth + X];
					Sum += (static_cast<double>(Pixel.R) + static_cast<double>(Pixel.G) + static_cast<double>(Pixel.B)) / 3.0;
					++Count;
				}
			}
			return Count > 0 ? Sum / static_cast<double>(Count) : 0.0;
		};

		double BestScore = 0.0;
		int32 BestCols = 1;
		int32 BestRows = 1;

		for (int32 Columns : CandidateDivisors)
		{
			for (int32 Rows : CandidateDivisors)
			{
				if (Columns * Rows < 4)
				{
					continue;
				}
				if (PreviewWidth % Columns != 0 || PreviewHeight % Rows != 0)
				{
					continue;
				}

				const int32 CellWidth = PreviewWidth / Columns;
				const int32 CellHeight = PreviewHeight / Rows;
				if (CellWidth < 8 || CellHeight < 8)
				{
					continue;
				}

				TArray<double> CellMeans;
				double Mean = 0.0;
				int32 NonEmptyCells = 0;
				for (int32 Row = 0; Row < Rows; ++Row)
				{
					for (int32 Col = 0; Col < Columns; ++Col)
					{
						const double CellMean = SampleBrightness(Col * CellWidth, Row * CellHeight, (Col + 1) * CellWidth, (Row + 1) * CellHeight);
						CellMeans.Add(CellMean);
						Mean += CellMean;
						if (CellMean > 8.0)
						{
							++NonEmptyCells;
						}
					}
				}

				if (CellMeans.Num() == 0)
				{
					continue;
				}

				Mean /= static_cast<double>(CellMeans.Num());
				double Variance = 0.0;
				for (const double CellMean : CellMeans)
				{
					Variance += FMath::Square(CellMean - Mean);
				}
				Variance /= static_cast<double>(CellMeans.Num());

				const double OccupancyRatio = static_cast<double>(NonEmptyCells) / static_cast<double>(CellMeans.Num());
				double Score = 0.0;
				Score += FMath::Clamp(OccupancyRatio, 0.0, 1.0) * 0.35;
				Score += FMath::Clamp(Variance / 1500.0, 0.0, 0.35);
				Score += (Columns >= 4 || Rows >= 4) ? 0.1 : 0.0;
				Score += bHasSheetKeyword ? 0.2 : 0.0;

				if (Score > BestScore)
				{
					BestScore = Score;
					BestCols = Columns;
					BestRows = Rows;
				}
			}
		}

		if (BestScore >= 0.45)
		{
			OutColumns = BestCols;
			OutRows = BestRows;
			OutConfidence = BestScore;
			return true;
		}

		return false;
	}

	static TSharedPtr<FJsonObject> BuildTextureMaterialGuidance(UTexture* Texture, const TArray<FColor>& PreviewBitmap, const int32 PreviewWidth, const int32 PreviewHeight)
	{
		TSharedPtr<FJsonObject> Guidance = MakeShareable(new FJsonObject());
		if (!Texture)
		{
			return Guidance;
		}

		const FString TextureNameLower = Texture->GetName().ToLower();
		const bool bSrgb = Texture->SRGB;
		const TextureCompressionSettings Compression = Texture->CompressionSettings;
		const bool bLikelyNormal = Compression == TC_Normalmap ||
			TextureNameLower.Contains(TEXT("normal")) ||
			TextureNameLower.Contains(TEXT("_n")) ||
			TextureNameLower.Contains(TEXT("nrm"));
		const bool bLikelyMaskCompression =
			Compression == TC_Masks ||
			Compression == TC_Grayscale ||
			Compression == TC_Alpha;

		bool bLooksGrayscale = false;
		double MeanBrightness = 0.0;
		if (PreviewBitmap.Num() > 0)
		{
			double ChannelDeltaTotal = 0.0;
			double BrightnessTotal = 0.0;
			for (const FColor& Pixel : PreviewBitmap)
			{
				ChannelDeltaTotal +=
					FMath::Abs(static_cast<double>(Pixel.R) - static_cast<double>(Pixel.G)) +
					FMath::Abs(static_cast<double>(Pixel.R) - static_cast<double>(Pixel.B)) +
					FMath::Abs(static_cast<double>(Pixel.G) - static_cast<double>(Pixel.B));
				BrightnessTotal += (static_cast<double>(Pixel.R) + static_cast<double>(Pixel.G) + static_cast<double>(Pixel.B)) / 3.0;
			}

			const double PixelCount = static_cast<double>(PreviewBitmap.Num());
			const double MeanChannelDelta = ChannelDeltaTotal / (PixelCount * 3.0);
			MeanBrightness = BrightnessTotal / PixelCount;
			bLooksGrayscale = MeanChannelDelta < 7.5;
		}

		bool bLikelyFlipbook = false;
		int32 FlipbookColumns = 1;
		int32 FlipbookRows = 1;
		double FlipbookConfidence = 0.0;
		if (const UTexture2D* Texture2D = Cast<UTexture2D>(Texture))
		{
			const int32 SizeX = Texture2D->GetSizeX();
			const int32 SizeY = Texture2D->GetSizeY();
			if (SizeX > 0 && SizeY > 0)
			{
				const double AspectRatio = static_cast<double>(SizeX) / static_cast<double>(SizeY);
				const bool bSheetKeyword =
					TextureNameLower.Contains(TEXT("flipbook")) ||
					TextureNameLower.Contains(TEXT("sheet")) ||
					TextureNameLower.Contains(TEXT("atlas")) ||
					TextureNameLower.Contains(TEXT("fire")) ||
					TextureNameLower.Contains(TEXT("smoke")) ||
					TextureNameLower.Contains(TEXT("ember")) ||
					TextureNameLower.Contains(TEXT("explosion"));
				bLikelyFlipbook = (AspectRatio >= 1.5 || AspectRatio <= 0.66) && bSheetKeyword;
			}
		}
		if (DetectLikelyFlipbookGrid(Texture, PreviewBitmap, PreviewWidth, PreviewHeight, FlipbookColumns, FlipbookRows, FlipbookConfidence))
		{
			bLikelyFlipbook = true;
		}

		FString ProbableContentType = TEXT("color_texture");
		FString SuggestedFormula = TEXT("Sample as a regular color texture first, then route into BaseColor or Emissive depending on the effect.");
		TArray<TSharedPtr<FJsonValue>> SuggestedRoles;
		TArray<TSharedPtr<FJsonValue>> AgentNotes;

		auto AddRole = [&SuggestedRoles](const FString& Role)
		{
			SuggestedRoles.Add(MakeShareable(new FJsonValueString(Role)));
		};
		auto AddNote = [&AgentNotes](const FString& Note)
		{
			AgentNotes.Add(MakeShareable(new FJsonValueString(Note)));
		};

		if (bLikelyNormal)
		{
			ProbableContentType = TEXT("normal_map");
			SuggestedFormula = TEXT("Use as a Normal input texture. Keep sRGB disabled, sample with Normal semantics, and avoid treating it like color data.");
			AddRole(TEXT("normal"));
			AddNote(TEXT("This texture reads like a normal map from compression settings or naming."));
		}
		else if (bLikelyFlipbook)
		{
			ProbableContentType = TEXT("flipbook_effect_sheet");
			SuggestedFormula = TEXT("Treat this as a flipbook sheet for particles or animated emissive effects. Pair with SubUV or animated UV logic rather than a static surface formula.");
			AddRole(TEXT("particle_flipbook"));
			AddRole(TEXT("emissive_effect"));
			AddNote(TEXT("Wide/tall sheet layout plus VFX-style naming suggests a flipbook texture."));
		}
		else if (!bSrgb && (bLikelyMaskCompression || bLooksGrayscale))
		{
			ProbableContentType = bLooksGrayscale ? TEXT("grayscale_mask") : TEXT("packed_mask_or_data");
			SuggestedFormula = TEXT("Treat as linear data. Use ComponentMask / channel extraction and avoid color correction. Good candidate for dissolve, opacity, roughness, or packed masks.");
			AddRole(TEXT("mask"));
			AddRole(TEXT("noise"));
			AddRole(TEXT("data_texture"));
			AddNote(TEXT("Texture is linear or grayscale, so it should drive math/data instead of final color."));
		}
		else if (bSrgb && bLooksGrayscale)
		{
			ProbableContentType = TEXT("soft_grayscale_effect");
			SuggestedFormula = TEXT("Looks grayscale but stored as color. It can drive emissive/noise or be converted to linear math if the effect needs mask-like control.");
			AddRole(TEXT("emissive_mask"));
			AddRole(TEXT("soft_noise"));
			AddNote(TEXT("Visual preview looks grayscale even though the texture is stored as color."));
		}
		else
		{
			AddRole(TEXT("base_color"));
			AddRole(TEXT("emissive"));
			AddRole(TEXT("particle_color"));
			AddNote(TEXT("This texture reads like visible color content and is safe to start from a color-driven material formula."));
		}

		Guidance->SetStringField(TEXT("probable_content_type"), ProbableContentType);
		Guidance->SetStringField(TEXT("suggested_material_formula"), SuggestedFormula);
		Guidance->SetArrayField(TEXT("suggested_material_roles"), SuggestedRoles);
		Guidance->SetArrayField(TEXT("agent_notes"), AgentNotes);
		Guidance->SetBoolField(TEXT("looks_grayscale"), bLooksGrayscale);
		Guidance->SetBoolField(TEXT("looks_like_flipbook"), bLikelyFlipbook);
		Guidance->SetBoolField(TEXT("looks_like_normal_map"), bLikelyNormal);
		Guidance->SetNumberField(TEXT("flipbook_columns"), FlipbookColumns);
		Guidance->SetNumberField(TEXT("flipbook_rows"), FlipbookRows);
		Guidance->SetNumberField(TEXT("flipbook_confidence"), FlipbookConfidence);
		Guidance->SetNumberField(TEXT("preview_mean_brightness"), MeanBrightness);
		Guidance->SetNumberField(TEXT("preview_width"), PreviewWidth);
		Guidance->SetNumberField(TEXT("preview_height"), PreviewHeight);
		return Guidance;
	}

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

	struct FMaterialParameterDefaults
	{
		TMap<FName, float> Scalars;
		TMap<FName, FLinearColor> Vectors;
		TMap<FName, FString> Textures;
	};

	static void BuildDefaultsFromMaterialInterface(UMaterialInterface* Source, FMaterialParameterDefaults& OutDefaults)
	{
		if (!Source)
		{
			return;
		}

		if (UMaterialInstanceConstant* SourceMIC = Cast<UMaterialInstanceConstant>(Source))
		{
			BuildDefaultsFromMaterialInterface(SourceMIC->Parent, OutDefaults);

			for (const FScalarParameterValue& ScalarValue : SourceMIC->ScalarParameterValues)
			{
				OutDefaults.Scalars.Add(ScalarValue.ParameterInfo.Name, ScalarValue.ParameterValue);
			}

			for (const FVectorParameterValue& VectorValue : SourceMIC->VectorParameterValues)
			{
				OutDefaults.Vectors.Add(VectorValue.ParameterInfo.Name, VectorValue.ParameterValue);
			}

			for (const FTextureParameterValue& TextureValue : SourceMIC->TextureParameterValues)
			{
				const FString TexturePath = TextureValue.ParameterValue
					? TextureValue.ParameterValue->GetPathName()
					: FString();
				OutDefaults.Textures.Add(TextureValue.ParameterInfo.Name, TexturePath);
			}
			return;
		}

		UMaterial* SourceMaterial = Cast<UMaterial>(Source);
		if (!SourceMaterial)
		{
			return;
		}

		for (UMaterialExpression* Expr : SourceMaterial->GetExpressionCollection().Expressions)
		{
			if (!Expr) continue;

			if (UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expr))
			{
				OutDefaults.Scalars.Add(ScalarParam->ParameterName, ScalarParam->DefaultValue);
				continue;
			}

			if (UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expr))
			{
				OutDefaults.Vectors.Add(VectorParam->ParameterName, VectorParam->DefaultValue);
				continue;
			}

			if (UMaterialExpressionTextureSampleParameter2D* TextureParam2D = Cast<UMaterialExpressionTextureSampleParameter2D>(Expr))
			{
				const FString TexturePath = TextureParam2D->Texture
					? TextureParam2D->Texture->GetPathName()
					: FString();
				OutDefaults.Textures.Add(TextureParam2D->ParameterName, TexturePath);
				continue;
			}
		}
	}

	static bool TryReadOptionalIntField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, int32& OutValue)
	{
		if (!Input.IsValid())
		{
			return false;
		}

		double NumericValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumericValue))
		{
			OutValue = static_cast<int32>(NumericValue);
			return true;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue) && !StringValue.IsEmpty())
		{
			OutValue = FCString::Atoi(*StringValue);
			return true;
		}

		return false;
	}

	static bool ParseColorFromString(const FString& Str, FLinearColor& OutColor)
	{
		FString Trimmed = Str;
		Trimmed = Trimmed.Replace(TEXT("["), TEXT("")).Replace(TEXT("]"), TEXT(""));
		TArray<FString> Parts;
		Trimmed.ParseIntoArray(Parts, TEXT(","));
		if (Parts.Num() >= 3)
		{
			OutColor.R = FCString::Atof(*Parts[0]);
			OutColor.G = FCString::Atof(*Parts[1]);
			OutColor.B = FCString::Atof(*Parts[2]);
			OutColor.A = Parts.Num() >= 4 ? FCString::Atof(*Parts[3]) : 1.0f;
			return true;
		}
		return false;
	}

	static bool ParseMaterialCustomOutputType(const FString& Str, ECustomMaterialOutputType& OutType)
	{
		const FString Normalized = Str.TrimStartAndEnd().ToLower();
		if (Normalized.IsEmpty() || Normalized == TEXT("float1") || Normalized == TEXT("scalar"))
		{
			OutType = CMOT_Float1;
			return true;
		}
		if (Normalized == TEXT("float2") || Normalized == TEXT("vector2"))
		{
			OutType = CMOT_Float2;
			return true;
		}
		if (Normalized == TEXT("float3") || Normalized == TEXT("vector3"))
		{
			OutType = CMOT_Float3;
			return true;
		}
		if (Normalized == TEXT("float4") || Normalized == TEXT("vector4") || Normalized == TEXT("color"))
		{
			OutType = CMOT_Float4;
			return true;
		}
		if (Normalized == TEXT("materialattributes") || Normalized == TEXT("material_attributes"))
		{
			OutType = CMOT_MaterialAttributes;
			return true;
		}
		return false;
	}

	static FString GetMaterialCustomOutputTypeLabel(const ECustomMaterialOutputType OutputType)
	{
		switch (OutputType)
		{
		case CMOT_Float1: return TEXT("Float1");
		case CMOT_Float2: return TEXT("Float2");
		case CMOT_Float3: return TEXT("Float3");
		case CMOT_Float4: return TEXT("Float4");
		case CMOT_MaterialAttributes: return TEXT("MaterialAttributes");
		default: return TEXT("Unknown");
		}
	}

	static TArray<TSharedPtr<FJsonValue>> MakeMaterialStringJsonArray(const TArray<FString>& Strings)
	{
		TArray<TSharedPtr<FJsonValue>> Values;
		for (const FString& Value : Strings)
		{
			Values.Add(MakeShareable(new FJsonValueString(Value)));
		}
		return Values;
	}

	static TArray<TSharedPtr<FJsonValue>> MakeMaterialIntJsonArray(const TArray<int32>& ValuesIn)
	{
		TArray<TSharedPtr<FJsonValue>> Values;
		for (const int32 Value : ValuesIn)
		{
			Values.Add(MakeShareable(new FJsonValueNumber(Value)));
		}
		return Values;
	}

	static TArray<UMaterialExpressionComment*> GetMaterialComments(UMaterial* Material)
	{
		TArray<UMaterialExpressionComment*> Comments;
		if (!Material)
		{
			return Comments;
		}

		for (TObjectPtr<UMaterialExpressionComment> Comment : Material->GetEditorComments())
		{
			if (Comment)
			{
				Comments.Add(Comment);
			}
		}
		return Comments;
	}

	static int32 FindMaterialCommentIndex(
		UMaterial* Material,
		const TOptional<int32>& RequestedIndex,
		const FString& ExactText,
		const FString& ContainsText)
	{
		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		if (RequestedIndex.IsSet())
		{
			return Comments.IsValidIndex(RequestedIndex.GetValue()) ? RequestedIndex.GetValue() : INDEX_NONE;
		}

		if (!ExactText.IsEmpty())
		{
			for (int32 CommentIndex = 0; CommentIndex < Comments.Num(); ++CommentIndex)
			{
				if (Comments[CommentIndex] && Comments[CommentIndex]->Text.Equals(ExactText, ESearchCase::IgnoreCase))
				{
					return CommentIndex;
				}
			}
		}

		if (!ContainsText.IsEmpty())
		{
			for (int32 CommentIndex = 0; CommentIndex < Comments.Num(); ++CommentIndex)
			{
				if (Comments[CommentIndex] && Comments[CommentIndex]->Text.Contains(ContainsText, ESearchCase::IgnoreCase))
				{
					return CommentIndex;
				}
			}
		}

		if (Comments.Num() == 1)
		{
			return 0;
		}

		return INDEX_NONE;
	}

	static bool ComputeMaterialExpressionBounds(
		UMaterial* Material,
		const TArray<int32>& ExpressionIndices,
		int32& OutMinX,
		int32& OutMinY,
		int32& OutMaxX,
		int32& OutMaxY)
	{
		if (!Material || ExpressionIndices.Num() == 0)
		{
			return false;
		}

		bool bFoundAny = false;
		for (const int32 ExpressionIndex : ExpressionIndices)
		{
			if (!Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex))
			{
				continue;
			}

			UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex];
			if (!Expr)
			{
				continue;
			}

			const int32 NodeMinX = Expr->MaterialExpressionEditorX;
			const int32 NodeMinY = Expr->MaterialExpressionEditorY;
			const int32 NodeMaxX = Expr->MaterialExpressionEditorX + 260;
			const int32 NodeMaxY = Expr->MaterialExpressionEditorY + 220;

			OutMinX = bFoundAny ? FMath::Min(OutMinX, NodeMinX) : NodeMinX;
			OutMinY = bFoundAny ? FMath::Min(OutMinY, NodeMinY) : NodeMinY;
			OutMaxX = bFoundAny ? FMath::Max(OutMaxX, NodeMaxX) : NodeMaxX;
			OutMaxY = bFoundAny ? FMath::Max(OutMaxY, NodeMaxY) : NodeMaxY;
			bFoundAny = true;
		}

		return bFoundAny;
	}

	static bool IsCommentContainingPoint(const UMaterialExpressionComment* Comment, float X, float Y)
	{
		if (!Comment)
		{
			return false;
		}

		const float MinX = static_cast<float>(Comment->MaterialExpressionEditorX);
		const float MinY = static_cast<float>(Comment->MaterialExpressionEditorY);
		const float MaxX = MinX + static_cast<float>(FMath::Max(Comment->SizeX, 0));
		const float MaxY = MinY + static_cast<float>(FMath::Max(Comment->SizeY, 0));
		return X >= MinX && X <= MaxX && Y >= MinY && Y <= MaxY;
	}

	static FString GetMaterialExpressionCategory(const FString& ExprType)
	{
		if (ExprType.Equals(TEXT("ScalarParameter"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("VectorParameter"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("TextureSampleParameter2D"), ESearchCase::IgnoreCase))
		{
			return TEXT("parameter");
		}

		if (ExprType.Equals(TEXT("TextureSample"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("TexCoord"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Panner"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Time"), ESearchCase::IgnoreCase))
		{
			return TEXT("input");
		}

		if (ExprType.Equals(TEXT("Multiply"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Add"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Lerp"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Clamp"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("OneMinus"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Power"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("ComponentMask"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("AppendVector"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("DotProduct"), ESearchCase::IgnoreCase) ||
			ExprType.Equals(TEXT("Fresnel"), ESearchCase::IgnoreCase))
		{
			return TEXT("math");
		}

		return TEXT("generic");
	}

	static void ComputeMaterialAutoPlacement(UMaterial* Material, const FString& ExprType, int32& OutX, int32& OutY)
	{
		int32 MinX = 0;
		int32 MaxX = 0;
		int32 MaxY = 0;
		bool bHasAnyNode = false;

		if (Material)
		{
			for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
			{
				if (!Expr)
				{
					continue;
				}
				MinX = bHasAnyNode ? FMath::Min(MinX, Expr->MaterialExpressionEditorX) : Expr->MaterialExpressionEditorX;
				MaxX = bHasAnyNode ? FMath::Max(MaxX, Expr->MaterialExpressionEditorX) : Expr->MaterialExpressionEditorX;
				MaxY = bHasAnyNode ? FMath::Max(MaxY, Expr->MaterialExpressionEditorY) : Expr->MaterialExpressionEditorY;
				bHasAnyNode = true;
			}

			for (UMaterialExpressionComment* Comment : GetMaterialComments(Material))
			{
				if (!Comment)
				{
					continue;
				}
				MinX = bHasAnyNode ? FMath::Min(MinX, Comment->MaterialExpressionEditorX) : Comment->MaterialExpressionEditorX;
				MaxX = bHasAnyNode ? FMath::Max(MaxX, Comment->MaterialExpressionEditorX + Comment->SizeX) : (Comment->MaterialExpressionEditorX + Comment->SizeX);
				MaxY = bHasAnyNode ? FMath::Max(MaxY, Comment->MaterialExpressionEditorY + Comment->SizeY) : (Comment->MaterialExpressionEditorY + Comment->SizeY);
				bHasAnyNode = true;
			}
		}

		if (!bHasAnyNode)
		{
			MinX = 0;
			MaxX = 0;
			MaxY = 0;
		}

		const FString Category = GetMaterialExpressionCategory(ExprType);
		if (Category == TEXT("parameter"))
		{
			OutX = MinX - 420;
		}
		else if (Category == TEXT("input"))
		{
			OutX = MinX - 160;
		}
		else if (Category == TEXT("math"))
		{
			OutX = MaxX + 120;
		}
		else
		{
			OutX = MaxX + 260;
		}

		OutY = MaxY + 180;
	}

	struct FMaterialGraphViewState
	{
		FBox2D FocusRect = FBox2D(EForceInit::ForceInit);
		float Zoom = 0.0f;
		bool bHasFocusRect = false;
	};

	struct FMaterialGraphVisualNode
	{
		int32 ExpressionIndex = INDEX_NONE;
		FString Label;
		FString Category;
		FVector2D Position = FVector2D::ZeroVector;
		FVector2D Size = FVector2D(260.0f, 180.0f);
	};

	struct FMaterialGraphVisualComment
	{
		int32 CommentIndex = INDEX_NONE;
		FString Text;
		FVector2D Position = FVector2D::ZeroVector;
		FVector2D Size = FVector2D(800.0f, 420.0f);
		FLinearColor Color = FLinearColor(1.0f, 0.92f, 0.15f, 1.0f);
	};

	struct FMaterialGraphVisualLink
	{
		int32 SourceExpressionIndex = INDEX_NONE;
		int32 TargetExpressionIndex = INDEX_NONE;
		FString InputName;
	};

	static TMap<FString, FMaterialGraphViewState> GMaterialGraphViewStates;

	static FString SanitizeMaterialProbeToken(FString Value)
	{
		Value = Value.TrimStartAndEnd();
		for (TCHAR& Character : Value)
		{
			if (!FChar::IsAlnum(Character) && Character != TEXT('_') && Character != TEXT('-'))
			{
				Character = TEXT('_');
			}
		}
		return Value.IsEmpty() ? TEXT("capture") : Value;
	}

	static FString GetDefaultMaterialProbeOutputDir(const FString& MaterialName, const FString& Prefix)
	{
		const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
		return FPaths::Combine(
			GetForgePilotUserDownloadsDir(),
			TEXT("ForgePilotMaterialProbes"),
			FString::Printf(TEXT("%s_%s_%s"), *SanitizeMaterialProbeToken(Prefix), *SanitizeMaterialProbeToken(MaterialName), *Timestamp));
	}

	static bool SaveColorBufferToPngFile(const TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const FString& OutputPath, FString& OutError)
	{
		if (Bitmap.Num() < Width * Height || Width <= 0 || Height <= 0)
		{
			OutError = TEXT("Invalid image buffer.");
			return false;
		}

		const FString OutputDir = FPaths::GetPath(OutputPath);
		IFileManager::Get().MakeDirectory(*OutputDir, true);

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		if (!ImageWrapper.IsValid())
		{
			OutError = TEXT("Failed to create PNG image wrapper.");
			return false;
		}

		TArray<uint8> RawData;
		RawData.SetNum(Bitmap.Num() * 4);
		for (int32 PixelIndex = 0; PixelIndex < Bitmap.Num(); ++PixelIndex)
		{
			RawData[PixelIndex * 4 + 0] = Bitmap[PixelIndex].B;
			RawData[PixelIndex * 4 + 1] = Bitmap[PixelIndex].G;
			RawData[PixelIndex * 4 + 2] = Bitmap[PixelIndex].R;
			RawData[PixelIndex * 4 + 3] = Bitmap[PixelIndex].A;
		}

		if (!ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), Width, Height, ERGBFormat::BGRA, 8))
		{
			OutError = TEXT("Failed to convert raw image buffer to PNG.");
			return false;
		}

		const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed(100);
		if (CompressedData.Num() == 0)
		{
			OutError = TEXT("Failed to compress PNG image.");
			return false;
		}

		if (!FFileHelper::SaveArrayToFile(TArrayView<const uint8>(CompressedData.GetData(), CompressedData.Num()), *OutputPath))
		{
			OutError = FString::Printf(TEXT("Failed to save capture file: %s"), *OutputPath);
			return false;
		}

		return true;
	}

	static bool SaveColorBufferToImageFile(
		const TArray<FColor>& Bitmap,
		const int32 Width,
		const int32 Height,
		const FString& OutputPath,
		FString& OutError,
		const int32 JpegQuality = 90)
	{
		if (Bitmap.Num() < Width * Height || Width <= 0 || Height <= 0)
		{
			OutError = TEXT("Invalid image buffer.");
			return false;
		}

		const FString Extension = FPaths::GetExtension(OutputPath).ToLower();
		if (Extension == TEXT("png"))
		{
			return SaveColorBufferToPngFile(Bitmap, Width, Height, OutputPath, OutError);
		}

		const FString OutputDir = FPaths::GetPath(OutputPath);
		IFileManager::Get().MakeDirectory(*OutputDir, true);

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		if (!ImageWrapper.IsValid())
		{
			OutError = TEXT("Failed to create JPEG image wrapper.");
			return false;
		}

		TArray<uint8> RawData;
		RawData.SetNum(Bitmap.Num() * 4);
		const FVector3f JpegBackground(52.0f / 255.0f, 56.0f / 255.0f, 64.0f / 255.0f);
		for (int32 PixelIndex = 0; PixelIndex < Bitmap.Num(); ++PixelIndex)
		{
			const FColor& Pixel = Bitmap[PixelIndex];
			const float Alpha = static_cast<float>(Pixel.A) / 255.0f;
			float SourceR = static_cast<float>(Pixel.R) / 255.0f;
			float SourceG = static_cast<float>(Pixel.G) / 255.0f;
			float SourceB = static_cast<float>(Pixel.B) / 255.0f;

			if (Alpha > KINDA_SMALL_NUMBER && Alpha < 1.0f)
			{
				const float InvAlpha = 1.0f / Alpha;
				SourceR = FMath::Clamp(SourceR * InvAlpha, 0.0f, 1.0f);
				SourceG = FMath::Clamp(SourceG * InvAlpha, 0.0f, 1.0f);
				SourceB = FMath::Clamp(SourceB * InvAlpha, 0.0f, 1.0f);
			}

			const float FlattenedR = FMath::Lerp(JpegBackground.X, SourceR, Alpha);
			const float FlattenedG = FMath::Lerp(JpegBackground.Y, SourceG, Alpha);
			const float FlattenedB = FMath::Lerp(JpegBackground.Z, SourceB, Alpha);

			RawData[PixelIndex * 4 + 0] = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(FlattenedB * 255.0f), 0, 255));
			RawData[PixelIndex * 4 + 1] = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(FlattenedG * 255.0f), 0, 255));
			RawData[PixelIndex * 4 + 2] = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(FlattenedR * 255.0f), 0, 255));
			RawData[PixelIndex * 4 + 3] = 255;
		}

		if (!ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), Width, Height, ERGBFormat::BGRA, 8))
		{
			OutError = TEXT("Failed to convert raw image buffer.");
			return false;
		}

		const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed(FMath::Clamp(JpegQuality, 1, 100));
		if (CompressedData.Num() == 0)
		{
			OutError = TEXT("Failed to compress image.");
			return false;
		}

		if (!FFileHelper::SaveArrayToFile(TArrayView<const uint8>(CompressedData.GetData(), CompressedData.Num()), *OutputPath))
		{
			OutError = FString::Printf(TEXT("Failed to save capture file: %s"), *OutputPath);
			return false;
		}

		return true;
	}

	static AActor* FindActorByLabelForMaterialTools(UWorld* World, const FString& ActorLabel)
	{
		if (!World || ActorLabel.IsEmpty())
		{
			return nullptr;
		}

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			if (It->GetActorLabel() == ActorLabel)
			{
				return *It;
			}
		}

		return nullptr;
	}

	static bool IsMaterialProbeCaptureMostlyBlack(const TArray<FColor>& Bitmap)
	{
		if (Bitmap.Num() == 0)
		{
			return true;
		}

		int64 LumaSum = 0;
		int32 NonBlackPixelCount = 0;
		for (const FColor& Pixel : Bitmap)
		{
			const int32 Luma = (static_cast<int32>(Pixel.R) + static_cast<int32>(Pixel.G) + static_cast<int32>(Pixel.B)) / 3;
			LumaSum += Luma;
			if (Luma > 8)
			{
				++NonBlackPixelCount;
			}
		}

		const double AverageLuma = static_cast<double>(LumaSum) / static_cast<double>(Bitmap.Num());
		const double NonBlackRatio = static_cast<double>(NonBlackPixelCount) / static_cast<double>(Bitmap.Num());
		return AverageLuma < 4.0 || NonBlackRatio < 0.01;
	}

	static bool ComputeFullMaterialGraphBounds(UMaterial* Material, FBox2D& OutBounds)
	{
		OutBounds = FBox2D(EForceInit::ForceInit);
		if (!Material)
		{
			return false;
		}

		bool bHasAny = false;
		for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
		{
			if (!Expr)
			{
				continue;
			}

			const FVector2D NodeMin(Expr->MaterialExpressionEditorX, Expr->MaterialExpressionEditorY);
			const FVector2D NodeMax(Expr->MaterialExpressionEditorX + 260.0f, Expr->MaterialExpressionEditorY + 180.0f);
			OutBounds += NodeMin;
			OutBounds += NodeMax;
			bHasAny = true;
		}

		for (UMaterialExpressionComment* Comment : GetMaterialComments(Material))
		{
			if (!Comment)
			{
				continue;
			}

			const FVector2D CommentMin(Comment->MaterialExpressionEditorX, Comment->MaterialExpressionEditorY);
			const FVector2D CommentMax(Comment->MaterialExpressionEditorX + Comment->SizeX, Comment->MaterialExpressionEditorY + Comment->SizeY);
			OutBounds += CommentMin;
			OutBounds += CommentMax;
			bHasAny = true;
		}

		return bHasAny;
	}

	static bool ComputeMaterialFocusBoundsFromExpressions(UMaterial* Material, const TArray<int32>& ExpressionIndices, const int32 Padding, FBox2D& OutBounds)
	{
		int32 MinX = 0;
		int32 MinY = 0;
		int32 MaxX = 0;
		int32 MaxY = 0;
		if (!ComputeMaterialExpressionBounds(Material, ExpressionIndices, MinX, MinY, MaxX, MaxY))
		{
			return false;
		}

		OutBounds = FBox2D(
			FVector2D(static_cast<float>(MinX - Padding), static_cast<float>(MinY - Padding)),
			FVector2D(static_cast<float>(MaxX + Padding), static_cast<float>(MaxY + Padding)));
		return true;
	}

	static bool ResolveMaterialExpressionsFromQuery(UMaterial* Material, const FString& FocusQuery, TArray<int32>& OutExpressionIndices, TArray<int32>& OutCommentIndices)
	{
		OutExpressionIndices.Reset();
		OutCommentIndices.Reset();

		if (!Material || FocusQuery.IsEmpty())
		{
			return false;
		}

		const FString QueryLower = FocusQuery.ToLower();
		for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex];
			if (!Expr)
			{
				continue;
			}

			FString SearchText = GetMaterialExpressionPracticalName(Expr);
			SearchText += TEXT(" ");
			SearchText += GetMaterialExpressionCaptionText(Expr);
			SearchText += TEXT(" ");
			SearchText += Expr->Desc;

			if (const UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expr))
			{
				SearchText += TEXT(" ");
				SearchText += ScalarParam->ParameterName.ToString();
			}
			else if (const UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expr))
			{
				SearchText += TEXT(" ");
				SearchText += VectorParam->ParameterName.ToString();
			}
			else if (const UMaterialExpressionTextureSampleParameter2D* TextureParam = Cast<UMaterialExpressionTextureSampleParameter2D>(Expr))
			{
				SearchText += TEXT(" ");
				SearchText += TextureParam->ParameterName.ToString();
			}

			if (SearchText.ToLower().Contains(QueryLower))
			{
				OutExpressionIndices.Add(ExpressionIndex);
			}
		}

		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		for (int32 CommentIndex = 0; CommentIndex < Comments.Num(); ++CommentIndex)
		{
			if (Comments[CommentIndex] && Comments[CommentIndex]->Text.ToLower().Contains(QueryLower))
			{
				OutCommentIndices.Add(CommentIndex);
			}
		}

		return OutExpressionIndices.Num() > 0 || OutCommentIndices.Num() > 0;
	}

	static bool ResolveMaterialFocusBounds(
		UMaterial* Material,
		const TSharedPtr<FJsonObject>& Input,
		const FMaterialGraphViewState* ExistingState,
		FBox2D& OutBounds,
		TArray<int32>& OutFocusExpressionIndices,
		TArray<int32>& OutFocusCommentIndices,
		FString& OutFocusSource)
	{
		OutBounds = FBox2D(EForceInit::ForceInit);
		OutFocusExpressionIndices.Reset();
		OutFocusCommentIndices.Reset();
		OutFocusSource.Reset();

		if (!Material)
		{
			return false;
		}

		int32 Padding = 120;
		TryReadOptionalIntField(Input, TEXT("padding"), Padding);

		const TArray<TSharedPtr<FJsonValue>>* RegionBoxArray = nullptr;
		if (Input.IsValid() && Input->TryGetArrayField(TEXT("focus_region_box"), RegionBoxArray) && RegionBoxArray && RegionBoxArray->Num() >= 4)
		{
			const float MinX = static_cast<float>((*RegionBoxArray)[0]->AsNumber());
			const float MinY = static_cast<float>((*RegionBoxArray)[1]->AsNumber());
			const float MaxX = static_cast<float>((*RegionBoxArray)[2]->AsNumber());
			const float MaxY = static_cast<float>((*RegionBoxArray)[3]->AsNumber());
			OutBounds = FBox2D(
				FVector2D(FMath::Min(MinX, MaxX), FMath::Min(MinY, MaxY)),
				FVector2D(FMath::Max(MinX, MaxX), FMath::Max(MinY, MaxY)));
			OutFocusSource = TEXT("focus_region_box");
			return true;
		}

		int32 FocusExpressionIndex = INDEX_NONE;
		if (TryReadOptionalIntField(Input, TEXT("expression_index"), FocusExpressionIndex) ||
			TryReadOptionalIntField(Input, TEXT("focus_expression_index"), FocusExpressionIndex))
		{
			OutFocusExpressionIndices.Add(FocusExpressionIndex);
			if (ComputeMaterialFocusBoundsFromExpressions(Material, OutFocusExpressionIndices, Padding, OutBounds))
			{
				OutFocusSource = TEXT("expression_index");
				return true;
			}
			OutFocusExpressionIndices.Reset();
		}

		const TArray<TSharedPtr<FJsonValue>>* ExpressionIndicesArray = nullptr;
		if (Input.IsValid() && Input->TryGetArrayField(TEXT("expression_indices"), ExpressionIndicesArray) && ExpressionIndicesArray)
		{
			for (const TSharedPtr<FJsonValue>& Value : *ExpressionIndicesArray)
			{
				if (Value.IsValid())
				{
					OutFocusExpressionIndices.Add(static_cast<int32>(Value->AsNumber()));
				}
			}
			if (OutFocusExpressionIndices.Num() > 0 && ComputeMaterialFocusBoundsFromExpressions(Material, OutFocusExpressionIndices, Padding, OutBounds))
			{
				OutFocusSource = TEXT("expression_indices");
				return true;
			}
			OutFocusExpressionIndices.Reset();
		}

		int32 CommentIndex = INDEX_NONE;
		if (TryReadOptionalIntField(Input, TEXT("comment_index"), CommentIndex))
		{
			const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
			if (Comments.IsValidIndex(CommentIndex) && Comments[CommentIndex])
			{
				UMaterialExpressionComment* Comment = Comments[CommentIndex];
				OutFocusCommentIndices.Add(CommentIndex);
				OutBounds = FBox2D(
					FVector2D(Comment->MaterialExpressionEditorX - Padding, Comment->MaterialExpressionEditorY - Padding),
					FVector2D(Comment->MaterialExpressionEditorX + Comment->SizeX + Padding, Comment->MaterialExpressionEditorY + Comment->SizeY + Padding));
				OutFocusSource = TEXT("comment_index");
				return true;
			}
			OutFocusCommentIndices.Reset();
		}

		FString CommentText;
		FString ContainsText;
		if (Input.IsValid())
		{
			Input->TryGetStringField(TEXT("comment_text"), CommentText);
			Input->TryGetStringField(TEXT("contains_text"), ContainsText);
		}
		if (!CommentText.IsEmpty() || !ContainsText.IsEmpty())
		{
			const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
			for (int32 Index = 0; Index < Comments.Num(); ++Index)
			{
				UMaterialExpressionComment* Comment = Comments[Index];
				if (!Comment)
				{
					continue;
				}

				const bool bMatchText = !CommentText.IsEmpty() && Comment->Text.Equals(CommentText, ESearchCase::IgnoreCase);
				const bool bMatchContains = !ContainsText.IsEmpty() && Comment->Text.Contains(ContainsText, ESearchCase::IgnoreCase);
				if (bMatchText || bMatchContains)
				{
					OutFocusCommentIndices.Add(Index);
				}
			}

			if (OutFocusCommentIndices.Num() > 0)
			{
				bool bInitialized = false;
				for (const int32 Index : OutFocusCommentIndices)
				{
					UMaterialExpressionComment* Comment = Comments[Index];
					const FBox2D CommentBounds(
						FVector2D(Comment->MaterialExpressionEditorX - Padding, Comment->MaterialExpressionEditorY - Padding),
						FVector2D(Comment->MaterialExpressionEditorX + Comment->SizeX + Padding, Comment->MaterialExpressionEditorY + Comment->SizeY + Padding));
					if (!bInitialized)
					{
						OutBounds = CommentBounds;
						bInitialized = true;
					}
					else
					{
						OutBounds += CommentBounds.Min;
						OutBounds += CommentBounds.Max;
					}
				}
				OutFocusSource = CommentText.IsEmpty() ? TEXT("contains_text") : TEXT("comment_text");
				return true;
			}
		}

		FString FocusQuery;
		if (Input.IsValid())
		{
			Input->TryGetStringField(TEXT("focus_query"), FocusQuery);
		}
		if (!FocusQuery.IsEmpty() && ResolveMaterialExpressionsFromQuery(Material, FocusQuery, OutFocusExpressionIndices, OutFocusCommentIndices))
		{
			bool bHasAny = false;
			if (OutFocusExpressionIndices.Num() > 0)
			{
				bHasAny = ComputeMaterialFocusBoundsFromExpressions(Material, OutFocusExpressionIndices, Padding, OutBounds);
			}

			if (OutFocusCommentIndices.Num() > 0)
			{
				const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
				for (const int32 Index : OutFocusCommentIndices)
				{
					if (!Comments.IsValidIndex(Index) || !Comments[Index])
					{
						continue;
					}

					const FVector2D CommentMin(Comments[Index]->MaterialExpressionEditorX - Padding, Comments[Index]->MaterialExpressionEditorY - Padding);
					const FVector2D CommentMax(Comments[Index]->MaterialExpressionEditorX + Comments[Index]->SizeX + Padding, Comments[Index]->MaterialExpressionEditorY + Comments[Index]->SizeY + Padding);
					if (!bHasAny)
					{
						OutBounds = FBox2D(CommentMin, CommentMax);
						bHasAny = true;
					}
					else
					{
						OutBounds += CommentMin;
						OutBounds += CommentMax;
					}
				}
			}

			if (bHasAny)
			{
				OutFocusSource = TEXT("focus_query");
				return true;
			}
		}

		if (ExistingState && ExistingState->bHasFocusRect)
		{
			OutBounds = ExistingState->FocusRect;
			OutFocusSource = TEXT("view_state");
			return true;
		}

		if (ComputeFullMaterialGraphBounds(Material, OutBounds))
		{
			OutBounds.Min -= FVector2D(Padding, Padding);
			OutBounds.Max += FVector2D(Padding, Padding);
			OutFocusSource = TEXT("full_graph");
			return true;
		}

		return false;
	}

	static FLinearColor GetMaterialNodeCategoryColor(const FString& Category)
	{
		if (Category.Equals(TEXT("parameter"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.24f, 0.36f, 0.14f, 1.0f);
		}
		if (Category.Equals(TEXT("input"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.12f, 0.25f, 0.42f, 1.0f);
		}
		if (Category.Equals(TEXT("math"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.18f, 0.18f, 0.18f, 1.0f);
		}
		return FLinearColor(0.22f, 0.22f, 0.22f, 1.0f);
	}

	static FExpressionInput* ResolveMaterialRootInput(UMaterial* Material, const FString& TargetPin);

	static TArray<FString> GetDefaultMaterialCleanupRootPins()
	{
		return {
			TEXT("BaseColor"),
			TEXT("EmissiveColor"),
			TEXT("AmbientOcclusion"),
			TEXT("Opacity"),
			TEXT("OpacityMask"),
			TEXT("Normal"),
			TEXT("Roughness"),
			TEXT("Metallic"),
			TEXT("Specular"),
			TEXT("WorldPositionOffset"),
			TEXT("MaterialAttributes")
		};
	}

	static FLinearColor GetMaterialCleanupLaneColor(const FString& LaneName)
	{
		if (LaneName.Equals(TEXT("Shared Inputs"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.32f, 0.28f, 0.25f, 1.0f);
		}
		if (LaneName.Equals(TEXT("BaseColor"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.34f, 0.34f, 0.34f, 1.0f);
		}
		if (LaneName.Equals(TEXT("EmissiveColor"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.58f, 0.18f, 0.04f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Opacity"), ESearchCase::IgnoreCase) || LaneName.Equals(TEXT("OpacityMask"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.52f, 0.08f, 0.08f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Normal"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.07f, 0.34f, 0.48f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Roughness"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.08f, 0.42f, 0.10f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Metallic"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.10f, 0.22f, 0.58f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Specular"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.40f, 0.36f, 0.30f, 1.0f);
		}
		if (LaneName.Equals(TEXT("WorldPositionOffset"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.30f, 0.16f, 0.42f, 1.0f);
		}
		if (LaneName.Equals(TEXT("Loose / Utility"), ESearchCase::IgnoreCase))
		{
			return FLinearColor(0.18f, 0.18f, 0.18f, 1.0f);
		}
		return FLinearColor(0.24f, 0.24f, 0.24f, 1.0f);
	}

	static int32 FindMaterialCommentIndexByExactText(UMaterial* Material, const FString& CommentText)
	{
		if (!Material || CommentText.IsEmpty())
		{
			return INDEX_NONE;
		}

		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		for (int32 CommentIndex = 0; CommentIndex < Comments.Num(); ++CommentIndex)
		{
			if (Comments[CommentIndex] && Comments[CommentIndex]->Text.Equals(CommentText, ESearchCase::CaseSensitive))
			{
				return CommentIndex;
			}
		}

		return INDEX_NONE;
	}

	static void CollectMaterialUpstreamExpressionIndices(
		UMaterialExpression* Expression,
		const TMap<UMaterialExpression*, int32>& ExpressionToIndex,
		TSet<int32>& OutIndices)
	{
		if (!Expression)
		{
			return;
		}

		const int32* ExpressionIndex = ExpressionToIndex.Find(Expression);
		if (!ExpressionIndex)
		{
			return;
		}

		if (OutIndices.Contains(*ExpressionIndex))
		{
			return;
		}

		OutIndices.Add(*ExpressionIndex);

		for (FExpressionInputIterator It{ Expression }; It; ++It)
		{
			FExpressionInput* ExprInput = It.Input;
			if (ExprInput && ExprInput->Expression)
			{
				CollectMaterialUpstreamExpressionIndices(ExprInput->Expression, ExpressionToIndex, OutIndices);
			}
		}
	}

	static int32 ComputeMaterialCleanupDepth(
		UMaterialExpression* Expression,
		const TSet<int32>& AllowedIndices,
		const TMap<UMaterialExpression*, int32>& ExpressionToIndex,
		TMap<int32, int32>& InOutDepthCache)
	{
		if (!Expression)
		{
			return 0;
		}

		const int32* ExpressionIndex = ExpressionToIndex.Find(Expression);
		if (!ExpressionIndex)
		{
			return 0;
		}

		if (!AllowedIndices.Contains(*ExpressionIndex))
		{
			return 0;
		}

		if (const int32* ExistingDepth = InOutDepthCache.Find(*ExpressionIndex))
		{
			return *ExistingDepth;
		}

		int32 MaxInputDepth = 0;
		for (FExpressionInputIterator It{ Expression }; It; ++It)
		{
			FExpressionInput* ExprInput = It.Input;
			if (ExprInput && ExprInput->Expression)
			{
				const int32 InputDepth = ComputeMaterialCleanupDepth(ExprInput->Expression, AllowedIndices, ExpressionToIndex, InOutDepthCache);
				MaxInputDepth = FMath::Max(MaxInputDepth, InputDepth + 1);
			}
		}

		InOutDepthCache.Add(*ExpressionIndex, MaxInputDepth);
		return MaxInputDepth;
	}

	static void CollectMaterialGraphVisualData(
		UMaterial* Material,
		TArray<FMaterialGraphVisualNode>& OutNodes,
		TArray<FMaterialGraphVisualComment>& OutComments,
		TArray<FMaterialGraphVisualLink>& OutLinks)
	{
		OutNodes.Reset();
		OutComments.Reset();
		OutLinks.Reset();

		if (!Material)
		{
			return;
		}

		TMap<const UMaterialExpression*, int32> ExpressionToIndex;
		for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			if (UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex])
			{
				ExpressionToIndex.Add(Expr, ExpressionIndex);

				FMaterialGraphVisualNode& Node = OutNodes.AddDefaulted_GetRef();
				Node.ExpressionIndex = ExpressionIndex;
				Node.Label = FString::Printf(TEXT("[%d] %s"), ExpressionIndex, *GetMaterialExpressionPracticalName(Expr));
				Node.Category = GetMaterialExpressionCategory(Expr->GetClass()->GetName());
				Node.Position = FVector2D(Expr->MaterialExpressionEditorX, Expr->MaterialExpressionEditorY);
				Node.Size = FVector2D(260.0f, 180.0f);
			}
		}

		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		for (int32 CommentIndex = 0; CommentIndex < Comments.Num(); ++CommentIndex)
		{
			if (!Comments[CommentIndex])
			{
				continue;
			}

			FMaterialGraphVisualComment& Comment = OutComments.AddDefaulted_GetRef();
			Comment.CommentIndex = CommentIndex;
			Comment.Text = Comments[CommentIndex]->Text;
			Comment.Position = FVector2D(Comments[CommentIndex]->MaterialExpressionEditorX, Comments[CommentIndex]->MaterialExpressionEditorY);
			Comment.Size = FVector2D(Comments[CommentIndex]->SizeX, Comments[CommentIndex]->SizeY);
			Comment.Color = Comments[CommentIndex]->CommentColor;
		}

		for (int32 TargetIndex = 0; TargetIndex < Material->GetExpressionCollection().Expressions.Num(); ++TargetIndex)
		{
			UMaterialExpression* TargetExpr = Material->GetExpressionCollection().Expressions[TargetIndex];
			if (!TargetExpr)
			{
				continue;
			}

			for (TFieldIterator<FProperty> PropIt(TargetExpr->GetClass()); PropIt; ++PropIt)
			{
				FProperty* Property = *PropIt;
				if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
				{
					if (StructProp->Struct && StructProp->Struct->GetFName() == FName(TEXT("ExpressionInput")))
					{
						void* ValuePtr = StructProp->ContainerPtrToValuePtr<void>(TargetExpr);
						const FExpressionInput* ExprInput = reinterpret_cast<const FExpressionInput*>(ValuePtr);
						if (ExprInput && ExprInput->Expression)
						{
							const int32* SourceIndex = ExpressionToIndex.Find(ExprInput->Expression);
							if (SourceIndex)
							{
								FMaterialGraphVisualLink& Link = OutLinks.AddDefaulted_GetRef();
								Link.SourceExpressionIndex = *SourceIndex;
								Link.TargetExpressionIndex = TargetIndex;
								Link.InputName = Property->GetName();
							}
						}
					}
				}
			}
		}
	}

	class SMaterialGraphCaptureWidget final : public SLeafWidget
	{
	public:
		SLATE_BEGIN_ARGS(SMaterialGraphCaptureWidget) {}
			SLATE_ARGUMENT(TArray<FMaterialGraphVisualNode>, Nodes)
			SLATE_ARGUMENT(TArray<FMaterialGraphVisualComment>, Comments)
			SLATE_ARGUMENT(TArray<FMaterialGraphVisualLink>, Links)
			SLATE_ARGUMENT(FBox2D, ViewRect)
			SLATE_ARGUMENT(FVector2D, CaptureSize)
			SLATE_ARGUMENT(float, Zoom)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			Nodes = InArgs._Nodes;
			Comments = InArgs._Comments;
			Links = InArgs._Links;
			ViewRect = InArgs._ViewRect;
			CaptureSize = InArgs._CaptureSize;
			Zoom = InArgs._Zoom;
		}

		virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
			FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
		{
			const FSlateBrush* WhiteBrush = FAppStyle::GetBrush(TEXT("WhiteBrush"));
			const FSlateFontInfo SmallFont = FAppStyle::GetFontStyle(TEXT("NormalFont"));
			const FSlateFontInfo TinyFont = FAppStyle::GetFontStyle(TEXT("SmallFont"));

			auto MakeOffsetGeometry = [&AllottedGeometry](const FVector2D& LocalOffset, const FVector2D& LocalSize) -> FPaintGeometry
			{
				return AllottedGeometry.ToPaintGeometry(
					FVector2f(static_cast<float>(LocalSize.X), static_cast<float>(LocalSize.Y)),
					FSlateLayoutTransform(FVector2f(static_cast<float>(LocalOffset.X), static_cast<float>(LocalOffset.Y))));
			};

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId++,
				MakeOffsetGeometry(FVector2D::ZeroVector, CaptureSize),
				WhiteBrush,
				ESlateDrawEffect::None,
				FLinearColor(0.07f, 0.07f, 0.07f, 1.0f));

			const float GridStep = FMath::Max(40.0f, 120.0f * Zoom);
			for (float X = 0.0f; X < CaptureSize.X; X += GridStep)
			{
				const TArray<FVector2D> Points{ FVector2D(X, 0.0f), FVector2D(X, CaptureSize.Y) };
				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points, ESlateDrawEffect::None, FLinearColor(0.14f, 0.14f, 0.14f, 1.0f), true, 1.0f);
			}
			for (float Y = 0.0f; Y < CaptureSize.Y; Y += GridStep)
			{
				const TArray<FVector2D> Points{ FVector2D(0.0f, Y), FVector2D(CaptureSize.X, Y) };
				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points, ESlateDrawEffect::None, FLinearColor(0.14f, 0.14f, 0.14f, 1.0f), true, 1.0f);
			}
			LayerId++;

			auto ToLocal = [this](const FVector2D& GraphPoint) -> FVector2D
			{
				return FVector2D(
					(GraphPoint.X - ViewRect.Min.X) * Zoom,
					(GraphPoint.Y - ViewRect.Min.Y) * Zoom);
			};

			for (const FMaterialGraphVisualComment& Comment : Comments)
			{
				const FVector2D CommentPos = ToLocal(Comment.Position);
				const FVector2D CommentSize = Comment.Size * Zoom;
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					MakeOffsetGeometry(CommentPos, CommentSize),
					WhiteBrush,
					ESlateDrawEffect::None,
					FLinearColor(Comment.Color.R, Comment.Color.G, Comment.Color.B, 0.22f));
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId + 1,
					MakeOffsetGeometry(CommentPos, CommentSize),
					WhiteBrush,
					ESlateDrawEffect::None,
					FLinearColor(Comment.Color.R, Comment.Color.G, Comment.Color.B, 0.12f));
				FSlateDrawElement::MakeText(
					OutDrawElements,
					LayerId + 2,
					MakeOffsetGeometry(CommentPos + FVector2D(10.0f, 6.0f), FVector2D(1.0f, 1.0f)),
					Comment.Text,
					SmallFont,
					ESlateDrawEffect::None,
					FLinearColor::White);
			}
			LayerId += 3;

			TMap<int32, const FMaterialGraphVisualNode*> NodeLookup;
			for (const FMaterialGraphVisualNode& Node : Nodes)
			{
				NodeLookup.Add(Node.ExpressionIndex, &Node);
			}

			for (const FMaterialGraphVisualLink& Link : Links)
			{
				const FMaterialGraphVisualNode* const* SourceNodePtr = NodeLookup.Find(Link.SourceExpressionIndex);
				const FMaterialGraphVisualNode* const* TargetNodePtr = NodeLookup.Find(Link.TargetExpressionIndex);
				if (!SourceNodePtr || !TargetNodePtr)
				{
					continue;
				}

				const FVector2D SourcePos = ToLocal((*SourceNodePtr)->Position + FVector2D((*SourceNodePtr)->Size.X, (*SourceNodePtr)->Size.Y * 0.5f));
				const FVector2D TargetPos = ToLocal((*TargetNodePtr)->Position + FVector2D(0.0f, (*TargetNodePtr)->Size.Y * 0.5f));
				const float MidX = FMath::Lerp(SourcePos.X, TargetPos.X, 0.5f);
				const TArray<FVector2D> Points{
					SourcePos,
					FVector2D(MidX, SourcePos.Y),
					FVector2D(MidX, TargetPos.Y),
					TargetPos
				};
				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points, ESlateDrawEffect::None, FLinearColor(0.82f, 0.82f, 0.82f, 1.0f), true, 2.0f);
			}
			LayerId++;

			for (const FMaterialGraphVisualNode& Node : Nodes)
			{
				const FVector2D NodePos = ToLocal(Node.Position);
				const FVector2D NodeSize = Node.Size * Zoom;
				const FLinearColor NodeColor = GetMaterialNodeCategoryColor(Node.Category);
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					MakeOffsetGeometry(NodePos, NodeSize),
					WhiteBrush,
					ESlateDrawEffect::None,
					NodeColor);
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId + 1,
					MakeOffsetGeometry(NodePos, NodeSize),
					WhiteBrush,
					ESlateDrawEffect::None,
					FLinearColor(0.0f, 0.0f, 0.0f, 0.18f));

				FSlateDrawElement::MakeText(
					OutDrawElements,
					LayerId + 2,
					MakeOffsetGeometry(NodePos + FVector2D(8.0f, 8.0f), FVector2D(1.0f, 1.0f)),
					Node.Label.Left(80),
					SmallFont,
					ESlateDrawEffect::None,
					FLinearColor::White);

				FSlateDrawElement::MakeText(
					OutDrawElements,
					LayerId + 3,
					MakeOffsetGeometry(NodePos + FVector2D(8.0f, 30.0f), FVector2D(1.0f, 1.0f)),
					Node.Category,
					TinyFont,
					ESlateDrawEffect::None,
					FLinearColor(0.85f, 0.85f, 0.85f, 1.0f));
			}

			return LayerId + 4;
		}

		virtual FVector2D ComputeDesiredSize(float) const override
		{
			return CaptureSize;
		}

	private:
		TArray<FMaterialGraphVisualNode> Nodes;
		TArray<FMaterialGraphVisualComment> Comments;
		TArray<FMaterialGraphVisualLink> Links;
		FBox2D ViewRect = FBox2D(EForceInit::ForceInit);
		FVector2D CaptureSize = FVector2D(1800.0f, 1000.0f);
		float Zoom = 1.0f;
	};

	static bool CaptureSlateWidgetToPng(const TSharedRef<SWidget>& Widget, const FVector2D& CaptureSize, const FString& OutputPath, FString& OutError)
	{
		TSharedRef<SWindow> Window = SNew(SWindow)
			.AutoCenter(EAutoCenter::PreferredWorkArea)
			.SizingRule(ESizingRule::FixedSize)
			.ClientSize(CaptureSize)
			.ScreenPosition(FVector2D(80.0f, 80.0f))
			.SupportsMinimize(false)
			.SupportsMaximize(false)
			.IsTopmostWindow(false)
			.FocusWhenFirstShown(false)
			.CreateTitleBar(false);
		Window->SetContent(Widget);

		FSlateApplication& SlateApp = FSlateApplication::Get();
		SlateApp.AddWindow(Window, true);
		Widget->SlatePrepass();
		for (int32 Attempt = 0; Attempt < 3; ++Attempt)
		{
			SlateApp.PumpMessages();
			SlateApp.Tick();
			SlateApp.ForceRedrawWindow(Window);
			FlushRenderingCommands();
			FPlatformProcess::Sleep(0.05f);
		}

		TArray<FColor> Bitmap;
		FIntVector Size = FIntVector::ZeroValue;
		bool bCaptured = SlateApp.TakeScreenshot(Widget, Bitmap, Size);
		if ((!bCaptured || Size.X <= 0 || Size.Y <= 0))
		{
			Bitmap.Reset();
			Size = FIntVector::ZeroValue;
			bCaptured = SlateApp.TakeScreenshot(Window, Bitmap, Size);
		}
		Window->RequestDestroyWindow();

		if (!bCaptured || Size.X <= 0 || Size.Y <= 0)
		{
			OutError = TEXT("Failed to capture temporary Slate graph window.");
			return false;
		}

		return SaveColorBufferToPngFile(Bitmap, Size.X, Size.Y, OutputPath, OutError);
	}

	static void BlendPixel(TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const int32 X, const int32 Y, const FColor& Color, const float Alpha = 1.0f)
	{
		if (X < 0 || Y < 0 || X >= Width || Y >= Height)
		{
			return;
		}

		const int32 Index = Y * Width + X;
		FColor& Dest = Bitmap[Index];
		const float ClampedAlpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		Dest.R = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Dest.R + (Color.R - Dest.R) * ClampedAlpha), 0, 255));
		Dest.G = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Dest.G + (Color.G - Dest.G) * ClampedAlpha), 0, 255));
		Dest.B = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Dest.B + (Color.B - Dest.B) * ClampedAlpha), 0, 255));
		Dest.A = 255;
	}

	static void DrawFilledRect(TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const FIntRect& Rect, const FColor& Color, const float Alpha = 1.0f)
	{
		const int32 MinX = FMath::Clamp(Rect.Min.X, 0, Width);
		const int32 MinY = FMath::Clamp(Rect.Min.Y, 0, Height);
		const int32 MaxX = FMath::Clamp(Rect.Max.X, 0, Width);
		const int32 MaxY = FMath::Clamp(Rect.Max.Y, 0, Height);
		for (int32 Y = MinY; Y < MaxY; ++Y)
		{
			for (int32 X = MinX; X < MaxX; ++X)
			{
				BlendPixel(Bitmap, Width, Height, X, Y, Color, Alpha);
			}
		}
	}

	static void DrawRectOutline(TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const FIntRect& Rect, const FColor& Color)
	{
		for (int32 X = Rect.Min.X; X < Rect.Max.X; ++X)
		{
			BlendPixel(Bitmap, Width, Height, X, Rect.Min.Y, Color, 1.0f);
			BlendPixel(Bitmap, Width, Height, X, Rect.Max.Y - 1, Color, 1.0f);
		}
		for (int32 Y = Rect.Min.Y; Y < Rect.Max.Y; ++Y)
		{
			BlendPixel(Bitmap, Width, Height, Rect.Min.X, Y, Color, 1.0f);
			BlendPixel(Bitmap, Width, Height, Rect.Max.X - 1, Y, Color, 1.0f);
		}
	}

	static void DrawLine(TArray<FColor>& Bitmap, const int32 Width, const int32 Height, const FVector2D& A, const FVector2D& B, const FColor& Color, const float Alpha = 1.0f)
	{
		const float DeltaX = B.X - A.X;
		const float DeltaY = B.Y - A.Y;
		const int32 Steps = FMath::Max(1, FMath::RoundToInt(FMath::Max(FMath::Abs(DeltaX), FMath::Abs(DeltaY))));
		for (int32 Step = 0; Step <= Steps; ++Step)
		{
			const float T = static_cast<float>(Step) / static_cast<float>(Steps);
			const int32 X = FMath::RoundToInt(FMath::Lerp(A.X, B.X, T));
			const int32 Y = FMath::RoundToInt(FMath::Lerp(A.Y, B.Y, T));
			BlendPixel(Bitmap, Width, Height, X, Y, Color, Alpha);
			BlendPixel(Bitmap, Width, Height, X + 1, Y, Color, Alpha * 0.7f);
			BlendPixel(Bitmap, Width, Height, X, Y + 1, Color, Alpha * 0.7f);
		}
	}

	static bool RenderMaterialGraphVisualFallbackToPng(
		const TArray<FMaterialGraphVisualNode>& Nodes,
		const TArray<FMaterialGraphVisualComment>& Comments,
		const TArray<FMaterialGraphVisualLink>& Links,
		const FBox2D& ViewRect,
		const int32 ImageWidth,
		const int32 ImageHeight,
		const float Zoom,
		const FString& OutputPath,
		FString& OutError)
	{
		if (ImageWidth <= 0 || ImageHeight <= 0 || !ViewRect.bIsValid)
		{
			OutError = TEXT("Invalid fallback graph render dimensions.");
			return false;
		}

		TArray<FColor> Bitmap;
		Bitmap.Init(FColor(28, 28, 28, 255), ImageWidth * ImageHeight);

		const FVector2D ViewSize = ViewRect.Max - ViewRect.Min;
		const FVector2D ContentSize(FMath::Max(1.0f, ViewSize.X * Zoom), FMath::Max(1.0f, ViewSize.Y * Zoom));
		const FVector2D Offset(
			FMath::Max(0.0f, (static_cast<float>(ImageWidth) - ContentSize.X) * 0.5f),
			FMath::Max(0.0f, (static_cast<float>(ImageHeight) - ContentSize.Y) * 0.5f));

		auto ToScreen = [&](const FVector2D& Point) -> FVector2D
		{
			return FVector2D(
				(Point.X - ViewRect.Min.X) * Zoom + Offset.X,
				(Point.Y - ViewRect.Min.Y) * Zoom + Offset.Y);
		};

		for (int32 X = 0; X < ImageWidth; X += 40)
		{
			for (int32 Y = 0; Y < ImageHeight; ++Y)
			{
				BlendPixel(Bitmap, ImageWidth, ImageHeight, X, Y, FColor(44, 44, 44, 255), 0.6f);
			}
		}
		for (int32 Y = 0; Y < ImageHeight; Y += 40)
		{
			for (int32 X = 0; X < ImageWidth; ++X)
			{
				BlendPixel(Bitmap, ImageWidth, ImageHeight, X, Y, FColor(44, 44, 44, 255), 0.6f);
			}
		}

		for (const FMaterialGraphVisualComment& Comment : Comments)
		{
			const FVector2D Min = ToScreen(Comment.Position);
			const FVector2D Max = ToScreen(Comment.Position + Comment.Size);
			const FColor Fill(
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Comment.Color.R * 255.0f), 0, 255)),
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Comment.Color.G * 255.0f), 0, 255)),
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Comment.Color.B * 255.0f), 0, 255)),
				255);
			const FIntRect Rect(
				FMath::RoundToInt(Min.X),
				FMath::RoundToInt(Min.Y),
				FMath::RoundToInt(Max.X),
				FMath::RoundToInt(Max.Y));
			DrawFilledRect(Bitmap, ImageWidth, ImageHeight, Rect, Fill, 0.18f);
			DrawRectOutline(Bitmap, ImageWidth, ImageHeight, Rect, Fill);
		}

		for (const FMaterialGraphVisualLink& Link : Links)
		{
			const FMaterialGraphVisualNode* SourceNode = Nodes.FindByPredicate([&](const FMaterialGraphVisualNode& Node)
			{
				return Node.ExpressionIndex == Link.SourceExpressionIndex;
			});
			const FMaterialGraphVisualNode* TargetNode = Nodes.FindByPredicate([&](const FMaterialGraphVisualNode& Node)
			{
				return Node.ExpressionIndex == Link.TargetExpressionIndex;
			});
			if (!SourceNode || !TargetNode)
			{
				continue;
			}

			const FVector2D A = ToScreen(SourceNode->Position + FVector2D(SourceNode->Size.X, SourceNode->Size.Y * 0.5f));
			const FVector2D B = ToScreen(TargetNode->Position + FVector2D(0.0f, TargetNode->Size.Y * 0.5f));
			DrawLine(Bitmap, ImageWidth, ImageHeight, A, B, FColor(210, 210, 210, 255), 0.95f);
		}

		for (const FMaterialGraphVisualNode& Node : Nodes)
		{
			const FVector2D Min = ToScreen(Node.Position);
			const FVector2D Max = ToScreen(Node.Position + Node.Size);
			const FLinearColor CategoryColor = GetMaterialNodeCategoryColor(Node.Category);
			const FColor Fill(
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(CategoryColor.R * 255.0f), 0, 255)),
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(CategoryColor.G * 255.0f), 0, 255)),
				static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(CategoryColor.B * 255.0f), 0, 255)),
				255);
			const FIntRect Rect(
				FMath::RoundToInt(Min.X),
				FMath::RoundToInt(Min.Y),
				FMath::RoundToInt(Max.X),
				FMath::RoundToInt(Max.Y));
			DrawFilledRect(Bitmap, ImageWidth, ImageHeight, Rect, Fill, 0.92f);
			DrawRectOutline(Bitmap, ImageWidth, ImageHeight, Rect, FColor(235, 235, 235, 255));
		}

		return SaveColorBufferToPngFile(Bitmap, ImageWidth, ImageHeight, OutputPath, OutError);
	}

	static void ApplyMaterialExpressionPosition(
		UMaterial* Material,
		UMaterialExpression* Expression,
		const TSharedPtr<FJsonObject>& Input,
		const FString& ExprType,
		TSharedPtr<FJsonObject> ResultObj)
	{
		if (!Expression)
		{
			return;
		}

		int32 EditorX = 0;
		int32 EditorY = 0;
		const bool bHasX = TryReadOptionalIntField(Input, TEXT("editor_x"), EditorX);
		const bool bHasY = TryReadOptionalIntField(Input, TEXT("editor_y"), EditorY);
		if (!(bHasX && bHasY))
		{
			ComputeMaterialAutoPlacement(Material, ExprType, EditorX, EditorY);
		}

		Expression->MaterialExpressionEditorX = EditorX;
		Expression->MaterialExpressionEditorY = EditorY;

		if (ResultObj.IsValid())
		{
			ResultObj->SetNumberField(TEXT("editor_x"), EditorX);
			ResultObj->SetNumberField(TEXT("editor_y"), EditorY);
		}
	}

	static FString GetFunctionInputTypeName(EFunctionInputType InputType)
	{
		const UEnum* EnumType = StaticEnum<EFunctionInputType>();
		return EnumType ? EnumType->GetNameStringByValue(static_cast<int64>(InputType)) : FString::FromInt(static_cast<int32>(InputType));
	}

	static FString GetMaterialExpressionCaptionText(UMaterialExpression* Expression)
	{
		if (!Expression)
		{
			return TEXT("Unknown");
		}

		TArray<FString> Captions;
		Expression->GetCaption(Captions);
		return Captions.Num() > 0 ? FString::Join(Captions, TEXT(" ")) : Expression->GetClass()->GetName();
	}

	static void CollectExpressionInputConnections(
		UMaterialExpression* Expression,
		const TMap<const UMaterialExpression*, int32>& ExpressionToIndex,
		TArray<TSharedPtr<FJsonValue>>& OutInputs,
		TArray<TSharedPtr<FJsonValue>>& OutConnections)
	{
		if (!Expression)
		{
			return;
		}

		auto AppendInput = [&](const FString& InputName, const FExpressionInput& InputValue, const FString& PropertyName)
		{
			TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject());
			InputObj->SetStringField(TEXT("input_name"), InputName);
			InputObj->SetStringField(TEXT("property_name"), PropertyName);
			InputObj->SetBoolField(TEXT("connected"), InputValue.Expression != nullptr);
			InputObj->SetNumberField(TEXT("output_index"), InputValue.OutputIndex);
			InputObj->SetStringField(TEXT("mask_label"),
				FString::Printf(TEXT("%s%s%s%s"),
					InputValue.MaskR ? TEXT("R") : TEXT(""),
					InputValue.MaskG ? TEXT("G") : TEXT(""),
					InputValue.MaskB ? TEXT("B") : TEXT(""),
					InputValue.MaskA ? TEXT("A") : TEXT("")));

			if (InputValue.Expression)
			{
				if (const int32* SourceIndex = ExpressionToIndex.Find(InputValue.Expression))
				{
					InputObj->SetNumberField(TEXT("source_index"), *SourceIndex);
				}
				InputObj->SetStringField(TEXT("source_caption"), GetMaterialExpressionCaptionText(InputValue.Expression));
				InputObj->SetStringField(TEXT("source_class"), InputValue.Expression->GetClass()->GetName());

				TSharedPtr<FJsonObject> ConnectionObj = MakeShareable(new FJsonObject());
				ConnectionObj->SetStringField(TEXT("source_caption"), GetMaterialExpressionCaptionText(InputValue.Expression));
				ConnectionObj->SetStringField(TEXT("target_caption"), GetMaterialExpressionCaptionText(Expression));
				ConnectionObj->SetStringField(TEXT("target_input"), InputName);
				ConnectionObj->SetStringField(TEXT("property_name"), PropertyName);
				ConnectionObj->SetNumberField(TEXT("source_output_index"), InputValue.OutputIndex);
				if (const int32* SourceIndex = ExpressionToIndex.Find(InputValue.Expression))
				{
					ConnectionObj->SetNumberField(TEXT("source_index"), *SourceIndex);
				}
				OutConnections.Add(MakeShareable(new FJsonValueObject(ConnectionObj)));
			}

			OutInputs.Add(MakeShareable(new FJsonValueObject(InputObj)));
		};

		const FName ExpressionInputStructName(TEXT("ExpressionInput"));
		for (TFieldIterator<FProperty> PropIt(Expression->GetClass()); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			if (!Property)
			{
				continue;
			}

			if (FStructProperty* StructProp = CastField<FStructProperty>(Property))
			{
				if (StructProp->Struct && StructProp->Struct->GetFName() == ExpressionInputStructName)
				{
					if (const FExpressionInput* ExprInput = StructProp->ContainerPtrToValuePtr<FExpressionInput>(Expression))
					{
						AppendInput(StructProp->GetName(), *ExprInput, StructProp->GetName());
					}
				}
			}
			else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
			{
				const FStructProperty* InnerStructProp = CastField<FStructProperty>(ArrayProp->Inner);
				if (!InnerStructProp || !InnerStructProp->Struct || InnerStructProp->Struct->GetFName() != ExpressionInputStructName)
				{
					continue;
				}

				FScriptArrayHelper ArrayHelper(ArrayProp, ArrayProp->ContainerPtrToValuePtr<void>(Expression));
				for (int32 ArrayIndex = 0; ArrayIndex < ArrayHelper.Num(); ++ArrayIndex)
				{
					const FExpressionInput* ExprInput = reinterpret_cast<const FExpressionInput*>(ArrayHelper.GetRawPtr(ArrayIndex));
					if (!ExprInput)
					{
						continue;
					}
					AppendInput(
						FString::Printf(TEXT("%s[%d]"), *ArrayProp->GetName(), ArrayIndex),
						*ExprInput,
						ArrayProp->GetName());
				}
			}
		}
	}
}

// ============================================================
// Material Tool Registration
// ============================================================

void FVFXToolRegistry::RegisterMaterialTools()
{
	// PHASE 3: Material Tools
	// ================================================================

	// ---- Tool: create_material ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material");
		Def.Description = TEXT(
			"Create a new Material asset with a specified base color. "
			"Returns the asset path of the created Material.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_name"),
			MakeStringProperty(TEXT("Name for the new Material (e.g. 'M_Fire')")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Content Browser folder path (e.g. '/Game/VFX/Materials/'). Default: '/Game/'")));
		Props->SetObjectField(TEXT("base_color"),
			MakeArrayProperty(TEXT("[R, G, B] values from 0.0 to 1.0. Default: white [1,1,1]"), TEXT("number")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string. Default false (dry-run only). Set true to create the asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterial);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_info");
		Def.Description = TEXT(
			"Get deep information about a Material asset: graph nodes, pin connections, root material inputs, "
			"parameter values, panner/UV settings, custom HLSL node data, and referenced texture details.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material (e.g. '/Game/VFX/M_Fire.M_Fire')")));
		Props->SetObjectField(TEXT("include_custom_code"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). If true, include full Custom node HLSL code.")));
		Props->SetObjectField(TEXT("max_custom_code_chars"),
			MakeStringProperty(TEXT("Optional number string. If include_custom_code is false, return this many chars as code preview (default: 300).")));
		Props->SetObjectField(TEXT("focus_query"),
			MakeStringProperty(TEXT("Optional keyword to focus on a local graph block (matches caption/description/parameter/comment/function names).")));
		Props->SetObjectField(TEXT("focus_expression_index"),
			MakeStringProperty(TEXT("Optional expression index seed for focus subgraph (can be used without focus_query).")));
		Props->SetObjectField(TEXT("focus_hops"),
			MakeStringProperty(TEXT("Optional neighborhood depth from seed nodes (default: 2).")));
		Props->SetObjectField(TEXT("focus_region_center"),
			MakeArrayProperty(TEXT("Optional [X, Y] graph position to seed focus by region."), TEXT("number")));
		Props->SetObjectField(TEXT("focus_region_radius"),
			MakeStringProperty(TEXT("Optional region radius around focus_region_center (default: 1200).")));
		Props->SetObjectField(TEXT("focus_region_shape"),
			MakeStringProperty(TEXT("Optional region shape: 'circle' (default) or 'box'.")));
		Props->SetObjectField(TEXT("focus_region_box"),
			MakeArrayProperty(TEXT("Optional [MinX, MinY, MaxX, MaxY] when using box region focus."), TEXT("number")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialInfo);
		RegisterTool(Def);
	}

	// ---- Tool: compile_material_and_get_diagnostics ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("compile_material_and_get_diagnostics");
		Def.Description = TEXT(
			"Force a real Material/Material Instance shader recompile and return the same compile diagnostics that back Material Editor Stats, "
			"including FMaterialResource compile errors and error expressions.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of a Material or Material Instance (for example '/Game/VFX/M_Fire.M_Fire').")));
		Props->SetObjectField(TEXT("save_asset"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). If true, save the asset after recompiling. Default: false.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CompileMaterialAndGetDiagnostics);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_editor_context ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_editor_context");
		Def.Description = TEXT(
			"Inspect the current Material Editor context: opened Materials, Material Instances, and Material Functions, plus preferred paths for follow-up inspection.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialEditorContext);
		RegisterTool(Def);
	}

	// ---- Tool: focus_material_graph_region ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("focus_material_graph_region");
		Def.Description = TEXT("Resolve and store a Material graph focus region using query, comment, expression, or explicit graph bounds.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("focus_query"), MakeStringProperty(TEXT("Optional keyword to focus a local block.")));
		Props->SetObjectField(TEXT("focus_region_box"), MakeArrayProperty(TEXT("Optional [MinX, MinY, MaxX, MaxY] graph bounds."), TEXT("number")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional expression index to focus.")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text to focus.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text to focus.")));
		Props->SetObjectField(TEXT("padding"), MakeStringProperty(TEXT("Optional graph padding in units. Default: 120.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("material_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FocusMaterialGraphRegion);
		RegisterTool(Def);
	}

	// ---- Tool: focus_material_graph_expression ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("focus_material_graph_expression");
		Def.Description = TEXT("Focus the Material graph view state around a specific expression index.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Expression index from get_material_info.")));
		Props->SetObjectField(TEXT("padding"), MakeStringProperty(TEXT("Optional graph padding in units. Default: 120.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("material_path"))),
			MakeShareable(new FJsonValueString(TEXT("expression_index")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FocusMaterialGraphExpression);
		RegisterTool(Def);
	}

	// ---- Tool: focus_material_graph_comment ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("focus_material_graph_comment");
		Def.Description = TEXT("Focus the Material graph view state around a specific comment region.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("comment_index"), MakeStringProperty(TEXT("Optional comment index from get_material_info.")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text.")));
		Props->SetObjectField(TEXT("padding"), MakeStringProperty(TEXT("Optional graph padding in units. Default: 120.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("material_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FocusMaterialGraphComment);
		RegisterTool(Def);
	}

	// ---- Tool: pan_material_graph ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("pan_material_graph");
		Def.Description = TEXT("Pan the stored Material graph view state by graph-unit delta.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("delta_x"), MakeStringProperty(TEXT("Horizontal pan delta in graph units.")));
		Props->SetObjectField(TEXT("delta_y"), MakeStringProperty(TEXT("Vertical pan delta in graph units.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("material_path"))),
			MakeShareable(new FJsonValueString(TEXT("delta_x"))),
			MakeShareable(new FJsonValueString(TEXT("delta_y")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_PanMaterialGraph);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_graph_zoom ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_graph_zoom");
		Def.Description = TEXT("Set the stored Material graph capture zoom. Use 0 to return to fit-to-region capture.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("zoom"), MakeStringProperty(TEXT("Target zoom scalar. Use 0 for auto-fit.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("material_path"))),
			MakeShareable(new FJsonValueString(TEXT("zoom")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialGraphZoom);
		RegisterTool(Def);
	}

	// ---- Tool: capture_material_graph_screenshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("capture_material_graph_screenshot");
		Def.Description = TEXT("Render a focused Material graph capture to a PNG under Downloads for review and self-check.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("focus_query"), MakeStringProperty(TEXT("Optional keyword to focus a local block.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional expression index to focus.")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text to focus.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text to focus.")));
		Props->SetObjectField(TEXT("focus_region_box"), MakeArrayProperty(TEXT("Optional [MinX, MinY, MaxX, MaxY] graph bounds."), TEXT("number")));
		Props->SetObjectField(TEXT("output_dir"), MakeStringProperty(TEXT("Optional absolute output directory. Defaults to Downloads/ForgePilotMaterialProbes/...")));
		Props->SetObjectField(TEXT("file_name"), MakeStringProperty(TEXT("Optional file name. Defaults to material_graph.png.")));
		Props->SetObjectField(TEXT("image_width"), MakeStringProperty(TEXT("Optional capture width. Default: 1800.")));
		Props->SetObjectField(TEXT("image_height"), MakeStringProperty(TEXT("Optional capture height. Default: 1000.")));
		Props->SetObjectField(TEXT("padding"), MakeStringProperty(TEXT("Optional focus padding in graph units. Default: 120.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("material_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CaptureMaterialGraphScreenshot);
		RegisterTool(Def);
	}

	// ---- Tool: validate_material_graph_block ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("validate_material_graph_block");
		Def.Description = TEXT("Validate a Material graph block for broken wiring, missing math inputs, missing comments, and root pin coverage.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("focus_query"), MakeStringProperty(TEXT("Optional keyword to validate a local block.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional expression index seed.")));
		Props->SetObjectField(TEXT("expression_indices"), MakeArrayProperty(TEXT("Optional explicit expression indices."), TEXT("number")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text to validate.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text to validate.")));
		Props->SetObjectField(TEXT("required_root_pins"), MakeArrayProperty(TEXT("Optional root pins that must be connected by this block."), TEXT("string")));
		Props->SetObjectField(TEXT("require_comment"), MakeStringProperty(TEXT("Optional bool. Default: true for focused block validation.")));
		Props->SetObjectField(TEXT("expect_uv_chain"), MakeStringProperty(TEXT("Optional bool. If true, warn when texture nodes have no UV expression.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("material_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ValidateMaterialGraphBlock);
		RegisterTool(Def);
	}

	// ---- Tool: summarize_material_block_wiring ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("summarize_material_block_wiring");
		Def.Description = TEXT("Summarize internal, incoming, and outgoing wiring for a focused Material graph block.");

		TSharedPtr<FJsonObject> Schema = MakeEmptyObjectSchema();
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("focus_query"), MakeStringProperty(TEXT("Optional keyword to summarize a local block.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional expression index seed.")));
		Props->SetObjectField(TEXT("expression_indices"), MakeArrayProperty(TEXT("Optional explicit expression indices."), TEXT("number")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text to summarize.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text to summarize.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), { MakeShareable(new FJsonValueString(TEXT("material_path"))) });

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SummarizeMaterialBlockWiring);
		RegisterTool(Def);
	}

	// ---- Tool: create_material_test_instance_set ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material_test_instance_set");
		Def.Description = TEXT(
			"Create a small set of Material Instances for visual tuning from one parent material. "
			"Use variants or one of the built-in presets for MI-first lookdev.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("parent_material_path"), MakeStringProperty(TEXT("Full asset path of the parent Material.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Content Browser folder path. Default: /Game/ForgePilot_Audit")));
		Props->SetObjectField(TEXT("name_prefix"), MakeStringProperty(TEXT("Optional MI name prefix.")));
		Props->SetObjectField(TEXT("preset"), MakeStringProperty(TEXT("Optional preset name such as fire_person_erosion.")));
		Props->SetObjectField(TEXT("variants"), MakeArrayProperty(TEXT("Optional array of variant objects. Each entry may contain name_suffix, scalar_overrides, vector_overrides, texture_overrides."), TEXT("object")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("parent_material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterialTestInstanceSet);
		RegisterTool(Def);
	}

	// ---- Tool: sweep_material_instance_parameters ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("sweep_material_instance_parameters");
		Def.Description = TEXT(
			"Create a sweep of Material Instance variants from one parent material by varying scalar, vector, or texture parameters.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("parent_material_path"), MakeStringProperty(TEXT("Full asset path of the parent Material.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Content Browser folder path. Default: /Game/ForgePilot_Audit")));
		Props->SetObjectField(TEXT("name_prefix"), MakeStringProperty(TEXT("Optional MI name prefix.")));
		Props->SetObjectField(TEXT("scalar_sweeps"), MakeStringProperty(TEXT("Optional JSON object, e.g. {\"Burn_Threshold\":[0.2,0.4,0.6]}")));
		Props->SetObjectField(TEXT("vector_sweeps"), MakeStringProperty(TEXT("Optional JSON object, e.g. {\"Fire_Color\":[[1,0.5,0,1],[1,0.8,0.2,1]]}")));
		Props->SetObjectField(TEXT("texture_sweeps"), MakeStringProperty(TEXT("Optional JSON object, e.g. {\"Burn_NoiseTex\":[\"/Game/T_A\",\"/Game/T_B\"]}")));
		Props->SetObjectField(TEXT("base_scalar_overrides"), MakeStringProperty(TEXT("Optional JSON object of shared scalar overrides.")));
		Props->SetObjectField(TEXT("base_vector_overrides"), MakeStringProperty(TEXT("Optional JSON object of shared vector overrides.")));
		Props->SetObjectField(TEXT("base_texture_overrides"), MakeStringProperty(TEXT("Optional JSON object of shared texture overrides.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("parent_material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SweepMaterialInstanceParameters);
		RegisterTool(Def);
	}

	// ---- Tool: spawn_material_probe_actor ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("spawn_material_probe_actor");
		Def.Description = TEXT(
			"Spawn a simple static mesh actor in the editor world for Material preview captures.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"), MakeStringProperty(TEXT("Optional actor label. Default: ForgePilot_MaterialProbe")));
		Props->SetObjectField(TEXT("mesh_path"), MakeStringProperty(TEXT("Optional static mesh asset path. Default: /Engine/BasicShapes/Sphere.Sphere")));
		Props->SetObjectField(TEXT("location"), MakeArrayProperty(TEXT("Optional [X,Y,Z] world location."), TEXT("number")));
		Props->SetObjectField(TEXT("rotation"), MakeArrayProperty(TEXT("Optional [Pitch,Yaw,Roll] rotation."), TEXT("number")));
		Props->SetObjectField(TEXT("scale"), MakeArrayProperty(TEXT("Optional [X,Y,Z] scale."), TEXT("number")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SpawnMaterialProbeActor);
		RegisterTool(Def);
	}

	// ---- Tool: apply_material_to_probe_actor ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("apply_material_to_probe_actor");
		Def.Description = TEXT("Apply a Material or Material Instance to a probe actor.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"), MakeStringProperty(TEXT("Exact actor label of the probe actor.")));
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material or Material Instance.")));
		Props->SetObjectField(TEXT("material_index"), MakeStringProperty(TEXT("Optional material slot index. Default: 0.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ApplyMaterialToProbeActor);
		RegisterTool(Def);
	}

	// ---- Tool: frame_probe_actor_for_capture ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("frame_probe_actor_for_capture");
		Def.Description = TEXT("Frame a probe actor in the active level viewport to prepare a deterministic screenshot.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"), MakeStringProperty(TEXT("Exact actor label to frame.")));
		Props->SetObjectField(TEXT("ensure_realtime"), MakeStringProperty(TEXT("Optional bool string. Default true. Forces realtime on before reporting viewport state.")));
		Props->SetObjectField(TEXT("wait_ms"), MakeStringProperty(TEXT("Optional integer wait after framing. Default 100.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_FrameProbeActorForCapture);
		RegisterTool(Def);
	}

	// ---- Tool: capture_material_visual_probe ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("capture_material_visual_probe");
		Def.Description = TEXT(
			"Capture a Material probe preview and save it to Downloads/ForgePilotMaterialProbes. Actor-based captures default to thumbnail-preview style for closer parity with editor material thumbnails.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"), MakeStringProperty(TEXT("Optional probe actor label to frame before capture.")));
		Props->SetObjectField(TEXT("output_dir"), MakeStringProperty(TEXT("Optional absolute output directory. Defaults to Downloads/ForgePilotMaterialProbes/...")));
		Props->SetObjectField(TEXT("file_name"), MakeStringProperty(TEXT("Optional output filename. Default: material_probe.jpg")));
		Props->SetObjectField(TEXT("frame_actor"), MakeStringProperty(TEXT("Optional bool string. Default: true when actor_label is provided.")));
		Props->SetObjectField(TEXT("wait_ms"), MakeStringProperty(TEXT("Optional wait time after framing. Default: 150, or 2000 for actor-based thumbnail-preview captures.")));
		Props->SetObjectField(TEXT("force_thumbnail_fallback"), MakeStringProperty(TEXT("Optional bool string. Default: true when actor_label is provided. When true, uses thumbnail-preview style instead of scene capture.")));
		Props->SetObjectField(TEXT("prefer_editor_preview"), MakeStringProperty(TEXT("Optional bool string. Default: true when actor_label is provided. Prefers editor-style thumbnail preview when available.")));
		Schema->SetObjectField(TEXT("properties"), Props);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CaptureMaterialVisualProbe);
		RegisterTool(Def);
	}

	// ---- Tool: compare_material_probe_captures ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("compare_material_probe_captures");
		Def.Description = TEXT("Compare two saved Material probe captures and return basic image difference metrics.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("image_a_path"), MakeStringProperty(TEXT("Absolute path to the first capture.")));
		Props->SetObjectField(TEXT("image_b_path"), MakeStringProperty(TEXT("Absolute path to the second capture.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("image_a_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("image_b_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CompareMaterialProbeCaptures);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_scalar_param ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_scalar_param");
		Def.Description = TEXT(
			"Add or update a Scalar Parameter expression in a Material. "
			"This creates a ScalarParameter node with the given name and default value.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Name of the scalar parameter (e.g. 'Opacity', 'Roughness')")));
		Props->SetObjectField(TEXT("default_value"),
			MakeStringProperty(TEXT("Default value as a number string (e.g. '0.5')")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("default_value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialScalarParam);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_material_expression_details ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_material_expression_details");
		Def.Description = TEXT(
			"Inspect currently selected Material expression nodes in the Material Editor. "
			"Returns expression details, inputs/outputs, links, and key type-specific settings.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Optional material asset path filter.")));
		Props->SetObjectField(TEXT("include_links"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Props->SetObjectField(TEXT("include_comment_context"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Include containing comments. Default: true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), TArray<TSharedPtr<FJsonValue>>{});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedMaterialExpressionDetails);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_instance_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_instance_info");
		Def.Description = TEXT(
			"Inspect a Material Instance deeply: parent chain, scalar/vector/texture overrides, "
			"inherited defaults, and suspect emissive-related parameters.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_instance_path"),
			MakeStringProperty(TEXT("Full asset path of the Material Instance (MIC).")));
		Props->SetObjectField(TEXT("include_inherited_defaults"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Props->SetObjectField(TEXT("only_overridden"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_instance_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialInstanceInfo);
		RegisterTool(Def);
	}

	// ---- Tool: get_texture_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_texture_info");
		Def.Description = TEXT(
			"Get detailed inspection data for a Texture asset: size, compression, sRGB, mip/LOD settings, "
			"and optional referencer assets.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("texture_path"),
			MakeStringProperty(TEXT("Full asset path of the texture (e.g. '/Game/Textures/T_Noise.T_Noise')")));
		Props->SetObjectField(TEXT("include_referencers"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("texture_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetTextureInfo);
		RegisterTool(Def);
	}

	// ---- Tool: get_texture_material_inspection ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_texture_material_inspection");
		Def.Description = TEXT(
			"Inspect a texture for material authoring: return texture metadata, render a visual preview PNG, "
			"and provide practical guidance about what material formula or texture role fits best.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("texture_path"),
			MakeStringProperty(TEXT("Full asset path of the texture (e.g. '/Game/Textures/T_Noise.T_Noise')")));
		Props->SetObjectField(TEXT("include_referencers"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Props->SetObjectField(TEXT("preview_size"),
			MakeStringProperty(TEXT("Optional integer. Default: 256. Thumbnail preview render size, clamped to 128..1024.")));
		Props->SetObjectField(TEXT("include_image_base64"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true. Include preview PNG as base64 in the JSON output.")));
		Props->SetObjectField(TEXT("save_preview"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true. Save preview PNG to disk for later review.")));
		Props->SetObjectField(TEXT("output_dir"),
			MakeStringProperty(TEXT("Optional output directory for saved preview PNG. Default: Downloads/ForgePilotTextureInspections/...")));
		Props->SetObjectField(TEXT("file_name"),
			MakeStringProperty(TEXT("Optional file name for the saved preview PNG. Default: texture_preview.png")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("texture_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetTextureMaterialInspection);
		RegisterTool(Def);
	}

	// ---- Tool: get_selected_texture_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_selected_texture_info");
		Def.Description = TEXT(
			"Get texture inspection data from selected assets in Content Browser. "
			"By default returns first selected texture.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("all_selected"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). If true, return all selected textures.")));
		Props->SetObjectField(TEXT("include_referencers"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), TArray<TSharedPtr<FJsonValue>>{});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetSelectedTextureInfo);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_mutation_snapshots ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_mutation_snapshots");
		Def.Description = TEXT(
			"List saved ForgePilot safety snapshots for a Material, Material Instance, or Material Function asset.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_asset_path"),
			MakeStringProperty(TEXT("Full asset path of a Material, Material Instance, or Material Function asset.")));
		Props->SetObjectField(TEXT("max_results"),
			MakeStringProperty(TEXT("Optional max number of snapshot files to return. Default: 20.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_asset_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialMutationSnapshots);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_mutation_audit_entries ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_mutation_audit_entries");
		Def.Description = TEXT(
			"List recent ForgePilot mutation audit entries filtered to Material / Material Instance / Material Function work.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("max_results"), MakeStringProperty(TEXT("Optional max number of audit entries to return. Default: 20.")));
		Props->SetObjectField(TEXT("material_asset_path"), MakeStringProperty(TEXT("Optional exact material asset path to filter.")));
		Props->SetObjectField(TEXT("tool_name_filter"), MakeStringProperty(TEXT("Optional exact tool_name filter.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialMutationAuditEntries);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_vector_param ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_vector_param");
		Def.Description = TEXT(
			"Add or update a Vector Parameter expression in a Material. "
			"This creates a VectorParameter node with the given name and default RGBA color.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Name of the vector parameter (e.g. 'BaseColor', 'EmissiveColor')")));
		Props->SetObjectField(TEXT("default_value"),
			MakeArrayProperty(TEXT("[R, G, B, A] values from 0.0 to 1.0"), TEXT("number")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("default_value"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialVectorParam);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_texture_param ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_texture_param");
		Def.Description = TEXT(
			"Add or update a TextureSampleParameter2D expression in a Material so the texture can be overridden from a Material Instance.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Texture parameter name to create or update")));
		Props->SetObjectField(TEXT("texture_path"),
			MakeStringProperty(TEXT("Texture asset path to assign to the parameter")));
		Props->SetObjectField(TEXT("editor_x"),
			MakeStringProperty(TEXT("Optional graph X position for the expression. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("editor_y"),
			MakeStringProperty(TEXT("Optional graph Y position for the expression. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("param_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("texture_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialTextureParam);
		RegisterTool(Def);
	}

	// ---- Tool: create_material_instance ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material_instance");
		Def.Description = TEXT(
			"Create a Material Instance from a parent Material. "
			"Optionally override scalar and vector parameters.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("instance_name"),
			MakeStringProperty(TEXT("Name for the Material Instance (e.g. 'MI_Fire_Red')")));
		Props->SetObjectField(TEXT("parent_material_path"),
			MakeStringProperty(TEXT("Full asset path of the parent Material")));
		Props->SetObjectField(TEXT("save_path"),
			MakeStringProperty(TEXT("Content Browser folder path. Default: '/Game/'")));
		Props->SetObjectField(TEXT("scalar_overrides"),
			MakeStringProperty(TEXT("JSON string of scalar overrides, e.g. '{\"Opacity\": 0.5, \"Roughness\": 0.8}'")));
		Props->SetObjectField(TEXT("vector_overrides"),
			MakeStringProperty(TEXT("JSON string of vector overrides, e.g. '{\"BaseColor\": [1,0,0,1]}'")));
		Props->SetObjectField(TEXT("texture_overrides"),
			MakeStringProperty(TEXT("JSON string of texture overrides, e.g. '{\"NoiseTex\": \"/Game/Textures/T_Noise.T_Noise\"}'")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("instance_name"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("parent_material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterialInstance);
		RegisterTool(Def);
	}

	// ---- Tool: set_actor_material ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_actor_material");
		Def.Description = TEXT(
			"Apply a Material to an actor in the current editor level. "
			"Finds the actor by label and sets the material on its first mesh component.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("actor_label"),
			MakeStringProperty(TEXT("The exact label of the actor in the World Outliner")));
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material or Material Instance to apply")));
		Props->SetObjectField(TEXT("material_index"),
			MakeStringProperty(TEXT("Material slot index (default: 0)")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("actor_label"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetActorMaterial);
		RegisterTool(Def);
	}

	// ================================================================
	// PHASE 3.5: Advanced Material Graph Tools
	// ================================================================

	// ---- Tool: add_material_expression ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("add_material_expression");
		Def.Description = TEXT(
			"Add a material expression node to an existing Material. "
			"Supported types: Multiply, Add, Subtract, Divide, Max, Min, Lerp, TextureSample, TextureSampleParameter2D, TextureObject, TextureObjectParameter, TexCoord, Constant, Constant2, Constant3, "
			"ScalarParameter, VectorParameter, Fresnel, Panner, Time, Clamp, OneMinus, Abs, Power, SquareRoot, Sine, Cosine, Tangent, Arctangent, Arctangent2, Arcsine, Arccosine, Floor, Frac, Fmod, Ceil, Round, Sign, Distance, Length, DotProduct, CrossProduct, Normalize, ReflectionVector, "
			"Step, SmoothStep, If, InverseLinearInterpolate, DepthFade, SceneDepth, PixelDepth, DistanceToNearestSurface, "
			"Saturate, ComponentMask, AppendVector, ParticleColor, ParticleSubUV, ParticleRelativeTime, ParticleRandom, WorldPosition, ObjectPositionWS, ObjectRadius, CameraVectorWS, DynamicParameter, Noise, "
			"FeatureLevelSwitch, StaticSwitchParameter, PreSkinnedLocalPosition, VertexNormalWS, Transform, TransformPosition, "
			"BlackBody, Desaturation, MaterialFunctionCall. "
			"Returns the expression index for use with connect_material_nodes.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("expression_type"),
			MakeStringProperty(TEXT("Type name: Multiply, Add, Subtract, Divide, Max, Min, Lerp, TextureSample, TextureSampleParameter2D, TextureObject, TextureObjectParameter, TexCoord, Constant, Constant2, Constant3, ScalarParameter, VectorParameter, Fresnel, Panner, Time, Clamp, OneMinus, Abs, Power, SquareRoot, Sine, Cosine, Tangent, Arctangent, Arctangent2, Arcsine, Arccosine, Floor, Frac, Fmod, Ceil, Round, Sign, Distance, Length, DotProduct, CrossProduct, Normalize, ReflectionVector, Step, SmoothStep, If, InverseLinearInterpolate, DepthFade, SceneDepth, PixelDepth, DistanceToNearestSurface, Saturate, ComponentMask, AppendVector, ParticleColor, ParticleSubUV, ParticleRelativeTime, ParticleRandom, WorldPosition, ObjectPositionWS, ObjectRadius, CameraVectorWS, DynamicParameter, Noise, FeatureLevelSwitch, StaticSwitchParameter, PreSkinnedLocalPosition, VertexNormalWS, Transform, TransformPosition, BlackBody, Desaturation, MaterialFunctionCall")));
		Props->SetObjectField(TEXT("param_name"),
			MakeStringProperty(TEXT("Parameter name (for ScalarParameter, VectorParameter, TextureSampleParameter2D, TextureObjectParameter)")));
		Props->SetObjectField(TEXT("default_value"),
			MakeStringProperty(TEXT("Default value as string. For Constant: '0.5', for Constant3/VectorParameter: '[R,G,B]' or '[R,G,B,A]'")));
		Props->SetObjectField(TEXT("texture_path"),
			MakeStringProperty(TEXT("For TextureSample, TextureSampleParameter2D, TextureObject, or TextureObjectParameter: asset path of the texture to use")));
		Props->SetObjectField(TEXT("function_path"),
			MakeStringProperty(TEXT("For MaterialFunctionCall: full asset path of the material function to use.")));
		Props->SetObjectField(TEXT("settings_json"),
			MakeStringProperty(TEXT("Optional JSON string for type-specific settings. Example Noise: '{\"noise_function\":\"SimplexTex\",\"scale\":8,\"quality\":1,\"levels\":3,\"output_min\":0,\"output_max\":1,\"level_scale\":2,\"turbulence\":false,\"tiling\":false,\"repeat_size\":128}'. Example Cosine/Tangent: '{\"period\":1.0}'. Example DynamicParameter: '{\"param_names\":[\"PowerEmissive\",\"FadeDistance\",\"CameraDpthFade\",\"Power\"],\"default_value\":[1,1,1,2],\"parameter_index\":0}'. Example DepthFade: '{\"opacity_default\":1.0,\"fade_distance_default\":64.0}'. Example SceneDepth: '{\"input_mode\":\"Coordinates\",\"const_input\":[0.5,0.5]}'. Example StaticSwitchParameter: '{\"default_value\":true,\"dynamic_branch\":false}'. Example If: '{\"equals_threshold\":0.0001,\"const_b\":0.0}'. Example Transform: '{\"source\":\"Local\",\"destination\":\"World\"}'. Example TransformPosition: '{\"source\":\"Local\",\"destination\":\"World\"}'. Example InverseLinearInterpolate: '{\"const_a\":0.0,\"const_b\":1.0,\"const_value\":0.5,\"clamp_result\":true}'.")));
		Props->SetObjectField(TEXT("editor_x"),
			MakeStringProperty(TEXT("Optional graph X position. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("editor_y"),
			MakeStringProperty(TEXT("Optional graph Y position. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("expression_type"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_AddMaterialExpression);
		RegisterTool(Def);
	}

	// ---- Tool: create_material_pattern_block ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material_pattern_block");
		Def.Description = TEXT(
			"Create a practical mini-pattern block inside a Material graph. Supported patterns: panner_texture_chain, emissive_multiply_chain, mask_extract_chain, dissolve_edge_chain.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("pattern_name"), MakeStringProperty(TEXT("Pattern name: panner_texture_chain, emissive_multiply_chain, mask_extract_chain, dissolve_edge_chain.")));
		Props->SetObjectField(TEXT("prefix"), MakeStringProperty(TEXT("Optional naming prefix such as Burn, Dissolve, Glow, Magic.")));
		Props->SetObjectField(TEXT("texture_path"), MakeStringProperty(TEXT("Optional texture asset path used by the pattern.")));
		Props->SetObjectField(TEXT("channels"), MakeStringProperty(TEXT("Optional mask channel string for mask_extract_chain, e.g. R, G, B, A, RG, RGB.")));
		Props->SetObjectField(TEXT("material_pin"), MakeStringProperty(TEXT("Optional material pin to connect the final output into, e.g. EmissiveColor, OpacityMask.")));
		Props->SetObjectField(TEXT("editor_x"), MakeStringProperty(TEXT("Optional starting X position. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("editor_y"), MakeStringProperty(TEXT("Optional starting Y position. If omitted, auto-placement is used.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("pattern_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterialPatternBlock);
		RegisterTool(Def);
	}

	// ---- Tool: create_material_parameter_pack ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material_parameter_pack");
		Def.Description = TEXT(
			"Create a reusable Material parameter pack for common VFX categories. "
			"Supported packs: fire, dissolve, magic, transform, decal, impact.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("pack_name"), MakeStringProperty(TEXT("Pack name: fire, dissolve, magic, transform, decal, impact.")));
		Props->SetObjectField(TEXT("prefix"), MakeStringProperty(TEXT("Optional parameter prefix. Defaults to the pack name with title casing.")));
		Props->SetObjectField(TEXT("texture_path"), MakeStringProperty(TEXT("Optional texture asset path used as the default for created texture parameters.")));
		Props->SetObjectField(TEXT("editor_x"), MakeStringProperty(TEXT("Optional starting X position for the pack block.")));
		Props->SetObjectField(TEXT("editor_y"), MakeStringProperty(TEXT("Optional starting Y position for the pack block.")));
		Props->SetObjectField(TEXT("create_comment"), MakeStringProperty(TEXT("Optional bool string. Default: false. Enable only when the user asks for a comment box around the pack.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("pack_name"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterialParameterPack);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_custom_node ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_custom_node");
		Def.Description = TEXT(
			"Create or update a Material Custom expression node for compact HLSL logic. This path is explicitly guarded because Custom nodes reduce graph readability and should only be authored when that tradeoff is intentional.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional existing Custom node expression index to update. If omitted, a new Custom node is created.")));
		Props->SetObjectField(TEXT("description"), MakeStringProperty(TEXT("Optional description / caption for the Custom node.")));
		Props->SetObjectField(TEXT("code"), MakeStringProperty(TEXT("Custom HLSL code body.")));
		Props->SetObjectField(TEXT("output_type"), MakeStringProperty(TEXT("Optional output type: Float1, Float2, Float3, Float4, MaterialAttributes. Default: Float1.")));
		Props->SetObjectField(TEXT("input_names"), MakeArrayProperty(TEXT("Optional array of Custom input names to create or replace."), TEXT("string")));
		Props->SetObjectField(TEXT("editor_x"), MakeStringProperty(TEXT("Optional graph X position for new nodes.")));
		Props->SetObjectField(TEXT("editor_y"), MakeStringProperty(TEXT("Optional graph Y position for new nodes.")));
		Props->SetObjectField(TEXT("allow_custom_hlsl"), MakeStringProperty(TEXT("Required bool string. Must be true to create or update a Custom/HLSL node because this path is less production-friendly than native graph authoring.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("code"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialCustomNode);
		RegisterTool(Def);
	}

	// ---- Tool: create_material_comment ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("create_material_comment");
		Def.Description = TEXT(
			"Create a Material graph comment box for documenting or framing an authored block. "
			"Useful for AI-authored regions and local graph readability.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Displayed comment text")));
		Props->SetObjectField(TEXT("editor_x"), MakeStringProperty(TEXT("Comment X position")));
		Props->SetObjectField(TEXT("editor_y"), MakeStringProperty(TEXT("Comment Y position")));
		Props->SetObjectField(TEXT("width"), MakeStringProperty(TEXT("Comment width in graph units")));
		Props->SetObjectField(TEXT("height"), MakeStringProperty(TEXT("Comment height in graph units")));
		Props->SetObjectField(TEXT("color_rgba"), MakeArrayProperty(TEXT("Optional [R,G,B,A] color values from 0.0 to 1.0"), TEXT("number")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("comment_text"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CreateMaterialComment);
		RegisterTool(Def);
	}

	// ---- Tool: update_material_comment ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("update_material_comment");
		Def.Description = TEXT(
			"Move, resize, recolor, or retitle an existing Material comment box. "
			"Can also wrap a set of expression indices to frame an authored graph block cleanly.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("comment_index"), MakeStringProperty(TEXT("Optional comment index from get_material_info.")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text used to resolve the comment.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional partial comment text used to resolve the comment.")));
		Props->SetObjectField(TEXT("new_text"), MakeStringProperty(TEXT("Optional replacement text for the comment.")));
		Props->SetObjectField(TEXT("editor_x"), MakeStringProperty(TEXT("Optional new X position.")));
		Props->SetObjectField(TEXT("editor_y"), MakeStringProperty(TEXT("Optional new Y position.")));
		Props->SetObjectField(TEXT("width"), MakeStringProperty(TEXT("Optional new width.")));
		Props->SetObjectField(TEXT("height"), MakeStringProperty(TEXT("Optional new height.")));
		Props->SetObjectField(TEXT("color_rgba"), MakeArrayProperty(TEXT("Optional [R,G,B,A] color values from 0.0 to 1.0"), TEXT("number")));
		Props->SetObjectField(TEXT("wrap_expression_indices"), MakeArrayProperty(TEXT("Optional expression indices to wrap inside the comment."), TEXT("number")));
		Props->SetObjectField(TEXT("padding"), MakeStringProperty(TEXT("Optional wrap padding in graph units. Default: 80.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_UpdateMaterialComment);
		RegisterTool(Def);
	}

	// ---- Tool: delete_material_comments ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("delete_material_comments");
		Def.Description = TEXT(
			"Delete Material comment boxes by index, exact text, contains-text match, or delete_all. "
			"Dry-run by default. Use when an AI-authored graph should be readable through layout only.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("comment_index"), MakeStringProperty(TEXT("Optional zero-based comment index from get_material_info.")));
		Props->SetObjectField(TEXT("comment_text"), MakeStringProperty(TEXT("Optional exact comment text to delete.")));
		Props->SetObjectField(TEXT("contains_text"), MakeStringProperty(TEXT("Optional substring to match comment text.")));
		Props->SetObjectField(TEXT("delete_all"), MakeStringProperty(TEXT("Optional bool string. Default: false. Delete all comment boxes when true.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DeleteMaterialComments);
		RegisterTool(Def);
	}

	// ---- Tool: delete_material_expressions ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("delete_material_expressions");
		Def.Description = TEXT(
			"Delete Material expression nodes by expression_index or expression_indices. "
			"Dry-run by default. Uses UE MaterialEditingLibrary deletion so links to deleted nodes are broken safely.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("expression_index"), MakeStringProperty(TEXT("Optional single expression index from get_material_info.")));
		Props->SetObjectField(TEXT("expression_indices"), MakeArrayProperty(TEXT("Optional array of expression indices from get_material_info."), TEXT("number")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_DeleteMaterialExpressions);
		RegisterTool(Def);
	}

	// ---- Tool: layout_material_expressions ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("layout_material_expressions");
		Def.Description = TEXT(
			"Move a set of Material expressions into a cleaner horizontal or vertical lane layout. "
			"Use this after AI-authored graph edits to reduce overlap.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("layout_mode"), MakeStringProperty(TEXT("horizontal or vertical. Default: horizontal")));
		Props->SetObjectField(TEXT("start_x"), MakeStringProperty(TEXT("Optional starting X position. Default: current minimum X.")));
		Props->SetObjectField(TEXT("start_y"), MakeStringProperty(TEXT("Optional starting Y position. Default: current minimum Y.")));
		Props->SetObjectField(TEXT("spacing"), MakeStringProperty(TEXT("Optional spacing between expressions. Default: 220")));
		Props->SetObjectField(TEXT("expression_indices"), MakeArrayProperty(TEXT("Optional array of expression indices to lay out. If omitted, all expressions are considered."), TEXT("number")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_LayoutMaterialExpressions);
		RegisterTool(Def);
	}

	// ---- Tool: cleanup_material_graph ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("cleanup_material_graph");
		Def.Description = TEXT(
			"Reorganize a Material graph into readable left-to-right lanes and isolate shared inputs "
			"so AI-authored graphs stay clean after functional edits. Comment boxes are opt-in only.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("root_pins"), MakeArrayProperty(TEXT("Optional ordered list of root pins to organize. Defaults to all connected major pins."), TEXT("string")));
		Props->SetObjectField(TEXT("start_x"), MakeStringProperty(TEXT("Optional graph X anchor. Default: current graph minimum X.")));
		Props->SetObjectField(TEXT("start_y"), MakeStringProperty(TEXT("Optional graph Y anchor. Default: current graph minimum Y.")));
		Props->SetObjectField(TEXT("column_spacing"), MakeStringProperty(TEXT("Optional horizontal spacing between graph columns. Default: 340.")));
		Props->SetObjectField(TEXT("row_spacing"), MakeStringProperty(TEXT("Optional vertical spacing between nodes within a column. Default: 220.")));
		Props->SetObjectField(TEXT("lane_spacing"), MakeStringProperty(TEXT("Optional vertical spacing between root-pin lanes. Default: 420.")));
		Props->SetObjectField(TEXT("comment_padding"), MakeStringProperty(TEXT("Optional padding around comment-wrapped blocks when create_comments is true. Default: 120.")));
		Props->SetObjectField(TEXT("create_comments"), MakeStringProperty(TEXT("Optional bool string. Default: false. Enable only when the user asks for comment boxes.")));
		Props->SetObjectField(TEXT("include_unconnected"), MakeStringProperty(TEXT("Optional bool string. Default: true.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_CleanupMaterialGraph);
		RegisterTool(Def);
	}

	// ---- Tool: connect_material_nodes ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("connect_material_nodes");
		Def.Description = TEXT(
			"Connect the output of one material expression to the input of another expression, "
			"or to a material property pin (BaseColor, Metallic, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, WorldPositionOffset). "
			"Use expression indices from get_material_info or add_material_expression.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("source_index"),
			MakeStringProperty(TEXT("Index of the source expression in the Expressions array")));
		Props->SetObjectField(TEXT("source_output_index"),
			MakeStringProperty(TEXT("Output pin index on the source expression (default: 0)")));
		Props->SetObjectField(TEXT("target_type"),
			MakeStringProperty(TEXT("'material_pin' to connect to a material property, or 'expression' to connect to another expression")));
		Props->SetObjectField(TEXT("target_pin"),
			MakeStringProperty(TEXT("If target_type='material_pin': BaseColor, Metallic, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, WorldPositionOffset")));
		Props->SetObjectField(TEXT("target_index"),
			MakeStringProperty(TEXT("If target_type='expression': index of the target expression")));
		Props->SetObjectField(TEXT("target_input_index"),
			MakeStringProperty(TEXT("Input pin index on the target expression (default: 0). For Multiply/Add: 0=A, 1=B. For Lerp: 0=A, 1=B, 2=Alpha")));
		Props->SetObjectField(TEXT("target_input_name"),
			MakeStringProperty(TEXT("Optional safer alternative to target_input_index for expression targets. Connect by exact input pin name (case-insensitive). Recommended for MaterialFunctionCall and nodes whose pin order is easy to misread.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("source_index"))));
		Req.Add(MakeShareable(new FJsonValueString(TEXT("target_type"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ConnectMaterialNodes);
		RegisterTool(Def);
	}

	// ---- Tool: set_material_properties ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("set_material_properties");
		Def.Description = TEXT(
			"Set material-level properties like Blend Mode, Shading Model, Two-Sided, etc.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material")));
		Props->SetObjectField(TEXT("blend_mode"),
			MakeStringProperty(TEXT("Opaque, Masked, Translucent, Additive, Modulate")));
		Props->SetObjectField(TEXT("shading_model"),
			MakeStringProperty(TEXT("DefaultLit, Unlit, Subsurface, ClearCoat")));
		Props->SetObjectField(TEXT("two_sided"),
			MakeStringProperty(TEXT("true or false")));
		Props->SetObjectField(TEXT("thumbnail_primitive"),
			MakeStringProperty(TEXT("Optional thumbnail primitive: None, Sphere, Cube, Plane, Cylinder.")));
		Props->SetObjectField(TEXT("preview_mesh_path"),
			MakeStringProperty(TEXT("Optional mesh asset path for Material preview/thumbnail when thumbnail_primitive is None or when you want to pin a custom preview mesh.")));
		Props->SetObjectField(TEXT("apply_changes"),
			MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_SetMaterialProperties);
		RegisterTool(Def);
	}

	// ---- Tool: review_material_asset ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_material_asset");
		Def.Description = TEXT(
			"Analyze a Material graph and return structured findings for setup quality, visual-risk hints, and optimization hotspots.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material to review")));
		Props->SetObjectField(TEXT("creative_mode"),
			MakeStringProperty(TEXT("Optional review flavor: fire, magic, transform, decal, impact.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewMaterialAsset);
		RegisterTool(Def);
	}

	// ---- Tool: review_material_graph_hygiene ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_material_graph_hygiene");
		Def.Description = TEXT(
			"Review a Material graph for production-friendly hygiene: orphan expressions, duplicate parameter names, optional comment coverage, and Custom node readability risks.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"),
			MakeStringProperty(TEXT("Full asset path of the Material to review.")));
		Props->SetObjectField(TEXT("require_comment_coverage"),
			MakeStringProperty(TEXT("Optional bool string. Default: false. When true, warns if larger graphs leave many nodes outside comment boxes.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewMaterialGraphHygiene);
		RegisterTool(Def);
	}

	// ---- Tool: run_suction_core_material_op ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_suction_core_material_op");
		Def.Description = TEXT(
			"Run a guarded V2 suction-core semantic Material op. These ops prefer Material Instance authoring and explicitly avoid Custom/HLSL graph mutations unless a separate guarded path is chosen.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("op_name"), MakeStringProperty(TEXT("Semantic op name: build_hero_streak_material, build_core_merge_glow_material, retune_longitudinal_flow, simplify_hero_ribbon_material, reduce_opacity_noise_preserve_body.")));
		Props->SetObjectField(TEXT("parent_material_path"), MakeStringProperty(TEXT("Parent/source material path used to create a tuned Material Instance.")));
		Props->SetObjectField(TEXT("instance_name"), MakeStringProperty(TEXT("Optional target MI name. Default uses op-based suffix.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Optional MI save folder. Default: /Game/ForgePilot_Generated/V2/Materials.")));
		Props->SetObjectField(TEXT("probe_actor_label"), MakeStringProperty(TEXT("Optional probe actor label for capture after authoring.")));
		Props->SetObjectField(TEXT("capture_file_name"), MakeStringProperty(TEXT("Optional capture file name when probe_actor_label is provided.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false. Dry-run returns the semantic contract and planned overrides without creating an asset.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("op_name"))),
			MakeShareable(new FJsonValueString(TEXT("parent_material_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunSuctionCoreMaterialOp);
		RegisterTool(Def);
	}

	// ---- Tool: run_v2_material_op ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("run_v2_material_op");
		Def.Description = TEXT(
			"Run a guarded V2 semantic Material op for supported orb-centric archetypes. "
			"Uses native Material Instance authoring only and returns structured semantic contract data.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("archetype"), MakeStringProperty(TEXT("Resolved V2 archetype: suction_core_energy, electric_arc_orb, hero_fire_orb_sweep, arcane_pull_orb, impact_pulse_core, or explosion_flash_core.")));
		Props->SetObjectField(TEXT("op_name"), MakeStringProperty(TEXT("Semantic op name such as build_hero_streak_material or build_core_merge_glow_material.")));
		Props->SetObjectField(TEXT("parent_material_path"), MakeStringProperty(TEXT("Parent/source material path used to create a tuned Material Instance.")));
		Props->SetObjectField(TEXT("instance_name"), MakeStringProperty(TEXT("Optional target MI name.")));
		Props->SetObjectField(TEXT("save_path"), MakeStringProperty(TEXT("Optional MI save folder.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string. Default: false.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		Schema->SetArrayField(TEXT("required"), {
			MakeShareable(new FJsonValueString(TEXT("archetype"))),
			MakeShareable(new FJsonValueString(TEXT("op_name"))),
			MakeShareable(new FJsonValueString(TEXT("parent_material_path")))
		});

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RunV2MaterialOp);
		RegisterTool(Def);
	}

	// ---- Tool: get_material_function_info ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("get_material_function_info");
		Def.Description = TEXT(
			"Inspect a Material Function deeply: internal expressions, function inputs/outputs, dependent functions, and local graph structure. "
			"Use the returned input names when wiring a MaterialFunctionCall; connect_material_nodes.target_input_name is safer than relying on raw input indices.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_function_path"), MakeStringProperty(TEXT("Full asset path of the Material Function or Material Function Instance.")));
		Props->SetObjectField(TEXT("focus_keyword"), MakeStringProperty(TEXT("Optional keyword to focus on a local block inside the function graph.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_function_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_GetMaterialFunctionInfo);
		RegisterTool(Def);
	}

	// ---- Tool: review_material_function ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_material_function");
		Def.Description = TEXT(
			"Review a Material Function for reusability, graph hygiene, and likely simplification opportunities.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_function_path"), MakeStringProperty(TEXT("Full asset path of the Material Function or Material Function Instance.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_function_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewMaterialFunction);
		RegisterTool(Def);
	}

	// ---- Tool: restore_material_mutation_snapshot ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("restore_material_mutation_snapshot");
		Def.Description = TEXT(
			"Restore a Material / Material Instance / Material Function package from a ForgePilot safety snapshot.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_asset_path"), MakeStringProperty(TEXT("Full asset path of the Material, Material Instance, or Material Function asset.")));
		Props->SetObjectField(TEXT("snapshot_path"), MakeStringProperty(TEXT("Optional full snapshot path. If omitted, use_latest=true will resolve the newest snapshot.")));
		Props->SetObjectField(TEXT("use_latest"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: true.")));
		Props->SetObjectField(TEXT("reopen_editor"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false.")));
		Props->SetObjectField(TEXT("apply_changes"), MakeStringProperty(TEXT("Optional bool string ('true'/'false'). Default: false (dry-run).")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_asset_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_RestoreMaterialMutationSnapshot);
		RegisterTool(Def);
	}

	// ---- Tool: review_material_graph_simplification ----
	{
		FVFXToolDefinition Def;
		Def.Name = TEXT("review_material_graph_simplification");
		Def.Description = TEXT("Review a Material graph specifically for wire clutter, lane structure, and opportunities to collapse logic into cleaner blocks.");

		TSharedPtr<FJsonObject> Schema = MakeShareable(new FJsonObject());
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		TSharedPtr<FJsonObject> Props = MakeShareable(new FJsonObject());
		Props->SetObjectField(TEXT("material_path"), MakeStringProperty(TEXT("Full asset path of the Material.")));
		Props->SetObjectField(TEXT("focus_query"), MakeStringProperty(TEXT("Optional keyword or local block query.")));
		Schema->SetObjectField(TEXT("properties"), Props);
		TArray<TSharedPtr<FJsonValue>> Req;
		Req.Add(MakeShareable(new FJsonValueString(TEXT("material_path"))));
		Schema->SetArrayField(TEXT("required"), Req);

		Def.InputSchema = Schema;
		Def.ExecuteDelegate.BindRaw(this, &FVFXToolRegistry::Tool_ReviewMaterialGraphSimplification);
		RegisterTool(Def);
	}
}

// ============================================================
// PHASE 3: Material Tool Implementations
// ============================================================

bool FVFXToolRegistry::Tool_CreateMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatName;
	if (!Input->TryGetStringField(TEXT("material_name"), MatName))
	{
		OutResult = TEXT("Missing required field: material_name");
		return false;
	}

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	// Parse base color
	FLinearColor BaseColor = FLinearColor::White;
	const TArray<TSharedPtr<FJsonValue>>* ColorArray;
	if (Input->TryGetArrayField(TEXT("base_color"), ColorArray) && ColorArray->Num() >= 3)
	{
		BaseColor.R = (*ColorArray)[0]->AsNumber();
		BaseColor.G = (*ColorArray)[1]->AsNumber();
		BaseColor.B = (*ColorArray)[2]->AsNumber();
		BaseColor.A = ColorArray->Num() >= 4 ? (*ColorArray)[3]->AsNumber() : 1.0f;
	}

	// Build package path
	FString PackagePath = SavePath;
	if (!PackagePath.EndsWith(TEXT("/")))
	{
		PackagePath += TEXT("/");
	}
	PackagePath += MatName;

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		OutResult = FString::Printf(TEXT("Asset already exists at path '%s'."), *PackagePath);
		return false;
	}

	TSharedPtr<FJsonObject> MatResult = MakeShareable(new FJsonObject());
	MatResult->SetBoolField(TEXT("success"), true);
	MatResult->SetStringField(TEXT("action"), TEXT("create_material"));
	MatResult->SetStringField(TEXT("asset_path"), PackagePath);
	MatResult->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	MatResult->SetBoolField(TEXT("will_mutate_asset"), bApplyChanges);
	if (!bApplyChanges)
	{
		MatResult->SetArrayField(TEXT("base_color"), {
			MakeShareable(new FJsonValueNumber(BaseColor.R)),
			MakeShareable(new FJsonValueNumber(BaseColor.G)),
			MakeShareable(new FJsonValueNumber(BaseColor.B)),
			MakeShareable(new FJsonValueNumber(BaseColor.A))
		});
		MatResult->SetStringField(
			TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create Material '%s' at '%s' with base color (R=%.2f, G=%.2f, B=%.2f)."),
				*MatName, *PackagePath, BaseColor.R, BaseColor.G, BaseColor.B));
		OutResult = JsonObjToString(MatResult);
		return true;
	}

	// Create Material using factory
	UMaterialFactoryNew* Factory = NewObject<UMaterialFactoryNew>();
	UPackage* Package = CreatePackage(*PackagePath);
	UMaterial* NewMaterial = Cast<UMaterial>(
		Factory->FactoryCreateNew(UMaterial::StaticClass(), Package, FName(*MatName), RF_Public | RF_Standalone, nullptr, GWarn));

	if (!NewMaterial)
	{
		OutResult = TEXT("Failed to create Material asset.");
		return false;
	}

	// Add a Constant3Vector expression for base color
	UMaterialExpressionConstant3Vector* ColorExpr = NewObject<UMaterialExpressionConstant3Vector>(NewMaterial);
	ColorExpr->Constant = BaseColor;
	NewMaterial->GetExpressionCollection().AddExpression(ColorExpr);
	NewMaterial->GetEditorOnlyData()->BaseColor.Expression = ColorExpr;

	// Compile & save
	NewMaterial->PreEditChange(nullptr);
	NewMaterial->PostEditChange();

	FAssetRegistryModule::AssetCreated(NewMaterial);
	NewMaterial->MarkPackageDirty();

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	FString PackageFileName = FPackageName::LongPackageNameToFilename(
		PackagePath, FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, NewMaterial, *PackageFileName, SaveArgs);

	MatResult->SetStringField(TEXT("asset_path"), NewMaterial->GetPathName());
	MatResult->SetBoolField(TEXT("saved"), true);
	const FString CreateMaterialMessage = FString::Printf(TEXT("Created Material '%s' with base color (R=%.2f, G=%.2f, B=%.2f)"),
		*MatName, BaseColor.R, BaseColor.G, BaseColor.B);
	MatResult->SetStringField(TEXT("message"), CreateMaterialMessage);
	AppendMaterialMutationAudit(MatResult, TEXT("create_material"), NewMaterial, CreateMaterialMessage);

	OutResult = JsonObjToString(MatResult);
	return true;
}

bool FVFXToolRegistry::Tool_GetMaterialEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	TArray<TSharedPtr<FJsonValue>> OpenedMaterials;
	TArray<TSharedPtr<FJsonValue>> OpenedMaterialInstances;
	TArray<TSharedPtr<FJsonValue>> OpenedMaterialFunctions;
	TArray<TSharedPtr<FJsonValue>> SelectedMaterials;
	TArray<TSharedPtr<FJsonValue>> SelectedMaterialFunctions;

	auto BuildAssetObject = [](UObject* Asset) -> TSharedPtr<FJsonObject>
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		if (!Asset)
		{
			return Obj;
		}

		Obj->SetStringField(TEXT("name"), Asset->GetName());
		Obj->SetStringField(TEXT("path"), Asset->GetPathName());
		Obj->SetStringField(TEXT("class"), Asset->GetClass()->GetName());
		return Obj;
	};

	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			for (UObject* Asset : AssetEditorSubsystem->GetAllEditedAssets())
			{
				if (!Asset)
				{
					continue;
				}

				TSharedPtr<FJsonObject> AssetObj = BuildAssetObject(Asset);
				if (Asset->IsA<UMaterial>())
				{
					OpenedMaterials.Add(MakeShareable(new FJsonValueObject(AssetObj)));
				}
				else if (Asset->IsA<UMaterialInstance>())
				{
					OpenedMaterialInstances.Add(MakeShareable(new FJsonValueObject(AssetObj)));
				}
				else if (Asset->IsA<UMaterialFunctionInterface>())
				{
					OpenedMaterialFunctions.Add(MakeShareable(new FJsonValueObject(AssetObj)));
				}
			}
		}
	}

	for (UObject* Asset : UEditorUtilityLibrary::GetSelectedAssets())
	{
		if (!Asset)
		{
			continue;
		}

		TSharedPtr<FJsonObject> AssetObj = BuildAssetObject(Asset);
		if (Asset->IsA<UMaterial>() || Asset->IsA<UMaterialInstance>())
		{
			SelectedMaterials.Add(MakeShareable(new FJsonValueObject(AssetObj)));
		}
		else if (Asset->IsA<UMaterialFunctionInterface>())
		{
			SelectedMaterialFunctions.Add(MakeShareable(new FJsonValueObject(AssetObj)));
		}
	}

	auto ResolvePreferredPath = [](const TArray<TSharedPtr<FJsonValue>>& Preferred, const TArray<TSharedPtr<FJsonValue>>& Fallback) -> FString
	{
		const auto ExtractPath = [](const TArray<TSharedPtr<FJsonValue>>& Values) -> FString
		{
			for (const TSharedPtr<FJsonValue>& Value : Values)
			{
				const TSharedPtr<FJsonObject> Obj = Value.IsValid() ? Value->AsObject() : nullptr;
				if (Obj.IsValid())
				{
					FString Path;
					if (Obj->TryGetStringField(TEXT("path"), Path) && !Path.IsEmpty())
					{
						return Path;
					}
				}
			}
			return FString();
		};

		const FString PreferredPath = ExtractPath(Preferred);
		return PreferredPath.IsEmpty() ? ExtractPath(Fallback) : PreferredPath;
	};

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_material_editor_context"));
	Result->SetArrayField(TEXT("opened_materials"), OpenedMaterials);
	Result->SetArrayField(TEXT("opened_material_instances"), OpenedMaterialInstances);
	Result->SetArrayField(TEXT("opened_material_functions"), OpenedMaterialFunctions);
	Result->SetArrayField(TEXT("selected_materials"), SelectedMaterials);
	Result->SetArrayField(TEXT("selected_material_functions"), SelectedMaterialFunctions);
	Result->SetStringField(TEXT("preferred_material_path"), ResolvePreferredPath(SelectedMaterials, OpenedMaterials));
	Result->SetStringField(TEXT("preferred_material_function_path"), ResolvePreferredPath(SelectedMaterialFunctions, OpenedMaterialFunctions));
	Result->SetNumberField(TEXT("opened_material_count"), OpenedMaterials.Num());
	Result->SetNumberField(TEXT("opened_material_instance_count"), OpenedMaterialInstances.Num());
	Result->SetNumberField(TEXT("opened_material_function_count"), OpenedMaterialFunctions.Num());
	Result->SetNumberField(TEXT("selected_material_count"), SelectedMaterials.Num());
	Result->SetNumberField(TEXT("selected_material_function_count"), SelectedMaterialFunctions.Num());

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetMaterialMutationAuditEntries(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	int32 MaxResults = 20;
	if (Input.IsValid())
	{
		TryReadOptionalIntField(Input, TEXT("max_results"), MaxResults);
	}
	MaxResults = FMath::Clamp(MaxResults, 1, 200);

	FString MaterialAssetPathFilter;
	FString ToolNameFilter;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("material_asset_path"), MaterialAssetPathFilter);
		Input->TryGetStringField(TEXT("tool_name_filter"), ToolNameFilter);
	}

	TArray<FString> EntryPaths;
	FVFXMutationAudit::GetRecentAuditEntryPaths(MaxResults * 5, EntryPaths);

	TArray<TSharedPtr<FJsonValue>> FilteredEntries;
	for (const FString& EntryPath : EntryPaths)
	{
		TSharedPtr<FJsonObject> EntryObj;
		if (!FVFXMutationAudit::ReadAuditEntry(EntryPath, EntryObj) || !EntryObj.IsValid())
		{
			continue;
		}

		const FString AssetClass = EntryObj->HasField(TEXT("asset_class")) ? EntryObj->GetStringField(TEXT("asset_class")) : TEXT("");
		const bool bIsMaterialAudit =
			AssetClass == TEXT("Material") ||
			AssetClass == TEXT("MaterialInstanceConstant") ||
			AssetClass == TEXT("MaterialFunction") ||
			AssetClass == TEXT("MaterialFunctionInstance");
		if (!bIsMaterialAudit)
		{
			continue;
		}

		if (!ToolNameFilter.IsEmpty())
		{
			const FString ToolName = EntryObj->HasField(TEXT("tool_name")) ? EntryObj->GetStringField(TEXT("tool_name")) : TEXT("");
			if (!ToolName.Equals(ToolNameFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		if (!MaterialAssetPathFilter.IsEmpty())
		{
			const FString AssetPath = EntryObj->HasField(TEXT("asset_path")) ? EntryObj->GetStringField(TEXT("asset_path")) : TEXT("");
			if (!AssetPath.Equals(MaterialAssetPathFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		FilteredEntries.Add(MakeShareable(new FJsonValueObject(EntryObj)));
		if (FilteredEntries.Num() >= MaxResults)
		{
			break;
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_material_mutation_audit_entries"));
	Result->SetStringField(TEXT("material_asset_path_filter"), MaterialAssetPathFilter);
	Result->SetStringField(TEXT("tool_name_filter"), ToolNameFilter);
	Result->SetNumberField(TEXT("count"), FilteredEntries.Num());
	Result->SetArrayField(TEXT("entries"), FilteredEntries);
	Result->SetStringField(TEXT("message"), FilteredEntries.Num() > 0
		? TEXT("Material mutation audit entries found.")
		: TEXT("No material mutation audit entries matched the provided filters."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CompileMaterialAndGetDiagnostics(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) || MaterialPath.IsEmpty())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_MISSING_MATERIAL_PATH"), TEXT("Missing required field: material_path"));
	}

	UObject* LoadedAsset = UEditorAssetLibrary::LoadAsset(MaterialPath);
	UMaterialInterface* MaterialInterface = Cast<UMaterialInterface>(LoadedAsset);
	if (!MaterialInterface)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_NOT_FOUND"),
			FString::Printf(TEXT("Could not load Material / Material Instance at path: %s"), *MaterialPath));
	}

	UMaterial* BaseMaterial = MaterialInterface->GetMaterial();
	if (!BaseMaterial)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_BASE_MATERIAL_MISSING"),
			FString::Printf(TEXT("Material interface '%s' does not resolve to a base material."), *MaterialInterface->GetPathName()));
	}

	const bool bSaveAsset = ParseOptionalBoolField(Input, TEXT("save_asset"), false);

	{
		FMaterialUpdateContext UpdateContext(FMaterialUpdateContext::EOptions::SyncWithRenderingThread);
		UpdateContext.AddMaterialInterface(MaterialInterface);

		if (UMaterial* MaterialAsset = Cast<UMaterial>(MaterialInterface))
		{
			UpdateContext.AddMaterial(MaterialAsset);
			MaterialAsset->PreEditChange(nullptr);
			MaterialAsset->PostEditChange();
		}
		else if (UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(MaterialInterface))
		{
			MaterialInstance->ForceRecompileForRendering();
		}
		else
		{
			MaterialInterface->ForceRecompileForRendering();
		}
	}

	TArray<UObject*> CompileTargets;
	CompileTargets.Add(MaterialInterface);
	if (BaseMaterial && BaseMaterial != MaterialInterface)
	{
		CompileTargets.Add(BaseMaterial);
	}

	FAssetCompilingManager::Get().FinishCompilationForObjects(CompileTargets);
	FlushRenderingCommands();

	const FMaterialResource* MaterialResource = MaterialInterface->GetMaterialResource(GMaxRHIShaderPlatform);
	if (!MaterialResource && BaseMaterial)
	{
		MaterialResource = BaseMaterial->GetMaterialResource(GMaxRHIShaderPlatform);
	}

	if (!MaterialResource)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_COMPILE_RESOURCE_MISSING"),
			FString::Printf(TEXT("Material '%s' did not expose a material resource after recompilation."), *MaterialInterface->GetPathName()));
	}

	const TArray<FString>& CompileErrors = MaterialResource->GetCompileErrors();
	const TArray<UMaterialExpression*>& ErrorExpressions = MaterialResource->GetErrorExpressions();

	TMap<UMaterialExpression*, int32> ExpressionIndices;
	if (BaseMaterial)
	{
		for (int32 ExpressionIndex = 0; ExpressionIndex < BaseMaterial->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			if (UMaterialExpression* Expression = BaseMaterial->GetExpressionCollection().Expressions[ExpressionIndex])
			{
				ExpressionIndices.Add(Expression, ExpressionIndex);
			}
		}
	}

	TArray<TSharedPtr<FJsonValue>> ErrorArray;
	for (int32 ErrorIndex = 0; ErrorIndex < CompileErrors.Num(); ++ErrorIndex)
	{
		TSharedPtr<FJsonObject> ErrorObj = MakeShareable(new FJsonObject());
		ErrorObj->SetNumberField(TEXT("error_index"), ErrorIndex);
		ErrorObj->SetStringField(TEXT("message"), CompileErrors[ErrorIndex]);

		if (ErrorExpressions.IsValidIndex(ErrorIndex) && ErrorExpressions[ErrorIndex])
		{
			UMaterialExpression* ErrorExpression = ErrorExpressions[ErrorIndex];
			ErrorObj->SetStringField(TEXT("expression_name"), GetMaterialExpressionPracticalName(ErrorExpression));
			ErrorObj->SetStringField(TEXT("expression_class"), ErrorExpression->GetClass()->GetName());
			ErrorObj->SetNumberField(TEXT("editor_x"), ErrorExpression->MaterialExpressionEditorX);
			ErrorObj->SetNumberField(TEXT("editor_y"), ErrorExpression->MaterialExpressionEditorY);

			if (const int32* ExpressionIndex = ExpressionIndices.Find(ErrorExpression))
			{
				ErrorObj->SetNumberField(TEXT("expression_index"), *ExpressionIndex);
			}
		}

		ErrorArray.Add(MakeShareable(new FJsonValueObject(ErrorObj)));
	}

	bool bSaveSucceeded = true;
	FString SaveError;
	if (bSaveAsset)
	{
		bSaveSucceeded = SaveMaterialMutationAsset(MaterialInterface, SaveError);
	}

	const ERHIFeatureLevel::Type FeatureLevel = GMaxRHIFeatureLevel;
	FString FeatureLevelName;
	GetFeatureLevelName(FeatureLevel, FeatureLevelName);
	if (FeatureLevelName.IsEmpty())
	{
		FeatureLevelName = FString::FromInt(static_cast<int32>(FeatureLevel));
	}

	FString ShaderPlatformName = LegacyShaderPlatformToShaderFormat(GMaxRHIShaderPlatform).ToString();
	if (ShaderPlatformName.IsEmpty())
	{
		ShaderPlatformName = FString::FromInt(static_cast<int32>(GMaxRHIShaderPlatform));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("compile_material_and_get_diagnostics"));
	Result->SetStringField(TEXT("material_path"), MaterialPath);
	Result->SetStringField(TEXT("resolved_asset_path"), MaterialInterface->GetPathName());
	Result->SetStringField(TEXT("asset_class"), MaterialInterface->GetClass()->GetName());
	Result->SetStringField(TEXT("base_material_path"), BaseMaterial->GetPathName());
	Result->SetStringField(TEXT("feature_level"), FeatureLevelName);
	Result->SetStringField(TEXT("shader_platform"), ShaderPlatformName);
	Result->SetBoolField(TEXT("resource_present"), MaterialResource != nullptr);
	Result->SetBoolField(TEXT("is_clean"), CompileErrors.Num() == 0);
	Result->SetBoolField(TEXT("compile_succeeded"), CompileErrors.Num() == 0);
	Result->SetNumberField(TEXT("error_count"), CompileErrors.Num());
	Result->SetNumberField(TEXT("warning_count"), 0);
	Result->SetBoolField(TEXT("save_requested"), bSaveAsset);
	Result->SetBoolField(TEXT("save_succeeded"), bSaveSucceeded);
	if (!SaveError.IsEmpty())
	{
		Result->SetStringField(TEXT("save_error"), SaveError);
	}
	Result->SetArrayField(TEXT("errors"), ErrorArray);
	Result->SetStringField(
		TEXT("message"),
		CompileErrors.Num() == 0
			? TEXT("Material compiled cleanly for the current feature level/resource.")
			: TEXT("Material compiled with shader errors. See errors[] for the same diagnostics backing Material Editor Stats."));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetMaterialInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	auto ParseOptionalBool = [&](const TCHAR* FieldName, bool DefaultValue) -> bool
	{
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
	};

	auto ParseOptionalInt = [&](const TCHAR* FieldName, int32 DefaultValue) -> int32
	{
		if (Input->HasTypedField<EJson::Number>(FieldName))
		{
			return static_cast<int32>(Input->GetNumberField(FieldName));
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue))
		{
			return FCString::Atoi(*StringValue);
		}
		return DefaultValue;
	};

	const bool bIncludeCustomCode = ParseOptionalBool(TEXT("include_custom_code"), false);
	const int32 MaxCustomCodeChars = FMath::Max(0, ParseOptionalInt(TEXT("max_custom_code_chars"), 300));
	FString FocusQuery;
	Input->TryGetStringField(TEXT("focus_query"), FocusQuery);
	const int32 FocusExpressionIndex = ParseOptionalInt(TEXT("focus_expression_index"), INDEX_NONE);
	const int32 FocusHops = FMath::Clamp(ParseOptionalInt(TEXT("focus_hops"), 2), 0, 8);

	bool bHasFocusRegion = false;
	bool bHasFocusRegionCenter = false;
	bool bHasFocusRegionBox = false;
	bool bShapeFieldProvided = Input->HasField(TEXT("focus_region_shape"));
	FString FocusRegionShape = TEXT("circle");
	FVector2D FocusRegionCenter(0.0f, 0.0f);
	FVector4 FocusRegionBox(0.0f, 0.0f, 0.0f, 0.0f); // MinX, MinY, MaxX, MaxY
	float FocusRegionRadius = 1200.0f;

	{
		FString RegionShapeString;
		if (Input->TryGetStringField(TEXT("focus_region_shape"), RegionShapeString))
		{
			const FString ShapeLower = RegionShapeString.TrimStartAndEnd().ToLower();
			if (ShapeLower == TEXT("box") || ShapeLower == TEXT("circle"))
			{
				FocusRegionShape = ShapeLower;
			}
		}
	}

	{
		const TArray<TSharedPtr<FJsonValue>>* RegionCenterArray = nullptr;
		if (Input->TryGetArrayField(TEXT("focus_region_center"), RegionCenterArray) &&
			RegionCenterArray && RegionCenterArray->Num() >= 2)
		{
			FocusRegionCenter.X = static_cast<float>((*RegionCenterArray)[0]->AsNumber());
			FocusRegionCenter.Y = static_cast<float>((*RegionCenterArray)[1]->AsNumber());
			bHasFocusRegion = true;
			bHasFocusRegionCenter = true;
		}
		else
		{
			FString RegionCenterString;
			if (Input->TryGetStringField(TEXT("focus_region_center"), RegionCenterString))
			{
				TArray<FString> Parts;
				RegionCenterString.ParseIntoArray(Parts, TEXT(","));
				if (Parts.Num() >= 2)
				{
					FocusRegionCenter.X = FCString::Atof(*Parts[0]);
					FocusRegionCenter.Y = FCString::Atof(*Parts[1]);
					bHasFocusRegion = true;
					bHasFocusRegionCenter = true;
				}
			}
		}
	}

	{
		const TArray<TSharedPtr<FJsonValue>>* RegionBoxArray = nullptr;
		if (Input->TryGetArrayField(TEXT("focus_region_box"), RegionBoxArray) &&
			RegionBoxArray && RegionBoxArray->Num() >= 4)
		{
			FocusRegionBox.X = static_cast<float>((*RegionBoxArray)[0]->AsNumber());
			FocusRegionBox.Y = static_cast<float>((*RegionBoxArray)[1]->AsNumber());
			FocusRegionBox.Z = static_cast<float>((*RegionBoxArray)[2]->AsNumber());
			FocusRegionBox.W = static_cast<float>((*RegionBoxArray)[3]->AsNumber());
			bHasFocusRegion = true;
			bHasFocusRegionBox = true;
		}
		else
		{
			FString RegionBoxString;
			if (Input->TryGetStringField(TEXT("focus_region_box"), RegionBoxString))
			{
				TArray<FString> Parts;
				RegionBoxString.ParseIntoArray(Parts, TEXT(","));
				if (Parts.Num() >= 4)
				{
					FocusRegionBox.X = FCString::Atof(*Parts[0]);
					FocusRegionBox.Y = FCString::Atof(*Parts[1]);
					FocusRegionBox.Z = FCString::Atof(*Parts[2]);
					FocusRegionBox.W = FCString::Atof(*Parts[3]);
					bHasFocusRegion = true;
					bHasFocusRegionBox = true;
				}
			}
		}
	}

	if (Input->HasTypedField<EJson::Number>(TEXT("focus_region_radius")))
	{
		FocusRegionRadius = static_cast<float>(Input->GetNumberField(TEXT("focus_region_radius")));
	}
	else
	{
		FString RegionRadiusString;
		if (Input->TryGetStringField(TEXT("focus_region_radius"), RegionRadiusString))
		{
			FocusRegionRadius = FCString::Atof(*RegionRadiusString);
		}
	}
	FocusRegionRadius = FMath::Max(1.0f, FocusRegionRadius);

	// Normalize box bounds and derive center from box when available
	if (bHasFocusRegionBox)
	{
		const float MinX = FMath::Min(FocusRegionBox.X, FocusRegionBox.Z);
		const float MinY = FMath::Min(FocusRegionBox.Y, FocusRegionBox.W);
		const float MaxX = FMath::Max(FocusRegionBox.X, FocusRegionBox.Z);
		const float MaxY = FMath::Max(FocusRegionBox.Y, FocusRegionBox.W);
		FocusRegionBox = FVector4(MinX, MinY, MaxX, MaxY);
		FocusRegionCenter = FVector2D((MinX + MaxX) * 0.5f, (MinY + MaxY) * 0.5f);
	}

	if (!bShapeFieldProvided && bHasFocusRegionBox)
	{
		FocusRegionShape = TEXT("box");
	}

	const bool bUseBoxRegion = FocusRegionShape.Equals(TEXT("box"), ESearchCase::IgnoreCase);
	if (bUseBoxRegion && !bHasFocusRegionBox && bHasFocusRegionCenter)
	{
		// If user asked for box but only gave center/radius, synthesize a square box.
		FocusRegionBox = FVector4(
			FocusRegionCenter.X - FocusRegionRadius,
			FocusRegionCenter.Y - FocusRegionRadius,
			FocusRegionCenter.X + FocusRegionRadius,
			FocusRegionCenter.Y + FocusRegionRadius);
		bHasFocusRegionBox = true;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Could not load Material at path: %s"), *MatPath);
		return false;
	}

	const TArray<UMaterialExpression*>& Expressions = Material->GetExpressionCollection().Expressions;
	TMap<const UMaterialExpression*, int32> ExpressionToIndex;
	for (int32 ExprIndex = 0; ExprIndex < Expressions.Num(); ++ExprIndex)
	{
		if (Expressions[ExprIndex])
		{
			ExpressionToIndex.Add(Expressions[ExprIndex], ExprIndex);
		}
	}

	auto EnumValueToString = [](const UEnum* EnumType, int64 RawValue) -> FString
	{
		if (!EnumType)
		{
			return FString::FromInt(static_cast<int32>(RawValue));
		}
		return EnumType->GetNameStringByValue(RawValue);
	};

	auto ResolveOutputName = [&](UMaterialExpression* SourceExpression, int32 OutputIndex) -> FString
	{
		if (!SourceExpression)
		{
			return TEXT("None");
		}

		const TArray<FExpressionOutput>& Outputs = SourceExpression->GetOutputs();
		if (Outputs.IsValidIndex(OutputIndex))
		{
			if (!Outputs[OutputIndex].OutputName.IsNone())
			{
				return Outputs[OutputIndex].OutputName.ToString();
			}
			return FString::Printf(TEXT("Output%d"), OutputIndex);
		}

		if (OutputIndex <= 0)
		{
			return TEXT("Output");
		}
		return FString::Printf(TEXT("Output%d"), OutputIndex);
	};

	auto GetMaskLabel = [](uint32 Mask) -> FString
	{
		FString Label;
		if (Mask & 1) Label += TEXT("R");
		if (Mask & 2) Label += TEXT("G");
		if (Mask & 4) Label += TEXT("B");
		if (Mask & 8) Label += TEXT("A");
		return Label.IsEmpty() ? TEXT("RGBA") : Label;
	};

	auto CountChannelsFromMask = [](uint32 Mask) -> int32
	{
		int32 Channels = 0;
		if (Mask & 1) Channels++;
		if (Mask & 2) Channels++;
		if (Mask & 4) Channels++;
		if (Mask & 8) Channels++;
		return Channels > 0 ? Channels : 4;
	};

	auto ResolveOutputMask = [](const FExpressionOutput& Output) -> uint32
	{
		uint32 Mask = 0;
		if (Output.MaskR) Mask |= 1;
		if (Output.MaskG) Mask |= 2;
		if (Output.MaskB) Mask |= 4;
		if (Output.MaskA) Mask |= 8;

		if (Mask == 0)
		{
			Mask = Output.Mask != 0 ? Output.Mask : (1 | 2 | 4 | 8);
		}
		return Mask;
	};

	auto MakeTextureInfoObject = [&](UTexture* Texture) -> TSharedPtr<FJsonObject>
	{
		TSharedPtr<FJsonObject> TextureObj = MakeShareable(new FJsonObject());
		if (!Texture)
		{
			return TextureObj;
		}

		TextureObj->SetStringField(TEXT("name"), Texture->GetName());
		TextureObj->SetStringField(TEXT("path"), Texture->GetPathName());
		TextureObj->SetStringField(TEXT("class"), Texture->GetClass()->GetName());
		TextureObj->SetBoolField(TEXT("srgb"), Texture->SRGB);
		TextureObj->SetStringField(
			TEXT("compression_settings"),
			EnumValueToString(StaticEnum<TextureCompressionSettings>(), static_cast<int64>(Texture->CompressionSettings)));
		TextureObj->SetStringField(
			TEXT("lod_group"),
			EnumValueToString(StaticEnum<TextureGroup>(), static_cast<int64>(Texture->LODGroup)));
		TextureObj->SetStringField(
			TEXT("mip_gen_settings"),
			EnumValueToString(StaticEnum<TextureMipGenSettings>(), static_cast<int64>(Texture->MipGenSettings)));
		TextureObj->SetStringField(
			TEXT("filter"),
			EnumValueToString(StaticEnum<TextureFilter>(), static_cast<int64>(Texture->Filter)));
		if (UTexture2D* Texture2D = Cast<UTexture2D>(Texture))
		{
			TextureObj->SetNumberField(TEXT("size_x"), Texture2D->GetSizeX());
			TextureObj->SetNumberField(TEXT("size_y"), Texture2D->GetSizeY());
		}

		return TextureObj;
	};

	TSharedPtr<FJsonObject> InfoResult = MakeShareable(new FJsonObject());
	InfoResult->SetStringField(TEXT("name"), Material->GetName());
	InfoResult->SetStringField(TEXT("path"), Material->GetPathName());

	// Blend mode
	FString BlendModeStr;
	switch (Material->BlendMode)
	{
	case BLEND_Opaque:      BlendModeStr = TEXT("Opaque"); break;
	case BLEND_Masked:      BlendModeStr = TEXT("Masked"); break;
	case BLEND_Translucent: BlendModeStr = TEXT("Translucent"); break;
	case BLEND_Additive:    BlendModeStr = TEXT("Additive"); break;
	case BLEND_Modulate:    BlendModeStr = TEXT("Modulate"); break;
	default:                BlendModeStr = TEXT("Unknown"); break;
	}
	InfoResult->SetStringField(TEXT("blend_mode"), BlendModeStr);

	// Shading model
	FString ShadingModelStr;
	switch (Material->GetShadingModels().GetFirstShadingModel())
	{
	case MSM_DefaultLit:    ShadingModelStr = TEXT("DefaultLit"); break;
	case MSM_Unlit:         ShadingModelStr = TEXT("Unlit"); break;
	case MSM_Subsurface:    ShadingModelStr = TEXT("Subsurface"); break;
	case MSM_ClearCoat:     ShadingModelStr = TEXT("ClearCoat"); break;
	default:                ShadingModelStr = TEXT("Other"); break;
	}
	InfoResult->SetStringField(TEXT("shading_model"), ShadingModelStr);
	InfoResult->SetBoolField(TEXT("two_sided"), Material->IsTwoSided());

	TArray<TSharedPtr<FJsonValue>> NodesArray;
	TArray<TSharedPtr<FJsonValue>> ConnectionsArray;
	TArray<TSharedPtr<FJsonValue>> MaterialInputsArray;
	TArray<TSharedPtr<FJsonValue>> TextureSamplesArray;
	TArray<TSharedPtr<FJsonValue>> UniqueTexturesArray;
	TArray<TSharedPtr<FJsonValue>> ScalarParamsArray;
	TArray<TSharedPtr<FJsonValue>> VectorParamsArray;
	TArray<TSharedPtr<FJsonValue>> PannersArray;
	TArray<TSharedPtr<FJsonValue>> CustomNodesArray;
	TArray<TSharedPtr<FJsonValue>> FunctionCallsArray;
	TArray<TSharedPtr<FJsonValue>> CommentsArray;

	TMap<FString, int32> UniqueTexturePathToIndex;
	TMap<int32, TSharedPtr<FJsonObject>> NodeByIndex;
	TMap<int32, FString> NodeSearchTextLowerByIndex;
	TMap<int32, FVector2D> NodePositionByIndex;
	TMap<int32, FVector4> CommentBoundsByIndex;

	const UEnum* SamplerTypeEnum = StaticEnum<EMaterialSamplerType>();
	const UEnum* MipModeEnum = StaticEnum<ETextureMipValueMode>();

	for (int32 TargetIndex = 0; TargetIndex < Expressions.Num(); ++TargetIndex)
	{
		UMaterialExpression* Expr = Expressions[TargetIndex];
		if (!Expr)
		{
			continue;
		}

		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetNumberField(TEXT("index"), TargetIndex);
		NodeObj->SetStringField(TEXT("class"), Expr->GetClass()->GetName());
		NodeObj->SetStringField(TEXT("description"), Expr->GetDescription());
		NodeObj->SetStringField(TEXT("path"), Expr->GetPathName());
		NodeObj->SetNumberField(TEXT("editor_x"), Expr->MaterialExpressionEditorX);
		NodeObj->SetNumberField(TEXT("editor_y"), Expr->MaterialExpressionEditorY);
		NodeObj->SetStringField(TEXT("practical_name"), GetMaterialExpressionPracticalName(Expr));
		NodePositionByIndex.Add(
			TargetIndex,
			FVector2D(
				static_cast<float>(Expr->MaterialExpressionEditorX),
				static_cast<float>(Expr->MaterialExpressionEditorY)));
		FString SearchText = Expr->GetClass()->GetName() + TEXT(" ") + Expr->GetDescription();

		TArray<FString> Captions;
		Expr->GetCaption(Captions);
		if (Captions.Num() > 0)
		{
			const FString CaptionText = FString::Join(Captions, TEXT(" | "));
			NodeObj->SetStringField(TEXT("caption"), CaptionText);
			SearchText += TEXT(" ") + CaptionText;
		}

		// Inputs + expression-to-expression connections
		TArray<TSharedPtr<FJsonValue>> NodeInputsArray;
		for (FExpressionInputIterator It{ Expr }; It; ++It)
		{
			const int32 InputIndex = It.Index;
			FExpressionInput* ExprInput = It.Input;
			if (!ExprInput)
			{
				continue;
			}

			TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject());
			InputObj->SetNumberField(TEXT("input_index"), InputIndex);

			FName InputName = Expr->GetInputName(InputIndex);
			InputObj->SetStringField(
				TEXT("input_name"),
				InputName.IsNone() ? FString::Printf(TEXT("Input%d"), InputIndex) : InputName.ToString());

			const bool bConnected = (ExprInput->Expression != nullptr);
			InputObj->SetBoolField(TEXT("connected"), bConnected);
			InputObj->SetNumberField(TEXT("source_output_index"), ExprInput->OutputIndex);

			if (bConnected)
			{
				const int32 SourceIndex = ExpressionToIndex.Contains(ExprInput->Expression)
					? ExpressionToIndex[ExprInput->Expression]
					: -1;
				InputObj->SetNumberField(TEXT("source_index"), SourceIndex);
				InputObj->SetStringField(TEXT("source_class"), ExprInput->Expression->GetClass()->GetName());
				InputObj->SetStringField(
					TEXT("source_output_name"),
					ResolveOutputName(ExprInput->Expression, ExprInput->OutputIndex));

				TSharedPtr<FJsonObject> ConnectionObj = MakeShareable(new FJsonObject());
				ConnectionObj->SetNumberField(TEXT("source_index"), SourceIndex);
				ConnectionObj->SetNumberField(TEXT("source_output_index"), ExprInput->OutputIndex);
				ConnectionObj->SetStringField(
					TEXT("source_output_name"),
					ResolveOutputName(ExprInput->Expression, ExprInput->OutputIndex));
				const TArray<FExpressionOutput>& SourceOutputs = ExprInput->Expression->GetOutputs();
				if (SourceOutputs.IsValidIndex(ExprInput->OutputIndex))
				{
					const uint32 OutputMask = ResolveOutputMask(SourceOutputs[ExprInput->OutputIndex]);
					ConnectionObj->SetStringField(TEXT("source_mask_label"), GetMaskLabel(OutputMask));
					ConnectionObj->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
					ConnectionObj->SetStringField(
						TEXT("summary"),
						BuildMaterialConnectionSummary(
							ExprInput->Expression,
							ResolveOutputName(ExprInput->Expression, ExprInput->OutputIndex),
							GetMaskLabel(OutputMask),
							GetMaterialExpressionPracticalName(Expr),
							InputObj->GetStringField(TEXT("input_name"))));
				}
				ConnectionObj->SetStringField(TEXT("target_type"), TEXT("expression"));
				ConnectionObj->SetNumberField(TEXT("target_index"), TargetIndex);
				ConnectionObj->SetNumberField(TEXT("target_input_index"), InputIndex);
				ConnectionObj->SetStringField(TEXT("target_input_name"), InputObj->GetStringField(TEXT("input_name")));
				ConnectionsArray.Add(MakeShareable(new FJsonValueObject(ConnectionObj)));
			}

			NodeInputsArray.Add(MakeShareable(new FJsonValueObject(InputObj)));
		}
		NodeObj->SetArrayField(TEXT("inputs"), NodeInputsArray);

		// Outputs
		TArray<TSharedPtr<FJsonValue>> NodeOutputsArray;
		const TArray<FExpressionOutput>& Outputs = Expr->GetOutputs();
		for (int32 OutputIndex = 0; OutputIndex < Outputs.Num(); ++OutputIndex)
		{
			const FExpressionOutput& Output = Outputs[OutputIndex];
			TSharedPtr<FJsonObject> OutputObj = MakeShareable(new FJsonObject());
			OutputObj->SetNumberField(TEXT("output_index"), OutputIndex);
			OutputObj->SetStringField(
				TEXT("output_name"),
				Output.OutputName.IsNone() ? FString::Printf(TEXT("Output%d"), OutputIndex) : Output.OutputName.ToString());
			OutputObj->SetNumberField(TEXT("mask"), Output.Mask);
			OutputObj->SetBoolField(TEXT("mask_r"), Output.MaskR != 0);
			OutputObj->SetBoolField(TEXT("mask_g"), Output.MaskG != 0);
			OutputObj->SetBoolField(TEXT("mask_b"), Output.MaskB != 0);
			OutputObj->SetBoolField(TEXT("mask_a"), Output.MaskA != 0);
			const int32 ChannelCount =
				(Output.MaskR != 0 ? 1 : 0) +
				(Output.MaskG != 0 ? 1 : 0) +
				(Output.MaskB != 0 ? 1 : 0) +
				(Output.MaskA != 0 ? 1 : 0);
			OutputObj->SetNumberField(TEXT("channel_count"), ChannelCount > 0 ? ChannelCount : 1);
			NodeOutputsArray.Add(MakeShareable(new FJsonValueObject(OutputObj)));
		}
		NodeObj->SetArrayField(TEXT("outputs"), NodeOutputsArray);

		// Type-specific details
		if (UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expr))
		{
			NodeObj->SetStringField(TEXT("param_name"), ScalarParam->ParameterName.ToString());
			NodeObj->SetNumberField(TEXT("default_value"), ScalarParam->DefaultValue);
			NodeObj->SetNumberField(TEXT("slider_min"), ScalarParam->SliderMin);
			NodeObj->SetNumberField(TEXT("slider_max"), ScalarParam->SliderMax);

			TSharedPtr<FJsonObject> ScalarObj = MakeShareable(new FJsonObject());
			ScalarObj->SetStringField(TEXT("name"), ScalarParam->ParameterName.ToString());
			ScalarObj->SetNumberField(TEXT("default_value"), ScalarParam->DefaultValue);
			ScalarObj->SetNumberField(TEXT("slider_min"), ScalarParam->SliderMin);
			ScalarObj->SetNumberField(TEXT("slider_max"), ScalarParam->SliderMax);
			ScalarObj->SetStringField(TEXT("group"), ScalarParam->Group.ToString());
			ScalarObj->SetNumberField(TEXT("sort_priority"), ScalarParam->SortPriority);
			ScalarObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			ScalarParamsArray.Add(MakeShareable(new FJsonValueObject(ScalarObj)));
			SearchText += TEXT(" ") + ScalarParam->ParameterName.ToString();
		}
		else if (UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expr))
		{
			NodeObj->SetStringField(TEXT("param_name"), VectorParam->ParameterName.ToString());
			TArray<TSharedPtr<FJsonValue>> ColorArray;
			ColorArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.R)));
			ColorArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.G)));
			ColorArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.B)));
			ColorArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.A)));
			NodeObj->SetArrayField(TEXT("default_value_rgba"), ColorArray);

			TSharedPtr<FJsonObject> VectorObj = MakeShareable(new FJsonObject());
			VectorObj->SetStringField(TEXT("name"), VectorParam->ParameterName.ToString());
			VectorObj->SetArrayField(TEXT("default_value_rgba"), ColorArray);
			VectorObj->SetStringField(TEXT("group"), VectorParam->Group.ToString());
			VectorObj->SetNumberField(TEXT("sort_priority"), VectorParam->SortPriority);
			VectorObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			VectorParamsArray.Add(MakeShareable(new FJsonValueObject(VectorObj)));
			SearchText += TEXT(" ") + VectorParam->ParameterName.ToString();
		}
		else if (UMaterialExpressionTextureSample* TextureSample = Cast<UMaterialExpressionTextureSample>(Expr))
		{
			TSharedPtr<FJsonObject> SampleObj = MakeShareable(new FJsonObject());
			SampleObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			SampleObj->SetStringField(TEXT("sampler_type"), EnumValueToString(SamplerTypeEnum, static_cast<int64>(TextureSample->SamplerType)));
			SampleObj->SetStringField(TEXT("mip_value_mode"), EnumValueToString(MipModeEnum, static_cast<int64>(TextureSample->MipValueMode)));

			if (TextureSample->Texture)
			{
				TSharedPtr<FJsonObject> TextureObj = MakeTextureInfoObject(TextureSample->Texture);
				SampleObj->SetObjectField(TEXT("texture"), TextureObj);
				NodeObj->SetObjectField(TEXT("texture"), TextureObj);

				const FString TexturePath = TextureSample->Texture->GetPathName();
				int32 UniqueTextureIdx = INDEX_NONE;
				if (const int32* ExistingIndex = UniqueTexturePathToIndex.Find(TexturePath))
				{
					UniqueTextureIdx = *ExistingIndex;
					TSharedPtr<FJsonObject> ExistingTexObj = UniqueTexturesArray[UniqueTextureIdx]->AsObject();
					TArray<TSharedPtr<FJsonValue>> ReferencedBy = ExistingTexObj->GetArrayField(TEXT("referenced_by_expression_indices"));
					ReferencedBy.Add(MakeShareable(new FJsonValueNumber(TargetIndex)));
					ExistingTexObj->SetArrayField(TEXT("referenced_by_expression_indices"), ReferencedBy);
				}
				else
				{
					TSharedPtr<FJsonObject> UniqueTexObj = MakeShareable(new FJsonObject(*TextureObj));
					TArray<TSharedPtr<FJsonValue>> ReferencedBy;
					ReferencedBy.Add(MakeShareable(new FJsonValueNumber(TargetIndex)));
					UniqueTexObj->SetArrayField(TEXT("referenced_by_expression_indices"), ReferencedBy);
					UniqueTextureIdx = UniqueTexturesArray.Num();
					UniqueTexturesArray.Add(MakeShareable(new FJsonValueObject(UniqueTexObj)));
					UniqueTexturePathToIndex.Add(TexturePath, UniqueTextureIdx);
				}

				SearchText += TEXT(" ") + TextureSample->Texture->GetName() + TEXT(" ") + TextureSample->Texture->GetPathName();
			}

			TextureSamplesArray.Add(MakeShareable(new FJsonValueObject(SampleObj)));
		}
		else if (UMaterialExpressionTextureCoordinate* TexCoordExpr = Cast<UMaterialExpressionTextureCoordinate>(Expr))
		{
			NodeObj->SetNumberField(TEXT("coordinate_index"), TexCoordExpr->CoordinateIndex);
			NodeObj->SetNumberField(TEXT("u_tiling"), TexCoordExpr->UTiling);
			NodeObj->SetNumberField(TEXT("v_tiling"), TexCoordExpr->VTiling);
		}
		else if (UMaterialExpressionPanner* PannerExpr = Cast<UMaterialExpressionPanner>(Expr))
		{
			NodeObj->SetNumberField(TEXT("speed_x"), PannerExpr->SpeedX);
			NodeObj->SetNumberField(TEXT("speed_y"), PannerExpr->SpeedY);

			TSharedPtr<FJsonObject> PannerObj = MakeShareable(new FJsonObject());
			PannerObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			PannerObj->SetNumberField(TEXT("speed_x"), PannerExpr->SpeedX);
			PannerObj->SetNumberField(TEXT("speed_y"), PannerExpr->SpeedY);
			PannersArray.Add(MakeShareable(new FJsonValueObject(PannerObj)));
		}
		else if (UMaterialExpressionPower* PowerExpr = Cast<UMaterialExpressionPower>(Expr))
		{
			NodeObj->SetNumberField(TEXT("const_exponent"), PowerExpr->ConstExponent);
		}
		else if (UMaterialExpressionMultiply* MultiplyExpr = Cast<UMaterialExpressionMultiply>(Expr))
		{
			NodeObj->SetNumberField(TEXT("const_a"), MultiplyExpr->ConstA);
			NodeObj->SetNumberField(TEXT("const_b"), MultiplyExpr->ConstB);
		}
		else if (UMaterialExpressionAdd* AddExpr = Cast<UMaterialExpressionAdd>(Expr))
		{
			NodeObj->SetNumberField(TEXT("const_a"), AddExpr->ConstA);
			NodeObj->SetNumberField(TEXT("const_b"), AddExpr->ConstB);
		}
		else if (UMaterialExpressionComponentMask* ComponentMask = Cast<UMaterialExpressionComponentMask>(Expr))
		{
			NodeObj->SetBoolField(TEXT("mask_r"), ComponentMask->R);
			NodeObj->SetBoolField(TEXT("mask_g"), ComponentMask->G);
			NodeObj->SetBoolField(TEXT("mask_b"), ComponentMask->B);
			NodeObj->SetBoolField(TEXT("mask_a"), ComponentMask->A);
		}
		else if (UMaterialExpressionCustom* CustomExpr = Cast<UMaterialExpressionCustom>(Expr))
		{
			TSharedPtr<FJsonObject> CustomObj = MakeShareable(new FJsonObject());
			CustomObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			CustomObj->SetStringField(TEXT("description"), CustomExpr->Description);
			CustomObj->SetNumberField(TEXT("input_count"), CustomExpr->Inputs.Num());

			TArray<TSharedPtr<FJsonValue>> InputNamesArray;
			for (const FCustomInput& CustomInput : CustomExpr->Inputs)
			{
				InputNamesArray.Add(MakeShareable(new FJsonValueString(CustomInput.InputName.ToString())));
			}
			CustomObj->SetArrayField(TEXT("input_names"), InputNamesArray);

			if (bIncludeCustomCode)
			{
				CustomObj->SetStringField(TEXT("code"), CustomExpr->Code);
			}
			else
			{
				FString CodePreview = CustomExpr->Code.Left(MaxCustomCodeChars);
				if (CustomExpr->Code.Len() > MaxCustomCodeChars)
				{
					CodePreview += TEXT("...");
				}
				CustomObj->SetStringField(TEXT("code_preview"), CodePreview);
				CustomObj->SetNumberField(TEXT("code_length"), CustomExpr->Code.Len());
			}

			NodeObj->SetObjectField(TEXT("custom"), CustomObj);
			CustomNodesArray.Add(MakeShareable(new FJsonValueObject(CustomObj)));
			SearchText += TEXT(" ") + CustomExpr->Description;
		}
		else if (UMaterialExpressionMaterialFunctionCall* FunctionCallExpr = Cast<UMaterialExpressionMaterialFunctionCall>(Expr))
		{
			TSharedPtr<FJsonObject> FunctionObj = MakeShareable(new FJsonObject());
			FunctionObj->SetNumberField(TEXT("expression_index"), TargetIndex);
			FunctionObj->SetStringField(TEXT("description"), FunctionCallExpr->Desc);
			if (FunctionCallExpr->MaterialFunction)
			{
				FunctionObj->SetStringField(TEXT("function_name"), FunctionCallExpr->MaterialFunction->GetName());
				FunctionObj->SetStringField(TEXT("function_path"), FunctionCallExpr->MaterialFunction->GetPathName());
				NodeObj->SetStringField(TEXT("function_name"), FunctionCallExpr->MaterialFunction->GetName());
				NodeObj->SetStringField(TEXT("function_path"), FunctionCallExpr->MaterialFunction->GetPathName());
				SearchText += TEXT(" ") + FunctionCallExpr->MaterialFunction->GetName() + TEXT(" ") + FunctionCallExpr->MaterialFunction->GetPathName();
			}
			FunctionCallsArray.Add(MakeShareable(new FJsonValueObject(FunctionObj)));
		}
		NodeByIndex.Add(TargetIndex, NodeObj);
		NodeSearchTextLowerByIndex.Add(TargetIndex, SearchText.ToLower());
		NodesArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));
	}

	auto AddMaterialRootInput = [&](const TCHAR* InputPinName, const FExpressionInput& RootInput)
	{
		TSharedPtr<FJsonObject> MaterialInputObj = MakeShareable(new FJsonObject());
		MaterialInputObj->SetStringField(TEXT("pin"), InputPinName);
		MaterialInputObj->SetBoolField(TEXT("connected"), RootInput.Expression != nullptr);
		MaterialInputObj->SetNumberField(TEXT("source_output_index"), RootInput.OutputIndex);

		if (RootInput.Expression)
		{
			const int32 SourceIndex = ExpressionToIndex.Contains(RootInput.Expression)
				? ExpressionToIndex[RootInput.Expression]
				: -1;
			MaterialInputObj->SetNumberField(TEXT("source_index"), SourceIndex);
			MaterialInputObj->SetStringField(TEXT("source_class"), RootInput.Expression->GetClass()->GetName());
			MaterialInputObj->SetStringField(
				TEXT("source_output_name"),
				ResolveOutputName(RootInput.Expression, RootInput.OutputIndex));

			TSharedPtr<FJsonObject> ConnectionObj = MakeShareable(new FJsonObject());
			ConnectionObj->SetNumberField(TEXT("source_index"), SourceIndex);
			ConnectionObj->SetNumberField(TEXT("source_output_index"), RootInput.OutputIndex);
			ConnectionObj->SetStringField(
				TEXT("source_output_name"),
				ResolveOutputName(RootInput.Expression, RootInput.OutputIndex));
			const TArray<FExpressionOutput>& SourceOutputs = RootInput.Expression->GetOutputs();
			if (SourceOutputs.IsValidIndex(RootInput.OutputIndex))
			{
				const uint32 OutputMask = ResolveOutputMask(SourceOutputs[RootInput.OutputIndex]);
				ConnectionObj->SetStringField(TEXT("source_mask_label"), GetMaskLabel(OutputMask));
				ConnectionObj->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
				ConnectionObj->SetStringField(
					TEXT("summary"),
					BuildMaterialConnectionSummary(
						RootInput.Expression,
						ResolveOutputName(RootInput.Expression, RootInput.OutputIndex),
						GetMaskLabel(OutputMask),
						TEXT("Material"),
						InputPinName));
			}
			ConnectionObj->SetStringField(TEXT("target_type"), TEXT("material_input"));
			ConnectionObj->SetStringField(TEXT("target_pin"), InputPinName);
			ConnectionsArray.Add(MakeShareable(new FJsonValueObject(ConnectionObj)));
		}

		MaterialInputsArray.Add(MakeShareable(new FJsonValueObject(MaterialInputObj)));
	};

	if (UMaterialEditorOnlyData* EditorOnlyData = Material->GetEditorOnlyData())
	{
		AddMaterialRootInput(TEXT("BaseColor"), EditorOnlyData->BaseColor);
		AddMaterialRootInput(TEXT("Metallic"), EditorOnlyData->Metallic);
		AddMaterialRootInput(TEXT("Specular"), EditorOnlyData->Specular);
		AddMaterialRootInput(TEXT("Roughness"), EditorOnlyData->Roughness);
		AddMaterialRootInput(TEXT("EmissiveColor"), EditorOnlyData->EmissiveColor);
		AddMaterialRootInput(TEXT("Opacity"), EditorOnlyData->Opacity);
		AddMaterialRootInput(TEXT("OpacityMask"), EditorOnlyData->OpacityMask);
		AddMaterialRootInput(TEXT("Normal"), EditorOnlyData->Normal);
		AddMaterialRootInput(TEXT("AmbientOcclusion"), EditorOnlyData->AmbientOcclusion);
		AddMaterialRootInput(TEXT("WorldPositionOffset"), EditorOnlyData->WorldPositionOffset);
		AddMaterialRootInput(TEXT("PixelDepthOffset"), EditorOnlyData->PixelDepthOffset);
		AddMaterialRootInput(TEXT("MaterialAttributes"), EditorOnlyData->MaterialAttributes);
	}

	TArray<TSharedPtr<FJsonValue>> ReferencedTexturePathsArray;
	for (const TSharedPtr<FJsonValue>& TextureValue : UniqueTexturesArray)
	{
		const TSharedPtr<FJsonObject> TextureObj = TextureValue->AsObject();
		if (TextureObj.IsValid() && TextureObj->HasTypedField<EJson::String>(TEXT("path")))
		{
			ReferencedTexturePathsArray.Add(MakeShareable(new FJsonValueString(TextureObj->GetStringField(TEXT("path")))));
		}
	}

	const TArray<UMaterialExpressionComment*> MaterialComments = GetMaterialComments(Material);
	for (int32 CommentArrayIndex = 0; CommentArrayIndex < MaterialComments.Num(); ++CommentArrayIndex)
	{
		UMaterialExpressionComment* CommentExpr = MaterialComments[CommentArrayIndex];
		if (!CommentExpr)
		{
			continue;
		}

		TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
		CommentObj->SetStringField(TEXT("text"), CommentExpr->Text);
		CommentObj->SetNumberField(TEXT("editor_x"), CommentExpr->MaterialExpressionEditorX);
		CommentObj->SetNumberField(TEXT("editor_y"), CommentExpr->MaterialExpressionEditorY);
		CommentObj->SetNumberField(TEXT("width"), CommentExpr->SizeX);
		CommentObj->SetNumberField(TEXT("height"), CommentExpr->SizeY);
		CommentsArray.Add(MakeShareable(new FJsonValueObject(CommentObj)));

		const float MinX = static_cast<float>(CommentExpr->MaterialExpressionEditorX);
		const float MinY = static_cast<float>(CommentExpr->MaterialExpressionEditorY);
		const float MaxX = MinX + static_cast<float>(CommentExpr->SizeX);
		const float MaxY = MinY + static_cast<float>(CommentExpr->SizeY);
		CommentBoundsByIndex.Add(-100000 - CommentArrayIndex, FVector4(MinX, MinY, MaxX, MaxY));
	}

	// Optional focus subgraph extraction to inspect a local block (e.g. Fire Dissolve)
	const bool bFocusRequested = !FocusQuery.IsEmpty() || (FocusExpressionIndex != INDEX_NONE) || bHasFocusRegion;
	if (bFocusRequested)
	{
		TSet<int32> FocusSeeds;
		if (FocusExpressionIndex != INDEX_NONE && NodeByIndex.Contains(FocusExpressionIndex))
		{
			FocusSeeds.Add(FocusExpressionIndex);
		}

		const FString FocusQueryLower = FocusQuery.ToLower();
		if (!FocusQueryLower.IsEmpty())
		{
			for (const TPair<int32, FString>& Pair : NodeSearchTextLowerByIndex)
			{
				if (Pair.Value.Contains(FocusQueryLower))
				{
					FocusSeeds.Add(Pair.Key);
				}
			}
		}

		if (bHasFocusRegion)
		{
			TArray<int32> CommentSeedIndices;

			auto PointInBox = [](const FVector2D& Point, const FVector4& BoxRect) -> bool
			{
				return Point.X >= BoxRect.X && Point.X <= BoxRect.Z && Point.Y >= BoxRect.Y && Point.Y <= BoxRect.W;
			};
			auto RectIntersectsRect = [](const FVector4& A, const FVector4& B) -> bool
			{
				return !(A.Z < B.X || B.Z < A.X || A.W < B.Y || B.W < A.Y);
			};
			auto DistanceSqPointToRect = [](const FVector2D& Point, const FVector4& Rect) -> float
			{
				const float ClampedX = FMath::Clamp(Point.X, Rect.X, Rect.Z);
				const float ClampedY = FMath::Clamp(Point.Y, Rect.Y, Rect.W);
				const float Dx = Point.X - ClampedX;
				const float Dy = Point.Y - ClampedY;
				return Dx * Dx + Dy * Dy;
			};

			if (bUseBoxRegion && bHasFocusRegionBox)
			{
				for (const TPair<int32, FVector2D>& Pair : NodePositionByIndex)
				{
					if (PointInBox(Pair.Value, FocusRegionBox))
					{
						FocusSeeds.Add(Pair.Key);
					}
				}

				for (const TPair<int32, FVector4>& Pair : CommentBoundsByIndex)
				{
					if (RectIntersectsRect(Pair.Value, FocusRegionBox))
					{
						CommentSeedIndices.Add(Pair.Key);
						FocusSeeds.Add(Pair.Key);
					}
				}
			}
			else
			{
				const float RadiusSq = FocusRegionRadius * FocusRegionRadius;
				for (const TPair<int32, FVector2D>& Pair : NodePositionByIndex)
				{
					if (FVector2D::DistSquared(Pair.Value, FocusRegionCenter) <= RadiusSq)
					{
						FocusSeeds.Add(Pair.Key);
					}
				}

				for (const TPair<int32, FVector4>& Pair : CommentBoundsByIndex)
				{
					if (DistanceSqPointToRect(FocusRegionCenter, Pair.Value) <= RadiusSq)
					{
						CommentSeedIndices.Add(Pair.Key);
						FocusSeeds.Add(Pair.Key);
					}
				}
			}

			for (int32 CommentIndex : CommentSeedIndices)
			{
				const FVector4* Bounds = CommentBoundsByIndex.Find(CommentIndex);
				if (!Bounds)
				{
					continue;
				}
				for (const TPair<int32, FVector2D>& NodePair : NodePositionByIndex)
				{
					const FVector2D& Pos = NodePair.Value;
					if (Pos.X >= Bounds->X && Pos.X <= Bounds->Z && Pos.Y >= Bounds->Y && Pos.Y <= Bounds->W)
					{
						FocusSeeds.Add(NodePair.Key);
					}
				}
			}
		}

		TMap<int32, TSet<int32>> Adjacency;
		for (const TSharedPtr<FJsonValue>& ConnectionValue : ConnectionsArray)
		{
			const TSharedPtr<FJsonObject> ConnObj = ConnectionValue->AsObject();
			if (!ConnObj.IsValid())
			{
				continue;
			}

			FString TargetType;
			if (!ConnObj->TryGetStringField(TEXT("target_type"), TargetType))
			{
				continue;
			}
			if (!TargetType.Equals(TEXT("expression"), ESearchCase::IgnoreCase))
			{
				continue;
			}
			if (!ConnObj->HasTypedField<EJson::Number>(TEXT("source_index")) ||
				!ConnObj->HasTypedField<EJson::Number>(TEXT("target_index")))
			{
				continue;
			}

			const int32 SourceIndex = static_cast<int32>(ConnObj->GetNumberField(TEXT("source_index")));
			const int32 TargetIndex = static_cast<int32>(ConnObj->GetNumberField(TEXT("target_index")));
			if (SourceIndex >= 0 && TargetIndex >= 0)
			{
				Adjacency.FindOrAdd(SourceIndex).Add(TargetIndex);
				Adjacency.FindOrAdd(TargetIndex).Add(SourceIndex);
			}
		}

		TSet<int32> FocusNodeSet = FocusSeeds;
		TArray<int32> Frontier = FocusSeeds.Array();
		for (int32 Hop = 0; Hop < FocusHops && Frontier.Num() > 0; ++Hop)
		{
			TArray<int32> NextFrontier;
			for (int32 NodeIndex : Frontier)
			{
				if (const TSet<int32>* Neighbors = Adjacency.Find(NodeIndex))
				{
					for (int32 NeighborIndex : *Neighbors)
					{
						if (!FocusNodeSet.Contains(NeighborIndex))
						{
							FocusNodeSet.Add(NeighborIndex);
							NextFrontier.Add(NeighborIndex);
						}
					}
				}
			}
			Frontier = MoveTemp(NextFrontier);
		}

		TArray<int32> SortedSeedIndices = FocusSeeds.Array();
		SortedSeedIndices.Sort();
		TArray<int32> SortedFocusIndices = FocusNodeSet.Array();
		SortedFocusIndices.Sort();

		TArray<TSharedPtr<FJsonValue>> FocusSeedIndicesArray;
		for (int32 SeedIndex : SortedSeedIndices)
		{
			FocusSeedIndicesArray.Add(MakeShareable(new FJsonValueNumber(SeedIndex)));
		}

		TArray<TSharedPtr<FJsonValue>> FocusNodeIndicesArray;
		TArray<TSharedPtr<FJsonValue>> FocusNodesArray;
		for (int32 NodeIndex : SortedFocusIndices)
		{
			FocusNodeIndicesArray.Add(MakeShareable(new FJsonValueNumber(NodeIndex)));
			if (const TSharedPtr<FJsonObject>* NodeObj = NodeByIndex.Find(NodeIndex))
			{
				FocusNodesArray.Add(MakeShareable(new FJsonValueObject(*NodeObj)));
			}
		}

		TArray<TSharedPtr<FJsonValue>> FocusConnectionsArray;
		for (const TSharedPtr<FJsonValue>& ConnectionValue : ConnectionsArray)
		{
			const TSharedPtr<FJsonObject> ConnObj = ConnectionValue->AsObject();
			if (!ConnObj.IsValid())
			{
				continue;
			}

			FString TargetType;
			if (!ConnObj->TryGetStringField(TEXT("target_type"), TargetType))
			{
				continue;
			}

			if (TargetType.Equals(TEXT("expression"), ESearchCase::IgnoreCase))
			{
				if (!ConnObj->HasTypedField<EJson::Number>(TEXT("source_index")) ||
					!ConnObj->HasTypedField<EJson::Number>(TEXT("target_index")))
				{
					continue;
				}
				const int32 SourceIndex = static_cast<int32>(ConnObj->GetNumberField(TEXT("source_index")));
				const int32 TargetIndex = static_cast<int32>(ConnObj->GetNumberField(TEXT("target_index")));
				if (FocusNodeSet.Contains(SourceIndex) && FocusNodeSet.Contains(TargetIndex))
				{
					FocusConnectionsArray.Add(MakeShareable(new FJsonValueObject(ConnObj)));
				}
			}
			else if (TargetType.Equals(TEXT("material_input"), ESearchCase::IgnoreCase))
			{
				if (!ConnObj->HasTypedField<EJson::Number>(TEXT("source_index")))
				{
					continue;
				}
				const int32 SourceIndex = static_cast<int32>(ConnObj->GetNumberField(TEXT("source_index")));
				if (FocusNodeSet.Contains(SourceIndex))
				{
					FocusConnectionsArray.Add(MakeShareable(new FJsonValueObject(ConnObj)));
				}
			}
		}

		TArray<TSharedPtr<FJsonValue>> FocusMaterialInputsArray;
		for (const TSharedPtr<FJsonValue>& MaterialInputValue : MaterialInputsArray)
		{
			const TSharedPtr<FJsonObject> MaterialInputObj = MaterialInputValue->AsObject();
			if (!MaterialInputObj.IsValid())
			{
				continue;
			}
			if (!MaterialInputObj->HasTypedField<EJson::Boolean>(TEXT("connected")) ||
				!MaterialInputObj->GetBoolField(TEXT("connected")))
			{
				continue;
			}
			if (!MaterialInputObj->HasTypedField<EJson::Number>(TEXT("source_index")))
			{
				continue;
			}
			const int32 SourceIndex = static_cast<int32>(MaterialInputObj->GetNumberField(TEXT("source_index")));
			if (FocusNodeSet.Contains(SourceIndex))
			{
				FocusMaterialInputsArray.Add(MakeShareable(new FJsonValueObject(MaterialInputObj)));
			}
		}

		TArray<TSharedPtr<FJsonValue>> FocusTextureSamplesArray;
		for (const TSharedPtr<FJsonValue>& TextureSampleValue : TextureSamplesArray)
		{
			const TSharedPtr<FJsonObject> SampleObj = TextureSampleValue->AsObject();
			if (!SampleObj.IsValid() || !SampleObj->HasTypedField<EJson::Number>(TEXT("expression_index")))
			{
				continue;
			}
			const int32 ExprIndex = static_cast<int32>(SampleObj->GetNumberField(TEXT("expression_index")));
			if (FocusNodeSet.Contains(ExprIndex))
			{
				FocusTextureSamplesArray.Add(MakeShareable(new FJsonValueObject(SampleObj)));
			}
		}

		TSharedPtr<FJsonObject> FocusObj = MakeShareable(new FJsonObject());
		FocusObj->SetStringField(TEXT("query"), FocusQuery);
		FocusObj->SetNumberField(TEXT("expression_index_request"), FocusExpressionIndex);
		FocusObj->SetNumberField(TEXT("hops"), FocusHops);
		FocusObj->SetBoolField(TEXT("has_region"), bHasFocusRegion);
		FocusObj->SetStringField(TEXT("region_shape"), bUseBoxRegion ? TEXT("box") : TEXT("circle"));
		if (bHasFocusRegion)
		{
			TArray<TSharedPtr<FJsonValue>> RegionCenterArray;
			RegionCenterArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionCenter.X)));
			RegionCenterArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionCenter.Y)));
			FocusObj->SetArrayField(TEXT("region_center"), RegionCenterArray);
			FocusObj->SetNumberField(TEXT("region_radius"), FocusRegionRadius);
			if (bHasFocusRegionBox)
			{
				TArray<TSharedPtr<FJsonValue>> RegionBoxArray;
				RegionBoxArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionBox.X)));
				RegionBoxArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionBox.Y)));
				RegionBoxArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionBox.Z)));
				RegionBoxArray.Add(MakeShareable(new FJsonValueNumber(FocusRegionBox.W)));
				FocusObj->SetArrayField(TEXT("region_box"), RegionBoxArray);
			}
		}
		FocusObj->SetArrayField(TEXT("seed_indices"), FocusSeedIndicesArray);
		FocusObj->SetArrayField(TEXT("node_indices"), FocusNodeIndicesArray);
		FocusObj->SetArrayField(TEXT("nodes"), FocusNodesArray);
		FocusObj->SetArrayField(TEXT("connections"), FocusConnectionsArray);
		FocusObj->SetArrayField(TEXT("material_inputs"), FocusMaterialInputsArray);
		FocusObj->SetArrayField(TEXT("texture_samples"), FocusTextureSamplesArray);
		FocusObj->SetNumberField(TEXT("seed_count"), SortedSeedIndices.Num());
		FocusObj->SetNumberField(TEXT("node_count"), FocusNodesArray.Num());
		FocusObj->SetNumberField(TEXT("connection_count"), FocusConnectionsArray.Num());
		FocusObj->SetNumberField(TEXT("material_input_count"), FocusMaterialInputsArray.Num());
		FocusObj->SetNumberField(TEXT("texture_sample_count"), FocusTextureSamplesArray.Num());
		FocusObj->SetBoolField(TEXT("has_matches"), SortedSeedIndices.Num() > 0);
		if (SortedSeedIndices.Num() == 0)
		{
			FocusObj->SetStringField(TEXT("message"), TEXT("No focus seed nodes matched query/index."));
		}

		InfoResult->SetObjectField(TEXT("focus"), FocusObj);
	}

	// Backward-compatible fields
	InfoResult->SetArrayField(TEXT("expressions"), NodesArray);
	InfoResult->SetNumberField(TEXT("expression_count"), NodesArray.Num());

	// New deep-inspection fields
	InfoResult->SetArrayField(TEXT("nodes"), NodesArray);
	InfoResult->SetArrayField(TEXT("connections"), ConnectionsArray);
	InfoResult->SetArrayField(TEXT("material_inputs"), MaterialInputsArray);
	InfoResult->SetArrayField(TEXT("comments"), CommentsArray);
	InfoResult->SetArrayField(TEXT("texture_samples"), TextureSamplesArray);
	InfoResult->SetArrayField(TEXT("textures"), UniqueTexturesArray);
	InfoResult->SetArrayField(TEXT("referenced_textures"), ReferencedTexturePathsArray);
	InfoResult->SetArrayField(TEXT("panners"), PannersArray);
	InfoResult->SetArrayField(TEXT("custom_nodes"), CustomNodesArray);
	InfoResult->SetArrayField(TEXT("function_calls"), FunctionCallsArray);

	TSharedPtr<FJsonObject> ParametersObj = MakeShareable(new FJsonObject());
	ParametersObj->SetArrayField(TEXT("scalar"), ScalarParamsArray);
	ParametersObj->SetArrayField(TEXT("vector"), VectorParamsArray);
	InfoResult->SetObjectField(TEXT("parameters"), ParametersObj);

	TSharedPtr<FJsonObject> StatsObj = MakeShareable(new FJsonObject());
	StatsObj->SetNumberField(TEXT("expression_count"), NodesArray.Num());
	StatsObj->SetNumberField(TEXT("connection_count"), ConnectionsArray.Num());
	StatsObj->SetNumberField(TEXT("material_input_count"), MaterialInputsArray.Num());
	StatsObj->SetNumberField(TEXT("texture_sample_count"), TextureSamplesArray.Num());
	StatsObj->SetNumberField(TEXT("unique_texture_count"), UniqueTexturesArray.Num());
	StatsObj->SetNumberField(TEXT("scalar_param_count"), ScalarParamsArray.Num());
	StatsObj->SetNumberField(TEXT("vector_param_count"), VectorParamsArray.Num());
	StatsObj->SetNumberField(TEXT("panner_count"), PannersArray.Num());
	StatsObj->SetNumberField(TEXT("custom_node_count"), CustomNodesArray.Num());
	StatsObj->SetNumberField(TEXT("function_call_count"), FunctionCallsArray.Num());
	InfoResult->SetObjectField(TEXT("stats"), StatsObj);

	OutResult = JsonObjToString(InfoResult);
	return true;
}

bool FVFXToolRegistry::Tool_GetSelectedMaterialExpressionDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPathFilter;
	const bool bIncludeLinks = ParseOptionalBoolField(Input, TEXT("include_links"), true);
	const bool bIncludeCommentContext = ParseOptionalBoolField(Input, TEXT("include_comment_context"), true);
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("material_path"), MaterialPathFilter);
	}

	if (!GEditor)
	{
		OutResult = TEXT("Editor is not available.");
		return false;
	}

	USelection* SelectedObjects = GEditor->GetSelectedObjects();
	if (!SelectedObjects)
	{
		OutResult = TEXT("No editor selection available.");
		return false;
	}

	struct FMaterialSelectionCache
	{
		TMap<const UMaterialExpression*, int32> ExpressionToIndex;
		TArray<UMaterialExpressionComment*> Comments;
	};

	TMap<UMaterial*, FMaterialSelectionCache> SelectionCacheByMaterial;
	auto EnsureSelectionCache = [&SelectionCacheByMaterial](UMaterial* Material) -> FMaterialSelectionCache&
	{
		FMaterialSelectionCache* Existing = SelectionCacheByMaterial.Find(Material);
		if (Existing)
		{
			return *Existing;
		}

		FMaterialSelectionCache NewCache;
		if (Material)
		{
			const TArray<UMaterialExpression*>& Expressions = Material->GetExpressionCollection().Expressions;
			for (int32 Index = 0; Index < Expressions.Num(); ++Index)
			{
				UMaterialExpression* Expr = Expressions[Index];
				if (!Expr) continue;
				NewCache.ExpressionToIndex.Add(Expr, Index);
			}

			NewCache.Comments = GetMaterialComments(Material);
		}

		return SelectionCacheByMaterial.Add(Material, MoveTemp(NewCache));
	};

	auto ResolveOutputName = [](UMaterialExpression* SourceExpression, int32 OutputIndex) -> FString
	{
		if (!SourceExpression)
		{
			return TEXT("Output");
		}

		const TArray<FExpressionOutput>& Outputs = SourceExpression->GetOutputs();
		if (Outputs.IsValidIndex(OutputIndex))
		{
			return Outputs[OutputIndex].OutputName.IsNone()
				? FString::Printf(TEXT("Output%d"), OutputIndex)
				: Outputs[OutputIndex].OutputName.ToString();
		}

		return OutputIndex <= 0 ? TEXT("Output") : FString::Printf(TEXT("Output%d"), OutputIndex);
	};

	TArray<TSharedPtr<FJsonValue>> ExpressionArray;
	int32 SelectedMaterialExpressionCount = 0;
	for (FSelectionIterator It(*SelectedObjects); It; ++It)
	{
		UObject* SelectedObj = *It;
		UMaterialExpression* Expression = Cast<UMaterialExpression>(SelectedObj);
		if (!Expression)
		{
			continue;
		}
		++SelectedMaterialExpressionCount;

		UMaterial* OwnerMaterial = Expression->GetTypedOuter<UMaterial>();
		if (!MaterialPathFilter.IsEmpty())
		{
			if (!OwnerMaterial || !OwnerMaterial->GetPathName().Equals(MaterialPathFilter, ESearchCase::IgnoreCase))
			{
				continue;
			}
		}

		int32 ExpressionIndex = INDEX_NONE;
		FMaterialSelectionCache* Cache = nullptr;
		if (OwnerMaterial)
		{
			Cache = &EnsureSelectionCache(OwnerMaterial);
			if (const int32* FoundIndex = Cache->ExpressionToIndex.Find(Expression))
			{
				ExpressionIndex = *FoundIndex;
			}
		}

		TSharedPtr<FJsonObject> ExpressionObj = MakeShareable(new FJsonObject());
		ExpressionObj->SetStringField(TEXT("class"), Expression->GetClass()->GetName());
		ExpressionObj->SetStringField(TEXT("description"), Expression->GetDescription());
		ExpressionObj->SetStringField(TEXT("practical_name"), GetMaterialExpressionPracticalName(Expression));
		ExpressionObj->SetNumberField(TEXT("editor_x"), Expression->MaterialExpressionEditorX);
		ExpressionObj->SetNumberField(TEXT("editor_y"), Expression->MaterialExpressionEditorY);
		ExpressionObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
		ExpressionObj->SetStringField(TEXT("material_path"), OwnerMaterial ? OwnerMaterial->GetPathName() : TEXT(""));

		TArray<FString> Captions;
		Expression->GetCaption(Captions);
		ExpressionObj->SetStringField(TEXT("caption"), FString::Join(Captions, TEXT(" | ")));

		TArray<TSharedPtr<FJsonValue>> InputsArray;
		for (FExpressionInputIterator InputIt{ Expression }; InputIt; ++InputIt)
		{
			const int32 InputIndex = InputIt.Index;
			FExpressionInput* ExprInput = InputIt.Input;
			if (!ExprInput) continue;

			TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject());
			InputObj->SetNumberField(TEXT("input_index"), InputIndex);
			InputObj->SetStringField(TEXT("input_name"), Expression->GetInputName(InputIndex).ToString());
			InputObj->SetBoolField(TEXT("connected"), ExprInput->Expression != nullptr);
			InputObj->SetNumberField(TEXT("source_output_index"), ExprInput->OutputIndex);

			if (bIncludeLinks && ExprInput->Expression)
			{
				InputObj->SetStringField(TEXT("source_class"), ExprInput->Expression->GetClass()->GetName());
				InputObj->SetStringField(TEXT("source_description"), ExprInput->Expression->GetDescription());
				InputObj->SetStringField(TEXT("source_practical_name"), GetMaterialExpressionPracticalName(ExprInput->Expression));
				if (Cache)
				{
					const int32* SourceIndex = Cache->ExpressionToIndex.Find(ExprInput->Expression);
					if (SourceIndex)
					{
						InputObj->SetNumberField(TEXT("source_index"), *SourceIndex);
					}
				}

				const TArray<FExpressionOutput>& SourceOutputs = ExprInput->Expression->GetOutputs();
				if (SourceOutputs.IsValidIndex(ExprInput->OutputIndex))
				{
					const FExpressionOutput& SourceOutput = SourceOutputs[ExprInput->OutputIndex];
					uint32 OutputMask = 0;
					if (SourceOutput.MaskR) OutputMask |= 1;
					if (SourceOutput.MaskG) OutputMask |= 2;
					if (SourceOutput.MaskB) OutputMask |= 4;
					if (SourceOutput.MaskA) OutputMask |= 8;
					if (OutputMask == 0)
					{
						OutputMask = SourceOutput.Mask != 0 ? SourceOutput.Mask : (1 | 2 | 4 | 8);
					}

					InputObj->SetStringField(TEXT("source_mask_label"), GetMaterialMaskLabel(OutputMask));
					InputObj->SetNumberField(TEXT("source_channel_count"), CountMaterialMaskChannels(OutputMask));
				}
			}

			InputsArray.Add(MakeShareable(new FJsonValueObject(InputObj)));
		}
		ExpressionObj->SetArrayField(TEXT("inputs"), InputsArray);

		TArray<TSharedPtr<FJsonValue>> OutputsArray;
		const TArray<FExpressionOutput>& Outputs = Expression->GetOutputs();
		for (int32 OutputIndex = 0; OutputIndex < Outputs.Num(); ++OutputIndex)
		{
			const FExpressionOutput& Output = Outputs[OutputIndex];
			TSharedPtr<FJsonObject> OutputObj = MakeShareable(new FJsonObject());
			OutputObj->SetNumberField(TEXT("output_index"), OutputIndex);
			OutputObj->SetStringField(
				TEXT("output_name"),
				Output.OutputName.IsNone() ? FString::Printf(TEXT("Output%d"), OutputIndex) : Output.OutputName.ToString());
			OutputObj->SetBoolField(TEXT("mask_r"), Output.MaskR != 0);
			OutputObj->SetBoolField(TEXT("mask_g"), Output.MaskG != 0);
			OutputObj->SetBoolField(TEXT("mask_b"), Output.MaskB != 0);
			OutputObj->SetBoolField(TEXT("mask_a"), Output.MaskA != 0);
			const int32 ChannelCount =
				(Output.MaskR != 0 ? 1 : 0) +
				(Output.MaskG != 0 ? 1 : 0) +
				(Output.MaskB != 0 ? 1 : 0) +
				(Output.MaskA != 0 ? 1 : 0);
			OutputObj->SetNumberField(TEXT("channel_count"), ChannelCount > 0 ? ChannelCount : 1);
			OutputsArray.Add(MakeShareable(new FJsonValueObject(OutputObj)));
		}
		ExpressionObj->SetArrayField(TEXT("outputs"), OutputsArray);

		if (bIncludeLinks && OwnerMaterial)
		{
			TArray<TSharedPtr<FJsonValue>> OutgoingLinksArray;
			const TArray<UMaterialExpression*>& AllExpressions = OwnerMaterial->GetExpressionCollection().Expressions;
			for (int32 TargetExprIndex = 0; TargetExprIndex < AllExpressions.Num(); ++TargetExprIndex)
			{
				UMaterialExpression* TargetExpr = AllExpressions[TargetExprIndex];
				if (!TargetExpr) continue;

				for (FExpressionInputIterator TargetIt{ TargetExpr }; TargetIt; ++TargetIt)
				{
					const int32 TargetInputIndex = TargetIt.Index;
					FExpressionInput* TargetInput = TargetIt.Input;
					if (!TargetInput || TargetInput->Expression != Expression)
					{
						continue;
					}

					TSharedPtr<FJsonObject> LinkObj = MakeShareable(new FJsonObject());
					LinkObj->SetStringField(TEXT("target_type"), TEXT("expression"));
					LinkObj->SetNumberField(TEXT("target_index"), TargetExprIndex);
					LinkObj->SetStringField(TEXT("target_class"), TargetExpr->GetClass()->GetName());
					LinkObj->SetStringField(TEXT("target_description"), TargetExpr->GetDescription());
					LinkObj->SetNumberField(TEXT("target_input_index"), TargetInputIndex);
					LinkObj->SetStringField(TEXT("target_input_name"), TargetExpr->GetInputName(TargetInputIndex).ToString());
					LinkObj->SetNumberField(TEXT("source_output_index"), TargetInput->OutputIndex);
					LinkObj->SetStringField(
						TEXT("summary"),
						BuildMaterialConnectionSummary(
							Expression,
							ResolveOutputName(Expression, TargetInput->OutputIndex),
							TEXT("RGBA"),
							GetMaterialExpressionPracticalName(TargetExpr),
							TargetExpr->GetInputName(TargetInputIndex).ToString()));
					OutgoingLinksArray.Add(MakeShareable(new FJsonValueObject(LinkObj)));
				}
			}

			if (UMaterialEditorOnlyData* EditorOnlyData = OwnerMaterial->GetEditorOnlyData())
			{
				auto TryAddRootLink = [&](const TCHAR* PinName, const FExpressionInput& RootInput)
				{
					if (RootInput.Expression != Expression)
					{
						return;
					}
					TSharedPtr<FJsonObject> LinkObj = MakeShareable(new FJsonObject());
					LinkObj->SetStringField(TEXT("target_type"), TEXT("material_input"));
					LinkObj->SetStringField(TEXT("target_pin"), PinName);
					LinkObj->SetNumberField(TEXT("source_output_index"), RootInput.OutputIndex);
					LinkObj->SetStringField(
						TEXT("summary"),
						BuildMaterialConnectionSummary(
							Expression,
							ResolveOutputName(Expression, RootInput.OutputIndex),
							TEXT("RGBA"),
							TEXT("Material"),
							PinName));
					OutgoingLinksArray.Add(MakeShareable(new FJsonValueObject(LinkObj)));
				};

				TryAddRootLink(TEXT("BaseColor"), EditorOnlyData->BaseColor);
				TryAddRootLink(TEXT("Metallic"), EditorOnlyData->Metallic);
				TryAddRootLink(TEXT("Specular"), EditorOnlyData->Specular);
				TryAddRootLink(TEXT("Roughness"), EditorOnlyData->Roughness);
				TryAddRootLink(TEXT("EmissiveColor"), EditorOnlyData->EmissiveColor);
				TryAddRootLink(TEXT("Opacity"), EditorOnlyData->Opacity);
				TryAddRootLink(TEXT("OpacityMask"), EditorOnlyData->OpacityMask);
				TryAddRootLink(TEXT("Normal"), EditorOnlyData->Normal);
				TryAddRootLink(TEXT("AmbientOcclusion"), EditorOnlyData->AmbientOcclusion);
				TryAddRootLink(TEXT("WorldPositionOffset"), EditorOnlyData->WorldPositionOffset);
				TryAddRootLink(TEXT("PixelDepthOffset"), EditorOnlyData->PixelDepthOffset);
				TryAddRootLink(TEXT("MaterialAttributes"), EditorOnlyData->MaterialAttributes);
			}

			ExpressionObj->SetArrayField(TEXT("outgoing_links"), OutgoingLinksArray);
		}

		if (UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expression))
		{
			ExpressionObj->SetStringField(TEXT("param_name"), ScalarParam->ParameterName.ToString());
			ExpressionObj->SetNumberField(TEXT("default_value"), ScalarParam->DefaultValue);
		}
		else if (UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expression))
		{
			ExpressionObj->SetStringField(TEXT("param_name"), VectorParam->ParameterName.ToString());
			TArray<TSharedPtr<FJsonValue>> RgbaArray;
			RgbaArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.R)));
			RgbaArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.G)));
			RgbaArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.B)));
			RgbaArray.Add(MakeShareable(new FJsonValueNumber(VectorParam->DefaultValue.A)));
			ExpressionObj->SetArrayField(TEXT("default_value_rgba"), RgbaArray);
		}
		else if (UMaterialExpressionTextureSample* TextureSample = Cast<UMaterialExpressionTextureSample>(Expression))
		{
			if (TextureSample->Texture)
			{
				ExpressionObj->SetObjectField(TEXT("texture"), BuildTextureInfoJson(TextureSample->Texture, false));
			}
		}
		else if (UMaterialExpressionTextureCoordinate* TexCoordExpr = Cast<UMaterialExpressionTextureCoordinate>(Expression))
		{
			ExpressionObj->SetNumberField(TEXT("coordinate_index"), TexCoordExpr->CoordinateIndex);
			ExpressionObj->SetNumberField(TEXT("u_tiling"), TexCoordExpr->UTiling);
			ExpressionObj->SetNumberField(TEXT("v_tiling"), TexCoordExpr->VTiling);
		}
		else if (UMaterialExpressionPanner* PannerExpr = Cast<UMaterialExpressionPanner>(Expression))
		{
			ExpressionObj->SetNumberField(TEXT("speed_x"), PannerExpr->SpeedX);
			ExpressionObj->SetNumberField(TEXT("speed_y"), PannerExpr->SpeedY);
		}
		else if (UMaterialExpressionPower* PowerExpr = Cast<UMaterialExpressionPower>(Expression))
		{
			ExpressionObj->SetNumberField(TEXT("const_exponent"), PowerExpr->ConstExponent);
		}

		if (bIncludeCommentContext && OwnerMaterial && Cache)
		{
			TArray<TSharedPtr<FJsonValue>> ContainingComments;
			const float X = static_cast<float>(Expression->MaterialExpressionEditorX);
			const float Y = static_cast<float>(Expression->MaterialExpressionEditorY);
			for (UMaterialExpressionComment* CommentExpr : Cache->Comments)
			{
				if (!CommentExpr || CommentExpr == Expression)
				{
					continue;
				}

				const float MinX = static_cast<float>(CommentExpr->MaterialExpressionEditorX);
				const float MinY = static_cast<float>(CommentExpr->MaterialExpressionEditorY);
				const float MaxX = MinX + static_cast<float>(CommentExpr->SizeX);
				const float MaxY = MinY + static_cast<float>(CommentExpr->SizeY);
				if (X >= MinX && X <= MaxX && Y >= MinY && Y <= MaxY)
				{
					TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
					CommentObj->SetStringField(TEXT("text"), CommentExpr->Text);
					const int32* CommentIndex = Cache->ExpressionToIndex.Find(CommentExpr);
					if (CommentIndex)
					{
						CommentObj->SetNumberField(TEXT("comment_expression_index"), *CommentIndex);
					}
					CommentObj->SetNumberField(TEXT("x"), CommentExpr->MaterialExpressionEditorX);
					CommentObj->SetNumberField(TEXT("y"), CommentExpr->MaterialExpressionEditorY);
					CommentObj->SetNumberField(TEXT("width"), CommentExpr->SizeX);
					CommentObj->SetNumberField(TEXT("height"), CommentExpr->SizeY);
					ContainingComments.Add(MakeShareable(new FJsonValueObject(CommentObj)));
				}
			}
			ExpressionObj->SetArrayField(TEXT("containing_comments"), ContainingComments);
		}

		ExpressionArray.Add(MakeShareable(new FJsonValueObject(ExpressionObj)));
	}

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), ExpressionArray.Num() > 0);
	ResultObj->SetNumberField(TEXT("selected_expression_count"), SelectedMaterialExpressionCount);
	ResultObj->SetNumberField(TEXT("returned_count"), ExpressionArray.Num());
	ResultObj->SetArrayField(TEXT("expressions"), ExpressionArray);
	ResultObj->SetStringField(TEXT("material_path_filter"), MaterialPathFilter);
	if (ExpressionArray.Num() == 0)
	{
		ResultObj->SetStringField(TEXT("message"), TEXT("No Material expressions are selected (or no selection matched material_path filter)."));
	}

	OutResult = JsonObjToString(ResultObj);
	return ExpressionArray.Num() > 0;
}

bool FVFXToolRegistry::Tool_GetMaterialInstanceInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MIPath;
	if (!Input->TryGetStringField(TEXT("material_instance_path"), MIPath))
	{
		OutResult = TEXT("Missing required field: material_instance_path");
		return false;
	}

	const bool bIncludeInheritedDefaults = ParseOptionalBoolField(Input, TEXT("include_inherited_defaults"), true);
	const bool bOnlyOverridden = ParseOptionalBoolField(Input, TEXT("only_overridden"), true);

	UMaterialInstanceConstant* MIC = Cast<UMaterialInstanceConstant>(UEditorAssetLibrary::LoadAsset(MIPath));
	if (!MIC)
	{
		OutResult = FString::Printf(TEXT("Could not load Material Instance Constant at path: %s"), *MIPath);
		return false;
	}

	FMaterialParameterDefaults ParentDefaults;
	if (bIncludeInheritedDefaults)
	{
		BuildDefaultsFromMaterialInterface(MIC->Parent, ParentDefaults);
	}

	TSet<FName> ScalarOverrideNames;
	TSet<FName> VectorOverrideNames;
	TSet<FName> TextureOverrideNames;

	TArray<TSharedPtr<FJsonValue>> ScalarArray;
	for (const FScalarParameterValue& ScalarValue : MIC->ScalarParameterValues)
	{
		ScalarOverrideNames.Add(ScalarValue.ParameterInfo.Name);
		TSharedPtr<FJsonObject> ScalarObj = MakeShareable(new FJsonObject());
		ScalarObj->SetStringField(TEXT("name"), ScalarValue.ParameterInfo.Name.ToString());
		ScalarObj->SetNumberField(TEXT("value"), ScalarValue.ParameterValue);
		ScalarObj->SetBoolField(TEXT("is_overridden"), true);
		if (bIncludeInheritedDefaults)
		{
			if (const float* ParentValue = ParentDefaults.Scalars.Find(ScalarValue.ParameterInfo.Name))
			{
				ScalarObj->SetNumberField(TEXT("inherited_default"), *ParentValue);
				ScalarObj->SetNumberField(TEXT("delta"), ScalarValue.ParameterValue - *ParentValue);
			}
		}
		ScalarArray.Add(MakeShareable(new FJsonValueObject(ScalarObj)));
	}

	TArray<TSharedPtr<FJsonValue>> VectorArray;
	for (const FVectorParameterValue& VectorValue : MIC->VectorParameterValues)
	{
		VectorOverrideNames.Add(VectorValue.ParameterInfo.Name);
		TSharedPtr<FJsonObject> VectorObj = MakeShareable(new FJsonObject());
		VectorObj->SetStringField(TEXT("name"), VectorValue.ParameterInfo.Name.ToString());

		TArray<TSharedPtr<FJsonValue>> ValueArray;
		ValueArray.Add(MakeShareable(new FJsonValueNumber(VectorValue.ParameterValue.R)));
		ValueArray.Add(MakeShareable(new FJsonValueNumber(VectorValue.ParameterValue.G)));
		ValueArray.Add(MakeShareable(new FJsonValueNumber(VectorValue.ParameterValue.B)));
		ValueArray.Add(MakeShareable(new FJsonValueNumber(VectorValue.ParameterValue.A)));
		VectorObj->SetArrayField(TEXT("value_rgba"), ValueArray);
		VectorObj->SetBoolField(TEXT("is_overridden"), true);

		if (bIncludeInheritedDefaults)
		{
			if (const FLinearColor* ParentValue = ParentDefaults.Vectors.Find(VectorValue.ParameterInfo.Name))
			{
				TArray<TSharedPtr<FJsonValue>> ParentArray;
				ParentArray.Add(MakeShareable(new FJsonValueNumber(ParentValue->R)));
				ParentArray.Add(MakeShareable(new FJsonValueNumber(ParentValue->G)));
				ParentArray.Add(MakeShareable(new FJsonValueNumber(ParentValue->B)));
				ParentArray.Add(MakeShareable(new FJsonValueNumber(ParentValue->A)));
				VectorObj->SetArrayField(TEXT("inherited_default_rgba"), ParentArray);
			}
		}
		VectorArray.Add(MakeShareable(new FJsonValueObject(VectorObj)));
	}

	TArray<TSharedPtr<FJsonValue>> TextureArray;
	for (const FTextureParameterValue& TextureValue : MIC->TextureParameterValues)
	{
		TextureOverrideNames.Add(TextureValue.ParameterInfo.Name);

		TSharedPtr<FJsonObject> TextureObj = MakeShareable(new FJsonObject());
		TextureObj->SetStringField(TEXT("name"), TextureValue.ParameterInfo.Name.ToString());
		TextureObj->SetBoolField(TEXT("is_overridden"), true);
		TextureObj->SetStringField(
			TEXT("value_texture_path"),
			TextureValue.ParameterValue ? TextureValue.ParameterValue->GetPathName() : TEXT(""));
		if (TextureValue.ParameterValue)
		{
			TextureObj->SetObjectField(TEXT("value_texture"), BuildTextureInfoJson(TextureValue.ParameterValue, false));
		}
		if (bIncludeInheritedDefaults)
		{
			if (const FString* ParentTexturePath = ParentDefaults.Textures.Find(TextureValue.ParameterInfo.Name))
			{
				TextureObj->SetStringField(TEXT("inherited_default_texture_path"), *ParentTexturePath);
			}
		}
		TextureArray.Add(MakeShareable(new FJsonValueObject(TextureObj)));
	}

	if (!bOnlyOverridden && bIncludeInheritedDefaults)
	{
		for (const TPair<FName, float>& Pair : ParentDefaults.Scalars)
		{
			if (ScalarOverrideNames.Contains(Pair.Key))
			{
				continue;
			}
			TSharedPtr<FJsonObject> ScalarObj = MakeShareable(new FJsonObject());
			ScalarObj->SetStringField(TEXT("name"), Pair.Key.ToString());
			ScalarObj->SetNumberField(TEXT("value"), Pair.Value);
			ScalarObj->SetNumberField(TEXT("inherited_default"), Pair.Value);
			ScalarObj->SetBoolField(TEXT("is_overridden"), false);
			ScalarArray.Add(MakeShareable(new FJsonValueObject(ScalarObj)));
		}

		for (const TPair<FName, FLinearColor>& Pair : ParentDefaults.Vectors)
		{
			if (VectorOverrideNames.Contains(Pair.Key))
			{
				continue;
			}
			TSharedPtr<FJsonObject> VectorObj = MakeShareable(new FJsonObject());
			VectorObj->SetStringField(TEXT("name"), Pair.Key.ToString());
			TArray<TSharedPtr<FJsonValue>> ValueArray;
			ValueArray.Add(MakeShareable(new FJsonValueNumber(Pair.Value.R)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(Pair.Value.G)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(Pair.Value.B)));
			ValueArray.Add(MakeShareable(new FJsonValueNumber(Pair.Value.A)));
			VectorObj->SetArrayField(TEXT("value_rgba"), ValueArray);
			VectorObj->SetArrayField(TEXT("inherited_default_rgba"), ValueArray);
			VectorObj->SetBoolField(TEXT("is_overridden"), false);
			VectorArray.Add(MakeShareable(new FJsonValueObject(VectorObj)));
		}

		for (const TPair<FName, FString>& Pair : ParentDefaults.Textures)
		{
			if (TextureOverrideNames.Contains(Pair.Key))
			{
				continue;
			}
			TSharedPtr<FJsonObject> TextureObj = MakeShareable(new FJsonObject());
			TextureObj->SetStringField(TEXT("name"), Pair.Key.ToString());
			TextureObj->SetStringField(TEXT("value_texture_path"), Pair.Value);
			TextureObj->SetStringField(TEXT("inherited_default_texture_path"), Pair.Value);
			TextureObj->SetBoolField(TEXT("is_overridden"), false);
			TextureArray.Add(MakeShareable(new FJsonValueObject(TextureObj)));
		}
	}

	TArray<TSharedPtr<FJsonValue>> ParentChainArray;
	UMaterialInterface* ParentInterface = MIC->Parent;
	for (int32 Hop = 0; Hop < 16 && ParentInterface; ++Hop)
	{
		TSharedPtr<FJsonObject> ParentObj = MakeShareable(new FJsonObject());
		ParentObj->SetStringField(TEXT("name"), ParentInterface->GetName());
		ParentObj->SetStringField(TEXT("path"), ParentInterface->GetPathName());
		ParentObj->SetStringField(TEXT("class"), ParentInterface->GetClass()->GetName());
		ParentChainArray.Add(MakeShareable(new FJsonValueObject(ParentObj)));

		if (UMaterialInstance* ParentMI = Cast<UMaterialInstance>(ParentInterface))
		{
			ParentInterface = ParentMI->Parent;
		}
		else
		{
			break;
		}
	}

	TSet<FString> SuspectParameters;
	auto CollectSuspect = [&SuspectParameters](const TArray<TSharedPtr<FJsonValue>>& ParamArray)
	{
		for (const TSharedPtr<FJsonValue>& Value : ParamArray)
		{
			const TSharedPtr<FJsonObject> ParamObj = Value->AsObject();
			if (!ParamObj.IsValid() || !ParamObj->HasTypedField<EJson::String>(TEXT("name")))
			{
				continue;
			}
			const FString Name = ParamObj->GetStringField(TEXT("name"));
			const FString NameLower = Name.ToLower();
			if (NameLower.Contains(TEXT("emiss")) ||
				NameLower.Contains(TEXT("glow")) ||
				NameLower.Contains(TEXT("hot")) ||
				NameLower.Contains(TEXT("heat")) ||
				NameLower.Contains(TEXT("burn")))
			{
				SuspectParameters.Add(Name);
			}
		}
	};
	CollectSuspect(ScalarArray);
	CollectSuspect(VectorArray);
	CollectSuspect(TextureArray);

	TArray<TSharedPtr<FJsonValue>> SuspectArray;
	for (const FString& Suspect : SuspectParameters)
	{
		SuspectArray.Add(MakeShareable(new FJsonValueString(Suspect)));
	}

	TSharedPtr<FJsonObject> OverridesObj = MakeShareable(new FJsonObject());
	OverridesObj->SetArrayField(TEXT("scalar"), ScalarArray);
	OverridesObj->SetArrayField(TEXT("vector"), VectorArray);
	OverridesObj->SetArrayField(TEXT("texture"), TextureArray);

	TSharedPtr<FJsonObject> CountsObj = MakeShareable(new FJsonObject());
	CountsObj->SetNumberField(TEXT("scalar"), ScalarArray.Num());
	CountsObj->SetNumberField(TEXT("vector"), VectorArray.Num());
	CountsObj->SetNumberField(TEXT("texture"), TextureArray.Num());
	CountsObj->SetNumberField(TEXT("total"), ScalarArray.Num() + VectorArray.Num() + TextureArray.Num());

	TSharedPtr<FJsonObject> OptionsObj = MakeShareable(new FJsonObject());
	OptionsObj->SetBoolField(TEXT("include_inherited_defaults"), bIncludeInheritedDefaults);
	OptionsObj->SetBoolField(TEXT("only_overridden"), bOnlyOverridden);

	TSharedPtr<FJsonObject> ResultObj = MakeShareable(new FJsonObject());
	ResultObj->SetBoolField(TEXT("success"), true);
	ResultObj->SetStringField(TEXT("name"), MIC->GetName());
	ResultObj->SetStringField(TEXT("path"), MIC->GetPathName());
	ResultObj->SetStringField(TEXT("class"), MIC->GetClass()->GetName());
	ResultObj->SetStringField(TEXT("parent_path"), MIC->Parent ? MIC->Parent->GetPathName() : TEXT(""));
	ResultObj->SetArrayField(TEXT("parent_chain"), ParentChainArray);
	ResultObj->SetObjectField(TEXT("options"), OptionsObj);
	ResultObj->SetObjectField(TEXT("override_counts"), CountsObj);
	ResultObj->SetBoolField(TEXT("has_emissive_related_overrides"), SuspectArray.Num() > 0);
	ResultObj->SetArrayField(TEXT("suspect_parameters"), SuspectArray);
	ResultObj->SetObjectField(TEXT("overrides"), OverridesObj);

	OutResult = JsonObjToString(ResultObj);
	return true;
}

bool FVFXToolRegistry::Tool_GetTextureInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString TexturePath;
	if (!Input->TryGetStringField(TEXT("texture_path"), TexturePath))
	{
		OutResult = TEXT("Missing required field: texture_path");
		return false;
	}

	auto ParseOptionalBool = [&](const TCHAR* FieldName, bool DefaultValue) -> bool
	{
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
	};

	const bool bIncludeReferencers = ParseOptionalBool(TEXT("include_referencers"), true);

	UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
	if (!Texture)
	{
		OutResult = FString::Printf(TEXT("Could not load Texture at path: %s"), *TexturePath);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetObjectField(TEXT("texture"), BuildTextureInfoJson(Texture, bIncludeReferencers));

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetTextureMaterialInspection(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString TexturePath;
	if (!Input->TryGetStringField(TEXT("texture_path"), TexturePath))
	{
		OutResult = TEXT("Missing required field: texture_path");
		return false;
	}

	const bool bIncludeReferencers = ParseOptionalBoolField(Input, TEXT("include_referencers"), true);
	const bool bIncludeImageBase64 = ParseOptionalBoolField(Input, TEXT("include_image_base64"), true);
	const bool bSavePreview = ParseOptionalBoolField(Input, TEXT("save_preview"), true);

	int32 PreviewSize = 256;
	TryReadOptionalIntField(Input, TEXT("preview_size"), PreviewSize);
	PreviewSize = FMath::Clamp(PreviewSize, 128, 1024);

	UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
	if (!Texture)
	{
		OutResult = FString::Printf(TEXT("Could not load Texture at path: %s"), *TexturePath);
		return false;
	}

	TArray<FColor> PreviewBitmap;
	int32 PreviewWidth = 0;
	int32 PreviewHeight = 0;
	FString PreviewError;
	FString CaptureSource;
	if (!RenderTexturePreviewBitmap(Texture, PreviewSize, PreviewBitmap, PreviewWidth, PreviewHeight, PreviewError, CaptureSource))
	{
		OutResult = PreviewError;
		return false;
	}

	FString OutputDir;
	if (!Input->TryGetStringField(TEXT("output_dir"), OutputDir) || OutputDir.IsEmpty())
	{
		OutputDir = GetDefaultTexturePreviewOutputDir(Texture->GetName());
	}
	IFileManager::Get().MakeDirectory(*OutputDir, true);

	FString FileName = TEXT("texture_preview.png");
	Input->TryGetStringField(TEXT("file_name"), FileName);
	if (!FileName.EndsWith(TEXT(".png"), ESearchCase::IgnoreCase))
	{
		FileName += TEXT(".png");
	}
	const FString OutputPath = FPaths::Combine(OutputDir, FileName);

	TArray64<uint8> PreviewPngBytes;
	if (bIncludeImageBase64 || bSavePreview)
	{
		if (!EncodeColorBufferToPngBytes(PreviewBitmap, PreviewWidth, PreviewHeight, PreviewPngBytes, PreviewError))
		{
			OutResult = PreviewError;
			return false;
		}
	}

	if (bSavePreview)
	{
		if (!FFileHelper::SaveArrayToFile(TArrayView<const uint8>(PreviewPngBytes.GetData(), PreviewPngBytes.Num()), *OutputPath))
		{
			OutResult = FString::Printf(TEXT("Failed to save preview PNG: %s"), *OutputPath);
			return false;
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetObjectField(TEXT("texture"), BuildTextureInfoJson(Texture, bIncludeReferencers));
	Result->SetObjectField(TEXT("material_guidance"), BuildTextureMaterialGuidance(Texture, PreviewBitmap, PreviewWidth, PreviewHeight));

	TSharedPtr<FJsonObject> PreviewObj = MakeShareable(new FJsonObject());
	PreviewObj->SetStringField(TEXT("capture_source"), CaptureSource);
	PreviewObj->SetNumberField(TEXT("width"), PreviewWidth);
	PreviewObj->SetNumberField(TEXT("height"), PreviewHeight);
	PreviewObj->SetStringField(TEXT("image_format"), TEXT("png"));
	PreviewObj->SetBoolField(TEXT("saved_to_disk"), bSavePreview);
	if (bSavePreview)
	{
		PreviewObj->SetStringField(TEXT("output_path"), OutputPath);
	}
	if (bIncludeImageBase64)
	{
		PreviewObj->SetStringField(TEXT("image_base64"), FBase64::Encode(PreviewPngBytes.GetData(), PreviewPngBytes.Num()));
	}
	Result->SetObjectField(TEXT("preview"), PreviewObj);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetSelectedTextureInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	auto ParseOptionalBool = [&](const TCHAR* FieldName, bool DefaultValue) -> bool
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
	};

	const bool bAllSelected = ParseOptionalBool(TEXT("all_selected"), false);
	const bool bIncludeReferencers = ParseOptionalBool(TEXT("include_referencers"), true);

	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	if (SelectedAssets.Num() == 0)
	{
		OutResult = TEXT("No selected assets found in Content Browser.");
		return false;
	}

	TArray<UTexture*> SelectedTextures;
	for (UObject* Asset : SelectedAssets)
	{
		if (UTexture* Texture = Cast<UTexture>(Asset))
		{
			SelectedTextures.Add(Texture);
		}
	}

	if (SelectedTextures.Num() == 0)
	{
		OutResult = TEXT("Selected assets contain no Texture objects.");
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetNumberField(TEXT("selected_texture_count"), SelectedTextures.Num());

	if (!bAllSelected)
	{
		Result->SetObjectField(TEXT("texture"), BuildTextureInfoJson(SelectedTextures[0], bIncludeReferencers));
		Result->SetStringField(TEXT("message"), TEXT("Returned first selected texture. Set all_selected=true to return all selected textures."));
	}
	else
	{
		TArray<TSharedPtr<FJsonValue>> TextureArray;
		for (UTexture* Texture : SelectedTextures)
		{
			TextureArray.Add(MakeShareable(new FJsonValueObject(BuildTextureInfoJson(Texture, bIncludeReferencers))));
		}
		Result->SetArrayField(TEXT("textures"), TextureArray);
	}

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetMaterialScalarParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath, ParamName, DefaultValueStr;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath) ||
		!Input->TryGetStringField(TEXT("param_name"), ParamName) ||
		!Input->TryGetStringField(TEXT("default_value"), DefaultValueStr))
	{
		OutResult = TEXT("Missing required fields: material_path, param_name, default_value");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Could not load Material at path: %s"), *MatPath);
		return false;
	}

	float DefaultValue = FCString::Atof(*DefaultValueStr);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("set_material_scalar_param"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	// Check if parameter already exists
	for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
	{
		if (UMaterialExpressionScalarParameter* Existing = Cast<UMaterialExpressionScalarParameter>(Expr))
		{
			if (Existing->ParameterName.ToString() == ParamName)
			{
				if (!bApplyChanges)
				{
					TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
					DryRunResult->SetBoolField(TEXT("success"), true);
					DryRunResult->SetBoolField(TEXT("dry_run"), true);
					DryRunResult->SetStringField(TEXT("action"), TEXT("update_existing_scalar_parameter"));
					DryRunResult->SetStringField(TEXT("param_name"), ParamName);
					DryRunResult->SetNumberField(TEXT("current_value"), Existing->DefaultValue);
					DryRunResult->SetNumberField(TEXT("target_value"), DefaultValue);
					DryRunResult->SetStringField(TEXT("message"),
						FString::Printf(TEXT("Dry-run: would update existing ScalarParameter '%s' from %.4f to %.4f"),
							*ParamName, Existing->DefaultValue, DefaultValue));
					OutResult = JsonObjToString(DryRunResult);
					return true;
				}

				Existing->DefaultValue = DefaultValue;
				Material->PreEditChange(nullptr);
				Material->PostEditChange();
				Material->MarkPackageDirty();
				FString SaveError;
				if (!SaveMaterialMutationAsset(Material, SaveError))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
				}

				TSharedPtr<FJsonObject> UpdResult = MakeShareable(new FJsonObject());
				UpdResult->SetBoolField(TEXT("success"), true);
				UpdResult->SetBoolField(TEXT("dry_run"), false);
				UpdResult->SetStringField(TEXT("action"), TEXT("update_existing_scalar_parameter"));
				const FString UpdateScalarMessage = FString::Printf(TEXT("Updated existing ScalarParameter '%s' to %.4f"), *ParamName, DefaultValue);
				UpdResult->SetStringField(TEXT("message"), UpdateScalarMessage);
				UpdResult->SetStringField(TEXT("snapshot_path"), SnapshotPath);
				UpdResult->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_scalar_param"), Material, SnapshotPath, true));
				AppendMaterialMutationAudit(UpdResult, TEXT("set_material_scalar_param"), Material, UpdateScalarMessage, nullptr, SnapshotPath);
				OutResult = JsonObjToString(UpdResult);
				return true;
			}
		}
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("action"), TEXT("create_scalar_parameter"));
		DryRunResult->SetStringField(TEXT("param_name"), ParamName);
		DryRunResult->SetNumberField(TEXT("target_value"), DefaultValue);
		DryRunResult->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create ScalarParameter '%s' with default value %.4f"), *ParamName, DefaultValue));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	// Create new scalar parameter expression
	UMaterialExpressionScalarParameter* ScalarParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	ScalarParam->ParameterName = FName(*ParamName);
	ScalarParam->DefaultValue = DefaultValue;
	Material->GetExpressionCollection().AddExpression(ScalarParam);

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();

	TSharedPtr<FJsonObject> ScalarResult = MakeShareable(new FJsonObject());
	ScalarResult->SetBoolField(TEXT("success"), true);
	ScalarResult->SetBoolField(TEXT("dry_run"), false);
	ScalarResult->SetStringField(TEXT("action"), TEXT("create_scalar_parameter"));
	const FString CreateScalarMessage = FString::Printf(TEXT("Created ScalarParameter '%s' with default value %.4f"), *ParamName, DefaultValue);
	ScalarResult->SetStringField(TEXT("message"), CreateScalarMessage);
	ScalarResult->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	ScalarResult->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_scalar_param"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(ScalarResult, TEXT("set_material_scalar_param"), Material, CreateScalarMessage, nullptr, SnapshotPath);

	OutResult = JsonObjToString(ScalarResult);
	return true;
}

bool FVFXToolRegistry::Tool_SetMaterialVectorParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath, ParamName;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath) ||
		!Input->TryGetStringField(TEXT("param_name"), ParamName))
	{
		OutResult = TEXT("Missing required fields: material_path, param_name");
		return false;
	}

	// Parse RGBA
	FLinearColor DefaultColor = FLinearColor::White;
	const TArray<TSharedPtr<FJsonValue>>* ColorArray;
	if (Input->TryGetArrayField(TEXT("default_value"), ColorArray) && ColorArray->Num() >= 3)
	{
		DefaultColor.R = (*ColorArray)[0]->AsNumber();
		DefaultColor.G = (*ColorArray)[1]->AsNumber();
		DefaultColor.B = (*ColorArray)[2]->AsNumber();
		DefaultColor.A = ColorArray->Num() >= 4 ? (*ColorArray)[3]->AsNumber() : 1.0f;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Could not load Material at path: %s"), *MatPath);
		return false;
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("set_material_vector_param"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	// Check if parameter already exists
	for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
	{
		if (UMaterialExpressionVectorParameter* Existing = Cast<UMaterialExpressionVectorParameter>(Expr))
		{
			if (Existing->ParameterName.ToString() == ParamName)
			{
				if (!bApplyChanges)
				{
					TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
					DryRunResult->SetBoolField(TEXT("success"), true);
					DryRunResult->SetBoolField(TEXT("dry_run"), true);
					DryRunResult->SetStringField(TEXT("action"), TEXT("update_existing_vector_parameter"));
					DryRunResult->SetStringField(TEXT("param_name"), ParamName);
					DryRunResult->SetStringField(TEXT("current_value"),
						FString::Printf(TEXT("(%.4f, %.4f, %.4f, %.4f)"),
							Existing->DefaultValue.R, Existing->DefaultValue.G, Existing->DefaultValue.B, Existing->DefaultValue.A));
					DryRunResult->SetStringField(TEXT("target_value"),
						FString::Printf(TEXT("(%.4f, %.4f, %.4f, %.4f)"),
							DefaultColor.R, DefaultColor.G, DefaultColor.B, DefaultColor.A));
					DryRunResult->SetStringField(TEXT("message"),
						FString::Printf(TEXT("Dry-run: would update existing VectorParameter '%s'"), *ParamName));
					OutResult = JsonObjToString(DryRunResult);
					return true;
				}

				Existing->DefaultValue = DefaultColor;
				Material->PreEditChange(nullptr);
				Material->PostEditChange();
				Material->MarkPackageDirty();
				FString SaveError;
				if (!SaveMaterialMutationAsset(Material, SaveError))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
				}

				TSharedPtr<FJsonObject> UpdResult = MakeShareable(new FJsonObject());
				UpdResult->SetBoolField(TEXT("success"), true);
				UpdResult->SetBoolField(TEXT("dry_run"), false);
				UpdResult->SetStringField(TEXT("action"), TEXT("update_existing_vector_parameter"));
				const FString UpdateVectorMessage = FString::Printf(TEXT("Updated existing VectorParameter '%s'"), *ParamName);
				UpdResult->SetStringField(TEXT("message"), UpdateVectorMessage);
				UpdResult->SetStringField(TEXT("snapshot_path"), SnapshotPath);
				UpdResult->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_vector_param"), Material, SnapshotPath, true));
				AppendMaterialMutationAudit(UpdResult, TEXT("set_material_vector_param"), Material, UpdateVectorMessage, nullptr, SnapshotPath);
				OutResult = JsonObjToString(UpdResult);
				return true;
			}
		}
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("action"), TEXT("create_vector_parameter"));
		DryRunResult->SetStringField(TEXT("param_name"), ParamName);
		DryRunResult->SetStringField(TEXT("target_value"),
			FString::Printf(TEXT("(%.4f, %.4f, %.4f, %.4f)"),
				DefaultColor.R, DefaultColor.G, DefaultColor.B, DefaultColor.A));
		DryRunResult->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create VectorParameter '%s'"), *ParamName));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	// Create new vector parameter expression
	UMaterialExpressionVectorParameter* VectorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	VectorParam->ParameterName = FName(*ParamName);
	VectorParam->DefaultValue = DefaultColor;
	Material->GetExpressionCollection().AddExpression(VectorParam);

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();

	TSharedPtr<FJsonObject> VecResult = MakeShareable(new FJsonObject());
	VecResult->SetBoolField(TEXT("success"), true);
	VecResult->SetBoolField(TEXT("dry_run"), false);
	VecResult->SetStringField(TEXT("action"), TEXT("create_vector_parameter"));
	const FString CreateVectorMessage = FString::Printf(TEXT("Created VectorParameter '%s' with RGBA (%.2f, %.2f, %.2f, %.2f)"),
		*ParamName, DefaultColor.R, DefaultColor.G, DefaultColor.B, DefaultColor.A);
	VecResult->SetStringField(TEXT("message"), CreateVectorMessage);
	VecResult->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	VecResult->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_vector_param"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(VecResult, TEXT("set_material_vector_param"), Material, CreateVectorMessage, nullptr, SnapshotPath);

	OutResult = JsonObjToString(VecResult);
	return true;
}

bool FVFXToolRegistry::Tool_SetMaterialTextureParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	FString ParamName;
	FString TexturePath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath) ||
		!Input->TryGetStringField(TEXT("param_name"), ParamName) ||
		!Input->TryGetStringField(TEXT("texture_path"), TexturePath))
	{
		OutResult = TEXT("Missing required fields: material_path, param_name, texture_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Could not load Material at path: %s"), *MatPath);
		return false;
	}

	UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
	if (!Texture)
	{
		OutResult = FString::Printf(TEXT("Could not load Texture at path: %s"), *TexturePath);
		return false;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("set_material_texture_param"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
	{
		if (UMaterialExpressionTextureSampleParameter2D* Existing = Cast<UMaterialExpressionTextureSampleParameter2D>(Expr))
		{
			if (Existing->ParameterName.ToString().Equals(ParamName, ESearchCase::IgnoreCase))
			{
				if (!bApplyChanges)
				{
					TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
					DryRunResult->SetBoolField(TEXT("success"), true);
					DryRunResult->SetBoolField(TEXT("dry_run"), true);
					DryRunResult->SetStringField(TEXT("action"), TEXT("update_existing_texture_parameter"));
					DryRunResult->SetStringField(TEXT("param_name"), ParamName);
					DryRunResult->SetStringField(TEXT("current_texture_path"), Existing->Texture ? Existing->Texture->GetPathName() : TEXT(""));
					DryRunResult->SetStringField(TEXT("target_texture_path"), Texture->GetPathName());
					DryRunResult->SetStringField(TEXT("message"),
						FString::Printf(TEXT("Dry-run: would update existing TextureSampleParameter2D '%s' to '%s'"),
							*ParamName, *Texture->GetPathName()));
					OutResult = JsonObjToString(DryRunResult);
					return true;
				}

				Existing->Texture = Texture;
				ApplyMaterialExpressionPosition(Material, Existing, Input, TEXT("TextureSampleParameter2D"), nullptr);
				Material->PreEditChange(nullptr);
				Material->PostEditChange();
				Material->MarkPackageDirty();
				FString SaveError;
				if (!SaveMaterialMutationAsset(Material, SaveError))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
				}

				TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
				Result->SetBoolField(TEXT("success"), true);
				Result->SetBoolField(TEXT("dry_run"), false);
				Result->SetStringField(TEXT("action"), TEXT("update_existing_texture_parameter"));
				Result->SetStringField(TEXT("param_name"), ParamName);
				Result->SetStringField(TEXT("texture_path"), Texture->GetPathName());
				Result->SetNumberField(TEXT("editor_x"), Existing->MaterialExpressionEditorX);
				Result->SetNumberField(TEXT("editor_y"), Existing->MaterialExpressionEditorY);
				const FString Message = FString::Printf(TEXT("Updated existing TextureSampleParameter2D '%s' to '%s'"), *ParamName, *Texture->GetPathName());
				Result->SetStringField(TEXT("message"), Message);
				Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
				Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_texture_param"), Material, SnapshotPath, true));
				AppendMaterialMutationAudit(Result, TEXT("set_material_texture_param"), Material, Message, nullptr, SnapshotPath);
				OutResult = JsonObjToString(Result);
				return true;
			}
		}
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		ApplyMaterialExpressionPosition(Material, NewObject<UMaterialExpressionTextureSampleParameter2D>(GetTransientPackage()), Input, TEXT("TextureSampleParameter2D"), DryRunResult);
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("action"), TEXT("create_texture_parameter"));
		DryRunResult->SetStringField(TEXT("param_name"), ParamName);
		DryRunResult->SetStringField(TEXT("texture_path"), Texture->GetPathName());
		DryRunResult->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would create TextureSampleParameter2D '%s' using '%s'"), *ParamName, *Texture->GetPathName()));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	UMaterialExpressionTextureSampleParameter2D* TextureParam = NewObject<UMaterialExpressionTextureSampleParameter2D>(Material);
	TextureParam->ParameterName = FName(*ParamName);
	TextureParam->Texture = Texture;
	Material->GetExpressionCollection().AddExpression(TextureParam);
	ApplyMaterialExpressionPosition(Material, TextureParam, Input, TEXT("TextureSampleParameter2D"), nullptr);

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("create_texture_parameter"));
	Result->SetStringField(TEXT("param_name"), ParamName);
	Result->SetStringField(TEXT("texture_path"), Texture->GetPathName());
	Result->SetNumberField(TEXT("editor_x"), TextureParam->MaterialExpressionEditorX);
	Result->SetNumberField(TEXT("editor_y"), TextureParam->MaterialExpressionEditorY);
	const FString Message = FString::Printf(TEXT("Created TextureSampleParameter2D '%s' using '%s'"), *ParamName, *Texture->GetPathName());
	Result->SetStringField(TEXT("message"), Message);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_texture_param"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(Result, TEXT("set_material_texture_param"), Material, Message, nullptr, SnapshotPath);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateMaterialInstance(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString InstanceName, ParentPath;
	if (!Input->TryGetStringField(TEXT("instance_name"), InstanceName) ||
		!Input->TryGetStringField(TEXT("parent_material_path"), ParentPath))
	{
		OutResult = TEXT("Missing required fields: instance_name, parent_material_path");
		return false;
	}

	FString SavePath = TEXT("/Game/");
	Input->TryGetStringField(TEXT("save_path"), SavePath);

	// Load parent material
	UMaterialInterface* ParentMaterial = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(ParentPath));
	if (!ParentMaterial)
	{
		OutResult = FString::Printf(TEXT("Could not load parent Material at path: %s"), *ParentPath);
		return false;
	}

	// Build package path
	FString PackagePath = SavePath;
	if (!PackagePath.EndsWith(TEXT("/")))
	{
		PackagePath += TEXT("/");
	}
	PackagePath += InstanceName;

	if (FPackageName::DoesPackageExist(PackagePath))
	{
		OutResult = FString::Printf(TEXT("Asset already exists at path '%s'."), *PackagePath);
		return false;
	}

	// Create MI using factory
	UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
	Factory->InitialParent = ParentMaterial;

	UPackage* Package = CreatePackage(*PackagePath);
	UMaterialInstanceConstant* MI = Cast<UMaterialInstanceConstant>(
		Factory->FactoryCreateNew(UMaterialInstanceConstant::StaticClass(), Package,
			FName(*InstanceName), RF_Public | RF_Standalone, nullptr, GWarn));

	if (!MI)
	{
		OutResult = TEXT("Failed to create Material Instance.");
		return false;
	}

#if WITH_EDITORONLY_DATA
	MI->PreviewMesh = ParentMaterial->PreviewMesh;
	if (ParentMaterial->ThumbnailInfo)
	{
		MI->ThumbnailInfo = DuplicateObject<UThumbnailInfo>(ParentMaterial->ThumbnailInfo, MI);
	}
#endif

	// Apply scalar overrides
	FString ScalarOverridesStr;
	if (Input->TryGetStringField(TEXT("scalar_overrides"), ScalarOverridesStr) && !ScalarOverridesStr.IsEmpty())
	{
		TSharedPtr<FJsonObject> ScalarOverrides;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ScalarOverridesStr);
		if (FJsonSerializer::Deserialize(Reader, ScalarOverrides) && ScalarOverrides.IsValid())
		{
			for (const auto& Pair : ScalarOverrides->Values)
			{
				MI->SetScalarParameterValueEditorOnly(FName(*Pair.Key), Pair.Value->AsNumber());
			}
		}
	}

	// Apply vector overrides
	FString VectorOverridesStr;
	if (Input->TryGetStringField(TEXT("vector_overrides"), VectorOverridesStr) && !VectorOverridesStr.IsEmpty())
	{
		TSharedPtr<FJsonObject> VectorOverrides;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VectorOverridesStr);
		if (FJsonSerializer::Deserialize(Reader, VectorOverrides) && VectorOverrides.IsValid())
		{
			for (const auto& Pair : VectorOverrides->Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* Arr;
				if (Pair.Value->TryGetArray(Arr) && Arr->Num() >= 3)
				{
					FLinearColor Color;
					Color.R = (*Arr)[0]->AsNumber();
					Color.G = (*Arr)[1]->AsNumber();
					Color.B = (*Arr)[2]->AsNumber();
					Color.A = Arr->Num() >= 4 ? (*Arr)[3]->AsNumber() : 1.0f;
					MI->SetVectorParameterValueEditorOnly(FName(*Pair.Key), Color);
				}
			}
		}
	}

	// Apply texture overrides
	FString TextureOverridesStr;
	if (Input->TryGetStringField(TEXT("texture_overrides"), TextureOverridesStr) && !TextureOverridesStr.IsEmpty())
	{
		TSharedPtr<FJsonObject> TextureOverrides;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextureOverridesStr);
		if (FJsonSerializer::Deserialize(Reader, TextureOverrides) && TextureOverrides.IsValid())
		{
			for (const auto& Pair : TextureOverrides->Values)
			{
				const FString TexturePath = Pair.Value.IsValid() ? Pair.Value->AsString() : FString();
				if (TexturePath.IsEmpty())
				{
					continue;
				}

				UTexture* OverrideTexture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
				if (OverrideTexture)
				{
					MI->SetTextureParameterValueEditorOnly(FName(*Pair.Key), OverrideTexture);
				}
			}
		}
	}

	FAssetRegistryModule::AssetCreated(MI);
	MI->MarkPackageDirty();

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	FString PackageFileName = FPackageName::LongPackageNameToFilename(
		PackagePath, FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, MI, *PackageFileName, SaveArgs);

	TSharedPtr<FJsonObject> MIResult = MakeShareable(new FJsonObject());
	MIResult->SetBoolField(TEXT("success"), true);
	MIResult->SetStringField(TEXT("asset_path"), MI->GetPathName());
	const FString CreateMIMessage = FString::Printf(TEXT("Created Material Instance '%s' from parent '%s'"),
		*InstanceName, *ParentMaterial->GetName());
	MIResult->SetStringField(TEXT("message"), CreateMIMessage);
	AppendMaterialMutationAudit(MIResult, TEXT("create_material_instance"), MI, CreateMIMessage);

	OutResult = JsonObjToString(MIResult);
	return true;
}

bool FVFXToolRegistry::Tool_SetActorMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ActorLabel, MaterialPath;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel) ||
		!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required fields: actor_label, material_path");
		return false;
	}

	int32 MaterialIndex = 0;
	FString MtlIdxStr;
	if (Input->TryGetStringField(TEXT("material_index"), MtlIdxStr))
	{
		MaterialIndex = FCString::Atoi(*MtlIdxStr);
	}

	// Load material
	UMaterialInterface* Material = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Could not load Material at path: %s"), *MaterialPath);
		return false;
	}

	// Find actor by label
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	AActor* FoundActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel() == ActorLabel)
		{
			FoundActor = *It;
			break;
		}
	}

	if (!FoundActor)
	{
		OutResult = FString::Printf(TEXT("Actor with label '%s' not found in level."), *ActorLabel);
		return false;
	}

	// Find mesh component
	UPrimitiveComponent* PrimComp = nullptr;
	TArray<UPrimitiveComponent*> PrimComps;
	FoundActor->GetComponents<UPrimitiveComponent>(PrimComps);
	if (PrimComps.Num() > 0)
	{
		PrimComp = PrimComps[0];
	}

	if (!PrimComp)
	{
		OutResult = FString::Printf(TEXT("Actor '%s' has no primitive component to apply material to."), *ActorLabel);
		return false;
	}

	PrimComp->SetMaterial(MaterialIndex, Material);
	PrimComp->MarkRenderStateDirty();
	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports(true);
	}
	FlushRenderingCommands();

	TSharedPtr<FJsonObject> ActorMatResult = MakeShareable(new FJsonObject());
	ActorMatResult->SetBoolField(TEXT("success"), true);
	ActorMatResult->SetStringField(TEXT("message"),
		FString::Printf(TEXT("Applied material '%s' to actor '%s' on slot %d"),
			*Material->GetName(), *ActorLabel, MaterialIndex));

	OutResult = JsonObjToString(ActorMatResult);
	return true;
}


// ============================================================
// PHASE 3.5: Advanced Material Graph Tool Implementations
// ============================================================

bool FVFXToolRegistry::Tool_AddMaterialExpression(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	FString ExprType;
	if (!Input->TryGetStringField(TEXT("expression_type"), ExprType))
	{
		OutResult = TEXT("Missing required field: expression_type");
		return false;
	}

	// Load the material
	UMaterial* Material = Cast<UMaterial>(
		UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MatPath);
		return false;
	}

	// Optional parameters
	FString ParamName;
	Input->TryGetStringField(TEXT("param_name"), ParamName);
	FString DefaultValueStr;
	Input->TryGetStringField(TEXT("default_value"), DefaultValueStr);
	FString TexturePath;
	Input->TryGetStringField(TEXT("texture_path"), TexturePath);
	FString FunctionPath;
	Input->TryGetStringField(TEXT("function_path"), FunctionPath);
	FString SettingsJson;
	Input->TryGetStringField(TEXT("settings_json"), SettingsJson);
	TSharedPtr<FJsonObject> SettingsObj;
	if (!SettingsJson.IsEmpty())
	{
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(SettingsJson);
		if (!FJsonSerializer::Deserialize(Reader, SettingsObj) || !SettingsObj.IsValid())
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_SETTINGS_JSON"), FString::Printf(TEXT("Failed to parse settings_json for expression '%s'."), *ExprType));
		}
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("add_material_expression"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}
	UObject* ExprOuter = bApplyChanges ? static_cast<UObject*>(Material) : GetTransientPackage();

	UMaterialExpression* NewExpr = nullptr;

	// Create the expression based on type
	if (ExprType.Equals(TEXT("Multiply"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionMultiply>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Add"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionAdd>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Subtract"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionSubtract>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Divide"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionDivide>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Min"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionMin>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Max"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionMax>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Lerp"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionLinearInterpolate>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("TextureSample"), ESearchCase::IgnoreCase))
	{
		auto* TexExpr = NewObject<UMaterialExpressionTextureSample>(ExprOuter);
		if (!TexturePath.IsEmpty())
		{
			UTexture* Tex = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
			if (!Tex)
			{
				OutResult = FString::Printf(TEXT("Texture not found at '%s'"), *TexturePath);
				return false;
			}
			TexExpr->Texture = Tex;
		}
		NewExpr = TexExpr;
	}
	else if (ExprType.Equals(TEXT("TextureSampleParameter2D"), ESearchCase::IgnoreCase))
	{
		auto* TexParamExpr = NewObject<UMaterialExpressionTextureSampleParameter2D>(ExprOuter);
		TexParamExpr->ParameterName = ParamName.IsEmpty() ? FName(TEXT("TextureParam")) : FName(*ParamName);
		if (!TexturePath.IsEmpty())
		{
			UTexture* Tex = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
			if (!Tex)
			{
				OutResult = FString::Printf(TEXT("Texture not found at '%s'"), *TexturePath);
				return false;
			}
			TexParamExpr->Texture = Tex;
		}
		NewExpr = TexParamExpr;
	}
	else if (ExprType.Equals(TEXT("TextureObject"), ESearchCase::IgnoreCase))
	{
		auto* TextureObjectExpr = NewObject<UMaterialExpressionTextureObject>(ExprOuter);
		if (!TexturePath.IsEmpty())
		{
			UTexture* Tex = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
			if (!Tex)
			{
				OutResult = FString::Printf(TEXT("Texture not found at '%s'"), *TexturePath);
				return false;
			}
			TextureObjectExpr->Texture = Tex;
		}
		NewExpr = TextureObjectExpr;
	}
	else if (ExprType.Equals(TEXT("TextureObjectParameter"), ESearchCase::IgnoreCase))
	{
		auto* TextureObjectParamExpr = NewObject<UMaterialExpressionTextureObjectParameter>(ExprOuter);
		TextureObjectParamExpr->ParameterName = ParamName.IsEmpty() ? FName(TEXT("TextureObjectParam")) : FName(*ParamName);
		if (!TexturePath.IsEmpty())
		{
			UTexture* Tex = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
			if (!Tex)
			{
				OutResult = FString::Printf(TEXT("Texture not found at '%s'"), *TexturePath);
				return false;
			}
			TextureObjectParamExpr->Texture = Tex;
		}
		NewExpr = TextureObjectParamExpr;
	}
	else if (ExprType.Equals(TEXT("TexCoord"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionTextureCoordinate>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Constant"), ESearchCase::IgnoreCase))
	{
		auto* ConstExpr = NewObject<UMaterialExpressionConstant>(ExprOuter);
		if (!DefaultValueStr.IsEmpty())
		{
			ConstExpr->R = FCString::Atof(*DefaultValueStr);
		}
		NewExpr = ConstExpr;
	}
	else if (ExprType.Equals(TEXT("Constant2"), ESearchCase::IgnoreCase))
	{
		auto* Const2Expr = NewObject<UMaterialExpressionConstant2Vector>(ExprOuter);
		FLinearColor Val;
		if (ParseColorFromString(DefaultValueStr, Val))
		{
			Const2Expr->R = Val.R;
			Const2Expr->G = Val.G;
		}
		NewExpr = Const2Expr;
	}
	else if (ExprType.Equals(TEXT("Constant3"), ESearchCase::IgnoreCase))
	{
		auto* Const3Expr = NewObject<UMaterialExpressionConstant3Vector>(ExprOuter);
		FLinearColor Val;
		if (ParseColorFromString(DefaultValueStr, Val))
		{
			Const3Expr->Constant = Val;
		}
		NewExpr = Const3Expr;
	}
	else if (ExprType.Equals(TEXT("ScalarParameter"), ESearchCase::IgnoreCase))
	{
		auto* SP = NewObject<UMaterialExpressionScalarParameter>(ExprOuter);
		SP->ParameterName = ParamName.IsEmpty() ? FName(TEXT("ScalarParam")) : FName(*ParamName);
		if (!DefaultValueStr.IsEmpty())
		{
			SP->DefaultValue = FCString::Atof(*DefaultValueStr);
		}
		NewExpr = SP;
	}
	else if (ExprType.Equals(TEXT("VectorParameter"), ESearchCase::IgnoreCase))
	{
		auto* VP = NewObject<UMaterialExpressionVectorParameter>(ExprOuter);
		VP->ParameterName = ParamName.IsEmpty() ? FName(TEXT("VectorParam")) : FName(*ParamName);
		FLinearColor Val;
		if (ParseColorFromString(DefaultValueStr, Val))
		{
			VP->DefaultValue = Val;
		}
		NewExpr = VP;
	}
	else if (ExprType.Equals(TEXT("Fresnel"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionFresnel>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Panner"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionPanner>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Time"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionTime>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Clamp"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionClamp>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("OneMinus"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionOneMinus>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Abs"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionAbs>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Power"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionPower>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("SquareRoot"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Sqrt"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionSquareRoot>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Sine"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionSine>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Cosine"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Cos"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionCosine>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Arctangent"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Atan"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionArctangent>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Arctangent2"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Atan2"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionArctangent2>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Arcsine"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Asin"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionArcsine>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Arccosine"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("Acos"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionArccosine>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Floor"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionFloor>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Frac"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionFrac>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Distance"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionDistance>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Length"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("VectorLength"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionLength>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Fmod"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionFmod>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Ceil"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionCeil>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Round"), ESearchCase::IgnoreCase))
	{
		UClass* RoundClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionRound"));
		if (!RoundClass)
		{
			RoundClass = FindFirstObject<UClass>(TEXT("MaterialExpressionRound"), EFindFirstObjectOptions::NativeFirst);
		}
		if (RoundClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, RoundClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_ROUND_CLASS_NOT_FOUND"), TEXT("Round expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("Sign"), ESearchCase::IgnoreCase))
	{
		UClass* SignClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionSign"));
		if (!SignClass)
		{
			SignClass = FindFirstObject<UClass>(TEXT("MaterialExpressionSign"), EFindFirstObjectOptions::NativeFirst);
		}
		if (SignClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, SignClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SIGN_CLASS_NOT_FOUND"), TEXT("Sign expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("Step"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionStep>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("SmoothStep"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionSmoothStep>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("If"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionIf>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("InverseLinearInterpolate"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("InverseLerp"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionInverseLinearInterpolate>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("DepthFade"), ESearchCase::IgnoreCase))
	{
		UClass* DepthFadeClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionDepthFade"));
		if (!DepthFadeClass)
		{
			DepthFadeClass = FindFirstObject<UClass>(TEXT("MaterialExpressionDepthFade"), EFindFirstObjectOptions::NativeFirst);
		}
		if (DepthFadeClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, DepthFadeClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_DEPTH_FADE_CLASS_NOT_FOUND"), TEXT("DepthFade expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("Saturate"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionSaturate>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("SceneDepth"), ESearchCase::IgnoreCase))
	{
		UClass* SceneDepthClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionSceneDepth"));
		if (!SceneDepthClass)
		{
			SceneDepthClass = FindFirstObject<UClass>(TEXT("MaterialExpressionSceneDepth"), EFindFirstObjectOptions::NativeFirst);
		}
		if (SceneDepthClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, SceneDepthClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SCENE_DEPTH_CLASS_NOT_FOUND"), TEXT("SceneDepth expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("PixelDepth"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionPixelDepth>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("DistanceToNearestSurface"), ESearchCase::IgnoreCase))
	{
		UClass* DistanceToNearestSurfaceClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionDistanceToNearestSurface"));
		if (!DistanceToNearestSurfaceClass)
		{
			DistanceToNearestSurfaceClass = FindFirstObject<UClass>(TEXT("MaterialExpressionDistanceToNearestSurface"), EFindFirstObjectOptions::NativeFirst);
		}
		if (DistanceToNearestSurfaceClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, DistanceToNearestSurfaceClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_DISTANCE_TO_NEAREST_SURFACE_CLASS_NOT_FOUND"), TEXT("DistanceToNearestSurface expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("ComponentMask"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionComponentMask>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("AppendVector"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionAppendVector>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("DotProduct"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionDotProduct>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("CrossProduct"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionCrossProduct>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Normalize"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionNormalize>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("WorldPosition"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionWorldPosition>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("ObjectPositionWS"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("ObjectPosition"), ESearchCase::IgnoreCase))
	{
		UClass* ObjectPositionWSClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionObjectPositionWS"));
		if (!ObjectPositionWSClass)
		{
			ObjectPositionWSClass = FindFirstObject<UClass>(TEXT("MaterialExpressionObjectPositionWS"), EFindFirstObjectOptions::NativeFirst);
		}
		if (ObjectPositionWSClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, ObjectPositionWSClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_OBJECT_POSITION_WS_CLASS_NOT_FOUND"), TEXT("ObjectPositionWS expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("ObjectRadius"), ESearchCase::IgnoreCase))
	{
		UClass* ObjectRadiusClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionObjectRadius"));
		if (!ObjectRadiusClass)
		{
			ObjectRadiusClass = FindFirstObject<UClass>(TEXT("MaterialExpressionObjectRadius"), EFindFirstObjectOptions::NativeFirst);
		}
		if (ObjectRadiusClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, ObjectRadiusClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_OBJECT_RADIUS_CLASS_NOT_FOUND"), TEXT("ObjectRadius expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("CameraVectorWS"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionCameraVectorWS>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("ReflectionVector"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("ReflectionVectorWS"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionReflectionVectorWS>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("DynamicParameter"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionDynamicParameter>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("ParticleSubUV"), ESearchCase::IgnoreCase))
	{
		auto* ParticleSubUVExpr = NewObject<UMaterialExpressionParticleSubUV>(ExprOuter);
		if (!TexturePath.IsEmpty())
		{
			UTexture* Tex = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
			if (!Tex)
			{
				OutResult = FString::Printf(TEXT("Texture not found at '%s'"), *TexturePath);
				return false;
			}
			ParticleSubUVExpr->Texture = Tex;
		}
		NewExpr = ParticleSubUVExpr;
	}
	else if (ExprType.Equals(TEXT("ParticleRelativeTime"), ESearchCase::IgnoreCase))
	{
		UClass* ParticleRelativeTimeClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionParticleRelativeTime"));
		if (!ParticleRelativeTimeClass)
		{
			ParticleRelativeTimeClass = FindFirstObject<UClass>(TEXT("MaterialExpressionParticleRelativeTime"), EFindFirstObjectOptions::NativeFirst);
		}
		if (ParticleRelativeTimeClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, ParticleRelativeTimeClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_PARTICLE_RELATIVE_TIME_CLASS_NOT_FOUND"), TEXT("ParticleRelativeTime expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("ParticleRandom"), ESearchCase::IgnoreCase))
	{
		UClass* ParticleRandomClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionParticleRandom"));
		if (!ParticleRandomClass)
		{
			ParticleRandomClass = FindFirstObject<UClass>(TEXT("MaterialExpressionParticleRandom"), EFindFirstObjectOptions::NativeFirst);
		}
		if (ParticleRandomClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, ParticleRandomClass);
		}
		else
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_PARTICLE_RANDOM_CLASS_NOT_FOUND"), TEXT("ParticleRandom expression class not found. Ensure the Engine module supports it."));
		}
	}
	else if (ExprType.Equals(TEXT("Noise"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionNoise>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("FeatureLevelSwitch"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionFeatureLevelSwitch>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("StaticSwitchParameter"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionStaticSwitchParameter>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("PreSkinnedLocalPosition"), ESearchCase::IgnoreCase) || ExprType.Equals(TEXT("PreSkinnedPosition"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionPreSkinnedPosition>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("VertexNormalWS"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionVertexNormalWS>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Transform"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionTransform>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("TransformPosition"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionTransformPosition>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Tangent"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionTangent>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("BlackBody"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionBlackBody>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("Desaturation"), ESearchCase::IgnoreCase))
	{
		NewExpr = NewObject<UMaterialExpressionDesaturation>(ExprOuter);
	}
	else if (ExprType.Equals(TEXT("MaterialFunctionCall"), ESearchCase::IgnoreCase))
	{
		auto* FunctionCallExpr = NewObject<UMaterialExpressionMaterialFunctionCall>(ExprOuter);
		if (!FunctionPath.IsEmpty())
		{
			UMaterialFunctionInterface* FunctionAsset = Cast<UMaterialFunctionInterface>(UEditorAssetLibrary::LoadAsset(FunctionPath));
			if (!FunctionAsset)
			{
				OutResult = FString::Printf(TEXT("Material function not found at '%s'"), *FunctionPath);
				return false;
			}
			if (!FunctionCallExpr->SetMaterialFunction(FunctionAsset))
			{
				OutResult = FString::Printf(TEXT("Failed to bind material function '%s'"), *FunctionPath);
				return false;
			}
		}
		NewExpr = FunctionCallExpr;
	}
	else if (ExprType.Equals(TEXT("ParticleColor"), ESearchCase::IgnoreCase))
	{
		// Use FindObject to avoid linker issues with UMaterialExpressionParticleColor
		UClass* ParticleColorClass = FindObject<UClass>(nullptr, TEXT("/Script/Engine.MaterialExpressionParticleColor"));
		if (!ParticleColorClass)
		{
			ParticleColorClass = FindFirstObject<UClass>(TEXT("MaterialExpressionParticleColor"), EFindFirstObjectOptions::NativeFirst);
		}
		if (ParticleColorClass)
		{
			NewExpr = NewObject<UMaterialExpression>(ExprOuter, ParticleColorClass);
		}
		else
		{
			OutResult = TEXT("{\"success\":false,\"error\":\"ParticleColor expression class not found. Ensure the Engine module supports it.\"}");
			return false;
		}
	}
	else
	{
		OutResult = FString::Printf(TEXT("Unsupported expression type: '%s'"), *ExprType);
		return false;
	}

	if (!NewExpr)
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"Failed to allocate expression.\"}");
		return false;
	}

	auto ParseNoiseFunctionName = [](const FString& InName, ENoiseFunction& OutNoiseFunction) -> bool
	{
		const FString Name = InName.TrimStartAndEnd();
		if (Name.Equals(TEXT("SimplexTex"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Simplex - Texture Based"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_SimplexTex;
			return true;
		}
		if (Name.Equals(TEXT("GradientTex"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Gradient - Texture Based"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_GradientTex;
			return true;
		}
		if (Name.Equals(TEXT("GradientTex3D"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Fast Gradient - 3D Texture"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_GradientTex3D;
			return true;
		}
		if (Name.Equals(TEXT("GradientALU"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Gradient - Computational"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_GradientALU;
			return true;
		}
		if (Name.Equals(TEXT("ValueALU"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Value - Computational"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_ValueALU;
			return true;
		}
		if (Name.Equals(TEXT("Voronoi"), ESearchCase::IgnoreCase))
		{
			OutNoiseFunction = NOISEFUNCTION_VoronoiALU;
			return true;
		}
		return false;
	};

	auto ParseVectorTransformSource = [](const FString& InName, EMaterialVectorCoordTransformSource& OutValue) -> bool
	{
		const FString Name = InName.TrimStartAndEnd();
		if (Name.Equals(TEXT("Tangent"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_Tangent; return true; }
		if (Name.Equals(TEXT("Local"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_Local; return true; }
		if (Name.Equals(TEXT("World"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_World; return true; }
		if (Name.Equals(TEXT("View"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_View; return true; }
		if (Name.Equals(TEXT("Camera"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_Camera; return true; }
		if (Name.Equals(TEXT("Instance"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Particle"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("InstanceParticle"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMSOURCE_Instance; return true; }
		return false;
	};

	auto ParseVectorTransformDestination = [](const FString& InName, EMaterialVectorCoordTransform& OutValue) -> bool
	{
		const FString Name = InName.TrimStartAndEnd();
		if (Name.Equals(TEXT("Tangent"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_Tangent; return true; }
		if (Name.Equals(TEXT("Local"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_Local; return true; }
		if (Name.Equals(TEXT("World"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_World; return true; }
		if (Name.Equals(TEXT("View"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_View; return true; }
		if (Name.Equals(TEXT("Camera"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_Camera; return true; }
		if (Name.Equals(TEXT("Instance"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Particle"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("InstanceParticle"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORM_Instance; return true; }
		return false;
	};

	auto ParsePositionTransformType = [](const FString& InName, EMaterialPositionTransformSource& OutValue) -> bool
	{
		const FString Name = InName.TrimStartAndEnd();
		if (Name.Equals(TEXT("Local"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_Local; return true; }
		if (Name.Equals(TEXT("World"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("AbsoluteWorld"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_World; return true; }
		if (Name.Equals(TEXT("TranslatedWorld"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("CameraRelativeWorld"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_TranslatedWorld; return true; }
		if (Name.Equals(TEXT("View"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_View; return true; }
		if (Name.Equals(TEXT("Camera"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_Camera; return true; }
		if (Name.Equals(TEXT("Instance"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("Particle"), ESearchCase::IgnoreCase) || Name.Equals(TEXT("InstanceParticle"), ESearchCase::IgnoreCase)) { OutValue = TRANSFORMPOSSOURCE_Instance; return true; }
		return false;
	};

	if (UMaterialExpressionNoise* NoiseExpr = Cast<UMaterialExpressionNoise>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			FString NoiseFunctionName;
			if (SettingsObj->TryGetStringField(TEXT("noise_function"), NoiseFunctionName))
			{
				ENoiseFunction NoiseFunctionValue = NOISEFUNCTION_SimplexTex;
				if (!ParseNoiseFunctionName(NoiseFunctionName, NoiseFunctionValue))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_NOISE_FUNCTION"), FString::Printf(TEXT("Unsupported noise_function '%s'"), *NoiseFunctionName));
				}
				NoiseExpr->NoiseFunction = NoiseFunctionValue;
			}

			double NumberValue = 0.0;
			bool BoolValue = false;
			if (SettingsObj->TryGetNumberField(TEXT("scale"), NumberValue)) NoiseExpr->Scale = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("quality"), NumberValue)) NoiseExpr->Quality = static_cast<int32>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("levels"), NumberValue)) NoiseExpr->Levels = static_cast<int32>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("output_min"), NumberValue)) NoiseExpr->OutputMin = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("output_max"), NumberValue)) NoiseExpr->OutputMax = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("level_scale"), NumberValue)) NoiseExpr->LevelScale = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("repeat_size"), NumberValue)) NoiseExpr->RepeatSize = static_cast<uint32>(NumberValue);
			if (SettingsObj->TryGetBoolField(TEXT("turbulence"), BoolValue)) NoiseExpr->bTurbulence = BoolValue;
			if (SettingsObj->TryGetBoolField(TEXT("tiling"), BoolValue)) NoiseExpr->bTiling = BoolValue;
		}
	}
	else if (UMaterialExpressionTangent* TangentExpr = Cast<UMaterialExpressionTangent>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			double NumberValue = 0.0;
			if (SettingsObj->TryGetNumberField(TEXT("period"), NumberValue))
			{
				TangentExpr->Period = static_cast<float>(NumberValue);
			}
		}
	}
	else if (UMaterialExpressionCosine* CosineExpr = Cast<UMaterialExpressionCosine>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			double NumberValue = 0.0;
			if (SettingsObj->TryGetNumberField(TEXT("period"), NumberValue))
			{
				CosineExpr->Period = static_cast<float>(NumberValue);
			}
		}
	}
	else if (NewExpr && NewExpr->GetClass()->GetName().Equals(TEXT("MaterialExpressionSceneDepth"), ESearchCase::IgnoreCase))
	{
		if (SettingsObj.IsValid())
		{
			FString InputModeName;
			if (SettingsObj->TryGetStringField(TEXT("input_mode"), InputModeName))
			{
				uint8 InputModeValue = 0;
				if (InputModeName.Equals(TEXT("Coordinates"), ESearchCase::IgnoreCase))
				{
					InputModeValue = static_cast<uint8>(EMaterialSceneAttributeInputMode::Coordinates);
				}
				else if (InputModeName.Equals(TEXT("OffsetFraction"), ESearchCase::IgnoreCase))
				{
					InputModeValue = static_cast<uint8>(EMaterialSceneAttributeInputMode::OffsetFraction);
				}
				else
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_SCENE_DEPTH_INPUT_MODE"), FString::Printf(TEXT("Unsupported SceneDepth input_mode '%s'"), *InputModeName));
				}

				if (FByteProperty* InputModeProp = FindFProperty<FByteProperty>(NewExpr->GetClass(), TEXT("InputMode")))
				{
					InputModeProp->SetPropertyValue_InContainer(NewExpr, InputModeValue);
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* ConstInputValues = nullptr;
			if (SettingsObj->TryGetArrayField(TEXT("const_input"), ConstInputValues) && ConstInputValues && ConstInputValues->Num() >= 2)
			{
				if (FStructProperty* ConstInputProp = FindFProperty<FStructProperty>(NewExpr->GetClass(), TEXT("ConstInput")))
				{
					if (FVector2D* ConstInputPtr = ConstInputProp->ContainerPtrToValuePtr<FVector2D>(NewExpr))
					{
						ConstInputPtr->X = static_cast<float>((*ConstInputValues)[0]->AsNumber());
						ConstInputPtr->Y = static_cast<float>((*ConstInputValues)[1]->AsNumber());
					}
				}
			}
		}
	}
	else if (UMaterialExpressionIf* IfExpr = Cast<UMaterialExpressionIf>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			double NumberValue = 0.0;
			if (SettingsObj->TryGetNumberField(TEXT("equals_threshold"), NumberValue))
			{
				IfExpr->EqualsThreshold = static_cast<float>(NumberValue);
			}
			if (SettingsObj->TryGetNumberField(TEXT("const_b"), NumberValue))
			{
				IfExpr->ConstB = static_cast<float>(NumberValue);
			}
		}
	}
	else if (UMaterialExpressionInverseLinearInterpolate* InverseLerpExpr = Cast<UMaterialExpressionInverseLinearInterpolate>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			double NumberValue = 0.0;
			bool BoolValue = false;
			if (SettingsObj->TryGetNumberField(TEXT("const_a"), NumberValue)) InverseLerpExpr->ConstA = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("const_b"), NumberValue)) InverseLerpExpr->ConstB = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetNumberField(TEXT("const_value"), NumberValue)) InverseLerpExpr->ConstValue = static_cast<float>(NumberValue);
			if (SettingsObj->TryGetBoolField(TEXT("clamp_result"), BoolValue)) InverseLerpExpr->bClampResult = BoolValue;
		}
	}
	else if (UMaterialExpressionTransform* TransformExpr = Cast<UMaterialExpressionTransform>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			FString EnumName;
			if (SettingsObj->TryGetStringField(TEXT("source"), EnumName))
			{
				EMaterialVectorCoordTransformSource SourceValue = TRANSFORMSOURCE_Local;
				if (!ParseVectorTransformSource(EnumName, SourceValue))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_TRANSFORM_SOURCE"), FString::Printf(TEXT("Unsupported Transform source '%s'"), *EnumName));
				}
				TransformExpr->TransformSourceType = SourceValue;
			}
			if (SettingsObj->TryGetStringField(TEXT("destination"), EnumName))
			{
				EMaterialVectorCoordTransform DestValue = TRANSFORM_World;
				if (!ParseVectorTransformDestination(EnumName, DestValue))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_TRANSFORM_DESTINATION"), FString::Printf(TEXT("Unsupported Transform destination '%s'"), *EnumName));
				}
				TransformExpr->TransformType = DestValue;
			}
		}
	}
	else if (UMaterialExpressionTransformPosition* TransformPositionExpr = Cast<UMaterialExpressionTransformPosition>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			FString EnumName;
			if (SettingsObj->TryGetStringField(TEXT("source"), EnumName))
			{
				EMaterialPositionTransformSource SourceValue = TRANSFORMPOSSOURCE_Local;
				if (!ParsePositionTransformType(EnumName, SourceValue))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_TRANSFORM_POSITION_SOURCE"), FString::Printf(TEXT("Unsupported TransformPosition source '%s'"), *EnumName));
				}
				TransformPositionExpr->TransformSourceType = SourceValue;
			}
			if (SettingsObj->TryGetStringField(TEXT("destination"), EnumName))
			{
				EMaterialPositionTransformSource DestValue = TRANSFORMPOSSOURCE_World;
				if (!ParsePositionTransformType(EnumName, DestValue))
				{
					return BuildMaterialToolError(OutResult, TEXT("MAT_INVALID_TRANSFORM_POSITION_DESTINATION"), FString::Printf(TEXT("Unsupported TransformPosition destination '%s'"), *EnumName));
				}
				TransformPositionExpr->TransformType = DestValue;
			}
		}
	}
	else if (UMaterialExpressionDesaturation* DesatExpr = Cast<UMaterialExpressionDesaturation>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			const TArray<TSharedPtr<FJsonValue>>* LuminanceValues = nullptr;
			if (SettingsObj->TryGetArrayField(TEXT("luminance_factors"), LuminanceValues) && LuminanceValues && LuminanceValues->Num() >= 3)
			{
				DesatExpr->LuminanceFactors = FLinearColor(
					static_cast<float>((*LuminanceValues)[0]->AsNumber()),
					static_cast<float>((*LuminanceValues)[1]->AsNumber()),
					static_cast<float>((*LuminanceValues)[2]->AsNumber()),
					1.0f);
			}
		}
	}
	else if (UMaterialExpressionStaticSwitchParameter* StaticSwitchExpr = Cast<UMaterialExpressionStaticSwitchParameter>(NewExpr))
	{
		StaticSwitchExpr->ParameterName = ParamName.IsEmpty() ? FName(TEXT("StaticSwitchParam")) : FName(*ParamName);
		if (!DefaultValueStr.IsEmpty())
		{
			StaticSwitchExpr->DefaultValue = DefaultValueStr.Equals(TEXT("true"), ESearchCase::IgnoreCase) || DefaultValueStr.Equals(TEXT("1"), ESearchCase::IgnoreCase);
		}
		if (SettingsObj.IsValid())
		{
			bool BoolValue = false;
			if (SettingsObj->TryGetBoolField(TEXT("default_value"), BoolValue))
			{
				StaticSwitchExpr->DefaultValue = BoolValue;
			}
			if (SettingsObj->TryGetBoolField(TEXT("dynamic_branch"), BoolValue))
			{
				StaticSwitchExpr->DynamicBranch = BoolValue;
			}
		}
	}
	else if (UMaterialExpressionDynamicParameter* DynamicParameterExpr = Cast<UMaterialExpressionDynamicParameter>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			const TArray<TSharedPtr<FJsonValue>>* ParamNamesValues = nullptr;
			if (SettingsObj->TryGetArrayField(TEXT("param_names"), ParamNamesValues) && ParamNamesValues)
			{
				DynamicParameterExpr->ParamNames.Reset();
				for (int32 NameIndex = 0; NameIndex < ParamNamesValues->Num(); ++NameIndex)
				{
					const TSharedPtr<FJsonValue>& NameValue = (*ParamNamesValues)[NameIndex];
					DynamicParameterExpr->ParamNames.Add(NameValue.IsValid() ? NameValue->AsString() : FString());
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* DefaultValueArray = nullptr;
			if (SettingsObj->TryGetArrayField(TEXT("default_value"), DefaultValueArray) && DefaultValueArray && DefaultValueArray->Num() >= 3)
			{
				DynamicParameterExpr->DefaultValue.R = static_cast<float>((*DefaultValueArray)[0]->AsNumber());
				DynamicParameterExpr->DefaultValue.G = static_cast<float>((*DefaultValueArray)[1]->AsNumber());
				DynamicParameterExpr->DefaultValue.B = static_cast<float>((*DefaultValueArray)[2]->AsNumber());
				DynamicParameterExpr->DefaultValue.A = DefaultValueArray->Num() >= 4 ? static_cast<float>((*DefaultValueArray)[3]->AsNumber()) : 1.0f;
			}

			double ParameterIndexValue = 0.0;
			if (SettingsObj->TryGetNumberField(TEXT("parameter_index"), ParameterIndexValue))
			{
				DynamicParameterExpr->ParameterIndex = static_cast<uint32>(FMath::Clamp<int32>(static_cast<int32>(ParameterIndexValue), 0, 3));
			}
		}
	}
	else if (UMaterialExpressionComponentMask* ComponentMaskExpr = Cast<UMaterialExpressionComponentMask>(NewExpr))
	{
		if (SettingsObj.IsValid())
		{
			bool BoolValue = false;
			if (SettingsObj->TryGetBoolField(TEXT("mask_r"), BoolValue)) ComponentMaskExpr->R = BoolValue;
			if (SettingsObj->TryGetBoolField(TEXT("mask_g"), BoolValue)) ComponentMaskExpr->G = BoolValue;
			if (SettingsObj->TryGetBoolField(TEXT("mask_b"), BoolValue)) ComponentMaskExpr->B = BoolValue;
			if (SettingsObj->TryGetBoolField(TEXT("mask_a"), BoolValue)) ComponentMaskExpr->A = BoolValue;
		}
	}
	else if (NewExpr && NewExpr->GetClass()->GetName().Equals(TEXT("MaterialExpressionDepthFade"), ESearchCase::IgnoreCase))
	{
		if (SettingsObj.IsValid())
		{
			double NumberValue = 0.0;
			if (FFloatProperty* OpacityDefaultProp = FindFProperty<FFloatProperty>(NewExpr->GetClass(), TEXT("OpacityDefault")))
			{
				if (SettingsObj->TryGetNumberField(TEXT("opacity_default"), NumberValue))
				{
					OpacityDefaultProp->SetPropertyValue_InContainer(NewExpr, static_cast<float>(NumberValue));
				}
			}
			if (FFloatProperty* FadeDistanceDefaultProp = FindFProperty<FFloatProperty>(NewExpr->GetClass(), TEXT("FadeDistanceDefault")))
			{
				if (SettingsObj->TryGetNumberField(TEXT("fade_distance_default"), NumberValue))
				{
					FadeDistanceDefaultProp->SetPropertyValue_InContainer(NewExpr, static_cast<float>(NumberValue));
				}
			}
		}
	}
	else if (UMaterialExpressionMaterialFunctionCall* FunctionCallExpr = Cast<UMaterialExpressionMaterialFunctionCall>(NewExpr))
	{
		if (!FunctionPath.IsEmpty() && !FunctionCallExpr->MaterialFunction)
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_FUNCTION_BIND_FAILED"), FString::Printf(TEXT("MaterialFunctionCall did not retain function '%s'."), *FunctionPath));
		}
	}

	auto GetMaskLabel = [](uint32 Mask) -> FString
	{
		FString Label;
		if (Mask & 1) Label += TEXT("R");
		if (Mask & 2) Label += TEXT("G");
		if (Mask & 4) Label += TEXT("B");
		if (Mask & 8) Label += TEXT("A");
		return Label.IsEmpty() ? TEXT("None") : Label;
	};

	auto CountChannelsFromMask = [](uint32 Mask) -> int32
	{
		int32 Channels = 0;
		if (Mask & 1) Channels++;
		if (Mask & 2) Channels++;
		if (Mask & 4) Channels++;
		if (Mask & 8) Channels++;
		return Channels;
	};

	auto ResolveOutputMask = [](const FExpressionOutput& Output) -> uint32
	{
		uint32 Mask = 0;
		if (Output.MaskR) Mask |= 1;
		if (Output.MaskG) Mask |= 2;
		if (Output.MaskB) Mask |= 4;
		if (Output.MaskA) Mask |= 8;

		if (Mask == 0)
		{
			if (Output.Mask != 0)
			{
				Mask = Output.Mask;
			}
			else
			{
				Mask = 1 | 2 | 4 | 8;
			}
		}
		return Mask;
	};

	auto BuildOutputPreview = [&](UMaterialExpression* Expr) -> TArray<TSharedPtr<FJsonValue>>
	{
		TArray<TSharedPtr<FJsonValue>> OutputArray;
		if (!Expr)
		{
			return OutputArray;
		}

		const TArray<FExpressionOutput>& Outputs = Expr->GetOutputs();
		for (int32 OutputIndex = 0; OutputIndex < Outputs.Num(); ++OutputIndex)
		{
			const FExpressionOutput& Output = Outputs[OutputIndex];
			const uint32 EffectiveMask = ResolveOutputMask(Output);
			TSharedPtr<FJsonObject> OutputObj = MakeShareable(new FJsonObject());
			OutputObj->SetNumberField(TEXT("index"), OutputIndex);
			OutputObj->SetStringField(TEXT("name"), Output.OutputName.IsNone() ? TEXT("Output") : Output.OutputName.ToString());
			OutputObj->SetNumberField(TEXT("mask"), EffectiveMask);
			OutputObj->SetStringField(TEXT("mask_label"), GetMaskLabel(EffectiveMask));
			OutputObj->SetNumberField(TEXT("channel_count"), CountChannelsFromMask(EffectiveMask));
			OutputArray.Add(MakeShareable(new FJsonValueObject(OutputObj)));
		}
		return OutputArray;
	};

	TArray<FString> CaptionLines;
	NewExpr->GetCaption(CaptionLines);
	const FString Caption = CaptionLines.Num() > 0 ? FString::Join(CaptionLines, TEXT(" | ")) : ExprType;
	const TArray<TSharedPtr<FJsonValue>> OutputPreview = BuildOutputPreview(NewExpr);
	const int32 PredictedExpressionIndex = Material->GetExpressionCollection().Expressions.Num();

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		ApplyMaterialExpressionPosition(Material, NewExpr, Input, ExprType, DryRunResult);
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("action"), TEXT("add_material_expression"));
		DryRunResult->SetStringField(TEXT("expression_type"), ExprType);
		DryRunResult->SetStringField(TEXT("expression_class"), NewExpr->GetClass()->GetName());
		DryRunResult->SetStringField(TEXT("caption"), Caption);
		DryRunResult->SetNumberField(TEXT("predicted_expression_index"), PredictedExpressionIndex);
		DryRunResult->SetArrayField(TEXT("output_preview"), OutputPreview);
		DryRunResult->SetNumberField(TEXT("output_count"), OutputPreview.Num());
		DryRunResult->SetBoolField(TEXT("will_mutate_material"), false);
		DryRunResult->SetStringField(TEXT("material_path"), MatPath);
		if (!ParamName.IsEmpty())
		{
			DryRunResult->SetStringField(TEXT("param_name"), ParamName);
		}
		if (!DefaultValueStr.IsEmpty())
		{
			DryRunResult->SetStringField(TEXT("default_value"), DefaultValueStr);
		}
		if (!TexturePath.IsEmpty())
		{
			DryRunResult->SetStringField(TEXT("texture_path"), TexturePath);
		}
		DryRunResult->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would add %s expression (predicted index %d)"),
				*ExprType, PredictedExpressionIndex));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	// Add to material
	Material->GetExpressionCollection().AddExpression(NewExpr);
	ApplyMaterialExpressionPosition(Material, NewExpr, Input, ExprType, nullptr);

	// Get index
	int32 ExprIndex = Material->GetExpressionCollection().Expressions.Num() - 1;

	// Compile
	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("add_material_expression"));
	Result->SetNumberField(TEXT("expression_index"), ExprIndex);
	Result->SetStringField(TEXT("expression_type"), ExprType);
	Result->SetStringField(TEXT("expression_class"), NewExpr->GetClass()->GetName());
	Result->SetNumberField(TEXT("editor_x"), NewExpr->MaterialExpressionEditorX);
	Result->SetNumberField(TEXT("editor_y"), NewExpr->MaterialExpressionEditorY);
	Result->SetStringField(TEXT("caption"), Caption);
	Result->SetArrayField(TEXT("output_preview"), OutputPreview);
	Result->SetNumberField(TEXT("output_count"), OutputPreview.Num());
	Result->SetBoolField(TEXT("will_mutate_material"), true);
	const FString AddExpressionMessage = FString::Printf(TEXT("Added %s expression at index %d"), *ExprType, ExprIndex);
	Result->SetStringField(TEXT("message"), AddExpressionMessage);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("add_material_expression"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(Result, TEXT("add_material_expression"), Material, AddExpressionMessage, nullptr, SnapshotPath);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateMaterialPatternBlock(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	FString PatternName;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) ||
		!Input->TryGetStringField(TEXT("pattern_name"), PatternName))
	{
		OutResult = TEXT("Missing required fields: material_path, pattern_name");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	if (!IsMaterialCustomHlslExplicitlyAllowed(Input))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_CUSTOM_HLSL_NOT_ALLOWED"),
			TEXT("set_material_custom_node authors a Material Custom/HLSL node, which is less production-friendly than native graph authoring. Re-run with allow_custom_hlsl=true only when that tradeoff is explicitly acceptable."));
	}

	FString Prefix = TEXT("VFX");
	FString TexturePath;
	FString Channels = TEXT("R");
	FString MaterialPin;
	Input->TryGetStringField(TEXT("prefix"), Prefix);
	Input->TryGetStringField(TEXT("texture_path"), TexturePath);
	Input->TryGetStringField(TEXT("channels"), Channels);
	Input->TryGetStringField(TEXT("material_pin"), MaterialPin);

	int32 StartX = 0;
	int32 StartY = 0;
	if (!TryReadOptionalIntField(Input, TEXT("editor_x"), StartX) ||
		!TryReadOptionalIntField(Input, TEXT("editor_y"), StartY))
	{
		ComputeMaterialAutoPlacement(Material, TEXT("Pattern"), StartX, StartY);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("create_material_pattern_block"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	UObject* ExprOuter = bApplyChanges ? static_cast<UObject*>(Material) : GetTransientPackage();
	const int32 BaseIndex = Material->GetExpressionCollection().Expressions.Num();
	TArray<TPair<UMaterialExpression*, FString>> CreatedExpressions;
	TArray<TSharedPtr<FJsonValue>> PlannedNodes;
	TArray<TSharedPtr<FJsonValue>> PlannedConnections;

	auto AddNodePlan = [&](UMaterialExpression* Expr, const FString& Role, const int32 NodeX, const int32 NodeY) -> int32
	{
		Expr->MaterialExpressionEditorX = NodeX;
		Expr->MaterialExpressionEditorY = NodeY;

		const int32 PredictedIndex = BaseIndex + CreatedExpressions.Num();
		CreatedExpressions.Add(TPair<UMaterialExpression*, FString>(Expr, Role));

		TArray<FString> Captions;
		Expr->GetCaption(Captions);
		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetNumberField(TEXT("predicted_expression_index"), PredictedIndex);
		NodeObj->SetStringField(TEXT("role"), Role);
		NodeObj->SetStringField(TEXT("expression_class"), Expr->GetClass()->GetName());
		NodeObj->SetStringField(TEXT("caption"), Captions.Num() > 0 ? FString::Join(Captions, TEXT(" | ")) : Expr->GetClass()->GetName());
		NodeObj->SetNumberField(TEXT("editor_x"), NodeX);
		NodeObj->SetNumberField(TEXT("editor_y"), NodeY);
		PlannedNodes.Add(MakeShareable(new FJsonValueObject(NodeObj)));
		return PredictedIndex;
	};

	auto AddConnectionPlan = [&](const int32 SourceIndex, const FString& SourceRole, const FString& SourceOutput, const FString& TargetLabel)
	{
		TSharedPtr<FJsonObject> ConnectionObj = MakeShareable(new FJsonObject());
		ConnectionObj->SetNumberField(TEXT("source_expression_index"), SourceIndex);
		ConnectionObj->SetStringField(TEXT("source_role"), SourceRole);
		ConnectionObj->SetStringField(TEXT("source_output"), SourceOutput);
		ConnectionObj->SetStringField(TEXT("target"), TargetLabel);
		PlannedConnections.Add(MakeShareable(new FJsonValueObject(ConnectionObj)));
	};

	auto ConnectInput = [](FExpressionInput& ExpressionInput, UMaterialExpression* SourceExpr, const int32 OutputIndex = 0)
	{
		ExpressionInput.Expression = SourceExpr;
		ExpressionInput.OutputIndex = OutputIndex;
	};

	auto ResolveMaterialInputPin = [&](const FString& TargetPin) -> FExpressionInput*
	{
		if (TargetPin.IsEmpty())
		{
			return nullptr;
		}

		auto* EditorData = Material->GetEditorOnlyData();
		if (TargetPin.Equals(TEXT("BaseColor"), ESearchCase::IgnoreCase)) return &EditorData->BaseColor;
		if (TargetPin.Equals(TEXT("Metallic"), ESearchCase::IgnoreCase)) return &EditorData->Metallic;
		if (TargetPin.Equals(TEXT("Roughness"), ESearchCase::IgnoreCase)) return &EditorData->Roughness;
		if (TargetPin.Equals(TEXT("Normal"), ESearchCase::IgnoreCase)) return &EditorData->Normal;
		if (TargetPin.Equals(TEXT("EmissiveColor"), ESearchCase::IgnoreCase)) return &EditorData->EmissiveColor;
		if (TargetPin.Equals(TEXT("Opacity"), ESearchCase::IgnoreCase)) return &EditorData->Opacity;
		if (TargetPin.Equals(TEXT("OpacityMask"), ESearchCase::IgnoreCase)) return &EditorData->OpacityMask;
		if (TargetPin.Equals(TEXT("WorldPositionOffset"), ESearchCase::IgnoreCase)) return &EditorData->WorldPositionOffset;
		return nullptr;
	};

	auto BuildTextureParam = [&](const FString& ParamName, const int32 NodeX, const int32 NodeY) -> UMaterialExpressionTextureSampleParameter2D*
	{
		auto* Expr = NewObject<UMaterialExpressionTextureSampleParameter2D>(ExprOuter);
		Expr->ParameterName = FName(*ParamName);
		if (!TexturePath.IsEmpty())
		{
			if (UTexture* Texture = Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath)))
			{
				Expr->Texture = Texture;
			}
		}
		AddNodePlan(Expr, ParamName, NodeX, NodeY);
		return Expr;
	};

	auto BuildScalarParam = [&](const FString& ParamName, const float DefaultValue, const int32 NodeX, const int32 NodeY) -> UMaterialExpressionScalarParameter*
	{
		auto* Expr = NewObject<UMaterialExpressionScalarParameter>(ExprOuter);
		Expr->ParameterName = FName(*ParamName);
		Expr->DefaultValue = DefaultValue;
		AddNodePlan(Expr, ParamName, NodeX, NodeY);
		return Expr;
	};

	auto BuildVectorParam = [&](const FString& ParamName, const FLinearColor& DefaultValue, const int32 NodeX, const int32 NodeY) -> UMaterialExpressionVectorParameter*
	{
		auto* Expr = NewObject<UMaterialExpressionVectorParameter>(ExprOuter);
		Expr->ParameterName = FName(*ParamName);
		Expr->DefaultValue = DefaultValue;
		AddNodePlan(Expr, ParamName, NodeX, NodeY);
		return Expr;
	};

	auto BuildExpression = [&](UMaterialExpression* Expr, const FString& Role, const int32 NodeX, const int32 NodeY) -> UMaterialExpression*
	{
		AddNodePlan(Expr, Role, NodeX, NodeY);
		return Expr;
	};

	UMaterialExpression* FinalOutputExpr = nullptr;
	FString PatternNameNormalized = PatternName.ToLower();
	if (PatternNameNormalized == TEXT("panner_texture_chain"))
	{
		auto* TexCoord = Cast<UMaterialExpressionTextureCoordinate>(BuildExpression(NewObject<UMaterialExpressionTextureCoordinate>(ExprOuter), TEXT("TexCoord"), StartX, StartY));
		auto* Panner = Cast<UMaterialExpressionPanner>(BuildExpression(NewObject<UMaterialExpressionPanner>(ExprOuter), TEXT("Panner"), StartX + 220, StartY));
		auto* TextureParam = BuildTextureParam(Prefix + TEXT("_Texture"), StartX + 440, StartY);
		ConnectInput(Panner->Coordinate, TexCoord);
		ConnectInput(TextureParam->Coordinates, Panner);
		AddConnectionPlan(BaseIndex + 0, TEXT("TexCoord"), TEXT("Output"), TEXT("Panner.Coordinate"));
		AddConnectionPlan(BaseIndex + 1, TEXT("Panner"), TEXT("Output"), TEXT("TextureSampleParameter2D.Coordinates"));
		FinalOutputExpr = TextureParam;
	}
	else if (PatternNameNormalized == TEXT("emissive_multiply_chain"))
	{
		UMaterialExpression* InputExpr = nullptr;
		if (!TexturePath.IsEmpty())
		{
			InputExpr = BuildTextureParam(Prefix + TEXT("_Texture"), StartX, StartY);
		}
		else
		{
			InputExpr = BuildVectorParam(Prefix + TEXT("_Color"), FLinearColor::White, StartX, StartY);
		}

		auto* IntensityParam = BuildScalarParam(Prefix + TEXT("_Intensity"), 1.0f, StartX, StartY + 180);
		auto* Multiply = Cast<UMaterialExpressionMultiply>(BuildExpression(NewObject<UMaterialExpressionMultiply>(ExprOuter), TEXT("Multiply"), StartX + 260, StartY + 80));
		ConnectInput(Multiply->A, InputExpr);
		ConnectInput(Multiply->B, IntensityParam);
		AddConnectionPlan(BaseIndex + 0, Prefix + TEXT("_Input"), TEXT("Output"), TEXT("Multiply.A"));
		AddConnectionPlan(BaseIndex + 1, Prefix + TEXT("_Intensity"), TEXT("Output"), TEXT("Multiply.B"));
		FinalOutputExpr = Multiply;
	}
	else if (PatternNameNormalized == TEXT("mask_extract_chain"))
	{
		auto* TextureParam = BuildTextureParam(Prefix + TEXT("_MaskTex"), StartX, StartY);
		auto* Mask = Cast<UMaterialExpressionComponentMask>(BuildExpression(NewObject<UMaterialExpressionComponentMask>(ExprOuter), TEXT("ComponentMask"), StartX + 220, StartY));
		const FString ChannelUpper = Channels.ToUpper();
		Mask->R = ChannelUpper.Contains(TEXT("R"));
		Mask->G = ChannelUpper.Contains(TEXT("G"));
		Mask->B = ChannelUpper.Contains(TEXT("B"));
		Mask->A = ChannelUpper.Contains(TEXT("A"));
		ConnectInput(Mask->Input, TextureParam, ChannelUpper == TEXT("RGBA") ? 5 : 0);
		AddConnectionPlan(BaseIndex + 0, Prefix + TEXT("_MaskTex"), TEXT("RGB/RGBA"), TEXT("ComponentMask.Input"));
		FinalOutputExpr = Mask;
	}
	else if (PatternNameNormalized == TEXT("dissolve_edge_chain"))
	{
		auto* TextureParam = BuildTextureParam(Prefix + TEXT("_MaskTex"), StartX, StartY);
		auto* Threshold = BuildScalarParam(Prefix + TEXT("_Threshold"), 0.5f, StartX, StartY + 180);
		auto* EdgeWidth = BuildScalarParam(Prefix + TEXT("_EdgeWidth"), 0.1f, StartX, StartY + 360);
		auto* Subtract = Cast<UMaterialExpressionSubtract>(BuildExpression(NewObject<UMaterialExpressionSubtract>(ExprOuter), TEXT("Subtract"), StartX + 240, StartY + 40));
		auto* Divide = Cast<UMaterialExpressionDivide>(BuildExpression(NewObject<UMaterialExpressionDivide>(ExprOuter), TEXT("Divide"), StartX + 480, StartY + 40));
		auto* Saturate = Cast<UMaterialExpressionSaturate>(BuildExpression(NewObject<UMaterialExpressionSaturate>(ExprOuter), TEXT("Saturate"), StartX + 700, StartY + 40));
		ConnectInput(Subtract->A, TextureParam, 1);
		ConnectInput(Subtract->B, Threshold);
		ConnectInput(Divide->A, Subtract);
		ConnectInput(Divide->B, EdgeWidth);
		ConnectInput(Saturate->Input, Divide);
		AddConnectionPlan(BaseIndex + 0, Prefix + TEXT("_MaskTex"), TEXT("R"), TEXT("Subtract.A"));
		AddConnectionPlan(BaseIndex + 1, Prefix + TEXT("_Threshold"), TEXT("Output"), TEXT("Subtract.B"));
		AddConnectionPlan(BaseIndex + 3, TEXT("Subtract"), TEXT("Output"), TEXT("Divide.A"));
		AddConnectionPlan(BaseIndex + 2, Prefix + TEXT("_EdgeWidth"), TEXT("Output"), TEXT("Divide.B"));
		AddConnectionPlan(BaseIndex + 4, TEXT("Divide"), TEXT("Output"), TEXT("Saturate.Input"));
		FinalOutputExpr = Saturate;
	}
	else
	{
		OutResult = FString::Printf(TEXT("Unsupported pattern_name '%s'"), *PatternName);
		return false;
	}

	if (!MaterialPin.IsEmpty())
	{
		if (!ResolveMaterialInputPin(MaterialPin))
		{
			OutResult = FString::Printf(TEXT("Unsupported material_pin '%s'"), *MaterialPin);
			return false;
		}
		AddConnectionPlan(BaseIndex + CreatedExpressions.Num() - 1, TEXT("FinalOutput"), TEXT("Output"), FString::Printf(TEXT("Material.%s"), *MaterialPin));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("create_material_pattern_block"));
		Result->SetStringField(TEXT("pattern_name"), PatternName);
		Result->SetArrayField(TEXT("created_nodes"), PlannedNodes);
		Result->SetArrayField(TEXT("connections"), PlannedConnections);
		Result->SetStringField(TEXT("message"), FString::Printf(TEXT("Dry-run: would create '%s' pattern with %d node(s)."), *PatternName, CreatedExpressions.Num()));
		OutResult = JsonObjToString(Result);
		return true;
	}

	for (const TPair<UMaterialExpression*, FString>& Pair : CreatedExpressions)
	{
		Material->GetExpressionCollection().AddExpression(Pair.Key);
	}

	if (!MaterialPin.IsEmpty() && FinalOutputExpr)
	{
		if (FExpressionInput* MaterialInput = ResolveMaterialInputPin(MaterialPin))
		{
			MaterialInput->Expression = FinalOutputExpr;
			MaterialInput->OutputIndex = 0;
		}
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TArray<TSharedPtr<FJsonValue>> CreatedIndices;
	for (int32 Index = 0; Index < CreatedExpressions.Num(); ++Index)
	{
		CreatedIndices.Add(MakeShareable(new FJsonValueNumber(BaseIndex + Index)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("create_material_pattern_block"));
	Result->SetStringField(TEXT("pattern_name"), PatternName);
	Result->SetArrayField(TEXT("created_expression_indices"), CreatedIndices);
	Result->SetArrayField(TEXT("created_nodes"), PlannedNodes);
	Result->SetArrayField(TEXT("connections"), PlannedConnections);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("create_material_pattern_block"), Material, SnapshotPath, true));
	Result->SetStringField(TEXT("message"), FString::Printf(TEXT("Created '%s' pattern block with %d node(s)."), *PatternName, CreatedExpressions.Num()));
	AppendMaterialMutationAudit(Result, TEXT("create_material_pattern_block"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateMaterialParameterPack(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	FString PackName;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) ||
		!Input->TryGetStringField(TEXT("pack_name"), PackName))
	{
		OutResult = TEXT("Missing required fields: material_path, pack_name");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	const FString PackLower = PackName.TrimStartAndEnd().ToLower();
	FString Prefix;
	Input->TryGetStringField(TEXT("prefix"), Prefix);
	if (Prefix.IsEmpty())
	{
		Prefix = PackLower.Left(1).ToUpper() + PackLower.Mid(1);
	}

	struct FScalarDef
	{
		FString Suffix;
		float DefaultValue = 0.0f;
	};
	struct FVectorDef
	{
		FString Suffix;
		FLinearColor DefaultValue = FLinearColor::White;
	};
	struct FTextureDef
	{
		FString Suffix;
	};

	TArray<FScalarDef> ScalarDefs;
	TArray<FVectorDef> VectorDefs;
	TArray<FTextureDef> TextureDefs;
	if (PackLower == TEXT("fire"))
	{
		ScalarDefs = { {TEXT("Intensity"), 1.0f}, {TEXT("EdgeWidth"), 0.08f}, {TEXT("Threshold"), 0.45f}, {TEXT("Distortion"), 0.15f}, {TEXT("PannerSpeed"), 0.5f} };
		VectorDefs = { {TEXT("Color"), FLinearColor(1.0f, 0.35f, 0.05f, 1.0f)}, {TEXT("EdgeColor"), FLinearColor(1.0f, 0.8f, 0.2f, 1.0f)} };
		TextureDefs = { {TEXT("NoiseTex")}, {TEXT("MaskTex")} };
	}
	else if (PackLower == TEXT("dissolve"))
	{
		ScalarDefs = { {TEXT("Threshold"), 0.5f}, {TEXT("EdgeWidth"), 0.08f}, {TEXT("Softness"), 0.15f}, {TEXT("GlowIntensity"), 1.0f} };
		VectorDefs = { {TEXT("EdgeColor"), FLinearColor(1.0f, 0.45f, 0.1f, 1.0f)}, {TEXT("BurnColor"), FLinearColor(0.2f, 0.2f, 0.2f, 1.0f)} };
		TextureDefs = { {TEXT("NoiseTex")}, {TEXT("SecondaryMaskTex")} };
	}
	else if (PackLower == TEXT("magic"))
	{
		ScalarDefs = { {TEXT("Intensity"), 1.0f}, {TEXT("PulseSpeed"), 1.0f}, {TEXT("Distortion"), 0.2f}, {TEXT("FresnelPower"), 4.0f} };
		VectorDefs = { {TEXT("PrimaryColor"), FLinearColor(0.2f, 0.65f, 1.0f, 1.0f)}, {TEXT("SecondaryColor"), FLinearColor(0.7f, 0.2f, 1.0f, 1.0f)} };
		TextureDefs = { {TEXT("MaskTex")}, {TEXT("FlowTex")} };
	}
	else if (PackLower == TEXT("transform"))
	{
		ScalarDefs = { {TEXT("Threshold"), 0.5f}, {TEXT("EdgeWidth"), 0.1f}, {TEXT("BlendAmount"), 0.0f}, {TEXT("CrackIntensity"), 1.0f} };
		VectorDefs = { {TEXT("EdgeColor"), FLinearColor(1.0f, 0.4f, 0.0f, 1.0f)}, {TEXT("CrackColor"), FLinearColor(1.0f, 0.1f, 0.02f, 1.0f)} };
		TextureDefs = { {TEXT("NoiseTex")}, {TEXT("CrackMaskTex")} };
	}
	else if (PackLower == TEXT("decal"))
	{
		ScalarDefs = { {TEXT("Opacity"), 1.0f}, {TEXT("EdgeSoftness"), 0.2f}, {TEXT("TintIntensity"), 1.0f} };
		VectorDefs = { {TEXT("TintColor"), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)} };
		TextureDefs = { {TEXT("AlbedoTex")}, {TEXT("MaskTex")} };
	}
	else if (PackLower == TEXT("impact"))
	{
		ScalarDefs = { {TEXT("Intensity"), 1.0f}, {TEXT("Radius"), 32.0f}, {TEXT("EdgeWidth"), 0.08f}, {TEXT("Fade"), 1.0f} };
		VectorDefs = { {TEXT("ImpactColor"), FLinearColor(1.0f, 0.5f, 0.1f, 1.0f)} };
		TextureDefs = { {TEXT("BreakupTex")}, {TEXT("NormalTex")} };
	}
	else
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_UNKNOWN_PARAMETER_PACK"), FString::Printf(TEXT("Unsupported pack_name '%s'."), *PackName));
	}

	FString TexturePath;
	Input->TryGetStringField(TEXT("texture_path"), TexturePath);
	UTexture* DefaultTexture = TexturePath.IsEmpty() ? nullptr : Cast<UTexture>(UEditorAssetLibrary::LoadAsset(TexturePath));
	if (!TexturePath.IsEmpty() && !DefaultTexture)
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_TEXTURE_NOT_FOUND"), FString::Printf(TEXT("Could not load Texture at path: %s"), *TexturePath));
	}

	int32 StartX = 0;
	int32 StartY = 0;
	if (!TryReadOptionalIntField(Input, TEXT("editor_x"), StartX) ||
		!TryReadOptionalIntField(Input, TEXT("editor_y"), StartY))
	{
		ComputeMaterialAutoPlacement(Material, TEXT("ParameterPack"), StartX, StartY);
	}

	const bool bCreateComment = ParseOptionalBoolField(Input, TEXT("create_comment"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const int32 ColumnSpacing = 260;
	const int32 RowSpacing = 180;

	TSet<FString> ExistingParametersLower;
	for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
	{
		if (const UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Expr))
		{
			ExistingParametersLower.Add(ScalarParam->ParameterName.ToString().ToLower());
		}
		else if (const UMaterialExpressionVectorParameter* VectorParam = Cast<UMaterialExpressionVectorParameter>(Expr))
		{
			ExistingParametersLower.Add(VectorParam->ParameterName.ToString().ToLower());
		}
		else if (const UMaterialExpressionTextureSampleParameter2D* TextureParam = Cast<UMaterialExpressionTextureSampleParameter2D>(Expr))
		{
			ExistingParametersLower.Add(TextureParam->ParameterName.ToString().ToLower());
		}
	}

	TArray<TSharedPtr<FJsonValue>> PlannedParams;
	TArray<TSharedPtr<FJsonValue>> CreatedParams;
	TArray<TSharedPtr<FJsonValue>> SkippedParams;
	TArray<int32> CreatedExpressionIndices;

	auto MakeFullName = [&Prefix](const FString& Suffix) -> FString
	{
		return Prefix.IsEmpty() ? Suffix : FString::Printf(TEXT("%s_%s"), *Prefix, *Suffix);
	};
	auto AddSkipped = [&SkippedParams](const FString& ParameterName, const FString& Reason)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("parameter_name"), ParameterName);
		Obj->SetStringField(TEXT("reason"), Reason);
		SkippedParams.Add(MakeShareable(new FJsonValueObject(Obj)));
	};
	auto AddPlanned = [&PlannedParams](const FString& ParameterType, const FString& ParameterName, const int32 NodeX, const int32 NodeY, const FString& DefaultValue)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("parameter_type"), ParameterType);
		Obj->SetStringField(TEXT("parameter_name"), ParameterName);
		Obj->SetStringField(TEXT("default_value"), DefaultValue);
		Obj->SetNumberField(TEXT("editor_x"), NodeX);
		Obj->SetNumberField(TEXT("editor_y"), NodeY);
		PlannedParams.Add(MakeShareable(new FJsonValueObject(Obj)));
	};

	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("create_material_parameter_pack"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	int32 ScalarRow = 0;
	for (const FScalarDef& Def : ScalarDefs)
	{
		const FString ParameterName = MakeFullName(Def.Suffix);
		const int32 NodeX = StartX;
		const int32 NodeY = StartY + ScalarRow * RowSpacing;
		++ScalarRow;
		AddPlanned(TEXT("ScalarParameter"), ParameterName, NodeX, NodeY, FString::SanitizeFloat(Def.DefaultValue));
		if (ExistingParametersLower.Contains(ParameterName.ToLower()))
		{
			AddSkipped(ParameterName, TEXT("A parameter with this name already exists in the material."));
			continue;
		}
		if (!bApplyChanges)
		{
			continue;
		}

		auto* Expr = NewObject<UMaterialExpressionScalarParameter>(Material);
		Expr->ParameterName = FName(*ParameterName);
		Expr->DefaultValue = Def.DefaultValue;
		Expr->MaterialExpressionEditorX = NodeX;
		Expr->MaterialExpressionEditorY = NodeY;
		Material->GetExpressionCollection().AddExpression(Expr);
		CreatedExpressionIndices.Add(Material->GetExpressionCollection().Expressions.Num() - 1);

		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("parameter_type"), TEXT("ScalarParameter"));
		Obj->SetStringField(TEXT("parameter_name"), ParameterName);
		Obj->SetNumberField(TEXT("default_value"), Def.DefaultValue);
		CreatedParams.Add(MakeShareable(new FJsonValueObject(Obj)));
		ExistingParametersLower.Add(ParameterName.ToLower());
	}

	int32 VectorRow = 0;
	for (const FVectorDef& Def : VectorDefs)
	{
		const FString ParameterName = MakeFullName(Def.Suffix);
		const int32 NodeX = StartX + ColumnSpacing;
		const int32 NodeY = StartY + VectorRow * RowSpacing;
		++VectorRow;
		AddPlanned(TEXT("VectorParameter"), ParameterName, NodeX, NodeY, Def.DefaultValue.ToString());
		if (ExistingParametersLower.Contains(ParameterName.ToLower()))
		{
			AddSkipped(ParameterName, TEXT("A parameter with this name already exists in the material."));
			continue;
		}
		if (!bApplyChanges)
		{
			continue;
		}

		auto* Expr = NewObject<UMaterialExpressionVectorParameter>(Material);
		Expr->ParameterName = FName(*ParameterName);
		Expr->DefaultValue = Def.DefaultValue;
		Expr->MaterialExpressionEditorX = NodeX;
		Expr->MaterialExpressionEditorY = NodeY;
		Material->GetExpressionCollection().AddExpression(Expr);
		CreatedExpressionIndices.Add(Material->GetExpressionCollection().Expressions.Num() - 1);

		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("parameter_type"), TEXT("VectorParameter"));
		Obj->SetStringField(TEXT("parameter_name"), ParameterName);
		Obj->SetStringField(TEXT("default_value"), Def.DefaultValue.ToString());
		CreatedParams.Add(MakeShareable(new FJsonValueObject(Obj)));
		ExistingParametersLower.Add(ParameterName.ToLower());
	}

	int32 TextureRow = 0;
	for (const FTextureDef& Def : TextureDefs)
	{
		const FString ParameterName = MakeFullName(Def.Suffix);
		const int32 NodeX = StartX + ColumnSpacing * 2;
		const int32 NodeY = StartY + TextureRow * RowSpacing;
		++TextureRow;
		AddPlanned(TEXT("TextureSampleParameter2D"), ParameterName, NodeX, NodeY, DefaultTexture ? DefaultTexture->GetPathName() : TEXT(""));
		if (ExistingParametersLower.Contains(ParameterName.ToLower()))
		{
			AddSkipped(ParameterName, TEXT("A parameter with this name already exists in the material."));
			continue;
		}
		if (!bApplyChanges)
		{
			continue;
		}

		auto* Expr = NewObject<UMaterialExpressionTextureSampleParameter2D>(Material);
		Expr->ParameterName = FName(*ParameterName);
		Expr->Texture = DefaultTexture;
		Expr->MaterialExpressionEditorX = NodeX;
		Expr->MaterialExpressionEditorY = NodeY;
		Material->GetExpressionCollection().AddExpression(Expr);
		CreatedExpressionIndices.Add(Material->GetExpressionCollection().Expressions.Num() - 1);

		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("parameter_type"), TEXT("TextureSampleParameter2D"));
		Obj->SetStringField(TEXT("parameter_name"), ParameterName);
		Obj->SetStringField(TEXT("default_value"), DefaultTexture ? DefaultTexture->GetPathName() : TEXT(""));
		CreatedParams.Add(MakeShareable(new FJsonValueObject(Obj)));
		ExistingParametersLower.Add(ParameterName.ToLower());
	}

	bool bCommentCreated = false;
	if (bApplyChanges && bCreateComment)
	{
		auto* CommentExpr = NewObject<UMaterialExpressionComment>(Material);
		CommentExpr->MaterialExpressionEditorX = StartX - 120;
		CommentExpr->MaterialExpressionEditorY = StartY - 120;
		CommentExpr->SizeX = ColumnSpacing * 3 + 200;
		CommentExpr->SizeY = FMath::Max3(ScalarDefs.Num(), VectorDefs.Num(), TextureDefs.Num()) * RowSpacing + 220;
		CommentExpr->Text = FString::Printf(TEXT("%s Parameter Pack"), *Prefix);
		CommentExpr->CommentColor = FLinearColor(1.0f, 0.92f, 0.15f, 1.0f);
		CommentExpr->bGroupMode = true;
		Material->GetExpressionCollection().AddComment(CommentExpr);
		bCommentCreated = true;
	}

	if (bApplyChanges)
	{
		Material->PreEditChange(nullptr);
		Material->PostEditChange();
		Material->MarkPackageDirty();
		FString SaveError;
		if (!SaveMaterialMutationAsset(Material, SaveError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("create_material_parameter_pack"));
	Result->SetStringField(TEXT("pack_name"), PackLower);
	Result->SetStringField(TEXT("prefix"), Prefix);
	Result->SetArrayField(TEXT("planned_parameters"), PlannedParams);
	Result->SetArrayField(TEXT("created_parameters"), CreatedParams);
	Result->SetArrayField(TEXT("skipped_existing"), SkippedParams);
	Result->SetArrayField(TEXT("created_expression_indices"), MakeMaterialIntJsonArray(CreatedExpressionIndices));
	Result->SetBoolField(TEXT("comment_created"), bCommentCreated);
	Result->SetStringField(TEXT("message"),
		FString::Printf(TEXT("%s parameter pack '%s' with %d new parameter node(s)."),
			bApplyChanges ? TEXT("Created") : TEXT("Planned"),
			*PackLower,
			CreatedExpressionIndices.Num()));

	if (bApplyChanges)
	{
		TSharedPtr<FJsonObject> Details = MakeShareable(new FJsonObject());
		Details->SetStringField(TEXT("pack_name"), PackLower);
		Details->SetStringField(TEXT("prefix"), Prefix);
		Details->SetNumberField(TEXT("created_parameter_count"), CreatedExpressionIndices.Num());
		Details->SetBoolField(TEXT("comment_created"), bCommentCreated);
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("create_material_parameter_pack"), Material, SnapshotPath, true, Details));
		AppendMaterialMutationAudit(Result, TEXT("create_material_parameter_pack"), Material, Result->GetStringField(TEXT("message")), Details, SnapshotPath);
	}

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetMaterialCustomNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	FString Code;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) ||
		!Input->TryGetStringField(TEXT("code"), Code))
	{
		OutResult = TEXT("Missing required fields: material_path, code");
		return false;
	}
	if (!IsMaterialCustomHlslExplicitlyAllowed(Input))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_CUSTOM_HLSL_NOT_ALLOWED"),
			TEXT("set_material_custom_node authors a Material Custom/HLSL node, which is less production-friendly than native graph authoring. Re-run with allow_custom_hlsl=true only when that tradeoff is explicitly acceptable."));
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	int32 ExpressionIndex = INDEX_NONE;
	TryReadOptionalIntField(Input, TEXT("expression_index"), ExpressionIndex);

	FString Description;
	FString OutputTypeString;
	Input->TryGetStringField(TEXT("description"), Description);
	Input->TryGetStringField(TEXT("output_type"), OutputTypeString);

	TArray<FString> InputNames;
	const TArray<TSharedPtr<FJsonValue>>* InputNamesArray = nullptr;
	if (Input->TryGetArrayField(TEXT("input_names"), InputNamesArray) && InputNamesArray)
	{
		for (const TSharedPtr<FJsonValue>& Value : *InputNamesArray)
		{
			const FString InputName = Value.IsValid() ? Value->AsString() : TEXT("");
			if (!InputName.IsEmpty())
			{
				InputNames.Add(InputName);
			}
		}
	}

	int32 EditorX = 0;
	int32 EditorY = 0;
	if (!TryReadOptionalIntField(Input, TEXT("editor_x"), EditorX) ||
		!TryReadOptionalIntField(Input, TEXT("editor_y"), EditorY))
	{
		ComputeMaterialAutoPlacement(Material, TEXT("Custom"), EditorX, EditorY);
	}

	ECustomMaterialOutputType OutputType = CMOT_Float1;
	if (!ParseMaterialCustomOutputType(OutputTypeString, OutputType))
	{
		OutResult = FString::Printf(TEXT("Unsupported output_type '%s'"), *OutputTypeString);
		return false;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("set_material_custom_node"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	auto& Expressions = Material->GetExpressionCollection().Expressions;
	const bool bUpdateExisting = ExpressionIndex != INDEX_NONE;
	UMaterialExpressionCustom* CustomExpr = nullptr;
	if (bUpdateExisting)
	{
		if (!Expressions.IsValidIndex(ExpressionIndex))
		{
			OutResult = FString::Printf(TEXT("Invalid expression_index %d"), ExpressionIndex);
			return false;
		}

		CustomExpr = Cast<UMaterialExpressionCustom>(Expressions[ExpressionIndex]);
		if (!CustomExpr)
		{
			OutResult = FString::Printf(TEXT("Expression %d is not a Custom node"), ExpressionIndex);
			return false;
		}
	}
	else
	{
		CustomExpr = NewObject<UMaterialExpressionCustom>(bApplyChanges ? static_cast<UObject*>(Material) : GetTransientPackage());
		CustomExpr->MaterialExpressionEditorX = EditorX;
		CustomExpr->MaterialExpressionEditorY = EditorY;
	}

	CustomExpr->Code = Code;
	CustomExpr->OutputType = OutputType;
	if (!Description.IsEmpty())
	{
		CustomExpr->Description = Description;
	}
	if (InputNames.Num() > 0)
	{
		CustomExpr->Inputs.Reset();
		for (const FString& InputName : InputNames)
		{
			FCustomInput CustomInput;
			CustomInput.InputName = FName(*InputName);
			CustomExpr->Inputs.Add(CustomInput);
		}
	}

	if (!bApplyChanges)
	{
		TArray<FString> CodeLines;
		const int32 CodeLineCount = Code.ParseIntoArrayLines(CodeLines, false);
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("set_material_custom_node"));
		Result->SetStringField(TEXT("mode"), bUpdateExisting ? TEXT("update") : TEXT("create"));
		Result->SetStringField(TEXT("output_type"), GetMaterialCustomOutputTypeLabel(OutputType));
		Result->SetStringField(TEXT("description"), CustomExpr->Description);
		Result->SetNumberField(TEXT("predicted_expression_index"), bUpdateExisting ? ExpressionIndex : Expressions.Num());
		Result->SetNumberField(TEXT("code_length"), Code.Len());
		Result->SetNumberField(TEXT("code_line_count"), CodeLineCount);
		Result->SetArrayField(TEXT("input_names"), MakeMaterialStringJsonArray(InputNames));
		Result->SetStringField(TEXT("message"), bUpdateExisting
			? TEXT("Dry-run: would update existing Custom node.")
			: TEXT("Dry-run: would create new Custom node."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (!bUpdateExisting)
	{
		Material->GetExpressionCollection().AddExpression(CustomExpr);
		ExpressionIndex = Material->GetExpressionCollection().Expressions.Num() - 1;
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TArray<FString> CodeLines;
	const int32 CodeLineCount = Code.ParseIntoArrayLines(CodeLines, false);
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("set_material_custom_node"));
	Result->SetStringField(TEXT("mode"), bUpdateExisting ? TEXT("update") : TEXT("create"));
	Result->SetNumberField(TEXT("expression_index"), ExpressionIndex);
	Result->SetStringField(TEXT("description"), CustomExpr->Description);
	Result->SetStringField(TEXT("output_type"), GetMaterialCustomOutputTypeLabel(OutputType));
	Result->SetNumberField(TEXT("code_length"), Code.Len());
	Result->SetNumberField(TEXT("code_line_count"), CodeLineCount);
	Result->SetArrayField(TEXT("input_names"), MakeMaterialStringJsonArray(InputNames));
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_custom_node"), Material, SnapshotPath, true));
	Result->SetStringField(TEXT("message"), bUpdateExisting
		? FString::Printf(TEXT("Updated Custom node at expression index %d."), ExpressionIndex)
		: FString::Printf(TEXT("Created Custom node at expression index %d."), ExpressionIndex));
	AppendMaterialMutationAudit(Result, TEXT("set_material_custom_node"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateMaterialComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	FString CommentText;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath) ||
		!Input->TryGetStringField(TEXT("comment_text"), CommentText))
	{
		OutResult = TEXT("Missing required fields: material_path, comment_text");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	int32 EditorX = 0;
	int32 EditorY = 0;
	int32 Width = 800;
	int32 Height = 420;
	if (!TryReadOptionalIntField(Input, TEXT("editor_x"), EditorX) ||
		!TryReadOptionalIntField(Input, TEXT("editor_y"), EditorY))
	{
		ComputeMaterialAutoPlacement(Material, TEXT("Comment"), EditorX, EditorY);
	}
	TryReadOptionalIntField(Input, TEXT("width"), Width);
	TryReadOptionalIntField(Input, TEXT("height"), Height);

	FLinearColor CommentColor(1.0f, 0.92f, 0.15f, 1.0f);
	const TArray<TSharedPtr<FJsonValue>>* ColorArray = nullptr;
	if (Input->TryGetArrayField(TEXT("color_rgba"), ColorArray) && ColorArray != nullptr && ColorArray->Num() >= 3)
	{
		CommentColor.R = static_cast<float>((*ColorArray)[0]->AsNumber());
		CommentColor.G = static_cast<float>((*ColorArray)[1]->AsNumber());
		CommentColor.B = static_cast<float>((*ColorArray)[2]->AsNumber());
		CommentColor.A = ColorArray->Num() >= 4 ? static_cast<float>((*ColorArray)[3]->AsNumber()) : 1.0f;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("create_material_comment"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}
	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRun = MakeShareable(new FJsonObject());
		DryRun->SetBoolField(TEXT("success"), true);
		DryRun->SetBoolField(TEXT("dry_run"), true);
		DryRun->SetStringField(TEXT("action"), TEXT("create_material_comment"));
		DryRun->SetStringField(TEXT("comment_text"), CommentText);
		DryRun->SetNumberField(TEXT("editor_x"), EditorX);
		DryRun->SetNumberField(TEXT("editor_y"), EditorY);
		DryRun->SetNumberField(TEXT("width"), Width);
		DryRun->SetNumberField(TEXT("height"), Height);
		DryRun->SetStringField(TEXT("message"), FString::Printf(TEXT("Dry-run: would create material comment '%s'"), *CommentText));
		OutResult = JsonObjToString(DryRun);
		return true;
	}

	UMaterialExpressionComment* CommentExpr = NewObject<UMaterialExpressionComment>(Material);
	CommentExpr->MaterialExpressionEditorX = EditorX;
	CommentExpr->MaterialExpressionEditorY = EditorY;
	CommentExpr->SizeX = Width;
	CommentExpr->SizeY = Height;
	CommentExpr->Text = CommentText;
	CommentExpr->CommentColor = CommentColor;
	CommentExpr->bGroupMode = true;
	Material->GetExpressionCollection().AddComment(CommentExpr);

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("create_material_comment"));
	Result->SetStringField(TEXT("comment_text"), CommentText);
	Result->SetNumberField(TEXT("editor_x"), EditorX);
	Result->SetNumberField(TEXT("editor_y"), EditorY);
	Result->SetNumberField(TEXT("width"), Width);
	Result->SetNumberField(TEXT("height"), Height);
	Result->SetStringField(TEXT("message"), FString::Printf(TEXT("Created material comment '%s'"), *CommentText));
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("create_material_comment"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(Result, TEXT("create_material_comment"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_UpdateMaterialComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	TOptional<int32> RequestedCommentIndex;
	int32 ParsedCommentIndex = INDEX_NONE;
	if (TryReadOptionalIntField(Input, TEXT("comment_index"), ParsedCommentIndex))
	{
		RequestedCommentIndex = ParsedCommentIndex;
	}

	FString CommentText;
	FString ContainsText;
	Input->TryGetStringField(TEXT("comment_text"), CommentText);
	Input->TryGetStringField(TEXT("contains_text"), ContainsText);

	const int32 CommentIndex = FindMaterialCommentIndex(Material, RequestedCommentIndex, CommentText, ContainsText);
	TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
	if (!Comments.IsValidIndex(CommentIndex) || !Comments[CommentIndex])
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_COMMENT_NOT_FOUND"),
			TEXT("Could not resolve the target Material comment. Provide comment_index, comment_text, or contains_text."));
	}

	UMaterialExpressionComment* CommentExpr = Comments[CommentIndex];

	int32 EditorX = CommentExpr->MaterialExpressionEditorX;
	int32 EditorY = CommentExpr->MaterialExpressionEditorY;
	int32 Width = CommentExpr->SizeX;
	int32 Height = CommentExpr->SizeY;
	TryReadOptionalIntField(Input, TEXT("editor_x"), EditorX);
	TryReadOptionalIntField(Input, TEXT("editor_y"), EditorY);
	TryReadOptionalIntField(Input, TEXT("width"), Width);
	TryReadOptionalIntField(Input, TEXT("height"), Height);

	TArray<int32> WrapExpressionIndices;
	const TArray<TSharedPtr<FJsonValue>>* WrapValues = nullptr;
	if (Input->TryGetArrayField(TEXT("wrap_expression_indices"), WrapValues) && WrapValues != nullptr)
	{
		for (const TSharedPtr<FJsonValue>& Value : *WrapValues)
		{
			if (Value.IsValid())
			{
				WrapExpressionIndices.Add(static_cast<int32>(Value->AsNumber()));
			}
		}
	}

	int32 Padding = 80;
	TryReadOptionalIntField(Input, TEXT("padding"), Padding);
	if (WrapExpressionIndices.Num() > 0)
	{
		int32 MinX = 0;
		int32 MinY = 0;
		int32 MaxX = 0;
		int32 MaxY = 0;
		if (!ComputeMaterialExpressionBounds(Material, WrapExpressionIndices, MinX, MinY, MaxX, MaxY))
		{
			return BuildMaterialToolError(
				OutResult,
				TEXT("MAT_COMMENT_WRAP_FAILED"),
				TEXT("Could not compute bounds from wrap_expression_indices."));
		}

		EditorX = MinX - Padding;
		EditorY = MinY - Padding;
		Width = (MaxX - MinX) + Padding * 2;
		Height = (MaxY - MinY) + Padding * 2;
	}

	FString NewText;
	Input->TryGetStringField(TEXT("new_text"), NewText);

	FLinearColor CommentColor = CommentExpr->CommentColor;
	const TArray<TSharedPtr<FJsonValue>>* ColorArray = nullptr;
	if (Input->TryGetArrayField(TEXT("color_rgba"), ColorArray) && ColorArray != nullptr && ColorArray->Num() >= 3)
	{
		CommentColor.R = static_cast<float>((*ColorArray)[0]->AsNumber());
		CommentColor.G = static_cast<float>((*ColorArray)[1]->AsNumber());
		CommentColor.B = static_cast<float>((*ColorArray)[2]->AsNumber());
		CommentColor.A = ColorArray->Num() >= 4 ? static_cast<float>((*ColorArray)[3]->AsNumber()) : 1.0f;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("update_material_comment"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	const FString EffectiveText = NewText.IsEmpty() ? CommentExpr->Text : NewText;
	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("update_material_comment"));
	Result->SetNumberField(TEXT("comment_index"), CommentIndex);
	Result->SetStringField(TEXT("original_text"), CommentExpr->Text);
	Result->SetStringField(TEXT("comment_text"), EffectiveText);
	Result->SetNumberField(TEXT("editor_x"), EditorX);
	Result->SetNumberField(TEXT("editor_y"), EditorY);
	Result->SetNumberField(TEXT("width"), Width);
	Result->SetNumberField(TEXT("height"), Height);
	Result->SetBoolField(TEXT("wrapped_expression_block"), WrapExpressionIndices.Num() > 0);
	Result->SetArrayField(TEXT("wrap_expression_indices"), MakeMaterialIntJsonArray(WrapExpressionIndices));

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would update the selected Material comment."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	CommentExpr->MaterialExpressionEditorX = EditorX;
	CommentExpr->MaterialExpressionEditorY = EditorY;
	CommentExpr->SizeX = Width;
	CommentExpr->SizeY = Height;
	CommentExpr->CommentColor = CommentColor;
	if (!NewText.IsEmpty())
	{
		CommentExpr->Text = NewText;
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Details = MakeShareable(new FJsonObject());
	Details->SetNumberField(TEXT("comment_index"), CommentIndex);
	Details->SetStringField(TEXT("comment_text"), EffectiveText);
	Details->SetBoolField(TEXT("wrapped_expression_block"), WrapExpressionIndices.Num() > 0);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("update_material_comment"), Material, SnapshotPath, true, Details));
	Result->SetStringField(TEXT("message"), TEXT("Updated Material comment."));
	AppendMaterialMutationAudit(Result, TEXT("update_material_comment"), Material, Result->GetStringField(TEXT("message")), Details, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_DeleteMaterialComments(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	const bool bDeleteAll = ParseOptionalBoolField(Input, TEXT("delete_all"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	TOptional<int32> RequestedCommentIndex;
	int32 ParsedCommentIndex = INDEX_NONE;
	if (TryReadOptionalIntField(Input, TEXT("comment_index"), ParsedCommentIndex))
	{
		RequestedCommentIndex = ParsedCommentIndex;
	}

	FString CommentText;
	FString ContainsText;
	Input->TryGetStringField(TEXT("comment_text"), CommentText);
	Input->TryGetStringField(TEXT("contains_text"), ContainsText);

	const bool bHasCriteria = bDeleteAll || RequestedCommentIndex.IsSet() || !CommentText.IsEmpty() || !ContainsText.IsEmpty();
	if (!bHasCriteria)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_COMMENT_DELETE_NEEDS_CRITERIA"),
			TEXT("Provide delete_all=true, comment_index, comment_text, or contains_text."));
	}

	const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
	TSet<int32> MatchedIndices;
	if (bDeleteAll)
	{
		for (int32 Index = 0; Index < Comments.Num(); ++Index)
		{
			if (Comments[Index])
			{
				MatchedIndices.Add(Index);
			}
		}
	}
	if (RequestedCommentIndex.IsSet() && Comments.IsValidIndex(RequestedCommentIndex.GetValue()) && Comments[RequestedCommentIndex.GetValue()])
	{
		MatchedIndices.Add(RequestedCommentIndex.GetValue());
	}
	for (int32 Index = 0; Index < Comments.Num(); ++Index)
	{
		UMaterialExpressionComment* Comment = Comments[Index];
		if (!Comment)
		{
			continue;
		}

		if (!CommentText.IsEmpty() && Comment->Text.Equals(CommentText, ESearchCase::CaseSensitive))
		{
			MatchedIndices.Add(Index);
		}
		if (!ContainsText.IsEmpty() && Comment->Text.Contains(ContainsText, ESearchCase::IgnoreCase))
		{
			MatchedIndices.Add(Index);
		}
	}

	TArray<int32> SortedMatchedIndices = MatchedIndices.Array();
	SortedMatchedIndices.Sort();
	if (SortedMatchedIndices.Num() == 0)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_COMMENT_NOT_FOUND"),
			TEXT("No Material comments matched the provided delete criteria."));
	}

	TArray<TSharedPtr<FJsonValue>> MatchedCommentArray;
	for (const int32 Index : SortedMatchedIndices)
	{
		if (!Comments.IsValidIndex(Index) || !Comments[Index])
		{
			continue;
		}

		UMaterialExpressionComment* Comment = Comments[Index];
		TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
		CommentObj->SetNumberField(TEXT("comment_index"), Index);
		CommentObj->SetStringField(TEXT("comment_text"), Comment->Text);
		CommentObj->SetNumberField(TEXT("editor_x"), Comment->MaterialExpressionEditorX);
		CommentObj->SetNumberField(TEXT("editor_y"), Comment->MaterialExpressionEditorY);
		CommentObj->SetNumberField(TEXT("width"), Comment->SizeX);
		CommentObj->SetNumberField(TEXT("height"), Comment->SizeY);
		MatchedCommentArray.Add(MakeShareable(new FJsonValueObject(CommentObj)));
	}

	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("delete_material_comments"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("delete_material_comments"));
	Result->SetStringField(TEXT("material_path"), MaterialPath);
	Result->SetNumberField(TEXT("matched_comment_count"), SortedMatchedIndices.Num());
	Result->SetArrayField(TEXT("matched_comment_indices"), MakeMaterialIntJsonArray(SortedMatchedIndices));
	Result->SetArrayField(TEXT("matched_comments"), MatchedCommentArray);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would delete matched Material comment boxes."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	for (int32 ReverseIndex = SortedMatchedIndices.Num() - 1; ReverseIndex >= 0; --ReverseIndex)
	{
		const int32 CommentIndex = SortedMatchedIndices[ReverseIndex];
		if (!Comments.IsValidIndex(CommentIndex) || !Comments[CommentIndex])
		{
			continue;
		}

		UMaterialExpressionComment* Comment = Comments[CommentIndex];
		Comment->Modify();
		Material->GetExpressionCollection().RemoveComment(Comment);
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Details = MakeShareable(new FJsonObject());
	Details->SetNumberField(TEXT("deleted_comment_count"), SortedMatchedIndices.Num());
	Details->SetArrayField(TEXT("deleted_comment_indices"), MakeMaterialIntJsonArray(SortedMatchedIndices));
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("delete_material_comments"), Material, SnapshotPath, true, Details));
	Result->SetStringField(TEXT("message"), TEXT("Deleted matched Material comment boxes."));
	AppendMaterialMutationAudit(Result, TEXT("delete_material_comments"), Material, Result->GetStringField(TEXT("message")), Details, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_DeleteMaterialExpressions(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	TSet<int32> RequestedIndices;
	int32 SingleIndex = INDEX_NONE;
	if (TryReadOptionalIntField(Input, TEXT("expression_index"), SingleIndex))
	{
		RequestedIndices.Add(SingleIndex);
	}

	const TArray<TSharedPtr<FJsonValue>>* IndexArray = nullptr;
	if (Input->TryGetArrayField(TEXT("expression_indices"), IndexArray) && IndexArray)
	{
		for (const TSharedPtr<FJsonValue>& IndexValue : *IndexArray)
		{
			if (IndexValue.IsValid())
			{
				RequestedIndices.Add(static_cast<int32>(IndexValue->AsNumber()));
			}
		}
	}

	if (RequestedIndices.Num() == 0)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_EXPRESSION_DELETE_NEEDS_INDEX"),
			TEXT("Provide expression_index or expression_indices."));
	}

	const TArray<TObjectPtr<UMaterialExpression>>& Expressions = Material->GetExpressionCollection().Expressions;
	TArray<int32> SortedIndices = RequestedIndices.Array();
	SortedIndices.Sort();

	TArray<UMaterialExpression*> ExpressionsToDelete;
	TArray<TSharedPtr<FJsonValue>> MatchedExpressionArray;
	for (const int32 ExpressionIndex : SortedIndices)
	{
		if (!Expressions.IsValidIndex(ExpressionIndex) || !Expressions[ExpressionIndex])
		{
			return BuildMaterialToolError(
				OutResult,
				TEXT("MAT_EXPRESSION_INDEX_INVALID"),
				FString::Printf(TEXT("Invalid expression_index %d for material with %d expressions."), ExpressionIndex, Expressions.Num()));
		}

		UMaterialExpression* Expression = Expressions[ExpressionIndex];
		if (!Expression->CanUserDeleteExpression())
		{
			return BuildMaterialToolError(
				OutResult,
				TEXT("MAT_EXPRESSION_DELETE_BLOCKED"),
				FString::Printf(TEXT("Expression index %d (%s) cannot be deleted by the user."), ExpressionIndex, *Expression->GetClass()->GetName()));
		}

		TSharedPtr<FJsonObject> ExprObj = MakeShareable(new FJsonObject());
		ExprObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
		ExprObj->SetStringField(TEXT("expression_class"), Expression->GetClass()->GetName());
		ExprObj->SetStringField(TEXT("caption"), GetMaterialExpressionPracticalName(Expression));
		MatchedExpressionArray.Add(MakeShareable(new FJsonValueObject(ExprObj)));
		ExpressionsToDelete.Add(Expression);
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("delete_material_expressions"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("delete_material_expressions"));
	Result->SetStringField(TEXT("material_path"), MaterialPath);
	Result->SetNumberField(TEXT("matched_expression_count"), ExpressionsToDelete.Num());
	Result->SetArrayField(TEXT("matched_expression_indices"), MakeMaterialIntJsonArray(SortedIndices));
	Result->SetArrayField(TEXT("matched_expressions"), MatchedExpressionArray);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would delete matched Material expression nodes."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	Material->Modify();
	for (UMaterialExpression* Expression : ExpressionsToDelete)
	{
		if (Expression)
		{
			Expression->Modify();
			UMaterialEditingLibrary::DeleteMaterialExpression(Material, Expression);
		}
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Details = MakeShareable(new FJsonObject());
	Details->SetNumberField(TEXT("deleted_expression_count"), ExpressionsToDelete.Num());
	Details->SetArrayField(TEXT("deleted_expression_indices"), MakeMaterialIntJsonArray(SortedIndices));
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("delete_material_expressions"), Material, SnapshotPath, true, Details));
	Result->SetStringField(TEXT("message"), TEXT("Deleted matched Material expression nodes."));
	AppendMaterialMutationAudit(Result, TEXT("delete_material_expressions"), Material, Result->GetStringField(TEXT("message")), Details, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_LayoutMaterialExpressions(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	FString LayoutMode = TEXT("horizontal");
	Input->TryGetStringField(TEXT("layout_mode"), LayoutMode);
	const bool bHorizontal = !LayoutMode.Equals(TEXT("vertical"), ESearchCase::IgnoreCase);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("layout_material_expressions"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	int32 Spacing = 220;
	TryReadOptionalIntField(Input, TEXT("spacing"), Spacing);

	TArray<int32> ExpressionIndices;
	const TArray<TSharedPtr<FJsonValue>>* ExpressionIndexValues = nullptr;
	if (Input->TryGetArrayField(TEXT("expression_indices"), ExpressionIndexValues) && ExpressionIndexValues != nullptr)
	{
		for (const TSharedPtr<FJsonValue>& Value : *ExpressionIndexValues)
		{
			if (!Value.IsValid())
			{
				continue;
			}
			ExpressionIndices.Add(static_cast<int32>(Value->AsNumber()));
		}
	}
	else
	{
		for (int32 Index = 0; Index < Material->GetExpressionCollection().Expressions.Num(); ++Index)
		{
			ExpressionIndices.Add(Index);
		}
	}

	ExpressionIndices.Sort();
	if (ExpressionIndices.Num() == 0)
	{
		OutResult = TEXT("{\"success\":false,\"error\":\"No material expressions were selected for layout.\"}");
		return false;
	}

	int32 StartX = 0;
	int32 StartY = 0;
	bool bHasStartX = TryReadOptionalIntField(Input, TEXT("start_x"), StartX);
	bool bHasStartY = TryReadOptionalIntField(Input, TEXT("start_y"), StartY);

	bool bInitializedFromGraph = false;
	for (const int32 ExpressionIndex : ExpressionIndices)
	{
		if (!Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex))
		{
			continue;
		}

		UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex];
		if (!Expr)
		{
			continue;
		}

		if (!bHasStartX)
		{
			StartX = bInitializedFromGraph ? FMath::Min(StartX, Expr->MaterialExpressionEditorX) : Expr->MaterialExpressionEditorX;
		}
		if (!bHasStartY)
		{
			StartY = bInitializedFromGraph ? FMath::Min(StartY, Expr->MaterialExpressionEditorY) : Expr->MaterialExpressionEditorY;
		}
		bInitializedFromGraph = true;
	}

	TArray<TSharedPtr<FJsonValue>> PlannedArray;
	for (int32 OrderIndex = 0; OrderIndex < ExpressionIndices.Num(); ++OrderIndex)
	{
		const int32 ExpressionIndex = ExpressionIndices[OrderIndex];
		if (!Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex))
		{
			continue;
		}
		UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex];
		if (!Expr)
		{
			continue;
		}

		const int32 TargetX = bHorizontal ? (StartX + OrderIndex * Spacing) : StartX;
		const int32 TargetY = bHorizontal ? StartY : (StartY + OrderIndex * Spacing);

		TSharedPtr<FJsonObject> ExprObj = MakeShareable(new FJsonObject());
		ExprObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
		ExprObj->SetStringField(TEXT("expression_class"), Expr->GetClass()->GetName());
		ExprObj->SetStringField(TEXT("caption"), GetMaterialExpressionPracticalName(Expr));
		ExprObj->SetNumberField(TEXT("target_x"), TargetX);
		ExprObj->SetNumberField(TEXT("target_y"), TargetY);
		PlannedArray.Add(MakeShareable(new FJsonValueObject(ExprObj)));

		if (bApplyChanges)
		{
			Expr->MaterialExpressionEditorX = TargetX;
			Expr->MaterialExpressionEditorY = TargetY;
		}
	}

	if (bApplyChanges)
	{
		Material->PreEditChange(nullptr);
		Material->PostEditChange();
		Material->MarkPackageDirty();
		FString SaveError;
		if (!SaveMaterialMutationAsset(Material, SaveError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("layout_material_expressions"));
	Result->SetStringField(TEXT("layout_mode"), bHorizontal ? TEXT("horizontal") : TEXT("vertical"));
	Result->SetNumberField(TEXT("expression_count"), PlannedArray.Num());
	Result->SetArrayField(TEXT("planned_layout"), PlannedArray);
	Result->SetStringField(TEXT("message"),
		FString::Printf(TEXT("%s layout for %d material expression(s)."),
			bApplyChanges ? TEXT("Applied") : TEXT("Planned"),
			PlannedArray.Num()));
	if (bApplyChanges)
	{
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("layout_material_expressions"), Material, SnapshotPath, true));
		AppendMaterialMutationAudit(Result, TEXT("layout_material_expressions"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
	}
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CleanupMaterialGraph(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MaterialPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MaterialPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MaterialPath);
		return false;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	const bool bCreateComments = ParseOptionalBoolField(Input, TEXT("create_comments"), false);
	const bool bIncludeUnconnected = ParseOptionalBoolField(Input, TEXT("include_unconnected"), true);

	int32 ColumnSpacing = 340;
	int32 RowSpacing = 220;
	int32 LaneSpacing = 420;
	int32 CommentPadding = 120;
	TryReadOptionalIntField(Input, TEXT("column_spacing"), ColumnSpacing);
	TryReadOptionalIntField(Input, TEXT("row_spacing"), RowSpacing);
	TryReadOptionalIntField(Input, TEXT("lane_spacing"), LaneSpacing);
	TryReadOptionalIntField(Input, TEXT("comment_padding"), CommentPadding);

	int32 StartX = 0;
	int32 StartY = 0;
	bool bHasStartX = TryReadOptionalIntField(Input, TEXT("start_x"), StartX);
	bool bHasStartY = TryReadOptionalIntField(Input, TEXT("start_y"), StartY);

	bool bInitializedFromGraph = false;
	for (UMaterialExpression* Expr : Material->GetExpressionCollection().Expressions)
	{
		if (!Expr)
		{
			continue;
		}

		if (!bHasStartX)
		{
			StartX = bInitializedFromGraph ? FMath::Min(StartX, Expr->MaterialExpressionEditorX) : Expr->MaterialExpressionEditorX;
		}
		if (!bHasStartY)
		{
			StartY = bInitializedFromGraph ? FMath::Min(StartY, Expr->MaterialExpressionEditorY) : Expr->MaterialExpressionEditorY;
		}
		bInitializedFromGraph = true;
	}

	for (UMaterialExpressionComment* Comment : GetMaterialComments(Material))
	{
		if (!Comment)
		{
			continue;
		}

		if (!bHasStartX)
		{
			StartX = bInitializedFromGraph ? FMath::Min(StartX, Comment->MaterialExpressionEditorX) : Comment->MaterialExpressionEditorX;
		}
		if (!bHasStartY)
		{
			StartY = bInitializedFromGraph ? FMath::Min(StartY, Comment->MaterialExpressionEditorY) : Comment->MaterialExpressionEditorY;
		}
		bInitializedFromGraph = true;
	}

	TMap<UMaterialExpression*, int32> ExpressionToIndex;
	for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
	{
		if (UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions[ExpressionIndex])
		{
			ExpressionToIndex.Add(Expr, ExpressionIndex);
		}
	}

	TArray<FString> RootPins;
	const TArray<TSharedPtr<FJsonValue>>* RootPinValues = nullptr;
	if (Input->TryGetArrayField(TEXT("root_pins"), RootPinValues) && RootPinValues != nullptr)
	{
		for (const TSharedPtr<FJsonValue>& Value : *RootPinValues)
		{
			if (Value.IsValid())
			{
				const FString RootPin = Value->AsString().TrimStartAndEnd();
				if (!RootPin.IsEmpty())
				{
					RootPins.Add(RootPin);
				}
			}
		}
	}

	if (RootPins.Num() == 0)
	{
		for (const FString& CandidatePin : GetDefaultMaterialCleanupRootPins())
		{
			if (FExpressionInput* RootInput = ResolveMaterialRootInput(Material, CandidatePin))
			{
				if (RootInput->Expression)
				{
					RootPins.Add(CandidatePin);
				}
			}
		}
	}

	struct FMaterialCleanupLane
	{
		FString LaneName;
		UMaterialExpression* RootExpression = nullptr;
		TSet<int32> AllIndices;
		TArray<int32> UniqueIndices;
		TMap<int32, int32> DepthByIndex;
		int32 BaseY = 0;
	};

	TArray<FMaterialCleanupLane> Lanes;
	for (const FString& RootPin : RootPins)
	{
		FExpressionInput* RootInput = ResolveMaterialRootInput(Material, RootPin);
		if (!RootInput || !RootInput->Expression)
		{
			continue;
		}

		if (RootPin.Equals(TEXT("MaterialAttributes"), ESearchCase::IgnoreCase))
		{
			UMaterialExpression* RootExpression = RootInput->Expression;
			bool bExpandedAttributes = false;
			for (FExpressionInputIterator It{ RootExpression }; RootExpression && It; ++It)
			{
				const int32 InputIndex = It.Index;
				FExpressionInput* AttributeInput = It.Input;
				if (!AttributeInput || !AttributeInput->Expression)
				{
					continue;
				}

				const FString AttributeName = RootExpression->GetInputName(InputIndex).ToString();
				if (AttributeName.IsEmpty() ||
					AttributeName.StartsWith(TEXT("CustomizedUVs"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("Tangent"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("SubsurfaceColor"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("ClearCoat"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("ClearCoatRoughness"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("Refraction"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("ShadingModel"), ESearchCase::IgnoreCase) ||
					AttributeName.Equals(TEXT("Displacement"), ESearchCase::IgnoreCase))
				{
					continue;
				}

				FMaterialCleanupLane& Lane = Lanes.AddDefaulted_GetRef();
				Lane.LaneName = AttributeName;
				Lane.RootExpression = AttributeInput->Expression;
				CollectMaterialUpstreamExpressionIndices(AttributeInput->Expression, ExpressionToIndex, Lane.AllIndices);
				bExpandedAttributes = true;
			}

			if (bExpandedAttributes)
			{
				continue;
			}
		}

		FMaterialCleanupLane& Lane = Lanes.AddDefaulted_GetRef();
		Lane.LaneName = RootPin;
		Lane.RootExpression = RootInput->Expression;
		CollectMaterialUpstreamExpressionIndices(RootInput->Expression, ExpressionToIndex, Lane.AllIndices);
	}

	if (Lanes.Num() == 0)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_CLEANUP_NO_CONNECTED_ROOTS"),
			TEXT("No connected Material root pins were found. Provide root_pins explicitly or connect the graph before cleanup."));
	}

	TMap<int32, int32> UsageCount;
	TSet<int32> CoveredIndices;
	for (const FMaterialCleanupLane& Lane : Lanes)
	{
		for (const int32 ExpressionIndex : Lane.AllIndices)
		{
			UsageCount.FindOrAdd(ExpressionIndex) += 1;
			CoveredIndices.Add(ExpressionIndex);
		}
	}

	TSet<int32> SharedIndices;
	for (const TPair<int32, int32>& Pair : UsageCount)
	{
		if (Pair.Value >= 2)
		{
			SharedIndices.Add(Pair.Key);
		}
	}

	for (FMaterialCleanupLane& Lane : Lanes)
	{
		for (const int32 ExpressionIndex : Lane.AllIndices)
		{
			if (!SharedIndices.Contains(ExpressionIndex))
			{
				Lane.UniqueIndices.Add(ExpressionIndex);
			}
		}

		TSet<int32> AllowedUniqueIndices;
		for (const int32 ExpressionIndex : Lane.UniqueIndices)
		{
			AllowedUniqueIndices.Add(ExpressionIndex);
		}

		for (const int32 ExpressionIndex : Lane.UniqueIndices)
		{
			UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex)
				? Material->GetExpressionCollection().Expressions[ExpressionIndex] : nullptr;
			ComputeMaterialCleanupDepth(Expr, AllowedUniqueIndices, ExpressionToIndex, Lane.DepthByIndex);
		}

		Lane.UniqueIndices.Sort([Material, &Lane](const int32 Left, const int32 Right)
		{
			const int32 LeftDepth = Lane.DepthByIndex.FindRef(Left);
			const int32 RightDepth = Lane.DepthByIndex.FindRef(Right);
			if (LeftDepth != RightDepth)
			{
				return LeftDepth < RightDepth;
			}

			const UMaterialExpression* LeftExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Left)
				? Material->GetExpressionCollection().Expressions[Left] : nullptr;
			const UMaterialExpression* RightExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Right)
				? Material->GetExpressionCollection().Expressions[Right] : nullptr;
			const int32 LeftY = LeftExpr ? LeftExpr->MaterialExpressionEditorY : 0;
			const int32 RightY = RightExpr ? RightExpr->MaterialExpressionEditorY : 0;
			return LeftY < RightY;
		});
	}

	TMap<int32, int32> SharedDepthByIndex;
	TArray<int32> SharedIndexArray = SharedIndices.Array();
	for (const int32 ExpressionIndex : SharedIndexArray)
	{
		UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex)
			? Material->GetExpressionCollection().Expressions[ExpressionIndex] : nullptr;
		ComputeMaterialCleanupDepth(Expr, SharedIndices, ExpressionToIndex, SharedDepthByIndex);
	}
	SharedIndexArray.Sort([Material, &SharedDepthByIndex](const int32 Left, const int32 Right)
	{
		const int32 LeftDepth = SharedDepthByIndex.FindRef(Left);
		const int32 RightDepth = SharedDepthByIndex.FindRef(Right);
		if (LeftDepth != RightDepth)
		{
			return LeftDepth < RightDepth;
		}

		const UMaterialExpression* LeftExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Left)
			? Material->GetExpressionCollection().Expressions[Left] : nullptr;
		const UMaterialExpression* RightExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Right)
			? Material->GetExpressionCollection().Expressions[Right] : nullptr;
		const int32 LeftY = LeftExpr ? LeftExpr->MaterialExpressionEditorY : 0;
		const int32 RightY = RightExpr ? RightExpr->MaterialExpressionEditorY : 0;
		return LeftY < RightY;
	});

	TMap<int32, FIntPoint> PlannedPositions;
	TMap<FString, TArray<int32>> CommentToIndices;

	const int32 SharedMaxDepth = [&SharedDepthByIndex]()
	{
		int32 Value = 0;
		for (const TPair<int32, int32>& Pair : SharedDepthByIndex)
		{
			Value = FMath::Max(Value, Pair.Value);
		}
		return Value;
	}();

	TMap<int32, int32> SharedRowsByDepth;
	for (const int32 ExpressionIndex : SharedIndexArray)
	{
		const int32 Depth = SharedDepthByIndex.FindRef(ExpressionIndex);
		const int32 RowIndex = SharedRowsByDepth.FindOrAdd(Depth);
		SharedRowsByDepth[Depth] = RowIndex + 1;
		PlannedPositions.Add(ExpressionIndex, FIntPoint(StartX + Depth * ColumnSpacing, StartY + RowIndex * RowSpacing));
		CommentToIndices.FindOrAdd(TEXT("Shared Inputs")).Add(ExpressionIndex);
	}

	const int32 SharedBlockWidth = SharedDepthByIndex.Num() > 0 ? (SharedMaxDepth + 1) * ColumnSpacing + ColumnSpacing : 0;
	const int32 LaneStartX = StartX + SharedBlockWidth;

	for (int32 LaneIndex = 0; LaneIndex < Lanes.Num(); ++LaneIndex)
	{
		FMaterialCleanupLane& Lane = Lanes[LaneIndex];
		Lane.BaseY = StartY + LaneIndex * LaneSpacing;
		TMap<int32, int32> RowsByDepth;
		for (const int32 ExpressionIndex : Lane.UniqueIndices)
		{
			const int32 Depth = Lane.DepthByIndex.FindRef(ExpressionIndex);
			const int32 RowIndex = RowsByDepth.FindOrAdd(Depth);
			RowsByDepth[Depth] = RowIndex + 1;
			PlannedPositions.Add(ExpressionIndex, FIntPoint(LaneStartX + Depth * ColumnSpacing, Lane.BaseY + RowIndex * RowSpacing));
			CommentToIndices.FindOrAdd(Lane.LaneName).Add(ExpressionIndex);
		}
	}

	if (bIncludeUnconnected)
	{
		TArray<int32> LooseIndices;
		for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			if (Material->GetExpressionCollection().Expressions[ExpressionIndex] && !CoveredIndices.Contains(ExpressionIndex))
			{
				LooseIndices.Add(ExpressionIndex);
			}
		}

		LooseIndices.Sort([Material](const int32 Left, const int32 Right)
		{
			const UMaterialExpression* LeftExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Left)
				? Material->GetExpressionCollection().Expressions[Left] : nullptr;
			const UMaterialExpression* RightExpr = Material->GetExpressionCollection().Expressions.IsValidIndex(Right)
				? Material->GetExpressionCollection().Expressions[Right] : nullptr;
			const FString LeftCategory = LeftExpr ? GetMaterialExpressionCategory(LeftExpr->GetClass()->GetName()) : TEXT("");
			const FString RightCategory = RightExpr ? GetMaterialExpressionCategory(RightExpr->GetClass()->GetName()) : TEXT("");
			if (LeftCategory != RightCategory)
			{
				return LeftCategory < RightCategory;
			}
			return Left < Right;
		});

		const int32 LooseBaseY = StartY + Lanes.Num() * LaneSpacing + RowSpacing;
		for (int32 LooseIndex = 0; LooseIndex < LooseIndices.Num(); ++LooseIndex)
		{
			const int32 ColumnIndex = LooseIndex % 3;
			const int32 RowIndex = LooseIndex / 3;
			PlannedPositions.Add(LooseIndices[LooseIndex], FIntPoint(LaneStartX + ColumnIndex * ColumnSpacing, LooseBaseY + RowIndex * RowSpacing));
			CommentToIndices.FindOrAdd(TEXT("Loose / Utility")).Add(LooseIndices[LooseIndex]);
		}
	}

	struct FMaterialCleanupCommentPlan
	{
		FString Text;
		FLinearColor Color;
		TArray<int32> ExpressionIndices;
		int32 EditorX = 0;
		int32 EditorY = 0;
		int32 Width = 0;
		int32 Height = 0;
	};

	TArray<FMaterialCleanupCommentPlan> CommentPlans;
	for (const TPair<FString, TArray<int32>>& Pair : CommentToIndices)
	{
		if (Pair.Value.Num() == 0)
		{
			continue;
		}

		int32 MinX = TNumericLimits<int32>::Max();
		int32 MinY = TNumericLimits<int32>::Max();
		int32 MaxX = TNumericLimits<int32>::Lowest();
		int32 MaxY = TNumericLimits<int32>::Lowest();
		for (const int32 ExpressionIndex : Pair.Value)
		{
			const FIntPoint* PlannedPoint = PlannedPositions.Find(ExpressionIndex);
			if (!PlannedPoint)
			{
				continue;
			}

			MinX = FMath::Min(MinX, PlannedPoint->X);
			MinY = FMath::Min(MinY, PlannedPoint->Y);
			MaxX = FMath::Max(MaxX, PlannedPoint->X + 260);
			MaxY = FMath::Max(MaxY, PlannedPoint->Y + 180);
		}

		if (MinX == TNumericLimits<int32>::Max())
		{
			continue;
		}

		FMaterialCleanupCommentPlan& Plan = CommentPlans.AddDefaulted_GetRef();
		Plan.Text = Pair.Key;
		Plan.Color = GetMaterialCleanupLaneColor(Pair.Key);
		Plan.ExpressionIndices = Pair.Value;
		Plan.EditorX = MinX - CommentPadding;
		Plan.EditorY = MinY - CommentPadding;
		Plan.Width = (MaxX - MinX) + CommentPadding * 2;
		Plan.Height = (MaxY - MinY) + CommentPadding * 2;
	}

	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("cleanup_material_graph"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), !bApplyChanges);
	Result->SetStringField(TEXT("action"), TEXT("cleanup_material_graph"));
	Result->SetStringField(TEXT("layout_style"), TEXT("left_to_right_lanes"));
	Result->SetNumberField(TEXT("expression_count"), PlannedPositions.Num());
	Result->SetNumberField(TEXT("lane_count"), Lanes.Num());
	Result->SetBoolField(TEXT("comments_enabled"), bCreateComments);

	TArray<TSharedPtr<FJsonValue>> PlannedExpressionArray;
	for (const TPair<int32, FIntPoint>& Pair : PlannedPositions)
	{
		const int32 ExpressionIndex = Pair.Key;
		UMaterialExpression* Expr = Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex)
			? Material->GetExpressionCollection().Expressions[ExpressionIndex] : nullptr;
		if (!Expr)
		{
			continue;
		}

		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
		NodeObj->SetStringField(TEXT("caption"), GetMaterialExpressionPracticalName(Expr));
		NodeObj->SetStringField(TEXT("expression_class"), Expr->GetClass()->GetName());
		NodeObj->SetNumberField(TEXT("target_x"), Pair.Value.X);
		NodeObj->SetNumberField(TEXT("target_y"), Pair.Value.Y);
		PlannedExpressionArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));

		if (bApplyChanges)
		{
			Expr->MaterialExpressionEditorX = Pair.Value.X;
			Expr->MaterialExpressionEditorY = Pair.Value.Y;
		}
	}
	Result->SetArrayField(TEXT("planned_expressions"), PlannedExpressionArray);

	TArray<TSharedPtr<FJsonValue>> LaneArray;
	for (const FMaterialCleanupLane& Lane : Lanes)
	{
		TSharedPtr<FJsonObject> LaneObj = MakeShareable(new FJsonObject());
		LaneObj->SetStringField(TEXT("lane_name"), Lane.LaneName);
		LaneObj->SetArrayField(TEXT("all_expression_indices"), MakeMaterialIntJsonArray(Lane.AllIndices.Array()));
		LaneObj->SetArrayField(TEXT("unique_expression_indices"), MakeMaterialIntJsonArray(Lane.UniqueIndices));
		LaneArray.Add(MakeShareable(new FJsonValueObject(LaneObj)));
	}
	Result->SetArrayField(TEXT("lanes"), LaneArray);
	Result->SetArrayField(TEXT("shared_expression_indices"), MakeMaterialIntJsonArray(SharedIndices.Array()));

	TArray<TSharedPtr<FJsonValue>> CommentArray;
	if (bCreateComments)
	{
		for (const FMaterialCleanupCommentPlan& Plan : CommentPlans)
		{
			TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
			CommentObj->SetStringField(TEXT("comment_text"), Plan.Text);
			CommentObj->SetNumberField(TEXT("editor_x"), Plan.EditorX);
			CommentObj->SetNumberField(TEXT("editor_y"), Plan.EditorY);
			CommentObj->SetNumberField(TEXT("width"), Plan.Width);
			CommentObj->SetNumberField(TEXT("height"), Plan.Height);
			CommentObj->SetArrayField(TEXT("expression_indices"), MakeMaterialIntJsonArray(Plan.ExpressionIndices));
			CommentArray.Add(MakeShareable(new FJsonValueObject(CommentObj)));
		}
	}
	Result->SetArrayField(TEXT("planned_comments"), CommentArray);

	if (!bApplyChanges)
	{
		Result->SetStringField(TEXT("message"), bCreateComments
			? TEXT("Dry-run: planned a left-to-right material graph cleanup with opt-in lane comments.")
			: TEXT("Dry-run: planned a left-to-right material graph cleanup without comment boxes."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	if (bCreateComments)
	{
		for (const FMaterialCleanupCommentPlan& Plan : CommentPlans)
		{
			const int32 ExistingCommentIndex = FindMaterialCommentIndexByExactText(Material, Plan.Text);
			TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
			UMaterialExpressionComment* CommentExpr = (Comments.IsValidIndex(ExistingCommentIndex) ? Comments[ExistingCommentIndex] : nullptr);
			if (!CommentExpr)
			{
				CommentExpr = NewObject<UMaterialExpressionComment>(Material);
				CommentExpr->Text = Plan.Text;
				CommentExpr->bGroupMode = true;
				Material->GetExpressionCollection().AddComment(CommentExpr);
			}

			CommentExpr->MaterialExpressionEditorX = Plan.EditorX;
			CommentExpr->MaterialExpressionEditorY = Plan.EditorY;
			CommentExpr->SizeX = Plan.Width;
			CommentExpr->SizeY = Plan.Height;
			CommentExpr->CommentColor = Plan.Color;
		}
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("cleanup_material_graph"), Material, SnapshotPath, true));
	Result->SetStringField(TEXT("message"), bCreateComments
		? TEXT("Applied left-to-right material graph cleanup with opt-in comment framing.")
		: TEXT("Applied left-to-right material graph cleanup without comment boxes."));
	AppendMaterialMutationAudit(Result, TEXT("cleanup_material_graph"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ConnectMaterialNodes(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MatPath);
		return false;
	}

	int32 SourceIndex = (int32)Input->GetNumberField(TEXT("source_index"));
	int32 SourceOutputIndex = 0;
	if (Input->HasField(TEXT("source_output_index")))
	{
		SourceOutputIndex = (int32)Input->GetNumberField(TEXT("source_output_index"));
	}

	FString TargetType;
	if (!Input->TryGetStringField(TEXT("target_type"), TargetType))
	{
		OutResult = TEXT("Missing required field: target_type");
		return false;
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString SnapshotPath;
	if (bApplyChanges)
	{
		FString SnapshotError;
		if (!CreateMaterialMutationSnapshot(Material, TEXT("connect_material_nodes"), SnapshotPath, SnapshotError))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
		}
	}

	// Get source expression
	auto& Expressions = Material->GetExpressionCollection().Expressions;
	if (SourceIndex < 0 || SourceIndex >= Expressions.Num())
	{
		OutResult = FString::Printf(TEXT("Invalid source_index %d (material has %d expressions)"),
			SourceIndex, Expressions.Num());
		return false;
	}
	UMaterialExpression* SourceExpr = Expressions[SourceIndex];

	// Validate source output index bounds
	const TArray<FExpressionOutput>& SourceOutputs = SourceExpr->GetOutputs();
	if (SourceOutputIndex >= SourceOutputs.Num() && SourceOutputs.Num() > 0)
	{
		OutResult = FString::Printf(
			TEXT("{\"success\":false,\"error\":\"source_output_index %d is out of bounds. Expression '%s' (index %d) has %d output(s).\"}"),
			SourceOutputIndex, *SourceExpr->GetClass()->GetName(), SourceIndex, SourceOutputs.Num());
		return false;
	}

	auto GetMaskLabel = [](uint32 Mask) -> FString
	{
		FString Label;
		if (Mask & 1) Label += TEXT("R");
		if (Mask & 2) Label += TEXT("G");
		if (Mask & 4) Label += TEXT("B");
		if (Mask & 8) Label += TEXT("A");
		return Label.IsEmpty() ? TEXT("None") : Label;
	};

	auto CountChannelsFromMask = [](uint32 Mask) -> int32
	{
		int32 Channels = 0;
		if (Mask & 1) Channels++;
		if (Mask & 2) Channels++;
		if (Mask & 4) Channels++;
		if (Mask & 8) Channels++;
		return Channels;
	};

	auto ResolveOutputMask = [](const FExpressionOutput& Output) -> uint32
	{
		uint32 Mask = 0;
		if (Output.MaskR) Mask |= 1;
		if (Output.MaskG) Mask |= 2;
		if (Output.MaskB) Mask |= 4;
		if (Output.MaskA) Mask |= 8;

		if (Mask == 0)
		{
			if (Output.Mask != 0)
			{
				Mask = Output.Mask;
			}
			else
			{
				Mask = 1 | 2 | 4 | 8;
			}
		}
		return Mask;
	};

	auto BuildExpressionSummary = [](UMaterialExpression* Expr, int32 ExprIndex) -> TSharedPtr<FJsonObject>
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		if (!Expr)
		{
			return Obj;
		}

		TArray<FString> Captions;
		Expr->GetCaption(Captions);
		const FString Caption = Captions.Num() > 0 ? FString::Join(Captions, TEXT(" | ")) : Expr->GetClass()->GetName();

		Obj->SetNumberField(TEXT("expression_index"), ExprIndex);
		Obj->SetStringField(TEXT("expression_class"), Expr->GetClass()->GetName());
		Obj->SetStringField(TEXT("caption"), Caption);
		return Obj;
	};

	auto BuildSourceOutputPreview = [&](UMaterialExpression* Expr) -> TArray<TSharedPtr<FJsonValue>>
	{
		TArray<TSharedPtr<FJsonValue>> OutputArray;
		if (!Expr)
		{
			return OutputArray;
		}

		const TArray<FExpressionOutput>& Outputs = Expr->GetOutputs();
		for (int32 OutputIndex = 0; OutputIndex < Outputs.Num(); ++OutputIndex)
		{
			const FExpressionOutput& Output = Outputs[OutputIndex];
			const uint32 EffectiveMask = ResolveOutputMask(Output);
			TSharedPtr<FJsonObject> OutputObj = MakeShareable(new FJsonObject());
			OutputObj->SetNumberField(TEXT("index"), OutputIndex);
			OutputObj->SetStringField(TEXT("name"), Output.OutputName.IsNone() ? TEXT("Output") : Output.OutputName.ToString());
			OutputObj->SetNumberField(TEXT("mask"), EffectiveMask);
			OutputObj->SetStringField(TEXT("mask_label"), GetMaskLabel(EffectiveMask));
			OutputObj->SetNumberField(TEXT("channel_count"), CountChannelsFromMask(EffectiveMask));
			OutputArray.Add(MakeShareable(new FJsonValueObject(OutputObj)));
		}
		return OutputArray;
	};

	// Self-connection prevention
	if (TargetType.Equals(TEXT("expression"), ESearchCase::IgnoreCase))
	{
		int32 EarlyTargetIndex = -1;
		if (Input->HasField(TEXT("target_index")))
		{
			EarlyTargetIndex = (int32)Input->GetNumberField(TEXT("target_index"));
		}
		if (EarlyTargetIndex == SourceIndex)
		{
			OutResult = TEXT("{\"success\":false,\"error\":\"Cannot connect an expression to itself.\"}");
			return false;
		}
	}

	if (TargetType.Equals(TEXT("material_pin"), ESearchCase::IgnoreCase))
	{
		FString TargetPin;
		if (!Input->TryGetStringField(TEXT("target_pin"), TargetPin))
		{
			OutResult = TEXT("Missing required field: target_pin (for material_pin target)");
			return false;
		}

		auto* EditorData = Material->GetEditorOnlyData();
		FExpressionInput* InputPin = nullptr;

		if (TargetPin.Equals(TEXT("BaseColor"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->BaseColor;
		else if (TargetPin.Equals(TEXT("Metallic"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->Metallic;
		else if (TargetPin.Equals(TEXT("Roughness"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->Roughness;
		else if (TargetPin.Equals(TEXT("Normal"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->Normal;
		else if (TargetPin.Equals(TEXT("EmissiveColor"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->EmissiveColor;
		else if (TargetPin.Equals(TEXT("Opacity"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->Opacity;
		else if (TargetPin.Equals(TEXT("OpacityMask"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->OpacityMask;
		else if (TargetPin.Equals(TEXT("WorldPositionOffset"), ESearchCase::IgnoreCase))
			InputPin = &EditorData->WorldPositionOffset;
		else
		{
			OutResult = FString::Printf(TEXT("Unknown material pin: '%s'. Valid: BaseColor, Metallic, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, WorldPositionOffset"), *TargetPin);
			return false;
		}

		if (!bApplyChanges)
		{
			const TArray<FExpressionOutput>& FinalSourceOutputs = SourceExpr->GetOutputs();
			const TArray<TSharedPtr<FJsonValue>> SourceOutputPreview = BuildSourceOutputPreview(SourceExpr);
			TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
			DryRunResult->SetBoolField(TEXT("success"), true);
			DryRunResult->SetBoolField(TEXT("dry_run"), true);
			DryRunResult->SetStringField(TEXT("action"), TEXT("connect_material_nodes"));
			DryRunResult->SetStringField(TEXT("target_type"), TEXT("material_pin"));
			DryRunResult->SetStringField(TEXT("target_pin"), TargetPin);
			DryRunResult->SetObjectField(TEXT("source_expression"), BuildExpressionSummary(SourceExpr, SourceIndex));
			DryRunResult->SetArrayField(TEXT("source_outputs"), SourceOutputPreview);
			DryRunResult->SetNumberField(TEXT("source_output_index"), SourceOutputIndex);
			DryRunResult->SetStringField(TEXT("message"),
				FString::Printf(TEXT("Dry-run: would connect expression[%d] output %d -> material %s pin"),
					SourceIndex, SourceOutputIndex, *TargetPin));
			DryRunResult->SetNumberField(TEXT("source_output_count"), FinalSourceOutputs.Num());
			if (FinalSourceOutputs.IsValidIndex(SourceOutputIndex))
			{
				const uint32 OutputMask = ResolveOutputMask(FinalSourceOutputs[SourceOutputIndex]);
				DryRunResult->SetStringField(TEXT("source_output_mask_label"), GetMaskLabel(OutputMask));
				DryRunResult->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
			}
			DryRunResult->SetBoolField(TEXT("target_pin_had_connection"), InputPin->Expression != nullptr);
			if (InputPin->Expression)
			{
				DryRunResult->SetObjectField(TEXT("target_pin_existing_connection"),
					BuildExpressionSummary(InputPin->Expression, INDEX_NONE));
				DryRunResult->SetNumberField(TEXT("target_pin_existing_output_index"), InputPin->OutputIndex);
			}
			OutResult = JsonObjToString(DryRunResult);
			return true;
		}

		SourceExpr->ConnectExpression(InputPin, SourceOutputIndex);

		Material->PreEditChange(nullptr);
		Material->PostEditChange();
		Material->MarkPackageDirty();

		const TArray<FExpressionOutput>& FinalSourceOutputs = SourceExpr->GetOutputs();
		const TArray<TSharedPtr<FJsonValue>> SourceOutputPreview = BuildSourceOutputPreview(SourceExpr);
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), false);
		Result->SetStringField(TEXT("action"), TEXT("connect_material_nodes"));
		Result->SetStringField(TEXT("target_type"), TEXT("material_pin"));
		Result->SetStringField(TEXT("target_pin"), TargetPin);
		Result->SetObjectField(TEXT("source_expression"), BuildExpressionSummary(SourceExpr, SourceIndex));
		Result->SetArrayField(TEXT("source_outputs"), SourceOutputPreview);
		Result->SetNumberField(TEXT("source_output_index"), SourceOutputIndex);
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Connected expression[%d] output %d -> material %s pin"),
				SourceIndex, SourceOutputIndex, *TargetPin));
		Result->SetNumberField(TEXT("source_output_count"), FinalSourceOutputs.Num());
		// Channel count info
		if (FinalSourceOutputs.IsValidIndex(SourceOutputIndex))
		{
			const uint32 OutputMask = ResolveOutputMask(FinalSourceOutputs[SourceOutputIndex]);
			Result->SetStringField(TEXT("source_output_mask_label"), GetMaskLabel(OutputMask));
			Result->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
		}
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("connect_material_nodes"), Material, SnapshotPath, true));
		AppendMaterialMutationAudit(Result, TEXT("connect_material_nodes"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
		OutResult = JsonObjToString(Result);
		return true;
	}
	else if (TargetType.Equals(TEXT("expression"), ESearchCase::IgnoreCase))
	{
		int32 TargetIndex = -1;
		if (Input->HasField(TEXT("target_index")))
		{
			TargetIndex = (int32)Input->GetNumberField(TEXT("target_index"));
		}
		else
		{
			OutResult = TEXT("Missing target_index for expression target type");
			return false;
		}

		if (TargetIndex < 0 || TargetIndex >= Expressions.Num())
		{
			OutResult = FString::Printf(TEXT("Invalid target_index %d"), TargetIndex);
			return false;
		}

		UMaterialExpression* TargetExpr = Expressions[TargetIndex];

		FString TargetInputName;
		Input->TryGetStringField(TEXT("target_input_name"), TargetInputName);
		auto NormalizeInputName = [](const FString& InName) -> FString
		{
			FString Normalized = InName.TrimStartAndEnd();
			const int32 SuffixIndex = Normalized.Find(TEXT(" ("), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			if (SuffixIndex != INDEX_NONE)
			{
				Normalized = Normalized.Left(SuffixIndex);
			}
			Normalized = Normalized.Replace(TEXT("_"), TEXT(" "));
			Normalized = Normalized.TrimStartAndEnd();
			return Normalized;
		};

		int32 TargetInputIndex = 0;
		if (!TargetInputName.IsEmpty())
		{
			TargetInputIndex = INDEX_NONE;
			const FString NormalizedTargetInputName = NormalizeInputName(TargetInputName);
			for (FExpressionInputIterator It{ TargetExpr }; It; ++It)
			{
				const int32 InputIndex = It.Index;
				const FString RawInputName = TargetExpr->GetInputName(InputIndex).ToString();
				const FString NormalizedCandidateName = NormalizeInputName(RawInputName);
				if (RawInputName.Equals(TargetInputName, ESearchCase::IgnoreCase) ||
					NormalizedCandidateName.Equals(NormalizedTargetInputName, ESearchCase::IgnoreCase))
				{
					TargetInputIndex = InputIndex;
					break;
				}
			}

			if (TargetInputIndex == INDEX_NONE)
			{
				TArray<FString> AvailableInputNames;
				for (FExpressionInputIterator It{ TargetExpr }; It; ++It)
				{
					const int32 InputIndex = It.Index;
					AvailableInputNames.Add(TargetExpr->GetInputName(InputIndex).ToString());
				}

				OutResult = FString::Printf(
					TEXT("Could not find input '%s' on expression[%d]. Available inputs: %s"),
					*TargetInputName,
					TargetIndex,
					*FString::Join(AvailableInputNames, TEXT(", ")));
				return false;
			}
		}
		else if (Input->HasField(TEXT("target_input_index")))
		{
			TargetInputIndex = (int32)Input->GetNumberField(TEXT("target_input_index"));
		}

		const int32 TargetInputCount = TargetExpr->CountInputs();
		if (TargetInputIndex < 0 || TargetInputIndex >= TargetInputCount)
		{
			OutResult = FString::Printf(TEXT("Invalid target_input_index %d (expression has %d inputs)"),
				TargetInputIndex, TargetInputCount);
			return false;
		}

		TArray<TSharedPtr<FJsonValue>> TargetInputPreview;
		for (int32 InputIndex = 0; InputIndex < TargetInputCount; ++InputIndex)
		{
			FExpressionInput* PreviewInput = TargetExpr->GetInput(InputIndex);
			TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject());
			InputObj->SetNumberField(TEXT("index"), InputIndex);
			InputObj->SetStringField(TEXT("name"), TargetExpr->GetInputName(InputIndex).ToString());
			InputObj->SetBoolField(TEXT("connected"), PreviewInput && PreviewInput->Expression != nullptr);
			if (PreviewInput && PreviewInput->Expression)
			{
				InputObj->SetStringField(TEXT("connected_expression_class"), PreviewInput->Expression->GetClass()->GetName());
				InputObj->SetNumberField(TEXT("connected_output_index"), PreviewInput->OutputIndex);
			}
			TargetInputPreview.Add(MakeShareable(new FJsonValueObject(InputObj)));
		}

		FExpressionInput* TargetInput = TargetExpr->GetInput(TargetInputIndex);
		if (!TargetInput)
		{
			OutResult = FString::Printf(TEXT("Target input %d is null on expression[%d]"), TargetInputIndex, TargetIndex);
			return false;
		}

		if (!bApplyChanges)
		{
			const TArray<FExpressionOutput>& FinalSourceOutputs = SourceExpr->GetOutputs();
			const TArray<TSharedPtr<FJsonValue>> SourceOutputPreview = BuildSourceOutputPreview(SourceExpr);
			TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
			DryRunResult->SetBoolField(TEXT("success"), true);
			DryRunResult->SetBoolField(TEXT("dry_run"), true);
			DryRunResult->SetStringField(TEXT("action"), TEXT("connect_material_nodes"));
			DryRunResult->SetStringField(TEXT("target_type"), TEXT("expression"));
			DryRunResult->SetObjectField(TEXT("source_expression"), BuildExpressionSummary(SourceExpr, SourceIndex));
			DryRunResult->SetArrayField(TEXT("source_outputs"), SourceOutputPreview);
			DryRunResult->SetNumberField(TEXT("source_output_index"), SourceOutputIndex);
			DryRunResult->SetObjectField(TEXT("target_expression"), BuildExpressionSummary(TargetExpr, TargetIndex));
			DryRunResult->SetNumberField(TEXT("target_input_index"), TargetInputIndex);
			DryRunResult->SetStringField(TEXT("target_input_name"), TargetExpr->GetInputName(TargetInputIndex).ToString());
			DryRunResult->SetArrayField(TEXT("target_inputs"), TargetInputPreview);
			DryRunResult->SetStringField(TEXT("message"),
				FString::Printf(TEXT("Dry-run: would connect expression[%d] output %d -> expression[%d] input %d"),
					SourceIndex, SourceOutputIndex, TargetIndex, TargetInputIndex));
			DryRunResult->SetNumberField(TEXT("source_output_count"), FinalSourceOutputs.Num());
			DryRunResult->SetNumberField(TEXT("target_input_count"), TargetInputCount);
			if (FinalSourceOutputs.IsValidIndex(SourceOutputIndex))
			{
				const uint32 OutputMask = ResolveOutputMask(FinalSourceOutputs[SourceOutputIndex]);
				DryRunResult->SetStringField(TEXT("source_output_mask_label"), GetMaskLabel(OutputMask));
				DryRunResult->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
			}
			DryRunResult->SetBoolField(TEXT("target_input_had_connection"), TargetInput->Expression != nullptr);
			if (TargetInput->Expression)
			{
				DryRunResult->SetObjectField(TEXT("target_input_existing_connection"),
					BuildExpressionSummary(TargetInput->Expression, INDEX_NONE));
				DryRunResult->SetNumberField(TEXT("target_input_existing_output_index"), TargetInput->OutputIndex);
			}
			OutResult = JsonObjToString(DryRunResult);
			return true;
		}

		SourceExpr->ConnectExpression(TargetInput, SourceOutputIndex);

		Material->PreEditChange(nullptr);
		Material->PostEditChange();
		Material->MarkPackageDirty();

		const TArray<FExpressionOutput>& FinalSourceOutputs = SourceExpr->GetOutputs();
		const TArray<TSharedPtr<FJsonValue>> SourceOutputPreview = BuildSourceOutputPreview(SourceExpr);
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), false);
		Result->SetStringField(TEXT("action"), TEXT("connect_material_nodes"));
		Result->SetStringField(TEXT("target_type"), TEXT("expression"));
		Result->SetObjectField(TEXT("source_expression"), BuildExpressionSummary(SourceExpr, SourceIndex));
		Result->SetArrayField(TEXT("source_outputs"), SourceOutputPreview);
		Result->SetNumberField(TEXT("source_output_index"), SourceOutputIndex);
		Result->SetObjectField(TEXT("target_expression"), BuildExpressionSummary(TargetExpr, TargetIndex));
		Result->SetNumberField(TEXT("target_input_index"), TargetInputIndex);
		Result->SetStringField(TEXT("target_input_name"), TargetExpr->GetInputName(TargetInputIndex).ToString());
		Result->SetArrayField(TEXT("target_inputs"), TargetInputPreview);
		Result->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Connected expression[%d] output %d -> expression[%d] input %d"),
				SourceIndex, SourceOutputIndex, TargetIndex, TargetInputIndex));
		Result->SetNumberField(TEXT("source_output_count"), FinalSourceOutputs.Num());
		Result->SetNumberField(TEXT("target_input_count"), TargetInputCount);
		if (FinalSourceOutputs.IsValidIndex(SourceOutputIndex))
		{
			const uint32 OutputMask = ResolveOutputMask(FinalSourceOutputs[SourceOutputIndex]);
			Result->SetStringField(TEXT("source_output_mask_label"), GetMaskLabel(OutputMask));
			Result->SetNumberField(TEXT("source_channel_count"), CountChannelsFromMask(OutputMask));
		}
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("connect_material_nodes"), Material, SnapshotPath, true));
		AppendMaterialMutationAudit(Result, TEXT("connect_material_nodes"), Material, Result->GetStringField(TEXT("message")), nullptr, SnapshotPath);
		OutResult = JsonObjToString(Result);
		return true;
	}
	else
	{
		OutResult = FString::Printf(TEXT("Invalid target_type '%s'. Use 'material_pin' or 'expression'."), *TargetType);
		return false;
	}
}

bool FVFXToolRegistry::Tool_SetMaterialProperties(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MatPath);
		return false;
	}

	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	TArray<FString> PlannedChanges;

	bool bHasBlendModeUpdate = false;
	EBlendMode TargetBlendMode = Material->BlendMode;
	FString BlendMode;
	if (Input->TryGetStringField(TEXT("blend_mode"), BlendMode))
	{
		if (BlendMode.Equals(TEXT("Opaque"), ESearchCase::IgnoreCase))
			TargetBlendMode = BLEND_Opaque;
		else if (BlendMode.Equals(TEXT("Masked"), ESearchCase::IgnoreCase))
			TargetBlendMode = BLEND_Masked;
		else if (BlendMode.Equals(TEXT("Translucent"), ESearchCase::IgnoreCase))
			TargetBlendMode = BLEND_Translucent;
		else if (BlendMode.Equals(TEXT("Additive"), ESearchCase::IgnoreCase))
			TargetBlendMode = BLEND_Additive;
		else if (BlendMode.Equals(TEXT("Modulate"), ESearchCase::IgnoreCase))
			TargetBlendMode = BLEND_Modulate;
		else
		{
			OutResult = FString::Printf(TEXT("Unknown blend_mode: '%s'"), *BlendMode);
			return false;
		}
		bHasBlendModeUpdate = true;
		PlannedChanges.Add(FString::Printf(TEXT("BlendMode=%s"), *BlendMode));
	}

	bool bHasShadingModelUpdate = false;
	EMaterialShadingModel TargetShadingModel = MSM_DefaultLit;
	FString ShadingModel;
	if (Input->TryGetStringField(TEXT("shading_model"), ShadingModel))
	{
		if (ShadingModel.Equals(TEXT("DefaultLit"), ESearchCase::IgnoreCase))
			TargetShadingModel = MSM_DefaultLit;
		else if (ShadingModel.Equals(TEXT("Unlit"), ESearchCase::IgnoreCase))
			TargetShadingModel = MSM_Unlit;
		else if (ShadingModel.Equals(TEXT("Subsurface"), ESearchCase::IgnoreCase))
			TargetShadingModel = MSM_Subsurface;
		else if (ShadingModel.Equals(TEXT("ClearCoat"), ESearchCase::IgnoreCase))
			TargetShadingModel = MSM_ClearCoat;
		else
		{
			OutResult = FString::Printf(TEXT("Unknown shading_model: '%s'"), *ShadingModel);
			return false;
		}
		bHasShadingModelUpdate = true;
		PlannedChanges.Add(FString::Printf(TEXT("ShadingModel=%s"), *ShadingModel));
	}

	bool bHasTwoSidedUpdate = false;
	bool bTargetTwoSided = Material->TwoSided;
	if (Input->HasField(TEXT("two_sided")))
	{
		bTargetTwoSided = ParseOptionalBoolField(Input, TEXT("two_sided"), Material->TwoSided);
		bHasTwoSidedUpdate = true;
		PlannedChanges.Add(FString::Printf(TEXT("TwoSided=%s"), bTargetTwoSided ? TEXT("true") : TEXT("false")));
	}

	bool bHasThumbnailPrimitiveUpdate = false;
	EThumbnailPrimType TargetThumbnailPrimitive = TPT_Sphere;
	bool bTargetThumbnailPrimitiveValid = false;
	FString ThumbnailPrimitive;
	if (Input->TryGetStringField(TEXT("thumbnail_primitive"), ThumbnailPrimitive))
	{
		if (ThumbnailPrimitive.Equals(TEXT("None"), ESearchCase::IgnoreCase))
		{
			TargetThumbnailPrimitive = TPT_None;
		}
		else if (ThumbnailPrimitive.Equals(TEXT("Sphere"), ESearchCase::IgnoreCase))
		{
			TargetThumbnailPrimitive = TPT_Sphere;
		}
		else if (ThumbnailPrimitive.Equals(TEXT("Cube"), ESearchCase::IgnoreCase))
		{
			TargetThumbnailPrimitive = TPT_Cube;
		}
		else if (ThumbnailPrimitive.Equals(TEXT("Plane"), ESearchCase::IgnoreCase))
		{
			TargetThumbnailPrimitive = TPT_Plane;
		}
		else if (ThumbnailPrimitive.Equals(TEXT("Cylinder"), ESearchCase::IgnoreCase))
		{
			TargetThumbnailPrimitive = TPT_Cylinder;
		}
		else
		{
			OutResult = FString::Printf(TEXT("Unknown thumbnail_primitive: '%s'"), *ThumbnailPrimitive);
			return false;
		}

		bHasThumbnailPrimitiveUpdate = true;
		bTargetThumbnailPrimitiveValid = true;
		PlannedChanges.Add(FString::Printf(TEXT("ThumbnailPrimitive=%s"), *ThumbnailPrimitive));
	}

	bool bHasPreviewMeshUpdate = false;
	FString PreviewMeshPath;
	if (Input->TryGetStringField(TEXT("preview_mesh_path"), PreviewMeshPath))
	{
		bHasPreviewMeshUpdate = true;
		PlannedChanges.Add(FString::Printf(TEXT("PreviewMesh=%s"), *PreviewMeshPath));
	}

	if (PlannedChanges.Num() == 0)
	{
		OutResult = TEXT("No properties were changed. Specify at least one of: blend_mode, shading_model, two_sided, thumbnail_primitive, preview_mesh_path");
		return false;
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRunResult = MakeShareable(new FJsonObject());
		DryRunResult->SetBoolField(TEXT("success"), true);
		DryRunResult->SetBoolField(TEXT("dry_run"), true);
		DryRunResult->SetStringField(TEXT("action"), TEXT("set_material_properties"));
		TArray<TSharedPtr<FJsonValue>> PlannedChangeValues;
		for (const FString& PlannedChange : PlannedChanges)
		{
			PlannedChangeValues.Add(MakeShareable(new FJsonValueString(PlannedChange)));
		}
		DryRunResult->SetArrayField(TEXT("planned_changes"), PlannedChangeValues);
		DryRunResult->SetStringField(TEXT("message"),
			FString::Printf(TEXT("Dry-run: would update material properties: %s"), *FString::Join(PlannedChanges, TEXT(", "))));
		OutResult = JsonObjToString(DryRunResult);
		return true;
	}

	FString SnapshotPath;
	FString SnapshotError;
	if (!CreateMaterialMutationSnapshot(Material, TEXT("set_material_properties"), SnapshotPath, SnapshotError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), SnapshotError);
	}

	if (bHasBlendModeUpdate)
	{
		Material->BlendMode = TargetBlendMode;
	}
	if (bHasShadingModelUpdate)
	{
		Material->SetShadingModel(TargetShadingModel);
	}
	if (bHasTwoSidedUpdate)
	{
		Material->TwoSided = bTargetTwoSided;
	}
	if (bHasThumbnailPrimitiveUpdate || bHasPreviewMeshUpdate)
	{
		USceneThumbnailInfoWithPrimitive* ThumbnailInfo = Cast<USceneThumbnailInfoWithPrimitive>(Material->ThumbnailInfo);
		if (!ThumbnailInfo)
		{
			ThumbnailInfo = NewObject<USceneThumbnailInfoWithPrimitive>(Material, NAME_None, RF_Transactional);
			Material->ThumbnailInfo = ThumbnailInfo;
		}

		if (ThumbnailInfo)
		{
			if (bHasThumbnailPrimitiveUpdate && bTargetThumbnailPrimitiveValid)
			{
				ThumbnailInfo->PrimitiveType = TargetThumbnailPrimitive;
				ThumbnailInfo->bUserModifiedShape = true;
			}
			if (bHasPreviewMeshUpdate)
			{
				ThumbnailInfo->PreviewMesh = FSoftObjectPath(PreviewMeshPath);
				Material->PreviewMesh = FSoftObjectPath(PreviewMeshPath);
			}
		}
	}

	Material->PreEditChange(nullptr);
	Material->PostEditChange();
	Material->MarkPackageDirty();
	FString SaveError;
	if (!SaveMaterialMutationAsset(Material, SaveError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SAVE_FAILED"), SaveError);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("set_material_properties"));
	const FString SetPropertiesMessage = FString::Printf(TEXT("Updated material properties: %s"), *FString::Join(PlannedChanges, TEXT(", ")));
	Result->SetStringField(TEXT("message"), SetPropertiesMessage);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(TEXT("set_material_properties"), Material, SnapshotPath, true));
	AppendMaterialMutationAudit(Result, TEXT("set_material_properties"), Material, SetPropertiesMessage, nullptr, SnapshotPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetMaterialFunctionInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString FunctionPath;
	if (!Input->TryGetStringField(TEXT("material_function_path"), FunctionPath))
	{
		OutResult = TEXT("Missing required field: material_function_path");
		return false;
	}

	FString FocusKeyword;
	Input->TryGetStringField(TEXT("focus_keyword"), FocusKeyword);

	UMaterialFunctionInterface* FunctionAsset = Cast<UMaterialFunctionInterface>(UEditorAssetLibrary::LoadAsset(FunctionPath));
	if (!FunctionAsset)
	{
		OutResult = FString::Printf(TEXT("Material Function not found at '%s'"), *FunctionPath);
		return false;
	}

	UMaterialFunctionInterface* InspectFunction = FunctionAsset->GetBaseFunctionInterface();
	if (!InspectFunction)
	{
		InspectFunction = FunctionAsset;
	}

	const TConstArrayView<TObjectPtr<UMaterialExpression>> ExpressionsView = InspectFunction->GetExpressions();
	TMap<const UMaterialExpression*, int32> ExpressionToIndex;
	for (int32 Index = 0; Index < ExpressionsView.Num(); ++Index)
	{
		if (ExpressionsView[Index])
		{
			ExpressionToIndex.Add(ExpressionsView[Index], Index);
		}
	}

	TArray<TSharedPtr<FJsonValue>> NodeArray;
	TArray<TSharedPtr<FJsonValue>> ConnectionArray;
	TArray<TSharedPtr<FJsonValue>> InputArray;
	TArray<TSharedPtr<FJsonValue>> OutputArray;

	for (int32 Index = 0; Index < ExpressionsView.Num(); ++Index)
	{
		UMaterialExpression* Expr = ExpressionsView[Index];
		if (!Expr)
		{
			continue;
		}

		TSharedPtr<FJsonObject> NodeObj = MakeShareable(new FJsonObject());
		NodeObj->SetNumberField(TEXT("index"), Index);
		NodeObj->SetStringField(TEXT("class"), Expr->GetClass()->GetName());
		NodeObj->SetStringField(TEXT("caption"), GetMaterialExpressionCaptionText(Expr));
		NodeObj->SetStringField(TEXT("desc"), Expr->Desc);
		NodeObj->SetNumberField(TEXT("editor_x"), Expr->MaterialExpressionEditorX);
		NodeObj->SetNumberField(TEXT("editor_y"), Expr->MaterialExpressionEditorY);

		TArray<TSharedPtr<FJsonValue>> ExprInputs;
		TArray<TSharedPtr<FJsonValue>> ExprConnections;
		CollectExpressionInputConnections(Expr, ExpressionToIndex, ExprInputs, ExprConnections);
		NodeObj->SetArrayField(TEXT("inputs"), ExprInputs);
		NodeObj->SetNumberField(TEXT("input_count"), ExprInputs.Num());
		NodeObj->SetStringField(TEXT("node_kind"), TEXT("expression"));

		if (UMaterialExpressionFunctionInput* FunctionInput = Cast<UMaterialExpressionFunctionInput>(Expr))
		{
			NodeObj->SetStringField(TEXT("node_kind"), TEXT("function_input"));
			NodeObj->SetStringField(TEXT("input_name"), FunctionInput->InputName.ToString());
			NodeObj->SetStringField(TEXT("description"), FunctionInput->Description);
			NodeObj->SetStringField(TEXT("input_type"), GetFunctionInputTypeName(FunctionInput->InputType));
			NodeObj->SetNumberField(TEXT("sort_priority"), FunctionInput->SortPriority);
			NodeObj->SetBoolField(TEXT("use_preview_as_default"), FunctionInput->bUsePreviewValueAsDefault);

			TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject());
			InputObj->SetStringField(TEXT("name"), FunctionInput->InputName.ToString());
			InputObj->SetStringField(TEXT("description"), FunctionInput->Description);
			InputObj->SetStringField(TEXT("input_type"), GetFunctionInputTypeName(FunctionInput->InputType));
			InputObj->SetNumberField(TEXT("sort_priority"), FunctionInput->SortPriority);
			InputArray.Add(MakeShareable(new FJsonValueObject(InputObj)));
		}
		else if (UMaterialExpressionFunctionOutput* FunctionOutput = Cast<UMaterialExpressionFunctionOutput>(Expr))
		{
			NodeObj->SetStringField(TEXT("node_kind"), TEXT("function_output"));
			NodeObj->SetStringField(TEXT("output_name"), FunctionOutput->OutputName.ToString());
			NodeObj->SetStringField(TEXT("description"), FunctionOutput->Description);
			NodeObj->SetNumberField(TEXT("sort_priority"), FunctionOutput->SortPriority);

			TSharedPtr<FJsonObject> OutputObj = MakeShareable(new FJsonObject());
			OutputObj->SetStringField(TEXT("name"), FunctionOutput->OutputName.ToString());
			OutputObj->SetStringField(TEXT("description"), FunctionOutput->Description);
			OutputObj->SetNumberField(TEXT("sort_priority"), FunctionOutput->SortPriority);
			if (FunctionOutput->A.Expression)
			{
				OutputObj->SetStringField(TEXT("source_caption"), GetMaterialExpressionCaptionText(FunctionOutput->A.Expression));
				if (const int32* SourceIndex = ExpressionToIndex.Find(FunctionOutput->A.Expression))
				{
					OutputObj->SetNumberField(TEXT("source_index"), *SourceIndex);
				}
			}
			OutputArray.Add(MakeShareable(new FJsonValueObject(OutputObj)));
		}
		else if (UMaterialExpressionMaterialFunctionCall* FunctionCall = Cast<UMaterialExpressionMaterialFunctionCall>(Expr))
		{
			NodeObj->SetStringField(TEXT("node_kind"), TEXT("function_call"));
			NodeObj->SetStringField(TEXT("function_name"), FunctionCall->MaterialFunction ? FunctionCall->MaterialFunction->GetName() : TEXT(""));
			NodeObj->SetStringField(TEXT("function_path"), FunctionCall->MaterialFunction ? FunctionCall->MaterialFunction->GetPathName() : TEXT(""));
		}
		else if (UMaterialExpressionCustom* CustomExpr = Cast<UMaterialExpressionCustom>(Expr))
		{
			NodeObj->SetStringField(TEXT("node_kind"), TEXT("custom"));
			NodeObj->SetNumberField(TEXT("code_length"), CustomExpr->Code.Len());
		}

		NodeArray.Add(MakeShareable(new FJsonValueObject(NodeObj)));
		ConnectionArray.Append(ExprConnections);
	}

	TArray<TSharedPtr<FJsonValue>> CommentArray;
	if (UMaterialFunction* BaseFunction = FunctionAsset->GetBaseFunction())
	{
		for (TObjectPtr<UMaterialExpressionComment> Comment : BaseFunction->GetEditorComments())
		{
			if (!Comment)
			{
				continue;
			}

			TSharedPtr<FJsonObject> CommentObj = MakeShareable(new FJsonObject());
			CommentObj->SetStringField(TEXT("text"), Comment->Text);
			CommentObj->SetNumberField(TEXT("editor_x"), Comment->MaterialExpressionEditorX);
			CommentObj->SetNumberField(TEXT("editor_y"), Comment->MaterialExpressionEditorY);
			CommentObj->SetNumberField(TEXT("width"), Comment->SizeX);
			CommentObj->SetNumberField(TEXT("height"), Comment->SizeY);
			CommentArray.Add(MakeShareable(new FJsonValueObject(CommentObj)));
		}
	}

	TArray<UMaterialFunctionInterface*> DependentFunctions;
	InspectFunction->GetDependentFunctions(DependentFunctions);
	TArray<TSharedPtr<FJsonValue>> DependentFunctionArray;
	for (UMaterialFunctionInterface* DependentFunction : DependentFunctions)
	{
		if (!DependentFunction)
		{
			continue;
		}

		TSharedPtr<FJsonObject> DepObj = MakeShareable(new FJsonObject());
		DepObj->SetStringField(TEXT("name"), DependentFunction->GetName());
		DepObj->SetStringField(TEXT("path"), DependentFunction->GetPathName());
		DependentFunctionArray.Add(MakeShareable(new FJsonValueObject(DepObj)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("material_function_path"), FunctionAsset->GetPathName());
	Result->SetStringField(TEXT("material_function_name"), FunctionAsset->GetName());
	Result->SetStringField(TEXT("class"), FunctionAsset->GetClass()->GetName());
	Result->SetStringField(TEXT("base_function_path"), InspectFunction ? InspectFunction->GetPathName() : TEXT(""));
	Result->SetStringField(TEXT("description"), FunctionAsset->GetDescription());
	Result->SetArrayField(TEXT("nodes"), NodeArray);
	Result->SetArrayField(TEXT("connections"), ConnectionArray);
	Result->SetArrayField(TEXT("function_inputs"), InputArray);
	Result->SetArrayField(TEXT("function_outputs"), OutputArray);
	Result->SetArrayField(TEXT("comments"), CommentArray);
	Result->SetArrayField(TEXT("dependent_functions"), DependentFunctionArray);
	Result->SetStringField(
		TEXT("call_site_connection_guidance"),
		TEXT("When this function is instantiated through a MaterialFunctionCall, prefer connect_material_nodes.target_input_name over raw target_input_index. Some call nodes are easier to wire safely by pin name than by assumed index order."));
	Result->SetNumberField(TEXT("node_count"), NodeArray.Num());
	Result->SetNumberField(TEXT("connection_count"), ConnectionArray.Num());
	Result->SetNumberField(TEXT("input_count"), InputArray.Num());
	Result->SetNumberField(TEXT("output_count"), OutputArray.Num());
	Result->SetNumberField(TEXT("dependent_function_count"), DependentFunctionArray.Num());
	Result->SetStringField(TEXT("focus_keyword"), FocusKeyword);

	if (!FocusKeyword.IsEmpty())
	{
		const FString FocusLower = FocusKeyword.ToLower();
		TArray<TSharedPtr<FJsonValue>> FocusNodes;
		for (const TSharedPtr<FJsonValue>& NodeValue : NodeArray)
		{
			const TSharedPtr<FJsonObject> NodeObj = NodeValue->AsObject();
			if (!NodeObj.IsValid())
			{
				continue;
			}

			FString SearchText;
			NodeObj->TryGetStringField(TEXT("caption"), SearchText);
			FString Desc;
			NodeObj->TryGetStringField(TEXT("desc"), Desc);
			SearchText += TEXT(" ") + Desc;
			FString InputName;
			NodeObj->TryGetStringField(TEXT("input_name"), InputName);
			SearchText += TEXT(" ") + InputName;
			FString OutputName;
			NodeObj->TryGetStringField(TEXT("output_name"), OutputName);
			SearchText += TEXT(" ") + OutputName;
			FString FunctionName;
			NodeObj->TryGetStringField(TEXT("function_name"), FunctionName);
			SearchText += TEXT(" ") + FunctionName;

			if (SearchText.ToLower().Contains(FocusLower))
			{
				FocusNodes.Add(MakeShareable(new FJsonValueObject(NodeObj)));
			}
		}

		TSharedPtr<FJsonObject> FocusObj = MakeShareable(new FJsonObject());
		FocusObj->SetStringField(TEXT("focus_keyword"), FocusKeyword);
		FocusObj->SetArrayField(TEXT("matched_nodes"), FocusNodes);
		FocusObj->SetNumberField(TEXT("matched_node_count"), FocusNodes.Num());
		Result->SetObjectField(TEXT("focus"), FocusObj);
	}

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewMaterialFunction(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString FunctionPath;
	if (!Input->TryGetStringField(TEXT("material_function_path"), FunctionPath))
	{
		OutResult = TEXT("Missing required field: material_function_path");
		return false;
	}

	UMaterialFunctionInterface* FunctionAsset = Cast<UMaterialFunctionInterface>(UEditorAssetLibrary::LoadAsset(FunctionPath));
	if (!FunctionAsset)
	{
		OutResult = FString::Printf(TEXT("Material Function not found at '%s'"), *FunctionPath);
		return false;
	}

	UMaterialFunctionInterface* InspectFunction = FunctionAsset->GetBaseFunctionInterface();
	if (!InspectFunction)
	{
		InspectFunction = FunctionAsset;
	}

	const TConstArrayView<TObjectPtr<UMaterialExpression>> Expressions = InspectFunction->GetExpressions();
	int32 InputCount = 0;
	int32 OutputCount = 0;
	int32 CustomNodeCount = 0;
	int32 FunctionCallCount = 0;
	int32 ConstantCount = 0;
	for (UMaterialExpression* Expr : Expressions)
	{
		if (!Expr)
		{
			continue;
		}

		InputCount += Expr->IsA<UMaterialExpressionFunctionInput>() ? 1 : 0;
		OutputCount += Expr->IsA<UMaterialExpressionFunctionOutput>() ? 1 : 0;
		CustomNodeCount += Expr->IsA<UMaterialExpressionCustom>() ? 1 : 0;
		FunctionCallCount += Expr->IsA<UMaterialExpressionMaterialFunctionCall>() ? 1 : 0;
		ConstantCount += (Expr->IsA<UMaterialExpressionConstant>() || Expr->IsA<UMaterialExpressionConstant2Vector>() || Expr->IsA<UMaterialExpressionConstant3Vector>()) ? 1 : 0;
	}

	TArray<TSharedPtr<FJsonValue>> Findings;
	TArray<TSharedPtr<FJsonValue>> Guidance;
	auto AddFinding = [&Findings](const FString& Severity, const FString& Finding, const FString& Reason, const FString& SuggestedFix)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("finding"), Finding);
		FindingObj->SetStringField(TEXT("reason"), Reason);
		FindingObj->SetStringField(TEXT("suggested_fix"), SuggestedFix);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	};
	auto AddGuidance = [&Guidance](const FString& Category, const FString& Suggestion, const FString& Reason)
	{
		TSharedPtr<FJsonObject> GuidanceObj = MakeShareable(new FJsonObject());
		GuidanceObj->SetStringField(TEXT("category"), Category);
		GuidanceObj->SetStringField(TEXT("suggestion"), Suggestion);
		GuidanceObj->SetStringField(TEXT("reason"), Reason);
		Guidance.Add(MakeShareable(new FJsonValueObject(GuidanceObj)));
	};

	if (InputCount == 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Function has no inputs"),
			TEXT("Material Function exposes no inputs, which limits reuse and tuning from callers."),
			TEXT("Expose at least the key artistic controls as function inputs if the logic is intended to be reused."));
	}

	if (OutputCount == 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Function has no outputs"),
			TEXT("Material Function exposes no outputs."),
			TEXT("Add at least one function output or remove the function if it is incomplete."));
	}

	if (Expressions.Num() > 120)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Large material function graph"),
			FString::Printf(TEXT("Function has %d expressions, which can make reuse and maintenance harder."), Expressions.Num()),
			TEXT("Break repeated logic into smaller helper functions or remove duplicated math blocks."));
	}

	if (CustomNodeCount > 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Custom HLSL nodes present"),
			FString::Printf(TEXT("Function contains %d Custom node(s)."), CustomNodeCount),
			TEXT("Verify the HLSL is documented and only used where graph nodes or smaller functions would be worse."));
	}

	if (ConstantCount > 6 && InputCount <= 1)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Many hard-coded constants"),
			FString::Printf(TEXT("Function contains %d constant-style nodes and only %d exposed input(s)."), ConstantCount, InputCount),
			TEXT("Expose more controls as function inputs if this logic is meant to be reused across multiple looks."));
	}

	if (FunctionCallCount == 0 && Expressions.Num() > 40)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Could be split into helper functions"),
			TEXT("The function is moderately large but does not call any other material functions."),
			TEXT("Consider extracting common sub-blocks into smaller reusable Material Functions."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious function-level issues"),
			TEXT("No high-signal maintainability or reuse issues were detected by current rules."),
			TEXT("Validate the function in its calling materials and tune exposed inputs for artist usability."));
	}

	int32 MaintainabilityScore = 100;
	MaintainabilityScore -= FMath::Clamp(static_cast<int32>(Expressions.Num() / 8), 0, 25);
	MaintainabilityScore -= CustomNodeCount > 0 ? 10 : 0;
	MaintainabilityScore -= ConstantCount > 6 ? 8 : 0;
	MaintainabilityScore -= InputCount == 0 ? 12 : 0;
	MaintainabilityScore = FMath::Clamp(MaintainabilityScore, 0, 100);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_material_function"));
	Result->SetStringField(TEXT("material_function_path"), FunctionAsset->GetPathName());
	Result->SetStringField(TEXT("material_function_name"), FunctionAsset->GetName());
	Result->SetNumberField(TEXT("expression_count"), Expressions.Num());
	Result->SetNumberField(TEXT("input_count"), InputCount);
	Result->SetNumberField(TEXT("output_count"), OutputCount);
	Result->SetNumberField(TEXT("custom_node_count"), CustomNodeCount);
	Result->SetNumberField(TEXT("function_call_count"), FunctionCallCount);
	Result->SetNumberField(TEXT("constant_count"), ConstantCount);
	Result->SetNumberField(TEXT("maintainability_score"), MaintainabilityScore);
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetArrayField(TEXT("findings"), Findings);

	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_GetMaterialMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("material_asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_MISSING_ASSET_PATH"), TEXT("Missing required field: material_asset_path"));
	}

	UObject* Asset = UEditorAssetLibrary::LoadAsset(AssetPath);
	if (!IsMaterialMutationAsset(Asset))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_ASSET_NOT_FOUND"),
			FString::Printf(TEXT("Material asset not found or unsupported at '%s'."), *AssetPath));
	}

	int32 MaxResults = 20;
	TryReadOptionalIntField(Input, TEXT("max_results"), MaxResults);
	MaxResults = FMath::Clamp(MaxResults, 1, 200);

	TArray<FString> SnapshotFiles;
	GetMaterialSnapshotFiles(Asset, SnapshotFiles);

	TArray<TSharedPtr<FJsonValue>> SnapshotArray;
	for (int32 Index = 0; Index < SnapshotFiles.Num() && Index < MaxResults; ++Index)
	{
		const FString& SnapshotPath = SnapshotFiles[Index];
		TSharedPtr<FJsonObject> SnapshotObj = MakeShareable(new FJsonObject());
		SnapshotObj->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		SnapshotObj->SetStringField(TEXT("file_name"), FPaths::GetCleanFilename(SnapshotPath));
		SnapshotObj->SetStringField(TEXT("timestamp_utc"), IFileManager::Get().GetTimeStamp(*SnapshotPath).ToIso8601());
		SnapshotObj->SetNumberField(TEXT("size_bytes"), static_cast<double>(IFileManager::Get().FileSize(*SnapshotPath)));
		SnapshotArray.Add(MakeShareable(new FJsonValueObject(SnapshotObj)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("get_material_mutation_snapshots"));
	Result->SetStringField(TEXT("material_asset_path"), Asset->GetPathName());
	Result->SetStringField(TEXT("asset_class"), Asset->GetClass()->GetName());
	Result->SetStringField(TEXT("snapshot_root"), GetMaterialMutationSnapshotRoot());
	Result->SetNumberField(TEXT("count"), SnapshotArray.Num());
	Result->SetArrayField(TEXT("snapshots"), SnapshotArray);
	Result->SetStringField(TEXT("message"), SnapshotArray.Num() > 0
		? TEXT("Material mutation snapshots found.")
		: TEXT("No material mutation snapshots found."));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RestoreMaterialMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString AssetPath;
	if (!Input->TryGetStringField(TEXT("material_asset_path"), AssetPath) || AssetPath.IsEmpty())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_MISSING_ASSET_PATH"), TEXT("Missing required field: material_asset_path"));
	}

	UObject* Asset = UEditorAssetLibrary::LoadAsset(AssetPath);
	if (!IsMaterialMutationAsset(Asset))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_ASSET_NOT_FOUND"),
			FString::Printf(TEXT("Material asset not found or unsupported at '%s'."), *AssetPath));
	}

	FString SnapshotPath;
	Input->TryGetStringField(TEXT("snapshot_path"), SnapshotPath);
	const bool bUseLatest = ParseOptionalBoolField(Input, TEXT("use_latest"), true);
	const bool bReopenEditor = ParseOptionalBoolField(Input, TEXT("reopen_editor"), false);
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);

	if (SnapshotPath.IsEmpty() && bUseLatest)
	{
		TArray<FString> SnapshotFiles;
		GetMaterialSnapshotFiles(Asset, SnapshotFiles);
		if (SnapshotFiles.Num() > 0)
		{
			SnapshotPath = SnapshotFiles[0];
		}
	}

	if (SnapshotPath.IsEmpty())
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_SNAPSHOT_NOT_SPECIFIED"),
			TEXT("No snapshot_path was provided and no latest snapshot could be resolved."));
	}

	SnapshotPath = FPaths::ConvertRelativePathToFull(SnapshotPath);
	if (!FPaths::FileExists(SnapshotPath))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_SNAPSHOT_NOT_FOUND"),
			FString::Printf(TEXT("Snapshot file does not exist: %s"), *SnapshotPath));
	}

	if (!IsMaterialSnapshotPathUnderRoot(SnapshotPath))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_SNAPSHOT_OUT_OF_BOUNDS"),
			TEXT("Snapshot path must live under the ForgePilot MaterialMutationSnapshots directory."));
	}

	const FString PackageFilename = GetAssetPackageFilename(Asset);
	if (PackageFilename.IsEmpty() || !FPaths::FileExists(PackageFilename))
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_PACKAGE_FILE_MISSING"),
			FString::Printf(TEXT("Material package file does not exist on disk: %s"), *PackageFilename));
	}

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetBoolField(TEXT("dry_run"), true);
		Result->SetStringField(TEXT("action"), TEXT("restore_material_mutation_snapshot"));
		Result->SetBoolField(TEXT("will_mutate_asset"), false);
		Result->SetStringField(TEXT("material_asset_path"), Asset->GetPathName());
		Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
		Result->SetStringField(TEXT("package_filename"), PackageFilename);
		Result->SetStringField(TEXT("message"), TEXT("Dry-run: would restore the Material package from the selected snapshot."));
		OutResult = JsonObjToString(Result);
		return true;
	}

	FString PreRestoreSnapshotPath;
	FString PreRestoreSnapshotError;
	if (!CreateMaterialMutationSnapshot(Asset, TEXT("pre_restore"), PreRestoreSnapshotPath, PreRestoreSnapshotError))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SNAPSHOT_FAILED"), PreRestoreSnapshotError);
	}

	int32 ClosedEditorCount = 0;
	if (GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			ClosedEditorCount = AssetEditorSubsystem->CloseAllEditorsForAsset(Asset);
		}
	}

	const uint32 CopyResult = IFileManager::Get().Copy(*PackageFilename, *SnapshotPath, true, true);
	if (CopyResult != COPY_OK)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_RESTORE_COPY_FAILED"),
			FString::Printf(TEXT("Failed to copy snapshot into package file (copy code %u)."), CopyResult));
	}

	FText ReloadError;
	const bool bReloadSucceeded = UPackageTools::ReloadPackages(
		{ Asset->GetOutermost() },
		ReloadError,
		EReloadPackagesInteractionMode::AssumePositive);

	UObject* ReloadedAsset = UEditorAssetLibrary::LoadAsset(AssetPath);
	if (bReopenEditor && ReloadedAsset && GEditor)
	{
		if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			AssetEditorSubsystem->OpenEditorForAsset(ReloadedAsset);
		}
	}

	TSharedPtr<FJsonObject> PatchDetails = MakeShareable(new FJsonObject());
	PatchDetails->SetStringField(TEXT("restored_from_snapshot"), SnapshotPath);
	PatchDetails->SetStringField(TEXT("pre_restore_snapshot"), PreRestoreSnapshotPath);
	PatchDetails->SetStringField(TEXT("package_filename"), PackageFilename);
	PatchDetails->SetNumberField(TEXT("closed_editor_count"), ClosedEditorCount);
	if (!ReloadError.IsEmpty())
	{
		PatchDetails->SetStringField(TEXT("reload_error"), ReloadError.ToString());
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), bReloadSucceeded);
	Result->SetBoolField(TEXT("dry_run"), false);
	Result->SetStringField(TEXT("action"), TEXT("restore_material_mutation_snapshot"));
	Result->SetBoolField(TEXT("will_mutate_asset"), true);
	Result->SetStringField(TEXT("material_asset_path"), AssetPath);
	Result->SetStringField(TEXT("snapshot_path"), SnapshotPath);
	Result->SetObjectField(TEXT("patch_report"), BuildMaterialMutationPatchReport(
		TEXT("restore_material_mutation_snapshot"),
		ReloadedAsset ? ReloadedAsset : Asset,
		PreRestoreSnapshotPath,
		bReloadSucceeded,
		PatchDetails));

	const FString RestoreMessage = bReloadSucceeded
		? TEXT("Material snapshot restored and package reloaded.")
		: TEXT("Snapshot file was copied, but package reload reported a problem. See patch_report details.");
	Result->SetStringField(TEXT("message"), RestoreMessage);
	AppendMaterialMutationAudit(
		Result,
		TEXT("restore_material_mutation_snapshot"),
		ReloadedAsset ? ReloadedAsset : Asset,
		RestoreMessage,
		PatchDetails,
		PreRestoreSnapshotPath,
		bReloadSucceeded);

	OutResult = JsonObjToString(Result);
	return bReloadSucceeded;
}

bool FVFXToolRegistry::Tool_ReviewMaterialAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	FString CreativeMode;
	Input->TryGetStringField(TEXT("creative_mode"), CreativeMode);

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MatPath);
		return false;
	}

	const TArray<UMaterialExpression*>& Expressions = Material->GetExpressionCollection().Expressions;
	int32 TextureSampleCount = 0;
	int32 TextureParameterCount = 0;
	int32 CustomExpressionCount = 0;
	int32 ConstantCount = 0;
	int32 ScalarParameterCount = 0;
	int32 VectorParameterCount = 0;
	int32 FunctionCallCount = 0;
	int32 HardcodedTextureSampleCount = 0;
	int32 HighSpeedPannerCount = 0;
	TArray<TSharedPtr<FJsonValue>> ParameterizationCandidates;
	for (UMaterialExpression* Expr : Expressions)
	{
		if (!Expr) continue;
		if (Expr->IsA<UMaterialExpressionTextureSample>())
		{
			++TextureSampleCount;
		}
		if (Expr->IsA<UMaterialExpressionTextureSampleParameter2D>())
		{
			++TextureParameterCount;
		}
		if (Expr->IsA<UMaterialExpressionCustom>())
		{
			++CustomExpressionCount;
		}
		if (Expr->IsA<UMaterialExpressionConstant>() ||
			Expr->IsA<UMaterialExpressionConstant2Vector>() ||
			Expr->IsA<UMaterialExpressionConstant3Vector>())
		{
			++ConstantCount;
		}
		if (Expr->IsA<UMaterialExpressionScalarParameter>())
		{
			++ScalarParameterCount;
		}
		if (Expr->IsA<UMaterialExpressionVectorParameter>())
		{
			++VectorParameterCount;
		}
		if (Expr->IsA<UMaterialExpressionMaterialFunctionCall>())
		{
			++FunctionCallCount;
		}
		if (UMaterialExpressionTextureSample* TextureSample = Cast<UMaterialExpressionTextureSample>(Expr))
		{
			if (!TextureSample->IsA<UMaterialExpressionTextureSampleParameter2D>() && TextureSample->Texture)
			{
				++HardcodedTextureSampleCount;
				TSharedPtr<FJsonObject> Candidate = MakeShareable(new FJsonObject());
				Candidate->SetStringField(TEXT("type"), TEXT("texture_parameter_candidate"));
				Candidate->SetStringField(TEXT("expression"), GetMaterialExpressionPracticalName(TextureSample));
				Candidate->SetStringField(TEXT("suggested_action"), TEXT("Convert this TextureSample into TextureSampleParameter2D so a Material Instance can swap the texture."));
				ParameterizationCandidates.Add(MakeShareable(new FJsonValueObject(Candidate)));
			}
		}
		if (UMaterialExpressionPanner* Panner = Cast<UMaterialExpressionPanner>(Expr))
		{
			if (FMath::Abs(Panner->SpeedX) > 1.5f || FMath::Abs(Panner->SpeedY) > 1.5f)
			{
				++HighSpeedPannerCount;
			}
		}
		if (UMaterialExpressionConstant* ScalarConstant = Cast<UMaterialExpressionConstant>(Expr))
		{
			TSharedPtr<FJsonObject> Candidate = MakeShareable(new FJsonObject());
			Candidate->SetStringField(TEXT("type"), TEXT("scalar_parameter_candidate"));
			Candidate->SetStringField(TEXT("expression"), GetMaterialExpressionPracticalName(ScalarConstant));
			Candidate->SetStringField(TEXT("suggested_action"), TEXT("Promote frequently tuned scalar constants into ScalarParameter for MI control."));
			ParameterizationCandidates.Add(MakeShareable(new FJsonValueObject(Candidate)));
		}
	}
	const int32 CommentCount = GetMaterialComments(Material).Num();
	const int32 TotalParameterCount = ScalarParameterCount + VectorParameterCount + TextureParameterCount;

	TArray<TSharedPtr<FJsonValue>> Findings;
	TArray<TSharedPtr<FJsonValue>> Guidance;
	auto AddFinding = [&Findings](const FString& Severity, const FString& Finding, const FString& Reason, const FString& SuggestedFix)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("finding"), Finding);
		FindingObj->SetStringField(TEXT("reason"), Reason);
		FindingObj->SetStringField(TEXT("suggested_fix"), SuggestedFix);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	};
	auto AddGuidance = [&Guidance](const FString& Category, const FString& Suggestion, const FString& Reason)
	{
		TSharedPtr<FJsonObject> GuidanceObj = MakeShareable(new FJsonObject());
		GuidanceObj->SetStringField(TEXT("category"), Category);
		GuidanceObj->SetStringField(TEXT("suggestion"), Suggestion);
		GuidanceObj->SetStringField(TEXT("reason"), Reason);
		Guidance.Add(MakeShareable(new FJsonValueObject(GuidanceObj)));
	};

	if (Expressions.Num() > 180)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("Large material graph"),
			FString::Printf(TEXT("Material has %d expressions, which can hurt maintainability and compile times."), Expressions.Num()),
			TEXT("Consider moving reusable logic to material functions and cleaning unused branches."));
	}

	if (TextureSampleCount > 16)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("High texture sample count"),
			FString::Printf(TEXT("Detected %d texture sample expressions."), TextureSampleCount),
			TEXT("Pack channels, reuse shared textures, or simplify branches to reduce sample cost."));
	}

	if (CustomExpressionCount > 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Custom HLSL nodes present"),
			FString::Printf(TEXT("Detected %d Custom expression node(s)."), CustomExpressionCount),
			TEXT("Verify platform compatibility and document expected input/output ranges in the material function or recipe docs."));
	}

	if (HardcodedTextureSampleCount > 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Hard-coded texture samples"),
			FString::Printf(TEXT("Detected %d TextureSample node(s) using direct texture assets instead of TextureSampleParameter2D."), HardcodedTextureSampleCount),
			TEXT("Convert look-driving textures into TextureSampleParameter2D when you expect to tune or swap them through Material Instances."));
	}

	if (ConstantCount > TotalParameterCount + 4)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Many constants compared to exposed parameters"),
			FString::Printf(TEXT("Detected %d constant-style nodes versus %d exposed parameter nodes."), ConstantCount, TotalParameterCount),
			TEXT("Promote artistic controls to ScalarParameter / VectorParameter so Material Instances can tune the effect without reopening the graph."));

		AddGuidance(
			TEXT("parameterization"),
			TEXT("Move art-facing constants into ScalarParameter / VectorParameter nodes and expose them through Material Instances."),
			TEXT("The graph currently uses noticeably more constants than reusable parameters."));
	}

	if (FunctionCallCount == 0 && Expressions.Num() > 50)
	{
		AddFinding(
			TEXT("info"),
			TEXT("Large graph without Material Functions"),
			TEXT("The graph is moderately large but does not appear to extract reusable logic into Material Functions."),
			TEXT("Move repeated math or reusable effect blocks into a Material Function to keep the master graph readable."));

		AddGuidance(
			TEXT("architecture"),
			TEXT("Extract repeated math into one Material Function instead of duplicating node islands."),
			TEXT("Large graphs without Material Functions become hard to tune and harder to reuse across looks."));
	}

	if (HighSpeedPannerCount > 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("High-speed panner values present"),
			FString::Printf(TEXT("Detected %d panner node(s) with relatively high UV speed."), HighSpeedPannerCount),
			TEXT("Validate the motion in target lighting and temporal AA settings; very fast panners can cause noisy or unstable VFX surfaces."));

		AddGuidance(
			TEXT("uv_motion"),
			TEXT("Reduce panner speed or add masking/noise breakup if the effect flickers under TAA."),
			TEXT("Fast UV motion is a common source of unstable emissive or distortion surfaces."));
	}

	if (Material->GetShadingModels().HasShadingModel(MSM_DefaultLit) && Material->BlendMode == BLEND_Translucent)
	{
		AddFinding(
			TEXT("info"),
			TEXT("DefaultLit + Translucent combination"),
			TEXT("This setup can be visually expensive depending on scene complexity."),
			TEXT("If acceptable visually, keep it; otherwise evaluate Unlit or reduce overdraw."));
	}

	auto* EditorData = Material->GetEditorOnlyData();
	if (EditorData)
	{
		if (!EditorData->BaseColor.Expression)
		{
			AddFinding(
				TEXT("warning"),
				TEXT("BaseColor not connected"),
				TEXT("BaseColor input is not connected."),
				TEXT("Connect a color/albedo source or ensure this is intentional."));
		}

		if (Material->GetShadingModels().HasShadingModel(MSM_DefaultLit) && !EditorData->Normal.Expression)
		{
			AddFinding(
				TEXT("info"),
				TEXT("Normal not connected"),
				TEXT("DefaultLit material has no normal input connection."),
				TEXT("Connect normal data if you need richer surface detail."));
		}
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("No obvious issues"),
			TEXT("No high-signal issues detected by automated review rules."),
			TEXT("Continue with visual validation in target lighting scenarios."));
	}

	const FString CreativeModeLower = CreativeMode.ToLower();
	if (CreativeModeLower == TEXT("fire"))
	{
		AddGuidance(TEXT("creative"), TEXT("Prefer a TextureSampleParameter2D + Panner + Power/Clamp edge block before escalating to Custom/HLSL."), TEXT("This keeps fire masks MI-friendly and easier to tune."));
		AddGuidance(TEXT("creative"), TEXT("Separate flame shape, emissive intensity, and edge width into different parameters."), TEXT("Fire looks usually need independent art controls for brightness versus breakup."));
	}
	else if (CreativeModeLower == TEXT("magic"))
	{
		AddGuidance(TEXT("creative"), TEXT("Use one reusable Material Function for pulse shaping instead of duplicating Power/Clamp/Fresnel math."), TEXT("Magic surfaces often repeat the same shaping logic across several materials."));
		AddGuidance(TEXT("creative"), TEXT("Prefer parameterized UV flow and color ramps over hard-coded constants."), TEXT("Magic looks usually iterate heavily during production."));
	}
	else if (CreativeModeLower == TEXT("transform"))
	{
		AddGuidance(TEXT("creative"), TEXT("Split the graph into mask, edge, and final combine regions, ideally with Material Functions."), TEXT("Transformation materials are easier to debug when the dissolve mask and emissive edge are isolated."));
		AddGuidance(TEXT("creative"), TEXT("Use one threshold + edge-width pair instead of many chained constants for dissolve timing."), TEXT("A compact parameter pair is easier to animate from Blueprint or MI."));
	}
	else if (CreativeModeLower == TEXT("decal"))
	{
		AddGuidance(TEXT("creative"), TEXT("Keep decal breakup texture and tint as MI parameters so each instance can vary without touching the master."), TEXT("Decal work usually needs many variations with the same base logic."));
	}
	else if (CreativeModeLower == TEXT("impact"))
	{
		AddGuidance(TEXT("creative"), TEXT("Build impact marks from one mask extract chain plus a tint/intensity multiply instead of duplicating entire graphs."), TEXT("Impact materials benefit from small reusable graph chunks."));
	}

	int32 MaintainabilityScore = 100;
	MaintainabilityScore -= FMath::Clamp(static_cast<int32>(Expressions.Num() / 8), 0, 28);
	MaintainabilityScore -= FMath::Clamp(HardcodedTextureSampleCount * 3, 0, 15);
	MaintainabilityScore -= FMath::Clamp(ConstantCount > TotalParameterCount ? (ConstantCount - TotalParameterCount) : 0, 0, 12);
	MaintainabilityScore -= FunctionCallCount == 0 && Expressions.Num() > 50 ? 8 : 0;
	MaintainabilityScore = FMath::Clamp(MaintainabilityScore, 0, 100);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_material_asset"));
	Result->SetStringField(TEXT("material_path"), MatPath);
	Result->SetNumberField(TEXT("expression_count"), Expressions.Num());
	Result->SetNumberField(TEXT("texture_sample_count"), TextureSampleCount);
	Result->SetNumberField(TEXT("texture_parameter_count"), TextureParameterCount);
	Result->SetNumberField(TEXT("custom_expression_count"), CustomExpressionCount);
	Result->SetNumberField(TEXT("constant_count"), ConstantCount);
	Result->SetNumberField(TEXT("scalar_parameter_count"), ScalarParameterCount);
	Result->SetNumberField(TEXT("vector_parameter_count"), VectorParameterCount);
	Result->SetNumberField(TEXT("function_call_count"), FunctionCallCount);
	Result->SetNumberField(TEXT("comment_count"), CommentCount);
	Result->SetNumberField(TEXT("maintainability_score"), MaintainabilityScore);
	Result->SetNumberField(TEXT("finding_count"), Findings.Num());
	Result->SetStringField(TEXT("creative_mode"), CreativeMode);
	Result->SetArrayField(TEXT("parameterization_candidates"), ParameterizationCandidates);
	Result->SetArrayField(TEXT("guidance"), Guidance);
	Result->SetArrayField(TEXT("findings"), Findings);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewMaterialGraphHygiene(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString MatPath;
	if (!Input->TryGetStringField(TEXT("material_path"), MatPath))
	{
		OutResult = TEXT("Missing required field: material_path");
		return false;
	}

	const bool bRequireCommentCoverage = ParseOptionalBoolField(Input, TEXT("require_comment_coverage"), false);

	UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(MatPath));
	if (!Material)
	{
		OutResult = FString::Printf(TEXT("Material not found at '%s'"), *MatPath);
		return false;
	}

	const TArray<UMaterialExpression*>& Expressions = Material->GetExpressionCollection().Expressions;
	TMap<UMaterialExpression*, int32> ExpressionToIndex;
	for (int32 ExpressionIndex = 0; ExpressionIndex < Expressions.Num(); ++ExpressionIndex)
	{
		if (UMaterialExpression* Expression = Expressions[ExpressionIndex])
		{
			ExpressionToIndex.Add(Expression, ExpressionIndex);
		}
	}

	TSet<int32> ReachableExpressionIndices;
	for (const FString& RootPin : GetDefaultMaterialCleanupRootPins())
	{
		if (FExpressionInput* RootInput = ResolveMaterialRootInput(Material, RootPin))
		{
			if (RootInput->Expression)
			{
				TSet<int32> RootIndices;
				CollectMaterialUpstreamExpressionIndices(RootInput->Expression, ExpressionToIndex, RootIndices);
				ReachableExpressionIndices.Append(RootIndices);
			}
		}
	}

	const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
	TSet<int32> CommentCoveredIndices;
	TMap<FString, TArray<int32>> ParameterNameToIndices;
	TArray<TSharedPtr<FJsonValue>> Findings;
	TArray<TSharedPtr<FJsonValue>> DuplicateParameterGroups;
	TArray<TSharedPtr<FJsonValue>> OrphanExpressions;
	TArray<TSharedPtr<FJsonValue>> CustomNodesOutsideComments;

	auto AddFinding = [&Findings](const FString& Severity, const FString& Category, const FString& Message, const FString& SuggestedFix)
	{
		TSharedPtr<FJsonObject> FindingObj = MakeShareable(new FJsonObject());
		FindingObj->SetStringField(TEXT("severity"), Severity);
		FindingObj->SetStringField(TEXT("category"), Category);
		FindingObj->SetStringField(TEXT("message"), Message);
		FindingObj->SetStringField(TEXT("suggested_fix"), SuggestedFix);
		Findings.Add(MakeShareable(new FJsonValueObject(FindingObj)));
	};

	int32 NonCommentExpressionCount = 0;
	int32 CustomExpressionCount = 0;
	int32 CustomOutsideCommentCount = 0;

	for (int32 ExpressionIndex = 0; ExpressionIndex < Expressions.Num(); ++ExpressionIndex)
	{
		UMaterialExpression* Expression = Expressions[ExpressionIndex];
		if (!Expression || Expression->IsA<UMaterialExpressionComment>())
		{
			continue;
		}

		++NonCommentExpressionCount;

		const float CenterX = Expression->MaterialExpressionEditorX + 130.0f;
		const float CenterY = Expression->MaterialExpressionEditorY + 90.0f;
		bool bInsideComment = false;
		for (const UMaterialExpressionComment* Comment : Comments)
		{
			if (IsCommentContainingPoint(Comment, CenterX, CenterY))
			{
				bInsideComment = true;
				CommentCoveredIndices.Add(ExpressionIndex);
				break;
			}
		}

		if (!ReachableExpressionIndices.Contains(ExpressionIndex))
		{
			TSharedPtr<FJsonObject> OrphanObj = MakeShareable(new FJsonObject());
			OrphanObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
			OrphanObj->SetStringField(TEXT("expression"), GetMaterialExpressionPracticalName(Expression));
			OrphanObj->SetStringField(TEXT("class"), Expression->GetClass()->GetName());
			OrphanExpressions.Add(MakeShareable(new FJsonValueObject(OrphanObj)));
		}

		if (Expression->IsA<UMaterialExpressionCustom>())
		{
			++CustomExpressionCount;
			if (!bInsideComment)
			{
				++CustomOutsideCommentCount;
				TSharedPtr<FJsonObject> CustomObj = MakeShareable(new FJsonObject());
				CustomObj->SetNumberField(TEXT("expression_index"), ExpressionIndex);
				CustomObj->SetStringField(TEXT("expression"), GetMaterialExpressionPracticalName(Expression));
				CustomObj->SetStringField(TEXT("suggested_fix"), TEXT("Document expected inputs/outputs in the material function or recipe docs. Use a comment box only when explicitly requested."));
				CustomNodesOutsideComments.Add(MakeShareable(new FJsonValueObject(CustomObj)));
			}
		}

		static const FName ParameterNamePropertyName(TEXT("ParameterName"));
		if (const FProperty* ParameterNameProperty = Expression->GetClass()->FindPropertyByName(ParameterNamePropertyName))
		{
			if (const FNameProperty* NameProperty = CastField<FNameProperty>(ParameterNameProperty))
			{
				const FName ParameterName = NameProperty->GetPropertyValue_InContainer(Expression);
				if (!ParameterName.IsNone())
				{
					ParameterNameToIndices.FindOrAdd(ParameterName.ToString()).Add(ExpressionIndex);
				}
			}
		}
	}

	for (const TPair<FString, TArray<int32>>& Pair : ParameterNameToIndices)
	{
		if (Pair.Value.Num() <= 1)
		{
			continue;
		}

		TSharedPtr<FJsonObject> GroupObj = MakeShareable(new FJsonObject());
		GroupObj->SetStringField(TEXT("parameter_name"), Pair.Key);
		TArray<TSharedPtr<FJsonValue>> IndexValues;
		TArray<TSharedPtr<FJsonValue>> ExpressionValues;
		for (const int32 ExpressionIndex : Pair.Value)
		{
			IndexValues.Add(MakeShareable(new FJsonValueNumber(ExpressionIndex)));
			if (Expressions.IsValidIndex(ExpressionIndex) && Expressions[ExpressionIndex])
			{
				ExpressionValues.Add(MakeShareable(new FJsonValueString(GetMaterialExpressionPracticalName(Expressions[ExpressionIndex]))));
			}
		}
		GroupObj->SetArrayField(TEXT("expression_indices"), IndexValues);
		GroupObj->SetArrayField(TEXT("expressions"), ExpressionValues);
		DuplicateParameterGroups.Add(MakeShareable(new FJsonValueObject(GroupObj)));
	}

	if (OrphanExpressions.Num() > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("orphan_expressions"),
			FString::Printf(TEXT("Detected %d expression(s) that are not reachable from any connected material root pin."), OrphanExpressions.Num()),
			TEXT("Delete dead node islands or reconnect them to a real output so the graph stays intentional."));
	}

	if (DuplicateParameterGroups.Num() > 0)
	{
		AddFinding(
			TEXT("warning"),
			TEXT("duplicate_parameters"),
			FString::Printf(TEXT("Detected %d duplicated parameter name group(s)."), DuplicateParameterGroups.Num()),
			TEXT("Rename duplicated parameters unless they are deliberately shared controls, and document shared controls in the recipe or material docs."));
	}

	if (bRequireCommentCoverage && CustomOutsideCommentCount > 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("custom_nodes_without_comment"),
			FString::Printf(TEXT("Detected %d Custom node(s) outside comment coverage."), CustomOutsideCommentCount),
			TEXT("Wrap Custom/HLSL nodes in comment boxes and note expected ranges, inputs, and purpose."));
	}

	const double CommentCoverageRatio = NonCommentExpressionCount > 0
		? static_cast<double>(CommentCoveredIndices.Num()) / static_cast<double>(NonCommentExpressionCount)
		: 1.0;

	if (bRequireCommentCoverage && NonCommentExpressionCount >= 20 && CommentCoverageRatio < 0.60)
	{
		AddFinding(
			TEXT("info"),
			TEXT("comment_coverage"),
			FString::Printf(TEXT("Only %.0f%% of non-comment expressions are covered by comment boxes."), CommentCoverageRatio * 100.0),
			TEXT("Group major blocks such as UV flow, masking, emissive, and output combine into comment regions."));
	}

	if (Findings.Num() == 0)
	{
		AddFinding(
			TEXT("info"),
			TEXT("clean_graph"),
			TEXT("No obvious graph hygiene issues detected."),
			TEXT("Continue with focused art iteration and rerun hygiene review after larger graph edits."));
	}

	int32 HygieneScore = 100;
	HygieneScore -= FMath::Min(25, OrphanExpressions.Num() * 4);
	HygieneScore -= FMath::Min(20, DuplicateParameterGroups.Num() * 6);
	HygieneScore -= bRequireCommentCoverage ? FMath::Min(15, CustomOutsideCommentCount * 5) : 0;
	HygieneScore -= (bRequireCommentCoverage && NonCommentExpressionCount >= 20)
		? FMath::Clamp(FMath::RoundToInt((0.60 - CommentCoverageRatio) * 30.0), 0, 20)
		: 0;
	HygieneScore = FMath::Clamp(HygieneScore, 0, 100);

	TArray<TSharedPtr<FJsonValue>> CleanupCandidates;
	if (OrphanExpressions.Num() > 0)
	{
		TSharedPtr<FJsonObject> Candidate = MakeShareable(new FJsonObject());
		Candidate->SetStringField(TEXT("type"), TEXT("remove_orphan_expressions"));
		Candidate->SetStringField(TEXT("suggested_action"), TEXT("Review and remove orphan node islands if they are not intentionally parked for iteration."));
		Candidate->SetArrayField(TEXT("expressions"), OrphanExpressions);
		CleanupCandidates.Add(MakeShareable(new FJsonValueObject(Candidate)));
	}
	if (DuplicateParameterGroups.Num() > 0)
	{
		TSharedPtr<FJsonObject> Candidate = MakeShareable(new FJsonObject());
		Candidate->SetStringField(TEXT("type"), TEXT("rename_duplicate_parameters"));
		Candidate->SetStringField(TEXT("suggested_action"), TEXT("Normalize duplicated parameter names so each exposed control is intentional and discoverable in Material Instances."));
		Candidate->SetArrayField(TEXT("groups"), DuplicateParameterGroups);
		CleanupCandidates.Add(MakeShareable(new FJsonValueObject(Candidate)));
	}
	if (bRequireCommentCoverage && CustomNodesOutsideComments.Num() > 0)
	{
		TSharedPtr<FJsonObject> Candidate = MakeShareable(new FJsonObject());
		Candidate->SetStringField(TEXT("type"), TEXT("comment_custom_nodes"));
		Candidate->SetStringField(TEXT("suggested_action"), TEXT("Add comment boxes around Custom/HLSL nodes and document why native nodes were not used."));
		Candidate->SetArrayField(TEXT("expressions"), CustomNodesOutsideComments);
		CleanupCandidates.Add(MakeShareable(new FJsonValueObject(Candidate)));
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("review_material_graph_hygiene"));
	Result->SetStringField(TEXT("material_path"), MatPath);
	Result->SetNumberField(TEXT("expression_count"), Expressions.Num());
	Result->SetNumberField(TEXT("non_comment_expression_count"), NonCommentExpressionCount);
	Result->SetNumberField(TEXT("comment_count"), Comments.Num());
	Result->SetNumberField(TEXT("comment_covered_expression_count"), CommentCoveredIndices.Num());
	Result->SetNumberField(TEXT("comment_coverage_ratio"), CommentCoverageRatio);
	Result->SetNumberField(TEXT("orphan_expression_count"), OrphanExpressions.Num());
	Result->SetNumberField(TEXT("duplicate_parameter_group_count"), DuplicateParameterGroups.Num());
	Result->SetNumberField(TEXT("custom_expression_count"), CustomExpressionCount);
	Result->SetNumberField(TEXT("custom_nodes_outside_comment_count"), CustomOutsideCommentCount);
	Result->SetNumberField(TEXT("graph_hygiene_score"), HygieneScore);
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetArrayField(TEXT("cleanup_candidates"), CleanupCandidates);
	Result->SetArrayField(TEXT("orphan_expressions"), OrphanExpressions);
	Result->SetArrayField(TEXT("duplicate_parameter_groups"), DuplicateParameterGroups);
	Result->SetArrayField(TEXT("custom_nodes_outside_comments"), CustomNodesOutsideComments);
	OutResult = JsonObjToString(Result);
	return true;
}

namespace
{
	static bool TryReadOptionalFloatField(const TSharedPtr<FJsonObject>& Input, const TCHAR* FieldName, float& OutValue)
	{
		if (!Input.IsValid())
		{
			return false;
		}

		double NumberValue = 0.0;
		if (Input->TryGetNumberField(FieldName, NumberValue))
		{
			OutValue = static_cast<float>(NumberValue);
			return true;
		}

		FString StringValue;
		if (Input->TryGetStringField(FieldName, StringValue) && !StringValue.IsEmpty())
		{
			OutValue = FCString::Atof(*StringValue);
			return true;
		}

		return false;
	}

	static UMaterial* LoadMaterialForWorkflowTool(const TSharedPtr<FJsonObject>& Input, FString& OutMaterialPath, FString& OutError)
	{
		OutError.Reset();
		if (!Input.IsValid() || !Input->TryGetStringField(TEXT("material_path"), OutMaterialPath))
		{
			OutError = TEXT("Missing required field: material_path");
			return nullptr;
		}

		UMaterial* Material = Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(OutMaterialPath));
		if (!Material)
		{
			OutError = FString::Printf(TEXT("Could not load Material at path: %s"), *OutMaterialPath);
			return nullptr;
		}

		return Material;
	}

	static FExpressionInput* ResolveMaterialRootInput(UMaterial* Material, const FString& TargetPin)
	{
		if (!Material || TargetPin.IsEmpty())
		{
			return nullptr;
		}

		auto* EditorData = Material->GetEditorOnlyData();
		if (!EditorData)
		{
			return nullptr;
		}

		if (TargetPin.Equals(TEXT("BaseColor"), ESearchCase::IgnoreCase)) return &EditorData->BaseColor;
		if (TargetPin.Equals(TEXT("Metallic"), ESearchCase::IgnoreCase)) return &EditorData->Metallic;
		if (TargetPin.Equals(TEXT("Specular"), ESearchCase::IgnoreCase)) return &EditorData->Specular;
		if (TargetPin.Equals(TEXT("Roughness"), ESearchCase::IgnoreCase)) return &EditorData->Roughness;
		if (TargetPin.Equals(TEXT("Normal"), ESearchCase::IgnoreCase)) return &EditorData->Normal;
		if (TargetPin.Equals(TEXT("EmissiveColor"), ESearchCase::IgnoreCase)) return &EditorData->EmissiveColor;
		if (TargetPin.Equals(TEXT("AmbientOcclusion"), ESearchCase::IgnoreCase)) return &EditorData->AmbientOcclusion;
		if (TargetPin.Equals(TEXT("Opacity"), ESearchCase::IgnoreCase)) return &EditorData->Opacity;
		if (TargetPin.Equals(TEXT("OpacityMask"), ESearchCase::IgnoreCase)) return &EditorData->OpacityMask;
		if (TargetPin.Equals(TEXT("WorldPositionOffset"), ESearchCase::IgnoreCase)) return &EditorData->WorldPositionOffset;
		if (TargetPin.Equals(TEXT("MaterialAttributes"), ESearchCase::IgnoreCase)) return &EditorData->MaterialAttributes;
		return nullptr;
	}

	static void CollectExpressionsWithinBounds(UMaterial* Material, const FBox2D& Bounds, TArray<int32>& OutExpressionIndices)
	{
		OutExpressionIndices.Reset();
		if (!Material)
		{
			return;
		}

		for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			UMaterialExpression* Expression = Material->GetExpressionCollection().Expressions[ExpressionIndex];
			if (!Expression)
			{
				continue;
			}

			const FBox2D NodeBounds(
				FVector2D(Expression->MaterialExpressionEditorX, Expression->MaterialExpressionEditorY),
				FVector2D(Expression->MaterialExpressionEditorX + 260.0f, Expression->MaterialExpressionEditorY + 180.0f));

			if (Bounds.Intersect(NodeBounds))
			{
				OutExpressionIndices.Add(ExpressionIndex);
			}
		}
	}

	static void CollectExpressionsInsideComments(UMaterial* Material, const TArray<int32>& CommentIndices, TArray<int32>& OutExpressionIndices)
	{
		OutExpressionIndices.Reset();
		if (!Material || CommentIndices.Num() == 0)
		{
			return;
		}

		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		TSet<int32> UniqueIndices;

		for (int32 ExpressionIndex = 0; ExpressionIndex < Material->GetExpressionCollection().Expressions.Num(); ++ExpressionIndex)
		{
			UMaterialExpression* Expression = Material->GetExpressionCollection().Expressions[ExpressionIndex];
			if (!Expression)
			{
				continue;
			}

			const float CenterX = Expression->MaterialExpressionEditorX + 130.0f;
			const float CenterY = Expression->MaterialExpressionEditorY + 90.0f;
			for (const int32 CommentIndex : CommentIndices)
			{
				if (Comments.IsValidIndex(CommentIndex) && IsCommentContainingPoint(Comments[CommentIndex], CenterX, CenterY))
				{
					UniqueIndices.Add(ExpressionIndex);
					break;
				}
			}
		}

		for (const int32 Value : UniqueIndices)
		{
			OutExpressionIndices.Add(Value);
		}
		OutExpressionIndices.Sort();
	}

	static void ExpandFocusExpressionsFromInput(
		UMaterial* Material,
		const TSharedPtr<FJsonObject>& Input,
		TArray<int32>& InOutExpressionIndices,
		TArray<int32>& InOutCommentIndices,
		FBox2D& OutBounds,
		FString& OutFocusSource)
	{
		const FMaterialGraphViewState* ExistingState = nullptr;
		if (Material)
		{
			ExistingState = GMaterialGraphViewStates.Find(Material->GetPathName());
		}

		if (!ResolveMaterialFocusBounds(Material, Input, ExistingState, OutBounds, InOutExpressionIndices, InOutCommentIndices, OutFocusSource))
		{
			return;
		}

		if (InOutExpressionIndices.Num() == 0)
		{
			if (InOutCommentIndices.Num() > 0)
			{
				CollectExpressionsInsideComments(Material, InOutCommentIndices, InOutExpressionIndices);
			}

			if (InOutExpressionIndices.Num() == 0)
			{
				CollectExpressionsWithinBounds(Material, OutBounds, InOutExpressionIndices);
			}
		}

		if (InOutExpressionIndices.Num() > 0)
		{
			FBox2D ExpressionBounds(EForceInit::ForceInit);
			if (ComputeMaterialFocusBoundsFromExpressions(Material, InOutExpressionIndices, 120, ExpressionBounds))
			{
				OutBounds += ExpressionBounds.Min;
				OutBounds += ExpressionBounds.Max;
			}
		}
	}

	static TArray<int32> GetRequiredInputIndicesForExpression(UMaterialExpression* Expression)
	{
		TArray<int32> Required;
		if (!Expression)
		{
			return Required;
		}

		if (Expression->IsA<UMaterialExpressionMultiply>() ||
			Expression->IsA<UMaterialExpressionAdd>() ||
			Expression->IsA<UMaterialExpressionSubtract>() ||
			Expression->IsA<UMaterialExpressionDivide>() ||
			Expression->IsA<UMaterialExpressionMax>() ||
			Expression->IsA<UMaterialExpressionDotProduct>() ||
			Expression->IsA<UMaterialExpressionAppendVector>())
		{
			Required = { 0, 1 };
		}
		else if (Expression->IsA<UMaterialExpressionLinearInterpolate>())
		{
			Required = { 0, 1, 2 };
		}
		else if (Expression->IsA<UMaterialExpressionPower>())
		{
			Required = { 0, 1 };
		}
		else if (Expression->IsA<UMaterialExpressionOneMinus>() ||
			Expression->IsA<UMaterialExpressionSaturate>() ||
			Expression->IsA<UMaterialExpressionComponentMask>())
		{
			Required = { 0 };
		}

		return Required;
	}

	static TSharedPtr<FJsonObject> BuildMaterialFocusResult(
		UMaterial* Material,
		const FBox2D& Bounds,
		const TArray<int32>& FocusExpressionIndices,
		const TArray<int32>& FocusCommentIndices,
		const FString& FocusSource,
		const float Zoom)
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetBoolField(TEXT("success"), true);
		Result->SetStringField(TEXT("material_path"), Material ? Material->GetPathName() : TEXT(""));
		Result->SetStringField(TEXT("focus_source"), FocusSource);
		Result->SetArrayField(TEXT("focus_expression_indices"), MakeMaterialIntJsonArray(FocusExpressionIndices));
		Result->SetArrayField(TEXT("focus_comment_indices"), MakeMaterialIntJsonArray(FocusCommentIndices));
		Result->SetArrayField(TEXT("focus_region_box"), {
			MakeShareable(new FJsonValueNumber(Bounds.Min.X)),
			MakeShareable(new FJsonValueNumber(Bounds.Min.Y)),
			MakeShareable(new FJsonValueNumber(Bounds.Max.X)),
			MakeShareable(new FJsonValueNumber(Bounds.Max.Y))
		});
		Result->SetNumberField(TEXT("focus_width"), FMath::Max(0.0f, Bounds.Max.X - Bounds.Min.X));
		Result->SetNumberField(TEXT("focus_height"), FMath::Max(0.0f, Bounds.Max.Y - Bounds.Min.Y));
		Result->SetNumberField(TEXT("zoom"), Zoom);
		return Result;
	}
}

bool FVFXToolRegistry::Tool_FocusMaterialGraphRegion(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	TArray<int32> FocusExpressionIndices;
	TArray<int32> FocusCommentIndices;
	FBox2D FocusBounds(EForceInit::ForceInit);
	FString FocusSource;
	ExpandFocusExpressionsFromInput(Material, Input, FocusExpressionIndices, FocusCommentIndices, FocusBounds, FocusSource);
	if (!FocusBounds.bIsValid)
	{
		OutResult = TEXT("Could not resolve a focus region for the material graph.");
		return false;
	}

	float Zoom = 0.0f;
	const FMaterialGraphViewState* ExistingState = GMaterialGraphViewStates.Find(MaterialPath);
	if (!TryReadOptionalFloatField(Input, TEXT("zoom"), Zoom) && ExistingState)
	{
		Zoom = ExistingState->Zoom;
	}

	FMaterialGraphViewState& ViewState = GMaterialGraphViewStates.FindOrAdd(MaterialPath);
	ViewState.FocusRect = FocusBounds;
	ViewState.Zoom = Zoom;
	ViewState.bHasFocusRect = true;

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource, Zoom);
	Result->SetStringField(TEXT("action"), TEXT("focus_material_graph_region"));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_FocusMaterialGraphExpression(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	TSharedPtr<FJsonObject> ForwardInput = MakeShareable(new FJsonObject(*Input));
	if (!ForwardInput->HasField(TEXT("focus_expression_index")) && ForwardInput->HasField(TEXT("expression_index")))
	{
		ForwardInput->SetField(TEXT("focus_expression_index"), ForwardInput->TryGetField(TEXT("expression_index")));
	}

	const bool bSuccess = Tool_FocusMaterialGraphRegion(ForwardInput, OutResult);
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> ResultObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutResult);
		if (FJsonSerializer::Deserialize(Reader, ResultObj) && ResultObj.IsValid())
		{
			ResultObj->SetStringField(TEXT("action"), TEXT("focus_material_graph_expression"));
			OutResult = JsonObjToString(ResultObj);
		}
	}
	return bSuccess;
}

bool FVFXToolRegistry::Tool_FocusMaterialGraphComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	const bool bSuccess = Tool_FocusMaterialGraphRegion(Input, OutResult);
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> ResultObj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutResult);
		if (FJsonSerializer::Deserialize(Reader, ResultObj) && ResultObj.IsValid())
		{
			ResultObj->SetStringField(TEXT("action"), TEXT("focus_material_graph_comment"));
			OutResult = JsonObjToString(ResultObj);
		}
	}
	return bSuccess;
}

bool FVFXToolRegistry::Tool_PanMaterialGraph(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	FMaterialGraphViewState& ViewState = GMaterialGraphViewStates.FindOrAdd(MaterialPath);
	if (!ViewState.bHasFocusRect)
	{
		if (!ComputeFullMaterialGraphBounds(Material, ViewState.FocusRect))
		{
			OutResult = TEXT("Material graph has no valid bounds to pan.");
			return false;
		}
		ViewState.bHasFocusRect = true;
	}

	float DeltaX = 0.0f;
	float DeltaY = 0.0f;
	TryReadOptionalFloatField(Input, TEXT("delta_x"), DeltaX);
	TryReadOptionalFloatField(Input, TEXT("delta_y"), DeltaY);
	ViewState.FocusRect.Min += FVector2D(DeltaX, DeltaY);
	ViewState.FocusRect.Max += FVector2D(DeltaX, DeltaY);

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, ViewState.FocusRect, {}, {}, TEXT("pan_material_graph"), ViewState.Zoom);
	Result->SetStringField(TEXT("action"), TEXT("pan_material_graph"));
	Result->SetNumberField(TEXT("delta_x"), DeltaX);
	Result->SetNumberField(TEXT("delta_y"), DeltaY);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SetMaterialGraphZoom(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	float Zoom = 0.0f;
	if (!TryReadOptionalFloatField(Input, TEXT("zoom"), Zoom))
	{
		OutResult = TEXT("Missing required field: zoom");
		return false;
	}

	FMaterialGraphViewState& ViewState = GMaterialGraphViewStates.FindOrAdd(MaterialPath);
	if (!ViewState.bHasFocusRect)
	{
		ViewState.bHasFocusRect = ComputeFullMaterialGraphBounds(Material, ViewState.FocusRect);
	}
	ViewState.Zoom = FMath::Max(0.0f, Zoom);

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, ViewState.FocusRect, {}, {}, TEXT("set_material_graph_zoom"), ViewState.Zoom);
	Result->SetStringField(TEXT("action"), TEXT("set_material_graph_zoom"));
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CaptureMaterialGraphScreenshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	FString OutputDir;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("output_dir"), OutputDir) || OutputDir.IsEmpty())
	{
		OutputDir = GetDefaultMaterialProbeOutputDir(Material->GetName(), TEXT("material_graph"));
	}
	IFileManager::Get().MakeDirectory(*OutputDir, true);

	FString FileName = TEXT("material_graph.png");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("file_name"), FileName);
	}
	if (!FileName.EndsWith(TEXT(".png"), ESearchCase::IgnoreCase))
	{
		FileName += TEXT(".png");
	}
	const FString OutputPath = FPaths::Combine(OutputDir, FileName);

	int32 ImageWidth = 2200;
	int32 ImageHeight = 1400;
	TryReadOptionalIntField(Input, TEXT("image_width"), ImageWidth);
	TryReadOptionalIntField(Input, TEXT("image_height"), ImageHeight);
	ImageWidth = FMath::Clamp(ImageWidth, 800, 4096);
	ImageHeight = FMath::Clamp(ImageHeight, 600, 4096);

	TArray<int32> FocusExpressionIndices;
	TArray<int32> FocusCommentIndices;
	FString FocusSource;
	FBox2D FocusBounds(EForceInit::ForceInit);
	const FMaterialGraphViewState* ExistingState = GMaterialGraphViewStates.Find(MaterialPath);
	if (!ResolveMaterialFocusBounds(Material, Input, ExistingState, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource))
	{
		OutResult = TEXT("Failed to resolve graph bounds for screenshot.");
		return false;
	}

	float Zoom = ExistingState ? ExistingState->Zoom : 0.0f;
	TryReadOptionalFloatField(Input, TEXT("zoom"), Zoom);
	if (Zoom <= 0.0f)
	{
		const float ViewWidth = FMath::Max(1.0f, FocusBounds.Max.X - FocusBounds.Min.X);
		const float ViewHeight = FMath::Max(1.0f, FocusBounds.Max.Y - FocusBounds.Min.Y);
		Zoom = FMath::Min(static_cast<float>(ImageWidth) / ViewWidth, static_cast<float>(ImageHeight) / ViewHeight);
		Zoom = FMath::Clamp(Zoom, 0.2f, 2.0f);
	}

	TArray<FMaterialGraphVisualNode> Nodes;
	TArray<FMaterialGraphVisualComment> Comments;
	TArray<FMaterialGraphVisualLink> Links;
	CollectMaterialGraphVisualData(Material, Nodes, Comments, Links);

	const TSharedRef<SMaterialGraphCaptureWidget> Widget =
		SNew(SMaterialGraphCaptureWidget)
		.Nodes(Nodes)
		.Comments(Comments)
		.Links(Links)
		.ViewRect(FocusBounds)
		.CaptureSize(FVector2D(static_cast<float>(ImageWidth), static_cast<float>(ImageHeight)))
		.Zoom(Zoom);

	bool bCaptured = CaptureSlateWidgetToPng(Widget, FVector2D(static_cast<float>(ImageWidth), static_cast<float>(ImageHeight)), OutputPath, Error);
	if (!bCaptured)
	{
		FString FallbackError;
		bCaptured = RenderMaterialGraphVisualFallbackToPng(Nodes, Comments, Links, FocusBounds, ImageWidth, ImageHeight, Zoom, OutputPath, FallbackError);
		if (!bCaptured)
		{
			OutResult = Error.IsEmpty() ? FallbackError : FString::Printf(TEXT("%s | Fallback: %s"), *Error, *FallbackError);
			return false;
		}
	}

	FMaterialGraphViewState& ViewState = GMaterialGraphViewStates.FindOrAdd(MaterialPath);
	ViewState.FocusRect = FocusBounds;
	ViewState.Zoom = Zoom;
	ViewState.bHasFocusRect = true;

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource, Zoom);
	Result->SetStringField(TEXT("action"), TEXT("capture_material_graph_screenshot"));
	Result->SetStringField(TEXT("output_path"), OutputPath);
	Result->SetStringField(TEXT("output_dir"), OutputDir);
	Result->SetNumberField(TEXT("image_width"), ImageWidth);
	Result->SetNumberField(TEXT("image_height"), ImageHeight);
	Result->SetNumberField(TEXT("node_count"), Nodes.Num());
	Result->SetNumberField(TEXT("comment_count"), Comments.Num());
	Result->SetNumberField(TEXT("link_count"), Links.Num());
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ValidateMaterialGraphBlock(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	TArray<int32> FocusExpressionIndices;
	TArray<int32> FocusCommentIndices;
	FBox2D FocusBounds(EForceInit::ForceInit);
	FString FocusSource;
	ExpandFocusExpressionsFromInput(Material, Input, FocusExpressionIndices, FocusCommentIndices, FocusBounds, FocusSource);
	if (FocusExpressionIndices.Num() == 0)
	{
		CollectExpressionsWithinBounds(Material, FocusBounds, FocusExpressionIndices);
	}

	TArray<TSharedPtr<FJsonValue>> Errors;
	TArray<TSharedPtr<FJsonValue>> Warnings;

	auto AddIssue = [](TArray<TSharedPtr<FJsonValue>>& Dest, const FString& Severity, const FString& Message, const int32 ExpressionIndex, const FString& ExpressionName, const FString& InputName)
	{
		TSharedPtr<FJsonObject> Issue = MakeShareable(new FJsonObject());
		Issue->SetStringField(TEXT("severity"), Severity);
		Issue->SetStringField(TEXT("message"), Message);
		Issue->SetNumberField(TEXT("expression_index"), ExpressionIndex);
		Issue->SetStringField(TEXT("expression_name"), ExpressionName);
		Issue->SetStringField(TEXT("input_name"), InputName);
		Dest.Add(MakeShareable(new FJsonValueObject(Issue)));
	};

	for (const int32 ExpressionIndex : FocusExpressionIndices)
	{
		if (!Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex))
		{
			continue;
		}

		UMaterialExpression* Expression = Material->GetExpressionCollection().Expressions[ExpressionIndex];
		if (!Expression)
		{
			continue;
		}

		const TArray<int32> RequiredInputs = GetRequiredInputIndicesForExpression(Expression);
		for (const int32 RequiredIndex : RequiredInputs)
		{
			FExpressionInput* RequiredInput = Expression->GetInput(RequiredIndex);
			if (!RequiredInput || RequiredInput->Expression == nullptr)
			{
				const FName InputName = Expression->GetInputName(RequiredIndex);
				AddIssue(
					Errors,
					TEXT("error"),
					FString::Printf(TEXT("Required input is not connected for %s."), *GetMaterialExpressionPracticalName(Expression)),
					ExpressionIndex,
					GetMaterialExpressionPracticalName(Expression),
					InputName.IsNone() ? FString::Printf(TEXT("Input%d"), RequiredIndex) : InputName.ToString());
			}
		}

		if (ParseOptionalBoolField(Input, TEXT("expect_uv_chain"), false) && Expression->IsA<UMaterialExpressionTextureSample>())
		{
			if (FExpressionInput* UVInput = Expression->GetInput(0); UVInput && UVInput->Expression == nullptr)
			{
				AddIssue(Warnings, TEXT("warning"), TEXT("Texture sample relies on default UVs; explicit UV flow was requested."), ExpressionIndex, GetMaterialExpressionPracticalName(Expression), TEXT("UVs"));
			}
		}
	}

	const TArray<TSharedPtr<FJsonValue>>* RequiredRootPins = nullptr;
	if (Input.IsValid() && Input->TryGetArrayField(TEXT("required_root_pins"), RequiredRootPins) && RequiredRootPins)
	{
		for (const TSharedPtr<FJsonValue>& PinValue : *RequiredRootPins)
		{
			if (!PinValue.IsValid())
			{
				continue;
			}

			const FString RootPin = PinValue->AsString();
			FExpressionInput* RootInput = ResolveMaterialRootInput(Material, RootPin);
			if (!RootInput || !RootInput->Expression)
			{
				TSharedPtr<FJsonObject> Issue = MakeShareable(new FJsonObject());
				Issue->SetStringField(TEXT("severity"), TEXT("error"));
				Issue->SetStringField(TEXT("message"), FString::Printf(TEXT("Required root pin '%s' is not connected."), *RootPin));
				Issue->SetStringField(TEXT("root_pin"), RootPin);
				Errors.Add(MakeShareable(new FJsonValueObject(Issue)));
			}
		}
	}

	if (ParseOptionalBoolField(Input, TEXT("require_comment"), false) && FocusExpressionIndices.Num() > 0)
	{
		const TArray<UMaterialExpressionComment*> Comments = GetMaterialComments(Material);
		for (const int32 ExpressionIndex : FocusExpressionIndices)
		{
			UMaterialExpression* Expression = Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex)
				? Material->GetExpressionCollection().Expressions[ExpressionIndex]
				: nullptr;
			if (!Expression)
			{
				continue;
			}

			const float CenterX = Expression->MaterialExpressionEditorX + 130.0f;
			const float CenterY = Expression->MaterialExpressionEditorY + 90.0f;
			bool bCovered = false;
			for (const UMaterialExpressionComment* Comment : Comments)
			{
				if (IsCommentContainingPoint(Comment, CenterX, CenterY))
				{
					bCovered = true;
					break;
				}
			}

			if (!bCovered)
			{
				AddIssue(Warnings, TEXT("warning"), TEXT("Focused node is not wrapped by any comment box."), ExpressionIndex, GetMaterialExpressionPracticalName(Expression), TEXT(""));
			}
		}
	}

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource, 0.0f);
	Result->SetStringField(TEXT("action"), TEXT("validate_material_graph_block"));
	Result->SetBoolField(TEXT("validation_passed"), Errors.Num() == 0);
	Result->SetNumberField(TEXT("scanned_expression_count"), FocusExpressionIndices.Num());
	Result->SetNumberField(TEXT("error_count"), Errors.Num());
	Result->SetNumberField(TEXT("warning_count"), Warnings.Num());
	Result->SetArrayField(TEXT("errors"), Errors);
	Result->SetArrayField(TEXT("warnings"), Warnings);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SummarizeMaterialBlockWiring(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	TArray<int32> FocusExpressionIndices;
	TArray<int32> FocusCommentIndices;
	FBox2D FocusBounds(EForceInit::ForceInit);
	FString FocusSource;
	ExpandFocusExpressionsFromInput(Material, Input, FocusExpressionIndices, FocusCommentIndices, FocusBounds, FocusSource);
	if (FocusExpressionIndices.Num() == 0)
	{
		CollectExpressionsWithinBounds(Material, FocusBounds, FocusExpressionIndices);
	}

	TSet<int32> FocusSet(FocusExpressionIndices);
	TArray<TSharedPtr<FJsonValue>> InternalLinks;
	TArray<TSharedPtr<FJsonValue>> IncomingLinks;
	TArray<TSharedPtr<FJsonValue>> OutgoingLinks;

	for (int32 TargetIndex = 0; TargetIndex < Material->GetExpressionCollection().Expressions.Num(); ++TargetIndex)
	{
		UMaterialExpression* TargetExpression = Material->GetExpressionCollection().Expressions[TargetIndex];
		if (!TargetExpression)
		{
			continue;
		}

		for (FExpressionInputIterator It{ TargetExpression }; It; ++It)
		{
			const int32 InputIndex = It.Index;
			FExpressionInput* ExprInput = It.Input;
			if (!ExprInput || !ExprInput->Expression)
			{
				continue;
			}

			int32 SourceIndex = INDEX_NONE;
			for (int32 CandidateIndex = 0; CandidateIndex < Material->GetExpressionCollection().Expressions.Num(); ++CandidateIndex)
			{
				if (Material->GetExpressionCollection().Expressions[CandidateIndex] == ExprInput->Expression)
				{
					SourceIndex = CandidateIndex;
					break;
				}
			}
			if (SourceIndex == INDEX_NONE)
			{
				continue;
			}

			const bool bSourceInFocus = FocusSet.Contains(SourceIndex);
			const bool bTargetInFocus = FocusSet.Contains(TargetIndex);
			if (!bSourceInFocus && !bTargetInFocus)
			{
				continue;
			}

			TSharedPtr<FJsonObject> LinkObj = MakeShareable(new FJsonObject());
			LinkObj->SetNumberField(TEXT("source_index"), SourceIndex);
			LinkObj->SetNumberField(TEXT("target_index"), TargetIndex);
			LinkObj->SetNumberField(TEXT("target_input_index"), InputIndex);
			const FName InputName = TargetExpression->GetInputName(InputIndex);
			LinkObj->SetStringField(TEXT("target_input_name"), InputName.IsNone() ? FString::Printf(TEXT("Input%d"), InputIndex) : InputName.ToString());

			if (bSourceInFocus && bTargetInFocus)
			{
				InternalLinks.Add(MakeShareable(new FJsonValueObject(LinkObj)));
			}
			else if (!bSourceInFocus && bTargetInFocus)
			{
				IncomingLinks.Add(MakeShareable(new FJsonValueObject(LinkObj)));
			}
			else if (bSourceInFocus && !bTargetInFocus)
			{
				OutgoingLinks.Add(MakeShareable(new FJsonValueObject(LinkObj)));
			}
		}
	}

	TArray<TSharedPtr<FJsonValue>> RootPinsTouched;
	auto AppendRootPin = [&](const FString& PinName, const FExpressionInput& RootInput)
	{
		if (!RootInput.Expression)
		{
			return;
		}

		for (int32 Index = 0; Index < Material->GetExpressionCollection().Expressions.Num(); ++Index)
		{
			if (Material->GetExpressionCollection().Expressions[Index] == RootInput.Expression && FocusSet.Contains(Index))
			{
				RootPinsTouched.Add(MakeShareable(new FJsonValueString(PinName)));
				return;
			}
		}
	};

	if (auto* EditorData = Material->GetEditorOnlyData())
	{
		AppendRootPin(TEXT("BaseColor"), EditorData->BaseColor);
		AppendRootPin(TEXT("Metallic"), EditorData->Metallic);
		AppendRootPin(TEXT("Roughness"), EditorData->Roughness);
		AppendRootPin(TEXT("Normal"), EditorData->Normal);
		AppendRootPin(TEXT("EmissiveColor"), EditorData->EmissiveColor);
		AppendRootPin(TEXT("Opacity"), EditorData->Opacity);
		AppendRootPin(TEXT("OpacityMask"), EditorData->OpacityMask);
		AppendRootPin(TEXT("WorldPositionOffset"), EditorData->WorldPositionOffset);
	}

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource, 0.0f);
	Result->SetStringField(TEXT("action"), TEXT("summarize_material_block_wiring"));
	Result->SetNumberField(TEXT("internal_link_count"), InternalLinks.Num());
	Result->SetNumberField(TEXT("incoming_link_count"), IncomingLinks.Num());
	Result->SetNumberField(TEXT("outgoing_link_count"), OutgoingLinks.Num());
	Result->SetArrayField(TEXT("internal_links"), InternalLinks);
	Result->SetArrayField(TEXT("incoming_links"), IncomingLinks);
	Result->SetArrayField(TEXT("outgoing_links"), OutgoingLinks);
	Result->SetArrayField(TEXT("root_pins_touched"), RootPinsTouched);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CreateMaterialTestInstanceSet(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ParentMaterialPath;
	if (!Input->TryGetStringField(TEXT("parent_material_path"), ParentMaterialPath))
	{
		OutResult = TEXT("Missing required field: parent_material_path");
		return false;
	}

	FString SavePath = TEXT("/Game/ForgePilot_Audit");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	FString NamePrefix = TEXT("MI_MaterialTest");
	Input->TryGetStringField(TEXT("name_prefix"), NamePrefix);
	const TArray<TSharedPtr<FJsonValue>>* VariantArray = nullptr;
	Input->TryGetArrayField(TEXT("variants"), VariantArray);
	FString Preset;
	Input->TryGetStringField(TEXT("preset"), Preset);

	TArray<TSharedPtr<FJsonValue>> CreatedInstances;
	auto BuildVariant = [&](const FString& Suffix, const FString& ScalarOverrides, const FString& VectorOverrides, const FString& TextureOverrides) -> bool
	{
		TSharedPtr<FJsonObject> CreateInput = MakeShareable(new FJsonObject());
		CreateInput->SetStringField(TEXT("instance_name"), FString::Printf(TEXT("%s_%s"), *NamePrefix, *Suffix));
		CreateInput->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
		CreateInput->SetStringField(TEXT("save_path"), SavePath);
		if (!ScalarOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("scalar_overrides"), ScalarOverrides);
		if (!VectorOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("vector_overrides"), VectorOverrides);
		if (!TextureOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("texture_overrides"), TextureOverrides);

		FString CreateResultString;
		if (!Tool_CreateMaterialInstance(CreateInput, CreateResultString))
		{
			return false;
		}

		TSharedPtr<FJsonObject> CreateResult;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CreateResultString);
		if (!FJsonSerializer::Deserialize(Reader, CreateResult) || !CreateResult.IsValid())
		{
			return false;
		}

		CreatedInstances.Add(MakeShareable(new FJsonValueObject(CreateResult)));
		return true;
	};

	if (VariantArray && VariantArray->Num() > 0)
	{
		for (const TSharedPtr<FJsonValue>& VariantValue : *VariantArray)
		{
			const TSharedPtr<FJsonObject> VariantObj = VariantValue.IsValid() ? VariantValue->AsObject() : nullptr;
			if (!VariantObj.IsValid())
			{
				continue;
			}

			FString Suffix = FString::Printf(TEXT("Variant%d"), CreatedInstances.Num());
			VariantObj->TryGetStringField(TEXT("name_suffix"), Suffix);

			FString ScalarOverrides;
			FString VectorOverrides;
			FString TextureOverrides;
			if (VariantObj->HasTypedField<EJson::Object>(TEXT("scalar_overrides")))
			{
				ScalarOverrides = JsonObjToString(VariantObj->GetObjectField(TEXT("scalar_overrides")));
			}
			else
			{
				VariantObj->TryGetStringField(TEXT("scalar_overrides"), ScalarOverrides);
			}
			if (VariantObj->HasTypedField<EJson::Object>(TEXT("vector_overrides")))
			{
				VectorOverrides = JsonObjToString(VariantObj->GetObjectField(TEXT("vector_overrides")));
			}
			else
			{
				VariantObj->TryGetStringField(TEXT("vector_overrides"), VectorOverrides);
			}
			if (VariantObj->HasTypedField<EJson::Object>(TEXT("texture_overrides")))
			{
				TextureOverrides = JsonObjToString(VariantObj->GetObjectField(TEXT("texture_overrides")));
			}
			else
			{
				VariantObj->TryGetStringField(TEXT("texture_overrides"), TextureOverrides);
			}

			if (!BuildVariant(Suffix, ScalarOverrides, VectorOverrides, TextureOverrides))
			{
				OutResult = FString::Printf(TEXT("Failed to create material instance variant '%s'."), *Suffix);
				return false;
			}
		}
	}
	else
	{
		const FString PresetLower = Preset.ToLower();
		if (PresetLower == TEXT("fire_person_erosion"))
		{
			if (!BuildVariant(TEXT("Low"), TEXT("{\"Burn_Threshold\":0.25,\"Glow_Intensity\":15.0,\"InnerFlame_Intensity\":3.0}"), TEXT(""), TEXT("")) ||
				!BuildVariant(TEXT("Mid"), TEXT("{\"Burn_Threshold\":0.45,\"Glow_Intensity\":35.0,\"InnerFlame_Intensity\":6.0}"), TEXT(""), TEXT("")) ||
				!BuildVariant(TEXT("Hot"), TEXT("{\"Burn_Threshold\":0.60,\"Glow_Intensity\":70.0,\"InnerFlame_Intensity\":10.0}"), TEXT(""), TEXT("")))
			{
				OutResult = TEXT("Failed to create preset material instance set.");
				return false;
			}
		}
		else if (!BuildVariant(TEXT("Base"), TEXT(""), TEXT(""), TEXT("")))
		{
			OutResult = TEXT("Failed to create base test material instance.");
			return false;
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("create_material_test_instance_set"));
	Result->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
	Result->SetStringField(TEXT("save_path"), SavePath);
	Result->SetNumberField(TEXT("count"), CreatedInstances.Num());
	Result->SetArrayField(TEXT("instances"), CreatedInstances);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_SweepMaterialInstanceParameters(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ParentMaterialPath;
	if (!Input->TryGetStringField(TEXT("parent_material_path"), ParentMaterialPath))
	{
		OutResult = TEXT("Missing required field: parent_material_path");
		return false;
	}

	FString SavePath = TEXT("/Game/ForgePilot_Audit");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	FString NamePrefix = TEXT("MI_Sweep");
	Input->TryGetStringField(TEXT("name_prefix"), NamePrefix);

	FString BaseScalarOverrides;
	FString BaseVectorOverrides;
	FString BaseTextureOverrides;
	Input->TryGetStringField(TEXT("base_scalar_overrides"), BaseScalarOverrides);
	Input->TryGetStringField(TEXT("base_vector_overrides"), BaseVectorOverrides);
	Input->TryGetStringField(TEXT("base_texture_overrides"), BaseTextureOverrides);

	TArray<TSharedPtr<FJsonValue>> Instances;
	auto CreateVariant = [&](const FString& Suffix, const FString& ScalarOverrides, const FString& VectorOverrides, const FString& TextureOverrides) -> bool
	{
		TSharedPtr<FJsonObject> CreateInput = MakeShareable(new FJsonObject());
		CreateInput->SetStringField(TEXT("instance_name"), FString::Printf(TEXT("%s_%s"), *NamePrefix, *Suffix));
		CreateInput->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
		CreateInput->SetStringField(TEXT("save_path"), SavePath);
		if (!ScalarOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("scalar_overrides"), ScalarOverrides);
		if (!VectorOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("vector_overrides"), VectorOverrides);
		if (!TextureOverrides.IsEmpty()) CreateInput->SetStringField(TEXT("texture_overrides"), TextureOverrides);

		FString CreateResultString;
		if (!Tool_CreateMaterialInstance(CreateInput, CreateResultString))
		{
			return false;
		}

		TSharedPtr<FJsonObject> CreateResult;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CreateResultString);
		if (!FJsonSerializer::Deserialize(Reader, CreateResult) || !CreateResult.IsValid())
		{
			return false;
		}

		Instances.Add(MakeShareable(new FJsonValueObject(CreateResult)));
		return true;
	};

	auto MergeOverrideJson = [](const FString& BaseJson, const FString& ParamName, const TSharedPtr<FJsonValue>& OverrideValue) -> FString
	{
		TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject());
		if (!BaseJson.IsEmpty())
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(BaseJson);
			FJsonSerializer::Deserialize(Reader, Root);
			if (!Root.IsValid())
			{
				Root = MakeShareable(new FJsonObject());
			}
		}
		Root->SetField(ParamName, OverrideValue);
		return JsonObjToString(Root);
	};

	FString ScalarSweepsJson;
	Input->TryGetStringField(TEXT("scalar_sweeps"), ScalarSweepsJson);
	if (!ScalarSweepsJson.IsEmpty())
	{
		TSharedPtr<FJsonObject> ScalarSweeps;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ScalarSweepsJson);
		if (FJsonSerializer::Deserialize(Reader, ScalarSweeps) && ScalarSweeps.IsValid())
		{
			for (const auto& Pair : ScalarSweeps->Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
				if (Pair.Value.IsValid() && Pair.Value->TryGetArray(Values) && Values)
				{
					for (int32 ValueIndex = 0; ValueIndex < Values->Num(); ++ValueIndex)
					{
						if (!CreateVariant(FString::Printf(TEXT("%s_%d"), *Pair.Key, ValueIndex), MergeOverrideJson(BaseScalarOverrides, Pair.Key, (*Values)[ValueIndex]), BaseVectorOverrides, BaseTextureOverrides))
						{
							OutResult = FString::Printf(TEXT("Failed to create scalar sweep variant for '%s'."), *Pair.Key);
							return false;
						}
					}
				}
			}
		}
	}

	FString VectorSweepsJson;
	Input->TryGetStringField(TEXT("vector_sweeps"), VectorSweepsJson);
	if (!VectorSweepsJson.IsEmpty())
	{
		TSharedPtr<FJsonObject> VectorSweeps;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(VectorSweepsJson);
		if (FJsonSerializer::Deserialize(Reader, VectorSweeps) && VectorSweeps.IsValid())
		{
			for (const auto& Pair : VectorSweeps->Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
				if (Pair.Value.IsValid() && Pair.Value->TryGetArray(Values) && Values)
				{
					for (int32 ValueIndex = 0; ValueIndex < Values->Num(); ++ValueIndex)
					{
						if (!CreateVariant(FString::Printf(TEXT("%s_%d"), *Pair.Key, ValueIndex), BaseScalarOverrides, MergeOverrideJson(BaseVectorOverrides, Pair.Key, (*Values)[ValueIndex]), BaseTextureOverrides))
						{
							OutResult = FString::Printf(TEXT("Failed to create vector sweep variant for '%s'."), *Pair.Key);
							return false;
						}
					}
				}
			}
		}
	}

	FString TextureSweepsJson;
	Input->TryGetStringField(TEXT("texture_sweeps"), TextureSweepsJson);
	if (!TextureSweepsJson.IsEmpty())
	{
		TSharedPtr<FJsonObject> TextureSweeps;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextureSweepsJson);
		if (FJsonSerializer::Deserialize(Reader, TextureSweeps) && TextureSweeps.IsValid())
		{
			for (const auto& Pair : TextureSweeps->Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* Values = nullptr;
				if (Pair.Value.IsValid() && Pair.Value->TryGetArray(Values) && Values)
				{
					for (int32 ValueIndex = 0; ValueIndex < Values->Num(); ++ValueIndex)
					{
						if (!CreateVariant(FString::Printf(TEXT("%s_%d"), *Pair.Key, ValueIndex), BaseScalarOverrides, BaseVectorOverrides, MergeOverrideJson(BaseTextureOverrides, Pair.Key, (*Values)[ValueIndex])))
						{
							OutResult = FString::Printf(TEXT("Failed to create texture sweep variant for '%s'."), *Pair.Key);
							return false;
						}
					}
				}
			}
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("sweep_material_instance_parameters"));
	Result->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
	Result->SetStringField(TEXT("save_path"), SavePath);
	Result->SetNumberField(TEXT("count"), Instances.Num());
	Result->SetArrayField(TEXT("instances"), Instances);
	OutResult = JsonObjToString(Result);
	return true;
}

namespace
{
	static bool LoadImagePixelsForCompare(const FString& ImagePath, TArray<FColor>& OutPixels, int32& OutWidth, int32& OutHeight, FString& OutError)
	{
		OutPixels.Reset();
		OutWidth = 0;
		OutHeight = 0;
		OutError.Reset();

		TArray<uint8> FileData;
		if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
		{
			OutError = FString::Printf(TEXT("Failed to read image file: %s"), *ImagePath);
			return false;
		}

		auto TryLoadWithFormat = [&](const EImageFormat Format) -> bool
		{
			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);
			if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
			{
				return false;
			}

			TArray64<uint8> RawBytes;
			if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawBytes) || RawBytes.Num() == 0)
			{
				return false;
			}

			OutWidth = ImageWrapper->GetWidth();
			OutHeight = ImageWrapper->GetHeight();
			OutPixels.SetNum(OutWidth * OutHeight);
			FMemory::Memcpy(OutPixels.GetData(), RawBytes.GetData(), FMath::Min(static_cast<int32>(RawBytes.Num()), OutPixels.Num() * static_cast<int32>(sizeof(FColor))));
			return true;
		};

		const FString Extension = FPaths::GetExtension(ImagePath).ToLower();
		if ((Extension == TEXT("png") && TryLoadWithFormat(EImageFormat::PNG)) ||
			((Extension == TEXT("jpg") || Extension == TEXT("jpeg")) && TryLoadWithFormat(EImageFormat::JPEG)) ||
			TryLoadWithFormat(EImageFormat::PNG) ||
			TryLoadWithFormat(EImageFormat::JPEG))
		{
			return true;
		}

		OutError = FString::Printf(TEXT("Unsupported or unreadable image format: %s"), *ImagePath);
		return false;
	}

	static FLevelEditorViewportClient* GetBestLevelViewportClientForMaterialTools()
	{
		auto ScoreViewportClient = [](FLevelEditorViewportClient* ViewportClient) -> int32
		{
			if (!ViewportClient || ViewportClient->Viewport == nullptr)
			{
				return MIN_int32;
			}

			int32 Score = 0;
			if (ViewportClient == GCurrentLevelEditingViewportClient)
			{
				Score += 1000;
			}
			if (ViewportClient == GLastKeyLevelEditingViewportClient)
			{
				Score += 500;
			}
			if (ViewportClient->IsPerspective())
			{
				Score += 200;
			}
			if (ViewportClient->IsRealtime())
			{
				Score += 50;
			}

			return Score;
		};

		FLevelEditorViewportClient* BestClient = nullptr;
		int32 BestScore = MIN_int32;

		if (GCurrentLevelEditingViewportClient)
		{
			BestClient = GCurrentLevelEditingViewportClient;
			BestScore = ScoreViewportClient(BestClient);
		}

		if (GLastKeyLevelEditingViewportClient)
		{
			const int32 LastKeyScore = ScoreViewportClient(GLastKeyLevelEditingViewportClient);
			if (LastKeyScore > BestScore)
			{
				BestClient = GLastKeyLevelEditingViewportClient;
				BestScore = LastKeyScore;
			}
		}

		if (GEditor)
		{
			for (FLevelEditorViewportClient* ViewportClient : GEditor->GetLevelViewportClients())
			{
				const int32 Score = ScoreViewportClient(ViewportClient);
				if (Score > BestScore)
				{
					BestClient = ViewportClient;
					BestScore = Score;
				}
			}
		}

		return BestClient;
	}
}

bool FVFXToolRegistry::Tool_SpawnMaterialProbeActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	FString ActorLabel = TEXT("ForgePilot_MaterialProbe");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("actor_label"), ActorLabel);
	}

	FVector Location(0.0f, 0.0f, 120.0f);
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale(1.0f, 1.0f, 1.0f);
	const TArray<TSharedPtr<FJsonValue>>* ValueArray = nullptr;
	if (Input.IsValid() && Input->TryGetArrayField(TEXT("location"), ValueArray) && ValueArray && ValueArray->Num() >= 3)
	{
		Location.X = static_cast<float>((*ValueArray)[0]->AsNumber());
		Location.Y = static_cast<float>((*ValueArray)[1]->AsNumber());
		Location.Z = static_cast<float>((*ValueArray)[2]->AsNumber());
	}
	if (Input.IsValid() && Input->TryGetArrayField(TEXT("rotation"), ValueArray) && ValueArray && ValueArray->Num() >= 3)
	{
		Rotation.Pitch = static_cast<float>((*ValueArray)[0]->AsNumber());
		Rotation.Yaw = static_cast<float>((*ValueArray)[1]->AsNumber());
		Rotation.Roll = static_cast<float>((*ValueArray)[2]->AsNumber());
	}
	if (Input.IsValid() && Input->TryGetArrayField(TEXT("scale"), ValueArray) && ValueArray && ValueArray->Num() >= 3)
	{
		Scale.X = static_cast<float>((*ValueArray)[0]->AsNumber());
		Scale.Y = static_cast<float>((*ValueArray)[1]->AsNumber());
		Scale.Z = static_cast<float>((*ValueArray)[2]->AsNumber());
	}

	FString MeshPath = TEXT("/Engine/BasicShapes/Sphere.Sphere");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("mesh_path"), MeshPath);
	}

	UStaticMesh* Mesh = Cast<UStaticMesh>(UEditorAssetLibrary::LoadAsset(MeshPath));
	if (!Mesh)
	{
		OutResult = FString::Printf(TEXT("Could not load StaticMesh at path: %s"), *MeshPath);
		return false;
	}

	AStaticMeshActor* ProbeActor = World->SpawnActor<AStaticMeshActor>(Location, Rotation);
	if (!ProbeActor)
	{
		OutResult = TEXT("Failed to spawn material probe actor.");
		return false;
	}

	ProbeActor->SetActorScale3D(Scale);
	ProbeActor->SetActorLabel(ActorLabel);
	if (UStaticMeshComponent* MeshComp = ProbeActor->GetStaticMeshComponent())
	{
		MeshComp->SetStaticMesh(Mesh);
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("spawn_material_probe_actor"));
	Result->SetStringField(TEXT("actor_label"), ProbeActor->GetActorLabel());
	Result->SetStringField(TEXT("actor_path"), ProbeActor->GetPathName());
	Result->SetStringField(TEXT("mesh_path"), MeshPath);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ApplyMaterialToProbeActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	return Tool_SetActorMaterial(Input, OutResult);
}

bool FVFXToolRegistry::Tool_FrameProbeActorForCapture(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ActorLabel;
	if (!Input->TryGetStringField(TEXT("actor_label"), ActorLabel))
	{
		OutResult = TEXT("Missing required field: actor_label");
		return false;
	}
	const bool bEnsureRealtime = ParseOptionalBoolField(Input, TEXT("ensure_realtime"), true);
	int32 WaitMs = 100;
	TryReadOptionalIntField(Input, TEXT("wait_ms"), WaitMs);
	WaitMs = FMath::Clamp(WaitMs, 0, 5000);

	UWorld* World = GetForgePilotEditorWorld();
	if (!World)
	{
		OutResult = TEXT("No editor world available.");
		return false;
	}

	AActor* ProbeActor = FindActorByLabelForMaterialTools(World, ActorLabel);
	if (!ProbeActor)
	{
		OutResult = FString::Printf(TEXT("Probe actor '%s' not found."), *ActorLabel);
		return false;
	}

	const FBox Bounds = ProbeActor->GetComponentsBoundingBox(true);

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("frame_probe_actor_for_capture"));
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetArrayField(TEXT("bounds_center"), {
		MakeShareable(new FJsonValueNumber(Bounds.GetCenter().X)),
		MakeShareable(new FJsonValueNumber(Bounds.GetCenter().Y)),
		MakeShareable(new FJsonValueNumber(Bounds.GetCenter().Z))
	});
	Result->SetArrayField(TEXT("bounds_extent"), {
		MakeShareable(new FJsonValueNumber(Bounds.GetExtent().X)),
		MakeShareable(new FJsonValueNumber(Bounds.GetExtent().Y)),
		MakeShareable(new FJsonValueNumber(Bounds.GetExtent().Z))
	});

	FLevelEditorViewportClient* ViewportClient = nullptr;
	if (FModuleManager::Get().IsModuleLoaded(TEXT("LevelEditor")))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
		if (TSharedPtr<SLevelViewport> ActiveViewport = LevelEditorModule.GetFirstActiveLevelViewport())
		{
			ViewportClient = &ActiveViewport->GetLevelViewportClient();
		}
	}
	if (!ViewportClient)
	{
		ViewportClient = GetBestLevelViewportClientForMaterialTools();
	}

	if (ViewportClient)
	{
		if (!ViewportClient->IsPerspective())
		{
			ViewportClient->SetViewportType(LVT_Perspective);
		}
		if (bEnsureRealtime)
		{
			ViewportClient->SetRealtime(true);
		}
		ViewportClient->FocusViewportOnBox(Bounds, true);
		if (GEditor)
		{
			GEditor->RedrawLevelEditingViewports(true);
		}
		if (WaitMs > 0)
		{
			FPlatformProcess::Sleep(static_cast<float>(WaitMs) / 1000.0f);
		}
		FlushRenderingCommands();
		Result->SetBoolField(TEXT("framed_viewport"), true);
		Result->SetBoolField(TEXT("perspective_viewport"), ViewportClient->IsPerspective());
		Result->SetBoolField(TEXT("realtime_viewport"), ViewportClient->IsRealtime());
	}
	else
	{
		Result->SetBoolField(TEXT("framed_viewport"), false);
		Result->SetStringField(TEXT("message"), TEXT("No level viewport available; skipped viewport framing."));
	}
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CaptureMaterialVisualProbe(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString ActorLabel;
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("actor_label"), ActorLabel);
	}

	const bool bFrameActor = ParseOptionalBoolField(Input, TEXT("frame_actor"), !ActorLabel.IsEmpty());
	const bool bForceThumbnailFallback = ParseOptionalBoolField(Input, TEXT("force_thumbnail_fallback"), !ActorLabel.IsEmpty());
	const bool bPreferEditorPreview = ParseOptionalBoolField(Input, TEXT("prefer_editor_preview"), !ActorLabel.IsEmpty());
	int32 WaitMs = 150;
	TryReadOptionalIntField(Input, TEXT("wait_ms"), WaitMs);
	WaitMs = FMath::Max(0, WaitMs);
	if ((bPreferEditorPreview || bForceThumbnailFallback) && !ActorLabel.IsEmpty())
	{
		WaitMs = FMath::Max(WaitMs, 2000);
	}

	if (bFrameActor && !ActorLabel.IsEmpty())
	{
		TSharedPtr<FJsonObject> FrameInput = MakeShareable(new FJsonObject());
		FrameInput->SetStringField(TEXT("actor_label"), ActorLabel);
		FString FrameResult;
		if (!Tool_FrameProbeActorForCapture(FrameInput, FrameResult))
		{
			OutResult = FrameResult;
			return false;
		}
	}

	if (WaitMs > 0)
	{
		FPlatformProcess::Sleep(static_cast<float>(WaitMs) / 1000.0f);
	}

	if (GEditor)
	{
		GEditor->RedrawLevelEditingViewports(true);
	}
	FlushRenderingCommands();

	FString OutputDir;
	if (!Input.IsValid() || !Input->TryGetStringField(TEXT("output_dir"), OutputDir) || OutputDir.IsEmpty())
	{
		OutputDir = GetDefaultMaterialProbeOutputDir(ActorLabel.IsEmpty() ? TEXT("material_probe") : ActorLabel, TEXT("material_visual"));
	}
	IFileManager::Get().MakeDirectory(*OutputDir, true);

	FString FileName = TEXT("material_probe.jpg");
	if (Input.IsValid())
	{
		Input->TryGetStringField(TEXT("file_name"), FileName);
	}
	if (!FileName.EndsWith(TEXT(".jpg"), ESearchCase::IgnoreCase) &&
		!FileName.EndsWith(TEXT(".jpeg"), ESearchCase::IgnoreCase) &&
		!FileName.EndsWith(TEXT(".png"), ESearchCase::IgnoreCase))
	{
		FileName += TEXT(".jpg");
	}
	const FString OutputPath = FPaths::Combine(OutputDir, FileName);

	bool bCaptured = false;
	FString CaptureMode = TEXT("viewport");

	UWorld* World = GetForgePilotEditorWorld();
	AActor* FoundActor = nullptr;
	if (!ActorLabel.IsEmpty())
	{
		if (!World)
		{
			OutResult = TEXT("No editor world available for material visual probe capture.");
			return false;
		}

		FoundActor = FindActorByLabelForMaterialTools(World, ActorLabel);
		if (!FoundActor)
		{
			OutResult = FString::Printf(TEXT("Actor with label '%s' not found for visual probe capture."), *ActorLabel);
			return false;
		}

		TArray<UPrimitiveComponent*> PrimCompsToRefresh;
		FoundActor->GetComponents<UPrimitiveComponent>(PrimCompsToRefresh);
		for (UPrimitiveComponent* PrimComp : PrimCompsToRefresh)
		{
			if (!PrimComp)
			{
				continue;
			}

			PrimComp->MarkRenderStateDirty();
			PrimComp->RecreateRenderState_Concurrent();
		}
	}

	if (!bForceThumbnailFallback && bPreferEditorPreview && FoundActor)
	{
		UPrimitiveComponent* PrimComp = nullptr;
		TArray<UPrimitiveComponent*> PrimComps;
		FoundActor->GetComponents<UPrimitiveComponent>(PrimComps);
		if (PrimComps.Num() > 0)
		{
			PrimComp = PrimComps[0];
		}

		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(PrimComp))
		{
			if (UStaticMesh* StaticMesh = StaticMeshComp->GetStaticMesh())
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				const int32 MaterialCount = FMath::Max(StaticMeshComp->GetNumMaterials(), 1);
				OverrideMaterials.Reserve(MaterialCount);
				for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
				{
					OverrideMaterials.Add(StaticMeshComp->GetMaterial(MaterialIndex));
				}

				TArray<FColor> MeshPreviewBitmap;
				int32 MeshPreviewWidth = 0;
				int32 MeshPreviewHeight = 0;
				FString MeshPreviewError;
				FString MeshPreviewSource;
				if (RenderStaticMeshMaterialPreviewBitmap(
					StaticMesh,
					OverrideMaterials,
					512,
					MeshPreviewBitmap,
					MeshPreviewWidth,
					MeshPreviewHeight,
					MeshPreviewError,
					MeshPreviewSource))
				{
					bool bMeshPreviewUsable = !IsMaterialProbeCaptureMostlyBlack(MeshPreviewBitmap);
					if (!bMeshPreviewUsable)
					{
						if (GEditor)
						{
							GEditor->RedrawLevelEditingViewports(true);
						}
						FlushRenderingCommands();
						FPlatformProcess::Sleep(0.10f);

						MeshPreviewBitmap.Reset();
						MeshPreviewWidth = 0;
						MeshPreviewHeight = 0;
						MeshPreviewError.Reset();
						MeshPreviewSource.Reset();
						if (RenderStaticMeshMaterialPreviewBitmap(
							StaticMesh,
							OverrideMaterials,
							512,
							MeshPreviewBitmap,
							MeshPreviewWidth,
							MeshPreviewHeight,
							MeshPreviewError,
							MeshPreviewSource))
						{
							bMeshPreviewUsable = !IsMaterialProbeCaptureMostlyBlack(MeshPreviewBitmap);
						}
					}

					if (bMeshPreviewUsable)
					{
						if (!SaveColorBufferToImageFile(MeshPreviewBitmap, MeshPreviewWidth, MeshPreviewHeight, OutputPath, OutResult, 92))
						{
							return false;
						}
						bCaptured = true;
						CaptureMode = TEXT("editor_preview_thumbnail");
					}
				}
			}
		}
	}

	if (!bCaptured && !bForceThumbnailFallback && FoundActor)
	{
		const FBox Bounds = FoundActor->GetComponentsBoundingBox(true);
		if (Bounds.IsValid)
		{
			const FVector Center = Bounds.GetCenter();
			const FVector Extent = Bounds.GetExtent();
			const float Radius = FMath::Max(Extent.Size(), 50.0f);
			const float CaptureFOV = 28.0f;
			const float HalfFOVRadians = FMath::DegreesToRadians(CaptureFOV * 0.5f);
			const FVector ViewDirection = FVector(-1.0f, -0.8f, 0.42f).GetSafeNormal();
			const float Distance = (Radius / FMath::Tan(HalfFOVRadians)) + Radius * 0.75f;
			const FVector CameraLocation = Center - (ViewDirection * Distance);
			const FRotator CameraRotation = (Center - CameraLocation).Rotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.ObjectFlags |= RF_Transient;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASceneCapture2D* SceneCaptureActor = World->SpawnActor<ASceneCapture2D>(CameraLocation, CameraRotation, SpawnParams);
			if (SceneCaptureActor)
			{
				USceneCaptureComponent2D* CaptureComponent = SceneCaptureActor->GetCaptureComponent2D();
				if (CaptureComponent)
				{
					UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(GetTransientPackage(), NAME_None, RF_Transient);
					if (RenderTarget)
					{
						RenderTarget->ClearColor = FLinearColor(0.20f, 0.22f, 0.25f, 1.0f);
						RenderTarget->TargetGamma = 2.2f;
						RenderTarget->InitCustomFormat(1024, 1024, PF_B8G8R8A8, false);
						RenderTarget->UpdateResourceImmediate(true);

						CaptureComponent->TextureTarget = RenderTarget;
						CaptureComponent->FOVAngle = CaptureFOV;
						CaptureComponent->bCaptureEveryFrame = false;
						CaptureComponent->bCaptureOnMovement = false;
						CaptureComponent->bAlwaysPersistRenderingState = true;
						CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
						CaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
						CaptureComponent->ShowOnlyActors.Reset();
						CaptureComponent->ShowOnlyComponents.Reset();
						CaptureComponent->ShowOnlyActors.Add(FoundActor);
						CaptureComponent->ShowOnlyActorComponents(FoundActor, true);
						CaptureComponent->ShowFlags.SetTemporalAA(true);
						for (int32 WarmupIndex = 0; WarmupIndex < 3; ++WarmupIndex)
						{
							CaptureComponent->CaptureScene();
						}
						FlushRenderingCommands();

						if (FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource())
						{
							TArray<FColor> Bitmap;
							if (RenderTargetResource->ReadPixels(Bitmap) && Bitmap.Num() > 0)
							{
								if (!IsMaterialProbeCaptureMostlyBlack(Bitmap))
								{
									const int32 ImageWidth = RenderTarget->SizeX;
									const int32 ImageHeight = RenderTarget->SizeY;
									FString SaveError;
									if (SaveColorBufferToImageFile(Bitmap, ImageWidth, ImageHeight, OutputPath, SaveError, 92))
									{
										bCaptured = true;
										CaptureMode = TEXT("scene_capture");
									}
									else
									{
										OutResult = SaveError;
									}
								}
							}
						}
					}
				}

				SceneCaptureActor->Destroy();
			}
		}
	}

	if (!bForceThumbnailFallback)
	{
		if (!bCaptured && ActorLabel.IsEmpty())
		{
			if (FLevelEditorViewportClient* ViewportClient = GetBestLevelViewportClientForMaterialTools())
			{
				if (!ViewportClient->IsPerspective())
				{
					ViewportClient->SetViewportType(LVT_Perspective);
				}
				ViewportClient->SetRealtime(true);
				if (FViewport* Viewport = ViewportClient->Viewport)
				{
					TArray<FColor> Bitmap;
					const FIntPoint ViewportSize = Viewport->GetSizeXY();
					if (ViewportSize.X > 0 && ViewportSize.Y > 0 && Viewport->ReadPixels(Bitmap) && Bitmap.Num() > 0)
					{
						FString SaveError;
						if (SaveColorBufferToImageFile(Bitmap, ViewportSize.X, ViewportSize.Y, OutputPath, SaveError, 90))
						{
							bCaptured = true;
							CaptureMode = TEXT("viewport");
						}
						else
						{
							OutResult = SaveError;
						}
					}
				}
			}
		}
	}

	if (!bCaptured)
	{
		CaptureMode = bForceThumbnailFallback ? TEXT("thumbnail_forced") : TEXT("thumbnail_fallback");
		if (!FoundActor)
		{
			OutResult = FString::Printf(TEXT("Actor with label '%s' not found for visual probe capture."), *ActorLabel);
			return false;
		}

		UPrimitiveComponent* PrimComp = nullptr;
		TArray<UPrimitiveComponent*> PrimComps;
		FoundActor->GetComponents<UPrimitiveComponent>(PrimComps);
		if (PrimComps.Num() > 0)
		{
			PrimComp = PrimComps[0];
		}
		if (!PrimComp)
		{
			OutResult = FString::Printf(TEXT("Actor '%s' has no primitive component for thumbnail fallback."), *ActorLabel);
			return false;
		}

		UMaterialInterface* MaterialInterface = PrimComp->GetMaterial(0);
		if (!MaterialInterface)
		{
			OutResult = FString::Printf(TEXT("Actor '%s' has no material on slot 0 for thumbnail fallback."), *ActorLabel);
			return false;
		}

		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(PrimComp))
		{
			if (UStaticMesh* StaticMesh = StaticMeshComp->GetStaticMesh())
			{
				TArray<UMaterialInterface*> OverrideMaterials;
				const int32 MaterialCount = FMath::Max(StaticMeshComp->GetNumMaterials(), 1);
				OverrideMaterials.Reserve(MaterialCount);
				for (int32 MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex)
				{
					OverrideMaterials.Add(StaticMeshComp->GetMaterial(MaterialIndex));
				}

				TArray<FColor> MeshPreviewBitmap;
				int32 MeshPreviewWidth = 0;
				int32 MeshPreviewHeight = 0;
				FString MeshPreviewError;
				FString MeshPreviewSource;
				bool bMeshPreviewReady = false;
				for (int32 AttemptIndex = 0; AttemptIndex < 3 && !bMeshPreviewReady; ++AttemptIndex)
				{
					MeshPreviewBitmap.Reset();
					MeshPreviewWidth = 0;
					MeshPreviewHeight = 0;
					MeshPreviewError.Reset();
					MeshPreviewSource.Reset();

					if (RenderStaticMeshMaterialPreviewBitmap(
						StaticMesh,
						OverrideMaterials,
						512,
						MeshPreviewBitmap,
						MeshPreviewWidth,
						MeshPreviewHeight,
						MeshPreviewError,
						MeshPreviewSource))
					{
						bMeshPreviewReady = !IsMaterialProbeCaptureMostlyBlack(MeshPreviewBitmap);
						if (!bMeshPreviewReady && AttemptIndex < 2)
						{
							if (GEditor)
							{
								GEditor->RedrawLevelEditingViewports(true);
							}
							FlushRenderingCommands();
							FPlatformProcess::Sleep(0.75f);
						}
					}
				}

				if (bMeshPreviewReady)
				{
					if (!SaveColorBufferToImageFile(MeshPreviewBitmap, MeshPreviewWidth, MeshPreviewHeight, OutputPath, OutResult, 92))
					{
						return false;
					}
					CaptureMode = TEXT("mesh_thumbnail_fallback");
					bCaptured = true;
				}
			}
		}

		if (bCaptured)
		{
			TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
			Result->SetBoolField(TEXT("success"), true);
			Result->SetStringField(TEXT("action"), TEXT("capture_material_visual_probe"));
			Result->SetStringField(TEXT("actor_label"), ActorLabel);
			Result->SetStringField(TEXT("output_path"), OutputPath);
			Result->SetStringField(TEXT("output_dir"), OutputDir);
			Result->SetBoolField(TEXT("frame_actor"), bFrameActor);
			Result->SetStringField(TEXT("capture_mode"), CaptureMode);
			OutResult = JsonObjToString(Result);
			return true;
		}

		FObjectThumbnail Thumbnail;
		ThumbnailTools::RenderThumbnail(MaterialInterface, 512, 512, ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush, nullptr, &Thumbnail);
		const TArray<uint8>& ThumbnailData = Thumbnail.GetUncompressedImageData();
		if (ThumbnailData.Num() == 0)
		{
			OutResult = TEXT("Thumbnail fallback produced no image data.");
			return false;
		}

		TArray<FColor> ThumbnailBitmap;
		ThumbnailBitmap.SetNum(Thumbnail.GetImageWidth() * Thumbnail.GetImageHeight());
		FMemory::Memcpy(ThumbnailBitmap.GetData(), ThumbnailData.GetData(), ThumbnailData.Num());
		if (!SaveColorBufferToImageFile(ThumbnailBitmap, Thumbnail.GetImageWidth(), Thumbnail.GetImageHeight(), OutputPath, OutResult, 92))
		{
			return false;
		}
		bCaptured = true;
	}

	if (!bCaptured)
	{
		OutResult = FString::Printf(TEXT("Failed to save visual probe image: %s"), *OutputPath);
		return false;
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("capture_material_visual_probe"));
	Result->SetStringField(TEXT("actor_label"), ActorLabel);
	Result->SetStringField(TEXT("output_path"), OutputPath);
	Result->SetStringField(TEXT("output_dir"), OutputDir);
	Result->SetBoolField(TEXT("frame_actor"), bFrameActor);
	Result->SetStringField(TEXT("capture_mode"), CaptureMode);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_CompareMaterialProbeCaptures(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		OutResult = TEXT("Missing input.");
		return false;
	}

	FString ImageAPath;
	FString ImageBPath;
	if (!Input->TryGetStringField(TEXT("image_a_path"), ImageAPath) ||
		!Input->TryGetStringField(TEXT("image_b_path"), ImageBPath))
	{
		OutResult = TEXT("Missing required fields: image_a_path, image_b_path");
		return false;
	}

	TArray<FColor> PixelsA;
	TArray<FColor> PixelsB;
	int32 WidthA = 0;
	int32 HeightA = 0;
	int32 WidthB = 0;
	int32 HeightB = 0;
	FString Error;
	if (!LoadImagePixelsForCompare(ImageAPath, PixelsA, WidthA, HeightA, Error))
	{
		OutResult = Error;
		return false;
	}
	if (!LoadImagePixelsForCompare(ImageBPath, PixelsB, WidthB, HeightB, Error))
	{
		OutResult = Error;
		return false;
	}

	const int32 CompareWidth = FMath::Min(WidthA, WidthB);
	const int32 CompareHeight = FMath::Min(HeightA, HeightB);
	if (CompareWidth <= 0 || CompareHeight <= 0)
	{
		OutResult = TEXT("Images have invalid dimensions.");
		return false;
	}

	double TotalDifference = 0.0;
	int32 ChangedPixels = 0;
	for (int32 Y = 0; Y < CompareHeight; ++Y)
	{
		for (int32 X = 0; X < CompareWidth; ++X)
		{
			const FColor& PixelA = PixelsA[Y * WidthA + X];
			const FColor& PixelB = PixelsB[Y * WidthB + X];
			const int32 Diff =
				FMath::Abs(static_cast<int32>(PixelA.R) - static_cast<int32>(PixelB.R)) +
				FMath::Abs(static_cast<int32>(PixelA.G) - static_cast<int32>(PixelB.G)) +
				FMath::Abs(static_cast<int32>(PixelA.B) - static_cast<int32>(PixelB.B)) +
				FMath::Abs(static_cast<int32>(PixelA.A) - static_cast<int32>(PixelB.A));
			TotalDifference += static_cast<double>(Diff) / 4.0;
			if (Diff > 16)
			{
				++ChangedPixels;
			}
		}
	}

	const double PixelCount = static_cast<double>(CompareWidth) * static_cast<double>(CompareHeight);
	const double MeanAbsoluteDifference = PixelCount > 0.0 ? (TotalDifference / PixelCount) : 0.0;
	const double ChangedRatio = PixelCount > 0.0 ? (static_cast<double>(ChangedPixels) / PixelCount) : 0.0;

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"), TEXT("compare_material_probe_captures"));
	Result->SetStringField(TEXT("image_a_path"), ImageAPath);
	Result->SetStringField(TEXT("image_b_path"), ImageBPath);
	Result->SetNumberField(TEXT("width_a"), WidthA);
	Result->SetNumberField(TEXT("height_a"), HeightA);
	Result->SetNumberField(TEXT("width_b"), WidthB);
	Result->SetNumberField(TEXT("height_b"), HeightB);
	Result->SetNumberField(TEXT("compare_width"), CompareWidth);
	Result->SetNumberField(TEXT("compare_height"), CompareHeight);
	Result->SetNumberField(TEXT("mean_absolute_difference"), MeanAbsoluteDifference);
	Result->SetNumberField(TEXT("changed_pixel_ratio"), ChangedRatio);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_ReviewMaterialGraphSimplification(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	FString MaterialPath;
	FString Error;
	UMaterial* Material = LoadMaterialForWorkflowTool(Input, MaterialPath, Error);
	if (!Material)
	{
		OutResult = Error;
		return false;
	}

	TArray<int32> FocusExpressionIndices;
	TArray<int32> FocusCommentIndices;
	FBox2D FocusBounds(EForceInit::ForceInit);
	FString FocusSource;
	ExpandFocusExpressionsFromInput(Material, Input, FocusExpressionIndices, FocusCommentIndices, FocusBounds, FocusSource);
	if (FocusExpressionIndices.Num() == 0)
	{
		CollectExpressionsWithinBounds(Material, FocusBounds, FocusExpressionIndices);
	}

	int32 ParameterNodeCount = 0;
	int32 MathNodeCount = 0;
	int32 ConstantNodeCount = 0;
	int32 CrossWireCount = 0;
	TArray<TSharedPtr<FJsonValue>> Findings;
	TArray<TSharedPtr<FJsonValue>> Suggestions;
	TSet<int32> FocusSet(FocusExpressionIndices);

	auto AddSuggestion = [&](const FString& Category, const FString& Title, const FString& Detail)
	{
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
		Obj->SetStringField(TEXT("category"), Category);
		Obj->SetStringField(TEXT("title"), Title);
		Obj->SetStringField(TEXT("detail"), Detail);
		Suggestions.Add(MakeShareable(new FJsonValueObject(Obj)));
	};

	for (const int32 ExpressionIndex : FocusExpressionIndices)
	{
		if (!Material->GetExpressionCollection().Expressions.IsValidIndex(ExpressionIndex))
		{
			continue;
		}

		UMaterialExpression* Expression = Material->GetExpressionCollection().Expressions[ExpressionIndex];
		if (!Expression)
		{
			continue;
		}

		const FString Category = GetMaterialExpressionCategory(Expression->GetClass()->GetName());
		if (Category == TEXT("parameter")) ++ParameterNodeCount;
		if (Category == TEXT("math")) ++MathNodeCount;
		if (Expression->IsA<UMaterialExpressionConstant>() || Expression->IsA<UMaterialExpressionConstant2Vector>() || Expression->IsA<UMaterialExpressionConstant3Vector>()) ++ConstantNodeCount;

		for (FExpressionInputIterator It{ Expression }; It; ++It)
		{
			FExpressionInput* ExprInput = It.Input;
			if (!ExprInput || !ExprInput->Expression)
			{
				continue;
			}

			for (int32 CandidateIndex = 0; CandidateIndex < Material->GetExpressionCollection().Expressions.Num(); ++CandidateIndex)
			{
				if (Material->GetExpressionCollection().Expressions[CandidateIndex] == ExprInput->Expression && FocusSet.Contains(CandidateIndex))
				{
					if (FMath::Abs(Expression->MaterialExpressionEditorY - ExprInput->Expression->MaterialExpressionEditorY) > 360 ||
						FMath::Abs(Expression->MaterialExpressionEditorX - ExprInput->Expression->MaterialExpressionEditorX) > 900)
					{
						++CrossWireCount;
					}
					break;
				}
			}
		}
	}

	if (CrossWireCount > 4)
	{
		AddSuggestion(TEXT("layout"), TEXT("Heavy cross-lane wiring"), TEXT("Move parameters, UV/noise inputs, and final output into clearer horizontal lanes to cut wire crossings."));
	}
	if (MathNodeCount > 10)
	{
		AddSuggestion(TEXT("logic"), TEXT("High arithmetic node count"), TEXT("Consider collapsing repeated mask or edge math into a Material Function or a compact Custom node."));
	}
	if (ConstantNodeCount > ParameterNodeCount && ConstantNodeCount > 4)
	{
		AddSuggestion(TEXT("parameterization"), TEXT("Too many constants relative to parameters"), TEXT("Promote art-facing constants to ScalarParameter or VectorParameter nodes so MI tuning can absorb more lookdev."));
	}
	if (FocusCommentIndices.Num() == 0 && FocusExpressionIndices.Num() > 8)
	{
		AddSuggestion(TEXT("readability"), TEXT("Local block is large"), TEXT("Keep the local block compact and lane-aligned. Add a comment box only if the user explicitly wants comment framing."));
	}
	if (Suggestions.Num() == 0)
	{
		AddSuggestion(TEXT("status"), TEXT("No obvious simplification blockers"), TEXT("The focused block is already relatively compact for its current scope."));
	}

	TSharedPtr<FJsonObject> Result = BuildMaterialFocusResult(Material, FocusBounds, FocusExpressionIndices, FocusCommentIndices, FocusSource, 0.0f);
	Result->SetStringField(TEXT("action"), TEXT("review_material_graph_simplification"));
	Result->SetNumberField(TEXT("focused_expression_count"), FocusExpressionIndices.Num());
	Result->SetNumberField(TEXT("parameter_node_count"), ParameterNodeCount);
	Result->SetNumberField(TEXT("math_node_count"), MathNodeCount);
	Result->SetNumberField(TEXT("constant_node_count"), ConstantNodeCount);
	Result->SetNumberField(TEXT("cross_wire_count"), CrossWireCount);
	Result->SetArrayField(TEXT("findings"), Findings);
	Result->SetArrayField(TEXT("suggestions"), Suggestions);
	OutResult = JsonObjToString(Result);
	return true;
}

bool FVFXToolRegistry::Tool_RunV2MaterialOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	return Tool_RunSuctionCoreMaterialOp(Input, OutResult);
}

bool FVFXToolRegistry::Tool_RunSuctionCoreMaterialOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult)
{
	if (!Input.IsValid())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_OP_INPUT_MISSING"), TEXT("Missing input."));
	}

	FString OpName;
	FString ParentMaterialPath;
	if (!Input->TryGetStringField(TEXT("op_name"), OpName) || OpName.IsEmpty())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_OP_NAME_MISSING"), TEXT("Missing required field: op_name."));
	}
	if (!Input->TryGetStringField(TEXT("parent_material_path"), ParentMaterialPath) || ParentMaterialPath.IsEmpty())
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_PARENT_REQUIRED"), TEXT("Missing required field: parent_material_path."));
	}
	const bool bApplyChanges = ParseOptionalBoolField(Input, TEXT("apply_changes"), false);
	FString Archetype = TEXT("suction_core_energy");
	Input->TryGetStringField(TEXT("archetype"), Archetype);

	UMaterialInterface* ParentMaterial = Cast<UMaterialInterface>(UEditorAssetLibrary::LoadAsset(ParentMaterialPath));
	if (!ParentMaterial)
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_SUCTION_PARENT_NOT_FOUND"),
			FString::Printf(TEXT("Could not load parent material at '%s'."), *ParentMaterialPath));
	}

	FString SavePath = TEXT("/Game/ForgePilot_Generated/V2/Materials");
	Input->TryGetStringField(TEXT("save_path"), SavePath);
	FString InstanceName;
	Input->TryGetStringField(TEXT("instance_name"), InstanceName);

	TSharedPtr<FJsonObject> ScalarOverrides = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> VectorOverrides = MakeShareable(new FJsonObject());
	FString RequiredMaterialFamily = TEXT("hero_streak_material");
	TArray<TSharedPtr<FJsonValue>> ForbiddenMutations;
	TArray<TSharedPtr<FJsonValue>> ExpectedReviewDeltas;

	auto AddForbidden = [&ForbiddenMutations](const FString& Text)
	{
		ForbiddenMutations.Add(MakeShared<FJsonValueString>(Text));
	};
	auto AddReviewDelta = [&ExpectedReviewDeltas](const FString& Text)
	{
		ExpectedReviewDeltas.Add(MakeShared<FJsonValueString>(Text));
	};

	if (OpName.Equals(TEXT("build_hero_streak_material"), ESearchCase::IgnoreCase))
	{
		if (Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("fire_hero_streak_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroFire_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 21.0);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.50);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 1.05);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.36);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(5.5), MakeShared<FJsonValueNumber>(1.75), MakeShared<FJsonValueNumber>(0.28), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("electric_hero_streak_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroElectric_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 17.5);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.24);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 1.55);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.18);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(2.6), MakeShared<FJsonValueNumber>(4.4), MakeShared<FJsonValueNumber>(6.6), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("arcane_hero_streak_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroArcane_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 16.0);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.40);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 1.20);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.30);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(1.6), MakeShared<FJsonValueNumber>(2.8), MakeShared<FJsonValueNumber>(5.2), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("impact_hero_streak_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroImpact_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 19.0);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.34);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 0.95);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.22);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(6.8), MakeShared<FJsonValueNumber>(2.9), MakeShared<FJsonValueNumber>(0.7), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("explosion_hero_streak_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroExplosion_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 23.0);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.58);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 0.85);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.24);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(8.4), MakeShared<FJsonValueNumber>(3.1), MakeShared<FJsonValueNumber>(0.75), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else
		{
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_HeroStreak_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 18.0);
			ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.42);
			ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 1.15);
			ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.42);
			VectorOverrides->SetArrayField(TEXT("SR_ColorHot"), {
				MakeShared<FJsonValueNumber>(3.5), MakeShared<FJsonValueNumber>(1.25), MakeShared<FJsonValueNumber>(0.32), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		AddReviewDelta(TEXT("Hero streak body should read broader and less like wire lines."));
	}
	else if (OpName.Equals(TEXT("build_core_merge_glow_material"), ESearchCase::IgnoreCase))
	{
		if (Archetype.Equals(TEXT("hero_fire_orb_sweep"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("fire_core_mass_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreFlame_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 54.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 9.0);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.18);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.40);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(6.2), MakeShared<FJsonValueNumber>(1.9), MakeShared<FJsonValueNumber>(0.35), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("electric_arc_orb"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("electric_core_mass_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreElectric_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 32.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 7.0);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.12);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.34);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(2.4), MakeShared<FJsonValueNumber>(4.6), MakeShared<FJsonValueNumber>(7.2), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("arcane_pull_orb"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("arcane_core_mass_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreArcane_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 36.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 8.0);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.15);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.42);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(1.8), MakeShared<FJsonValueNumber>(2.4), MakeShared<FJsonValueNumber>(5.6), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("impact_pulse_core"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("impact_core_mass_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreImpact_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 62.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 6.5);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.10);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.28);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(7.4), MakeShared<FJsonValueNumber>(3.4), MakeShared<FJsonValueNumber>(1.1), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else if (Archetype.Equals(TEXT("explosion_flash_core"), ESearchCase::IgnoreCase))
		{
			RequiredMaterialFamily = TEXT("explosion_core_mass_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreExplosion_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 88.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 10.0);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.14);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.30);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(9.2), MakeShared<FJsonValueNumber>(4.2), MakeShared<FJsonValueNumber>(1.0), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		else
		{
			RequiredMaterialFamily = TEXT("core_merge_glow_material");
			if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_CoreMerge_V2"), *ParentMaterial->GetName());
			ScalarOverrides->SetNumberField(TEXT("CG_CoreIntensity"), 42.0);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloIntensity"), 11.0);
			ScalarOverrides->SetNumberField(TEXT("CG_CoreRadius"), 0.16);
			ScalarOverrides->SetNumberField(TEXT("CG_HaloRadius"), 0.46);
			VectorOverrides->SetArrayField(TEXT("CG_ColorHot"), {
				MakeShared<FJsonValueNumber>(5.0), MakeShared<FJsonValueNumber>(1.7), MakeShared<FJsonValueNumber>(0.45), MakeShared<FJsonValueNumber>(1.0)
			});
		}
		AddReviewDelta(TEXT("Inner hot core and shell bind should read more clearly."));
	}
	else if (OpName.Equals(TEXT("retune_longitudinal_flow"), ESearchCase::IgnoreCase))
	{
		if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_Longitudinal_V2"), *ParentMaterial->GetName());
		ScalarOverrides->SetNumberField(TEXT("SR_FlowSpeed"), 1.45);
		ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.36);
		ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.38);
		AddReviewDelta(TEXT("Flow should read more longitudinal and less orbital."));
	}
	else if (OpName.Equals(TEXT("simplify_hero_ribbon_material"), ESearchCase::IgnoreCase))
	{
		if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_SimplifiedHero_V2"), *ParentMaterial->GetName());
		ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.25);
		ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.48);
		ScalarOverrides->SetNumberField(TEXT("SR_Intensity"), 15.0);
		AddReviewDelta(TEXT("Graphic linework should soften into broader flame/plasma mass."));
	}
	else if (OpName.Equals(TEXT("reduce_opacity_noise_preserve_body"), ESearchCase::IgnoreCase))
	{
		if (InstanceName.IsEmpty()) InstanceName = FString::Printf(TEXT("MI_%s_OpacityBody_V2"), *ParentMaterial->GetName());
		ScalarOverrides->SetNumberField(TEXT("SR_NoiseAmount"), 0.18);
		ScalarOverrides->SetNumberField(TEXT("SR_Width"), 0.44);
		AddReviewDelta(TEXT("Opacity noise should step back behind the body silhouette."));
	}
	else
	{
		return BuildMaterialToolError(
			OutResult,
			TEXT("MAT_SUCTION_OP_UNSUPPORTED"),
			FString::Printf(TEXT("Unsupported suction-core Material op '%s'."), *OpName));
	}

	AddForbidden(TEXT("No Custom/HLSL graph authoring in semantic material ops."));
	AddForbidden(TEXT("Do not emit custom-expression curve workarounds."));

	if (!bApplyChanges)
	{
		TSharedPtr<FJsonObject> DryRun = MakeShareable(new FJsonObject());
		DryRun->SetBoolField(TEXT("success"), true);
		DryRun->SetBoolField(TEXT("dry_run"), true);
		DryRun->SetStringField(TEXT("action"),
			Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
				? TEXT("run_suction_core_material_op")
				: TEXT("run_v2_material_op"));
		DryRun->SetStringField(TEXT("archetype"), Archetype);
		DryRun->SetStringField(TEXT("op_name"), OpName);
		DryRun->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
		DryRun->SetStringField(TEXT("planned_instance_name"), InstanceName);
		DryRun->SetStringField(TEXT("save_path"), SavePath);
		DryRun->SetStringField(TEXT("required_material_family"), RequiredMaterialFamily);
		DryRun->SetArrayField(TEXT("forbidden_mutations"), ForbiddenMutations);
		DryRun->SetArrayField(TEXT("expected_review_deltas"), ExpectedReviewDeltas);
		DryRun->SetObjectField(TEXT("planned_scalar_overrides"), ScalarOverrides);
		DryRun->SetObjectField(TEXT("planned_vector_overrides"), VectorOverrides);
		OutResult = JsonObjToString(DryRun);
		return true;
	}

	TSharedPtr<FJsonObject> CreateInput = MakeShareable(new FJsonObject());
	CreateInput->SetStringField(TEXT("instance_name"), InstanceName);
	CreateInput->SetStringField(TEXT("parent_material_path"), ParentMaterialPath);
	CreateInput->SetStringField(TEXT("save_path"), SavePath);
	CreateInput->SetStringField(TEXT("scalar_overrides"), JsonObjToString(ScalarOverrides));
	CreateInput->SetStringField(TEXT("vector_overrides"), JsonObjToString(VectorOverrides));

	FString CreateResultJson;
	if (!Tool_CreateMaterialInstance(CreateInput, CreateResultJson))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_INSTANCE_CREATE_FAILED"), CreateResultJson);
	}

	TSharedPtr<FJsonObject> CreateResult;
	if (!ParseToolJsonResult(CreateResultJson, CreateResult))
	{
		return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_INSTANCE_PARSE_FAILED"), TEXT("Could not parse create_material_instance result."));
	}

	const FString CreatedMaterialPath = CreateResult->GetStringField(TEXT("asset_path"));
	FString ProbeActorLabel;
	Input->TryGetStringField(TEXT("probe_actor_label"), ProbeActorLabel);
	FString CapturePath;
	if (!ProbeActorLabel.IsEmpty())
	{
		TSharedPtr<FJsonObject> ApplyInput = MakeShareable(new FJsonObject());
		ApplyInput->SetStringField(TEXT("actor_label"), ProbeActorLabel);
		ApplyInput->SetStringField(TEXT("material_path"), CreatedMaterialPath);
		FString ApplyResultJson;
		if (!Tool_SetActorMaterial(ApplyInput, ApplyResultJson))
		{
			return BuildMaterialToolError(OutResult, TEXT("MAT_SUCTION_PROBE_APPLY_FAILED"), ApplyResultJson);
		}

		TSharedPtr<FJsonObject> CaptureInput = MakeShareable(new FJsonObject());
		CaptureInput->SetStringField(TEXT("actor_label"), ProbeActorLabel);
		CaptureInput->SetBoolField(TEXT("prefer_editor_preview"), true);
		CaptureInput->SetBoolField(TEXT("force_thumbnail_fallback"), true);
		FString CaptureFileName;
		Input->TryGetStringField(TEXT("capture_file_name"), CaptureFileName);
		if (!CaptureFileName.IsEmpty())
		{
			CaptureInput->SetStringField(TEXT("file_name"), CaptureFileName);
		}
		FString CaptureResultJson;
		if (Tool_CaptureMaterialVisualProbe(CaptureInput, CaptureResultJson))
		{
			TSharedPtr<FJsonObject> CaptureResult;
			if (ParseToolJsonResult(CaptureResultJson, CaptureResult) && CaptureResult.IsValid())
			{
				CaptureResult->TryGetStringField(TEXT("output_path"), CapturePath);
			}
		}
	}

	TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
	Result->SetBoolField(TEXT("success"), true);
	Result->SetStringField(TEXT("action"),
		Archetype.Equals(TEXT("suction_core_energy"), ESearchCase::IgnoreCase)
			? TEXT("run_suction_core_material_op")
			: TEXT("run_v2_material_op"));
	Result->SetStringField(TEXT("archetype"), Archetype);
	Result->SetStringField(TEXT("op_name"), OpName);
	Result->SetStringField(TEXT("material_path"), CreatedMaterialPath);
	Result->SetStringField(TEXT("required_material_family"), RequiredMaterialFamily);
	Result->SetArrayField(TEXT("forbidden_mutations"), ForbiddenMutations);
	Result->SetArrayField(TEXT("expected_review_deltas"), ExpectedReviewDeltas);
	Result->SetArrayField(TEXT("mutated_assets"), {
		MakeShared<FJsonValueString>(CreatedMaterialPath)
	});
	Result->SetStringField(TEXT("compile_status"), TEXT("saved_material_instance"));
	Result->SetBoolField(TEXT("log_checked"), false);
	if (!CapturePath.IsEmpty())
	{
		Result->SetStringField(TEXT("capture_artifact_path"), CapturePath);
	}
	Result->SetArrayField(TEXT("predicted_resolved_failure_labels"), {
		MakeShared<FJsonValueString>(TEXT("linework_not_flame_body"))
	});
	Result->SetArrayField(TEXT("predicted_remaining_failure_labels"), {
		MakeShared<FJsonValueString>(TEXT("requires_visual_review"))
	});
	Result->SetObjectField(TEXT("created_instance"), CreateResult);
	OutResult = JsonObjToString(Result);
	return true;
}
