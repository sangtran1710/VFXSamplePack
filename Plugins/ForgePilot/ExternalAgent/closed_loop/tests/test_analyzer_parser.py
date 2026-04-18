from __future__ import annotations

import json

from ExternalAgent.closed_loop.analyzer.analyzer_parser import AnalyzerParseError, parse_analyzer_response


def test_analyzer_parser_accepts_valid_payload() -> None:
    payload = {
        "archetype": "emissive energy orb",
        "material_family": "emissive_energy",
        "layer_stack": [
            {
                "layer_id": "core",
                "label": "Core",
                "role": "core",
                "priority": 1,
                "description": "white-hot center",
                "desired_read": "bright center",
                "must_preserve": True,
            }
        ],
        "dominant_read": "White-hot center.",
        "secondary_reads": ["subtle rim"],
        "stage_recipe": [
            {
                "stage_id": "stage_1_core",
                "label": "Core",
                "goal": "Build the core.",
                "target_layer": "core",
                "allowed_node_families": ["radial_mask"],
                "forbidden_node_families": ["noise"],
                "validation_rules": ["center_visible"],
                "expected_visual_change": "core appears",
                "stop_conditions": ["off_center"],
            }
        ],
        "hard_constraints": ["keep center brightest"],
        "anti_patterns": ["cloudy orb"],
        "failure_sensitive_constraints": ["do not color-tune structure"],
        "visual_cues": [{"cue_id": "core", "description": "center hottest", "importance": "critical", "layer": "core"}],
        "analyzer_version": "v1",
        "model_name": "mock",
    }
    parsed = parse_analyzer_response(json.dumps(payload))
    assert parsed.parsed.archetype == "emissive energy orb"


def test_analyzer_parser_rejects_missing_stage_recipe() -> None:
    payload = {
        "archetype": "emissive energy orb",
        "material_family": "emissive_energy",
        "layer_stack": [],
        "dominant_read": "White-hot center.",
        "secondary_reads": [],
        "stage_recipe": [],
        "hard_constraints": [],
        "anti_patterns": [],
        "failure_sensitive_constraints": [],
        "visual_cues": [],
        "analyzer_version": "v1",
        "model_name": "mock",
    }
    try:
        parse_analyzer_response(json.dumps(payload))
    except AnalyzerParseError:
        return
    raise AssertionError("Expected AnalyzerParseError for empty stage recipe.")


def run() -> None:
    test_analyzer_parser_accepts_valid_payload()
    test_analyzer_parser_rejects_missing_stage_recipe()


if __name__ == "__main__":
    run()
    print("test_analyzer_parser.py passed")
