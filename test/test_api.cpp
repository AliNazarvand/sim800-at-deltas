// SPDX-License-Identifier: MIT
// ---------------------------------------------------------------------------
// Tests for the advanced query API (Phase 4)
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

    // --- find_modules_with_gps (count-only mode) ---
    std::size_t gps_count = find_modules_with_gps(nullptr, 0);
    CHECK(gps_count >= 2);  // SIM808 and SIM868

    // --- find_modules_with_gps (write mode) ---
    const ModuleSpec* results[16];
    std::size_t total = find_modules_with_gps(results, 16);
    CHECK(total == gps_count);
    for (std::size_t i = 0; i < total && i < 16; ++i) {
        CHECK(results[i] != nullptr);
        CHECK(results[i]->feat.has_gps == true);
    }

    // max_results smaller than total: still returns total count
    std::size_t total2 = find_modules_with_gps(results, 1);
    CHECK(total2 == gps_count);

    // --- find_modules_by_voltage (count-only) ---
    std::size_t v_count = find_modules_by_voltage(3400, 4400, nullptr, 0);
    CHECK(v_count == 9);

    std::size_t v_count_narrow = find_modules_by_voltage(3500, 4400, nullptr, 0);
    CHECK(v_count_narrow == 0);

    // --- find_modules_by_pin_count (count-only) ---
    std::size_t p88 = find_modules_by_pin_count(88, 88, nullptr, 0);
    CHECK(p88 == 2);  // SIM800L, SIM800H

    std::size_t p_wide = find_modules_by_pin_count(0, 200, nullptr, 0);
    CHECK(p_wide == 9);

    // --- compare_modules deterministic output ---
    const ModuleSpec* a = lookup_by_name("SIM800L");
    const ModuleSpec* b = lookup_by_name("SIM800H");
    char buf1[8192]; char buf2[8192];
    std::size_t n1 = compare_modules(a, b, buf1, sizeof(buf1));
    std::size_t n2 = compare_modules(a, b, buf2, sizeof(buf2));
    CHECK(n1 == n2);
    CHECK(std::memcmp(buf1, buf2, n1) == 0);

    // size query with buffer_size == 0
    std::size_t need = compare_modules(a, b, nullptr, 0);
    CHECK(need == n1);

    // null handling
    std::size_t nn = compare_modules(nullptr, b, buf1, sizeof(buf1));
    CHECK(nn == 4);
    CHECK(std::strcmp(buf1, "null") == 0);

    // --- module_to_json deterministic output ---
    const ModuleSpec* m = lookup_by_name("SIM808");
    char j1[8192]; char j2[8192];
    std::size_t jn1 = module_to_json(m, j1, sizeof(j1));
    std::size_t jn2 = module_to_json(m, j2, sizeof(j2));
    CHECK(jn1 == jn2);
    CHECK(std::memcmp(j1, j2, jn1) == 0);
    CHECK(std::strstr(j1, "\"name\":\"SIM808\"") != nullptr);
    CHECK(std::strstr(j1, "\"hw\":{") != nullptr);
    CHECK(std::strstr(j1, "\"feat\":{") != nullptr);
    CHECK(std::strstr(j1, "\"at\":{") != nullptr);
    CHECK(std::strstr(j1, "\"jamming_pin_enum\":\"PIN63\"") != nullptr);

    // size query with buffer_size == 0
    std::size_t jneed = module_to_json(m, nullptr, 0);
    CHECK(jneed == jn1);

    // null pointer
    std::size_t jnull = module_to_json(nullptr, j1, sizeof(j1));
    CHECK(jnull == 4);
    CHECK(std::strcmp(j1, "null") == 0);

    if (g_failures == 0) { std::printf("All API tests passed.\n"); return 0; }
    std::printf("%d test(s) failed.\n", g_failures);
    return 1;
}
