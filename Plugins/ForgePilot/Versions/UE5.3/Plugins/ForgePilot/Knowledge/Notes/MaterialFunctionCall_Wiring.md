# MaterialFunctionCall Wiring

## Problem

Some Material Functions are easy to miswire when a `MaterialFunctionCall` node is connected by raw input index alone.

In the burst-noise energy test case, the `HueShift` function looked correct in metadata, but the call-site wiring produced the wrong visual result when connected by assumed input order. The material still compiled, but the color branch collapsed into a dull grayscale/yellow look instead of the intended dynamic hue-shifted energy.

## Reliable Rule

For `MaterialFunctionCall` nodes, prefer wiring by input **name** instead of by guessed input **index** whenever possible.

Use:

- `connect_material_nodes.target_input_name`

instead of:

- `connect_material_nodes.target_input_index`

## Why

- Function metadata and perceived pin order are easy to misread during automation.
- A wrong connection may still compile, so the bug can hide as a visual mismatch instead of a hard failure.
- Pin-name wiring is more robust for reusable material automation.
- Call-site pin labels may include type suffixes such as `(S)` or `(V3)`. The tool should normalize those suffixes so `Hue Shift Percentage` can still match `Hue Shift Percentage (S)`.

## Example

For the engine `HueShift` material function:

- Connect the hue phase scalar to the input named `Hue Shift Percentage`
- Connect the base color/vector to the input named `Texture`

## Workflow Guidance

When building procedural materials that use Material Functions:

1. Query function metadata with `get_material_function_info`
2. Read the exposed input names
3. Connect the call node by `target_input_name`
4. Verify with a small control material before scaling up to a larger graph

## Reusable Lesson

This pattern applies beyond `HueShift`.

Any automation that instantiates:

- engine material functions
- custom studio material functions
- nested function-based master materials

should prefer **pin-name wiring** for safer graph construction.
