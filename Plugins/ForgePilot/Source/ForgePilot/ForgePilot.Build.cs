using UnrealBuildTool;

public class ForgePilot : ModuleRules
{
	public ForgePilot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false; // Disable Unity Build - each .cpp is a separate translation unit

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"ApplicationCore",
			"UnrealEd",
			"EditorSubsystem",
			"EditorFramework",
			"ToolMenus",
			"Projects",
			"HTTP",
			"Json",
			"JsonUtilities",
			"Sockets",
			"EditorScriptingUtilities",
			"Blutility",
			"ContentBrowser",
			"AssetTools",
			"AnimationBlueprintLibrary",
			"Kismet",
			"KismetCompiler",
			"BlueprintGraph",
			"RenderCore",
			"RHI",
			"Niagara",
			"NiagaraEditor",
			"ImageWrapper",
			"LevelEditor",
			"DesktopPlatform",
			"PythonScriptPlugin",
			"MaterialEditor",
			"Sequencer",
		});
	}
}
