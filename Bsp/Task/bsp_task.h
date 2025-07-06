#ifndef __BSP_TASK_H_
#define __BSP_TASK_H_

#include "main.h"
#include "gpio.h"
#include "freertos.h"
#include "bsp_rgb.h"
#include "bsp_key.h"
#include "bsp_buzzer.h"
#include "bsp_oled.h"
#include "pwm_servo.h"
#include "bsp_ultrasonic.h"

#define true 1
#define false 0

extern unsigned int Beep_Switch;
extern unsigned int LED1_Switch;
extern unsigned int LED2_Switch;

void Task_Entity_RGB(void);
void Task_Entity_LED(void);
void Task_Entity_BEEP(void);
void Task_Entity_KEY(void);
void Task_Entity_PS2(void);
void Task_Entity_OLED(void);
void Task_Entity_USRanging(void);
void Task_Entity_Encoder(void);
void Task_Entity_Motor(void);
void Task_Entity_Servo(void);
void Task_Entity_Temperature(void);

#endif
