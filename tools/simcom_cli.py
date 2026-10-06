#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""SIMCom SIM800 Series - CLI query tool.

Reads data/*.yaml (source of truth) and provides query subcommands.
No export or validate commands - users should call those scripts directly.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    sys.stderr.write("ERROR: PyYAML not installed. Run: pip install -r requirements.txt\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
DATA = ROOT / "data"

FEATURE_KEYS = [
    "has_bluetooth","has_gps","has_fm","has_audio","has_keypad","has_sd",
    "has_pcm","has_i2c","has_uart","has_usb","has_rtc","has_kpled",
    "has_rf_sync","has_antenna_gps","has_antenna_bt","has_tdd",
]


def _load():
    with (DATA / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}
    return mods, ats


def _merged(mods, ats):
    out = []
    for m in mods:
        rec = {"name": m["name"], "hw": dict(m["hw"]), "feat": dict(m["feat"]),
               "at": dict(ats[m["name"]])}
        rec["at"].pop("module", None)
        out.append(rec)
    return out


def cmd_list(args):
    mods, ats = _load()
    rows = _merged(mods, ats)
    if args.json:
        print(json.dumps([r["name"] for r in rows], indent=2))
    else:
        for r in rows:
            print(r["name"])


def cmd_show(args):
    mods, ats = _load()
    rows = _merged(mods, ats)
    match = next((r for r in rows if r["name"] == args.module), None)
    if match is None:
        sys.stderr.write(f"ERROR: module not found: {args.module}\n")
        return 1
    print(json.dumps(match, indent=2))
    return 0


def cmd_compare(args):
    mods, ats = _load()
    rows = _merged(mods, ats)
    a = next((r for r in rows if r["name"] == args.a), None)
    b = next((r for r in rows if r["name"] == args.b), None)
    if a is None or b is None:
        sys.stderr.write("ERROR: module not found\n")
        return 1
    if args.json:
        print(json.dumps({"a": a, "b": b}, indent=2))
        return 0
    print(f"--- {a['name']} | {b['name']} ---")
    for section in ("hw", "feat", "at"):
        print(f"[{section}]")
        for key in sorted(a[section].keys()):
            if a[section][key] != b[section].get(key):
                print(f"  {key}: {a[section][key]} | {b[section].get(key)}")
    return 0


def cmd_find(args):
    mods, ats = _load()
    rows = _merged(mods, ats)
    result = list(rows)
    if args.gps:        result = [r for r in result if r["feat"]["has_gps"]]
    if args.bluetooth:  result = [r for r in result if r["feat"]["has_bluetooth"]]
    if args.fm:         result = [r for r in result if r["feat"]["has_fm"]]
    if args.audio:      result = [r for r in result if r["feat"]["has_audio"]]
    if args.voltage:
        lo, hi = args.voltage
        result = [r for r in result
                  if r["hw"]["voltage_min_mv"] >= lo and r["hw"]["voltage_max_mv"] <= hi]
    if args.pins:
        lo, hi = args.pins
        result = [r for r in result
                  if r["hw"]["pin_count"] >= lo and r["hw"]["pin_count"] <= hi]
    if args.json:
        print(json.dumps([r["name"] for r in result], indent=2))
    else:
        for r in result:
            print(r["name"])
    return 0


def cmd_stats(args):
    mods, ats = _load()
    rows = _merged(mods, ats)
    stats = {
        "total_modules": len(rows),
        "with_gps": sum(1 for r in rows if r["feat"]["has_gps"]),
        "with_bluetooth": sum(1 for r in rows if r["feat"]["has_bluetooth"]),
        "with_fm": sum(1 for r in rows if r["feat"]["has_fm"]),
        "with_audio": sum(1 for r in rows if r["feat"]["has_audio"]),
        "with_quad_band": sum(1 for r in rows if r["at"]["cband_quad_band"]),
        "min_voltage_mv": min(r["hw"]["voltage_min_mv"] for r in rows),
        "max_voltage_mv": max(r["hw"]["voltage_max_mv"] for r in rows),
        "min_pin_count": min(r["hw"]["pin_count"] for r in rows),
        "max_pin_count": max(r["hw"]["pin_count"] for r in rows),
        "sum_of_known_flash_kb": sum(r["hw"]["flash_kb"] for r in rows),
        "unknown_flash_count": sum(1 for r in rows if r["hw"]["flash_kb"] == 0),
    }
    print(json.dumps(stats, indent=2))
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(prog="simcom-cli")
    ap.add_argument("--json", action="store_true")
    sub = ap.add_subparsers(dest="cmd", required=True)

    sub.add_parser("list")
    sub.add_parser("stats")

    p_show = sub.add_parser("show"); p_show.add_argument("module")
    p_cmp  = sub.add_parser("compare")
    p_cmp.add_argument("a"); p_cmp.add_argument("b")

    p_find = sub.add_parser("find")
    p_find.add_argument("--gps", action="store_true")
    p_find.add_argument("--bluetooth", action="store_true")
    p_find.add_argument("--fm", action="store_true")
    p_find.add_argument("--audio", action="store_true")
    p_find.add_argument("--voltage", nargs=2, type=int, metavar=("MIN","MAX"))
    p_find.add_argument("--pins", nargs=2, type=int, metavar=("MIN","MAX"))

    args = ap.parse_args()
    if args.cmd == "list":     return cmd_list(args)
    if args.cmd == "show":     return cmd_show(args)
    if args.cmd == "compare":  return cmd_compare(args)
    if args.cmd == "find":     return cmd_find(args)
    if args.cmd == "stats":    return cmd_stats(args)
    return 0


if __name__ == "__main__":
    sys.exit(main())