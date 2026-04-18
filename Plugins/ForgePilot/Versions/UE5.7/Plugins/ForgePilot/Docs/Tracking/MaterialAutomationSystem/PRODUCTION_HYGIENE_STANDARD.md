# Production Hygiene Standard

## Purpose
Define the minimum production hygiene expected from `ForgePilot` outputs.

This exists because "it works" is not enough for freelance or team handoff.

The output must also be:
- understandable
- organized
- reusable
- handover-ready

## Core Rule
Every generated asset should look like it belongs in a professional project, not in a temporary sandbox.

## Naming Expectations
Use clear, consistent prefixes.

Examples:
- `M_` for master materials
- `MI_` for material instances
- `NS_` for Niagara systems
- `NE_` or project-equivalent for emitters
- `T_` for textures
- `MF_` for material functions

Avoid:
- vague names
- versionless throwaway names unless explicitly test-only
- mixed naming styles in the same family

## Folder Expectations
Generated output should follow a feature-based, predictable structure.

At minimum, the system should preserve clear separation between:
- materials
- instances
- functions
- systems
- textures
- probes / tests

Test assets can live under:
- `/Game/ForgePilot_Test/...`

But production-minded structure should still be reflected even in tests.

## Reuse Expectations
The system should prefer reuse over duplication where reasonable.

Examples:
- reuse suitable master materials
- prefer parameterized instances over new masters
- prefer shared function blocks over monolithic repeated graph logic
- prefer category templates for Niagara over ad hoc emitter stacks

## Graph Hygiene Expectations
### Material
- comment blocks should be readable
- lanes should be laid out clearly
- dead/orphan nodes should be removed
- result/root placement should remain readable
- exposed parameters should have intentional names

### Niagara
- emitter naming should be readable
- stacks should be inspectable
- modules should be intentionally ordered
- renderer assignments should be obvious
- user params should not be cryptic

## Handover Expectations
Every important output should be understandable by a human artist who did not generate it.

That means:
- naming is clear
- lane/emitter intent is readable
- the structure suggests why the system was built that way
- the result is not a black box of accidental complexity

## Parameterization Expectations
Do not hard-code important art controls when they should be adjustable.

Expose parameters for things like:
- color
- intensity
- tiling
- speed
- opacity
- roughness range
- emissive strength
- distortion strength

The exact set depends on the family, but the principle is:
- likely tweak points should be parameterized

## Anti-Patterns
Avoid:
- unique master materials for every small variation
- cryptic node/param names
- unbounded graph sprawl
- too many tiny emitters when a cleaner layered structure exists
- output that compiles but is not handover-ready

## Short-Term Standard
In the short term, the system does not need perfect enterprise-scale hygiene.

But it does need:
- consistency
- readability
- reuse-minded structure
- handoff-safe naming

## Long-Term Standard
Long-term, every generated asset should be professional enough that:
- another artist can open it
- understand it
- tweak it
- and ship it without rebuilding from scratch

## Bottom Line
Production hygiene is part of the product.

If the output is visually useful but structurally messy, it still fails the commercial bar.
