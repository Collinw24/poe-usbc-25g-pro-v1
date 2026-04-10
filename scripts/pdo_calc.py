#!/usr/bin/env python3
"""CLI mirror of the firmware PD policy."""

from __future__ import annotations

import argparse
import json
from dataclasses import asdict, dataclass

STANDARD_HEADROOM = 25
PRO_HEADROOM = 15
SOFT_DERATE = 20
CLAMP_POWER_MW = 30_000


@dataclass
class Offer:
    voltage_mv: int
    current_ma: int
    is_pps: bool = False
    is_epr: bool = False


def base_budget_for_source(source: str) -> int:
    table = {
        "af": 10_000,
        "at": 22_500,
        "bt_type3": 45_000,
        "bt_type4": 71_300,
        "passive_48v": 80_000,
        "unknown": 5_000,
    }
    return table[source]


def calculate_budget(source: str, temp_c: int, profile: str, allocated_mw: int | None = None) -> int:
    budget = base_budget_for_source(source)
    if allocated_mw is not None:
        budget = min(budget, allocated_mw)

    if temp_c >= 90:
        return 0
    if temp_c >= 80:
        budget = CLAMP_POWER_MW
    elif temp_c >= 70:
        budget = int(budget * (100 - SOFT_DERATE) / 100)

    reserve = PRO_HEADROOM if profile == "pro" else STANDARD_HEADROOM
    return int(budget * (100 - reserve) / 100)


def build_offers(source: str, temp_c: int, profile: str, allocated_mw: int | None = None) -> dict:
    budget_mw = calculate_budget(source, temp_c, profile, allocated_mw)
    offers: list[Offer] = []

    if budget_mw <= 0:
        return {"budget_mw": 0, "offers": []}

    offers.append(Offer(5_000, 2_000))

    if budget_mw >= 15_000:
        offers.append(Offer(9_000, 2_000))
    if budget_mw >= 22_500:
        offers.append(Offer(15_000, 1_500))
    if budget_mw >= 45_000:
        current_ma = min(3_250, max(2_250, budget_mw // 20))
        offers.append(Offer(20_000, current_ma))
    if budget_mw >= 60_000:
        offers[-1] = Offer(20_000, 3_250)
    if budget_mw >= 63_000 and profile == "pro" and source in {"bt_type4", "passive_48v"}:
        offers.append(Offer(28_000, 2_250, is_epr=True))

    return {"budget_mw": budget_mw, "offers": [asdict(offer) for offer in offers]}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--class", dest="source", choices=["af", "at", "bt_type3", "bt_type4", "passive_48v", "unknown"], required=True)
    parser.add_argument("--temp-c", type=int, required=True)
    parser.add_argument("--profile", choices=["standard", "pro"], default="standard")
    parser.add_argument("--allocated-mw", type=int)
    args = parser.parse_args()

    print(json.dumps(build_offers(args.source, args.temp_c, args.profile, args.allocated_mw), indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
