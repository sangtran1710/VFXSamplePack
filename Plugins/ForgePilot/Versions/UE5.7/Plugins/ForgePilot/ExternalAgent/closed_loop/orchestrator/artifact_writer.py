from __future__ import annotations

import json
import shutil
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any


def _plugin_root() -> Path:
    return Path(__file__).resolve().parents[3]


@dataclass(slots=True)
class RunArtifacts:
    run_dir: Path
    probes_dir: Path
    critics_dir: Path
    plans_dir: Path
    snapshots_dir: Path
    analysis_dir: Path
    stage_probes_dir: Path
    decisions_dir: Path
    graphs_dir: Path
    gates_dir: Path


class ArtifactWriter:
    def __init__(self, output_root: str | Path | None = None) -> None:
        self.output_root = Path(output_root) if output_root else _plugin_root() / "Output" / "closed_loop"

    def create_run(self, label: str = "run") -> RunArtifacts:
        stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        run_dir = self.output_root / f"{label}_{stamp}"
        probes_dir = run_dir / "probes"
        critics_dir = run_dir / "critics"
        plans_dir = run_dir / "plans"
        snapshots_dir = run_dir / "snapshots"
        analysis_dir = run_dir / "analysis"
        stage_probes_dir = run_dir / "stage_probes"
        decisions_dir = run_dir / "decisions"
        graphs_dir = run_dir / "graphs"
        gates_dir = run_dir / "gates"
        for directory in (run_dir, probes_dir, critics_dir, plans_dir, snapshots_dir, analysis_dir, stage_probes_dir, decisions_dir, graphs_dir, gates_dir):
            directory.mkdir(parents=True, exist_ok=True)
        return RunArtifacts(
            run_dir=run_dir,
            probes_dir=probes_dir,
            critics_dir=critics_dir,
            plans_dir=plans_dir,
            snapshots_dir=snapshots_dir,
            analysis_dir=analysis_dir,
            stage_probes_dir=stage_probes_dir,
            decisions_dir=decisions_dir,
            graphs_dir=graphs_dir,
            gates_dir=gates_dir,
        )

    def write_json(self, path: str | Path, payload: Any) -> Path:
        target = Path(path)
        if not target.is_absolute():
            target = self.output_root / target
        target.parent.mkdir(parents=True, exist_ok=True)
        if hasattr(payload, "model_dump"):
            serializable = payload.model_dump(mode="json", exclude_none=True)
        else:
            serializable = payload
        target.write_text(json.dumps(serializable, ensure_ascii=False, indent=2), encoding="utf-8")
        return target

    def write_text(self, path: str | Path, text: str) -> Path:
        target = Path(path)
        if not target.is_absolute():
            target = self.output_root / target
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text, encoding="utf-8")
        return target

    def copy_file(self, source_path: str | Path, destination_path: str | Path) -> Path:
        source = Path(source_path)
        target = Path(destination_path)
        if not target.is_absolute():
            target = self.output_root / target
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        return target

    def write_critic_artifacts(
        self,
        artifacts: RunArtifacts,
        *,
        iteration_index: int,
        phase: str,
        raw_text: str,
        parsed_payload: Any,
    ) -> tuple[Path, Path]:
        prefix = f"iter_{iteration_index}_{phase}_critic"
        raw_path = self.write_text(artifacts.critics_dir / f"{prefix}.raw.txt", raw_text)
        json_path = self.write_json(artifacts.critics_dir / f"{prefix}.json", parsed_payload)
        return raw_path, json_path

    def copy_probe_image(
        self,
        artifacts: RunArtifacts,
        *,
        iteration_index: int,
        phase: str,
        source_image: str | Path,
    ) -> Path:
        destination = artifacts.probes_dir / f"iter_{iteration_index}_{phase}{Path(source_image).suffix.lower() or '.png'}"
        return self.copy_file(source_image, destination)

    def write_loop_result(self, artifacts: RunArtifacts, payload: Any) -> Path:
        return self.write_json(artifacts.run_dir / "loop_result.json", payload)

    def write_analysis_artifacts(
        self,
        artifacts: RunArtifacts,
        *,
        label: str,
        raw_text: str,
        parsed_payload: Any,
        prompt: str | None = None,
        request_payload: Any | None = None,
        response_payload: Any | None = None,
        metadata: Any | None = None,
    ) -> dict[str, Path]:
        paths: dict[str, Path] = {}
        prefix = artifacts.analysis_dir / label
        paths["raw"] = self.write_text(prefix.with_suffix(".raw.txt"), raw_text)
        paths["json"] = self.write_json(prefix.with_suffix(".json"), parsed_payload)
        if prompt is not None:
            paths["prompt"] = self.write_text(prefix.with_suffix(".prompt.txt"), prompt)
        if request_payload is not None:
            paths["request"] = self.write_json(prefix.with_suffix(".request.json"), request_payload)
        if response_payload is not None:
            paths["response"] = self.write_json(prefix.with_suffix(".response.json"), response_payload)
        if metadata is not None:
            paths["meta"] = self.write_json(prefix.with_suffix(".meta.json"), metadata)
        return paths

    def write_semantic_plan(self, artifacts: RunArtifacts, payload: Any, *, label: str = "semantic_plan") -> Path:
        return self.write_json(artifacts.plans_dir / f"{label}.json", payload)

    def copy_stage_probe_image(
        self,
        artifacts: RunArtifacts,
        *,
        stage_id: str,
        source_image: str | Path,
        attempt_index: int = 0,
    ) -> Path:
        suffix = Path(source_image).suffix.lower() or ".png"
        name = f"{stage_id}_attempt_{attempt_index}{suffix}"
        return self.copy_file(source_image, artifacts.stage_probes_dir / name)

    def write_fix_decision(
        self,
        artifacts: RunArtifacts,
        *,
        stage_id: str,
        attempt_index: int,
        payload: Any,
    ) -> Path:
        return self.write_json(artifacts.decisions_dir / f"{stage_id}_attempt_{attempt_index}.json", payload)

    def write_gate_report(
        self,
        artifacts: RunArtifacts,
        *,
        stage_id: str,
        attempt_index: int,
        payload: Any,
    ) -> Path:
        return self.write_json(artifacts.gates_dir / f"{stage_id}_attempt_{attempt_index}.json", payload)

    def copy_graph_capture(
        self,
        artifacts: RunArtifacts,
        *,
        stage_id: str,
        source_image: str | Path,
        variant: str = "logic",
    ) -> Path:
        suffix = Path(source_image).suffix.lower() or ".png"
        name = f"{stage_id}_{variant}{suffix}"
        return self.copy_file(source_image, artifacts.graphs_dir / name)
