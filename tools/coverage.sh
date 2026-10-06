#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
cd "$(dirname "$0")/.."
cmake -S . -B build-cov -DSIMCOM_ENABLE_COVERAGE=ON
cmake --build build-cov
ctest --test-dir build-cov --output-on-failure
if command -v gcovr >/dev/null 2>&1; then
  gcovr --root . --html --html-details -o build-cov/coverage.html
  echo "Coverage report: build-cov/coverage.html"
fi