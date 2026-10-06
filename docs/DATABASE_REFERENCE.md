# SIMCom SIM800 Series - Modules

## HardwareSpec

| name | package_type | pin_count | voltage_min_mv | voltage_max_mv | flash_kb |
|---|---|---|---|---|---|
| SIM800L | LGA | 88 | 3400 | 4400 | 2048 |
| SIM800C | SMT | 42 | 3400 | 4400 | 4096 |
| SIM808 | SMT | 68 | 3400 | 4400 | 0 |
| SIM868 | SMT+LGA | 77 | 3400 | 4400 | 4096 |
| SIM800A | SMT | 68 | 3400 | 4400 | 0 |
| SIM800F | SMT | 68 | 3400 | 4400 | 0 |
| SIM800H | LGA | 88 | 3400 | 4400 | 4096 |
| SIM800 | SMT | 68 | 3400 | 4400 | 0 |
| SIM800C-DS | SMT+LGA | 77 | 3400 | 4400 | 4096 |

## FeatureFlags

| name | has_bluetooth | has_gps | has_fm | has_audio | gpio_count |
|---|---|---|---|---|---|
| SIM800L | False | False | True | True | 3 |
| SIM800C | True | False | False | True | 0 |
| SIM808 | True | True | False | True | 2 |
| SIM868 | True | True | False | True | 2 |
| SIM800A | True | False | False | True | 3 |
| SIM800F | True | False | False | True | 3 |
| SIM800H | True | False | True | True | 3 |
| SIM800 | True | False | False | True | 2 |
| SIM800C-DS | True | False | True | True | 2 |

## ATCommandDeltas

| name | cband_quad_band | cmic_channels | sidet_channels | jamming_pin | extra_note |
|---|---|---|---|---|---|
| SIM800L | True | 3 | 3 | PIN5 |  |
| SIM800C | True | 2 | 2 | PIN29 |  |
| SIM808 | True | 3 | 3 | PIN63 |  |
| SIM868 | True | 3 | 3 | PIN29 |  |
| SIM800A | False | 2 | 2 | PIN67 |  |
| SIM800F | True | 2 | 2 | PIN67 |  |
| SIM800H | True | 3 | 3 | PIN5 |  |
| SIM800 | True | 2 | 2 | PIN67 |  |
| SIM800C-DS | True | 3 | 3 | PIN29 | ch.21.13: FD not supported |
