# Codex Local/Hybrid Setup For ForgePilot

This guide is the working setup for running ForgePilot automation with a local/hybrid Codex stack:

- cheap and repetitive steps run deterministically or on a local model through Ollama/LM Studio;
- risky C++/Unreal/VFX reasoning escalates to the strong OpenAI profile;
- Codex config stays split between a stable user config and optional project overrides.

## Config Files

Codex reads user config from:

```text
C:\Users\ADMIN\.codex\config.toml
```

Codex can also read trusted project overrides from the active host project:

```text
<host_project_root>\Plugins\ForgePilot\.codex\config.toml
```

Use the template here:

- [CODEX_LOCAL_HYBRID_CONFIG.template.toml](../Templates/CODEX_LOCAL_HYBRID_CONFIG.template.toml)

Do not overwrite the full user config blindly. Merge the template and keep existing MCP server blocks, especially `mcp_servers.forgepilot`.

## User Config Pattern

Keep the user-level config strong by default:

```toml
profile = "openai-strong"

model = "gpt-5.4"
model_provider = "openai"
model_reasoning_effort = "high"
model_reasoning_summary = "none"
model_verbosity = "medium"
approval_policy = "on-request"
sandbox_mode = "workspace-write"
web_search = "cached"

oss_provider = "ollama"

[model_providers.lmstudio_local]
name = "LM Studio Local"
base_url = "http://localhost:1234/v1"
```

This keeps normal coding/debugging on OpenAI while using the built-in Ollama provider for cheap local runs.

Do not use `[model_providers.ollama]` as a custom table name. `ollama` is a reserved built-in Codex provider ID in current Codex builds, and overriding it can prevent Codex Desktop from resuming a thread. Use `model_provider = "ollama"` without declaring a matching custom provider table.

## Cheap Project Override

For a local-only ForgePilot automation workspace, create:

```text
<host_project_root>\Plugins\ForgePilot\.codex\config.toml
```

Ollama version:

```toml
model = "your-local-model-id"
model_provider = "ollama"
model_reasoning_effort = "low"
model_reasoning_summary = "none"
model_verbosity = "low"
approval_policy = "never"
sandbox_mode = "workspace-write"
web_search = "disabled"
model_context_window = 32768
model_auto_compact_token_limit = 24000
```

LM Studio version:

```toml
model = "openai/gpt-oss-20b"
model_provider = "lmstudio_local"
model_reasoning_effort = "low"
model_reasoning_summary = "none"
model_verbosity = "low"
approval_policy = "never"
sandbox_mode = "workspace-write"
web_search = "disabled"
model_context_window = 32768
model_auto_compact_token_limit = 24000
```

Use the exact model ID shown by Ollama or LM Studio on your machine. If you are not actively using a local model, skip the project override and keep Codex Desktop on the normal OpenAI default.

## ForgePilot Routing Rule

Default rule:

```text
script first -> local model second -> strong model only on escalation
```

Local is allowed to classify, normalize, summarize, and apply fixed rules. Local is not allowed to make risky Unreal C++ changes, invent new VFX families, or sign off complex visual quality.

## Token-Saving Automation Pipeline

Run the PowerShell examples from the active host project root:

```text
<host_project_root>
```

### Phase 0. Deterministic preflight

No model.

Run:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -Ping
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ListTools
```

Also check:

- required parent material paths exist;
- save path is under the intended `/Game/...` root;
- `system_name` is safe and deterministic;
- no build is run unless C++ changed.

If C++ changed, use:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/build_unreal_with_recovery.ps1"
```

### Phase 1. Local request router

Use Ollama/LM Studio with low reasoning and a tiny schema. Input should be only:

- raw user request;
- known V2 families;
- allowed save roots;
- current bridge health summary.

The local model must output strict JSON under 1 KB:

```json
{
  "task_kind": "v2_family",
  "family": "suction_core_energy",
  "needs_realtimevfx_research": false,
  "should_build": false,
  "runner": "run_effect_workflow_v2",
  "critic": "review_effect_failure_taxonomy_v2",
  "save_path": "/Game/ForgePilot_Test/V2Platform",
  "system_name": "NS_SuctionCore_Auto",
  "max_repair_iterations": 1,
  "capture_thumbnail": true,
  "apply_changes": true,
  "keep_assets": false,
  "escalate_to_strong_model": false,
  "reason": "Known V2 family with existing workflow runner"
}
```

Reject local output if it is not valid JSON, if it asks to edit C++, or if it selects a runner outside the allowed tool list.

### Phase 2. Deterministic VFX execution

No strong model.

For known V2 orb-family work:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "resolve_effect_plan_v2" -ArgsJson '{"description":"impact pulse orb with a sharp shock burst and bright core flash"}'
```

Then run the production workflow:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_effect_workflow_v2" -ArgsJson '{"description":"impact pulse orb with a sharp shock burst and bright core flash","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/V2Platform","system_name":"NS_ImpactPulse_Auto","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true,"keep_assets":false}'
```

For `suction_core_energy`, the compatibility wrapper is allowed when explicitly requested:

```powershell
powershell -File "Plugins/ForgePilot/Tools/Validation/invoke_bridge.ps1" -ToolName "run_suction_core_v2_workflow" -ArgsJson '{"description":"fire energy suction orb pulling streaks from all directions into a bright hot core","hero_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireCoreSunergit_Ribbon_A01.M_FireCoreSunergit_Ribbon_A01","core_parent_material_path":"/Game/ForgePilot_Test/FireEnergyArcs/M_FireEnergyCoreGlow_Sprite_A01.M_FireEnergyCoreGlow_Sprite_A01","save_path":"/Game/ForgePilot_Test/CoreEnergy","system_name":"NS_SuctionCore_Auto","max_repair_iterations":1,"capture_thumbnail":true,"apply_changes":true}'
```

### Phase 3. Deterministic critic

Prefer the rules-based critic. The local model only summarizes the critic output.

Allowed local output:

```json
{
  "decision": "pass",
  "retry_allowed": false,
  "escalate_to_strong_model": false,
  "summary": "Workflow completed and critic did not trigger a hard gate."
}
```

Allowed `decision` values:

- `pass`
- `retry_once`
- `escalate`

### Phase 4. Escalation gate

Escalate to the strong OpenAI profile when any condition is true:

- `task_kind` is `bespoke_vfx`;
- RealTimeVFX research is required;
- C++ files under `Source/ForgePilot/` must change;
- bridge returns malformed or contradictory JSON;
- Unreal build fails;
- editor crashes or recovery script fires;
- critic hard-gates the result;
- local retry budget is exhausted;
- task touches raw Niagara/material tooling beyond the V2 runner path.

The strong model receives only distilled context:

```json
{
  "normalized_task": {},
  "failed_command": "",
  "tool_name": "",
  "tight_log_excerpt": "",
  "critic_summary": "",
  "screenshot_paths": [],
  "relevant_files": []
}
```

Do not pass full logs or the whole chat unless needed.

### Phase 5. Strong-model intervention

Use `openai-strong` for:

- cross-file C++ edits in `VFXToolRegistry*`, bridge, cache, audit, rate limiter, or metrics;
- adding or changing MCP tools;
- build failure diagnosis;
- crash recovery reasoning;
- novel Niagara/material authoring outside the current V2 families;
- RealTimeVFX research and implementation translation.

### Phase 6. Cheap closeout

Use deterministic output or the local model for:

- final run summary;
- report formatting;
- asset path list;
- retry note;
- short handoff for a later strong-model thread.

## Local Router Prompt

Use this prompt with Ollama/LM Studio:

```text
You are the cheap ForgePilot router. Return only strict JSON.
Do not propose C++ edits. Do not invent tools.
Classify the request into v2_family, bespoke_vfx, infra, or validation.
For v2_family, choose only run_effect_workflow_v2 unless the request explicitly asks for suction_core_energy compatibility.
Escalate when the task needs RealTimeVFX research, new authoring logic, C++ edits, crash recovery, or non-V2 visual invention.
Keep output under 1 KB.
```

## Practical Desktop Setup

Use two entry points:

- main plugin root for strong coding/debugging work with `openai-strong`;
- a trusted cheap automation workspace with `.codex/config.toml` pinned to Ollama or LM Studio.

This avoids constantly flipping global model settings and reduces the chance that a hard debugging job accidentally runs on the weak local profile.

## References

- [OpenAI Codex config basics](https://developers.openai.com/codex/config-basic)
- [OpenAI Codex advanced config](https://developers.openai.com/codex/config-advanced)
- [OpenAI Codex configuration reference](https://developers.openai.com/codex/config-reference)
- [LM Studio OpenAI-compatible endpoints](https://lmstudio.ai/docs/developer/openai-compat)
