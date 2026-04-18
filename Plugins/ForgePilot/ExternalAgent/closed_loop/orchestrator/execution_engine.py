from __future__ import annotations

from copy import deepcopy
from typing import Any

from ..models.orchestrator_models import ExecutionResult, RollbackEvent, utc_now
from ..models.planner_models import McpBinding, PatchPlan, PatchStep
from ..planner.binding_resolver import resolve_mcp_binding


def _resolve_placeholder(path: str, result_map: dict[str, dict[str, Any]]) -> Any:
    if not path.startswith("$$result."):
        return path
    parts = path.split(".")
    if len(parts) < 3:
        return path
    step_id = parts[1]
    current: Any = result_map.get(step_id, {})
    for part in parts[2:]:
        if isinstance(current, dict):
            current = current.get(part)
        else:
            return None
    return current


def _resolve_value(value: Any, result_map: dict[str, dict[str, Any]]) -> Any:
    if isinstance(value, str):
        return _resolve_placeholder(value, result_map)
    if isinstance(value, list):
        return [_resolve_value(item, result_map) for item in value]
    if isinstance(value, dict):
        return {key: _resolve_value(item, result_map) for key, item in value.items()}
    return value


def _resolve_binding(binding: McpBinding, result_map: dict[str, dict[str, Any]]) -> McpBinding:
    return McpBinding(tool_name=binding.tool_name, arguments=_resolve_value(binding.arguments, result_map))


def _topological_steps(plan: PatchPlan) -> list[PatchStep]:
    steps = list(plan.steps)
    step_map = {step.step_id: step for step in steps}
    ordered: list[PatchStep] = []
    remaining = set(step_map.keys())
    while remaining:
        progressed = False
        for step_id in list(remaining):
            step = step_map[step_id]
            if all(dependency in {item.step_id for item in ordered} or dependency not in step_map for dependency in step.op.depends_on):
                ordered.append(step)
                remaining.remove(step_id)
                progressed = True
        if not progressed:
            for step_id in sorted(remaining):
                ordered.append(step_map[step_id])
            break
    return ordered


def _preconditions_ok(step: PatchStep, result_map: dict[str, dict[str, Any]]) -> tuple[bool, str | None]:
    for precondition in step.preconditions:
        left = _resolve_value(precondition.get("left"), result_map)
        right = _resolve_value(precondition.get("right"), result_map)
        operator = precondition.get("operator", "equals")
        if operator == "equals" and left != right:
            return False, f"precondition failed: {left!r} != {right!r}"
        if operator == "truthy" and not left:
            return False, f"precondition failed: {left!r} is not truthy"
    return True, None


def _rollback_compound_group(
    compound_group: str,
    successful_steps: list[PatchStep],
    mcp_client: Any,
    result_map: dict[str, dict[str, Any]],
) -> list[RollbackEvent]:
    events: list[RollbackEvent] = []
    for step in reversed([item for item in successful_steps if item.compound_group == compound_group]):
        for rollback_op in step.rollback_ops:
            try:
                rollback_binding = resolve_mcp_binding(rollback_op)
                resolved_binding = _resolve_binding(rollback_binding, result_map)
                if hasattr(mcp_client, "execute_binding"):
                    response = mcp_client.execute_binding(resolved_binding.tool_name, dict(resolved_binding.arguments))
                else:
                    response = {"ok": False, "error": "mcp_client missing execute_binding"}
                events.append(
                    RollbackEvent(
                        scope="step",
                        success=bool(response.get("ok", True)),
                        reason=f"compound_group:{compound_group}",
                        details={"step_id": step.step_id, "response": response},
                    )
                )
            except Exception as exc:
                events.append(
                    RollbackEvent(
                        scope="step",
                        success=False,
                        reason=f"compound_group:{compound_group}",
                        details={"step_id": step.step_id, "error": str(exc)},
                    )
                )
    return events


async def execute_plan(plan: PatchPlan, effective_risk: str, mcp_client) -> list[ExecutionResult]:
    ordered_steps = _topological_steps(plan)
    result_map: dict[str, dict[str, Any]] = {}
    execution_results: list[ExecutionResult] = []
    successful_steps: list[PatchStep] = []
    failed_steps: set[str] = set()
    max_steps = 2 if effective_risk == "conservative" else None

    for index, step in enumerate(ordered_steps, start=1):
        if max_steps is not None and index > max_steps:
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error="conservative_step_budget", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue
        if effective_risk == "conservative" and step.risk == "high":
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error="conservative_high_risk", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue
        if effective_risk == "conservative" and step.compound_group:
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error="conservative_compound_group", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue
        if effective_risk == "conservative" and step.op.resolver_tier == "fuzzy":
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error="conservative_fuzzy_resolution", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue
        if any(dependency in failed_steps for dependency in step.op.depends_on):
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error="dependency_failed", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue

        preconditions_ok, precondition_error = _preconditions_ok(step, result_map)
        if not preconditions_ok:
            failed_steps.add(step.step_id)
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="skipped", error=precondition_error, started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue

        if step.binding is None:
            failed_steps.add(step.step_id)
            execution_results.append(
                ExecutionResult(step_id=step.step_id, status="failed", error="missing_binding", started_at=utc_now(), finished_at=utc_now(), risk=step.risk)
            )
            continue

        started_at = utc_now()
        resolved_binding = _resolve_binding(step.binding, result_map)
        try:
            if hasattr(mcp_client, "execute_binding"):
                response = mcp_client.execute_binding(resolved_binding.tool_name, dict(resolved_binding.arguments))
            else:
                response = {"ok": False, "error": "mcp_client missing execute_binding"}
            ok = bool(response.get("ok", True))
            status = "success" if ok else "failed"
            if ok:
                result_payload = response.get("result", response)
                result_map[step.step_id] = deepcopy(result_payload if isinstance(result_payload, dict) else {"value": result_payload})
                successful_steps.append(step)
            else:
                failed_steps.add(step.step_id)
            execution_results.append(
                ExecutionResult(
                    step_id=step.step_id,
                    status=status,
                    tool_name=resolved_binding.tool_name,
                    request=dict(resolved_binding.arguments),
                    response=response if isinstance(response, dict) else {"value": response},
                    error=None if ok else str(response.get("error", "execution_failed")),
                    started_at=started_at,
                    finished_at=utc_now(),
                    risk=step.risk,
                )
            )
            if not ok and step.compound_group:
                rollback_events = _rollback_compound_group(step.compound_group, successful_steps, mcp_client, result_map)
                for event in rollback_events:
                    execution_results.append(
                        ExecutionResult(
                            step_id=f"{step.step_id}_rollback",
                            status="success" if event.success else "failed",
                            tool_name="rollback",
                            request={},
                            response=event.details,
                            error=None if event.success else str(event.details.get("error", "rollback_failed")),
                            started_at=utc_now(),
                            finished_at=utc_now(),
                            risk=step.risk,
                        )
                    )
        except Exception as exc:
            failed_steps.add(step.step_id)
            execution_results.append(
                ExecutionResult(
                    step_id=step.step_id,
                    status="failed",
                    tool_name=resolved_binding.tool_name,
                    request=dict(resolved_binding.arguments),
                    response={},
                    error=str(exc),
                    started_at=started_at,
                    finished_at=utc_now(),
                    risk=step.risk,
                )
            )

    return execution_results
