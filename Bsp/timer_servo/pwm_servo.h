/*
 * pwm_servo.h
 *
 *  Created on: Sep 22, 2023
 *      Author: YB-101
 */

#ifndef PWM_SERVO_H_
#define PWM_SERVO_H_

#include "main.h"

#define MAX_PWM_SERVO 1

#define USE_SERVO_J1

#define SERVO_1_HIGH() HAL_GPIO_WritePin(GPIOB, S1_Pin, GPIO_PIN_SET)
#define SERVO_1_LOW() HAL_GPIO_WritePin(GPIOB, S1_Pin, GPIO_PIN_RESET)

void PwmServo_Init(void);
void PwmServo_Set_Angle(uint8_t index, uint8_t angle);
void PwmServo_Handle(void);

#endif /* PWM_SERVO_H_ */
