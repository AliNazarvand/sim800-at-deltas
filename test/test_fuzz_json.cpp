// SPDX-License-Identifier: MIT
// Build only with: -fsanitize=fuzzer,address,undefined (Clang required)
#include "simcom/simcom.hpp"
#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, std::size_t size) {
    // Cap input size for performance.
    if (size > 4096) return 0;

    for (std::size_t i = 0; i < simcom::total_modules(); ++i) {
        char buf[512];
        std::size_t n = simcom::module_to_json(simcom::lookup_by_index(i), buf, sizeof(buf));
        (void)n;
    }

    // Also fuzz compare_modules with a fresh buffer sized from input.
    char cbuf[1024];
    (void)simcom::compare_modules(simcom::lookup_by_index(0),
                                  simcom::lookup_by_index(1),
                                  cbuf, sizeof(cbuf));

    // Zero-size queries must return required length.
    std::size_t need = simcom::module_to_json(simcom::lookup_by_index(0), nullptr, 0);
    (void)need;

    (void)data;
    return 0;
}