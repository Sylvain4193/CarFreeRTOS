/*
 * pwm_servo.c
 */

#include "pwm_servo.h"
#include "tim.h"

uint16_t g_pwm_pulse = 0;

uint8_t g_pwm_angle[MAX_PWM_SERVO] = {90};
uint16_t g_angle_num[MAX_PWM_SERVO] = {149};

// The Angle is converted to the number of pulses, angle= [0, 180]
static uint16_t PwmServo_Angle_To_Pulse(uint8_t angle)
{
	uint16_t pulse = (angle * 11 + 500) / 10;
	return pulse;
}

// PWM steering gear control, in the timer call, analog output PWM signal
void PwmServo_Handle(void)
{
	g_pwm_pulse++;

	if (g_pwm_pulse <= g_angle_num[0])
		SERVO_1_HIGH();
	else
		SERVO_1_LOW();

	if (g_pwm_pulse >= 2000)
		g_pwm_pulse = 0;
}

// Initialize the steering gear
void PwmServo_Init(void)
{
	for (int i = 0; i < MAX_PWM_SERVO; i++)
	{
		g_pwm_angle[i] = 90;
		g_angle_num[i] = PwmServo_Angle_To_Pulse(g_pwm_angle[i]);
	}
}

// Set the PWM servo Angle, index=0~MAX_PWM_SERVO, Angle to 0-180
void PwmServo_Set_Angle(uint8_t index, uint8_t angle)
{
	if (index >= MAX_PWM_SERVO)
		return;
	if (angle > 180)
		return;
	g_pwm_angle[index] = angle;
	g_angle_num[index] = PwmServo_Angle_To_Pulse(angle);
}
