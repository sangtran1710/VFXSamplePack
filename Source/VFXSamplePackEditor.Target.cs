using UnrealBuildTool;

public class VFXSamplePackEditorTarget : TargetRules
{
	public VFXSamplePackEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "VFXSamplePack" });
	}
}
