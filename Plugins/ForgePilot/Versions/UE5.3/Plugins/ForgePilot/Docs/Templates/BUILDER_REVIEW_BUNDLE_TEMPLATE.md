# Builder Review Bundle Template

Use this file when Builder prepares a pass for Art Visual Assistant and Tester QA.

---

## Bundle Metadata

- `Task Name`:
- `Pass Name`:
- `Date`:
- `Map / Scene`:
- `Primary Asset(s)`:
- `Capture Context`:
  - editor preview / PIE / gameplay / test harness

## Target

- `Effect Goal`:
- `Reference / Mood`:
- `Gameplay Requirement`:

## What Changed

- `Change 1`:
- `Change 2`:
- `Change 3`:

## Real Verification

- `Build result`:
- `Bridge/tool result`:
- `Capture result`:
- `Asset compile result`:
- `Runtime result`:

## Known Weaknesses

- `Weakness 1`:
- `Weakness 2`:
- `Weakness 3`:

## Art Review Focus

Frames Art Visual Assistant should check first:

1. `frame_00_start`
2. `frame_01_impact`
3. `frame_02_peak`
4. `frame_03_decay`

Primary art questions:

1. Is the target read correct?
2. Is the effect readable in gameplay view?
3. Which visual issue should be fixed first?

## Tester QA Focus

Frames or logs Tester QA should check first:

1. `frame_00_start`
2. `frame_01_impact`
3. `frame_02_peak`
4. `frame_03_decay`
5. `relevant log excerpt`

Primary QA questions:

1. Did the effect trigger correctly?
2. Is it attached or spawned at the right place?
3. Are there cleanup, regression, or wrong-asset issues?

## Expected Bundle Layout

- `Builder/summary.json`
- `Builder/builder_notes.md`
- `Builder/shots/`
- `Art/art_visual_report.md`
- `QA/tester_qa_report.md`
