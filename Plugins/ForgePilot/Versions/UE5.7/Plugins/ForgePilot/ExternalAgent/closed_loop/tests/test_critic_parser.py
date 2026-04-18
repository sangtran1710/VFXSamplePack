from __future__ import annotations

from ExternalAgent.closed_loop.critic.critic_parser import CriticParseError, parse_critic_response


VALID_SAMPLE = """
{
  "summary": "Current material is darker and flatter than the target.",
  "overall_score": 0.62,
  "axis_scores": {
    "value_match": 0.55,
    "color_match": 0.68,
    "contrast_match": 0.63
  },
  "deltas": [
    {
      "axis": "value_match",
      "issue": "Image is too dark.",
      "observation": "Midtones are below the target.",
      "direction": "increase",
      "severity": 0.45,
      "confidence": 0.81
    }
  ],
  "top_fixes": [
    {
      "fix_id": "raise_emissive",
      "axis": "value_match",
      "instruction": "Increase emissive intensity.",
      "rationale": "The probe is too dark overall.",
      "priority": 1,
      "confidence": 0.88,
      "risk": "medium",
      "target_hint": "emissive scalar",
      "expected_effect": "Lift brightness."
    }
  ],
  "passed": false,
  "critic_version": "v2",
  "model_name": "golden-test"
}
""".strip()

INVALID_JSON_SAMPLE = '{"summary": "broken", "overall_score": 0.5'

MISSING_AXIS_SAMPLE = """
{
  "summary": "Broken schema.",
  "overall_score": 0.4,
  "deltas": [],
  "top_fixes": [],
  "passed": false,
  "critic_version": "v2"
}
""".strip()


def run() -> None:
    parsed = parse_critic_response(VALID_SAMPLE)
    assert parsed.parsed.axis_scores.root["value_match"] == 0.55

    try:
        parse_critic_response(INVALID_JSON_SAMPLE)
    except CriticParseError:
        pass
    else:
        raise AssertionError("Expected invalid JSON sample to fail.")

    try:
        parse_critic_response(MISSING_AXIS_SAMPLE)
    except CriticParseError:
        pass
    else:
        raise AssertionError("Expected missing axis_scores sample to fail.")


if __name__ == "__main__":
    run()
    print("test_critic_parser.py passed")
