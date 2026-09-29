# Architecture

## Overview

The project ships a static, compile-time database of SIMCom SIM800
Series modules together with ESP32-WROOM-32 integration scaffolding.
The database is header-only; all data lives in ROM/flash when linked
into a firmware image.

## Layers

1. Types (`include/simcom/simcom_types.hpp`)
   Plain structs describing hardware, feature flags and AT command
   variations. No functions, no data.

2. Database (`include/simcom/simcom_database.hpp`)
   A single `inline constexpr` array `MODULES` with one record per
   module. No allocation, no string handling.

3. API (`include/simcom/simcom_api.hpp`)
   `inline` lookup helpers returning pointers into the database.
   No allocation, no exceptions.

4. Umbrella (`include/simcom/simcom.hpp`)
   Includes the three layers above; the only header user code needs.

## Extending

To add a new module:

1. Append a new entry to `MODULES` in
   `include/simcom/simcom_database.hpp`.
2. Update `EXPECTED_MODULES` in `tools/validate.py`.
3. Run `tools/validate.py`.

No API change is required.

## Data Sources

- SIM800 Series_AT Command Manual_V1.10 - Chapter 21.
- Hardware Design datasheets for each supported module.

## Known Limitations

- `flash_kb == 0` marks modules whose flash size is not stated in the
  Hardware Design datasheet (SIM808, SIM800A, SIM800F, SIM800).
- `gpio_count`, `adc_count`, `pwm_count` count dedicated pins only;
  pins available through multiplexing are not included.
- `jamming_pin` uses the pin name from Chapter 21.6.
