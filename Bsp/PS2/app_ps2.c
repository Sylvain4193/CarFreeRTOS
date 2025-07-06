/*
 * app_ps2.c
 *
 */

#include "app_ps2.h"

int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;
int State_PSB_PAD_UP = 0;
int State_PSB_PAD_DOWN = 0;
int State_PSB_PAD_RIGHT = 0;
int State_PSB_PAD_LEFT = 0;
int8_t Beep_Trigger_State_PS2 = 0;
int8_t Dist_Trigger_State_PS2 = 0;
int8_t Temp_Trigger_State_PS2 = 0;
MotorCommand_t motorCmd;
BeepCommand_t beepCmdPS2 = {0};
//char buff[20] = {'\0'}; //Debug
// Function function: PS2 control car

void User_PS2_Control(void)
{
	//If the handle is not connected, i.e. 4 255
	PS2_LX = PS2_AnologData(PSS_LX);
	PS2_LY = PS2_AnologData(PSS_LY);
	PS2_RX = PS2_AnologData(PSS_RX);
	PS2_RY = PS2_AnologData(PSS_RY);
	PS2_KEY = PS2_DataKey(); // To only one key at the time but use Handkey to do multi-key


	// The handle is not communicating
	if ((PS2_LX == 255) && (PS2_LY == 255) && (PS2_RX == 255) && (PS2_RY == 255))
	{
		Set_RGB(RGB_Max, white);
		return;
	}
	else if ((PS2_LX == 0) && (PS2_LY == 0) && (PS2_RX == 0) && (PS2_RY == 0))
	{
		Set_RGB(RGB_Max, white);
		return;
	}
	else if ((PS2_LX == 192) || (PS2_LY == 192) || (PS2_RX == 192) || (PS2_RY == 192)) // This is a misjudgment value, you can adjust it yourself then
	{
		Set_RGB(RGB_Max, white);
		return;
	}

	//Multi key at the time

	/*
	 * Direction :
	 * PS2_LX : Go Right and Left
	 * R1 	  : Boost
	 * R2     : Forward
	 * L1     : Spin
	 * L2     : Backward
	 */

	motorCmd.joystickX = PS2_LX;
	motorCmd.R1_pressed = ((Handkey & (1 << (MASK[PSB_R1 - 1]-1))) == 0);
	motorCmd.R2_pressed = ((Handkey & (1 << (MASK[PSB_R2 - 1]-1))) == 0);
	motorCmd.L1_pressed = ((Handkey & (1 << (MASK[PSB_L1 - 1]-1))) == 0);
	motorCmd.L2_pressed = ((Handkey & (1 << (MASK[PSB_L2 - 1]-1))) == 0);
	xQueueSend(myQueue_controlMotor, &motorCmd, 0);

	if (PS2_RX < 125 || PS2_RX > 150){
		xTaskNotify(myTask_ServoHandle, PS2_RX, eSetValueWithOverwrite);
	}

	// Klaxon on green button
	if ((((Handkey & (1 << (MASK[PSB_GREEN-1] - 1))) == 0) && (Beep_Trigger_State_PS2 == 0) ) ||
					(((Handkey & (1 << (MASK[PSB_GREEN-1] - 1))) != 0) && (Beep_Trigger_State_PS2 == 1)))
	{
		xQueueSend(myQueue_controlMotor, &beepCmdPS2, 0);
		Beep_Trigger_State_PS2 = !Beep_Trigger_State_PS2;
	}

	// Get Distance on blue button
	if ((((Handkey & (1 << (MASK[PSB_BLUE-1] - 1))) == 0) && (Dist_Trigger_State_PS2 == 0) )  //Push Button
					)
	{
		xTaskNotify(myTask_USRanginHandle, 0, eNoAction);
		Dist_Trigger_State_PS2 = !Dist_Trigger_State_PS2;
	}
	else if ((((Handkey & (1 << (MASK[PSB_BLUE-1] - 1))) != 0) && (Dist_Trigger_State_PS2 == 1))){ // Release Button
		Dist_Trigger_State_PS2 = !Dist_Trigger_State_PS2;
	}
	// Temperature on pink button
	if ((((Handkey & (1 << (MASK[PSB_PINK-1] - 1))) == 0) && (Temp_Trigger_State_PS2 == 0) )  //Push Button
					)
	{
		xTaskNotify(myTask_TempHandle, 0, eNoAction);
		Temp_Trigger_State_PS2 = !Temp_Trigger_State_PS2;
	}
	else if ((((Handkey & (1 << (MASK[PSB_PINK-1] - 1))) != 0) && (Temp_Trigger_State_PS2 == 1))){ // Release Button
		Temp_Trigger_State_PS2 = !Temp_Trigger_State_PS2;
	}


	// Need to be fix, don't work if multi key push
	switch (PS2_KEY) //One key at the time
	{
	case PSB_PAD_UP:
		if (State_PSB_PAD_UP == 0){
			xTaskNotify(myTask_OLEDHandle, PSB_PAD_UP, eSetValueWithOverwrite);
			State_PSB_PAD_UP = 1;    // Forces you to release the button
		}
		break;
	case PSB_PAD_RIGHT:
		if (State_PSB_PAD_RIGHT == 0){
			xTaskNotify(myTask_OLEDHandle, PSB_PAD_RIGHT, eSetValueWithOverwrite);
			State_PSB_PAD_RIGHT = 1; // Forces you to release the button
		}
		break;
	case PSB_PAD_DOWN:
		if (State_PSB_PAD_DOWN == 0){
			xTaskNotify(myTask_OLEDHandle, PSB_PAD_DOWN, eSetValueWithOverwrite);
			State_PSB_PAD_DOWN = 1;  // Forces you to release the button
		}
		break;
	case PSB_PAD_LEFT:
		if (State_PSB_PAD_LEFT == 0){
			xTaskNotify(myTask_OLEDHandle, PSB_PAD_LEFT, eSetValueWithOverwrite);
			State_PSB_PAD_LEFT = 1;  // Forces you to release the button
		}
		break;
	case PSB_BLUE:
		Set_RGB(RGB_R, blue);
		break;
	case PSB_PINK:
		Set_RGB(RGB_L, purple);
		break;
	case PSB_RED:
		Set_RGB(RGB_Max, red);
		break;
	default:
		State_PSB_PAD_UP = 0;     // Forces you to release the button
		State_PSB_PAD_DOWN = 0;   // Forces you to release the button
		State_PSB_PAD_RIGHT = 0;  // Forces you to release the button
		State_PSB_PAD_LEFT = 0;   // Forces you to release the button
		break;
	}

	//HAL_Delay(20); // Replace with vTaskDelay(pdMS_TO_TICKS(20)); in the task, enables anti-bounce and staying connected
}
