# VFXSamplePack UE5.7 Workspace

This folder is the UE5.7 rebuild internal workspace for ForgePilot.

- Project file: `VFXSamplePack.uproject`
- Plugin: `Plugins/ForgePilot`
- Engine association: `5.7`
- Product role: canonical ForgePilot build/rebuild workspace

Internal path: `Plugins/ForgePilot/Versions/UE5.7`

Use this folder when refactoring or rebuilding ForgePilot for Unreal Engine 5.7 without leaving plugin-owned files at the project root.

## Policy

- Build new tools here first.
- Validate bridge/tool behavior here first.
- Do not mirror work to `UE5.3` unless legacy maintenance is explicitly requested.
