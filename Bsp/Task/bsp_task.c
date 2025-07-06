#include "bsp_task.h"

unsigned int LED1_Switch = 0;	//0
unsigned int LED2_Switch = 0;	//0
unsigned int previousPad = 0;   //0

void Task_Entity_RGB(void)// RGB task entity function
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, red);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, green);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, blue);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, yellow);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, purple);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, lake);
		vTaskDelay(pdMS_TO_TICKS(1000));
		Set_RGB(RGB_Max, white);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Task_Entity_BEEP(void)// Buzzer task entity functions
{
	uint8_t stateHorn = 0;
	int8_t stateBackward = 0;
	BeepCommand_t beepCmdSender = {0};
	TickType_t delay = portMAX_DELAY;
	uint8_t incr = 0;
	while(1)
	{

		if (xQueueReceive(myQueue_controlMotor, &beepCmdSender, delay) == pdPASS) {
			if (beepCmdSender.taskSender == 0){
				stateHorn = !stateHorn;
			}
			else {
				stateBackward = !stateBackward;
				}
			}
		if (stateHorn == 1){            // Horn ON
			Set_Buzzer(stateHorn);
		}
		else if (stateBackward == 1){   // Backward ON
			if (incr == 0){
				Set_Buzzer(1);
				delay = pdMS_TO_TICKS(10);
			}
			else if (incr == 25){
				Set_Buzzer(0);
			}
			incr += 1;
			if (incr == 75)             //Horn 250ms On, 500ms Off
				incr = 0;
		}
		else if (stateBackward == 0){   // Backward OFF
			delay = portMAX_DELAY;
			incr = 0;
			if (stateHorn == 0){        //Horn OFF
				Set_Buzzer(stateHorn);
			}
		}
	}
}

void Task_Entity_LED(void)// LED task entity functions
{
	while(1)
	{
		if(LED1_Switch)
		{
			HAL_GPIO_WritePin(GPIOG, LED1_Pin|LED2_Pin, GPIO_PIN_SET);
		}
		if(LED2_Switch)
		{
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOG, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Task_Entity_KEY(void)// Button entity function
{
	while(1)
	{
		if(Key1_State(1) == KEY_PRESS)
		{
			LED1_Switch = !LED1_Switch;
			LED2_Switch = !LED2_Switch;
		}
		if(Key2_State(1) == KEY_PRESS)
		{
			LED1_Switch = !LED1_Switch;
		}
		if(Key3_State(1) == KEY_PRESS)
		{
			LED2_Switch = !LED2_Switch;
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Task_Entity_PS2(void)// Controller entity function
{
	while(1)
	{
		User_PS2_Control();
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void Task_Entity_OLED(void)// OLED entity function
{
	uint32_t firstPad = PSB_PAD_UP;
	uint32_t secondPad;
	BaseType_t result;

	while(1){
		xTaskNotifyWait(0x00, 0x00, &firstPad, portMAX_DELAY);
		result = xTaskNotifyWait(0, 0xFFFFFFFF, &secondPad, pdMS_TO_TICKS(1000));
			if (result == pdTRUE)
            {
				// QuickChat
				OLED_QuickChat(firstPad, secondPad);
            }
			else{
				//Clear if no button was press
				OLED_Clear();
				OLED_Refresh();
			}
	}

}

void Task_Entity_USRanging(){
	float distance = 0;
	char line2[20] = {'\0'};
	while(1){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		distance = Get_distance();

		sprintf(line2, "Distance = %.3f   ", distance);
		OLED_Draw_Line(line2, 2, false, true);
	}
}


void Task_Entity_Encoder(void){
	while(1){
		Encoder_Update_Count();//10ms
		Motion_Handle();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}


void Task_Entity_Motor(void){
	while(1){
		MotorCommand_t motorCmd;

		while (1)
		{
			if (xQueueReceive(myQueue_controlMotor, &motorCmd, 20) == pdPASS) {
	            // Update Motor with Controller
				updateMotorState(motorCmd);
	        }
	        else
	        {
	            // Nothing here, maybe add slow-down function
	            //handleNoCommand(); // optionnel
	        }
	        vTaskDelay(pdMS_TO_TICKS(10)); // To avoid flood CPU
	    }
	}
}

void Task_Entity_Servo(void){
	uint32_t cmd;
	uint8_t angle; // The Angle is converted to the number of pulses, angle= [0, 180]
	PwmServo_Set_Angle(0, 0);
	while(1){
		xTaskNotifyWait(0x00, 0x00, &cmd, portMAX_DELAY);
		//Do another way if you want more sensibily on servo control
		if (cmd < 125){
			angle = angle - 1;
		}
		else{
			angle = angle + 1;
		}
		PwmServo_Set_Angle(0, angle); // S1
	}
}

void Task_Entity_Temperature(void){
	uint16_t g_ADC_Value = 0;
	double Current_Voltage = 0;
	double Inter_Temperature = 0;
	char line3[20] = {'\0'};

	while(1){
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,10);
		g_ADC_Value = HAL_ADC_GetValue(&hadc1);
		Current_Voltage = g_ADC_Value / 4096.0 * 3.3;
		Inter_Temperature = (1.43 - Current_Voltage) / 0.0043 + 25.0;
		sprintf(line3, "Int Temp : %.2f   ", Inter_Temperature);
		OLED_Draw_Line(line3, 2, false, true);

	}
}
