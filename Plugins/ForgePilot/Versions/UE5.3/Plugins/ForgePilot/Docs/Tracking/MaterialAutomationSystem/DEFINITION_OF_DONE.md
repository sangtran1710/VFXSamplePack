# Definition Of Done

## Purpose
Define what "done" means for major `ForgePilot` improvements.

This prevents false progress where:
- code exists
- tools compile
- but the workflow is still not dependable

## Global Done Criteria
A change is not done just because it compiles.

It is done when:
1. the code builds
2. the relevant tool or workflow runs
3. the result is structured and deterministic
4. the result improves the intended lane
5. the change does not break the main workflow invariants

## Material Done Criteria
A Material capability upgrade is done when:
- it integrates into the existing workflow
- it can be triggered through the proper tools
- it produces critique output, not just a graph mutation
- it participates in decisioning and session history when relevant
- it improves a real family workflow, not just an isolated node test

Examples:
- a new image cue field is not done until it shows up in analysis output and is useful to planning or critique
- a new failure tag is not done until it appears in critique, persists in session history, and influences decision logic

## Niagara Done Criteria
A Niagara capability upgrade is done when:
- it improves emitter/system authoring in a repeatable way
- it fits into an analyze/plan/build/critique/iterate structure
- it can be validated on a real effect workflow
- it does not remain a one-off stack edit helper only

## Shared Workflow Done Criteria
A workflow/orchestration change is done when:
- build succeeds
- bridge verification succeeds
- happy path works
- at least one realistic non-happy path is also verified
- session behavior remains correct
- restart/fork behavior stays explicit and bounded

## Critique Done Criteria
A critique improvement is done when:
- the new output fields are present
- they are deterministic
- they correspond to real failure/read logic
- they are consumed downstream when appropriate

## Understand Done Criteria
An understanding upgrade is done when:
- it improves structured analysis outputs
- it stays text-first unless bounded image evidence exists
- it does not silently override persisted branch truth
- it reduces strategy errors on real family tests

## Commercially Meaningful Done
Something is commercially meaningful when it helps one of these:
- faster first pass
- better revision speed
- safer restart/fork
- better family-specific direction
- more reliable collaboration with a senior artist

If a change does not improve at least one of those, it may be interesting technically but not high priority.

## Not Done
A change is not done if:
- it only works in direct tool testing but not in workflow
- it requires manual hidden assumptions to succeed
- it produces outputs no downstream tool consumes
- it adds complexity without increasing control or quality

## Bottom Line
"Done" means the system is more reliable, more structured, and more useful in real collaboration.
Not just that the codebase has one more feature.
