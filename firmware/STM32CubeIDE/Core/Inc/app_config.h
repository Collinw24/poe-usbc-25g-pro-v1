#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>

#define APP_UI_PAGE_COUNT                 4U
#define APP_BUTTON_DEBOUNCE_MS            20U
#define APP_BUTTON_LONG_PRESS_MS          1200U
#define APP_BOOT_COMBO_WINDOW_MS          350U

#define APP_STANDARD_HEADROOM_PERCENT     25U
#define APP_PRO_HEADROOM_PERCENT          15U
#define APP_SOFT_DERATE_PERCENT           20U

#define APP_TEMP_SOFT_DERATE_C            70
#define APP_TEMP_HARD_DERATE_C            80
#define APP_TEMP_SHUTDOWN_C               90

#define APP_CLAMP_POWER_MW                30000U
#define APP_BURST_POWER_MW                70000U
#define APP_CONTINUOUS_POWER_MW           65000U

#define APP_TPS26750_MAX_OFFERS           6U
#define APP_OLED_LINE_COUNT               6U
#define APP_OLED_LINE_LENGTH              24U

#define APP_I2C_TIMEOUT_MS                25U

#endif /* APP_CONFIG_H */

