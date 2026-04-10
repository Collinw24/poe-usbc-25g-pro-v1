# Fusion / Mechanical CAD Notes

## Intent

Create a premium but engineering-forward enclosure that can actually absorb heat from a `65W`-class PoE-powered power converter.

## Internal Zoning

- Left side: RJ45 and primary PoE section
- Center: isolation barrier and transformer region
- Right side upper: USB-C cable exit and PD power stage
- Right side lower: MCU, OLED, buttons

## Thermal Interface Targets

- primary hot-swap region
- isolated converter switching region
- secondary buck-boost inductor / FET region

Use compressible gap pads to bridge these regions to the aluminum shell while protecting sensitive USB and UI regions from direct hot-spot conduction.

## Window Guidance

- Use a small smoked polycarbonate insert above the OLED/LED zone.
- Avoid large clear areas that reduce the shell's thermal cross-section.
- Window must not become the structural anchor for button forces.

## Assembly Guidance

- Prefer hidden screws or underside fasteners.
- Keep board insertion path simple enough for prototype rework.
- Maintain access to SWD/test pads in at least one prototype configuration.

