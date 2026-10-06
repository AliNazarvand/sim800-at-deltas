# Contributing

## Rules

- YAML under `data/` is the single source of truth. Never hand-edit
  `include/simcom/simcom_database.hpp`.
- After editing YAML:
    1. `python tools/validate_yaml.py --strict`
    2. `python tools/generate_cpp.py`
    3. `python tools/export_markdown.py`
- Keep output deterministic: LF endings, UTF-8 without BOM, 4-space indent.
- Code must remain header-only, no dynamic allocation, no exceptions,
  no Arduino dependency in `include/simcom/*.hpp`.
- Tests must pass: `ctest --test-dir build --output-on-failure`.

## Code style

- C++17, `inline constexpr`.
- No `float`/`double` in `inline constexpr` (use smaller units).
- `.clang-format` (Google, IndentWidth=4).