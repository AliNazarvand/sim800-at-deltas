#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Extract raw tables from the 13 datasheet PDFs into data/raw/."""

from __future__ import annotations

import sys
from pathlib import Path

try:
    import fitz  # PyMuPDF
except ImportError:
    sys.stderr.write("ERROR: PyMuPDF not installed. Run: pip install -r requirements.txt\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
PDF_DIR = ROOT / "data" / "pdfs"
RAW_DIR = ROOT / "data" / "raw"


def main() -> int:
    PDF_DIR.mkdir(parents=True, exist_ok=True)
    RAW_DIR.mkdir(parents=True, exist_ok=True)

    pdfs = sorted(PDF_DIR.glob("*.pdf")) + sorted(PDF_DIR.glob("*.PDF"))
    if not pdfs:
        print(f"No PDFs found in {PDF_DIR}. Place datasheets there and rerun.")
        return 0

    for pdf in pdfs:
        try:
            doc = fitz.open(pdf)
        except Exception as exc:  # pragma: no cover
            sys.stderr.write(f"WARN: cannot open {pdf}: {exc}\n")
            continue
        out_path = RAW_DIR / (pdf.stem + ".txt")
        with out_path.open("w", encoding="utf-8", newline="\n") as fh:
            for i, page in enumerate(doc):
                fh.write(f"===== PAGE {i + 1} =====\n")
                fh.write(page.get_text("text"))
                fh.write("\n")
        print(f"Extracted {pdf.name} -> {out_path.relative_to(ROOT)}")
        doc.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())
