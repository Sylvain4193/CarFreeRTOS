#include "bsp.h"

//Hardware Initialization
//Parameter:None
void BSP_Init(void)
{
	//Temp sensor
	HAL_ADCEx_Calibration_Start(&hadc1);

	Delay_Init();
	PID_Param_Init();// Motor PID initialization
	Bsp_Tim_Init();// Motor related timer initialization

	PwmServo_Init(); //Servo Motot init

	OLED_Init();
	OLED_Draw_Line("oled init success!", 1, true, true);

	PS2_SetInit();
	Motion_Set_Speed(0,0,0,0);//Car stop
}

//Loop Run Function
//Parameter:None
void BSP_Loop(void)
{

}

