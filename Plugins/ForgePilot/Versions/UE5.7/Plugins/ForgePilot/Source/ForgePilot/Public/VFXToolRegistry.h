// Copyright 2026, HenryFX. ForgePilot Plugin.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

DECLARE_DELEGATE_RetVal_TwoParams(bool, FVFXToolExecuteDelegate,
	const TSharedPtr<FJsonObject>&, FString&);

struct FVFXToolDefinition
{
	FString Name;
	FString Description;
	TSharedPtr<FJsonObject> InputSchema;
	FString ToolGroup;
	FString UnrealDomain;
	FString WorkflowLane;
	FString SurfaceArea;
	FString PrimaryLane;
	FString VfxAffinity;
	TArray<FString> CapabilityTags;
	FString Mode;
	FString MutationLevel;
	bool bSafeDefault = false;
	bool bProjectGrounded = false;
	FString DefaultExposure;
	FVFXToolExecuteDelegate ExecuteDelegate;
};

class FVFXToolRegistry
{
public:
	FVFXToolRegistry();
	friend void RegisterNiagaraStackEditingTools(FVFXToolRegistry& Registry);

	// Tool registration
	void RegisterTool(const FVFXToolDefinition& ToolDef);
	void UnregisterTool(const FString& ToolName);

	// Tool execution
	bool ExecuteTool(const FString& ToolName, const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Tool queries
	TArray<TSharedPtr<FJsonValue>> GetToolDefinitionsJson() const;
	bool HasTool(const FString& ToolName) const;
	int32 GetToolCount() const { return Tools.Num(); }
	bool AreDefaultToolsRegistered() const { return bDefaultToolsRegistered; }

	// Default registration
	void RegisterDefaultTools();

	// Per-phase registration
	void RegisterSceneTools();
	void RegisterBlueprintTools();
	void RegisterMaterialTools();
	void RegisterNiagaraTools();
	void RegisterAnimationTools();

	// ============================================================
	// Lazy Loading
	// ============================================================

	/**
	 * Register tools for a specific category on-demand
	 * @param Category - "scene", "blueprint", "material", "niagara"
	 */
	void RegisterToolsForCategory(const FString& Category);

	/**
	 * Check if a category is loaded
	 */
	bool IsCategoryLoaded(const FString& Category) const;

	/**
	 * Get list of loaded categories
	 */
	TArray<FString> GetLoadedCategories() const;

	/**
	 * Unload all tools (for memory optimization)
	 */
	void UnloadAllTools();

private:
	TMap<FString, FVFXToolDefinition> Tools;

	// Lazy loading state
	TSet<FString> LoadedCategories;
	bool bDefaultToolsRegistered = false;

	// Phase 1 test tools
	bool Tool_GetSceneActors(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedActorDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_TraceActorRelationships(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SpawnActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DestroyActorByLabel(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedBlueprintPath(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMutationAuditEntries(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ScanAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SearchAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_TraceAssetRelationships(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_IndexVFXContentLibrary(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateAssetFolder(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RenameAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_MoveAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DuplicateAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FixRedirectorsInFolder(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FindEmptyFolders(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ListRecentAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_BatchRenameAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_BatchMoveAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FindLargeAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FindAssetsModifiedSince(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ImportAssetFromFile(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_OpenAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_BrowseToAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DeleteAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_VerifyViewportCaptureState(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CaptureViewport(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CaptureAssetThumbnail(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_StartPlayInViewport(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_StopPlaySession(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetPIESessionState(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SendPIEKeyInput(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunPIEProbeScenario(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_InspectSkeletalAttachmentContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_LoadVFXRecipe(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ResolveEffectPlan(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ResolveEffectPlanV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GenerateVisualContractDataAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewEffectFailureTaxonomyV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewSuctionCoreFailureTaxonomy(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunEffectWorkflowV2(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunSuctionCoreV2Workflow(const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Phase 2: Blueprint manipulation tools
	bool Tool_GetBlueprintSummary(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintVariables(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintComponents(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintNodeDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedBlueprintNodeDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintInventory(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintComponentDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintGraphLayout(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SearchBlueprintNodes(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FindBlueprintAssetUsage(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_TraceBlueprintExecChain(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SummarizeBlueprintValueFlow(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SummarizeBlueprintMaterialRuntime(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintClassDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_InspectBlueprintArrayDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetBlueprintMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateBlueprint(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddVariable(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddComponent(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_EditComponentProperty(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateCustomEventNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateBlueprintCallFunctionNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateBlueprintVariableSetNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateBlueprintVariableGetNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AttachComponentToComponentSocket(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DeleteBlueprintNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ConnectBlueprintPins(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetBlueprintNodePinDefault(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CompileBlueprintWithFeedback(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RestoreBlueprintMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewBlueprintAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Phase 3: Material tools
	bool Tool_CreateMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CompileMaterialAndGetDiagnostics(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FocusMaterialGraphRegion(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FocusMaterialGraphExpression(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FocusMaterialGraphComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_PanMaterialGraph(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialGraphZoom(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CaptureMaterialGraphScreenshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ValidateMaterialGraphBlock(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SummarizeMaterialBlockWiring(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedMaterialExpressionDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialInstanceInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetTextureInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetTextureMaterialInspection(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetSelectedTextureInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialMutationAuditEntries(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialScalarParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialVectorParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialTextureParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateMaterialInstance(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateMaterialTestInstanceSet(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SweepMaterialInstanceParameters(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetActorMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SpawnMaterialProbeActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ApplyMaterialToProbeActor(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FrameProbeActorForCapture(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CaptureMaterialVisualProbe(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CompareMaterialProbeCaptures(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddMaterialExpression(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateMaterialPatternBlock(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateMaterialParameterPack(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialCustomNode(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateMaterialComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_UpdateMaterialComment(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DeleteMaterialComments(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DeleteMaterialExpressions(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_LayoutMaterialExpressions(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CleanupMaterialGraph(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ConnectMaterialNodes(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetMaterialProperties(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetMaterialFunctionInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewMaterialFunction(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RestoreMaterialMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewMaterialGraphSimplification(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewMaterialGraphHygiene(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewMaterialAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunV2MaterialOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunSuctionCoreMaterialOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Phase 4: Niagara VFX tools
	bool Tool_CreateEmptyNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CreateNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_FindNiagaraTemplateEmitters(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddEmptyNiagaraEmitter(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraSystemInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraEmitterDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraRendererDetails(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraUserParameterDefaults(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraActorUserParameterValues(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraMutationSnapshots(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RestoreNiagaraMutationSnapshot(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CleanupNiagaraAuditAssets(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CleanupNiagaraAuthoringResidue(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraEmitterStack(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetNiagaraModuleInputs(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SpawnNiagaraAtLocation(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RemoveNiagaraUserParameter(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraUserParameterDefault(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_DuplicateNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddNiagaraEmitterToSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_AddNiagaraModuleToStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RemoveNiagaraEmittersFromSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RemoveNiagaraModuleFromStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_MoveNiagaraModuleInStage(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_CompileNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraModuleInputConstant(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraModuleInputLinked(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraModuleInputCurve(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraModuleInputDynamic(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ResetNiagaraModuleInput(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraSpawnRate(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraLifetime(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraSpriteSize(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraColorOverLife(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraOpacityOverLife(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraVelocity(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraConeVelocity(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraGravityForce(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraDrag(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraCurlNoiseForce(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraLocalSpace(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraEmitterEnabled(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraRendererEnabled(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraRendererMaterial(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraRendererMaterialUserParam(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraRibbonDeterministicStreams(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraSpriteRendererSubUV(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraSystemFixedBounds(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_SetNiagaraEmitterFixedBounds(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewNiagaraRenderer(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewNiagaraEmitter(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewNiagaraSystem(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunV2NiagaraOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_RunSuctionCoreNiagaraOp(const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Phase 5: Animation-adjacent inspection tools
	bool Tool_GetAnimationEditorContext(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_GetAnimationAssetInfo(const TSharedPtr<FJsonObject>& Input, FString& OutResult);
	bool Tool_ReviewAnimationAsset(const TSharedPtr<FJsonObject>& Input, FString& OutResult);

	// Helper: convert type string to FEdGraphPinType
	static bool ConvertTypeToPinType(const FString& TypeStr, struct FEdGraphPinType& OutPinType);
};
