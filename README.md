# PoE-to-USB-C 2.5GbE Pro Adapter v1

Private internal repository for the `PoE-to-USB-C-2.5GbE-Pro-Adapter-v1` product program. This repository is the canonical engineering scaffold for hardware, firmware, test planning, and bring-up of a boutique PoE-powered USB-C Ethernet/power adapter.

## Status

- Phase: architecture scaffold
- Hardware maturity: pre-schematic review
- Firmware maturity: HAL-based control skeleton
- Manufacturing maturity: candidate BOM only

## Product Intent

This device accepts `2.5GbE + PoE` on RJ45 and delivers:

- `USB 3.x Ethernet` to the host over a captive USB-C cable
- `USB-C PD source power` up to a dynamic ceiling based on available PoE and thermal state
- On-device telemetry via `0.96" OLED + two buttons`

Primary v1 targets:

- `65W continuous / 70W burst` user-facing output target
- `802.3af/at/bt` standards support
- advanced `48V passive` compatibility mode for selected ecosystems
- `STM32G0B1` control plane
- `ASIX AX88279A` USB-to-2.5GbE data path

## Repository Map

- [`docs/`](docs): internal specification, architecture notes, BOM, thermal analysis, and validation plan
- [`hardware/`](hardware): KiCad project scaffold, mechanical notes, and release placeholders
- [`firmware/`](firmware): STM32CubeIDE project skeleton, HAL-based modules, and alternative CMake flow
- [`scripts/`](scripts): engineering helper scripts for BOM checks, PDO calculation, and thermal-envelope estimation
- [`tests/`](tests): host-side policy and parser tests
- [`tools/`](tools): process and CAD/tooling support files

## Quick Start

### 1. Review the internal spec

Start with:

- [`docs/SPEC.md`](docs/SPEC.md)
- [`docs/HARDWARE_ARCH.md`](docs/HARDWARE_ARCH.md)
- [`docs/FIRMWARE_ARCH.md`](docs/FIRMWARE_ARCH.md)

### 2. Hardware workflow

1. Open [`hardware/kicad/poe-usbc-25g-pro.kicad_pro`](hardware/kicad/poe-usbc-25g-pro.kicad_pro) in KiCad 8.
2. Treat the schematic and PCB as architecture scaffolds, not release-ready design data.
3. Replace placeholder pin-level detail and footprint ownership before ordering boards.

### 3. Firmware workflow

1. Import [`firmware/STM32CubeIDE`](firmware/STM32CubeIDE) into STM32CubeIDE.
2. Review module boundaries in [`docs/FIRMWARE_ARCH.md`](docs/FIRMWARE_ARCH.md).
3. Fill in STM32CubeMX-generated startup, clock tree, HAL init, and peripheral handles before hardware bring-up.

### 4. Scripted helpers

```bash
cd poe-usbc-25g-pro-v1
python3 scripts/pdo_calc.py --class bt_type4 --temp-c 42 --profile pro
python3 scripts/thermal_envelope.py --input-w 71 --ambient-c 35
python3 scripts/bom_check.py docs/BOM.csv
```

## Safety And Scope Notes

- This repository contains architecture and scaffold material only.
- The KiCad and firmware projects are intentionally coherent but incomplete at production detail level.
- `48V passive` mode is treated as an explicit advanced capability and must never be enabled by default in shipping firmware.
- Thermal and power-limiting logic is mandatory, not optional polish.

## BOM Summary

Critical architectural components:

- PoE front end: `ADI LTC9105`, `LT4321`, hot-swap FET, isolated telemetry bridge
- Isolated converter: `MAX5974C`-class forward controller / equivalent implementation placeholder
- USB-C PD: `TPS26750 + BQ25756`
- Data path: `AX88279A`
- Control/UI: `STM32G0B1`, `INA228`, `TMP117 x2`, `SSD1306 OLED`

See [`docs/BOM.csv`](docs/BOM.csv) for candidate sourcing.

## Build Notes

### Firmware

- Toolchain target: `arm-none-eabi-gcc`
- MCU family: `STM32G0`
- HAL integration expected
- DFU handoff hooks are scaffolded but not production-tested

### Hardware

- Target CAD: `KiCad 8`
- PCB stackup assumption: `6-layer`
- USB high-speed constraints and isolation spacing must be revalidated before layout release

## Internal Use

This repository is confidential internal engineering material. Do not publish, mirror, or reuse externally without explicit written approval.

