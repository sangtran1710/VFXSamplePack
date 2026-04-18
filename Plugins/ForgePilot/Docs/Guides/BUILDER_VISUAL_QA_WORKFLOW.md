# Single-Agent Phased Workflow

This workflow keeps all work inside one agent, but forces that agent to switch modes in order:

1. `Builder`
2. `Reviewer`
3. `Debugger`
4. `Final QA`

This gives the benefits of review and correction without losing context during agent handoff.

The user remains the final approver and final art director.

---

## Core Principle

The agent must behave as if four different specialists are checking the same task, but all within one continuous session.

The agent is not allowed to:

- build and immediately self-approve
- skip evidence
- hide uncertainty
- keep expanding scope while fixing bugs

The agent must follow this rhythm:

`build -> review -> fix -> final verify -> hand off to user`

---

## Phase Definitions

### Phase 1: Builder

Goal:

- build the first usable candidate
- focus on implementation, not approval

Allowed work:

- edit `Blueprint`, `Material`, `Niagara`, plugin tooling, and sandbox assets
- gather screenshots, logs, compile results, and runtime captures

Rules:

- do not self-praise
- do not declare the result final
- do not claim the first pass is done just because it compiles

Output:

- `Builder/summary.json`
- `Builder/builder_notes.md`
- `Builder/shots/`

### Phase 2: Reviewer

Goal:

- critique the candidate before touching it again

The Reviewer phase uses two lenses:

- `Art Visual Assistant lens`
  - target match
  - timing
  - mood
  - color read
  - silhouette and readability
- `Tester QA lens`
  - trigger path
  - wrong asset hookup
  - attach point or socket issues
  - lingering effects
  - cleanup
  - regressions

Rules:

- do not fix during review
- identify only the top issues first
- point to real evidence:
  - frames
  - logs
  - compile errors
  - runtime behavior

Output:

- `Art/art_visual_report.md`
- `QA/tester_qa_report.md`

### Phase 3: Debugger

Goal:

- fix the highest-priority issues from the Reviewer phase

Rules:

- fix `P0` and `P1` first
- do not widen scope unless the fix requires it
- do not redesign the entire solution unless the existing one is fundamentally wrong
- if art and technical feedback conflict, resolve technical blockers first, then the strongest visual mismatch

Output:

- updated assets or tooling
- optional `Builder/builder_response.md` if tradeoffs need to be documented

### Phase 4: Final QA

Goal:

- prove the result is actually ready for user review

Required checks:

- compile or build result
- runtime result
- screenshots or captures
- comparison against the target or reference
- sanity check for obvious regressions

Rules:

- if Final QA fails, the agent must go back to Phase 2 or Phase 3
- the agent must not hand off to the user without this gate

Output:

- updated `summary.json`
- final evidence bundle
- concise statement of remaining risks, if any

---

## Recommended Loop

1. Read task scope and target.
2. Phase 1: Builder creates candidate A.
3. Phase 2: Reviewer identifies the top 2-3 issues.
4. Phase 3: Debugger fixes those issues.
5. Phase 4: Final QA verifies compile, runtime, screenshots, and target fit.
6. If Final QA fails, loop back.
7. If Final QA passes, hand off to the user.

This loop should usually run for at most 2-3 rounds before escalating.

---

## Top-Issues Rule

To avoid endless self-critique, the Reviewer phase should focus on only the biggest issues first:

- `2-3 issues max` per round
- sorted by severity
- each issue must be tied to evidence

This keeps the Debugger phase focused and keeps loops short.

---

## Evidence Gates

The agent must not hand off unless all are true:

1. The effect or tool exists and runs in the intended context.
2. The pass was checked with real output.
3. The bundle contains:
   - `summary.json`
   - `builder_notes.md`
   - ordered screenshots or frames
4. The review outputs identify:
   - what changed
   - what is still weak
   - what the Debugger fixed
   - what Final QA rechecked

---

## Task-Specific Interpretation

### Niagara / VFX Task

Use the phases like this:

1. `Builder`
   - build the effect
2. `Reviewer`
   - check 6 visual criteria
3. `Debugger`
   - fix the 2-3 largest issues
4. `Final QA`
   - compile
   - runtime check
   - screenshots
   - compare reference

### Blueprint Tool Task

Use the phases like this:

1. `Builder`
   - build the logic
2. `Reviewer`
   - check readability, exposed params, and edge cases
3. `Debugger`
   - fix bugs and clean the graph
4. `Final QA`
   - compile
   - test main path
   - test simple bad input cases

---

## Bundle Structure

Recommended review bundle path in content:

`/Game/ForgePilot_Test/Reviews/<TaskName>/`

Recommended exported disk bundle:

`<workspace or export root>/Reviews/<TaskName>/<PassName>/`

Inside each bundle:

- `Builder/summary.json`
- `Builder/builder_notes.md`
- `Builder/shots/`
- `Art/art_visual_report.md`
- `QA/tester_qa_report.md`
- `Builder/builder_response.md`
  - optional

The same agent may generate all of these during different phases.

---

## Priority Scale

- `P0`
  - broken effect, wrong context, crash, or unusable result
- `P1`
  - major visual mismatch or technical failure
- `P2`
  - noticeable issue but still readable or usable
- `P3`
  - polish-only issue

---

## Definition of Done

The pass is done only when:

1. Builder produced a candidate bundle.
2. Reviewer produced art and technical findings.
3. Debugger addressed the blocking issues.
4. Final QA passed with real evidence.
5. The user approved the candidate or explicitly accepted remaining risks.

---

## Scope Guardrails

- Do not modify host-project game code under `Source/<HostProject>/*` unless explicitly approved by the user.
- Do not touch character textures unless explicitly approved by the user.
- Keep temporary work in `/Game/ForgePilot_Test` until approval.
- Move only approved assets into production folders.
