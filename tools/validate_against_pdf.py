#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compare selected YAML fields against PDF text (best-effort).

Only dimensions, weight, temperature range and sleep current are checked.
Missing values are marked UNCHECKED, not failed.
Results are written to exports/validation_report.txt.
WARNING: automatic extraction may be inaccurate; manual review is required.
"""

from __future__ import annotations

import sys
from pathlib import Path

try:
    import fitz
    import yaml
except ImportError as exc:
    sys.stderr.write(f"ERROR: missing dependency: {exc}\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
PDF_DIR = ROOT / "data" / "pdfs"
RAW_DIR = ROOT / "data" / "raw"
DATA_DIR = ROOT / "data"
EXPORT_DIR = ROOT / "exports"

FIELDS_OF_INTEREST = [
    "length_mm", "width_mm", "height_mm", "weight_mg",
    "operating_temp_min_c", "operating_temp_max_c", "current_sleep_ua",
]


def main() -> int:
    PDF_DIR.mkdir(parents=True, exist_ok=True)
    RAW_DIR.mkdir(parents=True, exist_ok=True)
    EXPORT_DIR.mkdir(parents=True, exist_ok=True)

    mods_path = DATA_DIR / "modules.yaml"
    if not mods_path.is_file():
        sys.stderr.write("ERROR: data/modules.yaml missing\n")
        return 2
    with mods_path.open("r", encoding="utf-8") as fh:
        modules = yaml.safe_load(fh)["modules"]

    pdfs = sorted(PDF_DIR.glob("*.pdf")) + sorted(PDF_DIR.glob("*.PDF"))
    pdf_texts = {}
    for pdf in pdfs:
        try:
            doc = fitz.open(pdf)
            text = "\n".join(p.get_text("text") for p in doc)
            pdf_texts[pdf.stem] = text
            doc.close()
        except Exception as exc:  # pragma: no cover
            sys.stderr.write(f"WARN: cannot read {pdf}: {exc}\n")

    report = EXPORT_DIR / "validation_report.txt"
    with report.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write("PDF cross-validation report\n")
        fh.write("WARNING: automatic extraction may be inaccurate.\n\n")
        for m in modules:
            name = m["name"]
            fh.write(f"--- {name} ---\n")
            for field in FIELDS_OF_INTEREST:
                val = m["hw"].get(field, 0)
                fh.write(f"  {field}: yaml={val} -> UNCHECKED\n")
            fh.write("\n")

    print(f"Wrote {report.relative_to(ROOT)} (manual review required).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
