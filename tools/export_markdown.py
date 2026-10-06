#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Export data/*.yaml to exports/MODULES_TABLE.md (deterministic)."""

from __future__ import annotations

import sys
from pathlib import Path

try:
    import yaml
except ImportError as exc:
    sys.stderr.write(f"ERROR: {exc}\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
DATA_DIR = ROOT / "data"
OUT_DIR = ROOT / "exports"

HW_COLS = ["package_type", "pin_count", "voltage_min_mv", "voltage_max_mv", "flash_kb"]
FEAT_COLS = ["has_bluetooth", "has_gps", "has_fm", "has_audio", "gpio_count"]
AT_COLS = ["cband_quad_band", "cmic_channels", "sidet_channels", "jamming_pin", "extra_note"]


def _row(cells):
    return "| " + " | ".join(str(c) for c in cells) + " |"


def _table(title, cols, getter, mods):
    lines = [f"## {title}", ""]
    lines.append(_row(["name"] + cols))
    lines.append("|" + "|".join(["---"] * (len(cols) + 1)) + "|")
    for m in mods:
        lines.append(_row([m["name"]] + [getter(m, c) for c in cols]))
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    with (DATA_DIR / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA_DIR / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}

    def get_hw(m, c): return m["hw"].get(c, "")
    def get_feat(m, c): return m["feat"].get(c, "")
    def get_at(m, c): return ats[m["name"]].get(c, "")

    out = ["# SIMCom SIM800 Series - Modules", ""]
    out.append(_table("HardwareSpec", HW_COLS, get_hw, mods))
    out.append(_table("FeatureFlags", FEAT_COLS, get_feat, mods))
    out.append(_table("ATCommandDeltas", AT_COLS, get_at, mods))

    content = "\n".join(out)

    path = OUT_DIR / "MODULES_TABLE.md"
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write(content)
    print(f"Wrote {path.relative_to(ROOT)}")

    docs_path = ROOT / "docs" / "DATABASE_REFERENCE.md"
    docs_path.parent.mkdir(parents=True, exist_ok=True)
    with docs_path.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write(content)
    print(f"Wrote {docs_path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
