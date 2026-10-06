# Bridge Design (Cross-Project)

Per `CROSS_PROJECT_NOTES.md` Section 5 ("No cross-project headers"), this
project does NOT ship a C++ bridge header. This document describes the
interface contract only.

## Interface: AtDeltaTableData

    struct AtDeltaTableData {
        const char* module_name;
        const ATCommandDeltas* deltas;
    };

`AtDeltaTableData` is a compile-time table of per-module AT differences.
It is consumed by the firmware layer L3-B (Modem Profiles), not by this
header-only library.

## Functions (pseudo-code, not C++)

    get_at_delta(module_name) -> const ATCommandDeltas*
        Return the AT deltas for the given module, or null if not found.

    apply_delta_to_profile(module_name, profile)
        Merge ATCommandDeltas into the given profile descriptor.
        Called once per modem during profile construction.

## Integration Point

    L0-B Foundations
      |
    L3-B Modem Profiles
      |   - ProfileTable              <- from AT Command & URC project
      |   - AtCommandDictionaryData   <- from AT Command & URC project
      |   - AtDeltaTableData          <- from THIS project
      |   - PowerSequenceTableData    <- from hardware datasheets
      |   - CapabilityData            <- merged from both projects
      |
    L3-C Modem Drivers

See `CROSS_PROJECT_NOTES.md` for the full context.