#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Generate exports/sbom.json in SPDX 2.3 format (Python deps only)."""

from __future__ import annotations

import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
OUT  = ROOT / "exports"

DEPENDENCIES = [
    {"name": "PyMuPDF",    "version": "1.23.0", "license": "AGPL-3.0"},
    {"name": "Jinja2",     "version": "3.1.0",  "license": "BSD-3-Clause"},
    {"name": "PyYAML",     "version": "6.0",    "license": "MIT"},
    {"name": "jsonschema", "version": "4.20.0", "license": "MIT"},
    {"name": "hypothesis", "version": "6.90.0", "license": "MPL-2.0"},
    {"name": "pytest",     "version": "7.4.0",  "license": "MIT"},
]


def main() -> int:
    OUT.mkdir(parents=True, exist_ok=True)
    packages = []
    for dep in sorted(DEPENDENCIES, key=lambda d: d["name"].lower()):
        packages.append({
            "SPDXID": f"SPDXRef-Package-{dep['name']}",
            "name": dep["name"],
            "versionInfo": dep["version"],
            "downloadLocation": "NOASSERTION",
            "licenseConcluded": dep["license"],
            "licenseDeclared": dep["license"],
            "copyrightText": "NOASSERTION",
        })
    sbom = {
        "spdxVersion": "SPDX-2.3",
        "dataLicense": "CC0-1.0",
        "SPDXID": "SPDXRef-DOCUMENT",
        "name": "simcom-sim800-series",
        "documentNamespace": "https://example.invalid/simcom-sim800-series/sbom",
        "creationInfo": {
            "created": "1970-01-01T00:00:00Z",
            "creators": ["Tool: tools/generate_sbom.py"],
        },
        "packages": packages,
    }
    path = OUT / "sbom.json"
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        json.dump(sbom, fh, indent=2, sort_keys=False, ensure_ascii=False)
        fh.write("\n")
    print(f"Wrote {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())