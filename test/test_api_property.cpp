// SPDX-License-Identifier: MIT
// Property-based test for module_to_json under random buffer sizes.
//
// Contract reminder: module_to_json() ALWAYS returns the total required
// length, regardless of buffer_size. So the invariant is written == need.
// Buffer overflow is not detectable from the return value; rely on ASan
// when fuzzing.
#include "simcom/simcom.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <random>

// Fixed seed for determinism. Never change without explicit human review.
static constexpr std::uint32_t SIMCOM_PROPERTY_SEED = 0x5EED;

int main() {
    using namespace simcom;
    std::mt19937 rng(SIMCOM_PROPERTY_SEED);
    std::uniform_int_distribution<std::size_t> dist(0, 8192);

    int failures = 0;

    for (int iter = 0; iter < 200; ++iter) {
        const ModuleSpec* m = lookup_by_index(dist(rng) % MODULE_COUNT);
        std::size_t sz = dist(rng);
        std::size_t need = module_to_json(m, nullptr, 0);

        char* buf = new char[sz > 0 ? sz : 1];
        std::size_t written = module_to_json(m, buf, sz);

        if (written != need) {
            std::printf("FAIL iter=%d: written=%zu need=%zu\n", iter, written, need);
            ++failures;
        }
        delete[] buf;
    }

    // Determinism: same module, same output, twice.
    char a[4096];
    char b[4096];
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        std::size_t na = module_to_json(lookup_by_index(i), a, sizeof(a));
        std::size_t nb = module_to_json(lookup_by_index(i), b, sizeof(b));
        if (na != nb) { std::printf("FAIL: determinism size\n"); ++failures; }
        if (std::memcmp(a, b, na) != 0) {
            std::printf("FAIL: determinism bytes\n");
            ++failures;
        }
    }

    if (failures == 0) { std::printf("api_property tests passed.\n"); return 0; }
    std::printf("%d api_property failure(s).\n", failures);
    return 1;
}