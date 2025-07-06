#include <bsp_ultrasonic.h>

uint32_t ultrasonic_num = 0;
uint8_t ultrasonic_flag = 0; // 0: Ranging not started 1: Ranging started

/*
 * Get the average of 5 measurements
 * */
float Get_distance(void)
{
	return Get_distance_default(5);
}

float Get_distance_default(int nbOccur){
	if (nbOccur >= 5 || nbOccur <= 0)  //more than 5 is useless here and below or equal to 0 is not possible
		return 0;

	float distance = 0, aveg = 0;
	uint16_t tim, count;
	uint8_t i = 0;

	while (i != nbOccur)
	{
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
		osDelay(20);
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

		while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET)
			;
		ultrasonic_flag = 1;

		i += 1;
		while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
		{
			count = ultrasonic_num;
			if (count >= 10000)
			{
				ultrasonic_flag = 0;
				ultrasonic_num = 0;
				return 0;
			}
		}

		ultrasonic_flag = 0;
		tim = TIM7->CNT;
		distance = (tim + ultrasonic_num * 10) / 58.5;
		aveg = distance + aveg;
		ultrasonic_num = 0;
		osDelay(10);
	}
	distance = aveg / nbOccur;
	return distance;
}
