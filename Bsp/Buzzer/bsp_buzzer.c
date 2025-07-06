/*
 * bsp_buzzer.c
 *
 */

#include "bsp_buzzer.h"
#include "main.h"
/*
 * Turn on/off the buzzer
 * i: 0 or 1
 * 0: turn off the buzzer
 * 1: turn on the buzzer
 * */
void Set_Buzzer(uint8_t i)
{
	if (i == 0)
	{
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET); // turn off the buzzer
	}
	else
	{
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET); // turn on the buzzer
	}
}
