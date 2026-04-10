# Test Plan

## Objective

Validate the rev-A prototype against the internal product spec with emphasis on:

- safe power behavior
- stable USB Ethernet operation
- correct UI and control behavior
- thermal protection

## Test Matrix

| Area | Test Group | Purpose |
| --- | --- | --- |
| Power input | PoE class matrix | validate af/at/bt/passive handling |
| PD output | contract matrix | validate PDO/APDO offer logic |
| Telemetry | sensor readback | validate measured values and fault handling |
| UI | pages and buttons | validate displayed state and long-press logic |
| Networking | enumeration and link | validate USB NIC behavior across hosts |
| Thermal | soak and throttling | validate power derating thresholds |
| Fault | abuse and recovery | validate latch behavior and safe failure |

## Detailed Cases

### 1. PoE Source Detection

- [ ] `802.3af` source recognized and clamped to low-power PDO set
- [ ] `802.3at` source recognized and mid-tier PDO set applied
- [ ] `802.3bt Type 3` source recognized and `20V` offers limited appropriately
- [ ] `802.3bt Type 4` source recognized and highest normal offers enabled
- [ ] `48V passive` mode requires boot-time combo and visual warning
- [ ] passive mode remains disabled in normal boot path

### 2. PD Output Behavior

- [ ] default boot advertises conservative `Standard` profile
- [ ] right-button long press toggles `Pro` profile
- [ ] right short press forces renegotiation
- [ ] PDOs shrink correctly under thermal derate
- [ ] fault state removes unsafe high-power offers
- [ ] fixed `5V` source fallback remains available where possible

### 3. Telemetry

- [ ] INA228 voltage/current/power readback reasonable against bench instruments
- [ ] both TMP117 sensors report stable values
- [ ] PoE telemetry over isolated I2C is robust across hot plug
- [ ] OLED input page shows source class, budget, and live power
- [ ] OLED PD page shows active contract and top offer
- [ ] OLED thermal page shows both sensors and state
- [ ] OLED link page shows link state and negotiated rate

### 4. USB Ethernet

- [ ] enumerates on macOS
- [ ] enumerates on Windows
- [ ] enumerates on Linux
- [ ] enumerates on USB-C iPad/tablet target
- [ ] maintains stable 1GbE link
- [ ] maintains stable 2.5GbE link on qualified host/cable

### 5. Thermal

- [ ] `45W` continuous, `25C` ambient
- [ ] `60W` continuous, `25C` ambient
- [ ] max allowed `Pro` profile, `25C` ambient
- [ ] `45W` continuous, `35C` ambient
- [ ] `60W` continuous, `35C` ambient
- [ ] threshold transitions at `70C`, `80C`, `90C`

### 6. Fault Injection

- [ ] source unplug during active load
- [ ] repeated PD renegotiation
- [ ] USB-C overcurrent or short event
- [ ] bad passive source attempt with mode disabled
- [ ] sensor read timeout
- [ ] isolated I2C fault
- [ ] repeated PD controller programming failure

## Equipment

- PoE injectors/switches covering `af`, `at`, `bt Type 3`, `bt Type 4`
- selected `48V passive` injector
- programmable electronic load / PD sink
- USB protocol/power analyzer where available
- thermal camera and thermocouples
- host systems across target OS matrix
- 2.5GbE-capable network peer / switch

## Exit Criteria

- No unsafe power advertisement under any validated source condition
- No uncontrolled overtemperature behavior
- Stable Ethernet operation on all primary host targets
- All UI states match underlying telemetry and fault state
- Clear list of rev-B issues captured for any deferred features

