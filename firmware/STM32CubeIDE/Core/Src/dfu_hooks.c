#include "dfu_hooks.h"

#include <stdint.h>

static uint32_t g_dfu_request_cookie;
static const uint32_t kDfuRequestMagic = 0x44554631U; /* "DUF1" */

void dfu_request_bootloader(void)
{
    g_dfu_request_cookie = kDfuRequestMagic;
}

bool dfu_should_enter_bootloader(void)
{
    if (g_dfu_request_cookie == kDfuRequestMagic)
    {
        g_dfu_request_cookie = 0U;
        return true;
    }

    return false;
}

void dfu_jump_to_bootloader(void)
{
    /*
     * TODO:
     * - Replace with STM32G0 system bootloader jump sequence
     * - Deinit clocks/peripherals and remap vector table as required
     */
    while (1)
    {
    }
}

