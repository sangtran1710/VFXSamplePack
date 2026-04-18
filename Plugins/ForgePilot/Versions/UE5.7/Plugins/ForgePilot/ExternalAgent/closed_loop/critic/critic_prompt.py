from __future__ import annotations

import json
from typing import Any, Mapping

from ..models.critic_models import CriticResult


def _sanitize_context(value: Any) -> Any:
    if isinstance(value, (str, int, float, bool)) or value is None:
        return value
    if isinstance(value, dict):
        return {str(key): _sanitize_context(item) for key, item in value.items()}
    if isinstance(value, (list, tuple, set)):
        return [_sanitize_context(item) for item in value]
    return str(value)


def render_critic_prompt(context: Mapping[str, Any] | None = None) -> str:
    safe_context = json.dumps(_sanitize_context(dict(context or {})), ensure_ascii=False, indent=2, sort_keys=True)
    schema = json.dumps(CriticResult.model_json_schema(), ensure_ascii=False, indent=2, sort_keys=True)
    return (
        "You are ForgePilot Visual Critic v2 for Unreal Engine material-only correction.\n"
        "You will receive two images in this order:\n"
        "1. Target reference image.\n"
        "2. Current material probe image.\n\n"
        "Compare the current probe against the target and return exactly one JSON object.\n"
        "Do not wrap the JSON in markdown. Do not add any commentary.\n\n"
        "Required rules:\n"
        "- axis_scores must always be present and non-empty.\n"
        "- Every delta must include a direction.\n"
        "- top_fixes must contain at most 5 items.\n"
        "- Focus on material look only. Do not discuss Niagara, animation, or camera edits.\n"
        "- Prefer concrete corrective guidance that can become material mutations.\n\n"
        "Context:\n"
        f"{safe_context}\n\n"
        "Return JSON matching this schema:\n"
        f"{schema}\n"
    )


def render_repair_prompt(original_prompt: str, invalid_response: str, error_message: str) -> str:
    return (
        f"{original_prompt}\n\n"
        "Your previous answer was invalid.\n"
        f"Parser error: {error_message}\n"
        "Rewrite the answer as a single valid JSON object only.\n"
        "Do not omit required fields. Do not add explanations.\n"
        "Previous invalid response:\n"
        f"{invalid_response}"
    )
