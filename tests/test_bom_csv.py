from pathlib import Path
import sys

sys.path.append(str(Path(__file__).resolve().parents[1] / "scripts"))

import bom_check


def test_bom_columns_exist():
    issues = bom_check.check_bom(Path(__file__).resolve().parents[1] / "docs" / "BOM.csv")
    assert issues
    assert any("TBD" in issue.message for issue in issues)

