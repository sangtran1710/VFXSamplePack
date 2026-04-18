from __future__ import annotations

from pathlib import Path

from ExternalAgent.closed_loop.builder.material_stage_builder import build_stage_from_semantic_plan
from ExternalAgent.closed_loop.builder.stage_macros import register_default_stage_macros
from ExternalAgent.closed_loop.mcp.material_tools import MaterialTools
from ExternalAgent.closed_loop.mcp.probe_tools import ProbeTools
from ExternalAgent.closed_loop.models.semantic_models import LocalValidationReport, SemanticStagePlan, StableProbePreset
from ExternalAgent.closed_loop.orchestrator.artifact_writer import ArtifactWriter
from ExternalAgent.closed_loop.orchestrator.local_validation_gates import run_pre_critic_gates
from ExternalAgent.closed_loop.orchestrator.probe_preset import capture_stage_probe, ensure_stable_probe_actor


class FakeMaterialTools(MaterialTools):
    def __init__(self, mock_image: Path, *, compile_success: bool = True, graph_valid: bool = True) -> None:
        self.mock_image = mock_image
        self.compile_success = compile_success
        self.graph_valid = graph_valid
        self.next_index = 1
        self.calls: list[tuple[str, dict]] = []
        self.comments: dict[str, list[int]] = {}

    def _index(self) -> int:
        value = self.next_index
        self.next_index += 1
        return value

    def set_material_properties(self, material_path: str, **kwargs):
        self.calls.append(("set_material_properties", {"material_path": material_path, **kwargs}))
        return {"success": True}

    def add_material_expression(self, material_path: str, expression_type: str, **kwargs):
        index = self._index()
        self.calls.append(("add_material_expression", {"material_path": material_path, "expression_type": expression_type, **kwargs, "expression_index": index}))
        return {"success": True, "expression_index": index}

    def connect_material_nodes(self, material_path: str, source_index: int, target_type: str, **kwargs):
        self.calls.append(("connect_material_nodes", {"material_path": material_path, "source_index": source_index, "target_type": target_type, **kwargs}))
        return {"success": True}

    def create_material_comment(self, material_path: str, **kwargs):
        self.calls.append(("create_material_comment", {"material_path": material_path, **kwargs}))
        self.comments[str(kwargs["comment_text"])] = []
        return {"success": True}

    def update_material_comment(self, material_path: str, **kwargs):
        self.calls.append(("update_material_comment", {"material_path": material_path, **kwargs}))
        self.comments[str(kwargs["comment_text"])] = list(kwargs.get("wrap_expression_indices", []))
        return {"success": True}

    def capture_material_graph_screenshot(self, material_path: str, **kwargs):
        self.calls.append(("capture_material_graph_screenshot", {"material_path": material_path, **kwargs}))
        return {"success": True, "output_path": str(self.mock_image)}

    def get_material_info(self, asset_path: str):
        self.calls.append(("get_material_info", {"material_path": asset_path}))
        return {
            "success": True,
            "result": {
                "parameters": {
                    "scalar": [{"name": "EmissiveIntensity", "value": 4.0}],
                    "vector": [],
                }
            },
        }

    def summarize_material_block_wiring(self, material_path: str, **kwargs):
        self.calls.append(("summarize_material_block_wiring", {"material_path": material_path, **kwargs}))
        return {"success": True, "summary": "ok"}

    def review_material_asset(self, material_path: str, **kwargs):
        self.calls.append(("review_material_asset", {"material_path": material_path, **kwargs}))
        return {"success": True}

    def compile_material_and_get_errors(self, asset_path: str):
        self.calls.append(("compile_material_and_get_errors", {"material_path": asset_path}))
        return {"success": self.compile_success, "errors": [] if self.compile_success else ["compile failed"]}

    def validate_material_graph_block(self, material_path: str, **kwargs):
        self.calls.append(("validate_material_graph_block", {"material_path": material_path, **kwargs}))
        return {"success": self.graph_valid}


class FakeProbeTools(ProbeTools):
    def __init__(self, mock_image: Path, *, diff: float = 2.5) -> None:
        self.mock_image = mock_image
        self.diff = diff
        self.spawn_calls = 0
        self.apply_calls = 0
        self.capture_calls = 0

    def spawn_material_probe_actor(self, **kwargs):
        self.spawn_calls += 1
        return {"success": True, **kwargs}

    def apply_material_to_probe_actor(self, actor_label: str, material_path: str, **kwargs):
        self.apply_calls += 1
        return {"success": True, "actor_label": actor_label, "material_path": material_path, **kwargs}

    def capture_material_visual_probe(self, **kwargs):
        self.capture_calls += 1
        return {
            "success": True,
            "actor_label": kwargs.get("actor_label"),
            "output_path": str(self.mock_image),
            "output_dir": str(self.mock_image.parent),
            "capture_mode": "scene_capture",
        }

    def compare_material_probe_captures(self, *, image_a_path: str, image_b_path: str):
        return {"success": True, "image_a_path": image_a_path, "image_b_path": image_b_path, "mean_absolute_difference": self.diff}


def _stage(stage_id: str, target_layer: str, label: str) -> SemanticStagePlan:
    return SemanticStagePlan(
        stage_id=stage_id,
        label=label,
        goal=label,
        target_layer=target_layer,
        allowed_ops=[],
        forbidden_ops=[],
        validation_rules=["core_visible"],
        expected_visual_change="visible change",
        hard_constraints=[],
        anti_patterns=[],
        stop_conditions=[],
        preferred_lanes=["emissive_visual"],
    )


def test_macro_registry_and_stage_builder() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    writer = ArtifactWriter()
    artifacts = writer.create_run(label="fast_workflow_test")
    material_tools = FakeMaterialTools(mock_image)
    probe_tools = FakeProbeTools(mock_image)
    context = {
        "artifact_writer": writer,
        "run_artifacts": artifacts,
        "material_tools": material_tools,
        "probe_tools": probe_tools,
        "stage_macro_registry": register_default_stage_macros(),
        "material_context": {},
    }
    result = build_stage_from_semantic_plan("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", _stage("stage_1_core", "core", "Core Mask"), context)
    assert result.success
    assert result.compile_ok
    assert result.comment_name == "Core Mask"
    assert len(result.expression_indices) >= 8
    assert any(call[0] == "add_material_expression" for call in material_tools.calls)
    assert any(call[0] == "create_material_comment" for call in material_tools.calls)
    assert any(call[0] == "capture_material_graph_screenshot" for call in material_tools.calls)


def test_stage_builder_stops_on_compile_failure() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    writer = ArtifactWriter()
    artifacts = writer.create_run(label="fast_workflow_compile_fail")
    material_tools = FakeMaterialTools(mock_image, compile_success=False)
    context = {
        "artifact_writer": writer,
        "run_artifacts": artifacts,
        "material_tools": material_tools,
        "probe_tools": FakeProbeTools(mock_image),
        "stage_macro_registry": register_default_stage_macros(),
        "material_context": {},
    }
    result = build_stage_from_semantic_plan("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", _stage("stage_1_core", "core", "Core Mask"), context)
    assert not result.success
    assert not result.compile_ok


def test_stable_probe_preset_reuses_actor() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    probe_tools = FakeProbeTools(mock_image)
    preset = StableProbePreset(
        preset_id="test",
        mesh_path="/Engine/BasicShapes/Sphere.Sphere",
        actor_label_template="Probe_{asset_token}",
        location=[0.0, 0.0, 0.0],
        rotation=[0.0, 0.0, 0.0],
        scale=[1.0, 1.0, 1.0],
        capture_mode_policy=["scene_capture", "mesh_thumbnail_fallback"],
    )
    context = {"probe_tools": probe_tools, "probe_preset": preset}
    label_a = ensure_stable_probe_actor("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", context)
    label_b = ensure_stable_probe_actor("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", context)
    capture = capture_stage_probe("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "stage_1_core", context)
    assert label_a == label_b
    assert probe_tools.spawn_calls == 1
    assert capture.success
    assert capture.capture_mode == "scene_capture"


def test_local_validation_gates_skip_critic_on_low_delta() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    material_tools = FakeMaterialTools(mock_image)
    probe_tools = FakeProbeTools(mock_image, diff=0.1)
    build_result = build_stage_from_semantic_plan(
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        _stage("stage_1_core", "core", "Core Mask"),
        {
            "artifact_writer": ArtifactWriter(),
            "run_artifacts": ArtifactWriter().create_run(label="gates"),
            "material_tools": material_tools,
            "probe_tools": probe_tools,
            "stage_macro_registry": register_default_stage_macros(),
            "material_context": {},
        },
    )
    current_probe = capture_stage_probe(
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        "stage_1_core",
        {"probe_tools": probe_tools},
    )
    previous_probe = current_probe.model_copy()
    report = run_pre_critic_gates(_stage("stage_1_core", "core", "Core Mask"), build_result, current_probe, previous_probe, {"material_tools": material_tools, "probe_tools": probe_tools, "probe_visual_change_threshold": 0.75})
    assert not report.hard_failures
    assert not report.should_call_critic
    assert report.failure_class_override == "no_visible_change"


def test_local_validation_gates_block_on_compile_failure() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    material_tools = FakeMaterialTools(mock_image, compile_success=False)
    build_result = build_stage_from_semantic_plan(
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        _stage("stage_1_core", "core", "Core Mask"),
        {
            "artifact_writer": ArtifactWriter(),
            "run_artifacts": ArtifactWriter().create_run(label="gates_compile"),
            "material_tools": material_tools,
            "probe_tools": FakeProbeTools(mock_image),
            "stage_macro_registry": register_default_stage_macros(),
            "material_context": {},
        },
    )
    current_probe = capture_stage_probe("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "stage_1_core", {"probe_tools": FakeProbeTools(mock_image)})
    report = run_pre_critic_gates(_stage("stage_1_core", "core", "Core Mask"), build_result, current_probe, None, {"material_tools": material_tools, "probe_tools": FakeProbeTools(mock_image)})
    assert report.hard_failures
    assert not report.should_call_critic
    assert report.failure_class_override == "failed"


def test_local_validation_gates_block_on_missing_probe() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    material_tools = FakeMaterialTools(mock_image)
    build_result = build_stage_from_semantic_plan(
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        _stage("stage_1_core", "core", "Core Mask"),
        {
            "artifact_writer": ArtifactWriter(),
            "run_artifacts": ArtifactWriter().create_run(label="gates_probe"),
            "material_tools": material_tools,
            "probe_tools": FakeProbeTools(mock_image),
            "stage_macro_registry": register_default_stage_macros(),
            "material_context": {},
        },
    )
    report = run_pre_critic_gates(
        _stage("stage_1_core", "core", "Core Mask"),
        build_result,
        {"success": False, "output_path": "", "material_path": build_result.asset_path, "capture_mode": "scene_capture"},
        None,
        {"material_tools": material_tools, "probe_tools": FakeProbeTools(mock_image)},
    )
    assert report.hard_failures
    assert not report.should_call_critic
    assert report.failure_class_override == "capture"


def test_local_validation_gates_block_on_graph_validation_failure() -> None:
    root = Path(__file__).resolve().parents[3]
    mock_image = root / "Tools" / "Validation" / "MaterialProbe" / "ice_realistic_current_v6.png"
    material_tools = FakeMaterialTools(mock_image, graph_valid=False)
    build_result = build_stage_from_semantic_plan(
        "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb",
        _stage("stage_1_core", "core", "Core Mask"),
        {
            "artifact_writer": ArtifactWriter(),
            "run_artifacts": ArtifactWriter().create_run(label="gates_graph"),
            "material_tools": material_tools,
            "probe_tools": FakeProbeTools(mock_image),
            "stage_macro_registry": register_default_stage_macros(),
            "material_context": {},
        },
    )
    current_probe = capture_stage_probe("/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "/Game/VFX/Test/M_EnergyOrb.M_EnergyOrb", "stage_1_core", {"probe_tools": FakeProbeTools(mock_image)})
    report = run_pre_critic_gates(_stage("stage_1_core", "core", "Core Mask"), build_result, current_probe, None, {"material_tools": material_tools, "probe_tools": FakeProbeTools(mock_image)})
    assert report.hard_failures
    assert not report.should_call_critic
    assert report.failure_class_override == "structural"


def run() -> None:
    test_macro_registry_and_stage_builder()
    test_stage_builder_stops_on_compile_failure()
    test_stable_probe_preset_reuses_actor()
    test_local_validation_gates_skip_critic_on_low_delta()
    test_local_validation_gates_block_on_compile_failure()
    test_local_validation_gates_block_on_missing_probe()
    test_local_validation_gates_block_on_graph_validation_failure()


if __name__ == "__main__":
    run()
    print("test_fast_visual_workflow.py passed")
