from __future__ import annotations

import json
from dataclasses import dataclass

from pydantic import ValidationError

from ..models.semantic_models import AnalyzerOutput


class AnalyzerParseError(ValueError):
    pass


@dataclass(slots=True)
class ParsedAnalyzerPayload:
    raw_text: str
    parsed: AnalyzerOutput


def _strip_code_fence(text: str) -> str:
    candidate = text.strip()
    if candidate.startswith("```") and candidate.endswith("```"):
        lines = candidate.splitlines()
        if len(lines) >= 3:
            return "\n".join(lines[1:-1]).strip()
    return candidate


def parse_analyzer_response(raw_text: str) -> ParsedAnalyzerPayload:
    candidate = _strip_code_fence(raw_text)
    try:
        loaded = json.loads(candidate)
    except json.JSONDecodeError as exc:
        raise AnalyzerParseError(f"Invalid JSON: {exc}") from exc
    if not isinstance(loaded, dict):
        raise AnalyzerParseError("Analyzer response must decode to a JSON object.")
    try:
        parsed = AnalyzerOutput.model_validate(loaded)
    except ValidationError as exc:
        raise AnalyzerParseError(f"Schema validation failed: {exc}") from exc
    return ParsedAnalyzerPayload(raw_text=raw_text, parsed=parsed)
