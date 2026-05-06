# Epic Niagara Sample Pack Study

This guide summarizes the first ForgePilot knowledge pass over `/Game/NiagaraExamples` in `VFXSamplePack`.

The goal is not to catalog every asset. The useful thing is to learn the production patterns: why a material or Niagara system is shaped that way, what it buys, and where it becomes a bad tradeoff.

Machine-readable knowledge lives in:

- `Plugins/ForgePilot/Knowledge/Samples/EpicNiagaraExamples/index.json`
- `Plugins/ForgePilot/Knowledge/Samples/EpicNiagaraExamples/material_mi_archetypes.json`
- `Plugins/ForgePilot/Knowledge/Samples/EpicNiagaraExamples/niagara_archetypes.json`
- `Plugins/ForgePilot/Knowledge/Samples/EpicNiagaraExamples/blueprint_runtime_contracts.json`

## Study Lens

Read every effect in layers:

1. **Material/MI:** what does the pixel shader own?
2. **Niagara:** what does timing, spawn, motion, renderer choice, and user parameter binding own?
3. **Blueprint:** what runtime state, trigger, target, socket, or gameplay decision owns the effect?

If those responsibilities blur together, the asset becomes harder to reuse.

## Material Lessons

### 1. Additive Mesh Boundary

Representative assets:

- `M_Mesh_Add`
- `MI_Boundary_TechGrid`
- `NS_Boundary`

`M_Mesh_Add` is a strong example of a math-driven VFX master material. It is `Additive`, `Unlit`, `Two Sided`, and connects only `EmissiveColor`, `Opacity`, and optional `WorldPositionOffset`.

The graph is organized into:

- `World Grid`
- `Z Axis Gradient`
- `World Aligned Screen Space Noise`
- `WPO`

It uses only one noise texture, then builds most of the look with masks, gradients, Fresnel-like shaping, panning, and exposure compensation.

Why it works:

- One texture sample keeps the material reusable and relatively cheap.
- Z-axis gradient makes the shape feel grounded or height-aware.
- Static switch for WPO lets the expensive/deforming path compile away when unused.
- Particle color keeps the Niagara contract simple.

Tradeoffs:

- 75 expressions is not beginner-friendly.
- Some parameter names are too generic for clean instance authoring.
- Two-sided additive surfaces can get expensive through overdraw.

Vietnamese learning note:

> Material nay xịn vi no dùng texture như dữ liệu, không dùng texture như toàn bộ hình ảnh. Học nó theo cụm: UV/noise -> mask -> emissive/opacity -> WPO.

### 2. Basic Sprite

Representative assets:

- `M_BasicSprite`
- `MI_BasicSprite`
- `MI_BasicSprite_Masked`
- `MI_BasicSprite_Translucent`

This is the opposite end of the complexity spectrum: 15 expressions, no texture samples, `Radius` and `Hardness` parameters, Particle Color-driven output, and `MF_ExposureCompensation`.

Why it works:

- Niagara owns color, alpha, timing, size, and variation.
- The material only supplies a soft procedural sprite shape.
- It is cheap enough to use for many small accent particles.

Tradeoffs:

- It looks generic if Niagara does not add variation.
- It has no authored internal texture breakup.

Vietnamese learning note:

> Nếu sprite chỉ cần glow mềm, đừng build graph lớn. Cho Niagara điều khiển màu/alpha/size, material chỉ cần mask tốt.

### 3. Smoke And Fire Flipbook

Representative assets:

- `M_SmokeAndFire_Sprites`
- `M_Turbulent_SmokeSubUV`
- `MI_Explosion_8x8`
- `MI_SmokePuff_8x8`
- `MI_FireBall_8x8`
- `MI_MuzzleFlash_Sphere`

This is the heavyweight material family. `M_SmokeAndFire_Sprites` has 225 expressions, 260 connections, four texture samples, SubUV logic, normals, fake lighting, depth fade, near fade, temperature controls, and exposure compensation.

Why it works:

- Smoke/fire needs authored texture motion and volumetric-looking shading.
- One parent supports explosion, fire, smoke, and muzzle variants.
- Temperature, opacity, fake directional light, and fake ambient light give art direction beyond raw color picking.
- Depth fade and near fade solve common card intersection problems.

Tradeoffs:

- It is much harder to debug.
- Atlas quality and channel packing become critical.
- It is overkill for tiny support particles.

Vietnamese learning note:

> Đây là master material lớn vì nó phục vụ cả một họ effect. Học từng lớp: atlas -> SubUV -> opacity -> color/temperature -> fake light -> depth fade.

### 4. Flare And Bright Core

Representative assets:

- `M_Flare`
- `M_BrightCore`
- `MI_ExplosionFlare`
- `MI_RocketFlareCore`

These materials act as readability accents. They are mostly additive, small, and controlled by perceptual parameters such as `Intensity`, `Exponent`, `Desaturation`, `Fade Distance`, `Temperature`, `Emissive Gain`, and `Glow Exponent`.

Why it works:

- Hero effects need a strong focal layer.
- A bright core should not force the smoke/fire body material to become brighter and noisier.
- Camera/depth fade protects against obvious clipping.

Tradeoffs:

- Easy to overuse and blow out.
- Needs background testing because additive visibility changes by scene.

Vietnamese learning note:

> Flare/core là lớp đọc nhãn. Nó giúp mắt biết đâu là điểm nóng của effect, nhưng dùng quá tay sẽ thành cháy trắng.

### 5. Distortion Support

Representative assets:

- `M_Distortion`
- `MI_Distortion`
- `M_MuzzleFlash_Distortion`

This is a support layer, not the main visual read. It exists to sell air displacement, heat, pressure, or shock without becoming the color body.

Why it works:

- Distortion can be added only where needed.
- It keeps pyro or flash materials simpler.
- It reads best when layered with brighter or smokier emitters.

Tradeoffs:

- Hard to judge in isolation.
- Translucent distortion can be expensive and can sort poorly.

Vietnamese learning note:

> Distortion là cảm giác, không phải hình chính. Nó nói “không khí đang bị uốn” chứ không nói “đây là màu của effect”.

### 6. Ribbon And Tracer Motion

Representative assets:

- `M_Ribbon_Arc`
- `M_Tracer`
- `M_TracerRibbon`
- `MI_Tracer`
- `MI_TracerRibbon`

Fast effects need readable length and direction. `M_Tracer` uses motion stretch logic and shutter/delta-time controls. `M_Ribbon_Arc` is more complex and supports arc/ribbon behavior with WPO, camera offset, and dynamic parameter use.

Why it works:

- Real fast objects are often too small to see at gameplay speed.
- Stretching and ribbon topology communicate direction and velocity.
- Camera fade and offset reduce harsh view artifacts.

Tradeoffs:

- Camera-facing tricks can fail at unusual angles.
- Ribbon materials are easier to break than simple sprite materials.

Vietnamese learning note:

> Projectile nhanh không nên là một chấm nhỏ. Material và Niagara phải giúp người xem đọc được hướng, tốc độ, và trail.

### 7. Decal Impact Marks

Representative assets:

- `M_Decal_SubUV`
- `M_Decal_Boundary`
- `M_BulletHole`
- `MI_Explosion_Decal`
- `MI_BulletHole`

Decals tell the story after the particles fade. These materials use surface-oriented logic: SubUV frame selection, `MF_DecalNormalFade`, roughness/specular/normal controls, and opacity shaping.

Why it works:

- Impact VFX needs a lingering surface mark.
- Surface projection has different requirements than airborne sprites.
- One atlas can provide multiple decal frames.

Tradeoffs:

- Decals have projection, sorting, and receiver limitations.
- Too many lingering decals can become expensive.

Vietnamese learning note:

> Particle là khoảnh khắc. Decal là dấu vết. Đừng bắt một sprite bay trong không gian làm nhiệm vụ của decal.

### 8. Mesh Overlay Status Effect

Representative assets:

- `M_Mesh_Overlay`
- `MI_Mesh_Overlay_TeslaCoil`
- `MI_Overlay_Player_Buff`
- `MI_Overlay_Weapon_DeBuff`

This material is a reusable status shell. It uses Fresnel, noise, panning, blend-in/out timing, flicker, and opacity controls to sit over existing meshes.

Why it works:

- The base character/weapon material does not need to be rewritten.
- Blueprint or Niagara can drive time/duration.
- Fresnel plus noise reads clearly on many mesh shapes.

Tradeoffs:

- Large translucent overlays can be expensive.
- The parameter surface is broad and needs discipline.
- Sorting can become visible in complex scenes.

Vietnamese learning note:

> Overlay là lớp phụ trên mesh. Base material vẫn là da/kim loại/vũ khí; overlay chỉ nói “đang bị buff, shock, debuff, highlight”.

## Niagara Lessons

### Explosion

`NS_Explosion` is layered, not monolithic. The inspected system has eight emitters: streamer source, streamers, decal/light flash, explosion body, debris, spark debris, post-process, and ground dust.

Useful pattern:

- Flash/core for first read.
- Body sprite for volume.
- Debris mesh for force.
- Dust/smoke for environment.
- Decal for evidence.

Do not copy this whole structure for tiny impacts. Copy the layer idea and scale it down.

### Smoke Plume

`NS_Smoke_Plume` shows a loop-oriented smoke pattern:

- emitter-level buoyancy
- wind direction
- base size
- spawn rate
- shape location
- vector noise
- temperature/color
- sprite renderer using `MI_SmokePuffLight_8x8`

The key lesson is time domain. Smoke feels good when each particle owns its own life curve while the emitter owns shared plume behavior.

### Muzzle Flash

`NS_MuzzleFlash` uses a burst sprite renderer plus a light renderer. It has SubUV animation, dynamic material parameters, scale color, scale sprite size, and light attributes.

The lesson: muzzle flash is a timing problem. It needs to appear immediately, illuminate the scene briefly, then disappear cleanly.

### Boundary

Boundary systems expose `User.Color` and `User.Dimensions`. That is a clean runtime contract: external code controls color and size, while material/Niagara handle the visual language.

### Pickup

Pickup effects split idle, spawn, success, and timeout into separate systems. The Blueprint owns the state machine and the systems own the visual state.

### Tesla / Status

Tesla-style effects combine:

- Blueprint target state.
- Niagara ribbon/arc or boundary visuals.
- Mesh overlay material for status read.

This is the strongest full-chain teaching pattern in the pack.

## Blueprint Lessons

Blueprint should own runtime facts:

- Which target exists.
- Which socket or attach point to use.
- Whether a pickup is available.
- Whether a weapon can fire.
- Which Niagara system should play for success, timeout, impact, or explosion.

Niagara should own visual timing:

- spawn burst/rate
- lifetime
- velocity
- renderer choice
- SubUV playback
- dynamic material parameter values

Material should own pixel logic:

- emissive
- opacity
- normal/fake lighting
- texture/atlas interpretation
- Fresnel/noise/gradient masks
- WPO/PDO where needed

## ForgePilot Reuse Rules

Use these defaults when generating future VFX:

- Start from a simple material if Niagara can provide the variation.
- Use a heavy master material only when many related variants benefit from one shared parent.
- Prefer MI overrides for art tuning before changing master material logic.
- Keep Blueprint runtime state out of material graphs.
- Use Niagara user parameters for high-level external controls such as color, dimensions, target, or material references.
- Use dynamic material parameters when per-particle timing should affect material response.
- Separate effect layers by visual responsibility: core, body, support, debris, decal, overlay.

## Common Failure Signs

- A small support particle uses a huge material master.
- A material has gameplay assumptions in parameter names or graph logic.
- A Niagara system has one emitter doing unrelated jobs.
- A Blueprint is only used to compensate for poor Niagara/material parameterization.
- Additive layers look good on black but vanish or blow out in real lighting.
- Decals or overlays are used where simple sprite timing would be enough.

## First Learning Path

For learning material specifically, study in this order:

1. `M_BasicSprite`
2. `M_Flare`
3. `M_Mesh_Add`
4. `M_Decal_SubUV`
5. `M_Tracer`
6. `M_Mesh_Overlay`
7. `M_SmokeAndFire_Sprites`

This order moves from simple output logic to full production graph complexity.
