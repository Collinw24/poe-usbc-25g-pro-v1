cmake_minimum_required(VERSION 3.20)

project(poe_usbc_25g_pro_v1_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(adapter_core STATIC
    STM32CubeIDE/Core/Src/main.c
    STM32CubeIDE/Core/Src/ltc9105_i2c.c
    STM32CubeIDE/Core/Src/ina228.c
    STM32CubeIDE/Core/Src/tmp117.c
    STM32CubeIDE/Core/Src/telemetry.c
    STM32CubeIDE/Core/Src/pd_policy.c
    STM32CubeIDE/Core/Src/tps26750.c
    STM32CubeIDE/Core/Src/bq25756.c
    STM32CubeIDE/Core/Src/oled_ui.c
    STM32CubeIDE/Core/Src/buttons.c
    STM32CubeIDE/Core/Src/safety.c
    STM32CubeIDE/Core/Src/dfu_hooks.c
)

target_include_directories(adapter_core PUBLIC
    STM32CubeIDE/Core/Inc
)

target_compile_definitions(adapter_core PUBLIC
    STM32G0B1xx
    USE_HAL_DRIVER
)

target_compile_options(adapter_core PRIVATE
    -Wall
    -Wextra
    -Werror=return-type
)

message(STATUS "This CMake target is intended for static analysis and host-side parsing checks.")
message(STATUS "CubeIDE/CubeMX integration is still the primary embedded build path.")

