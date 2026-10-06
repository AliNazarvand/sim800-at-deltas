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

```cmake
include(FetchContent)
FetchContent_Declare(simcom
    GIT_REPOSITORY https://github.com/AliNazarvand/sim800-at-deltas.git
    GIT_TAG        v1.0.0)  # Pin to a release tag, not main
FetchContent_MakeAvailable(simcom)

target_link_libraries(my_app PRIVATE simcom)
```

### As an installed package

```cmake
find_package(simcom REQUIRED)
target_link_libraries(my_app PRIVATE simcom::simcom)
```

### Via pkg-config

```bash
pkg-config --cflags --libs simcom
```

### Header include

```cpp
#include "simcom/simcom.hpp"

const simcom::ModuleSpec* m = simcom::lookup_by_name("SIM808");
if (m && m->feat.has_gps) {
    // ...
}
```

> **⚠️ Important:** Use `GIT_TAG v1.0.0` instead of `main` in production to avoid breaking changes.

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [API Reference](docs/API.md) | Complete Doxygen-generated API documentation. |
| [Data Pipeline](docs/DATA_PIPELINE.md) | How YAML files are processed into C++ headers. |
| [Contributing Guide](CONTRIBUTING.md) | How to contribute to the project. |
| [Code of Conduct](CODE_OF_CONDUCT.md) | Community guidelines and expectations. |
| [Security Policy](SECURITY.md) | How to report security vulnerabilities. |

---

## 🏗️ Repository Layout

<details>
<summary>Click to expand file tree</summary>

```
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
```

</details>

---

## ⚙️ Requirements

- **C++17** compiler (GCC 9+, Clang 10+, MSVC 2019+)
- **CMake** 3.16+
- **Python** 3.10+ (for tools and code generation)
- **No dynamic allocation**, no exceptions, no Arduino dependency in headers.

Install Python dependencies:

```bash
pip install -r requirements.txt
```

---

## 🔄 Data Pipeline

<details>
<summary>Click to expand pipeline details</summary>

1. Human developers download the 13 datasheet PDFs into `data/pdfs/` (gitignored).
2. `python tools/extract_pdf.py` extracts raw tables into `data/raw/`.
3. Humans review the raw values and edit `data/*.yaml`.
4. `python tools/validate_yaml.py --strict` validates the YAML.
5. `python tools/generate_cpp.py` regenerates `include/simcom/simcom_database.hpp`.
6. `python tools/export_json.py`, `export_csv.py`, `export_markdown.py` produce `exports/` artifacts.

</details>

---

## 🧪 Build and Test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Expected: **8/8 tests pass** (after setting `APPROVED_BY` in `test/golden/README.md`).

> **Note:** The `APPROVED_BY` field is a golden test approval marker used to verify that expected test outputs have been reviewed and signed off by a maintainer.

### Available CMake targets

| Target | Description |
|--------|-------------|
| `simcom` | Header-only INTERFACE library |
| `simcom_demo` | Example program |
| `simcom_generate` | Run `tools/generate_cpp.py` |
| `simcom_validate` | Run `tools/validate.py` + `validate_yaml.py --strict` |
| `simcom_export` | Run all exporters |

---

## 🗺️ Roadmap

- [x] Core header-only database
- [x] CMake FetchContent integration
- [x] pkg-config support
- [x] CI pipeline with automated tests
- [x] JSON Schema validation
- [ ] Doxygen API documentation
- [ ] Published releases with semantic versioning
- [ ] Additional module families (SIM7000, SIM7600)
- [ ] PlatformIO registry publication

---

## ❓ FAQ

<details>
<summary><strong>Q: Can I edit the generated header manually?</strong></summary>

No. The header `include/simcom/simcom_database.hpp` is auto-generated. All changes must be made in the YAML source files and regenerated via `tools/generate_cpp.py`.

</details>

<details>
<summary><strong>Q: Does this library require dynamic memory allocation?</strong></summary>

No. The library is designed for embedded systems with strict memory constraints. All data is static and known at compile time.

</details>

<details>
<summary><strong>Q: How do I add a new module to the database?</strong></summary>

1. Add the datasheet PDF to `data/pdfs/`.
2. Run `tools/extract_pdf.py`.
3. Edit the YAML file in `data/` with the extracted values.
4. Run `tools/validate_yaml.py --strict`.
5. Run `tools/generate_cpp.py` to regenerate the header.

See the [Data Pipeline](#-data-pipeline) section for full details.

</details>

---

## 🤝 Contributing

We welcome contributions! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

### Quick Contribution Flow

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

All contributors are expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

---

## 🔒 Security

Please report security issues per [SECURITY.md](SECURITY.md). **Do not open public issues for security vulnerabilities.**

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- SIMCom for the SIM800 Series documentation
- The ESP32 community for testing and feedback
- All contributors who have helped improve this project

---

## 📝 Citation

If you use this project in academic work, please cite:

```bibtex
@misc{sim800-at-deltas,
  author = {Nazarvand, Ali},
  title = {SIMCom SIM800 Series — Static Module Database},
  year = {2024},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/AliNazarvand/sim800-at-deltas}}
}
```

---

## 👤 Author

**Ali Nazarvand**

- GitHub: [@AliNazarvand](https://github.com/AliNazarvand)
- Email: [Ali.Nazarvand@Gmail.com](mailto:ali.nazarvand@example.com) *(replace with real email)*

---

<div align="center">

**[⬆ Back to Top](#simcom-sim800-series--static-module-database)**

Made with ❤️ for the embedded community

</div>
