#ifndef MAIN_H
#define MAIN_H

#include "stm32g0xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim3;

void Error_Handler(void);

#endif /* MAIN_H */

