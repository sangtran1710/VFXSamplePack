# Task Template for Codex

Copy this format when assigning tasks. It is designed for the phased workflow:

`Builder -> Reviewer -> Debugger -> Final QA`

---

## Template

```
### Task: [short name]

**What**: [1-2 sentences, what to build/fix]

**Files to modify**:
- [list exact files]

**Files to NOT touch**:
- Source/<HostProject>/* (host project game code)
- Knowledge/*.json (managed by tech advisor)

**Acceptance criteria**:
1. [ ] Build pass: `build_unreal_with_recovery.ps1`
2. [ ] Bridge verify: `invoke_bridge.ps1 -ToolName "<tool>" -ArgsJson '<json>'`
3. [ ] [specific runtime or visual test]

**Phase gates**:
- Builder: [what must exist before review starts]
- Reviewer: [top 2-3 things to critique]
- Debugger: [highest-priority fixes only]
- Final QA: [what must be proven with evidence]

**Max rounds**:
- [usually 2 or 3]

**Pattern to follow**: [reference existing file as example]
```

---

## Example

```
### Task: Add set_niagara_module_parameter tool

**What**: New tool that sets a specific parameter on a Niagara module within an emitter stack. Needed for automated effect creation from recipes.

**Files to modify**:
- VFXToolRegistry.h (add declaration)
- VFXToolRegistry_Niagara.cpp (add implementation)

**Files to NOT touch**:
- Source/<HostProject>/*
- Knowledge/*.json

**Acceptance criteria**:
1. [ ] Build pass: `build_unreal_with_recovery.ps1`
2. [ ] Bridge verify: `invoke_bridge.ps1 -ToolName "set_niagara_module_parameter" -ArgsJson '{"system_path":"/Game/test","emitter_name":"Flames","module_name":"Spawn Rate","param_name":"SpawnRate","value":30}'`
3. [ ] Returns success JSON with previous value

**Phase gates**:
- Builder: tool exists and compiles
- Reviewer: parameter naming, error handling, and response JSON are checked
- Debugger: fix the top API or validation issues
- Final QA: compile + bridge invoke + bad input smoke test

**Max rounds**:
- 2

**Pattern to follow**: See `set_niagara_user_parameter` in VFXToolRegistry_Niagara.cpp
```
