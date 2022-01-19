/********************************************************************
 **
 **
 ** file    		Rtos_mutex.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 8, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_mutex.c
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

/* ==================================================================== */
/* ====================== private functions =========================== */
/* ==================================================================== */

/* Function prototypes for private (static) functions go here */

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
 * @brief   Mutex create
 * @note
 * @param   RTOS_mutex_t *
 * @retval  None
 */
void RTOS_mutexCreate(RTOS_mutex_t *pMutex, uint32_t initialValue) {

	/* check input parameters */
	ASSERT_param(NULL!=pMutex);
	ASSERT_param((initialValue == 0) || (initialValue == 1));

	/* initialize mutex waiting list */
	RTOS_listInit(&pMutex->waitingList);

	/*initialize the mutex value*/
	pMutex->mutexValue = initialValue;

#if(1 == USE_PRIORITY_INHERITANCE)
	{
	  /* Initialize mutex holder */
	  pMutex->mutexHolder = NULL;
	}
	#endif

}

/**
 * @brief   Mutex lock
 * @note    waitFlag means that he wants to be put in waiting list if the mutex is locked
 * @param   RTOS_mutex_t *, int32_t
 * @retval  RTOS_return_t
 */
RTOS_return_t RTOS_mutexLock(RTOS_mutex_t *pMutex, int32_t waitTime) {

	/* check input parameters */
	ASSERT_param(NULL!=pMutex);
	ASSERT_param(WAIT_INDEFINITELY<=waitTime);

	/*create return Status*/
	RTOS_return_t returnStatus = RTOS_FAILURE;

	/* Pointer to the current running thread */
	RTOS_thread_t *pRunningThread;

	/* create terminator of the while loop */
	uint32_t terminate = 0;

	/* Try to lock the mutex */
	while (terminate == 0) {
		/* Check mutex value, note in ARMv7-M exceptions automatically clear the
		 * exclusive state in the local monitor, no need to use CLREX instruction */
		if (1 == __LDREXW(&pMutex->mutexValue)) {
			/* Mutex is free, locked it */
			if (0 == __STREXW(0, &pMutex->mutexValue)) {

				/* Required before accessing protected resource */
				// Do not start any other memory access until memory barrier is complete
				__DMB();

				/* Mutex is locked */
				returnStatus = RTOS_SUCCESS;

#if(1 == USE_PRIORITY_INHERITANCE)
				{
			          /* Update mutex holder */
			          pMutex->mutexHolder = RTOS_threadGetRunning();
				}
				#endif

				/* Mutex lock succeeded, terminate the loop */
				terminate = 1;

			} else {

				/* Mutex lock failed within locking we will try again */
			}
		} else {
			/* Return status un changed */
			/* Mutex is locked, terminate the loop */
			terminate = 1;
		}

	}

	/* Check waiting flag and return status */
	/* waiting flag is the flag telling us whether the thread will wait
	 * and be blocked till the mutex is released or not */
	if ((NO_WAIT != waitTime) && (RTOS_SUCCESS != returnStatus)) {
		/* Get current running thread */
		pRunningThread = RTOS_threadGetRunning();

		/* Remove current thread from ready list */
		RTOS_listRemove(&pRunningThread->genericListItem);

		/* Put current thread into the waiting list in priority order */
		RTOS_listInsert(&pMutex->waitingList, &pRunningThread->eventListItem);

		/* Trigger context switch, set PendSV to pending */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

		/* Check waiting time */
		/* if wait time is greater than 0 so we put it in timer waiting list */
		if (NO_WAIT < waitTime) {

			/* Waiting time configured, add current to the timer list */
			RTOS_threadAddRunningToTimerList(waitTime);

		} else {
			/* Thread will wait indefinitely, do nothing */
		}

		#if(1 == USE_PRIORITY_INHERITANCE)
		{
		  /* Inherit the priority for the mutex holder */
		  RTOS_threadPriorityInherit(pMutex->mutexHolder);
		}
		#endif

		/* Return to SVC as indication of context switch */
		returnStatus = RTOS_CONTEXT_SWITCH_TRIGGERED;
	} else {
		/* No blocking required, do nothing */

	}
	/* Return */
	return returnStatus;
}

/**
 * @brief   Mutex release
 * @note
 * @param   RTOS_mutex_t *
 * @retval  None
 */
void RTOS_mutexRelease(RTOS_mutex_t *pMutex) {

	/* Check input parameters */
	ASSERT_param(NULL != pMutex);

	/* Pointer to the unblocked thread */
	RTOS_thread_t *pThread;

	/* Required before releasing protected resource */
	__DMB();

	/* Release the mutex */
	pMutex->mutexValue = 1;

	/* Get highest priority waiting thread, the one next from the end */
	if (0 < pMutex->waitingList.numOfItems) {

		/* when we inserted the items we insert it using priority order so list number is the number after the endList item */
		pThread = pMutex->waitingList.listEnd.pNext->pThread;

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
		/* No threads are waiting, do nothing */
	}

	#if(1 == USE_PRIORITY_INHERITANCE)
	  {
		/* Disinherit the priority for the current thread */
		RTOS_threadPriorityDisinherit();

		/* Update mutex holder */
		pMutex->mutexHolder = NULL;
	  }
	  #endif

}
