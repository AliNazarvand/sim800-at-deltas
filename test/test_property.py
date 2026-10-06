# SPDX-License-Identifier: MIT
"""Property-based test on JSON output of export_json.py (run via pytest)."""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
TOOLS = ROOT / "tools"
EXPORTS = ROOT / "exports"

REQUIRED_TOP = {"name", "hw", "feat", "at"}
REQUIRED_FEAT = {
    "has_bluetooth","has_gps","has_fm","has_audio","has_keypad","has_sd",
    "has_pcm","has_i2c","has_uart","has_usb","has_rtc","has_kpled",
    "has_rf_sync","has_antenna_gps","has_antenna_bt","has_tdd",
}


def _ensure_json():
    subprocess.run([sys.executable, str(TOOLS / "export_json.py")],
                   check=True, cwd=str(ROOT))
    return json.loads((EXPORTS / "modules.json").read_text(encoding="utf-8"))


def test_structure():
    data = _ensure_json()
    assert isinstance(data, list)
    assert len(data) == 9
    for rec in data:
        assert REQUIRED_TOP.issubset(rec.keys())
        assert REQUIRED_FEAT.issubset(rec["feat"].keys())


def test_no_negative_numbers():
    data = _ensure_json()
    for rec in data:
        for k, v in rec["hw"].items():
            if isinstance(v, int) and k != "operating_temp_min_c" and k != "operating_temp_max_c":
                assert v >= 0, f"{rec['name']}.hw.{k} negative: {v}"
        for k, v in rec["feat"].items():
            if isinstance(v, int):
                assert v >= 0


def test_json_parses():
    data = _ensure_json()
    assert json.dumps(data)  # round-trip