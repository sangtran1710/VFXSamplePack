# ForgePilot UE5.3 Compatibility Rules

This document is the operating contract for maintaining the UE5.3-compatible ForgePilot copy.

## Goals

- Preserve the existing UE5.3 workflow so older projects can still use ForgePilot.
- Keep the MCP bridge, tool registry, dry-run behavior, mutation audit, Knowledge Base, Material tools, Niagara tools, and Blueprint safety behavior aligned with the original product intent.
- Accept shared fixes from UE5.7 only when they are compatible with UE5.3 APIs or safely guarded.

## Boundaries

- Do not introduce UE5.7-only C++ APIs, module dependencies, editor APIs, Python assumptions, or Niagara/Material graph calls into this copy without a UE5.3-compatible fallback.
- Do not mix build artifacts from UE5.7 into this folder.
- Keep generated artifacts out of source: `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, and `Output`.
- Keep host-project game code changes minimal and explicit; plugin work should stay under `Plugins/ForgePilot` unless the user asks otherwise.

## Required checks

- Descriptor JSON must parse: `VFXSamplePack.uproject` and `Plugins/ForgePilot/ForgePilot.uplugin`.
- Plugin source hygiene must not depend on hardcoded local project paths.
- Validation scripts must run from the UE5.3 workspace root.
- Unreal build verification should use UE5.3 when available.
- Any tool that can mutate assets must preserve dry-run and `apply_changes` semantics.
