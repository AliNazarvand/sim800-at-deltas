// SPDX-License-Identifier: MIT
#include "simcom/simcom.hpp"
#include "support/mini_json.hpp"
#include <cstdio>
#include <cstring>

static int g_failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond); ++g_failures; } \
} while (0)

int main() {
    using namespace simcom;
    for (std::size_t i = 0; i < total_modules(); ++i) {
        const ModuleSpec* m = lookup_by_index(i);
        char buf[8192];
        std::size_t n = module_to_json(m, buf, sizeof(buf));
        CHECK(n < sizeof(buf));

        mini_json::JsonValue* root = mini_json::parse(buf, n);
        CHECK(root != nullptr);
        if (!root) continue;

        CHECK(root->type == mini_json::Type::Object);
        // name
        std::string name;
        for (auto& kv : root->object) {
            if (kv.first == "name") name = kv.second->str;
        }
        CHECK(name == m->name);

        mini_json::free_json(root);
    }
    if (g_failures == 0) { std::printf("roundtrip tests passed.\n"); return 0; }
    std::printf("%d roundtrip failure(s).\n", g_failures);
    return 1;
}