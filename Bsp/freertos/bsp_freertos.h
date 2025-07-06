/*
 * bsp_freertos.h
 *
 *  Created on: Jun 1, 2025
 *      Author: 33777
 */

#ifndef FREERTOS_BSP_FREERTOS_H_
#define FREERTOS_BSP_FREERTOS_H_
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "bsp_task.h"


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN queues */
extern QueueHandle_t myQueue_controlMotor;
extern QueueHandle_t myQueue_controlBeep;
/* USER CODE END queues */

/* USER CODE BEGIN Mutex */

/* USER CODE END Mutex */

/* USER CODE BEGIN Task */
extern TaskHandle_t myTask_BuzzerHandle;
extern TaskHandle_t myTask_OLEDHandle;
extern TaskHandle_t myTask_USRanginHandle;
extern TaskHandle_t myTask_TempHandle;
extern TaskHandle_t myTask_ServoHandle;
/*USER CODE END Task */

/* USER CODE BEGIN Struct */
typedef struct {
	uint8_t taskSender;
	//uint16_t taskMsg;
} BeepCommand_t;
/* USER CODE END Struct */

void StartTask_PS2(void * argument);
void StartTask_RGB(void * argument);
void StartTask_KEY(void * argument);
void StartTask_LED(void * argument);
void StartTask_Buzzer(void * argument);
void StartTask_USRanging(void * argument);
void StartTask_QuickChat(void * argument);
void StartTask_Motor(void * argument);
void StartTask_Encoder(void * argument);
void StartTask_Servo(void * argument);
void StartTask_Temperature(void * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

#endif /* FREERTOS_BSP_FREERTOS_H_ */
