# Versioned ForgePilot Layout

This project keeps separate ForgePilot workspaces for each Unreal Engine line, but they do not have equal product priority.

## Canonical folders

- `Plugins/ForgePilot/Versions/UE5.3/Plugins/ForgePilot` is the UE5.3 legacy compatibility workspace.
- `Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot` is the canonical UE5.7 rebuild workspace.
- `Plugins/ForgePilot` is the staging/shared plugin copy used for active repo cleanup and documentation.

## Build policy

- Build and refactor primary work from `Plugins/ForgePilot/Versions/UE5.7`.
- Do not treat UE5.3 as a required parity branch.
- Only touch `Plugins/ForgePilot/Versions/UE5.3` when the user explicitly requests a compatibility fix or legacy maintenance task.
- Do not mix generated artifacts, binaries, or intermediate files between engine versions.
- Keep `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, and `Output` out of versioned source copies unless a release package explicitly needs them.

## Migration policy

- UE5.7 changes should land in `Plugins/ForgePilot/Versions/UE5.7/Plugins/ForgePilot` first.
- UE5.3 compatibility fixes should land in `Plugins/ForgePilot/Versions/UE5.3/Plugins/ForgePilot` only when explicitly requested.
- Shared behavior does not need to be ported back to UE5.3 by default.

## Token policy

To avoid wasting context and implementation time:

1. Plan for UE5.7 first.
2. Validate on UE5.7 first.
3. Document UE5.3 gaps when they matter.
4. Do not spend tokens keeping both branches structurally identical unless the user asks for that work.
