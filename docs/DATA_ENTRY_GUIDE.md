# Data Entry Guide

This document walks a human developer through filling in the YAML
files in `data/` from the 13 source PDFs.

## 1. Place the PDFs

Download the following files from the `Package-Datasheet` repository
and copy them into `data/pdfs/`:

Hardware Design datasheets:
- SIM800A_Hardware Design_V1.02.pdf
- SIM800C-DS_Hardware_Design_V1.01.pdf
- SIM800C_Hardware_Design_V1.02.pdf
- SIM800F_Hardware Design_V1.05.pdf
- SIM800H&SIM800L_Hardware Design_V2.02.PDF
- SIM800H_Hardware Design_V2.03.pdf
- SIM800L_Hardware Design_V1.00.pdf
- SIM800_Hardware Design_V1.09.pdf
- SIM808_Hardware Design_V1.03.pdf
- SIM868_Hardware_Design_V1.00.pdf

AT Command Manuals:
- SIM800 Series_AT Command Manual_V1.01.pdf
- SIM800 Series_AT Command Manual_V1.10.pdf
- SIM800 Series_AT Command Manual_V1.12.pdf

## 2. Extract raw text

    python tools/extract_pdf.py

Raw text dumps appear in `data/raw/`.

## 3. Fill in YAML

### `data/modules.yaml`

Field -> PDF source:

| YAML field | Source |
|------------|--------|
| `package_type` | Hardware Design datasheet, package section |
| `pin_count` | Hardware Design datasheet, pin list |
| `voltage_min_mv` / `voltage_max_mv` | Hardware Design, power supply |
| `flash_kb` | Hardware Design, internal flash (0 if not stated) |
| `length_mm` / `width_mm` / `height_mm` | Hardware Design, mechanical dimensions |
| `weight_mg` | Hardware Design, weight |
| `operating_temp_min_c` / `operating_temp_max_c` | Hardware Design, temperature range |
| `current_sleep_ua` | Hardware Design, sleep current |
| `current_peak_ma` | Hardware Design, peak current |
| `gprs_class` | Hardware Design, GPRS section |
| `gprs_downlink_kbps` / `gprs_uplink_kbps` | Hardware Design, GPRS throughput |
| `baud_rate_min` / `baud_rate_max` | Hardware Design, UART section |
| `sim_card_type` | Hardware Design, SIM interface |
| `antenna_connector` | Hardware Design, antenna section |
| `uart_count` | Hardware Design |
| `usb_support` | Hardware Design |
| `rtc_backup` | Hardware Design |
| `network_status_pin` | Hardware Design, pin table (format `PIN<num>` or `""`) |
| `operating_status_pin` | Hardware Design, pin table |

### `data/at_deltas.yaml`

Field -> source:

| YAML field | Source |
|------------|--------|
| `cband_quad_band` | AT Command Manual, `AT+CBAND` |
| `cmic_channels` | AT Command Manual, `AT+CMIC` |
| `sidet_channels` | AT Command Manual, `AT+SIDET` |
| `supports_csclk2` | AT Command Manual, `AT+CSCLK` |
| `cfgri_default` | AT Command Manual, `AT+CFGRI` |
| `chfa_pcm_support` | AT Command Manual, `AT+CHFA` |
| `jamming_pin` | AT Command Manual, Chapter 21.6 |
| `extra_note` | Free-form; used to record ambiguity between shared and per-module datasheets |

### `data/version_deltas.yaml`

Record commands whose parameters changed between the AT manual versions
V1.01, V1.10 and V1.12. Leave the array empty if there is no data.

### `data/cross_references.yaml`

Record audio pins and related commands per module.
Maximum 8 audio pins and 16 related commands per module.

## 4. Validate

    python tools/validate_yaml.py --strict

## 5. Regenerate the C++ header

    python tools/generate_cpp.py

## 6. Optional cross-validation against PDFs

    python tools/validate_against_pdf.py

**Warning:** automatic extraction may produce incorrect values.
Manual review of `exports/validation_report.txt` is required.

## 7. Rules

- Never edit `include/simcom/simcom_database.hpp` by hand.
- The three YAML files (`modules`, `at_deltas`, `cross_references`)
  must list the same 9 modules in the same order.
- `schema_version` must be `1` in all four YAML files.
- Module list is fixed:
  `SIM800L, SIM800C, SIM808, SIM868, SIM800A, SIM800F, SIM800H, SIM800, SIM800C-DS`.
- Modules without a dedicated datasheet (SIM800M64, SIM800G, SIM800W,
  SIM840W, SIM800V) are not allowed.
