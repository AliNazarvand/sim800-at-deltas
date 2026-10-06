# SPDX-License-Identifier: MIT
"""Determinism test for all exporters (run via pytest)."""

from __future__ import annotations

import hashlib
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
TOOLS = ROOT / "tools"
EXPORTS = ROOT / "exports"


def _run(script: str) -> None:
    subprocess.run([sys.executable, str(TOOLS / script)],
                   check=True, cwd=str(ROOT))


def _hash(path: Path) -> str:
    h = hashlib.sha256()
    h.update(path.read_bytes())
    return h.hexdigest()


def _check(script: str, output: str) -> None:
    _run(script)
    first = _hash(EXPORTS / output)
    _run(script)
    second = _hash(EXPORTS / output)
    assert first == second, f"{script} not deterministic"


def test_export_json():
    _check("export_json.py", "modules.json")


def test_export_csv():
    _check("export_csv.py", "modules.csv")


def test_export_markdown():
    _check("export_markdown.py", "MODULES_TABLE.md")


def test_export_html():
    _check("export_html.py", "index.html")


def test_no_bom_and_lf():
    for name in ("modules.json", "modules.csv", "MODULES_TABLE.md", "index.html"):
        p = EXPORTS / name
        if not p.is_file():
            continue
        data = p.read_bytes()
        assert not data.startswith(b"\xef\xbb\xbf"), f"{name} has BOM"
        assert b"\r\n" not in data, f"{name} has CRLF"