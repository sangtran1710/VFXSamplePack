# Change Protocol

## Purpose
Define the exact way AI agents should make changes to `ForgePilot` so the system grows without losing structure.

## Before Any Change
The agent must answer these questions first:
1. Which lane is this change for?
- `Material`
- `Niagara`
- `Shared workflow`
- `Blueprint support`

2. Which pipeline stage does it affect?
- `Understand`
- `Plan`
- `Build`
- `Critique`
- `Iterate`

3. What existing tool or workflow output will it modify?

4. What is the expected user-visible improvement?

5. What regression risk does it introduce?

If the change cannot be placed clearly in this framework, it is probably too vague.

## Preferred Change Order
For every meaningful change:
1. update or confirm the plan
2. make the smallest viable code/doc change
3. build
4. verify through bridge/workflow
5. check that old behavior still works
6. only then continue to the next layer

## One Change Scope At A Time
Do not mix these in one patch unless truly necessary:
- new workflow tool
- new family recipe
- new critique logic
- new session behavior
- new hard rendering technique

Reason:
- mixed patches are hard to verify
- when they fail, the failure is hard to localize

## Required Verification Levels
### Code-level change
Always:
- build succeeds

### Tool change
Always:
- `list_tools`
- direct tool invocation

### Workflow change
Always:
- real workflow pass through the relevant entrypoint
- confirm result structure
- confirm non-regression of session behavior

### Critique or understanding change
Always:
- verify output fields are populated
- verify deterministic behavior
- verify new tags/reads actually flow into decision/session layers

## Required Non-Regression Checks
When touching orchestration/session:
- existing session resumes correctly
- invalid session ids fail loudly
- persisted family remains authoritative

When touching critique:
- preview-benchmark path still works
- skipped world probe is not misread as failure
- candidate tags flow into decisions and session history

When touching planning:
- family mapping still resolves correctly
- pass selection still respects persisted session family

## Restart Rule
If a branch or subsystem is already showing unstable behavior:
- do not stack new features on top
- stabilize first

## Documentation Rule
Every significant system change should update at least one of:
- target
- status
- next actions
- roadmap

If no tracking doc changes, the change is probably under-specified.

## Bottom Line
The system should be extended by small, verifiable, workflow-aware steps.
Not by piling on fixes until nobody knows which part is authoritative anymore.
