/********************************************************************
 **
 **
 ** file    		Rtos_mailbox.h
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 8, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_mailbox.h
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

#ifndef INC_RTOS_MAILBOX_H_
#define INC_RTOS_MAILBOX_H_

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
 * @brief      Mailbox type
 * @note
 * @see
 */
typedef struct {
	int8_t *pStart;			 	/**< Start location ?*/
	int8_t *pEnd;			 	/**< Last location ?*/
	int8_t *pReadIndex;		 	/**< Read location */
	int8_t *pWriteIndex; 	 	/**< Write location */
	uint32_t bufferLength;	 	/**< Length of the mailbox buffer */
	uint32_t messageSize;	 	/**< Mailbox message size in bytes either 1== byte ,2==half word , 4== full word*/
	uint32_t messagesNum;	 	/**< Current number of messages inside the buffer */
	RTOS_list_t waitingList; 	/**< Waiting list */
	/* we have only one waiting list for reading wait and writing wait
	 * because it is impossible to have in the same waiting list threads
	 * blocked for writing and threads blocked for reading
	 * because there is only 2 option : 1. mail box is full  so no thread can write but anyone can read
	 * 									2. mail box is empty so no thread can read  but anyone can write  */

} RTOS_mailbox_t;
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
 * @brief   Mailbox create
 * @note
 * @param   RTOS_mailbox_t *, void *, uint32_t, uint32_t
 * @retval  None
 */
void RTOS_mailboxCreate(RTOS_mailbox_t *pMailbox, void *pBuffer,
		uint32_t bufferLength, uint32_t messageSize);
void RTOS_SVC_mailboxCreate(RTOS_mailbox_t *pMailbox, void *pBuffer,
		uint32_t bufferLength, uint32_t messageSize);

/**
 * @brief   Mailbox write
 * @note
 * @param   RTOS_mailbox_t *, int32_t, const void * const
 * @retval  RTOS_return_t
 */
RTOS_return_t RTOS_mailboxWrite(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		const void *const pMessage);
RTOS_return_t RTOS_SVC_mailboxWrite(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		const void *const pMessage);

/**
 * @brief   Mailbox read
 * @note
 * @param   RTOS_mailbox_t *, int32_t, void * const
 * @retval  uint32_t
 */
RTOS_return_t RTOS_mailboxRead(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		void *const pMessage);
RTOS_return_t RTOS_SVC_mailboxRead(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		void *const pMessage);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_RTOS_MAILBOX_H_ */
