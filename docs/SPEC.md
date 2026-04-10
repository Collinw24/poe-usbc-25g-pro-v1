# PoE-to-USB-C 2.5GbE Pro Adapter v1

## Canonical Internal Product Spec

### Summary

- Build a boutique, instrumented, larger-than-normal inline adapter that takes `2.5GbE + PoE` on RJ45 and delivers `USB 3.x Ethernet + USB-C PD source` over a captive USB-C cable.
- Public story: `up to 65W continuous / 70W burst`, `2.5GbE`, OLED telemetry, and support for `802.3af/at/bt` plus selected enterprise `48V passive` ecosystems.
- Engineering story: treat vendor labels like Ubiquiti `PoE+++` as `IEEE 802.3bt Type 4`, not a new standard; support `48V passive` only as an explicit advanced mode.
- Product posture stays hacker-premium, not compliance-first: CNC aluminum body as heatsink, transparent polycarbonate window, passive cooling, no radio, no host app, USB personality is Ethernet only.

### Public Interfaces / Product Spec

- Network input: `1x RJ45 2.5GbE PoE PD`, supporting `802.3af`, `802.3at`, `802.3bt Type 3`, `802.3bt Type 4`, and `48V passive` advanced mode.
- Host output: fixed `20 cm` captive `USB-C male` cable, `USB 3.2 Gen 1` data, `USB PD source`, Ethernet function only on USB.
- UI: `0.96" 128x64 OLED`, `2 buttons`, one status LED behind the window.

Required OLED pages:

- Input page: PoE mode, allocated power, live input V/I/W, passive-mode warning.
- PD page: currently advertised PDOs/APDOs, active contract, cable EPR capability.
- Thermal page: primary temp, secondary temp, throttle state.
- Link page: Ethernet link state and negotiated speed.

Buttons:

- Left short press cycles pages.
- Right short press forces PD renegotiation.
- Right long press toggles `Standard` vs `Pro` power profile.
- Both buttons at boot enable `48V passive` mode for that session only.

Output policy:

- Always advertise only PDOs that fit live input budget and thermal state.
- `Standard` profile is conservative and clamps harder.
- `Pro` profile exposes the highest allowed PDOs and more aggressive thermal headroom.

Default PDO ladder by source class:

- `af`: 5V/2A max.
- `at`: 5V/3A, 9V/2A, 15V/1.5A.
- `bt Type 3`: up to 20V/2.25A.
- `bt Type 4` and qualifying passive/high-power inputs: up to 20V/3.25A and 28V EPR up to 2.25A when cable and thermal budget allow.

Public copy says `up to 65W`; firmware enforces lower ceilings automatically on weaker links or hot conditions.

### Implementation / BOM Direction

- PoE front end: `ADI LTC9105` for `802.3bt` telemetry and allocated-power readback, `LT4321` ideal bridge to cut dissipation, external hot-swap FET, and `ADuM1252` to isolate control telemetry from the secondary side.
- Isolated power stage: `MAX5974C`-class isolated forward converter targeting a `24V-28V secondary bus`, sized for aggressive `65W` operation with passive enclosure cooling.
- USB-C PD stage: `TI TPS26750` as the PD3.1 source controller plus `BQ25756` as the programmable reverse-mode buck-boost stage so the design can tell a real `EPR` story without custom PD stack work.
- USB NIC: `ASIX AX88279A` as the preferred `USB 3.2 Gen 1 to 2.5GbE` bridge because its public docs/reference hardware and driverless positioning are stronger than the usual consumer-dongle parts.
- MCU: `STM32G0B1` class MCU, not ESP32, because v1 does not need RF and should stay focused on control/UI/bring-up with built-in USB DFU.
- Sensors and UI: `TMP117 x2`, one primary and one secondary; `INA228` on the output side for delivered-power telemetry; `SSD1306` OLED; 2 tactile switches.
- Protection: dedicated ESD arrays on RJ45 and USB-C, surge-aware primary layout, cable-side current limiting, and a hard latch-out path for repeated PD fault/overtemp events.
- Mechanical target: `~95 x 42 x 18 mm` aluminum body with internal thermal pads to the bridge, hot-swap FET, isolated converter transformer/FETs, and secondary buck-boost magnetics; smoked polycarbonate window over OLED and indicator LED.
- Prototype cost target: accept a low-volume electronics BOM around `~$70-$110` before enclosure and assembly; do not optimize part count yet.

### Firmware / Behavior

- MCU lives on the secondary side and owns UI, PD policy, thermal policy, passive-mode gating, and logging.
- MCU polls `LTC9105` over isolated I2C for source class, allocated power, and live input telemetry; polls `INA228` for actual USB-C delivery; polls both `TMP117`s for thermal state.

Throttle policy:

- Soft derate starts at `70C` hottest sensor.
- Hard derate at `80C` drops advertised power to `30W` class.
- Shutdown/latch fault at `90C`.

- `Standard` profile reserves more headroom for conversion and NIC load.
- `Pro` profile is allowed to push the top `20V` and `28V` PDOs when source type, live power, and temperature permit.
- Cable-length/TDR diagnostics are explicitly out of v1.
- Real-time throughput on the OLED is best treated as rev-B unless AX88279A counters are proven accessible without adding host software.

### Test Plan

- Validate enumeration and charging with at least one `MacBook`, one `Windows USB-C laptop`, one `Linux host`, and one `iPad/USB-C tablet`.
- Validate source matrix against `af`, `at`, `bt Type 3`, `bt Type 4`, and a `48V passive` Ubiquiti-style injector.
- Validate PD matrix with standard and e-marked captive cable assemblies, including `20V` and `28V` contracts.
- Run thermal soak at `25C` and `35C` ambient for `45W`, `60W`, and max-allowed `Pro` mode output while passing live Ethernet traffic.
- Fault tests: unplug/replug under load, shorted VBUS, passive-mode wrong-source handling, repeated renegotiation, no-link/low-link-power states, and overtemp recovery.
- EMI/layout focus for proto bring-up: RJ45 front-end losses, isolation barrier leakage/spacing, USB 3 routing margin, and enclosure-ground strategy.

### Assumptions And Defaults

- Use vendor marketing labels like `PoE++` and `PoE+++` only in marketing/UI; engineering docs must map them back to `IEEE type/class` plus `48V passive`.
- Inference from source material: honest sustained `65W` on pure `bt Type 4` is tight once conversion and data-path losses are included, so v1 should rely on dynamic PDO limits instead of a single fixed guarantee.
- `48V passive` support is session-gated and visually obvious; it is not the default boot mode.
- The product stays mostly closed; the plan does not optimize for open-hardware publishing.

### References

- Ubiquiti PoE modes: https://help.ui.com/hc/en-us/articles/115000263008-PoE-Availability-and-Modes
- ADI LTC9105: https://www.analog.com/en/products/ltc9105.html
- TI TPS26750 datasheet: https://www.ti.com/lit/gpn/TPS26750
- TI BQ25756: https://www.ti.com/product/BQ25756
- ASIX AX88279A: https://www.asix.com.tw/tw/product/USBEthernet/Super-Speed_USB_Ethernet/AX88279A

## Engineering Clarifications

### Why This Stays Niche

This product class is commercially real but still niche because it combines:

- `802.3bt` or adjacent high-power PoE negotiation
- isolation and thermal management at `65W`-class output
- USB-C PD source behavior with dynamic power policy
- USB 3.x signal integrity in a thermally dense enclosure
- Ethernet bridge, UI, telemetry, and enclosure constraints in a dongle-like form

The architecture therefore treats `marketing wattage` and `standards wattage` separately.

### PoE+++ Reality Check

- There is no separate IEEE `PoE+++` standard beyond `802.3bt Type 4`.
- Ubiquiti and other vendors sometimes market `Type 4 / 100W-class switch ports` as `PoE+++`.
- This project supports that language in the UI and marketing layer only as a user-facing label.
- Engineering implementation must map all inputs to one of:
  - `802.3af`
  - `802.3at`
  - `802.3bt Type 3`
  - `802.3bt Type 4`
  - `48V passive advanced mode`

### Output Contract Philosophy

- The adapter must never over-advertise power relative to measured and classified input budget.
- The firmware owns the output contract table and may rewrite PDO/APDO sets whenever:
  - PoE class changes
  - measured output load shifts materially
  - thermal state changes
  - user toggles `Standard` or `Pro`
- `EPR` support is part of the product architecture and story, but not a promise that every source condition can sustain `28V` operation.

### Telemetry Philosophy

The OLED is not decorative. It is part of the product experience and must surface:

- input source identity
- available power
- actual PD offer set
- current PD contract
- temperatures
- Ethernet link rate
- warning states

`Traffic graphing` and `cable length estimation` remain explicitly out of v1 unless proven with zero compromise to the primary power/data design.

### Firmware Scope Boundary

The host sees Ethernet only. There is no user-facing CDC console or utility function in normal operation. Bring-up logging and recovery paths may exist through:

- SWD
- test pads
- DFU mode

### Mechanical Direction

Industrial design direction is a hybrid:

- predominantly CNC-machined aluminum for heat spreading and visual quality
- local transparent or smoked polycarbonate window to expose OLED/indicator region and hint at internal structure

The enclosure is intentionally larger than a commodity dongle to buy passive thermal margin.

## Internal Review Checklist

- [ ] Confirm final PoE PD controller choice against latest vendor availability and telemetry needs.
- [ ] Confirm isolated converter controller and transformer approach before schematic capture.
- [ ] Confirm whether `28V EPR` stays enabled in rev-A firmware or behind a feature flag.
- [ ] Confirm passive-mode electrical protection and labeling before prototype assembly.
- [ ] Confirm AX88279A driver behavior on macOS, Windows, Linux, and iPadOS target devices.

