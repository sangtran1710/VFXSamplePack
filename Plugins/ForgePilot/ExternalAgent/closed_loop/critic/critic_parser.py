from __future__ import annotations

import json
from dataclasses import dataclass

from pydantic import ValidationError

from ..models.critic_models import CriticResult


class CriticParseError(ValueError):
    pass


@dataclass(slots=True)
class ParsedCriticPayload:
    raw_text: str
    parsed: CriticResult


def _strip_code_fence(text: str) -> str:
    candidate = text.strip()
    if candidate.startswith("```") and candidate.endswith("```"):
        lines = candidate.splitlines()
        if len(lines) >= 3:
            return "\n".join(lines[1:-1]).strip()
    return candidate


def parse_critic_response(raw_text: str) -> ParsedCriticPayload:
    candidate = _strip_code_fence(raw_text)
    try:
        loaded = json.loads(candidate)
    except json.JSONDecodeError as exc:
        raise CriticParseError(f"Invalid JSON: {exc}") from exc
    if not isinstance(loaded, dict):
        raise CriticParseError("Critic response must decode to a JSON object.")
    try:
        parsed = CriticResult.model_validate(loaded)
    except ValidationError as exc:
        raise CriticParseError(f"Schema validation failed: {exc}") from exc
    return ParsedCriticPayload(raw_text=raw_text, parsed=parsed)
