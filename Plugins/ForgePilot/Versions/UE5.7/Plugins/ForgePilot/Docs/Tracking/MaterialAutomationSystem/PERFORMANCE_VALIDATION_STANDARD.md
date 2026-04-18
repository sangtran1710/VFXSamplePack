# Performance Validation Standard

## Purpose
Define how `ForgePilot` should think about performance and validation while building Material and Niagara outputs.

This file exists to prevent a common failure mode:
- effects that look impressive in isolation
- but are too expensive, too noisy, or too unstable for real production use

## Core Rule
Visual success is not enough.

A generated result should also be:
- technically sane
- performance-aware
- critiqueable
- safe to iterate

## Validation Layers
Performance and validation should happen at multiple levels:
1. `compile validity`
2. `structural sanity`
3. `visual read sanity`
4. `performance risk awareness`

## Material Validation
### Required checks
- compile succeeds
- root pins connect as expected
- benchmark preview capture succeeds
- critique output exists

### Performance-aware checks
Material validation should watch for:
- instruction count growth
- over-complex translucent paths
- heavy refraction use
- POM or distortion overuse
- unnecessary per-material uniqueness instead of reuse

### Material risk examples
- too many instructions for a common archetype
- costly optical features before basic read is solved
- too much dependence on noisy hacks instead of reusable logic

## Niagara Validation
### Required checks
- compile succeeds
- stack is inspectable
- expected renderers are assigned
- critique output exists when workflow critique arrives

### Performance-aware checks
Niagara validation should watch for:
- too many emitters without justification
- renderer misuse
- obvious overdraw risks
- GPU/CPU mismatch
- density or spawn behavior that will not scale

### Niagara risk examples
- too many overlapping translucent sprites
- ribbon usage without clear visual payoff
- noisy effect layers that damage readability

## Combined Effect Validation
Eventually the system should validate:
- material and Niagara read together
- focal hierarchy
- shell/core readability
- motion clarity
- effect density
- whether supporting layers overpower the main read

This should come later, after both lanes mature individually.

## Critique Expectations
Performance validation should not just report raw numbers.

It should eventually support meaningful tags such as:
- `overbuilt_graph`
- `effect_too_dense`
- `opaque_wall`
- `front_view_fade`
- `emissive_flooding`
- `shell_too_faint`
- `pattern_dominates_shell`

The goal is to make performance-awareness actionable, not merely descriptive.

## Practical Heuristic Use
Heuristics are useful, but they must remain heuristics.

Do not hard-code fragile global rules such as:
- one exact instruction count threshold for all families
- one exact emitter count limit for all effects

Instead, use:
- family-aware budgets
- target-platform awareness
- caution flags

## Target Behavior
The system should eventually:
- catch obviously risky paths early
- prefer cheaper safe strategies first
- only escalate to expensive techniques when justified by the target effect

Examples:
- do not jump into heavy translucency before proving the main read
- do not use too many emitters when layered clarity can be solved more cleanly

## What Good Looks Like
A good validation system helps the agent choose:
- safe first-pass strategies
- bounded next steps
- earlier restart when a direction becomes too expensive or too messy

## Short-Term Goal
Short-term, the system should:
- compile safely
- critique structure and read
- surface obvious performance risks

## Long-Term Goal
Long-term, the system should:
- include stronger profiling-aware critique
- compare branches not only on look but also on cost
- support production decisions, not just visual guesses

## Bottom Line
Performance validation is part of artistic quality.

If an effect looks good but is structurally or performance-wise reckless, it is not production-ready.
