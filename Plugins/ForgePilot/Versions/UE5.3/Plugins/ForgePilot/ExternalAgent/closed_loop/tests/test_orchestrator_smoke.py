from __future__ import annotations

import asyncio
from pathlib import Path

from ExternalAgent.closed_loop.models.critic_models import CriticAxisScores, CriticFix, CriticResult
from ExternalAgent.closed_loop.models.orchestrator_models import LoopConfig
from ExternalAgent.closed_loop.orchestrator.artifact_writer import ArtifactWriter
from ExternalAgent.closed_loop.orchestrator.closed_loop_orchestrator import run_closed_loop


class MockMcpClient:
    def __init__(self) -> None:
        self.scalar_params = {"EmissiveIntensity": 2.0}
        self.vector_params = {"ShellTintColor": [1.0, 0.5, 0.2, 1.0]}
        self.snapshots: dict[str, dict] = {}
        self.snapshot_counter = 0

    def execute_binding(self, tool_name: str, arguments: dict) -> dict:
        if tool_name == "set_material_scalar_param":
            self.scalar_params[arguments["param_name"]] = float(arguments["default_value"])
            return {"ok": True, "result": {"param_name": arguments["param_name"], "value": self.scalar_params[arguments["param_name"]]}}
        if tool_name == "set_material_vector_param":
            self.vector_params[arguments["param_name"]] = list(arguments["default_value"])
            return {"ok": True, "result": {"param_name": arguments["param_name"], "value": self.vector_params[arguments["param_name"]]}}
        if tool_name == "add_material_expression":
            return {"ok": True, "result": {"expression_index": 99}}
        return {"ok": True, "result": {}}

    def compile_material(self, asset_path: str) -> dict:
        return {"ok": True, "compiled": True, "asset_path": asset_path}

    def readback_param(self, asset_path: str, param_name: str) -> dict:
        if param_name in self.scalar_params:
            return {"ok": True, "value": self.scalar_params[param_name], "asset_path": asset_path}
        return {"ok": True, "value": self.vector_params[param_name], "asset_path": asset_path}

    def create_snapshot(self, asset_path: str, label: str) -> str:
        snapshot_id = f"{asset_path}::{label}::{self.snapshot_counter}"
        self.snapshots[snapshot_id] = {
            "scalar": dict(self.scalar_params),
            "vector": {key: list(value) for key, value in self.vector_params.items()},
        }
        self.snapshot_counter += 1
        return snapshot_id

    def restore_snapshot(self, asset_path: str, snapshot_path: str) -> dict:
        snapshot = self.snapshots[snapshot_path]
        self.scalar_params = dict(snapshot["scalar"])
        self.vector_params = {key: list(value) for key, value in snapshot["vector"].items()}
        return {"ok": True, "asset_path": asset_path, "snapshot_path": snapshot_path}


def _critic_for_iteration(iteration_index: int, phase: str) -> CriticResult:
    if iteration_index == 0 and phase == "pre":
        return CriticResult(
            summary="too dark and too saturated",
            overall_score=0.42,
            axis_scores=CriticAxisScores({"value_match": 0.3, "color_match": 0.45, "contrast_match": 0.5}),
            deltas=[],
            top_fixes=[
                CriticFix(
                    fix_id="raise_emissive",
                    axis="value_match",
                    instruction="Increase emissive intensity on the glow scalar.",
                    rationale="The probe is too dark.",
                    priority=1,
                    confidence=0.92,
                    risk="medium",
                    target_hint="EmissiveIntensity",
                    expected_effect="Raise brightness.",
                )
            ],
            passed=False,
            critic_version="v2",
        )
    if iteration_index == 0 and phase == "post":
        return CriticResult(
            summary="improved but still not enough",
            overall_score=0.71,
            axis_scores=CriticAxisScores({"value_match": 0.74, "color_match": 0.66, "contrast_match": 0.73}),
            deltas=[],
            top_fixes=[
                CriticFix(
                    fix_id="rebalance_tint",
                    axis="color_match",
                    instruction="Reduce saturation in the shell tint color.",
                    rationale="Hue still too hot.",
                    priority=1,
                    confidence=0.88,
                    risk="low",
                    target_hint="ShellTintColor",
                    expected_effect="Cool the tint.",
                )
            ],
            passed=False,
            critic_version="v2",
        )
    if iteration_index == 1 and phase == "pre":
        return CriticResult(
            summary="second pass pre",
            overall_score=0.71,
            axis_scores=CriticAxisScores({"value_match": 0.74, "color_match": 0.66, "contrast_match": 0.73}),
            deltas=[],
            top_fixes=[
                CriticFix(
                    fix_id="rebalance_tint",
                    axis="color_match",
                    instruction="Reduce saturation in the shell tint color.",
                    rationale="Hue still too hot.",
                    priority=1,
                    confidence=0.88,
                    risk="low",
                    target_hint="ShellTintColor",
                    expected_effect="Cool the tint.",
                )
            ],
            passed=False,
            critic_version="v2",
        )
    return CriticResult(
        summary="pass",
        overall_score=0.93,
        axis_scores=CriticAxisScores({"value_match": 0.94, "color_match": 0.92, "contrast_match": 0.93}),
        deltas=[],
        top_fixes=[],
        passed=True,
        critic_version="v2",
    )


async def critic_runner(target_image: str, current_image: str, context: dict) -> CriticResult:
    del target_image, current_image
    return _critic_for_iteration(int(context["iteration_index"]), str(context["phase"]))


def planner_func(critic_result: CriticResult, asset_context: dict):
    from ExternalAgent.closed_loop.planner.patch_planner import generate_patch_plan

    return generate_patch_plan(critic_result, asset_context)


async def capture_provider(asset_path: str, iteration_index: int, phase: str, state, context: dict) -> str:
    del asset_path, iteration_index, phase, state
    return str(context["mock_image"])


async def snapshot_provider(asset_path: str, label: str, context: dict) -> str:
    return context["mcp_client"].create_snapshot(asset_path, label)


def test_mocked_two_iteration_loop() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    writer = ArtifactWriter()
    mcp_client = MockMcpClient()
    config = LoopConfig(pass_threshold=0.9, max_iterations=3, max_total_steps=6, plateau_threshold=0.01)
    loop_result = asyncio.run(
        run_closed_loop(
            target_image=str(mock_image),
            asset_path="/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
            config=config,
            initial_context={
                "mock_image": str(mock_image),
                "critic_runner": critic_runner,
                "planner_func": planner_func,
                "capture_provider": capture_provider,
                "snapshot_provider": snapshot_provider,
                "mcp_client": mcp_client,
                "artifact_writer": writer,
                "asset_context": {
                    "material_path": "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
                    "parameters": {
                        "scalar": [{"name": "EmissiveIntensity", "value": 2.0}],
                        "vector": [{"name": "ShellTintColor", "value": [1.0, 0.5, 0.2, 1.0]}],
                    },
                    "expressions": [],
                },
            },
        )
    )
    assert loop_result.status == "passed"
    assert len(loop_result.iterations) == 2
    assert loop_result.iterations[0].pre_score == 0.42
    assert loop_result.iterations[0].post_score == 0.71
    assert loop_result.iterations[1].post_score == 0.93
    assert loop_result.final_score == 0.93


def run() -> None:
    test_mocked_two_iteration_loop()


if __name__ == "__main__":
    run()
    print("test_orchestrator_smoke.py passed")
