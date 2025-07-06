/*
 * bsp_encoder.c
 *
 */

#include "bsp_encoder.h"
#include "bsp_motor.h"

int g_Encoder_M1_Now = 0;
int g_Encoder_M2_Now = 0;
int g_Encoder_M3_Now = 0;
int g_Encoder_M4_Now = 0;

/**
 * @Brief: To read the encoder count, call every 10 milliseconds
 * @Note:
 * @Parm: Motor id：:MOTOR_ID_M1
 * @Retval: Returns encoder count data
 */
static int16_t Encoder_Read_CNT(uint8_t Motor_id)
{
	int16_t Encoder_TIM = 0;
	switch (Motor_id)
	{
	case MOTOR_ID_M1:
		Encoder_TIM = 0x7fff - (short)TIM4->CNT;
		TIM4->CNT = 0x7fff;
		break;
	case MOTOR_ID_M2:
		Encoder_TIM = 0x7fff - (short)TIM2->CNT;
		TIM2->CNT = 0x7fff;
		break;
	case MOTOR_ID_M3:
		Encoder_TIM = 0x7fff - (short)TIM5->CNT;
		TIM5->CNT = 0x7fff;
		break;
	case MOTOR_ID_M4:
		Encoder_TIM = 0x7fff - (short)TIM3->CNT;
		TIM3->CNT = 0x7fff;
		break;
	default:
		break;
	}
	return Encoder_TIM;
}

// Returns the total count of encoders from boot up to now (single channel)
int Encoder_Get_Count_Now(uint8_t Motor_id)
{
	if (Motor_id == MOTOR_ID_M1)
		return g_Encoder_M1_Now;
	if (Motor_id == MOTOR_ID_M2)
		return g_Encoder_M2_Now;
	if (Motor_id == MOTOR_ID_M3)
		return g_Encoder_M3_Now;
	if (Motor_id == MOTOR_ID_M4)
		return g_Encoder_M4_Now;
	return 0;
}


// Get the total four - way encoder count up to now
void Encoder_Get_ALL(int *Encoder_all)
{
	Encoder_all[0] = g_Encoder_M1_Now;
	Encoder_all[1] = g_Encoder_M2_Now;
	Encoder_all[2] = g_Encoder_M3_Now;
	Encoder_all[3] = g_Encoder_M4_Now;
}


// Update the count value of the encoder. call every 10 milliseconds
void Encoder_Update_Count(void)
{
	g_Encoder_M1_Now -= Encoder_Read_CNT(MOTOR_ID_M1);
	g_Encoder_M2_Now -= Encoder_Read_CNT(MOTOR_ID_M2);
	g_Encoder_M3_Now += Encoder_Read_CNT(MOTOR_ID_M3);
	g_Encoder_M4_Now += Encoder_Read_CNT(MOTOR_ID_M4);
}
