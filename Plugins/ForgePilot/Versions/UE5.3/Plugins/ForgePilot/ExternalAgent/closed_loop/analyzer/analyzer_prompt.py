from __future__ import annotations

import json
from typing import Any, Mapping

from ..models.semantic_models import AnalyzerOutput


def _sanitize(value: Any) -> Any:
    if isinstance(value, (str, int, float, bool)) or value is None:
        return value
    if isinstance(value, dict):
        return {str(key): _sanitize(item) for key, item in value.items()}
    if isinstance(value, (list, tuple, set)):
        return [_sanitize(item) for item in value]
    return str(value)


def render_analyzer_prompt(context: Mapping[str, Any] | None = None) -> str:
    safe_context = json.dumps(_sanitize(dict(context or {})), ensure_ascii=False, indent=2, sort_keys=True)
    schema = json.dumps(AnalyzerOutput.model_json_schema(), ensure_ascii=False, indent=2, sort_keys=True)
    return (
        "You are ForgePilot Visual Analyzer for Unreal Engine material reference understanding.\n"
        "You will receive one or more reference images for a target material archetype.\n"
        "Return exactly one JSON object that decomposes the reference into semantic material layers and a deterministic stage recipe.\n"
        "Do not wrap the JSON in markdown. Do not add commentary.\n\n"
        "Rules:\n"
        "- Be specific about the dominant read and anti-patterns.\n"
        "- The stage recipe must be deterministic and stage-scoped.\n"
        "- Hard constraints and anti-patterns must be actionable.\n"
        "- Do not output graph code or MCP calls here.\n\n"
        "Context:\n"
        f"{safe_context}\n\n"
        "Return JSON matching this schema:\n"
        f"{schema}\n"
    )


def render_analyzer_repair_prompt(original_prompt: str, invalid_response: str, error_message: str) -> str:
    return (
        f"{original_prompt}\n\n"
        "Your previous answer was invalid.\n"
        f"Parser error: {error_message}\n"
        "Rewrite the answer as one valid JSON object only.\n"
        "Do not omit required fields. Do not add explanations.\n"
        "Previous invalid response:\n"
        f"{invalid_response}"
    )
