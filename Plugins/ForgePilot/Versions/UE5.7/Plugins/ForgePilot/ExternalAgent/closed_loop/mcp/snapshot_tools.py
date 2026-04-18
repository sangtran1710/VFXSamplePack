from __future__ import annotations

from typing import Any

from .ue_mcp_client import UeMcpClient, bool_string


class SnapshotTools:
    def __init__(self, client: UeMcpClient) -> None:
        self.client = client

    def get_material_mutation_snapshots(
        self,
        material_asset_path: str,
        *,
        max_results: int = 20,
    ) -> dict[str, Any]:
        return self.client.call_tool(
            "get_material_mutation_snapshots",
            {
                "material_asset_path": material_asset_path,
                "max_results": str(max_results),
            },
        )

    def restore_material_mutation_snapshot(
        self,
        material_asset_path: str,
        *,
        snapshot_path: str | None = None,
        use_latest: bool = True,
        reopen_editor: bool = False,
        apply_changes: bool = True,
    ) -> dict[str, Any]:
        payload: dict[str, Any] = {
            "material_asset_path": material_asset_path,
            "use_latest": bool_string(use_latest),
            "reopen_editor": bool_string(reopen_editor),
            "apply_changes": bool_string(apply_changes),
        }
        if snapshot_path is not None:
            payload["snapshot_path"] = snapshot_path
        return self.client.call_tool("restore_material_mutation_snapshot", payload)
