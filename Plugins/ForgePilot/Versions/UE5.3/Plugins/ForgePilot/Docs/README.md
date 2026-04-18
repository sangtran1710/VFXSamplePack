# ForgePilot Docs

This folder groups the plugin's reusable internal docs so the repo root stays focused on code, knowledge assets, and tools.

## Folders

- `Agents/`: role prompts and execution prompts for AI-assisted workflows.
- `Templates/`: reusable report/task templates.
- `Plans/`: implementation plans and rebuild notes.
- `Guides/`: workflow guides, cookbooks, and test-structure docs.
- `Tracking/`: backlog and improvement logs.
- `Legacy/`: alias docs kept for old references.

## Notes

- `Knowledge/` remains separate because it is runtime knowledge loaded by the plugin.
- `Docs/` is for human/agent reference and workflow reuse.
- `Agents/VFX_ASSISTANT_PROMPT.md` is the canonical default assistant prompt for the Ask + Assist first product direction.
- `Guides/ASK_ASSIST_FIRST.md` is the canonical default usage guide.
- `Guides/TOOL_SURFACE.md` is the grouped Ask / Assist / Advanced Build inventory for the current tool surface.
- `Guides/FORGEPILOT_MULTI_PROJECT_STRATEGY.md` describes the long-term multi-project, multi-style, and version-aware direction for ForgePilot.
- `Guides/VFX_ART_STYLE_TAXONOMY.md` defines the major art-style families the agent should choose between before authoring visual effects.
- `Guides/REALISTIC_VFX_ART_DIRECTION.md` captures realistic VFX art direction rules for fire, embers, smoke, dust, heat, and grounded magic.
- `Guides/DYNAMIC_PARAMETER_WORKFLOW.md` defines the Niagara-to-Material Dynamic Parameter contract, packing strategy, art-style use cases, and validation gate.
- `Guides/NIAGARA_CURVE_LOGIC_PATTERNS.md` captures Niagara timing-domain choice, biased random distribution, linked randomness, and curve-driven behavior design patterns.
- `Guides/SUCTION_CORE_V2_WORKFLOW.md` is the canonical doc for the `suction_core_energy` V2 production path.
- `Guides/ORB_FAMILIES_V2_PLATFORM.md` is the canonical doc for the generic 6-family V2 orb platform in this milestone.
- `Plans/UE57_REBUILD_RULES.md` is the mandatory operating contract for refactoring/rebuilding ForgePilot for Unreal Engine 5.7.
- Builder/reviewer prompts remain available as advanced/internal build-flow prompts rather than the default assistant identity.
