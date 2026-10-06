#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Interactive helper to fill default values in data/*.yaml."""

from __future__ import annotations

import argparse
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

DEFAULTS = {0, 0.0, False, "", None}


def _coerce(current, raw):
    if isinstance(current, bool):
        return raw.strip().lower() in ("1", "true", "yes", "y")
    if isinstance(current, int) and not isinstance(current, bool):
        return int(raw)
    return raw


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--field", help="Fill one field across all modules")
    ap.add_argument("--module", help="Limit to one module")
    args = ap.parse_args()

    with (DATA / "modules.yaml").open("r", encoding="utf-8") as fh:
        doc = yaml.safe_load(fh)

    touched = 0
    for m in doc["modules"]:
        if args.module and m["name"] != args.module:
            continue
        for section_name in ("hw", "feat"):
            section = m[section_name]
            for k, v in list(section.items()):
                if args.field and k != args.field:
                    continue
                if v not in DEFAULTS:
                    continue
                prompt = f"{m['name']}.{section_name}.{k} [{v!r}] = "
                try:
                    raw = input(prompt)
                except EOFError:
                    print()
                    return 0
                if raw.strip() == "":
                    continue
                section[k] = _coerce(v, raw)
                touched += 1

    with (DATA / "modules.yaml").open("w", encoding="utf-8", newline="\n") as fh:
        yaml.safe_dump(doc, fh, sort_keys=False, allow_unicode=True)
    print(f"Updated {touched} field(s).")
    return 0


if __name__ == "__main__":
    sys.exit(main())