/********************************************************************
**
** 
** file    		Rtos_config.h
** author  		Ahmed Wasfy
** email   		ahmedessam0128@gmail.com
** date    		Dec 3, 2021
** Target  		STM32F4291-discoveryboard
** Version     	1.0.0
**
** brief   		Rtos_config.h 
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



#ifndef INC_RTOS_CONFIG_H_
#define INC_RTOS_CONFIG_H_


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




#include"HelpFun.h"

/* ==================================================================== */
/* ======================== Macros and Defines ======================== */
/* ==================================================================== */

/* #define and enum statements and macros go here */

/**
 * @brief   Stack size for each thread in double word eg. 1024 means 8Kbytes (kbyte==1024 bit ,8byte = 64 bits )
 */
#define THREAD_STACK_SIZE		((uint32_t) 1024u)

/**
 * @brief   Thread maximum priority,
 * 			 priority from 0 ->  THREAD_PRIORITY_LEVEL-1,
 * 			 used in initializing the ready and waiting list
 */
#define THREAD_PRIORITY_LEVELS	((uint32_t) 16u)

/**
 * @brief   CPU  clock frequency in hertz
 */
#define SYSTEM_CORE_CLOCK_HZ	((uint32_t) 180000000u)

/**
 * @brief   system tick frequency in hertz
 */
#define SYS_TICK_RATE_HZ		((uint32_t) 1000u)


/**
 * @brief   Select using priority inheritance
 */
#define USE_PRIORITY_INHERITANCE             (1u)

/**
 * @brief   Select using set priority, used for priority ceiling
 */
#define USE_PRIORITY_SET                     (1u)


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

/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif /* INC_RTOS_CONFIG_H_ */
