from __future__ import annotations

import asyncio
import json
import os
import urllib.error
import urllib.request
from dataclasses import dataclass
from typing import Any, Protocol

from ..critic.critic_adapter import (
    CriticAdapterError,
    _extract_openrouter_output_text,
    _image_to_data_url,
    _resolve_real_api_key,
    _resolve_real_provider,
)
from ..models.semantic_models import AnalyzerOutput, StageRecipeItem, VisualCue, VisualLayerSpec
from ..orchestrator.artifact_writer import ArtifactWriter, RunArtifacts
from .analyzer_parser import AnalyzerParseError, parse_analyzer_response
from .analyzer_prompt import render_analyzer_prompt, render_analyzer_repair_prompt


@dataclass(slots=True)
class AnalyzerAdapterResponse:
    raw_text: str
    model_name: str
    raw_payload: dict | None = None
    request_payload: dict | None = None
    provider_name: str | None = None


class VisionAnalyzerAdapter(Protocol):
    async def analyze(
        self,
        *,
        prompt: str,
        reference_images: list[str],
    ) -> AnalyzerAdapterResponse:
        ...


def build_analyzer_response_schema() -> dict[str, Any]:
    non_empty_string = {"type": "string", "minLength": 1}
    return {
        "type": "object",
        "additionalProperties": False,
        "properties": {
            "archetype": non_empty_string,
            "material_family": non_empty_string,
            "layer_stack": {
                "type": "array",
                "minItems": 1,
                "items": {
                    "type": "object",
                    "additionalProperties": False,
                    "properties": {
                        "layer_id": non_empty_string,
                        "label": non_empty_string,
                        "role": {"type": "string", "enum": ["core", "body", "breakup", "depth", "rim", "color_identity", "other"]},
                        "priority": {"type": "integer", "minimum": 1, "maximum": 10},
                        "description": non_empty_string,
                        "desired_read": non_empty_string,
                        "must_preserve": {"type": "boolean"},
                    },
                    "required": ["layer_id", "label", "role", "priority", "description", "desired_read", "must_preserve"],
                },
            },
            "dominant_read": non_empty_string,
            "secondary_reads": {"type": "array", "items": non_empty_string},
            "stage_recipe": {
                "type": "array",
                "minItems": 1,
                "items": {
                    "type": "object",
                    "additionalProperties": False,
                    "properties": {
                        "stage_id": non_empty_string,
                        "label": non_empty_string,
                        "goal": non_empty_string,
                        "target_layer": non_empty_string,
                        "allowed_node_families": {"type": "array", "items": non_empty_string},
                        "forbidden_node_families": {"type": "array", "items": non_empty_string},
                        "validation_rules": {"type": "array", "items": non_empty_string},
                        "expected_visual_change": non_empty_string,
                        "stop_conditions": {"type": "array", "items": non_empty_string},
                    },
                    "required": [
                        "stage_id",
                        "label",
                        "goal",
                        "target_layer",
                        "allowed_node_families",
                        "forbidden_node_families",
                        "validation_rules",
                        "expected_visual_change",
                        "stop_conditions",
                    ],
                },
            },
            "hard_constraints": {"type": "array", "items": non_empty_string},
            "anti_patterns": {"type": "array", "items": non_empty_string},
            "failure_sensitive_constraints": {"type": "array", "items": non_empty_string},
            "visual_cues": {
                "type": "array",
                "items": {
                    "type": "object",
                    "additionalProperties": False,
                    "properties": {
                        "cue_id": non_empty_string,
                        "description": non_empty_string,
                        "importance": {"type": "string", "enum": ["critical", "major", "supporting"]},
                        "layer": {"anyOf": [{"type": "string"}, {"type": "null"}]},
                    },
                    "required": ["cue_id", "description", "importance", "layer"],
                },
            },
            "analyzer_version": non_empty_string,
            "model_name": {"anyOf": [{"type": "string"}, {"type": "null"}]},
        },
        "required": [
            "archetype",
            "material_family",
            "layer_stack",
            "dominant_read",
            "secondary_reads",
            "stage_recipe",
            "hard_constraints",
            "anti_patterns",
            "failure_sensitive_constraints",
            "visual_cues",
            "analyzer_version",
            "model_name",
        ],
    }


class RealVisionAnalyzerAdapter:
    def __init__(
        self,
        *,
        api_key: str | None = None,
        model_name: str | None = None,
        api_url: str | None = None,
    ) -> None:
        self.api_key = _resolve_real_api_key(api_key)
        if not self.api_key:
            raise CriticAdapterError("Real visual analyzer requires FORGEPILOT_VISUAL_CRITIC_API_KEY or OPENAI_API_KEY.")
        provider = _resolve_real_provider(api_key=self.api_key)
        self.provider_name = "openrouter-chat" if provider == "openrouter" else "openai-responses"
        self.model_name = model_name or os.environ.get(
            "FORGEPILOT_VISUAL_ANALYZER_MODEL",
            os.environ.get("FORGEPILOT_VISUAL_CRITIC_MODEL", "openai/gpt-4o-mini" if provider == "openrouter" else "gpt-4.1-mini"),
        )
        self.api_url = api_url or (
            os.environ.get("FORGEPILOT_OPENROUTER_CHAT_URL", "https://openrouter.ai/api/v1/chat/completions")
            if provider == "openrouter"
            else os.environ.get("FORGEPILOT_OPENAI_RESPONSES_URL", "https://api.openai.com/v1/responses")
        )
        self.provider = provider

    async def analyze(
        self,
        *,
        prompt: str,
        reference_images: list[str],
    ) -> AnalyzerAdapterResponse:
        return await asyncio.to_thread(self._analyze_sync, prompt=prompt, reference_images=reference_images)

    def _analyze_sync(self, *, prompt: str, reference_images: list[str]) -> AnalyzerAdapterResponse:
        schema = build_analyzer_response_schema()
        if self.provider == "openrouter":
            body = {
                "model": self.model_name,
                "messages": [
                    {
                        "role": "user",
                        "content": [{"type": "text", "text": prompt}]
                        + [{"type": "image_url", "image_url": {"url": _image_to_data_url(path)}} for path in reference_images],
                    }
                ],
                "response_format": {
                    "type": "json_schema",
                    "json_schema": {
                        "name": "forgepilot_visual_analyzer_result",
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
                    "HTTP-Referer": "https://local.forgepilot.semantic",
                    "X-Title": "ForgePilot Visual Analyzer",
                },
                method="POST",
            )
        else:
            body = {
                "model": self.model_name,
                "input": [
                    {
                        "role": "user",
                        "content": [{"type": "input_text", "text": prompt}]
                        + [{"type": "input_image", "image_url": _image_to_data_url(path)} for path in reference_images],
                    }
                ],
                "text": {
                    "format": {
                        "type": "json_schema",
                        "name": "forgepilot_visual_analyzer_result",
                        "schema": schema,
                        "strict": True,
                    }
                },
            }
            request = urllib.request.Request(
                self.api_url,
                data=json.dumps(body).encode("utf-8"),
                headers={"Authorization": f"Bearer {self.api_key}", "Content-Type": "application/json"},
                method="POST",
            )
        try:
            with urllib.request.urlopen(request, timeout=90) as response:
                raw_bytes = response.read()
        except urllib.error.HTTPError as exc:
            details = exc.read().decode("utf-8", errors="replace")
            raise CriticAdapterError(f"Visual analyzer request failed with HTTP {exc.code}: {details}") from exc
        except urllib.error.URLError as exc:
            raise CriticAdapterError(f"Visual analyzer request failed: {exc}") from exc

        payload = json.loads(raw_bytes.decode("utf-8"))
        if self.provider == "openrouter":
            raw_text = _extract_openrouter_output_text(payload)
        else:
            raw_text = str(payload.get("output_text") or "").strip()
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
            raise CriticAdapterError("Visual analyzer response did not contain output text.")
        return AnalyzerAdapterResponse(
            raw_text=raw_text,
            model_name=self.model_name,
            raw_payload=payload,
            request_payload=body,
            provider_name=self.provider_name,
        )


class MockVisualAnalyzerAdapter:
    def __init__(self, *, model_name: str = "mock-visual-analyzer-v1") -> None:
        self.model_name = model_name

    async def analyze(
        self,
        *,
        prompt: str,
        reference_images: list[str],
    ) -> AnalyzerAdapterResponse:
        del prompt
        reference_count = len(reference_images)
        parsed = AnalyzerOutput(
            archetype="emissive energy orb",
            material_family="emissive_energy",
            layer_stack=[
                VisualLayerSpec(layer_id="core", label="Core Energy", role="core", priority=1, description="White-hot concentrated center.", desired_read="Bright centered core.", must_preserve=True),
                VisualLayerSpec(layer_id="body", label="Orb Body", role="body", priority=2, description="Orange plasma body volume.", desired_read="Warm spherical energy mass.", must_preserve=True),
                VisualLayerSpec(layer_id="breakup", label="Filaments", role="breakup", priority=3, description="Thin bright filament lines.", desired_read="Readable plasma lines, not cloudy noise.", must_preserve=True),
                VisualLayerSpec(layer_id="depth", label="Secondary Depth", role="depth", priority=4, description="Softer turbulence behind primary filaments.", desired_read="Secondary internal complexity.", must_preserve=False),
                VisualLayerSpec(layer_id="rim", label="Rim Accent", role="rim", priority=5, description="Subtle edge accent.", desired_read="Controlled outer glow.", must_preserve=False),
                VisualLayerSpec(layer_id="color_identity", label="Heat Gradient", role="color_identity", priority=6, description="White-hot center to orange body.", desired_read="Hot center, warm body.", must_preserve=False),
            ],
            dominant_read="White-hot orb core with bright thin plasma filaments inside a warm orange spherical body.",
            secondary_reads=["Secondary internal turbulence", "Subtle edge accent"],
            stage_recipe=[
                StageRecipeItem(stage_id="stage_1_core", label="Core Mask", goal="Build a centered white-hot core.", target_layer="core", allowed_node_families=["radial_mask", "power", "multiply", "scalar_param"], forbidden_node_families=["noise", "fresnel"], validation_rules=["center_is_brightest", "core_is_readable"], expected_visual_change="Strong centered emissive core appears.", stop_conditions=["core_off_center", "core_missing"]),
                StageRecipeItem(stage_id="stage_2_filament", label="Primary Filament Breakup", goal="Create thin bright filament breakup inside the orb.", target_layer="breakup", allowed_node_families=["noise", "panner", "one_minus", "power", "clamp", "lerp", "multiply"], forbidden_node_families=["rim", "second_noise"], validation_rules=["filament_readable", "core_preserved"], expected_visual_change="Readable bright plasma lines appear inside the core.", stop_conditions=["cloudy_white_orb", "core_destroyed"]),
                StageRecipeItem(stage_id="stage_3_depth", label="Secondary Turbulence", goal="Add subtle supporting turbulence at a different scale.", target_layer="depth", allowed_node_families=["noise", "lerp", "multiply"], forbidden_node_families=["rim", "color_only"], validation_rules=["secondary_depth_visible", "filament_still_dominant"], expected_visual_change="Supporting internal depth appears behind primary breakup.", stop_conditions=["muddy_internal_read"]),
                StageRecipeItem(stage_id="stage_4_rim", label="Rim Accent", goal="Add a subtle edge glow.", target_layer="rim", allowed_node_families=["fresnel", "power", "multiply", "add"], forbidden_node_families=["noise"], validation_rules=["rim_is_subtle", "core_still_brightest"], expected_visual_change="Slight outer glow improves spherical read.", stop_conditions=["thick_outline"]),
                StageRecipeItem(stage_id="stage_5_color", label="Heat Gradient", goal="Create a white-hot center and warm orange body.", target_layer="color_identity", allowed_node_families=["lerp", "vector_param", "multiply"], forbidden_node_families=["flat_tint"], validation_rules=["white_hot_center", "orange_body_visible", "breakup_still_readable"], expected_visual_change="Clear heat gradient appears across the orb.", stop_conditions=["flat_orange_ball", "mostly_white_orb"]),
            ],
            hard_constraints=["Keep the orb spherical.", "Keep the center brightest.", "Preserve readable breakup."],
            anti_patterns=["cloudy white orb", "flat orange ball", "thick white outline"],
            failure_sensitive_constraints=["Do not solve filament failure with color-only tweaks.", "Reject structure if breakup is cloudy instead of filamentary."],
            visual_cues=[
                VisualCue(cue_id="hot_core", description="Center must read white-hot.", importance="critical", layer="core"),
                VisualCue(cue_id="thin_filaments", description="Breakup should read as thin bright filaments, not soft clouds.", importance="critical", layer="breakup"),
            ],
            analyzer_version="v1",
            model_name=self.model_name,
        )
        return AnalyzerAdapterResponse(
            raw_text=parsed.to_json(indent=2),
            model_name=self.model_name,
            raw_payload={"mock": True},
            request_payload={"reference_count": reference_count},
            provider_name="mock",
        )


class VisualAnalyzer:
    def __init__(self, *, adapter: VisionAnalyzerAdapter | None = None, mode: str | None = None) -> None:
        self.adapter = adapter or self._build_default_adapter(mode)

    def _build_default_adapter(self, mode: str | None) -> VisionAnalyzerAdapter:
        selected_mode = (mode or os.environ.get("FORGEPILOT_VISUAL_ANALYZER_MODE") or os.environ.get("FORGEPILOT_VISUAL_CRITIC_MODE") or "").strip().lower()
        if selected_mode in {"", "auto"}:
            if _resolve_real_api_key():
                return RealVisionAnalyzerAdapter()
            return MockVisualAnalyzerAdapter()
        if selected_mode == "mock":
            return MockVisualAnalyzerAdapter()
        if selected_mode in {"real", "openai", "openrouter"}:
            return RealVisionAnalyzerAdapter()
        raise CriticAdapterError(f"Unsupported visual analyzer mode: {selected_mode}")

    async def analyze_material_reference_visual(
        self,
        reference_images: list[str],
        context: dict[str, Any],
    ) -> AnalyzerOutput:
        prompt = render_analyzer_prompt(context)
        first = await self.adapter.analyze(prompt=prompt, reference_images=reference_images)
        try:
            parsed = parse_analyzer_response(first.raw_text)
            result = parsed.parsed.model_copy(update={"model_name": first.model_name})
            self._write_artifacts(context, "reference_analysis", prompt, first, result)
            return result
        except AnalyzerParseError as first_error:
            self._write_artifacts(context, "reference_analysis.initial_invalid", prompt, first, None, str(first_error))
            repair_prompt = render_analyzer_repair_prompt(prompt, first.raw_text, str(first_error))
            second = await self.adapter.analyze(prompt=repair_prompt, reference_images=reference_images)
            try:
                parsed = parse_analyzer_response(second.raw_text)
            except AnalyzerParseError as second_error:
                self._write_artifacts(context, "reference_analysis.repair_invalid", repair_prompt, second, None, str(second_error))
                raise AnalyzerParseError(
                    f"Analyzer returned invalid JSON twice. First error: {first_error}. Second error: {second_error}."
                ) from second_error
            result = parsed.parsed.model_copy(update={"model_name": second.model_name})
            self._write_artifacts(context, "reference_analysis", repair_prompt, second, result)
            return result

    def _write_artifacts(
        self,
        context: dict[str, Any],
        label: str,
        prompt: str,
        response: AnalyzerAdapterResponse,
        parsed: AnalyzerOutput | None,
        parse_error: str | None = None,
    ) -> None:
        writer = context.get("artifact_writer")
        artifacts = context.get("run_artifacts")
        if not isinstance(writer, ArtifactWriter) or not isinstance(artifacts, RunArtifacts):
            return
        metadata = {"model_name": response.model_name, "provider_name": response.provider_name}
        if parse_error is not None:
            metadata["parse_error"] = parse_error
        writer.write_analysis_artifacts(
            artifacts,
            label=label,
            raw_text=response.raw_text,
            parsed_payload=parsed or {"parse_error": True},
            prompt=prompt,
            request_payload=response.request_payload,
            response_payload=response.raw_payload,
            metadata=metadata,
        )


async def analyze_material_reference_visual(reference_images: list[str], context: dict[str, Any]) -> AnalyzerOutput:
    analyzer = VisualAnalyzer(adapter=context.get("analyzer_adapter"), mode=context.get("analyzer_mode"))
    return await analyzer.analyze_material_reference_visual(reference_images, context)
