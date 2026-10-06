#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Generate include/simcom/simcom_database.hpp from data/*.yaml."""

from __future__ import annotations

import sys
from pathlib import Path

try:
    import yaml
    from jinja2 import Environment, FileSystemLoader, StrictUndefined
except ImportError as exc:  # pragma: no cover
    sys.stderr.write(f"Missing Python dependency: {exc}\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(HERE))

from enum_mappings import (  # noqa: E402
    JAMMING_PIN_MAP,
    MODULE_ORDER,
    SUPPORTED_SCHEMA_VERSION,
)

DATA_DIR = ROOT / "data"
TEMPLATE_DIR = HERE
OUT_PATH = ROOT / "include" / "simcom" / "simcom_database.hpp"

REQUIRED_FILES = [
    "modules.yaml",
    "at_deltas.yaml",
    "version_deltas.yaml",
    "cross_references.yaml",
]


def _load(name: str) -> dict:
    p = DATA_DIR / name
    if not p.is_file():
        sys.stderr.write(f"ERROR: missing YAML file: {p}\n")
        sys.exit(2)
    with p.open("r", encoding="utf-8") as fh:
        return yaml.safe_load(fh)


def _check_order(names, label: str) -> None:
    if list(names) != MODULE_ORDER:
        sys.stderr.write(
            f"ERROR: module order in {label} does not match canonical order.\n"
            f"  expected: {MODULE_ORDER}\n"
            f"  got:      {list(names)}\n"
        )
        sys.exit(3)


def main() -> int:
    mods_doc = _load("modules.yaml")
    at_doc = _load("at_deltas.yaml")
    vd_doc = _load("version_deltas.yaml")
    xr_doc = _load("cross_references.yaml")

    for doc, label in [
        (mods_doc, "modules.yaml"),
        (at_doc, "at_deltas.yaml"),
        (vd_doc, "version_deltas.yaml"),
        (xr_doc, "cross_references.yaml"),
    ]:
        ver = doc.get("schema_version")
        if ver != SUPPORTED_SCHEMA_VERSION:
            sys.stderr.write(
                f"ERROR: schema_version mismatch in {label}: "
                f"{ver!r} != {SUPPORTED_SCHEMA_VERSION}\n"
            )
            sys.exit(4)

    modules = mods_doc["modules"]
    at_deltas = at_doc["at_deltas"]
    version_deltas = vd_doc.get("version_deltas") or []
    cross_refs = xr_doc.get("cross_references") or []

    mod_names = [m["name"] for m in modules]
    at_names = [a["module"] for a in at_deltas]
    xr_names = [x["module"] for x in cross_refs]

    if mod_names != at_names:
        sys.stderr.write("ERROR: module names differ between modules.yaml and at_deltas.yaml\n")
        sys.exit(5)
    if mod_names != xr_names:
        sys.stderr.write("ERROR: module names differ between modules.yaml and cross_references.yaml\n")
        sys.exit(6)
    _check_order(mod_names, "modules.yaml")
    _check_order(at_names, "at_deltas.yaml")
    _check_order(xr_names, "cross_references.yaml")

    at_by_mod = {a["module"]: a for a in at_deltas}

    merged = []
    for m in modules:
        name = m["name"]
        at = at_by_mod[name]
        jp = at.get("jamming_pin", "None")
        if jp not in JAMMING_PIN_MAP:
            sys.stderr.write(f"ERROR: unknown jamming_pin value {jp!r} for {name}\n")
            sys.exit(7)
        merged.append({
            "name": name,
            "hw": m["hw"],
            "feat": m["feat"],
            "at": {
                "cband_quad_band":  at["cband_quad_band"],
                "cmic_channels":    at["cmic_channels"],
                "sidet_channels":   at["sidet_channels"],
                "supports_csclk2":  at["supports_csclk2"],
                "cfgri_default":    at["cfgri_default"],
                "chfa_pcm_support": at["chfa_pcm_support"],
                "jamming_pin":      jp,
                "extra_note":       at.get("extra_note", "") or "",
                "jamming_pin_enum": JAMMING_PIN_MAP[jp],
            },
        })

    env = Environment(
        loader=FileSystemLoader(str(TEMPLATE_DIR)),
        undefined=StrictUndefined,
        keep_trailing_newline=True,
        trim_blocks=True,
        lstrip_blocks=True,
    )
    template = env.get_template("template.hpp.j2")
    rendered = template.render(
        modules=merged,
        version_deltas=version_deltas,
        cross_references=cross_refs,
        schema_version=SUPPORTED_SCHEMA_VERSION,
    )
    rendered = rendered.replace("\r\n", "\n").replace("\r", "\n")

    OUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    OUT_PATH.write_text(rendered, encoding="utf-8", newline="\n")
    print(f"Generated {OUT_PATH}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
