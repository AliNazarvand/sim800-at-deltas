#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Export data/*.yaml to exports/index.html (self-contained, deterministic)."""

from __future__ import annotations

import json
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    sys.stderr.write("ERROR: PyYAML not installed. Run: pip install -r requirements.txt\n")
    sys.exit(1)

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
DATA = ROOT / "data"
OUT  = ROOT / "exports"


HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>SIMCom SIM800 Series Database</title>
<style>
  body {{ font-family: system-ui, sans-serif; margin: 20px; background: #f5f5f5; }}
  h1 {{ color: #333; }}
  table {{ border-collapse: collapse; background: white; margin: 10px 0; }}
  th, td {{ border: 1px solid #ccc; padding: 6px 10px; font-size: 13px; text-align: left; }}
  th {{ background: #2c3e50; color: white; position: sticky; top: 0; }}
  tr:nth-child(even) {{ background: #f9f9f9; }}
  .tab {{ display: inline-block; padding: 6px 14px; background: #ddd; cursor: pointer; margin-right: 4px; }}
  .tab.active {{ background: #2c3e50; color: white; }}
  #search {{ padding: 6px; width: 300px; margin: 10px 0; }}
</style>
</head>
<body>
<h1>SIMCom SIM800 Series Database</h1>
<input id="search" placeholder="Filter by module name..." />
<div>
  <span class="tab active" data-tab="hw">HardwareSpec</span>
  <span class="tab" data-tab="feat">FeatureFlags</span>
  <span class="tab" data-tab="at">ATCommandDeltas</span>
</div>
<div id="tables"></div>
<script>
const MODULES = {data_json};
function render(tab) {{
  const container = document.getElementById('tables');
  container.innerHTML = '';
  const filter = document.getElementById('search').value.toLowerCase();
  const rows = MODULES.filter(m => m.name.toLowerCase().includes(filter));
  if (!rows.length) return;
  const keys = Object.keys(rows[0][tab]);
  let html = '<table><tr><th>name</th>' + keys.map(k => '<th>' + k + '</th>').join('') + '</tr>';
  for (const r of rows) {{
    html += '<tr><td>' + r.name + '</td>' +
      keys.map(k => '<td>' + (r[tab][k] === null ? 'null' : r[tab][k]) + '</td>').join('') +
      '</tr>';
  }}
  html += '</table>';
  container.innerHTML = html;
}}
document.querySelectorAll('.tab').forEach(t => {{
  t.addEventListener('click', () => {{
    document.querySelectorAll('.tab').forEach(x => x.classList.remove('active'));
    t.classList.add('active');
    render(t.dataset.tab);
  }});
}});
document.getElementById('search').addEventListener('input', () => {{
  const active = document.querySelector('.tab.active');
  if (active) render(active.dataset.tab);
}});
render('hw');
</script>
</body>
</html>
"""


def main() -> int:
    with (DATA / "modules.yaml").open("r", encoding="utf-8") as fh:
        mods = yaml.safe_load(fh)["modules"]
    with (DATA / "at_deltas.yaml").open("r", encoding="utf-8") as fh:
        ats = {a["module"]: a for a in yaml.safe_load(fh)["at_deltas"]}

    records = []
    for m in mods:
        at = dict(ats[m["name"]])
        at.pop("module", None)
        records.append({"name": m["name"], "hw": m["hw"], "feat": m["feat"], "at": at})

    data_json = json.dumps(records, indent=2, ensure_ascii=False)
    rendered = HTML_TEMPLATE.format(data_json=data_json)
    rendered = rendered.replace("\r\n", "\n").replace("\r", "\n")

    OUT.mkdir(parents=True, exist_ok=True)
    path = OUT / "index.html"
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write(rendered)
    print(f"Wrote {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())