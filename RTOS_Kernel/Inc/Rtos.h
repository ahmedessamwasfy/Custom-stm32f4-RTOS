/********************************************************************
 **
 **
 ** file    		Rtos.h
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos.h
 ** note
 **
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU General Public License
 ** as published by the Free Software Foundation; either version 2
 ** of the License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 **
 ********************************************************************/

#ifndef INC_RTOS_H_
#define INC_RTOS_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

/* Inclusion of system and local header files goes here */

/**
 * @brief   
 */
#include "stddef.h"
#include "string.h"
//#include "stm32f4xx.h"
#include "itm.h"
#include "Rtos_config.h"


/* ==================================================================== */
/* ======================== Macros and Defines ======================== */
/* ==================================================================== */

/* #define and enum statements and macros go here */

/**
 * @brief   
 */

/**
 * @brief   Waiting flags
 */
#define NO_WAIT                 ((int32_t) 0)
#define WAIT_INDEFINITELY       ((int32_t) -1)




/* ==================================================================== */
/* ========================== TypeDefines  ============================ */
/* ==================================================================== */

/* Struct and typedef go here */

/**
 * @brief   RTOS return types
 */
typedef enum
{
  RTOS_SUCCESS,
  RTOS_FAILURE,
  RTOS_CONTEXT_SWITCH_TRIGGERED
} RTOS_return_t;



///* ==================================================================== */
/* ======================= Exported Variables ========================== */
/* ==================================================================== */

/* Global variables definitions go here */

/**
 * @brief   
 */

/* ==================================================================== */
/* ========================== private data ============================ */
/* ==================================================================== */

/* Definition of private datatypes go here */

/* ==================================================================== */
/* ====================== private functions =========================== */
/* ==================================================================== */

/* Function prototypes for private (static) functions go here */
/**
 * @brief   Idle thread function
 * @note
 * @param   None
 * @retval  None
 */

/* ==================================================================== */
/* =================== Exported Public functions ====================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */

/**
 * @brief   RTOS Initialization function
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_init(void);


/**
 * @brief   SVC handler main
 * @note
 * @param   uint32_t *
 * @retval  None
 */
void RTOS_SVC_Handler_main(uint32_t * svc_args);


/**
 * @brief   SVC handler main
 * @note
 * @param   uint32_t *
 * @retval  None
 */
void RTOS_SVC_schedulerStart(void);



/**
 * @brief Start RTOS scheduler
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_schedulerStart(void);

/**
 * @brief   Returns scheduler running status
 * @note
 * @param   None
 * @retval  None
 */
uint32_t RTOS_isSchedulerRunning(void);

/**
 * @brief   SysTick Handler
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_SysTick_Handler(void);




#include "Rtos_list.h"
#include "Rtos_thread.h"
#include "Rtos_mutex.h"
#include "Rtos_semaphore.h"
#include "Rtos_mailbox.h"



/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_RTOS_H_ */
