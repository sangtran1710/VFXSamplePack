from __future__ import annotations

import asyncio
import base64
import json
import os
import urllib.error
import urllib.request
from dataclasses import dataclass
from pathlib import Path
from typing import Protocol

from PIL import Image, ImageStat

from ..models.critic_models import CriticAxisScores, CriticDelta, CriticFix, CriticResult


class CriticAdapterError(RuntimeError):
    pass


@dataclass(slots=True)
class CriticAdapterResponse:
    raw_text: str
    model_name: str
    raw_payload: dict | None = None
    request_payload: dict | None = None
    provider_name: str | None = None


class VisionModelAdapter(Protocol):
    async def evaluate(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        ...


def _image_to_data_url(path: str) -> str:
    suffix = Path(path).suffix.lower()
    media_type = {
        ".png": "image/png",
        ".jpg": "image/jpeg",
        ".jpeg": "image/jpeg",
        ".webp": "image/webp",
    }.get(suffix, "application/octet-stream")
    encoded = base64.b64encode(Path(path).read_bytes()).decode("ascii")
    return f"data:{media_type};base64,{encoded}"


def _resolve_real_api_key(explicit_api_key: str | None = None) -> str | None:
    return (
        explicit_api_key
        or os.environ.get("FORGEPILOT_VISUAL_CRITIC_API_KEY")
        or os.environ.get("OPENAI_API_KEY")
    )


def has_real_critic_configuration() -> bool:
    return bool(_resolve_real_api_key())


def _resolve_real_provider(explicit_provider: str | None = None, api_key: str | None = None) -> str:
    provider = (explicit_provider or os.environ.get("FORGEPILOT_VISUAL_CRITIC_PROVIDER") or "").strip().lower()
    if provider:
        return provider
    effective_key = api_key or _resolve_real_api_key()
    if effective_key and effective_key.startswith("sk-or-"):
        return "openrouter"
    return "openai"


def _extract_openai_output_text(payload: dict) -> str:
    raw_text = payload.get("output_text")
    if raw_text:
        return str(raw_text).strip()
    output = payload.get("output", [])
    chunks: list[str] = []
    if isinstance(output, list):
        for item in output:
            if not isinstance(item, dict):
                continue
            for content in item.get("content", []):
                if isinstance(content, dict) and content.get("type") == "output_text":
                    text = content.get("text")
                    if isinstance(text, str):
                        chunks.append(text)
    return "\n".join(chunks).strip()


def _extract_openrouter_output_text(payload: dict) -> str:
    choices = payload.get("choices", [])
    if not isinstance(choices, list) or not choices:
        return ""
    message = choices[0].get("message", {})
    if not isinstance(message, dict):
        return ""
    content = message.get("content")
    if isinstance(content, str):
        return content.strip()
    if isinstance(content, list):
        chunks: list[str] = []
        for item in content:
            if isinstance(item, dict):
                text = item.get("text")
                if isinstance(text, str):
                    chunks.append(text)
            elif isinstance(item, str):
                chunks.append(item)
        return "\n".join(chunks).strip()
    if isinstance(content, dict):
        return json.dumps(content, ensure_ascii=False)
    return ""


def build_critic_response_schema() -> dict:
    score_field = {
        "type": "number",
        "minimum": 0.0,
        "maximum": 1.0,
    }
    nullable_string_field = {
        "anyOf": [
            {"type": "string"},
            {"type": "null"},
        ]
    }
    return {
        "type": "object",
        "additionalProperties": False,
        "properties": {
            "summary": {"type": "string", "minLength": 1},
            "overall_score": score_field,
            "axis_scores": {
                "type": "object",
                "additionalProperties": False,
                "properties": {
                    "value_match": score_field,
                    "color_match": score_field,
                    "contrast_match": score_field,
                },
                "required": [
                    "value_match",
                    "color_match",
                    "contrast_match",
                ],
            },
            "deltas": {
                "type": "array",
                "items": {
                    "type": "object",
                    "additionalProperties": False,
                    "properties": {
                        "axis": {"type": "string", "minLength": 1},
                        "issue": {"type": "string", "minLength": 1},
                        "observation": {"type": "string", "minLength": 1},
                        "direction": {"type": "string", "enum": ["increase", "decrease", "preserve", "shift"]},
                        "severity": {
                            "anyOf": [
                                score_field,
                                {"type": "null"},
                            ]
                        },
                        "confidence": {
                            "anyOf": [
                                score_field,
                                {"type": "null"},
                            ]
                        },
                    },
                    "required": ["axis", "issue", "observation", "direction", "severity", "confidence"],
                },
            },
            "top_fixes": {
                "type": "array",
                "maxItems": 5,
                "items": {
                    "type": "object",
                    "additionalProperties": False,
                    "properties": {
                        "fix_id": {"type": "string", "minLength": 1},
                        "axis": {"type": "string", "minLength": 1},
                        "instruction": {"type": "string", "minLength": 1},
                        "rationale": {"type": "string", "minLength": 1},
                        "priority": {"type": "integer", "minimum": 1, "maximum": 5},
                        "confidence": score_field,
                        "risk": nullable_string_field,
                        "target_hint": nullable_string_field,
                        "expected_effect": nullable_string_field,
                    },
                    "required": [
                        "fix_id",
                        "axis",
                        "instruction",
                        "rationale",
                        "priority",
                        "confidence",
                        "risk",
                        "target_hint",
                        "expected_effect",
                    ],
                },
            },
            "passed": {"type": "boolean"},
            "critic_version": {"type": "string"},
            "model_name": nullable_string_field,
        },
        "required": [
            "summary",
            "overall_score",
            "axis_scores",
            "deltas",
            "top_fixes",
            "passed",
            "critic_version",
            "model_name",
        ],
    }


class RealVisionCriticAdapter:
    def __init__(
        self,
        *,
        api_key: str | None = None,
        model_name: str | None = None,
        api_url: str | None = None,
    ) -> None:
        self.api_key = _resolve_real_api_key(api_key)
        self.model_name = model_name or os.environ.get("FORGEPILOT_VISUAL_CRITIC_MODEL", "gpt-4.1-mini")
        self.api_url = api_url or os.environ.get("FORGEPILOT_OPENAI_RESPONSES_URL", "https://api.openai.com/v1/responses")
        self.provider_name = "openai-responses"
        if not self.api_key:
            raise CriticAdapterError(
                "Real vision critic requires FORGEPILOT_VISUAL_CRITIC_API_KEY or OPENAI_API_KEY."
            )

    async def evaluate(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        return await asyncio.to_thread(
            self._evaluate_sync,
            prompt=prompt,
            target_image=target_image,
            current_image=current_image,
        )

    def _evaluate_sync(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        schema = build_critic_response_schema()
        body = {
            "model": self.model_name,
            "input": [
                {
                    "role": "user",
                    "content": [
                        {"type": "input_text", "text": prompt},
                        {"type": "input_image", "image_url": _image_to_data_url(target_image)},
                        {"type": "input_image", "image_url": _image_to_data_url(current_image)},
                    ],
                }
            ],
            "text": {
                "format": {
                    "type": "json_schema",
                    "name": "forgepilot_visual_critic_result",
                    "schema": schema,
                    "strict": True,
                }
            },
        }
        request = urllib.request.Request(
            self.api_url,
            data=json.dumps(body).encode("utf-8"),
            headers={
                "Authorization": f"Bearer {self.api_key}",
                "Content-Type": "application/json",
            },
            method="POST",
        )
        try:
            with urllib.request.urlopen(request, timeout=90) as response:
                raw_bytes = response.read()
        except urllib.error.HTTPError as exc:
            details = exc.read().decode("utf-8", errors="replace")
            raise CriticAdapterError(f"OpenAI request failed with HTTP {exc.code}: {details}") from exc
        except urllib.error.URLError as exc:
            raise CriticAdapterError(f"OpenAI request failed: {exc}") from exc

        payload = json.loads(raw_bytes.decode("utf-8"))
        raw_text = payload.get("output_text")
        if not raw_text:
            output = payload.get("output", [])
            chunks: list[str] = []
            if isinstance(output, list):
                for item in output:
                    if not isinstance(item, dict):
                        continue
                    for content in item.get("content", []):
                        if isinstance(content, dict) and content.get("type") == "output_text":
                            text = content.get("text")
                            if isinstance(text, str):
                                chunks.append(text)
            raw_text = "\n".join(chunks).strip()
        if not raw_text:
            raise CriticAdapterError("OpenAI response did not contain output text.")
        return CriticAdapterResponse(
            raw_text=raw_text,
            model_name=self.model_name,
            raw_payload=payload,
            request_payload=body,
            provider_name=self.provider_name,
        )


class OpenAiResponsesCriticAdapter(RealVisionCriticAdapter):
    pass


class OpenRouterChatCriticAdapter:
    def __init__(
        self,
        *,
        api_key: str | None = None,
        model_name: str | None = None,
        api_url: str | None = None,
    ) -> None:
        self.api_key = _resolve_real_api_key(api_key)
        self.model_name = model_name or os.environ.get("FORGEPILOT_VISUAL_CRITIC_MODEL", "openai/gpt-4o-mini")
        self.api_url = api_url or os.environ.get(
            "FORGEPILOT_OPENROUTER_CHAT_URL",
            "https://openrouter.ai/api/v1/chat/completions",
        )
        self.provider_name = "openrouter-chat"
        if not self.api_key:
            raise CriticAdapterError(
                "OpenRouter critic requires FORGEPILOT_VISUAL_CRITIC_API_KEY or OPENAI_API_KEY."
            )

    async def evaluate(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        return await asyncio.to_thread(
            self._evaluate_sync,
            prompt=prompt,
            target_image=target_image,
            current_image=current_image,
        )

    def _evaluate_sync(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        schema = build_critic_response_schema()
        body = {
            "model": self.model_name,
            "messages": [
                {
                    "role": "user",
                    "content": [
                        {"type": "text", "text": prompt},
                        {"type": "image_url", "image_url": {"url": _image_to_data_url(target_image)}},
                        {"type": "image_url", "image_url": {"url": _image_to_data_url(current_image)}},
                    ],
                }
            ],
            "response_format": {
                "type": "json_schema",
                "json_schema": {
                    "name": "forgepilot_visual_critic_result",
                    "strict": True,
                    "schema": schema,
                },
            },
            "temperature": 0,
            "provider": {"require_parameters": True},
        }
        request = urllib.request.Request(
            self.api_url,
            data=json.dumps(body).encode("utf-8"),
            headers={
                "Authorization": f"Bearer {self.api_key}",
                "Content-Type": "application/json",
                "HTTP-Referer": "https://local.forgepilot.closed-loop",
                "X-Title": "ForgePilot Closed Loop Critic",
            },
            method="POST",
        )
        try:
            with urllib.request.urlopen(request, timeout=90) as response:
                raw_bytes = response.read()
        except urllib.error.HTTPError as exc:
            details = exc.read().decode("utf-8", errors="replace")
            raise CriticAdapterError(f"OpenRouter request failed with HTTP {exc.code}: {details}") from exc
        except urllib.error.URLError as exc:
            raise CriticAdapterError(f"OpenRouter request failed: {exc}") from exc

        payload = json.loads(raw_bytes.decode("utf-8"))
        raw_text = _extract_openrouter_output_text(payload)
        if not raw_text:
            raise CriticAdapterError("OpenRouter response did not contain assistant text.")
        return CriticAdapterResponse(
            raw_text=raw_text,
            model_name=self.model_name,
            raw_payload=payload,
            request_payload=body,
            provider_name=self.provider_name,
        )


class MockVisionCriticAdapter:
    def __init__(self, *, model_name: str = "mock-vision-critic-v2") -> None:
        self.model_name = model_name

    async def evaluate(
        self,
        *,
        prompt: str,
        target_image: str,
        current_image: str,
    ) -> CriticAdapterResponse:
        del prompt
        result = self._build_result(target_image=target_image, current_image=current_image)
        return CriticAdapterResponse(
            raw_text=result.to_json(indent=2),
            model_name=self.model_name,
            raw_payload={"mock": True},
            request_payload={"target_image": target_image, "current_image": current_image},
            provider_name="mock",
        )

    def _build_result(self, *, target_image: str, current_image: str) -> CriticResult:
        target_stats = self._compute_stats(target_image)
        current_stats = self._compute_stats(current_image)

        color_delta = min(
            1.0,
            abs(target_stats["avg_r"] - current_stats["avg_r"]) / 255.0
            + abs(target_stats["avg_g"] - current_stats["avg_g"]) / 255.0
            + abs(target_stats["avg_b"] - current_stats["avg_b"]) / 255.0,
        )
        value_delta = min(1.0, abs(target_stats["brightness"] - current_stats["brightness"]) / 255.0)
        contrast_delta = min(1.0, abs(target_stats["contrast"] - current_stats["contrast"]) / 128.0)

        axis_scores = CriticAxisScores(
            {
                "value_match": round(max(0.0, 1.0 - value_delta), 4),
                "color_match": round(max(0.0, 1.0 - min(1.0, color_delta / 3.0)), 4),
                "contrast_match": round(max(0.0, 1.0 - contrast_delta), 4),
            }
        )
        overall_score = round(sum(axis_scores.root.values()) / len(axis_scores.root), 4)

        deltas: list[CriticDelta] = []
        fixes: list[CriticFix] = []

        if current_stats["brightness"] < target_stats["brightness"]:
            deltas.append(
                CriticDelta(
                    axis="value_match",
                    issue="Current material reads darker than the target.",
                    observation="Probe luminance sits below the target reference average.",
                    direction="increase",
                    severity=round(value_delta, 4),
                    confidence=0.8,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="raise_emissive_or_value",
                    axis="value_match",
                    instruction="Increase emissive or brighten the dominant value-driving scalar path.",
                    rationale="The current probe under-shoots the target brightness.",
                    priority=1,
                    confidence=0.8,
                    risk="medium",
                    target_hint="emissive or base value scalar lane",
                    expected_effect="Lift the overall value toward the reference.",
                )
            )
        elif current_stats["brightness"] > target_stats["brightness"]:
            deltas.append(
                CriticDelta(
                    axis="value_match",
                    issue="Current material reads brighter than the target.",
                    observation="Probe luminance exceeds the target reference average.",
                    direction="decrease",
                    severity=round(value_delta, 4),
                    confidence=0.8,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="reduce_emissive_or_value",
                    axis="value_match",
                    instruction="Reduce emissive intensity or darken the dominant value-driving scalar path.",
                    rationale="The current probe overshoots the target brightness.",
                    priority=1,
                    confidence=0.8,
                    risk="medium",
                    target_hint="emissive or base value scalar lane",
                    expected_effect="Lower the overall value toward the reference.",
                )
            )

        if current_stats["saturation"] < target_stats["saturation"]:
            deltas.append(
                CriticDelta(
                    axis="color_match",
                    issue="Current material is less saturated than the target.",
                    observation="Probe color variation is flatter than the reference.",
                    direction="increase",
                    severity=round(abs(target_stats["saturation"] - current_stats["saturation"]) / 255.0, 4),
                    confidence=0.7,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="increase_color_separation",
                    axis="color_match",
                    instruction="Increase color saturation or strengthen the color-driving vector parameter.",
                    rationale="The current probe reads flatter than the target.",
                    priority=2,
                    confidence=0.7,
                    risk="low",
                    target_hint="vector parameter or texture color lane",
                    expected_effect="Push hue separation closer to the target.",
                )
            )
        elif current_stats["saturation"] > target_stats["saturation"]:
            deltas.append(
                CriticDelta(
                    axis="color_match",
                    issue="Current material is more saturated than the target.",
                    observation="Probe color separation is stronger than the reference.",
                    direction="decrease",
                    severity=round(abs(target_stats["saturation"] - current_stats["saturation"]) / 255.0, 4),
                    confidence=0.7,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="reduce_color_separation",
                    axis="color_match",
                    instruction="Reduce saturation or soften the color-driving vector parameter.",
                    rationale="The current probe color reads hotter than the target.",
                    priority=2,
                    confidence=0.7,
                    risk="low",
                    target_hint="vector parameter or texture color lane",
                    expected_effect="Bring color intensity closer to the target.",
                )
            )

        if current_stats["contrast"] < target_stats["contrast"]:
            deltas.append(
                CriticDelta(
                    axis="contrast_match",
                    issue="Current material contrast is flatter than the target.",
                    observation="Probe tonal spread is narrower than the reference image.",
                    direction="increase",
                    severity=round(contrast_delta, 4),
                    confidence=0.72,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="increase_contrast_shape",
                    axis="contrast_match",
                    instruction="Increase contrast in the mask or multiply chain that shapes the main highlight/shadow split.",
                    rationale="The current probe does not separate dark and bright regions strongly enough.",
                    priority=3,
                    confidence=0.72,
                    risk="medium",
                    target_hint="mask or multiply chain",
                    expected_effect="Improve tonal separation toward the reference.",
                )
            )
        elif current_stats["contrast"] > target_stats["contrast"]:
            deltas.append(
                CriticDelta(
                    axis="contrast_match",
                    issue="Current material contrast is harsher than the target.",
                    observation="Probe tonal spread is wider than the reference image.",
                    direction="decrease",
                    severity=round(contrast_delta, 4),
                    confidence=0.72,
                )
            )
            fixes.append(
                CriticFix(
                    fix_id="soften_contrast_shape",
                    axis="contrast_match",
                    instruction="Soften the contrast in the mask or multiply chain that shapes the main value split.",
                    rationale="The current probe exaggerates bright-dark separation.",
                    priority=3,
                    confidence=0.72,
                    risk="medium",
                    target_hint="mask or multiply chain",
                    expected_effect="Reduce tonal separation toward the reference.",
                )
            )

        if not deltas:
            deltas.append(
                CriticDelta(
                    axis="value_match",
                    issue="Current material is already close to the target.",
                    observation="The target and current probe statistics are closely aligned.",
                    direction="preserve",
                    severity=0.0,
                    confidence=0.9,
                )
            )

        return CriticResult(
            summary="Deterministic mock critic comparison based on image statistics.",
            overall_score=overall_score,
            axis_scores=axis_scores,
            deltas=deltas,
            top_fixes=fixes[:5],
            passed=overall_score >= 0.9,
            critic_version="v2",
            model_name=self.model_name,
        )

    def _compute_stats(self, image_path: str) -> dict[str, float]:
        with Image.open(image_path) as image:
            rgb = image.convert("RGB")
            stat = ImageStat.Stat(rgb)
            mean_r, mean_g, mean_b = stat.mean
            brightness = (mean_r + mean_g + mean_b) / 3.0
            contrast = sum(stat.stddev) / 3.0
            saturation = max(mean_r, mean_g, mean_b) - min(mean_r, mean_g, mean_b)
            return {
                "avg_r": mean_r,
                "avg_g": mean_g,
                "avg_b": mean_b,
                "brightness": brightness,
                "contrast": contrast,
                "saturation": saturation,
            }


def build_default_adapter(mode: str | None = None) -> VisionModelAdapter:
    selected_mode = (mode or os.environ.get("FORGEPILOT_VISUAL_CRITIC_MODE") or "").strip().lower()
    provider = _resolve_real_provider()
    if selected_mode in {"", "auto"}:
        if has_real_critic_configuration():
            if provider == "openrouter":
                return OpenRouterChatCriticAdapter()
            return RealVisionCriticAdapter()
        return MockVisionCriticAdapter()
    if selected_mode == "mock":
        return MockVisionCriticAdapter()
    if selected_mode == "openrouter":
        return OpenRouterChatCriticAdapter()
    if selected_mode in {"openai", "real"}:
        if provider == "openrouter":
            return OpenRouterChatCriticAdapter()
        return RealVisionCriticAdapter()
    raise CriticAdapterError(f"Unsupported visual critic mode: {selected_mode}")
