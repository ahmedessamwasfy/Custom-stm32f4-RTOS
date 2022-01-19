/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "gpio.h"
#include "can.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static RTOS_thread_t thread1;
static RTOS_stack_t thread1stack;
static RTOS_thread_t thread2;
static RTOS_stack_t thread2stack;
static RTOS_thread_t thread3;
static RTOS_stack_t thread3stack;
static RTOS_thread_t thread4;
static RTOS_stack_t thread4stack;

static RTOS_mutex_t mutex1;
static RTOS_semaphore_t semaphore1;
static RTOS_mailbox_t mailbox1;
static uint32_t mailbox1buffer[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/**
 * @brief   thread1function
 * @note
 * @param   none
 * @retval  none
 */
void thread1function(void) {
	uint32_t msg = 0x11223344;

	while (1) {
		RTOS_SVC_semaphoreTake(&semaphore1, 1);
		 RTOS_SVC_semaphoreGive(&semaphore1);

		if(RTOS_SUCCESS == RTOS_SVC_mutexLock(&mutex1, 10))
		    {
		      RTOS_SVC_threadDelay(15);
		      RTOS_SVC_mutexRelease(&mutex1);
		    }
		    else
		    {
		    }
	    RTOS_SVC_threadDelay(1);
		 CAN1_Transmit_Red1();
	    RTOS_SVC_threadDelay(10);

		 CAN1_Transmit_Red2();
	    RTOS_SVC_threadDelay(10);

		 CAN1_Transmit_Green1();
	    RTOS_SVC_threadDelay(10);

		 CAN1_Transmit_Green2();
	    RTOS_SVC_threadDelay(10);
	}
}

/**
 * @brief   thread2function
 * @note
 * @param   none
 * @retval  none
 */
void thread2function(void) {

	uint32_t msg = 0x55667788;

	while (1) {
		RTOS_SVC_semaphoreTake(&semaphore1, 1);
		 RTOS_SVC_semaphoreGive(&semaphore1);

		if(RTOS_SUCCESS == RTOS_SVC_mutexLock(&mutex1, 10))
		    {
		      RTOS_SVC_threadDelay(15);
		      RTOS_SVC_mutexRelease(&mutex1);
		    }
		    else
		    {
		    }

		    RTOS_SVC_threadDelay(1);
	    CAN1_Receive();
	    RTOS_SVC_threadDelay(1);
	}
}

/**
 * @brief   thread2function
 * @note
 * @param   none
 * @retval  none
 */
void thread3function(void) {
	uint32_t msg = 0x99101011;

	while (1) {
	    if(RTOS_SUCCESS == RTOS_SVC_mutexLock(&mutex1, 10))
	    {
	      ITM_Printf("Thread 3: Mutex lock succeeded \n");
	      RTOS_SVC_threadDelay(15);
	      RTOS_SVC_mutexRelease(&mutex1);
	    }
	    else
	    {

	    }

	    RTOS_SVC_threadDelay(1);
	}
}

/**
 * @brief   thread2function
 * @note
 * @param   none
 * @retval  none
 */
void thread4function(void) {

	while (1) {


	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */

	/* USER CODE BEGIN SysInit */

	CAN1_Init();

	RTOS_init();

	RTOS_SVC_threadCreate(&thread1, &thread1stack, 1, thread1function);
	RTOS_SVC_threadCreate(&thread2, &thread2stack, 1, thread2function);
	RTOS_SVC_threadCreate(&thread3, &thread3stack, 1, thread3function);
	RTOS_SVC_threadCreate(&thread4, &thread4stack, 1, thread4function);

	RTOS_SVC_mutexCreate(&mutex1, 1);
	RTOS_SVC_semaphoreCreate(&semaphore1, 1);
	RTOS_SVC_mailboxCreate(&mailbox1, mailbox1buffer, 2, 4);

	RTOS_SVC_schedulerStart();

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 180;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
