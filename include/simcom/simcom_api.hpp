#ifndef SIMCOM_API_HPP
#define SIMCOM_API_HPP

#include "simcom_types.hpp"
#include "simcom_database.hpp"
#include <cstddef>

namespace simcom {

inline bool str_equal(const char* a, const char* b) noexcept {
    if (a == nullptr || b == nullptr) return false;
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) return false;
        ++a; ++b;
    }
    return *a == *b;
}

inline const ModuleSpec* lookup_by_name(const char* name) noexcept {
    if (name == nullptr) return nullptr;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (str_equal(MODULES[i].name, name)) return &MODULES[i];
    }
    return nullptr;
}

inline const ModuleSpec* lookup_by_index(std::size_t index) noexcept {
    if (index >= MODULE_COUNT) return nullptr;
    return &MODULES[index];
}

inline std::size_t total_modules() noexcept {
    return MODULE_COUNT;
}

inline const ModuleSpec* get_all() noexcept {
    return MODULES;
}

} // namespace simcom

#endif // SIMCOM_API_HPP
