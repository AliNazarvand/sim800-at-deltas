# Cross-Project Notes

This document describes the relationship between this project
(`ATCommandDeltas`) and its sibling project (`AT Command & URC`).
Both projects are independent repositories. This file is the only
place where the relationship is documented.

## 1. Purpose of Each Project

### `AT Command & URC`
- Ships the **complete catalog** of AT Commands (346) and URCs (95)
  for the SIMCom SIM800 Series family.
- Answers: *what does each command/URC mean, what parameters does it
  take, which modules support it*.
- Source of truth: `data/*.yaml` (human-edited).
- Consumption: generated C++17 headers under `include/sim800_at/`.

### `ATCommandDeltas` (this project)
- Ships the **per-module differences** derived from Chapter 21 of the
  SIM800 Series AT Command Manual V1.10 plus the Hardware Design
  datasheets.
- Answers: *where do the 9 modules differ in AT syntax and hardware
  capabilities*.
- Source of truth: `include/simcom/simcom_database.hpp` (hand-written).
- Consumption: header-only static database, no generation step.

## 2. Comparison Table

| Aspect                | `AT Command & URC`           | `ATCommandDeltas`            |
|-----------------------|------------------------------|------------------------------|
| Purpose               | Full command / URC catalog   | Per-module AT & HW deltas    |
| Data volume           | 346 commands + 95 URCs       | 9 module records             |
| Data source           | 3 AT manuals (V1.01/10/12)   | V1.10 Ch.21 + HW datasheets  |
| Editing model         | YAML edited by hand          | C++ edited by hand           |
| Generation step       | Yes (`tools/generate.py`)    | No                           |
| Header namespace      | `sim800_at`                  | `simcom`                     |
| Include umbrella      | `sim800_at/sim800_at.hpp`    | `simcom/simcom.hpp`          |

## 3. How the Two Projects Relate

The two projects are **complementary**, not overlapping:

- `AT Command & URC` tells **what** each command is.
- `ATCommandDeltas` tells **where** each command differs between modules.

Example:
- `AT Command & URC` knows `AT+CMIC` exists and lists its parameters.
- `ATCommandDeltas` knows `AT+CMIC` has 3 channels on SIM808 but
  2 channels on SIM800C.

Together they answer both questions:
> *which command is supported on which module, and with what
>  per-module parameters*.

## 4. Integration Point in the Architecture

Both projects feed into **layer L3-B (Modem Profiles)** of the
Control Board SMS firmware:

    L0-B  Foundations      (ModemDescriptor, CapabilityDescriptor)
      ^
      |
    L3-B  Modem Profiles
      |  - ProfileTable              <-- from AT Command & URC
      |  - AtCommandDictionaryData   <-- from AT Command & URC
      |  - AtDeltaTableData          <-- from ATCommandDeltas
      |  - PowerSequenceTableData    <-- from hardware datasheets
      |  - CapabilityData            <-- merged from both
      |
    L3-C  Modem Drivers
      |  - SIM800xDriver, SIM840xDriver, ...
      |
    ...

`AtDeltaTableData` is the bridge: a compile-time table of per-module
differences applied on top of a family-wide `AtCommandDictionary`.

## 5. Rules for AI Agents

When reading these projects, follow these rules:

1. **Single source of truth per project.**
   - `AT Command & URC`: `data/*.yaml` is the source of truth.
     Never edit `include/sim800_at/*.hpp` by hand.
   - `ATCommandDeltas`: `include/simcom/simcom_database.hpp` is the
     source of truth. There is no generation step.

2. **Do not duplicate data across projects.**
   - If a fact belongs to the AT catalog, it lives in `AT Command & URC`.
   - If a fact belongs to per-module deltas, it lives here.
   - If a fact could live in both, prefer the deltas project when it is
     about a difference between modules; prefer the catalog when it is
     about the command itself.

3. **Recommended reading order for a new task.**
   - First: `AT Command & URC/docs/ARCHITECTURE.md` (catalog schema).
   - Then: `ATCommandDeltas/docs/ARCHITECTURE.md` (this project schema).
   - Then: `ATCommandDeltas/docs/CROSS_PROJECT_NOTES.md` (this file).
   - Then: `ATCommandDeltas/include/simcom/simcom_database.hpp`
     (concrete deltas per module).

4. **Naming consistency.**
   - Module names must match exactly between the two projects.
     The canonical list is the 9 modules below.
     If a module name changes, update both projects in the same change
     set.

5. **No cross-project headers.**
   - Neither project `#include`s the other at this time. The bridge
     happens at L3-B in the firmware project, not inside these two
     header-only libraries.

## 6. Canonical Module List (9 modules)

Both projects must agree on this exact list and order:

1. SIM800L
2. SIM800C
3. SIM808
4. SIM868
5. SIM800A
6. SIM800F
7. SIM800H
8. SIM800
9. SIM800C-DS

## 7. Known Overlaps and Non-Overlaps

Overlap: none by design.

Adjacent but non-overlapping topics:
- AT syntax variants of a single command (`AT+X` vs `AT+X=...`):
  lives in `AT Command & URC` (`parameters` field).
- Module support scope of a command (`supported_on` field):
  lives in `AT Command & URC`.
- Numeric defaults that differ per module (`cmic_channels`,
  `cfgri_default`, `sidet_channels`): lives here.
- Hardware pin differences (`jamming_pin`): lives here.
- Boolean feature presence (`has_gps`, `has_bluetooth`): could live
  in either; currently lives here because it drives AT behavior.

## 8. Change Policy

- A change to the AT catalog does **not** require a change here unless
  it introduces a new per-module delta.
- A change here does **not** require a change to the AT catalog unless
  it introduces a command that does not yet exist there.
- If a change touches both projects, both must be committed together
  in the same session to keep them consistent.

## 9. External References

- Original PDFs (AT manuals, hardware datasheets):
  hosted in a separate `Package-Datasheet` repository.
- The two projects do not embed the PDFs; they only reference them
  in their `README.md` files.