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

    if (g_failures == 0) { std::printf("All database tests passed.\n"); return 0; }
    std::printf("%d test(s) failed.\n", g_failures);
    return 1;
}
