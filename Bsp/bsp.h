#ifndef __BSP_H_
#define __BSP_H_

#include <stdio.h>
#include <string.h>

#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define bool _Bool
#define true 1
#define false 0

#include "main.h"

// HAL drivers/utils
#include "gpio.h"
#include "i2c.h"
#include "tim.h"
#include "delay.h"
#include "adc.h"
#include "stdio.h"

//FreeRTOS
#include "freertos.h"
#include "cmsis_os.h"

//BSP drivers
#include "bsp_ps2.h"
#include "bsp_key.h"
#include "bsp_rgb.h"
#include "bsp_buzzer.h"
#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "bsp_tim.h"
#include "bsp_PID_motor.h"
#include "bsp_oled_i2c.h"
#include "bsp_oled.h"


//App-Level Logic
#include "bsp_task.h"
#include "app_ps2.h"
#include "app_motor.h"
#include "bsp_freertos.h"


// External libs/font
#include "oled_fonts.h"

void BSP_Init(void);
void BSP_Loop(void);

#endif




