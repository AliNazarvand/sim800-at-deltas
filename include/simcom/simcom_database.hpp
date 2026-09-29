#ifndef SIMCOM_DATABASE_HPP
#define SIMCOM_DATABASE_HPP

#include "simcom_types.hpp"

namespace simcom {

// ---------------------------------------------------------------------------
// Static database - 9 modules
//  0: SIM800L  |  1: SIM800C  |  2: SIM808  |  3: SIM868  |  4: SIM800A
//  5: SIM800F  |  6: SIM800H  |  7: SIM800   |  8: SIM800C-DS
// ---------------------------------------------------------------------------
inline constexpr ModuleSpec MODULES[] = {
    // 0) SIM800L
    {
        "SIM800L",
        { "LGA", 88, 3400, 4400, 2048 },
        { false, false, true, true, true, true, true, true, 3, 1, 1 },
        { true, 3, 3, true, 0, true, "PIN5", "" }
    },
    // 1) SIM800C
    {
        "SIM800C",
        { "SMT", 42, 3400, 4400, 4096 },
        { true, false, false, true, false, false, false, false, 0, 1, 0 },
        { true, 2, 2, true, 2, false, "PIN29", "" }
    },
    // 2) SIM808
    {
        "SIM808",
        { "SMT", 68, 3400, 4400, 0 },
        { true, true, false, true, true, true, true, true, 2, 2, 2 },
        { true, 3, 3, true, 2, true, "PIN63", "" }
    },
    // 3) SIM868
    {
        "SIM868",
        { "SMT+LGA", 77, 3400, 4400, 4096 },
        { true, true, false, true, false, true, true, true, 2, 1, 0 },
        { true, 3, 3, true, 2, true, "PIN29", "" }
    },
    // 4) SIM800A
    {
        "SIM800A",
        { "SMT", 68, 3400, 4400, 0 },
        { true, false, false, true, true, false, false, true, 3, 1, 2 },
        { false, 2, 2, true, 2, false, "PIN67", "" }
    },
    // 5) SIM800F
    {
        "SIM800F",
        { "SMT", 68, 3400, 4400, 0 },
        { true, false, false, true, true, false, false, true, 3, 1, 2 },
        { true, 2, 2, true, 2, false, "PIN67", "" }
    },
    // 6) SIM800H
    {
        "SIM800H",
        { "LGA", 88, 3400, 4400, 4096 },
        { true, false, true, true, true, true, true, true, 3, 1, 1 },
        { true, 3, 3, true, 0, true, "PIN5", "" }
    },
    // 7) SIM800
    {
        "SIM800",
        { "SMT", 68, 3400, 4400, 0 },
        { true, false, false, true, true, false, true, true, 2, 1, 2 },
        { true, 2, 2, true, 2, true, "PIN67", "" }
    },
    // 8) SIM800C-DS
    {
        "SIM800C-DS",
        { "SMT+LGA", 77, 3400, 4400, 4096 },
        { true, false, true, true, false, true, true, true, 2, 1, 0 },
        { true, 3, 3, true, 0, true, "PIN29", "ch.21.13: FD not supported" }
    }
};

inline constexpr std::size_t MODULE_COUNT =
    sizeof(MODULES) / sizeof(MODULES[0]);

} // namespace simcom

#endif // SIMCOM_DATABASE_HPP
