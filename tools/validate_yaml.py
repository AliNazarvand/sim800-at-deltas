#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Validate data/*.yaml against JSON Schemas and cross-file rules."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

try:
    import yaml
    import jsonschema
except ImportError as exc:  # pragma: no cover
    sys.stderr.write(f"Missing Python dependency: {exc}\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

from enum_mappings import (  # noqa: E402
    JAMMING_PIN_MAP,
    SIM_CARD_TYPE_MAP,
    ANTENNA_TYPE_MAP,
    AT_MANUAL_VERSION_MAP,
    SUPPORTED_SCHEMA_VERSION,
    MODULE_ORDER,
)

DATA_DIR = ROOT / "data"
SCHEMA_DIR = HERE / "schemas"

PIN_RE = re.compile(r"^PIN\d+$|^$")


def _load_yaml(path: Path):
    with path.open("r", encoding="utf-8") as fh:
        return yaml.safe_load(fh)


def _load_schema(path: Path):
    with path.open("r", encoding="utf-8") as fh:
        return json.load(fh)


def _fail(msg: str, strict: bool, warnings: list):
    if strict:
        warnings.append(msg)
    else:
        print(f"WARN: {msg}")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--strict", action="store_true")
    args = ap.parse_args()

    warnings: list[str] = []

    files = [
        ("modules.yaml", "modules.schema.json"),
        ("at_deltas.yaml", "at_deltas.schema.json"),
        ("version_deltas.yaml", "version_deltas.schema.json"),
        ("cross_references.yaml", "cross_references.schema.json"),
    ]
    docs = {}
    for yml_name, sch_name in files:
        yml_path = DATA_DIR / yml_name
        sch_path = SCHEMA_DIR / sch_name
        if not yml_path.is_file():
            sys.stderr.write(f"ERROR: missing {yml_path}\n")
            return 2
        if not sch_path.is_file():
            sys.stderr.write(f"ERROR: missing {sch_path}\n")
            return 2
        doc = _load_yaml(yml_path)
        schema = _load_schema(sch_path)
        try:
            jsonschema.validate(instance=doc, schema=schema)
        except jsonschema.ValidationError as exc:
            sys.stderr.write(f"ERROR: {yml_name} fails schema: {exc.message}\n")
            return 3
        docs[yml_name] = doc

    versions = {n: docs[n].get("schema_version") for n, _ in files}
    if any(v != SUPPORTED_SCHEMA_VERSION for v in versions.values()):
        sys.stderr.write(f"ERROR: schema_version mismatch: {versions}\n")
        return 4

    mods = docs["modules.yaml"]["modules"]
    ats = docs["at_deltas.yaml"]["at_deltas"]
    xrs = docs["cross_references.yaml"]["cross_references"]

    mod_names = [m["name"] for m in mods]
    at_names = [a["module"] for a in ats]
    xr_names = [x["module"] for x in xrs]

    if mod_names != at_names or mod_names != xr_names:
        sys.stderr.write("ERROR: module name lists differ across YAML files\n")
        return 5
    if mod_names != MODULE_ORDER:
        sys.stderr.write("ERROR: module order does not match canonical order\n")
        return 6

    for a in ats:
        jp = a["jamming_pin"]
        if jp not in JAMMING_PIN_MAP:
            sys.stderr.write(f"ERROR: unknown jamming_pin {jp!r}\n")
            return 7
    for m in mods:
        if m["hw"]["sim_card_type"] not in SIM_CARD_TYPE_MAP:
            sys.stderr.write(f"ERROR: unknown sim_card_type for {m['name']}\n")
            return 8
        if m["hw"]["antenna_connector"] not in ANTENNA_TYPE_MAP:
            sys.stderr.write(f"ERROR: unknown antenna_connector for {m['name']}\n")
            return 9
        for field in ("network_status_pin", "operating_status_pin"):
            v = m["hw"][field]
            if not PIN_RE.match(v):
                _fail(f"{m['name']}.{field} = {v!r} fails regex", args.strict, warnings)

    for vd in docs["version_deltas.yaml"].get("version_deltas") or []:
        for field in ("added_in", "modified_in", "deprecated_in"):
            if vd.get(field) not in AT_MANUAL_VERSION_MAP:
                sys.stderr.write(f"ERROR: bad {field} value {vd.get(field)!r}\n")
                return 10

    for x in xrs:
        if len(x.get("audio_pins", [])) > 8:
            sys.stderr.write(f"ERROR: {x['module']} has >8 audio_pins\n")
            return 11
        if len(x.get("related_commands", [])) > 16:
            sys.stderr.write(f"ERROR: {x['module']} has >16 related_commands\n")
            return 12

    if warnings and args.strict:
        for w in warnings:
            sys.stderr.write(f"ERROR(strict): {w}\n")
        return 13

    print("YAML validation passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
