# KiCad Notes

This directory contains a KiCad 8 project scaffold for the rev-A architecture.

Important:

- The schematic is an architecture-level capture, not a release-ready pin-accurate design.
- The PCB file encodes board envelope, placement zones, layer stack assumptions, and critical layout notes.
- Treat both files as the starting point for a real capture/layout cycle.

Recommended next actions:

1. Replace text block placeholders with real symbol instances and pin-level wiring.
2. Lock exact packages for the PoE, PD, and AX88279A paths.
3. Confirm stackup with the intended board house before finalizing USB differential constraints.

