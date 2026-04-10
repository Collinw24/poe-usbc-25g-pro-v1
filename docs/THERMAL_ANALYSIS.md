# Thermal Analysis

## Purpose

This document sets a conservative thermal envelope for the rev-A architecture. It is not a CFD report. It is an engineering sizing note to keep the enclosure and power-stage assumptions honest.

## Targets

- User-facing target: `65W continuous`, `70W burst`
- Ambient design points: `25C`, `35C`
- Cooling: passive only
- Enclosure: CNC aluminum primary shell with local polycarbonate window

## Estimated Loss Buckets

| Domain | Estimate @ 65W out | Notes |
| --- | --- | --- |
| PoE bridge / front end | 1.5W-2.5W | ideal bridge helps materially |
| Primary converter losses | 3.0W-4.5W | depends on topology, transformer, switching devices |
| Secondary buck-boost / PD path | 2.5W-4.0W | worst near top voltage and current |
| USB NIC + support rails | 1.5W-2.5W | traffic and PHY dependent |
| MCU, OLED, sensors | 0.3W-0.8W | relatively minor |
| Total internal dissipation | 8.8W-14.3W | enclosure must handle this passively |

## Hot Components

Primary expected hot spots:

- ideal bridge controller / FETs
- hot-swap FET
- primary converter switching devices
- transformer
- secondary buck-boost inductor and switching FETs
- USB-C source power-path components

The OLED and MCU should be mechanically decoupled from the hottest copper masses where possible.

## Sensor Placement

Required sensors per product spec:

- `TMP117 #1`: near PoE hot-swap / primary hot region
- `TMP117 #2`: near secondary buck-boost inductor or source FET region

Recommended supplementary observability:

- use internal MCU ADC/NTC pads for non-calibrated debug points during bring-up

## Thermal Policy

| Hottest Sensor | Action |
| --- | --- |
| `< 70C` | no thermal derate |
| `>= 70C` | 20% max-power reduction |
| `>= 80C` | clamp to 30W class |
| `>= 90C` | disable PD source, latch fault |

## Mechanical Implications

- Aluminum shell should directly contact at least:
  - primary hot-swap region
  - main converter switch or heat spreader
  - secondary buck-boost magnetic region
- Polycarbonate window area must not become the primary heat escape path.
- The USB-C captive cable exit needs strain relief without blocking local heat spreading copper or chassis interface.

## Risks

- `65W continuous` on pure `802.3bt Type 4` may be source-limited after cable and conversion losses.
- A too-small enclosure will force either uncomfortable case temperature or premature throttling.
- Thermal interaction between USB SuperSpeed routing and power copper can create placement tradeoffs.

## Validation Plan

- Bench thermal soak at `25C` and `35C`
- Infrared imaging of primary and secondary hot spots
- Sensor correlation against IR and thermocouple spot checks
- Case-temperature mapping near user touch points
- Derating verification under live Ethernet traffic and active PD load

