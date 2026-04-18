# Material Workflow Rebuild Plan

See also:

- [MATERIAL_COOKBOOK.md](../Guides/MATERIAL_COOKBOOK.md) for reusable Material patterns and validation rules learned from real runs.

## Purpose

This plan captures the next rebuild pass for Material support in `ForgePilot` after the `FirePersonErosion` test.

The goal is not just to author a Material that technically compiles. The goal is to make the agent reliable at:

- building clean Material graphs
- validating graph wiring automatically
- tuning visuals through Material Instances first
- capturing readable graph screenshots
- capturing usable visual proof from preview/level shots
- only returning to master Material logic when MI tuning is insufficient

This plan is the execution baseline before deleting the current test Material and rerunning a fresh `FirePersonErosion` build.

## Key Findings From FirePersonErosion Test

### What worked

- The agent can create a new Material asset.
- The agent can add expressions and connect them correctly.
- The agent can parameterize the graph for MI use.
- The agent can create comments and apply coarse layout.
- The agent can scan the graph and detect disconnected math inputs.
- The agent can create a preview MI and override parameters.

### What failed or remained weak

- Graph screenshots were not reliable because the plugin cannot yet focus/pan/zoom the Material graph in a deterministic way.
- Visual proof was weak because viewport capture did not guarantee that the modified Material was actually visible in frame.
- Layout was functionally valid but still too wire-heavy and difficult to review quickly.
- The validation loop checked wiring, but not graph readability.
- The workflow went to master Material edits too early instead of sweeping MI values first.

## Target Workflow

The rebuilt Material workflow should be:

1. Build a minimal master Material graph with clean lanes.
2. Validate graph connectivity automatically.
3. Focus and capture the Material graph clearly.
4. Create one or more MI test variants.
5. Sweep MI parameters and capture visual results.
6. Compare captures and decide whether the issue is:
   - parameter tuning
   - graph layout/readability
   - master graph logic
7. Only then mutate the master graph again.

## Phase MAT-R1: Graph Viewport Control

### Goal

Make Material graph screenshots reliable and readable.

### Deliverables

- `focus_material_graph_region`
- `focus_material_graph_expression`
- `focus_material_graph_comment`
- `pan_material_graph`
- `set_material_graph_zoom`
- `capture_material_graph_screenshot`

### Behavior

The tools should support:

- focusing by expression index
- focusing by comment text
- focusing by query / local block
- explicit pan delta
- explicit zoom target
- capture of graph region only, not whole UE window

### Acceptance criteria

- The agent can capture a graph screenshot where authored nodes are visible without manual user assistance.
- The screenshot path is written to `Downloads`, not inside the plugin.
- The tool returns enough metadata to know:
  - material asset path
  - focus source
  - zoom
  - capture path

## Phase MAT-R2: Graph Sanity Validator

### Goal

Make Material authoring fail closed instead of fail open.

### Deliverables

- `validate_material_graph_block`
- `summarize_material_block_wiring`
- optional: `repair_common_material_wiring_issues`

### Required checks

- required math inputs connected
- required root pins connected
- source output indices valid
- texture sample UV chain present if expected
- comments exist for AI-authored regions
- authored block nodes are within intended layout region

### Acceptance criteria

- The validator catches the exact class of issue found in the first erosion attempt: missing `B` inputs on math nodes.
- The validator can be run after every mutate pass before the task is considered complete.

## Phase MAT-R3: MI-First Visual Tuning Loop

### Goal

Shift visual iteration away from the master graph and into MI sweeps.

### Deliverables

- `create_material_test_instance_set`
- `sweep_material_instance_parameters`
- `capture_material_visual_probe`
- `compare_material_probe_captures`

### Intended use

For a test Material:

- create base MI
- create tuned MI variants
- vary:
  - threshold
  - softness
  - edge width
  - emissive intensity
  - inner flame intensity
  - colors
- capture a sequence of results

### Acceptance criteria

- The agent can generate a small parameter sweep set and capture each visual output.
- The agent can summarize which MI is closest to the intended effect.
- Master graph changes are not made until MI sweeps are exhausted.

## Phase MAT-R4: Visual Probe Reliability

### Goal

Make viewport screenshots useful as evidence.

### Deliverables

- `spawn_material_probe_actor`
- `apply_material_to_probe_actor`
- `frame_probe_actor_for_capture`
- optional preset probe scenes

### Requirements

- Probe actor must spawn in a known visible position.
- Probe actor should be easy to destroy after the run.
- Viewport capture should confirm that the tested Material is visible in frame.

### Acceptance criteria

- Captured images clearly show the test mesh with the Material applied.
- The agent no longer depends on whatever happened to be in the active viewport.

## Phase MAT-R5: Master Graph Simplification

### Goal

Reduce visual clutter and wire noise in authored graphs.

### Deliverables

- `review_material_graph_simplification`
- smarter lane placement heuristics
- pattern guidance for when to replace node chains with:
  - `Custom`
  - small `Material Function`
  - existing UE nodes that collapse math

### Rules

- Use MI parameters for art control.
- Use master graph only for reusable logic.
- Prefer fewer long lanes over wire crossfire.
- Keep final output block close to root pins.
- Separate:
  - parameter pack
  - UV/noise flow
  - mask logic
  - edge logic
  - emissive/final output

### Acceptance criteria

- The graph screenshot is readable without zooming into every local patch.
- Wires are reduced compared to the first FirePersonErosion attempt.
- The block structure is obvious from the screenshot alone.

## Phase MAT-R6: End-to-End Retest

### Goal

Rerun `FirePersonErosion` from a clean slate after the plugin rebuild.

### Execution order

1. Delete old test Material and old test MI.
2. Rebuild a fresh master Material.
3. Run graph validator.
4. Capture graph screenshots.
5. Create MI variants.
6. Run MI sweep.
7. Capture visual probe images.
8. Decide whether to:
   - keep MI-only tuning
   - simplify graph
   - revise core logic

### Acceptance criteria

- Final master Material has no broken wiring.
- Graph screenshot is readable.
- Visual proof exists in `Downloads`.
- At least one MI variant produces a visibly correct fire-person erosion look.

## Outputs

All runtime artifacts should go outside the plugin, under `Downloads`, for example:

- `C:\\Users\\ADMIN\\Downloads\\ForgePilotMaterialProbes\\...`
- `C:\\Users\\ADMIN\\Downloads\\ForgePilot\\MaterialMutationSnapshots\\...`
- `C:\\Users\\ADMIN\\Downloads\\ForgePilot\\MutationAudit\\...`

The plugin directory should keep only:

- source
- scripts
- docs

## Immediate Next Step

Implement `MAT-R1` and `MAT-R2` first.

Without graph focus/capture and graph validation, another full Material retest will repeat the same failure mode:

- graph technically exists
- screenshot is not reviewable
- visual proof is ambiguous

After `MAT-R1` and `MAT-R2`, delete the current `FirePersonErosion` test assets and rerun the Material from scratch.
