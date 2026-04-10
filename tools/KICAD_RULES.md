# KiCad Rule Notes

## Mandatory Checks Before Layout Release

- Enforce isolation keepout and review creepage manually.
- Assign `USB3_SS` class to all `SS_*` nets.
- Assign `POWER_HI` class to:
  - `POE_*`
  - `BUS_*`
  - `VBUS_SRC`
- Review copper-to-edge at the cable-exit side.
- Place stitching vias intentionally around shield/chassis strategy.

## Manual Review Items

- No noisy switch-node copper beneath AX88279A or SuperSpeed pairs
- No accidental primary/secondary copper overlap across the barrier
- Thermal-via fields under:
  - hot-swap FET
  - primary converter FETs
  - secondary inductor / power FET region

