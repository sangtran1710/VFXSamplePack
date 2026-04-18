from __future__ import annotations

import json
from datetime import datetime, timezone
from typing import Any, Literal

from pydantic import BaseModel, ConfigDict

JsonPrimitive = str | int | float | bool | None
JsonValue = Any
RiskLevel = Literal["low", "medium", "high"]
ExecutionStatus = Literal["pending", "success", "failed", "skipped"]
ValidationStatus = Literal["passed", "warning", "critical"]


class StrictJsonModel(BaseModel):
    model_config = ConfigDict(
        extra="forbid",
        strict=True,
        validate_assignment=True,
        populate_by_name=True,
    )

    def to_json_dict(self) -> dict[str, Any]:
        return self.model_dump(mode="json", exclude_none=True)

    def to_json(self, *, indent: int = 2) -> str:
        return json.dumps(self.to_json_dict(), ensure_ascii=False, indent=indent)


def utc_now() -> datetime:
    return datetime.now(timezone.utc)
