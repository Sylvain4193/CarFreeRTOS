/*
 * bsp_tim.c
 *
 */

#include "bsp_tim.h"
/*
 *  Initialize TIM1.2.3.4.5.8
 * */
void Bsp_Tim_Init(void)

{
	// Start the pwm output of tim1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	// Start the pwm output of tim8
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

	TIM2->CNT = 0x7fff;
	// Start the encoder mode of tim2
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM3->CNT = 0x7fff;
	// Start the encoder mode of tim3
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM4->CNT = 0x7fff;
	// Start the encoder mode of tim4
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM5->CNT = 0x7fff;
	// Start the encoder mode of tim5
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	//Servo Motor
	// This interrupt handler generates excessive CPU load.
	// Consider using an external PWM generator module that communicates via I2C to offload the task.
	HAL_TIM_Base_Start_IT(&htim7);

}

/*
 * Redefine in main
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)//10ms
	{
		Encoder_Update_Count();//10ms
		Motion_Handle();//

	}


	if (htim->Instance == TIM7)//10us
	{
		if (ultrasonic_flag)
		{
			ultrasonic_num++;
		}
	}
}*/



