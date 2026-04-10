from pathlib import Path
import sys

sys.path.append(str(Path(__file__).resolve().parents[1] / "scripts"))

import thermal_envelope


def test_dissipation_increases_with_power():
    low = thermal_envelope.build_report(45.0, 25.0)
    high = thermal_envelope.build_report(65.0, 25.0)
    assert high["total_dissipation_w"] > low["total_dissipation_w"]

