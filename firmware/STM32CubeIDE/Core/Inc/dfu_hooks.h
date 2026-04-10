#ifndef DFU_HOOKS_H
#define DFU_HOOKS_H

#include <stdbool.h>

void dfu_request_bootloader(void);
bool dfu_should_enter_bootloader(void);
void dfu_jump_to_bootloader(void);

#endif /* DFU_HOOKS_H */

