#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Export data/*.yaml to exports/modules.csv (deterministic, no BOM)."""

from __future__ import annotations

import csv
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
from enum_mappings import JAMMING_PIN_MAP  # noqa: E402

DATA_DIR = ROOT / "data"
OUT_DIR = ROOT / "exports"

HW_FIELDS = [
    "package_type", "pin_count", "voltage_min_mv", "voltage_max_mv", "flash_kb",
    "length_mm", "width_mm", "height_mm", "weight_mg",
    "operating_temp_min_c", "operating_temp_max_c",
    "current_sleep_ua", "current_peak_ma", "gprs_class",
    "gprs_downlink_kbps", "gprs_uplink_kbps",
    "baud_rate_min", "baud_rate_max", "sim_card_type", "antenna_connector",
    "uart_count", "usb_support", "rtc_backup",
    "network_status_pin", "operating_status_pin",
]
FEAT_FIELDS = [
    "has_bluetooth", "has_gps", "has_fm", "has_audio", "has_keypad",
    "has_sd", "has_pcm", "has_i2c", "gpio_count", "adc_count", "pwm_count",
    "has_uart", "has_usb", "has_rtc", "has_kpled", "has_rf_sync",
    "has_antenna_gps", "has_antenna_bt", "has_tdd",
]
AT_FIELDS = [
    "cband_quad_band", "cmic_channels", "sidet_channels", "supports_csclk2",
    "cfgri_default", "chfa_pcm_support", "jamming_pin", "extra_note",
]


def main() -> int:
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    with (DATA_DIR / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA_DIR / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}

    path = OUT_DIR / "modules.csv"
    with path.open("w", encoding="utf-8", newline="") as fh:
        writer = csv.writer(fh, lineterminator="\n")
        header = ["name"] + HW_FIELDS + FEAT_FIELDS + AT_FIELDS
        writer.writerow(header)
        for m in mods:
            at = ats[m["name"]]
            row = [m["name"]]
            for f in HW_FIELDS: row.append(m["hw"].get(f, ""))
            for f in FEAT_FIELDS: row.append(m["feat"].get(f, ""))
            for f in AT_FIELDS:
                row.append(at.get(f, ""))
            writer.writerow(row)
    print(f"Wrote {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
