#!/usr/bin/env python3
import argparse
import json
from pathlib import Path


def load_json(path: str) -> dict:
    return json.loads(Path(path).read_text(encoding="utf-8"))


def index_params(param_list: list[dict]) -> dict[str, dict]:
    result: dict[str, dict] = {}
    for item in param_list or []:
        name = str(item.get("name", ""))
        if name:
            result[name] = item
    return result


def comparable_param_value(item: dict) -> object:
    if "value" in item:
        return item.get("value")
    if "value_object_path" in item:
        return item.get("value_object_path")
    if "value_bytes" in item:
        return item.get("value_bytes")
    return None


def compare_param_sets(baseline: list[dict], candidate: list[dict]) -> list[dict]:
    baseline_index = index_params(baseline)
    candidate_index = index_params(candidate)
    names = sorted(set(baseline_index.keys()) | set(candidate_index.keys()))
    changes: list[dict] = []
    for name in names:
        before = baseline_index.get(name)
        after = candidate_index.get(name)
        if before is None or after is None:
            changes.append(
                {
                    "name": name,
                    "change_type": "added" if before is None else "removed",
                    "before": before,
                    "after": after,
                }
            )
            continue

        before_value = comparable_param_value(before)
        after_value = comparable_param_value(after)
        if before_value != after_value or before.get("type") != after.get("type"):
            changes.append(
                {
                    "name": name,
                    "change_type": "modified",
                    "before_type": before.get("type"),
                    "after_type": after.get("type"),
                    "before_value": before_value,
                    "after_value": after_value,
                }
            )
    return changes


def compare_renderer_state(baseline: dict, candidate: dict) -> dict:
    keys = [
        "enabled",
        "renderer_type",
        "material_path",
        "material_user_param",
        "override_materials_enabled",
        "override_materials",
        "used_materials",
        "renderer_enabled_binding",
    ]
    changes: dict[str, dict] = {}
    for key in keys:
        before = baseline.get(key)
        after = candidate.get(key)
        if before != after:
            changes[key] = {"before": before, "after": after}
    return changes


def main() -> int:
    parser = argparse.ArgumentParser(description="Compare two Niagara probe summary.json files.")
    parser.add_argument("--baseline-summary", required=True)
    parser.add_argument("--candidate-summary", required=True)
    parser.add_argument("--output", help="Optional output JSON path.")
    args = parser.parse_args()

    baseline = load_json(args.baseline_summary)
    candidate = load_json(args.candidate_summary)

    baseline_analysis = baseline.get("analysis", {})
    candidate_analysis = candidate.get("analysis", {})

    result = {
        "success": True,
        "baseline_summary": str(Path(args.baseline_summary).resolve()),
        "candidate_summary": str(Path(args.candidate_summary).resolve()),
        "baseline_mode": baseline.get("mode", ""),
        "candidate_mode": candidate.get("mode", ""),
        "default_param_changes": compare_param_sets(
            baseline_analysis.get("defaults_after_set_default", []),
            candidate_analysis.get("defaults_after_set_default", []),
        ),
        "runtime_param_changes": compare_param_sets(
            baseline_analysis.get("runtime_after_override", []),
            candidate_analysis.get("runtime_after_override", []),
        ),
        "renderer_state_changes": compare_renderer_state(
            baseline_analysis.get("renderer_details_after", baseline_analysis.get("renderer_details_before", {})),
            candidate_analysis.get("renderer_details_after", candidate_analysis.get("renderer_details_before", {})),
        ),
        "baseline_audit_count": baseline.get("steps", {}).get("audit", {}).get("count"),
        "candidate_audit_count": candidate.get("steps", {}).get("audit", {}).get("count"),
    }

    output_text = json.dumps(result, ensure_ascii=False, indent=2)
    if args.output:
        Path(args.output).write_text(output_text, encoding="utf-8")
    print(output_text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
