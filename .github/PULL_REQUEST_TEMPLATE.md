## Description

<!-- Describe your changes in detail. -->

## Related Issue

<!-- Link the issue this PR closes, e.g. "Closes #42". -->

Closes #

## Type of Change

- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that changes existing behavior)
- [ ] Documentation update
- [ ] Data update (YAML in `data/`)
- [ ] Tooling / CI change
- [ ] Refactor / chore

## Checklist

- [ ] I have read the [CONTRIBUTING.md](../CONTRIBUTING.md) guide.
- [ ] My code follows the project style (LF endings, UTF-8 without BOM).
- [ ] I did **not** hand-edit `include/simcom/simcom_database.hpp`.
- [ ] If I edited `data/*.yaml`, I ran:
  - [ ] `python tools/validate_yaml.py --strict`
  - [ ] `python tools/generate_cpp.py`
  - [ ] `python tools/export_markdown.py`
- [ ] I ran the full test suite: `ctest --test-dir build --output-on-failure`.
- [ ] All tests pass (or the failing ones are justified in the description).
- [ ] I added tests that prove my fix is effective or my feature works.
- [ ] I updated the documentation where relevant.
- [ ] I updated `CHANGELOG.md` under `[Unreleased]`.
- [ ] `tools/generate_cpp.py` remains deterministic (byte-identical output).

## How Has This Been Tested?

<!-- Provide details about the test environment. -->

- OS / compiler:
- CMake version:
- Python version:

## Screenshots / Logs

<!-- If applicable. -->