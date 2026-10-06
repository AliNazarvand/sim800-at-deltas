# Contributing

Thanks for your interest in \$RepoName\!

## Quick Start

\\\ash
git clone https://github.com/AliNazarvand/sim800-at-deltas.git
cd sim800-at-deltas
pip install -r requirements.txt
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
\\\

## Ground Rules

### Data is the single source of truth

- YAML under \data/\ is the single source of truth.
- **Never hand-edit \include/simcom/simcom_database.hpp\** — it is
  auto-generated from YAML by \	ools/generate_cpp.py\.

### Workflow after editing YAML

1. \python tools/validate_yaml.py --strict\
2. \python tools/generate_cpp.py\
3. \python tools/export_markdown.py\
4. \cmake --build build && ctest --test-dir build --output-on-failure\

### Coding rules (C++)

- C++17, header-only, \inline constexpr\ where possible.
- **No \loat\ or \double\ in \inline constexpr\** — use smaller integer
  units (e.g. \weight_mg\, \current_sleep_ua\).
- No dynamic allocation, no exceptions, no Arduino dependency in
  \include/simcom/*.hpp\.
- **Exception:** headers under \	est/\ are exempt from the no-malloc rule.
- New struct fields go **at the end** of the struct.

### Determinism

- LF line endings, UTF-8 without BOM, 4-space indent.
- \	ools/generate_cpp.py\ output must be byte-for-byte identical between
  runs. Verify with \Get-FileHash\ (Windows) or \sha256sum\ (POSIX).

### Module list

The set of modules is **fixed**:

\\\
SIM800L, SIM800C, SIM808, SIM868, SIM800A, SIM800F, SIM800H, SIM800, SIM800C-DS
\\\

Adding \SIM800M64\, \SIM800G\, \SIM800W\, \SIM840W\, or \SIM800V\ is
**not allowed** without dedicated datasheets.

## Commit Messages

- Use the imperative mood: \Add Foo\, not \Added Foo\.
- Reference issues: \Fix #42\.
- Keep the subject line under 72 characters.

## Pull Requests

1. Fork and create a topic branch (\eature/foo\, \ix/bar\).
2. Ensure all tests pass locally.
3. Fill out the PR template.
4. Link the related issue if any.

## Code of Conduct

By participating, you agree to follow our
[Code of Conduct](CODE_OF_CONDUCT.md).

## License

By contributing, you agree your contributions are licensed under the MIT
License (see [LICENSE](LICENSE)).

## Contact

- Maintainer: **Ali Nazarvand** — <ali.nazarvand@example.com>