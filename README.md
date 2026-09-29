# SIMCom SIM800 Series - Include All Module

Static, header-only C++17 database of SIMCom SIM800 Series module
specifications, targeting ESP32-WROOM-32 integrations.

## Supported Modules

- SIM800L
- SIM800C
- SIM808
- SIM868
- SIM800A
- SIM800F
- SIM800H
- SIM800
- SIM800C-DS

## Layout

    .
    |-- include/simcom/   C++ headers (header-only)
    |-- src/              Application sources
    |-- test/             Unit tests
    |-- tools/            Validate helper
    `-- docs/             Documentation

## Requirements

- C++17
- No dynamic allocation
- No Arduino-specific dependencies inside headers
- UTF-8 (no BOM) source encoding

## Usage

    python tools/validate.py
    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Related project: sim800-at-urc — full AT command / URC catalog.
