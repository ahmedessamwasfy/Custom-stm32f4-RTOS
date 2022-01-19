/********************************************************************
 **
 **
 ** file    		Rtos_thread.h
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_thread.h
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

#ifndef INC_RTOS_THREAD_H_
#define INC_RTOS_THREAD_H_

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
#include "Rtos_config.h"
#include "Rtos_list.h"

/* ==================================================================== */
/* ======================== Macros and Defines ======================== */
/* ==================================================================== */

/* #define and enum statements and macros go here */

/**
 * @brief   
 */

/* ==================================================================== */
/* ========================== TypeDefines  ============================ */
/* ==================================================================== */

/* Struct and typedef go here */

/**
 * @brief	its the thread object that contain Tcb ptr , priority and its list item
 */
typedef struct thread_t {
	uint32_t pStackPointer; /* A pointer to the thread stack */

	uint32_t priority; /* the thread priority and points to the place of TCB */

	uint32_t threadID; /**< Used for trace */

	RTOS_listItem_t item; /* List item of this thread  , part of the list to tell us its position*/

	RTOS_listItem_t genericListItem; /**< Generic list item of this thread */

	RTOS_listItem_t eventListItem; /**< Event list item of this thread */

	#if (1 == USE_PRIORITY_INHERITANCE)
	uint32_t originalPriority;     /**< Thread original priority */
	#endif

} RTOS_thread_t;

/**
 * @brief   type def of the stack
 */

typedef struct {
	uint64_t stack[THREAD_STACK_SIZE];
} RTOS_stack_t;

/* ==================================================================== */
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

/* ==================================================================== */
/* =================== Exported Public functions ====================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */

/**
 * @brief   Initialize thread list
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadInitLists(void);

/**
 * @brief
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_threadCreate(RTOS_thread_t *pThread, RTOS_stack_t *pStack,
		uint32_t priority, void *pFunction);

void RTOS_SVC_threadCreate(RTOS_thread_t *pThread, RTOS_stack_t *pStack,
		uint32_t priority, void *pFunction);
/**
 * @brief
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_threadGetCuurentReady(void);

/**
 * @brief
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_threadSwitchContext(void);

/**
 * @brief   Add thread to the ready list
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadAddToReadyList(RTOS_thread_t *pThread);

/**
 * @brief   Switch the current running thread
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadSwitchRunning(void);

/**
 * @brief   Get running thread
 * @note
 * @param
 * @retval  None
 */
RTOS_thread_t* RTOS_threadGetRunning(void);

/**
 * @brief   Add thread to the timer list
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_threadAddRunningToTimerList(uint32_t waitTime);

/**
 * @brief   Refresh the timer list
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_threadRefreshTimerList(void);
void RTOS_SVC_threadDelay(uint32_t waitTime);



/**
 * @brief   Destroys a thread
 * @note
 * @param   RTOS_thread_t *
 * @retval  None
 */
void RTOS_threadDestroy(RTOS_thread_t *pThread);
void RTOS_SVC_threadDestroy(RTOS_thread_t *pThread);




void RTOS_threadPriorityInherit(RTOS_thread_t * pThread);
void RTOS_threadPriorityDisinherit(void);

void RTOS_threadPrioritySet(uint32_t newPriority);
void RTOS_SVC_threadPrioritySet(uint32_t newPriority);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_RTOS_THREAD_H_ */
