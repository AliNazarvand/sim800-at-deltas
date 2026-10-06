#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Report default-valued fields in data/*.yaml -> exports/missing_fields.txt."""

from __future__ import annotations

import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    sys.stderr.write("ERROR: PyYAML not installed.\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
DATA = ROOT / "data"
OUT  = ROOT / "exports"

DEFAULTS = {0, 0.0, False, "", None}


def main() -> int:
    with (DATA / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}

    OUT.mkdir(parents=True, exist_ok=True)
    path = OUT / "missing_fields.txt"
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write("Missing/default-valued fields\n")
        fh.write("=============================\n\n")
        for m in mods:
            fh.write(f"--- {m['name']} ---\n")
            for section_name, section in (("hw", m["hw"]), ("feat", m["feat"])):
                for k, v in section.items():
                    if v in DEFAULTS:
                        fh.write(f"  {section_name}.{k} = {v!r}\n")
            at = ats[m["name"]]
            for k, v in at.items():
                if k == "module":
                    continue
                if v in DEFAULTS:
                    fh.write(f"  at.{k} = {v!r}\n")
            fh.write("\n")
    print(f"Wrote {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())