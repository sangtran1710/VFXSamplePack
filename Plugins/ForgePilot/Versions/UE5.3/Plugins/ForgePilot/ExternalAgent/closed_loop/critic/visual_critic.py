from __future__ import annotations

from typing import Any

from ..models.critic_models import CriticResult
from ..orchestrator.artifact_writer import ArtifactWriter, RunArtifacts
from .critic_adapter import CriticAdapterResponse, VisionModelAdapter, build_default_adapter
from .critic_parser import CriticParseError, parse_critic_response
from .critic_prompt import render_critic_prompt, render_repair_prompt


class VisualCritic:
    def __init__(self, *, adapter: VisionModelAdapter | None = None, mode: str | None = None) -> None:
        self.adapter = adapter or build_default_adapter(mode)

    async def evaluate_visual(
        self,
        target_image: str,
        current_image: str,
        context: dict[str, Any],
    ) -> CriticResult:
        prompt = render_critic_prompt(context)
        first_response = await self.adapter.evaluate(
            prompt=prompt,
            target_image=target_image,
            current_image=current_image,
        )
        try:
            parsed = parse_critic_response(first_response.raw_text)
            result = parsed.parsed.model_copy(update={"model_name": first_response.model_name})
            self._write_attempt_artifacts(context=context, response=first_response, label=None, prompt=prompt)
            self._write_artifacts(context=context, raw_text=first_response.raw_text, parsed=result)
            return result
        except CriticParseError as first_error:
            self._write_attempt_artifacts(
                context=context,
                response=first_response,
                label="initial_invalid",
                prompt=prompt,
                parse_error=str(first_error),
            )
            repair_prompt = render_repair_prompt(prompt, first_response.raw_text, str(first_error))
            second_response = await self.adapter.evaluate(
                prompt=repair_prompt,
                target_image=target_image,
                current_image=current_image,
            )
            try:
                parsed = parse_critic_response(second_response.raw_text)
            except CriticParseError as second_error:
                self._write_attempt_artifacts(
                    context=context,
                    response=second_response,
                    label="repair_invalid",
                    prompt=repair_prompt,
                    parse_error=str(second_error),
                )
                self._write_artifacts(context=context, raw_text=second_response.raw_text, parsed=None)
                raise CriticParseError(
                    f"Critic returned invalid JSON twice. First error: {first_error}. Second error: {second_error}."
                ) from second_error
            self._write_attempt_artifacts(context=context, response=second_response, label="repair", prompt=repair_prompt)
            result = parsed.parsed.model_copy(update={"model_name": second_response.model_name})
            self._write_artifacts(context=context, raw_text=second_response.raw_text, parsed=result)
            return result

    def _write_artifacts(
        self,
        *,
        context: dict[str, Any],
        raw_text: str,
        parsed: CriticResult | None,
    ) -> None:
        artifact_writer = context.get("artifact_writer")
        artifacts = context.get("run_artifacts")
        if not isinstance(artifact_writer, ArtifactWriter) or not isinstance(artifacts, RunArtifacts):
            return
        iteration_index = int(context.get("iteration_index", 0))
        phase = str(context.get("phase", "pre"))
        payload = parsed if parsed is not None else {"parse_error": True}
        artifact_writer.write_critic_artifacts(
            artifacts,
            iteration_index=iteration_index,
            phase=phase,
            raw_text=raw_text,
            parsed_payload=payload,
        )

    def _write_attempt_artifacts(
        self,
        *,
        context: dict[str, Any],
        response: CriticAdapterResponse,
        label: str | None,
        prompt: str,
        parse_error: str | None = None,
    ) -> None:
        artifact_writer = context.get("artifact_writer")
        artifacts = context.get("run_artifacts")
        if not isinstance(artifact_writer, ArtifactWriter) or not isinstance(artifacts, RunArtifacts):
            return
        iteration_index = int(context.get("iteration_index", 0))
        phase = str(context.get("phase", "pre"))
        suffix = "" if label is None else f".{label}"
        prefix = f"iter_{iteration_index}_{phase}_critic{suffix}"
        artifact_writer.write_text(artifacts.critics_dir / f"{prefix}.prompt.txt", prompt)
        artifact_writer.write_text(artifacts.critics_dir / f"{prefix}.raw.txt", response.raw_text)
        if response.request_payload is not None:
            artifact_writer.write_json(artifacts.critics_dir / f"{prefix}.request.json", response.request_payload)
        if response.raw_payload is not None:
            artifact_writer.write_json(artifacts.critics_dir / f"{prefix}.response.json", response.raw_payload)
        metadata = {
            "model_name": response.model_name,
            "provider_name": response.provider_name,
        }
        if parse_error is not None:
            metadata["parse_error"] = parse_error
        artifact_writer.write_json(artifacts.critics_dir / f"{prefix}.meta.json", metadata)


async def evaluate_visual(
    target_image: str,
    current_image: str,
    context: dict[str, Any],
) -> CriticResult:
    critic = VisualCritic(adapter=context.get("critic_adapter"), mode=context.get("critic_mode"))
    return await critic.evaluate_visual(target_image=target_image, current_image=current_image, context=context)
