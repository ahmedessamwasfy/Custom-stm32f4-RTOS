/********************************************************************
**
** 
** file    		Rtos_mutex.h
** author  		Ahmed Wasfy
** email   		ahmedessam0128@gmail.com
** date    		Dec 8, 2021
** Target  		STM32F4291-discoveryboard
** Version     	1.0.0
**
** brief   		Rtos_mutex.h 
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



#ifndef INC_RTOS_MUTEX_H_
#define INC_RTOS_MUTEX_H_


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
 * @brief   
 */

/**
 * @brief      Mutex type
 * @note
 * @see
 */
typedef struct
{
  uint32_t mutexValue;            /**< Mutex value */
#if (1 == USE_PRIORITY_INHERITANCE)
  RTOS_thread_t * mutexHolder;    /**< Thread currently holding the mutex */
#endif
  RTOS_list_t waitingList;        /**< Waiting list of the mutex */
} RTOS_mutex_t;


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
 * @brief
 * @note
 * @param   void
 * @retval  void
 */


/**
 * @brief   Mutex create
 * @note
 * @param   RTOS_mutex_t *
 * @retval  None
 */
void RTOS_mutexCreate(RTOS_mutex_t * pMutex, uint32_t initialValue);
void RTOS_SVC_mutexCreate(RTOS_mutex_t * pMutex, uint32_t initialValue);


/**
 * @brief   Mutex lock
 * @note    waitFlag means that he wants to be put in waiting list if the mutex is locked
 * @param   RTOS_mutex_t *, int32_t
 * @retval  RTOS_return_t
 */
RTOS_return_t RTOS_mutexLock(RTOS_mutex_t * pMutex, int32_t waitFlag);
RTOS_return_t RTOS_SVC_mutexLock(RTOS_mutex_t * pMutex, uint32_t waitFlag);



/**
 * @brief   Mutex release
 * @note
 * @param   RTOS_mutex_t *
 * @retval  None
 */
void RTOS_mutexRelease(RTOS_mutex_t * pMutex);
void RTOS_SVC_mutexRelease(RTOS_mutex_t * pMutex);


/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif /* INC_RTOS_MUTEX_H_ */
