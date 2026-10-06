#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Export data/*.yaml to exports/modules.json (deterministic)."""

from __future__ import annotations

import json
import sys
from pathlib import Path

try:
    import yaml
except ImportError as exc:
    sys.stderr.write(f"ERROR: {exc}\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))
from enum_mappings import (  # noqa: E402
    JAMMING_PIN_MAP, SIM_CARD_TYPE_MAP, ANTENNA_TYPE_MAP, MODULE_ORDER,
)

DATA_DIR = ROOT / "data"
OUT_DIR = ROOT / "exports"


def main() -> int:
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    with (DATA_DIR / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA_DIR / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}

    if [m["name"] for m in mods] != MODULE_ORDER:
        sys.stderr.write("ERROR: module order mismatch\n")
        return 2

    out = []
    for m in mods:
        at = ats[m["name"]]
        record = {
            "name": m["name"],
            "hw": dict(m["hw"]),
            "feat": dict(m["feat"]),
            "at": {
                "cband_quad_band":  at["cband_quad_band"],
                "cmic_channels":    at["cmic_channels"],
                "sidet_channels":   at["sidet_channels"],
                "supports_csclk2":  at["supports_csclk2"],
                "cfgri_default":    at["cfgri_default"],
                "chfa_pcm_support": at["chfa_pcm_support"],
                "jamming_pin":      at["jamming_pin"],
                "extra_note":       at.get("extra_note", "") or "",
                "jamming_pin_enum": JAMMING_PIN_MAP[at["jamming_pin"]].split("::")[1],
            },
        }
        out.append(record)

    path = OUT_DIR / "modules.json"
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        json.dump(out, fh, indent=2, sort_keys=False, ensure_ascii=False)
        fh.write("\n")
    print(f"Wrote {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
