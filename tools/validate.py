#!/usr/bin/env python3
"""Static validation of the SIMCom SIM800 Series database."""

from __future__ import annotations

import re
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
DB_HEADER = PROJECT_ROOT / "include" / "simcom" / "simcom_database.hpp"

EXPECTED_MODULES = [
    "SIM800L", "SIM800C", "SIM808", "SIM868", "SIM800A",
    "SIM800F", "SIM800H", "SIM800", "SIM800C-DS",
]

ALLOWED_JAMMING = {"PIN5", "PIN63", "PIN67", "PIN29", "None"}


def main() -> int:
    if not DB_HEADER.is_file():
        print(f"Missing database header: {DB_HEADER}", file=sys.stderr)
        return 1

    text = DB_HEADER.read_text(encoding="utf-8")
    failures = 0

    for name in EXPECTED_MODULES:
        if f'"{name}"' not in text:
            print(f"Missing module literal: {name}")
            failures += 1

    for value in re.findall(r'"(PIN\d+|None)"', text):
        if value not in ALLOWED_JAMMING:
            print(f"Unexpected jamming pin literal: {value}")
            failures += 1

    seen = len(re.findall(r'^\s*"[A-Z0-9\-]+",\s*$', text, re.MULTILINE))
    if seen < len(EXPECTED_MODULES):
        print(f"Only {seen} module records detected "
              f"(expected {len(EXPECTED_MODULES)})")
        failures += 1

    if failures:
        print(f"Validation failed with {failures} issue(s).", file=sys.stderr)
        return 1
    print("Database validation passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())

# SPDX-License-Identifier: MIT