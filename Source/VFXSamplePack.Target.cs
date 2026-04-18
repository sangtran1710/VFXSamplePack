using UnrealBuildTool;

public class VFXSamplePackTarget : TargetRules
{
	public VFXSamplePackTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[] { "VFXSamplePack" });
	}
}
