/********************************************************************
 **
 **
 ** file    		Rtos_mailbox.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 8, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_mailbox.c
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

/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

/* Inclusion of system and local header files goes here */

/**
 * brief   
 */

#include "Rtos.h"

/* ==================================================================== */
/* ======================== Macros and Defines ======================== */
/* ==================================================================== */

/* #define and enum statements and macros go here */

/**
 * brief   
 */

/* ==================================================================== */
/* ======================= Exported Variables ========================== */
/* ==================================================================== */

/* Global variables definitions go here */

/**
 * brief  
 */

/* ==================================================================== */
/* ========================== private data ============================ */
/* ==================================================================== */

/* Definition of private datatypes go here */

/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */

/**
 * @defgroup rtos_mailbox_private_function_prototypes
 * @{
 */

static void checkWaitingThreads(RTOS_mailbox_t *pMailbox);
static void blockCurrentThread(RTOS_mailbox_t *pMailbox);

/* ==================================================================== */
/* ====================== private functions =========================== */
/* ==================================================================== */

/* Function prototypes for private (static) functions go here */

/**
 * @brief   Check if threads are waiting for a new data
 * @note
 * @param   RTOS_mailbox_t *
 * @retval  None
 */
static void checkWaitingThreads(RTOS_mailbox_t *pMailbox) {
	/* Check input parameters */
	ASSERT_param(NULL != pMailbox);

	/* Pointer to the unblocked thread */
	RTOS_thread_t *pThread;

	/* Check if threads are waiting for the data */
	if (0 < pMailbox->waitingList.numOfItems) {
		/* Get first one */
		pThread = pMailbox->waitingList.listEnd.pNext->pThread;

		/* Check returned thread */
		ASSERT_param(NULL != pThread);

		/* Remove the returned thread item from the waiting list */
		RTOS_listRemove(&pThread->eventListItem);

		/* Check if the generic item in any list */
		if (NULL != pThread->genericListItem.pList) {
			/* Remove the generic item from the current list,
			 * as it will be inserted into ready list */
			RTOS_listRemove(&pThread->genericListItem);
		} else {
			/* Do nothing, generic item is not in any list */
		}

		/* Add the returned thread into ready list */
		RTOS_threadAddToReadyList(pThread);
	} else {
		/* List is empty */
	}
}

/**
 * @brief   Block current thread when buffer is full or empty
 * @note
 * @param   RTOS_mailbox_t *
 * @retval  None
 */
static void blockCurrentThread(RTOS_mailbox_t *pMailbox) {
	/* Pointer to the current running thread */
	RTOS_thread_t *pRunningThread;

	/* Get current running thread */
	pRunningThread = RTOS_threadGetRunning();

	/* Remove current thread from ready list */
	RTOS_listRemove(&pRunningThread->genericListItem);

	/* Put current thread into the waiting list */
	RTOS_listInsert(&pMailbox->waitingList, &pRunningThread->eventListItem);

	/* Trigger context switch, set PendSV to pending */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* ==================================================================== */
/* =================== Exported Public functions ====================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */

/**
 * brief
 * note
 * param   void
 * retval  void
 */

/**
 * @brief   Mailbox create
 * @note
 * @param   RTOS_mailbox_t *, void *, uint32_t, uint32_t
 * @retval  None
 */
void RTOS_mailboxCreate(RTOS_mailbox_t *pMailbox, void *pBuffer,
		uint32_t bufferLength, uint32_t messageSize) {

	/* Check input parameters */
	ASSERT_param(NULL != pMailbox);
	ASSERT_param(NULL != pBuffer);
	ASSERT_param(0 != bufferLength);
	ASSERT_param(
			(1 == messageSize) || (2 == messageSize) || (4 == messageSize));

	/*Set mailbox buffer start and end*/
	pMailbox->pStart = (int8_t*) pBuffer;

	/* the end of the mail box would be points to an address after the last element ,
	 *  so if we have buffer of 10 items pEnd will points to buffer[10] not buffer[9] */
	pMailbox->pEnd = pMailbox->pStart + (bufferLength * messageSize);

	/* Initialize read and write indices */
	pMailbox->pReadIndex = pMailbox->pStart;
	pMailbox->pWriteIndex = pMailbox->pStart;

	/* Set buffer length and message size */
	pMailbox->bufferLength = bufferLength;
	pMailbox->messageSize = messageSize;

	/* Initialize current number of messages */
	pMailbox->messagesNum = 0;

	/* Initialize mailbox waiting list */
	RTOS_listInit(&pMailbox->waitingList);

}
/**
 * @brief   Mailbox write
 * @note
 * @param   RTOS_mailbox_t *, int32_t, const void * const
 * @retval  RTOS_return_t
 */
RTOS_return_t RTOS_mailboxWrite(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		const void *const pMessage) {

	/* Check input parameters */
	ASSERT_param(NULL != pMailbox);
	ASSERT_param(WAIT_INDEFINITELY <= waitTime);
	ASSERT_param(NULL != pMessage);

	/* Return status */
	RTOS_return_t returnStatus = RTOS_FAILURE;

	/* Check if there is a free place to write */
	if (pMailbox->bufferLength > pMailbox->messagesNum) {

		/* add the message to the mail box */
		memcpy((void*) pMailbox->pWriteIndex, pMessage, pMailbox->messageSize);

		/* Increment write index to point to the next message */
		pMailbox->pWriteIndex += pMailbox->messageSize;

		/* Check if the buffer is written until the end */
		if (pMailbox->pWriteIndex >= pMailbox->pEnd) {

			/* Start again from the beginning of the buffer */
			pMailbox->pWriteIndex = pMailbox->pStart;
		} else {
			/* it still didn't reach the end of buffer*/
		}

		/*increase number of messages in mailbox*/
		pMailbox->messagesNum++;

		/* Check waiting threads */
		checkWaitingThreads(pMailbox);

		/* Set the return status to succeeded  */
		returnStatus = RTOS_SUCCESS;

	} else {
		/* Buffer is Full */
	}
	/* check if the buffer was full and the waiting flag is set */
	if ((NO_WAIT != waitTime) && (RTOS_SUCCESS != returnStatus)) {

		/* use the blocking function  */
		/* Block current thread */
		blockCurrentThread(pMailbox);

		/* Check waiting time */
		if (NO_WAIT < waitTime) {
			/* Waiting time configured, add current to the timer list */
			RTOS_threadAddRunningToTimerList(waitTime);
		} else {
			/* Thread will wait indefinitely, do nothing */
		}

		/* Set the return status to failed and blocked  */
		returnStatus = RTOS_CONTEXT_SWITCH_TRIGGERED;

	} else {
		/*  return status to failed and un blocked  */
	}

	return returnStatus;
}

/**
 * @brief   Mailbox read
 * @note
 * @param   RTOS_mailbox_t *, int32_t, void * const
 * @retval  uint32_t
 */
RTOS_return_t RTOS_mailboxRead(RTOS_mailbox_t *pMailbox, int32_t waitTime,
		void *const pMessage) {

	/* Check input parameters */
	ASSERT_param(NULL != pMailbox);
	ASSERT_param(WAIT_INDEFINITELY <= waitTime);
	ASSERT_param(NULL != pMessage);

	/* Return status */
	RTOS_return_t returnStatus = RTOS_FAILURE;

	/*  */

	/* check if the buffer has items  */
	if (0 < pMailbox->messagesNum) {

		/* copy the message */
		memcpy(pMessage, (void*) pMailbox->pReadIndex, pMailbox->messageSize);

		/* Increment Read index to point to the next message */

		pMailbox->pReadIndex += pMailbox->messageSize;

		/* check if we reached to the end of the buffer  */
		if (pMailbox->pReadIndex >= pMailbox->pEnd) {
			/* Start over */
			pMailbox->pReadIndex = pMailbox->pStart;
		} else {
			/* we didn't reach the end */
		}

		/* Decrement the number of messages */
		pMailbox->messagesNum--;

		/* Check waiting threads */
		checkWaitingThreads(pMailbox);

		/* New message is removed from the buffer, return OK */
		returnStatus = RTOS_SUCCESS;

	} else {
		/* buffer is empty , there is nothing to read */
	}

	/* Check waiting flag and return status */
	if ((NO_WAIT == waitTime) && (RTOS_SUCCESS != returnStatus)) {

		/* Block current thread */
		blockCurrentThread(pMailbox);

		/* Check waiting time */
		if (NO_WAIT < waitTime) {
			/* Waiting time configured, add current to the timer list */
			RTOS_threadAddRunningToTimerList(waitTime);
		} else {
			/* Thread will wait indefinitely, do nothing */
		}

		/* Return to SVC as indication of context switch */
		returnStatus = RTOS_CONTEXT_SWITCH_TRIGGERED;

	} else {
		/* No blocking required, do nothing */

	}

	return returnStatus;

}

