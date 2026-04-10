#!/usr/bin/env python3
"""Basic integrity checker for the candidate BOM CSV."""

from __future__ import annotations

import csv
import pathlib
import sys
from dataclasses import dataclass


@dataclass
class BomIssue:
    row: int
    message: str


def check_bom(path: pathlib.Path) -> list[BomIssue]:
    issues: list[BomIssue] = []
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        required = {
            "Item",
            "Manufacturer",
            "MPN",
            "Qty",
            "Validation_Status",
            "Why_This_Part",
        }
        missing = required.difference(reader.fieldnames or [])
        if missing:
            issues.append(BomIssue(0, f"Missing required columns: {', '.join(sorted(missing))}"))
            return issues

        for index, row in enumerate(reader, start=2):
            if not row["Manufacturer"].strip():
                issues.append(BomIssue(index, "Empty Manufacturer"))
            if not row["MPN"].strip():
                issues.append(BomIssue(index, "Empty MPN"))
            if any("TBD" in str(value).upper() for value in row.values() if value is not None):
                issues.append(BomIssue(index, "Contains unresolved TBD placeholder"))
            if "TBD" in row["Validation_Status"].upper():
                issues.append(BomIssue(index, "Validation status still marked TBD"))
            if "TBD" in row["MPN"].upper():
                issues.append(BomIssue(index, "MPN still contains TBD marker"))
            if not row["Why_This_Part"].strip():
                issues.append(BomIssue(index, "Missing rationale"))

    return issues


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: bom_check.py <path-to-bom.csv>", file=sys.stderr)
        return 2

    path = pathlib.Path(argv[1])
    issues = check_bom(path)
    if not issues:
        print(f"{path}: OK")
        return 0

    print(f"{path}: {len(issues)} issue(s)")
    for issue in issues:
        print(f"  line {issue.row}: {issue.message}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
