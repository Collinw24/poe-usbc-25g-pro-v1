#!/usr/bin/env python3
"""Quick thermal-envelope estimator for enclosure sizing discussions."""

from __future__ import annotations

import argparse
import json


def estimate_losses(output_power_w: float) -> dict[str, float]:
    return {
        "poe_front_end_w": round(output_power_w * 0.03, 2),
        "primary_converter_w": round(output_power_w * 0.055, 2),
        "secondary_pd_w": round(output_power_w * 0.045, 2),
        "usb_nic_w": round(max(1.5, output_power_w * 0.025), 2),
        "ui_control_w": 0.5,
    }


def total_dissipation(losses: dict[str, float]) -> float:
    return round(sum(losses.values()), 2)


def estimate_case_rise_c(total_loss_w: float, thermal_resistance_c_per_w: float = 4.2) -> float:
    return round(total_loss_w * thermal_resistance_c_per_w, 1)


def build_report(input_w: float, ambient_c: float) -> dict[str, float | dict[str, float]]:
    losses = estimate_losses(input_w)
    dissipation = total_dissipation(losses)
    case_rise = estimate_case_rise_c(dissipation)
    return {
        "output_power_w": input_w,
        "ambient_c": ambient_c,
        "losses_w": losses,
        "total_dissipation_w": dissipation,
        "estimated_case_temp_c": round(ambient_c + case_rise, 1),
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--input-w", type=float, required=True, help="Desired output power")
    parser.add_argument("--ambient-c", type=float, required=True)
    args = parser.parse_args()

    print(json.dumps(build_report(args.input_w, args.ambient_c), indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

