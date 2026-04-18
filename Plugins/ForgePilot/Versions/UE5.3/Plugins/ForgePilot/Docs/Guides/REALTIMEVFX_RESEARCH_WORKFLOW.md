# RealTimeVFX Research Workflow

## Purpose

Use this guide before authoring or debugging VFX assets.

The goal is to prevent agents from guessing VFX construction details when community references, breakdowns, and production examples already exist.

## Required Research Step

Before creating, rebuilding, or deeply debugging a VFX effect, the agent should research RealTimeVFX first.

Start from:

- `https://realtimevfx.com/search?q=<topic>`

Because the RealTimeVFX search page is Discourse/JavaScript-driven, agents should prefer web search queries that target the domain directly.

Recommended query pattern:

- `site:realtimevfx.com <effect type> <engine/tool> <technique>`

Examples:

- `site:realtimevfx.com fire flipbook niagara unreal`
- `site:realtimevfx.com smoke flipbook erosion material`
- `site:realtimevfx.com dissolve edge niagara material`
- `site:realtimevfx.com projectile trail ribbon niagara`
- `site:realtimevfx.com explosion flipbook unreal niagara`

## What To Extract

Keep the research compact. Do not copy long forum posts into the answer.

Extract only:

- effect structure
- material graph technique
- Niagara module pattern
- texture/flipbook assumptions
- common failure modes
- tuning ranges when present
- links to useful threads

## When To Skip

Do not browse RealTimeVFX for pure plugin/tooling tasks that are not visual-effect authoring, such as:

- C++ bridge tool registration
- build failures
- PowerShell validation script fixes
- JSON parsing/tool schema bugs
- repo cleanup

## Decision Rule

If the task involves how an effect should look or be built visually, research RealTimeVFX first.

If the task is only about fixing ForgePilot infrastructure, do local code inspection first.

## Human And AI Authoring Split

Prefer a workflow where Niagara stays readable to the artist and the material carries the complex visual shaping.

Use Niagara primarily for:

- spawn timing
- random size
- random velocity
- speed
- drag
- acceleration and forces
- collision
- scale size over life
- lifetime
- sprite rotation
- renderer binding
- simple color/tint controls
- SubUV playback

Use materials and Dynamic Parameters for:

- fade in and fade out
- erosion and dissolve
- shape masks
- noise breakup
- distortion and refraction
- emissive shaping
- edge width and edge brightness
- material-side randomization controls
- alpha/luma thresholding

Treat Niagara Scratch Scripts as a second-phase tool. They are appropriate after the ordinary module workflow is stable, or when the requested style needs motion that stock modules cannot express clearly. Do not start a normal fire, smoke, or flipbook task by inventing Scratch Scripts.

## Output Contract

Before making assets, briefly summarize:

- RealTimeVFX references checked
- selected style from `Docs/Guides/VFX_ART_STYLE_TAXONOMY.md`
- chosen production pattern
- rejected alternatives
- expected material/Niagara contract
- art-direction assumptions from `Docs/Guides/REALISTIC_VFX_ART_DIRECTION.md`

Then implement with compile/validation gates.

## Fire Flipbook Note

For fire flipbook work, combine this guide with:

- `Docs/Guides/VFX_ART_STYLE_TAXONOMY.md`
- `Docs/Guides/REALISTIC_VFX_ART_DIRECTION.md`
- `Docs/Guides/FIRE_FLIPBOOK_SUBUV_WORKFLOW.md`
- `Docs/Guides/FIRE_FLIPBOOK_DYNAMIC_EROSION_WORKFLOW.md`
- `Knowledge/Materials/Techniques/particle_subuv_flipbook_fire_smoke.json`

Do not jump directly to `FlipBook_MotionVectors` unless the reference specifically needs motion-vector interpolation.

Current project preference for AI-assisted fire:

- Use `ParticleSubUV` plus Niagara `Sub UVAnimation` as the baseline.
- Use Dynamic Parameter-driven erosion when alpha fade needs to be reliable and art-directable.
- Let human artists tune regular Niagara modules instead of hiding look controls in hardcoded material expressions.
- Treat motion vectors, normal atlases, and custom material flipbook playback as second-pass features.
