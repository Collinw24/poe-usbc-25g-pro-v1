from pathlib import Path
import sys

sys.path.append(str(Path(__file__).resolve().parents[1] / "scripts"))

import pdo_calc


def test_passive_pro_includes_epr():
    result = pdo_calc.build_offers("passive_48v", 35, "pro")
    assert result["budget_mw"] >= 63_000
    assert result["offers"][-1]["is_epr"] is True
    assert result["offers"][-1]["voltage_mv"] == 28_000


def test_soft_derate_reduces_budget():
    cool = pdo_calc.build_offers("bt_type4", 50, "standard")
    warm = pdo_calc.build_offers("bt_type4", 72, "standard")
    assert warm["budget_mw"] < cool["budget_mw"]


def test_shutdown_removes_offers():
    result = pdo_calc.build_offers("bt_type4", 92, "pro")
    assert result["budget_mw"] == 0
    assert result["offers"] == []
