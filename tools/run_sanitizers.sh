#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
cd "$(dirname "$0")/.."
cmake -S . -B build-asan -DSIMCOM_ENABLE_ASAN=ON -DSIMCOM_ENABLE_UBSAN=ON
cmake --build build-asan
ctest --test-dir build-asan --output-on-failure