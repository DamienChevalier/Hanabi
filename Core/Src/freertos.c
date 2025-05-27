/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hanabi.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum GameState {
	INIT,
	TITLE_SCREEN,
	CHOICE_SCREEN,
	HOST_GAME_SCREEN,
	JOIN_GAME_SCREEN
} GameState;

typedef enum DisplayOrder {
	DISPLAY_TITLE,
	DISPLAY_CHOICE,
	DISPLAY_HOST,
	DISPLAY_JOIN
} DisplayOrder;

typedef enum TouchState {
	TOUCHED_TITLE,
	HOST_GAME_BTN,
	JOIN_GAME_BTN
} TouchState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
GameState state = INIT;
static TS_StateTypeDef TS_State;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId displayTaskHandle;
osThreadId touchTaskHandle;
osThreadId stateTaskHandle;
osMessageQId displayQueueHandle;
osMessageQId touchQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void displayTaskFunction(void const * argument);
void touchTaskFunction(void const * argument);
void stateTaskFunction(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

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

  /* Create the queue(s) */
  /* definition and creation of displayQueue */
  osMessageQDef(displayQueue, 3, DisplayOrder);
  displayQueueHandle = osMessageCreate(osMessageQ(displayQueue), NULL);

  /* definition and creation of touchQueue */
  osMessageQDef(touchQueue, 3, TouchState);
  touchQueueHandle = osMessageCreate(osMessageQ(touchQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of displayTask */
  osThreadDef(displayTask, displayTaskFunction, osPriorityAboveNormal, 0, 1024);
  displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

  /* definition and creation of touchTask */
  osThreadDef(touchTask, touchTaskFunction, osPriorityAboveNormal, 0, 256);
  touchTaskHandle = osThreadCreate(osThread(touchTask), NULL);

  /* definition and creation of stateTask */
  osThreadDef(stateTask, stateTaskFunction, osPriorityHigh, 0, 512);
  stateTaskHandle = osThreadCreate(osThread(stateTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for(;;)
	{
		osDelay(10000);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_displayTaskFunction */
/**
 * @brief Function implementing the displayTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_displayTaskFunction */
void displayTaskFunction(void const * argument)
{
  /* USER CODE BEGIN displayTaskFunction */
	osEvent event;
	/* Infinite loop */
	for(;;)
	{
		event = osMessageGet(displayQueueHandle, 100);
		switch(state) {
		case INIT:
			break;

		case TITLE_SCREEN:
			if (event.status == osEventMessage) {
				if (event.value.v == DISPLAY_TITLE) {

					BSP_LCD_SelectLayer(0);
					BSP_LCD_Clear(LCD_COLOR_DARKBLUE);
					BSP_LCD_DrawBitmap(0,0,(uint8_t*)hanabi_bmp);

					BSP_LCD_SelectLayer(1);
					BSP_LCD_Clear(00);
					BSP_LCD_SetFont(&Font12);
					BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
					BSP_LCD_SetBackColor(00);
					BSP_LCD_DisplayStringAt(0, LINE(16), (void*)"Touch the screen to continue", CENTER_MODE);
					osDelay (400);
				}
			}
			break;

		case CHOICE_SCREEN:
			if (event.status == osEventMessage) {
				if (event.value.v == DISPLAY_CHOICE) {
					BSP_LCD_SelectLayer(0);
					BSP_LCD_Clear(LCD_COLOR_DARKBLUE);
					BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
					BSP_LCD_FillRect(70, 36, 140, 200);
					BSP_LCD_SetTextColor(LCD_COLOR_RED);
					BSP_LCD_FillRect(270, 36, 140, 200);

					BSP_LCD_SelectLayer(1);
					BSP_LCD_Clear(00);
					BSP_LCD_SetBackColor(00);
					BSP_LCD_SetFont(&Font24);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(-100, LINE(4.5), (void*)"HOST", CENTER_MODE);
					BSP_LCD_DisplayStringAt(-100, LINE(5.5), (void*)"GAME", CENTER_MODE);
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_DisplayStringAt(100, LINE(4.5), (void*)"JOIN", CENTER_MODE);
					BSP_LCD_DisplayStringAt(100, LINE(5.5), (void*)"GAME", CENTER_MODE);
					osDelay (400);
				} else {
					// Unexpected message
				}
			}
			break;

		case HOST_GAME_SCREEN:
			if (event.status == osEventMessage) {
				if (event.value.v == DISPLAY_HOST) {
					BSP_LCD_SelectLayer(0);
					BSP_LCD_Clear(LCD_COLOR_DARKGREEN);

					BSP_LCD_SelectLayer(1);
					BSP_LCD_Clear(00);
					BSP_LCD_SetBackColor(00);
					BSP_LCD_SetFont(&Font24);
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_DisplayStringAt(0, LINE(5), (void*)"HOST GAME", CENTER_MODE);
					osDelay (400);
				}
			}
			break;

		case JOIN_GAME_SCREEN:
			if (event.status == osEventMessage) {
				if (event.value.v == DISPLAY_JOIN) {
					BSP_LCD_SelectLayer(0);
					BSP_LCD_Clear(LCD_COLOR_DARKRED);

					BSP_LCD_SelectLayer(1);
					BSP_LCD_Clear(00);
					BSP_LCD_SetBackColor(00);
					BSP_LCD_SetFont(&Font24);
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_DisplayStringAt(0, LINE(5), (void*)"JOIN GAME", CENTER_MODE);
					osDelay (400);
				}
			}
			break;
		}

	}
	osDelay(10);
  /* USER CODE END displayTaskFunction */
}

/* USER CODE BEGIN Header_touchTaskFunction */
/**
 * @brief Function implementing the touchTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_touchTaskFunction */
void touchTaskFunction(void const * argument)
{
  /* USER CODE BEGIN touchTaskFunction */
	uint32_t x;
	uint32_t y;
	osStatus status;
	/* Infinite loop */
	for(;;)
	{
		BSP_TS_GetState(&TS_State);
		switch(state) {
		case INIT:

			break;
		case TITLE_SCREEN:
			if (TS_State.touchDetected) {
				status = osMessagePut(touchQueueHandle, TOUCHED_TITLE, 0);
				osDelay (400);
			}
			break;
		case CHOICE_SCREEN:
			if (TS_State.touchDetected) {
				x = TS_State.touchX[0];
				y = TS_State.touchY[0];
				if (x >= 70 && x <= 210 && y >= 36 && y <= 236) {
					status = osMessagePut(touchQueueHandle, HOST_GAME_BTN, 0);
				} else if (x >= 270 && x <= 410 && y >= 36 && y <= 236) {
					status = osMessagePut(touchQueueHandle, JOIN_GAME_BTN, 0);
				}
				osDelay (400);
			}
			break;
		}
		osDelay(50);
	}
  /* USER CODE END touchTaskFunction */
}

/* USER CODE BEGIN Header_stateTaskFunction */
/**
 * @brief Function implementing the stateTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_stateTaskFunction */
void stateTaskFunction(void const * argument)
{
  /* USER CODE BEGIN stateTaskFunction */
	osEvent event;
	osStatus status;
	/* Infinite loop */
	for(;;)
	{
		event = osMessageGet(touchQueueHandle, 100);
		switch(state) {
		case INIT:
			state = TITLE_SCREEN;
			osMessagePut(displayQueueHandle, DISPLAY_TITLE, 0);
			break;
		case TITLE_SCREEN:
			if (event.status == osEventMessage && event.value.v == TOUCHED_TITLE) {
				state = CHOICE_SCREEN;
				status = osMessagePut(displayQueueHandle, DISPLAY_CHOICE, 0);
			}
			break;
		case CHOICE_SCREEN:
			if (event.status == osEventMessage && event.value.v == HOST_GAME_BTN) {
				state = HOST_GAME_SCREEN;
				status = osMessagePut(displayQueueHandle, DISPLAY_HOST, 0);
			} else if (event.status == osEventMessage && event.value.v == JOIN_GAME_BTN) {
				state = JOIN_GAME_SCREEN;
				status = osMessagePut(displayQueueHandle, DISPLAY_JOIN, 0);
			}
			break;
		}
		osDelay(50);
	}
  /* USER CODE END stateTaskFunction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

