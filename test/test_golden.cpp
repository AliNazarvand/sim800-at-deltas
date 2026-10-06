// SPDX-License-Identifier: MIT
//
// Golden test for module_to_json and compare_modules.
//
// Usage:
//   test_golden                 -> compare against test/golden/expected_outputs.txt
//                                   (requires APPROVED_BY in README.md)
//   test_golden --dump <path>   -> write current output to <path> and exit 0
//
// The --dump mode is used by tooling to regenerate the golden file after
// an intentional output change. Human approval (APPROVED_BY=...) is
// required before the comparison mode will run.

#include "simcom/simcom.hpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string read_file(const char* path) {
    std::FILE* f = std::fopen(path, "rb");
    if (!f) return std::string();
    std::string out;
    char buf[4096];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) out.append(buf, n);
    std::fclose(f);
    return out;
}

static bool write_file(const char* path, const std::string& content) {
    std::FILE* f = std::fopen(path, "wb");
    if (!f) return false;
    std::size_t w = std::fwrite(content.data(), 1, content.size(), f);
    std::fclose(f);
    return w == content.size();
}

static std::string build_actual() {
    using namespace simcom;
    std::string actual;
    char buf[16384];

    for (std::size_t i = 0; i < total_modules(); ++i) {
        actual += "===== module_to_json: ";
        actual += lookup_by_index(i)->name;
        actual += " =====\n";
        std::size_t n = module_to_json(lookup_by_index(i), buf, sizeof(buf));
        actual.append(buf, n);
        actual += "\n";
    }

    static const char* pairs[][2] = {
        {"SIM800L","SIM800H"},
        {"SIM800C","SIM800C-DS"},
        {"SIM808","SIM868"},
    };
    for (auto& p : pairs) {
        actual += "===== compare_modules: ";
        actual += p[0];
        actual += " | ";
        actual += p[1];
        actual += " =====\n";
        std::size_t n = compare_modules(lookup_by_name(p[0]),
                                        lookup_by_name(p[1]),
                                        buf, sizeof(buf));
        actual.append(buf, n);
        actual += "\n";
    }
    return actual;
}

int main(int argc, char** argv) {
    std::string actual = build_actual();

    // --dump <path> : write actual output, exit 0
    if (argc >= 3 && std::strcmp(argv[1], "--dump") == 0) {
        if (!write_file(argv[2], actual)) {
            std::fprintf(stderr, "failed to write %s\n", argv[2]);
            return 1;
        }
        std::printf("Dumped golden to %s (%zu bytes)\n", argv[2], actual.size());
        return 0;
    }

    // Comparison mode: requires APPROVED_BY in README.md
    std::string readme = read_file("test/golden/README.md");
    if (readme.empty()) {
        std::fprintf(stderr, "golden README.md missing; test_golden skipped (exit 2)\n");
        return 2;
    }

    // Approved iff "APPROVED_BY=" appears AND is followed by at least one
    // non-newline character before the next newline.
    bool approved = false;
    std::size_t pos = readme.find("APPROVED_BY=");
    while (pos != std::string::npos) {
        std::size_t valStart = pos + std::strlen("APPROVED_BY=");
        std::size_t valEnd = readme.find_first_of("\r\n", valStart);
        if (valEnd == std::string::npos) valEnd = readme.size();
        if (valEnd > valStart) { approved = true; break; }
        pos = readme.find("APPROVED_BY=", valStart);
    }
    if (!approved) {
        std::fprintf(stderr, "golden README.md not approved; test_golden skipped (exit 2)\n");
        return 2;
    }

    std::string expected = read_file("test/golden/expected_outputs.txt");
    if (expected.empty()) {
        std::fprintf(stderr, "golden expected_outputs.txt missing or empty; exit 2\n");
        return 2;
    }

    if (actual == expected) {
        std::printf("golden test passed.\n");
        return 0;
    }
    std::fprintf(stderr, "golden test FAILED\n");
    return 1;
}