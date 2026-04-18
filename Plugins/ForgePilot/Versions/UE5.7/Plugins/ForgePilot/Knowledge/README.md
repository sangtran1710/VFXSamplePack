# VFX Knowledge Base

Structured VFX knowledge for the ForgePilot AI system.
The agent reads these files before executing any VFX task to ensure correct timing, color, scale, and technique.

ForgePilot is designed to grow across multiple projects, art styles, and Unreal Engine versions. The active plugin target is UE 5.7, while UE 5.3 remains a legacy-maintenance baseline. Knowledge should stay version-aware so future projects can adapt it to newer engine versions without losing older production recipes.

## Structure

```
Knowledge/
├── Principles/          ← Universal VFX rules (timing, color theory, scale)
├── Effects/             ← Per-effect-type recipes (Fire, Smoke, Explosion...)
│   └── <Type>/
│       ├── recipe.json  ← Full Niagara setup instructions
│       ├── timing.json  ← Lifetime, spawn curves, burst patterns
│       └── materials.json ← Material graph setup
├── Materials/
│   ├── Techniques/      ← Dissolve, erosion, fresnel, panning UV...
│   └── Patterns/        ← Reusable node combinations
└── Niagara/
    ├── Modules/         ← Common module configurations
    └── Patterns/        ← Emitter archetypes and setups
```

## How the Agent Uses This

Before the normal recipe flow, the agent should identify project context, engine version, target platform, selected art style, and effect type. For visual tasks, it should also check the style guidance under `Docs/Guides/`, especially `FORGEPILOT_MULTI_PROJECT_STRATEGY.md`, `VFX_ART_STYLE_TAXONOMY.md`, and the relevant art-direction guide.

1. User provides art reference or text description
2. Agent analyzes → identifies effect type(s)
3. Agent loads relevant recipes from Knowledge/
4. Agent builds execution plan based on recipes
5. Agent executes via tool registry
6. Agent captures screenshot → compares with reference
7. Agent adjusts parameters → iterates
8. User does final polish
