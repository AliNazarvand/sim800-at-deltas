<!-- ===== HEADER ===== -->
<div align="center">

# SIMCom SIM800 Series — Static Module Database

**A static, header-only C++17 database of SIMCom SIM800 Series module specifications, targeting ESP32-WROOM-32 integrations.**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![Header-Only](https://img.shields.io/badge/header--only-yes-brightgreen.svg?style=for-the-badge)]()
[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-orange.svg?style=for-the-badge&logo=platformio)]()
[![GitHub release](https://img.shields.io/github/v/release/AliNazarvand/sim800-at-deltas?style=for-the-badge&color=green&label=Release)](https://github.com/AliNazarvand/sim800-at-deltas/releases)
[![CI](https://github.com/AliNazarvand/sim800-at-deltas/actions/workflows/ci.yml/badge.svg?style=for-the-badge)](https://github.com/AliNazarvand/sim800-at-deltas/actions/workflows/ci.yml)

[Quick Start](#-quick-start) · [Features](#-features) · [Modules](#-supported-modules) · [Docs](#-documentation) · [Contributing](#-contributing) · [License](#-license)

</div>

---

## 📖 Overview

This project provides a **data-driven, zero-overhead C++17 header-only library** that exposes a static database of SIMCom SIM800 Series module specifications. It is designed for developers building firmware for ESP32-WROOM-32 and similar microcontrollers who need reliable, compile-time access to module capabilities without dynamic allocation.

The project is **data-driven**: the single source of truth is a set of YAML files under `data/`. The C++ header `include/simcom/simcom_database.hpp` is **auto-generated** from those YAML files and must not be edited by hand.

> **Key Design Principles:**
> - 🚫 No dynamic allocation
> - 🚫 No exceptions
> - 🚫 No Arduino dependency in headers
> - ✅ Header-only integration (CMake, pkg-config, FetchContent)

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| **Header-Only** | Zero build configuration — just include and use. |
| **C++17** | Modern standards with `constexpr` and structured bindings. |
| **Static Database** | All specifications known at compile time — no runtime lookup cost. |
| **YAML-Driven** | Human-readable YAML source of truth, auto-generated C++ header. |
| **Zero Dependencies** | No external libraries required in headers. |
| **ESP32-Ready** | Optimized for ESP32-WROOM-32 and similar MCUs. |
| **Multi-Format Export** | Generated JSON, CSV, and Markdown artifacts. |
| **Strict Validation** | JSON Schema validation for all YAML data. |

---

## 📦 Supported Modules

| # | Module     | Package  | Pins | GPS | BT  | FM  | Audio |
|---|------------|----------|-----:|:---:|:---:|:---:|:-----:|
| 1 | SIM800L    | LGA      |  88  |  -  |  -  |  ✓  |   ✓   |
| 2 | SIM800C    | SMT      |  42  |  -  |  ✓  |  -  |   ✓   |
| 3 | SIM808     | SMT      |  68  |  ✓  |  ✓  |  -  |   ✓   |
| 4 | SIM868     | SMT+LGA  |  77  |  ✓  |  ✓  |  -  |   ✓   |
| 5 | SIM800A    | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 6 | SIM800F    | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 7 | SIM800H    | LGA      |  88  |  -  |  ✓  |  ✓  |   ✓   |
| 8 | SIM800     | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 9 | SIM800C-DS | SMT+LGA  |  77  |  -  |  ✓  |  ✓  |   ✓   |

---

## 🚀 Quick Start

### As a CMake subproject (FetchContent)































# SIMCom SIM800 Series — Static Module Database

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Header-Only](https://img.shields.io/badge/header--only-yes-brightgreen.svg)]()
[![CI](https://github.com/AliNazarvand/sim800-at-deltas/actions/workflows/ci.yml/badge.svg)](https://github.com/AliNazarvand/sim800-at-deltas/actions/workflows/ci.yml)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-compatible-orange.svg)]()

> A static, header-only C++17 database of SIMCom SIM800 Series module
> specifications, targeting ESP32-WROOM-32 integrations.

The project is **data-driven**: the single source of truth is a set of YAML
files under data/. The C++ header include/simcom/simcom_database.hpp is
**auto-generated** from those YAML files and must not be edited by hand.

---

## Supported Modules

| # | Module     | Package  | Pins | GPS | BT  | FM  | Audio |
|---|------------|----------|-----:|:---:|:---:|:---:|:-----:|
| 1 | SIM800L    | LGA      |  88  |  -  |  -  |  ✓  |   ✓   |
| 2 | SIM800C    | SMT      |  42  |  -  |  ✓  |  -  |   ✓   |
| 3 | SIM808     | SMT      |  68  |  ✓  |  ✓  |  -  |   ✓   |
| 4 | SIM868     | SMT+LGA  |  77  |  ✓  |  ✓  |  -  |   ✓   |
| 5 | SIM800A    | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 6 | SIM800F    | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 7 | SIM800H    | LGA      |  88  |  -  |  ✓  |  ✓  |   ✓   |
| 8 | SIM800     | SMT      |  68  |  -  |  ✓  |  -  |   ✓   |
| 9 | SIM800C-DS | SMT+LGA  |  77  |  -  |  ✓  |  ✓  |   ✓   |

---

## Quick Start

### As a CMake subproject (FetchContent)

\\\cmake
include(FetchContent)
FetchContent_Declare(simcom
    GIT_REPOSITORY https://github.com/AliNazarvand/sim800-at-deltas.git
    GIT_TAG        main)
FetchContent_MakeAvailable(simcom)

target_link_libraries(my_app PRIVATE simcom)
\\\

### As an installed package

\\\cmake
find_package(simcom REQUIRED)
target_link_libraries(my_app PRIVATE simcom::simcom)
\\\

### Via pkg-config

\\\ash
pkg-config --cflags --libs simcom
\\\

### Header include

\\\cpp
#include "simcom/simcom.hpp"

const simcom::ModuleSpec* m = simcom::lookup_by_name("SIM808");
if (m && m->feat.has_gps) {
    // ...
}
\\\

---

## Repository Layout

\\\
.
├── include/simcom/     C++ headers (header-only)
├── src/                Demo application
├── test/               Unit tests (C++ and Python)
├── tools/              Python code generation + validation
│   └── schemas/        JSON Schemas for the YAML files
├── data/               YAML source of truth (modules, deltas, xrefs)
├── docs/               Documentation
├── exports/            Generated JSON/CSV/Markdown (gitignored)
├── CMakeLists.txt
├── requirements.txt    Python dependencies
└── LICENSE
\\\

---

## Requirements

- **C++17** compiler (GCC 9+, Clang 10+, MSVC 2019+)
- **CMake** 3.16+
- **Python** 3.10+ (for tools and code generation)
- **No dynamic allocation**, no exceptions, no Arduino dependency in headers.

Install Python dependencies:

\\\ash
pip install -r requirements.txt
\\\

---

## Data Pipeline

1. Human developers download the 13 datasheet PDFs into data/pdfs/
   (gitignored).
2. \python tools/extract_pdf.py\ extracts raw tables into \data/raw/\.
3. Humans review the raw values and edit \data/*.yaml\.
4. \python tools/validate_yaml.py --strict\ validates the YAML.
5. \python tools/generate_cpp.py\ regenerates
   \include/simcom/simcom_database.hpp\.
6. \python tools/export_json.py\, \xport_csv.py\, \xport_markdown.py\
   produce \xports/\ artifacts.

---

## Build and Test

\\\ash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
\\\

Expected: **8/8 tests pass** (after setting \APPROVED_BY\ in
\	est/golden/README.md\).

### Available CMake targets

- \simcom\              — header-only INTERFACE library
- \simcom_demo\         — example program
- \simcom_generate\     — run \	ools/generate_cpp.py\
- \simcom_validate\     — run \	ools/validate.py\ + \alidate_yaml.py --strict\
- \simcom_export\       — run all exporters

---

## AI vs. Human Responsibilities

- **AI Agent** — constructs the infrastructure (headers, tools, schemas,
  templates, sample YAML using only existing C++ database values).
- **Human Developer** — downloads the PDFs, extracts raw data, fills in real
  values in YAML, and runs the generator.

AI must never fabricate data from training memory; it may only transcribe
values already present in the current \simcom_database.hpp\.

---

## Related Projects

- [sim800-at-deltas](https://github.com/AliNazarvand/sim800-at-deltas) *(this repository)*
- [sim800-at-urc](https://github.com/AliNazarvand/sim800-at-urc)
- [sim800-capabilities](https://github.com/AliNazarvand/sim800-capabilities)

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) and
[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md).

All contributors are expected to follow our
[Code of Conduct](CODE_OF_CONDUCT.md).

---

## Security

Please report security issues per [SECURITY.md](SECURITY.md).

---

## License

MIT — see [LICENSE](LICENSE).

## Author

**Ali Nazarvand** — <ali.nazarvand@example.com>
