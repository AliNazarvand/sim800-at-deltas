// SPDX-License-Identifier: MIT
#include "simcom/simcom.hpp"

static_assert(simcom::MODULE_COUNT == 9, "expected 9 modules");
static_assert(simcom::lookup_by_index(0)->name[0] == 'S', "first module name");
static_assert(static_cast<int>(simcom::JammingPin::PIN5)  == 1, "enum value");
static_assert(static_cast<int>(simcom::JammingPin::PIN29) == 2, "enum value");
static_assert(static_cast<int>(simcom::JammingPin::PIN63) == 3, "enum value");
static_assert(static_cast<int>(simcom::JammingPin::PIN67) == 4, "enum value");

int main() { return 0; }