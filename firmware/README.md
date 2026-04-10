# Firmware Notes

This directory contains the rev-A STM32 firmware scaffold for the adapter control plane.

## Scope

The firmware is responsible for:

- PoE source telemetry via isolated I2C
- output bus telemetry and temperature sensing
- OLED UI and button handling
- PD source policy generation
- thermal derating and latched-fault behavior
- DFU entry hooks

The firmware is **not** responsible for implementing USB Ethernet bridging. That remains in the AX88279A hardware path.

## Expected Bring-Up Order

1. Import `STM32CubeIDE/` into STM32CubeIDE.
2. Generate and reconcile CubeMX startup, clock, and HAL handle definitions.
3. Validate I2C buses, GPIO, OLED, and buttons.
4. Validate INA228 and TMP117 reads.
5. Validate isolated LTC9105 telemetry.
6. Enable fixed 5V PD source path.
7. Enable dynamic PDO control.

## DFU / Debug

- Primary debug path: SWD
- Recovery path: USB DFU handoff hooks in `dfu_hooks.*`
- Product mode remains Ethernet-only on the USB data path

