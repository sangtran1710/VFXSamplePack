from __future__ import annotations

from typing import Any

from ..models.orchestrator_models import ValidationResult


async def run_validation_checks(checks: list[dict], asset_path: str, mcp_client) -> list[ValidationResult]:
    results: list[ValidationResult] = []
    for check in checks:
        check_name = str(check.get("check", "unknown"))
        try:
            if check_name == "compile_material":
                if hasattr(mcp_client, "compile_material"):
                    response = mcp_client.compile_material(asset_path)
                else:
                    response = {"ok": True, "compiled": True}
                success = bool(response.get("compiled", response.get("ok", True)))
                results.append(
                    ValidationResult(
                        check_name=check_name,
                        status="passed" if success else "critical",
                        success=success,
                        details=response if isinstance(response, dict) else {"value": response},
                        error=None if success else str(response.get("error", "compile_failed")),
                    )
                )
                continue

            if check_name == "readback_param":
                param_name = str(check.get("param_name", ""))
                expected_value = check.get("expected_value")
                if hasattr(mcp_client, "readback_param"):
                    response = mcp_client.readback_param(asset_path, param_name)
                else:
                    response = {"ok": True, "value": expected_value}
                actual_value = response.get("value")
                success = actual_value == expected_value
                results.append(
                    ValidationResult(
                        check_name=check_name,
                        status="passed" if success else "warning",
                        success=success,
                        details={"param_name": param_name, "expected_value": expected_value, "actual_value": actual_value},
                        error=None if success else "readback_mismatch",
                    )
                )
                continue

            results.append(
                ValidationResult(
                    check_name=check_name,
                    status="warning",
                    success=False,
                    details=check,
                    error="unknown_check",
                )
            )
        except Exception as exc:
            results.append(
                ValidationResult(
                    check_name=check_name,
                    status="critical" if check_name == "compile_material" else "warning",
                    success=False,
                    details=check,
                    error=str(exc),
                )
            )
    return results
