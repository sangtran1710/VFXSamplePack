from __future__ import annotations

from typing import Any

from ..models.orchestrator_models import ExecutionResult, RollbackEvent
from ..models.planner_models import PatchPlan
from ..planner.binding_resolver import resolve_mcp_binding


async def rollback_plan(plan: PatchPlan, execution_results: list[ExecutionResult], mcp_client) -> list[RollbackEvent]:
    events: list[RollbackEvent] = []
    successful_step_ids = {result.step_id for result in execution_results if result.status == "success"}
    for step in reversed(plan.steps):
        if step.step_id not in successful_step_ids:
            continue
        for rollback_op in step.rollback_ops:
            try:
                binding = resolve_mcp_binding(rollback_op)
                if hasattr(mcp_client, "execute_binding"):
                    response = mcp_client.execute_binding(binding.tool_name, dict(binding.arguments))
                else:
                    response = {"ok": False, "error": "mcp_client missing execute_binding"}
                events.append(
                    RollbackEvent(
                        scope="plan",
                        success=bool(response.get("ok", True)),
                        reason="plan_rollback",
                        details={"step_id": step.step_id, "response": response},
                    )
                )
            except Exception as exc:
                events.append(
                    RollbackEvent(
                        scope="plan",
                        success=False,
                        reason="plan_rollback",
                        details={"step_id": step.step_id, "error": str(exc)},
                    )
                )
    return events


async def restore_snapshot(asset_path: str, snapshot_path: str, mcp_client, *, scope: str = "snapshot") -> RollbackEvent:
    try:
        if hasattr(mcp_client, "restore_snapshot"):
            response = mcp_client.restore_snapshot(asset_path, snapshot_path)
        else:
            response = {"ok": False, "error": "mcp_client missing restore_snapshot"}
        return RollbackEvent(
            scope=scope,
            success=bool(response.get("ok", True)),
            reason="restore_snapshot",
            details={"asset_path": asset_path, "snapshot_path": snapshot_path, "response": response},
        )
    except Exception as exc:
        return RollbackEvent(
            scope=scope,
            success=False,
            reason="restore_snapshot",
            details={"asset_path": asset_path, "snapshot_path": snapshot_path, "error": str(exc)},
        )


async def restore_best_state(
    asset_path: str,
    *,
    best_snapshot_path: str | None,
    baseline_snapshot_path: str | None,
    current_snapshot_path: str | None,
    mcp_client,
) -> RollbackEvent | None:
    if best_snapshot_path and current_snapshot_path != best_snapshot_path:
        return await restore_snapshot(asset_path, best_snapshot_path, mcp_client, scope="best_state")
    if not best_snapshot_path and baseline_snapshot_path:
        return await restore_snapshot(asset_path, baseline_snapshot_path, mcp_client, scope="best_state")
    return None
