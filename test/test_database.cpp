// SPDX-License-Identifier: MIT
// ---------------------------------------------------------------------------
// Host-side sanity tests for the SIMCom SIM800 Series database
// ---------------------------------------------------------------------------

#include "simcom/simcom.hpp"
#include <cstdio>
#include <cstring>

static int g_failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                               \
        }                                                               \
    } while (0)

int main() {
    using namespace simcom;

    CHECK(total_modules() == 9);

    for (std::size_t i = 0; i < total_modules(); ++i) {
        const ModuleSpec* m = lookup_by_index(i);
        CHECK(m != nullptr);
        CHECK(m->name != nullptr);
        CHECK(m->hw.package_type != nullptr);
        CHECK(m->at.jamming_pin != nullptr);
        CHECK(m->at.extra_note != nullptr);
        CHECK(m->hw.network_status_pin != nullptr);
        CHECK(m->hw.operating_status_pin != nullptr);
    }

    CHECK(lookup_by_index(0) && str_equal(lookup_by_index(0)->name, "SIM800L"));
    CHECK(lookup_by_index(1) && str_equal(lookup_by_index(1)->name, "SIM800C"));
    CHECK(lookup_by_index(2) && str_equal(lookup_by_index(2)->name, "SIM808"));
    CHECK(lookup_by_index(3) && str_equal(lookup_by_index(3)->name, "SIM868"));
    CHECK(lookup_by_index(4) && str_equal(lookup_by_index(4)->name, "SIM800A"));
    CHECK(lookup_by_index(5) && str_equal(lookup_by_index(5)->name, "SIM800F"));
    CHECK(lookup_by_index(6) && str_equal(lookup_by_index(6)->name, "SIM800H"));
    CHECK(lookup_by_index(7) && str_equal(lookup_by_index(7)->name, "SIM800"));
    CHECK(lookup_by_index(8) && str_equal(lookup_by_index(8)->name, "SIM800C-DS"));

    const ModuleSpec* m808 = lookup_by_name("SIM808");
    CHECK(m808 != nullptr);
    CHECK(m808->feat.has_gps == true);
    CHECK(m808->feat.has_bluetooth == true);

    CHECK(lookup_by_name("NotARealModule") == nullptr);
    CHECK(lookup_by_name(nullptr) == nullptr);
    CHECK(lookup_by_index(999) == nullptr);

    CHECK(lookup_by_name("SIM800A")->at.cband_quad_band == false);
    CHECK(lookup_by_name("SIM800F")->at.cband_quad_band == true);
    CHECK(lookup_by_name("SIM800C-DS")->at.extra_note[0] != '\0');

    // to_string round-trips (must be exact inverse of YAML -> enum mappings)
    CHECK(str_equal(to_string(SimCardType::UNKNOWN),  "UNKNOWN"));
    CHECK(str_equal(to_string(SimCardType::MICRO),    "MICRO"));
    CHECK(str_equal(to_string(SimCardType::NANO),     "NANO"));
    CHECK(str_equal(to_string(SimCardType::STANDARD), "STANDARD"));
    CHECK(str_equal(to_string(AntennaType::UNKNOWN),  "UNKNOWN"));
    CHECK(str_equal(to_string(AntennaType::UFL),      "UFL"));
    CHECK(str_equal(to_string(AntennaType::SPRING),   "SPRING"));
    CHECK(str_equal(to_string(AntennaType::SMA),      "SMA"));
    CHECK(str_equal(to_string(AntennaType::PCB),      "PCB"));
    CHECK(str_equal(to_string(JammingPin::NONE),      "None"));
    CHECK(str_equal(to_string(JammingPin::PIN5),      "PIN5"));
    CHECK(str_equal(to_string(JammingPin::PIN29),     "PIN29"));
    CHECK(str_equal(to_string(JammingPin::PIN63),     "PIN63"));
    CHECK(str_equal(to_string(JammingPin::PIN67),     "PIN67"));
    CHECK(str_equal(to_string(AtManualVersion::UNKNOWN), "UNKNOWN"));
    CHECK(str_equal(to_string(AtManualVersion::V1_01),   "V1.01"));
    CHECK(str_equal(to_string(AtManualVersion::V1_10),   "V1.10"));
    CHECK(str_equal(to_string(AtManualVersion::V1_12),   "V1.12"));

    // jamming_pin_enum is derived from jamming_pin
    CHECK(lookup_by_name("SIM800L")->at.jamming_pin_enum    == JammingPin::PIN5);
    CHECK(lookup_by_name("SIM800C")->at.jamming_pin_enum    == JammingPin::PIN29);
    CHECK(lookup_by_name("SIM808")->at.jamming_pin_enum     == JammingPin::PIN63);
    CHECK(lookup_by_name("SIM868")->at.jamming_pin_enum     == JammingPin::PIN29);
    CHECK(lookup_by_name("SIM800A")->at.jamming_pin_enum    == JammingPin::PIN67);
    CHECK(lookup_by_name("SIM800F")->at.jamming_pin_enum    == JammingPin::PIN67);
    CHECK(lookup_by_name("SIM800H")->at.jamming_pin_enum    == JammingPin::PIN5);
    CHECK(lookup_by_name("SIM800")->at.jamming_pin_enum     == JammingPin::PIN67);
    CHECK(lookup_by_name("SIM800C-DS")->at.jamming_pin_enum == JammingPin::PIN29);

    // Cross-reference table sizing (one record per module)
    CHECK(CROSS_REFERENCE_COUNT == MODULE_COUNT);
    CHECK(VERSION_DELTA_COUNT >= 0);

    if (g_failures == 0) { std::printf("All database tests passed.\n"); return 0; }
    std::printf("%d test(s) failed.\n", g_failures);
    return 1;
}
