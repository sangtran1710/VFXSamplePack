# ForgePilot Version Workspaces

ForgePilot is split by Unreal Engine line here.

## UE5.7

- Path: `UE5.7/VFXSamplePack.uproject`
- Plugin: `UE5.7/Plugins/ForgePilot`
- Role: active/canonical build and rebuild workspace
- Plugin browser label: `ForgePilot UE5.7`

## UE5.3

- Path: `UE5.3/VFXSamplePack.uproject`
- Plugin: `UE5.3/Plugins/ForgePilot`
- Role: legacy compatibility maintenance only
- Plugin browser label: `ForgePilot UE5.3 Legacy`

Do new ForgePilot work in `UE5.7` first. Touch `UE5.3` only when an explicit legacy compatibility task requires it.
