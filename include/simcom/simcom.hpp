#ifndef SIMCOM_HPP
#define SIMCOM_HPP

// ---------------------------------------------------------------------------
// SIMCom SIM800 Series - Header-Only Static Module Database
//
// Usage:
//   #include "simcom.hpp"
//
//   const simcom::ModuleSpec* m = simcom::lookup_by_name("SIM800L");
//   if (m) { /* m->hw.pin_count, m->feat.has_gps, ... */ }
//
// Requirements:
//   - C++17 (inline constexpr)
//   - No dynamic allocation
//   - No Arduino-specific dependencies
//
// Data sources:
//   - SIM800 Series_AT Command Manual_V1.10 (Chapter 21)
//   - Hardware Design datasheets per module
// ---------------------------------------------------------------------------

#include "simcom_types.hpp"
#include "simcom_database.hpp"
#include "simcom_api.hpp"

#endif // SIMCOM_HPP
