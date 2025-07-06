/*
 * bsp.freertos.c
 *
 *  Created on: Jun 1, 2025
 *      Author: 33777
 */
/* USER CODE BEGIN Header */

#include "bsp_freertos.h"

/* USER CODE BEGIN Variables */
TaskHandle_t myTask_PS2Handle;
TaskHandle_t myTask_RGBHandle;
TaskHandle_t myTask_KEYHandle;
TaskHandle_t myTask_LEDHandle;
TaskHandle_t myTask_BuzzerHandle;
TaskHandle_t myTask_USRanginHandle;
TaskHandle_t myTask_OLEDHandle;
TaskHandle_t myTask_EncoderHandle;
TaskHandle_t myTask_MotorHandle;
TaskHandle_t myTask_ServoHandle;
TaskHandle_t myTask_TempHandle;
/* USER CODE END Variables */

/* USER CODE BEGIN Queues */
QueueHandle_t myQueue_controlMotor;
QueueHandle_t myQueue_controlBeep;
/* USER CODE END Queues */

/* USER CODE BEGIN Mutex */
SemaphoreHandle_t xBuzzerMutex; //Useless Replace with a Queue
/* USER CODE END Mutex */


/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	myQueue_controlMotor = xQueueCreate(10, sizeof(MotorCommand_t));
	myQueue_controlBeep = xQueueCreate(4, sizeof(BeepCommand_t));;
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of myTask_PS2 */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(StartTask_PS2,         "PS2",      1024, NULL, tskIDLE_PRIORITY + 3, &myTask_PS2Handle);
  xTaskCreate(StartTask_RGB,         "RGB",       128, NULL, tskIDLE_PRIORITY, &myTask_RGBHandle);
  xTaskCreate(StartTask_KEY,         "KEY",       128, NULL, tskIDLE_PRIORITY + 1, &myTask_KEYHandle);
  xTaskCreate(StartTask_LED,         "LED",       128, NULL, tskIDLE_PRIORITY, &myTask_LEDHandle);
  xTaskCreate(StartTask_Buzzer,      "BUZZER",    128, NULL, tskIDLE_PRIORITY, &myTask_BuzzerHandle);
  xTaskCreate(StartTask_USRanging,  "USRanging", 256, NULL, tskIDLE_PRIORITY + 2, &myTask_USRanginHandle);
  xTaskCreate(StartTask_QuickChat,   "OLED",      256, NULL, tskIDLE_PRIORITY + 1, &myTask_OLEDHandle);
  xTaskCreate(StartTask_Encoder,     "ENCODER",   128, NULL, tskIDLE_PRIORITY + 3, &myTask_EncoderHandle);
  xTaskCreate(StartTask_Motor,       "MOTOR",     512, NULL, tskIDLE_PRIORITY + 2, &myTask_MotorHandle);
  xTaskCreate(StartTask_Servo,       "SERVO",     128, NULL, tskIDLE_PRIORITY + 1, &myTask_ServoHandle);
  xTaskCreate(StartTask_Temperature,       "TEMP",     128, NULL, tskIDLE_PRIORITY, &myTask_TempHandle);
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartTask_RGB */
/**
* @brief Function implementing the myTask_RGB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_RGB */
void StartTask_RGB(void * argument)
{
  /* USER CODE BEGIN StartTask_RGB */
	/* Infinite loop */
	Task_Entity_RGB();
  /* USER CODE END StartTask_RGB */
}

/* USER CODE BEGIN Header_StartTask_KEY */
/**
* @brief Function implementing the myTask_KEY thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_KEY */
void StartTask_KEY(void * argument)
{
  /* USER CODE BEGIN StartTask_KEY */
	/* Infinite loop */
	Task_Entity_KEY();
  /* USER CODE END StartTask_KEY */
}

/* USER CODE BEGIN Header_StartTask_LED */
/**
* @brief Function implementing the myTask_LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_LED */
void StartTask_LED(void * argument)
{
  /* USER CODE BEGIN StartTask_LED */
	/* Infinite loop */
	Task_Entity_LED();
  /* USER CODE END StartTask_LED */
}

/* USER CODE BEGIN Header_StartTask_PS2 */
/**
* @brief Function implementing the myTask_PS2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_PS2 */
void StartTask_PS2(void * argument)
{
  /* USER CODE BEGIN StartTask_PS2 */
  /* Infinite loop */
	Task_Entity_PS2();
  /* USER CODE END StartTask_PS2 */
}

/* USER CODE BEGIN Header_StartTask_Buzzer */
/**
* @brief Function implementing the myTask_Buzzer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Buzzer */
void StartTask_Buzzer(void * argument)
{
  /* USER CODE BEGIN StartTask_Buzzer */
  /* Infinite loop */
	Task_Entity_BEEP();
  /* USER CODE END StartTask_Buzzer */
}

/* USER CODE BEGIN Header_StartTask_USRanging */
/**
* @brief Function implementing the myTask_USRangin thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_USRanging */
void StartTask_USRanging(void * argument)
{
  /* USER CODE BEGIN StartTask_USRanging */
  /* Infinite loop */
	Task_Entity_USRanging();
  /* USER CODE END StartTask_USRanging */
}

/* USER CODE BEGIN Header_StartTask_QuickChat */
/**
* @brief Function implementing the myTask_QuickCha thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_QuickChat */
void StartTask_QuickChat(void * argument)
{
  /* USER CODE BEGIN StartTask_QuickChat */
  /* Infinite loop */
	Task_Entity_OLED();
  /* USER CODE END StartTask_QuickChat */
}

/* USER CODE BEGIN Header_StartTask_Encoder */
/**
* @brief Function implementing the myTask_Encoder thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Encoder */
void StartTask_Encoder(void * argument)
{
  /* USER CODE BEGIN StartTask_Encoder */
  /* Infinite loop */
  Task_Entity_Encoder();
  /* USER CODE END StartTask_Encoder */
}

/* USER CODE BEGIN Header_StartTask_Motor */
/**
* @brief Function implementing the myTask_Motor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Motor */
void StartTask_Motor(void * argument)
{
  /* USER CODE BEGIN StartTask_Motor */
  /* Infinite loop */
	  Task_Entity_Motor();
  /* USER CODE END StartTask_Motor */
}

void StartTask_Servo(void * argument)
{
  /* USER CODE BEGIN StartTask_Motor */
  /* Infinite loop */
	  Task_Entity_Servo();
  /* USER CODE END StartTask_Motor */
}

void StartTask_Temperature(void * argument)
{
  /* USER CODE BEGIN StartTask_Motor */
  /* Infinite loop */
	  Task_Entity_Temperature();
  /* USER CODE END StartTask_Motor */
}


