#ifndef SAFETY_H
#define SAFETY_H

#include "telemetry.h"

throttle_state_t safety_evaluate(telemetry_snapshot_t *snapshot);
bool safety_should_latch_fault(const telemetry_snapshot_t *snapshot);

#endif /* SAFETY_H */

