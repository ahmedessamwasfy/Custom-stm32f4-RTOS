/********************************************************************
 **
 **
 ** file    		Rtos_semaphore.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 8, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_semaphore.c
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
 * @brief   Semaphore create
 * @note
 * @param   RTOS_semaphore_t *
 * @retval  None
 */
void RTOS_semaphoreCreate(RTOS_semaphore_t *pSemaphore, uint32_t initialValue) {

	/* Check input parameters */
	ASSERT_param(NULL != pSemaphore);

	/* Initialize semaphore waiting list */
	RTOS_listInit(&pSemaphore->waitingList);

	/* Initialize semaphore value */
	pSemaphore->semaphoreValue = initialValue;
}

/**
 * @brief   Semaphore take
 * @note    waitFlag means that he wants to be put in waiting list if the Semaphore is locked
 * @param   RTOS_semaphore_t *, int32_t
 * @retval  RTOS_return_t
 */
RTOS_return_t RTOS_semaphoreTake(RTOS_semaphore_t *pSemaphore, int32_t waitTime) {

	/* Check input parameters */
	ASSERT_param(NULL != pSemaphore);
	ASSERT_param(WAIT_INDEFINITELY <= waitTime);

	/* Semaphore take return status */
	RTOS_return_t returnStatus = RTOS_FAILURE;

	/*  RTOS_SUCCESS,
  RTOS_CONTEXT_SWITCH_TRIGGERED*/

	/* Pointer to the current running thread */
	RTOS_thread_t *pRunningThread;

	/* Temp variable to store the semaphore value */
	/* We use temp semaphore value because if the load store failed
	 * it will try again so we take the current value of the semaphore again
	 * and edit it in store again in order not to make accumulative error  */
	uint32_t semaphoreValueTemp = 0;

	/* Variable to terminate the while loop */
	uint32_t terminate = 0;

	while (0 == terminate) {

		/* Read the semaphore counter value */
		semaphoreValueTemp = __LDREXW(&pSemaphore->semaphoreValue);

		/* Check semaphore counter value, note in ARMv7-M exceptions automatically
		 * clear the exclusive state in the local monitor,
		 * no need to use CLREX instruction */
		if (0 < semaphoreValueTemp) {

			/* Semaphore is not fully taken, take one by decrement */
			if (0
					== __STREXW((semaphoreValueTemp - 1),
							&pSemaphore->semaphoreValue)) {

				/* Required before accessing protected resource */
				__DMB();

				/* Semaphore is taken, return OK */
				returnStatus = RTOS_SUCCESS;

				/* Store succeeded, terminate the loop */
				terminate = 1;

			} else {
				/* Problem during decrementing, try again */

			}

		} else { /* Semaphore is not available, terminate the loop */

			/* clear the local record of the executing processor that an address has had a request for an exclusive access.
			 * used because if we couldn't reach storeEx after the load we clear the exclusive state */
			/* its the somthing that telling us that the load store falied */
			__CLREX();
			terminate = 1;
		}
	}

	/* Check waiting flag and return status */
	if ((NO_WAIT != waitTime) && (RTOS_SUCCESS != returnStatus)) {

		/* Get current running thread */
		pRunningThread = RTOS_threadGetRunning();

		/* Remove current thread from ready list */
		RTOS_listRemove(&pRunningThread->genericListItem);

		/* Put current thread into the waiting list */
		RTOS_listInsert(&pSemaphore->waitingList, &pRunningThread->eventListItem);

		/* Trigger context switch, set PendSV to pending */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	    /* Check waiting time */
	    if(NO_WAIT < waitTime)
	    {
	      /* Waiting time configured, add current to the timer list */
	      RTOS_threadAddRunningToTimerList(waitTime);
	    }
	    else
	    {
	      /* Thread will wait indefinitely, do nothing */
	    }


		/* Return to SVC as indication of context switch */
		returnStatus = RTOS_CONTEXT_SWITCH_TRIGGERED;
	} else {
		/* No blocking required, do nothing */
		/* returnStatus still equals Zero */
	}
	/* Return */
	return returnStatus;
}

/**
 * @brief   Semaphore give
 * @note
 * @param   RTOS_semaphore_t *
 * @retval  None
 */
void RTOS_semaphoreGive(RTOS_semaphore_t *pSemaphore) {

	  /* Check input parameters */
	  ASSERT_param(NULL != pSemaphore);

	  /* Pointer to the unblocked thread */
	  RTOS_thread_t * pThread;

	  /* Temp variable used to read the semaphore counter value */
	  uint32_t semaphoreValueTemp = 0;

	  /* Variable to terminate the while loop */
	  uint32_t terminate = 0;

	  /* Try incrementing the semaphore counter */
	  while(1 != terminate)
	  {
	    /* Get the semaphore value */
	    semaphoreValueTemp = __LDREXW(&pSemaphore->semaphoreValue);

	    /* Increment temp variable */
	    semaphoreValueTemp++;

	    /* Try to store the incremented semaphore value */
	    if(0 == __STREXW(semaphoreValueTemp, &pSemaphore->semaphoreValue))
	    {
	      /* Store succeeded, terminate the loop */
	      terminate = 1;

	      /* Required before releasing protected resource */
	      __DMB();
	    }
	    else
	    {
	      /* Store failed, try again */
	    }
	  }

	  /* Get highest priority waiting thread, the one next from the end */
	  if(0 < pSemaphore->waitingList.numOfItems)
	  {
		  /* get the highest priority thread in the semaphore waiting list */
	    pThread = pSemaphore->waitingList.listEnd.pNext->pThread;

	    /* Check returned thread */
	    ASSERT_param(NULL != pThread);

	    /* Remove the returned thread item from the waiting list */
	    RTOS_listRemove(&pThread->eventListItem);


	    /* Check if the generic item in any list */
	    if(NULL != pThread->genericListItem.pList)
	    {
	      /* Remove the generic item from the current list,
	       * as it will be inserted into ready list */
	      RTOS_listRemove(&pThread->genericListItem);
	    }
	    else
	    {
	      /* Do nothing, generic item is not in any list */
	    }

	    /* Add the returned thread into ready list */
	    RTOS_threadAddToReadyList(pThread);
	  }
	  else
	  {
	    /* No threads are waiting, do nothing */
	  }
}

