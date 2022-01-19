/********************************************************************
 **
 **
 ** file    		Rtos_thread.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_thread.c
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
 * brief   RTOS_Kernel/IncRTOS_Kernel/Inc
 */

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

/**
 * @brief   ready list for the threads waiting for execution arranged by priority
 *
 * 	@NOTE	this ready list is an array of linked list each element is indication for the linked list of
 * 			each level (eg. list[0] there is a linked list of all threads with priority 0 and so on)
 */
static RTOS_list_t readyList[THREAD_PRIORITY_LEVELS];

/**
 * @brief   Timer list for the threads waiting, arranged by timeout value.
 */
static RTOS_list_t timerList;

/**
 * @brief	the current top priority to help us accessing the array of linked list
 *
 *
 * */
static uint32_t currentTopPriotiy = (THREAD_PRIORITY_LEVELS - 1);

/**
 * @brief   Currently running thread pointer.
 */
static RTOS_thread_t *pRunningThread;

/**
 * @brief   Currently running thread ID.
 */
static uint32_t runningThreadID = 0;

/**
 * @brief   Number of threads
 */
static uint32_t numOfThreads = 0;

/**
 * @brief   Variable to store millisecond ticks
 */
static volatile uint32_t sysTickCounter = 0;

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
 * @brief   Initialize thread list
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadInitLists(void) {
	/* Initialize ready list */
	for (uint32_t priority = 0; priority < THREAD_PRIORITY_LEVELS; priority++) {
		RTOS_listInit(&readyList[priority]);
	}
}

/**
 * @brief	Creates new thread.
 * @note	Here we create TCB of the new thread
 * @param   RTOS_thread_t * thread object , Stack , priority of thread , function of thread
 * @retval  void
 */
void RTOS_threadCreate(RTOS_thread_t *pThread, RTOS_stack_t *pStack,
		uint32_t priority, void *pFunction) {

	/* Check input parameters */
	ASSERT_param(pThread != NULL);
	ASSERT_param(pStack != NULL);
	ASSERT_param(pFunction != NULL);
	ASSERT_param(priority < THREAD_PRIORITY_LEVELS);

	/* Create stack frame, size multiplied with 8 for the double word
	 * length converted to byte length, stack frame size is 18 words */
	/* Note that Stack is 64bit and the special registers are 32 bit so to
	 * put the stack pointer just above the special registers we add the whole
	 * size (stack size * 8)Byte  and decrease by (18 * 4)byte */
	pThread->pStackPointer =
			((uint32_t) pStack + THREAD_STACK_SIZE * 8 - 18 * 4); /* WATCH CAREFULLY  */ //(THREAD_STACK_SIZE * 8 == 8K )

	/* Write thread function into return address */
	/*PUT the function adders in the LR (link register) which is the register
	 * Responsible for the returning to the function after branching like the
	 * assembly instruction (bl) means this is the reg that keep our function to
	 * come back after interrupt .... also this is what mainly will make the context
	 * switching of the function*/
	/*@Note		16<<2  == 16*4  thats because is LR is third register from the the end
	 * 			after program counter*/

	MEM32_ADDRESS(pThread->pStackPointer + (16 << 2)) = (uint32_t) pFunction;

	/* Write initial xPSR, program status register, thumb */
	MEM32_ADDRESS((pThread->pStackPointer + (17 << 2))) = 0x01000000;

	/* Write EXC_RETURN, since the execution threads are using PSP, this will
	 * allow SVC to return to thpStackPointere thread with PSP */
	MEM32_ADDRESS(pThread->pStackPointer) = 0xFFFFFFFDUL;

	/* Write initial CONTROL register value UNPRIVILEGED, PSP & no FPU */
	/* it controls three things bit[0] PRIVILEGED(0) or UNPRIVILEGED(1) bit[1] Stack pointer as current stack(0)
	 * or Use Process stack pointer (1) and bit[2] Floating point not active(0) floating point active(1)   */
	MEM32_ADDRESS((pThread->pStackPointer + (1 << 2))) = 0x3;

	/* Set thread priority */
	pThread->priority = priority;

#if(1 == USE_PRIORITY_INHERITANCE)
	{
		/* Set thread original priority */
		pThread->originalPriority = priority;
	}
#endif

	/* Increment number of threads and set the thread ID */
	if (0 == pThread->threadID) {
		/* New thread is created */
		pThread->threadID = ++numOfThreads;
	} else {
		/* Do nothing, this thread was re-created */
	}

	/* Thread is not yet in any list */
	pThread->genericListItem.pList = NULL;
	pThread->eventListItem.pList = NULL;

	/* Link this thread with its list item */
	pThread->genericListItem.pThread = (void*) pThread;
	pThread->eventListItem.pThread = (void*) pThread;

	/* Set the event item value to the priority, this will be used to order the
	 * items by priority in synchronization events list, for the generic lists
	 * e.g. timer list, items are ordered with the timeout value */
	pThread->eventListItem.itemValue = priority;

	/* Add new thread to ready list */
	RTOS_threadAddToReadyList(pThread);
}

/**
 * @brief
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_threadGetCuurentReady(void) {

}

/**
 * @brief
 * @note
 * @param   void
 * @retval  void
 */
void RTOS_threadSwitchContext(void) {

}

/**
 * @brief   Add thread to the ready list
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadAddToReadyList(RTOS_thread_t *pThread) {

	/* Check input parameters */
	ASSERT_param(pThread != NULL);

	/* Add new thread to ready list */
	/* ready list is array of lists based on priority */
	RTOS_listInsertEnd(&readyList[pThread->priority],
			&pThread->genericListItem); // WATCH CAREFULLY FOR THE POSTION AND INDEX

	/* Set current top priority */
	if (pThread->priority < currentTopPriotiy) {
		currentTopPriotiy = pThread->priority;
	}
	/* Check the need for context switch when scheduler is running
	 * and this thread is the higher priority than the running thread */
	/* the lower the priority number the higher the priority */
	if ((NULL != pRunningThread)
			&& (pThread->priority < pRunningThread->priority)) {

		/* Trigger context switch, set PendSV to pending */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	} else {
		/* Context switch is not required */
	}

}

/**
 * @brief   Switch the current running thread
 * @note
 * @param
 * @retval  None
 */
void RTOS_threadSwitchRunning(void) {
	/* find the highest priority ready thread */
	while (readyList[currentTopPriotiy].numOfItems == 0) {
		ASSERT_param(THREAD_PRIORITY_LEVELS>currentTopPriotiy);

		currentTopPriotiy++;
		/* my thought */
		currentTopPriotiy = currentTopPriotiy % THREAD_PRIORITY_LEVELS;
	}
	/* Threads are found, update list index to the next thread */
	/* so we can switch the CPU between the threads with same priority so each one will take 1 quantum */
	RTOS_list_t *pReadyList = &readyList[currentTopPriotiy];
	pReadyList->pIndex = pReadyList->pIndex->pNext;

	/* Check if the new index pointing to the end of list  */
	/* we made the casting and dereference because listEnd and List index are different data type */
	if (pReadyList->pIndex == (RTOS_listItem_t*) &pReadyList->listEnd) {
		/* Get the next thread */
		pReadyList->pIndex = pReadyList->pIndex->pNext;
	} else {
		/* Do nothing, index is not pointing to the end */
	}

	/* Update current running thread  */
	pRunningThread = (RTOS_thread_t*) pReadyList->pIndex->pThread;

	/*update the current running thread ID*/
	runningThreadID = ((RTOS_thread_t*) pReadyList->pIndex->pThread)->threadID;

}
/**
 * @brief   Get running thread
 * @note
 * @param
 * @retval  None
 */
RTOS_thread_t* RTOS_threadGetRunning(void) {
	return pRunningThread;
}

/**
 * @brief   Add thread to the timer list
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_threadAddRunningToTimerList(uint32_t waitTime) {

	/* Check the input parameter */
	ASSERT_param(0 != waitTime);

	/* Temp variable for the wake up tick */
	uint32_t wakeUpTick = 0;

	/* Calculate wake up tick  */
	wakeUpTick = sysTickCounter + waitTime;

	/*Check counter overflow*/
	ASSERT_param(wakeUpTick > sysTickCounter);

	/* Set generic list item value */
	pRunningThread->genericListItem.itemValue = wakeUpTick;

	/* Remove from ready list */
	RTOS_listRemove(&pRunningThread->genericListItem);

	/* Add to timer list */
	RTOS_listInsert(&timerList, &pRunningThread->genericListItem);

	/* Trigger context switch, set PendSV to pending */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/**
 * @brief   Refresh the timer list
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_threadRefreshTimerList(void) {
	/* Pointer to the unblocked thread */
	RTOS_thread_t *pThread;

	/* Check scheduler status */
	if (1 == RTOS_isSchedulerRunning()) {

		/* Increment SysTick counter */
		++sysTickCounter;

		/* Check SysTick counter overflow */
		/* it shouldn't overflow to overflow it should stay on for 30 or 40 days */
		if (0 == sysTickCounter) {
			/* TODO: Handle counter overflow */
			ASSERT_param(0);
		} else {
			/* No counter overflow, do nothing */
		}

		/* Check if timer list has threads waiting */
		if (0 < timerList.numOfItems) {
			/* Timer list is not empty, check timeout values */
			while (sysTickCounter >= timerList.listEnd.pNext->itemValue) {
				/* Get first thread waiting */
				pThread = timerList.listEnd.pNext->pThread;

				/* Check returned thread */
				ASSERT_param(NULL != pThread);

				/* Thread timeout, remove from timer list */
				RTOS_listRemove(&pThread->genericListItem);

				/* Check if the thread waiting for synchronization event */
				if (NULL != pThread->eventListItem.pList) {
					/* Remove the thread from the event list */
					RTOS_listRemove(&pThread->eventListItem);
				} else {
					/* Do nothing, this thread is not in any event lists */
				}

				/* Add the returned thread into ready list */
				RTOS_threadAddToReadyList(pThread);

				/* here I don't trigger a context switching cause it's redundant
				 * as before entring this function we trigger it  */

			}
		} else {
			/* Timer list is empty, do nothing */
		}
	} else {
		/* Scheduler is not running, do nothing */
	}

}

void RTOS_threadDestroy(RTOS_thread_t *pThread) {
	/* Check input parameters */
	ASSERT_param(NULL != pThread);

	/* Check if the generic list item in any list */
	if (NULL != pThread->genericListItem.pList) {
		/* Remove the generic list item from the current list */
		RTOS_listRemove(&pThread->genericListItem);
	} else {
		/* Do nothing, generic list item is not in any list */
	}

	/* Check if the event list item in any list */
	if (NULL != pThread->eventListItem.pList) {
		/* Remove the event list item from the current list */
		RTOS_listRemove(&pThread->eventListItem);
	} else {
		/* Do nothing, event list item is not in any list */
	}

	/* Check if the removed thread is the current thread */
	if (pThread == pRunningThread) {
		/* The current thread will be removed, request context switch  */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	} else {
		/* Do nothing */
	}
}

/**
 * @brief   Priority inherit
 * @note    Gives the passed thread the priority of the current thread
 * @param   RTOS_thread_t *
 * @retval  None
 */
#if (1 == USE_PRIORITY_INHERITANCE)
void RTOS_threadPriorityInherit(RTOS_thread_t *pThread) {
	/* Check input parameters */
	ASSERT_param(NULL != pThread);

	/* Check if the passed thread has higher priority that the running thread */
	if (pThread->priority > pRunningThread->priority) {

		/* Passed thread has priority lower than the current thread,
		 * do priority inheritance */
		/* Update event list item value */
		/* in case he is in waiting for another mutex */
		pThread->eventListItem.itemValue = pRunningThread->priority;

		/* Check if the passed thread in the ready list,
		 * it will be removed and added again */
		if ((void*) &readyList[pThread->priority]
				== pThread->genericListItem.pList) {

			/* Remove from the ready list */
			RTOS_listRemove(&pThread->genericListItem);

			/* Update thread priority */
			pThread->priority = pRunningThread->priority;

			/* Added to the ready list */
			RTOS_threadAddToReadyList(pThread);
		} else {
			/* Passed thread is not in the ready state, only update the priority */
			pThread->priority = pRunningThread->priority;
		}

	} else {
		/* Do nothing, passed thread has priority higher than or equal to
		 * the current thread */
	}

}
#endif
/**
 * @brief   Priority disinherit
 * @note    Restores the original priority of the current thread
 * @param   None
 * @retval  None
 */
#if (1 == USE_PRIORITY_INHERITANCE)
void RTOS_threadPriorityDisinherit(void) {
	/* Check current priority */
	if (pRunningThread->priority != pRunningThread->originalPriority) {

		/* Remove from the ready list */
		RTOS_listRemove(&pRunningThread->genericListItem);

		/* Update thread priority */
		pRunningThread->priority = pRunningThread->originalPriority;

		/* Update event list item value */
		pRunningThread->eventListItem.itemValue =
				pRunningThread->originalPriority;

		/* Added to the ready list */
		RTOS_threadAddToReadyList(pRunningThread);
	} else {
		/* Do nothing */
	}
}
#endif

/**
 * @brief   Sets the priority of the running thread
 * @note    Used for priority ceiling
 * @param   None
 * @retval  None
 */
#if (1 == USE_PRIORITY_SET)
void RTOS_threadPrioritySet(uint32_t newPriority) {

	/* Check input parameters */
	ASSERT_param(THREAD_PRIORITY_LEVELS > newPriority);

	/* Get running thread priority */
	uint32_t runningThreadPriority;

	/* Read priority */
#if (1 == USE_PRIORITY_INHERITANCE)
	{
		runningThreadPriority = pRunningThread->originalPriority;
	}
#else
	  {
	    /* No priority inheritance */
	    runningThreadPriority = pRunningThread->priority;
	  }
	  #endif

	  /* Change the priority */
#if (1==USE_PRIORITY_INHERITANCE )
	  {
		  /* check if the thread inherted the priority */
	      /* No information if the thread inherited the priority */
	      if(pRunningThread->originalPriority == pRunningThread->priority)
	      {
	        /* No inheritance */
	        pRunningThread->priority = newPriority;
	      }
	      else
	      {
	        /* Do nothing */
	      }

	      /* Change the original priority */
	      pRunningThread->originalPriority = newPriority;
	  }

#else
	  {
		  /* No priority inheritance */
		  pRunningThread->priority = newPriority;
	  }
#endif
	/* Compare new priority with the current priority */
	if (newPriority != runningThreadPriority) {

		/* The new priority is lower priority means another thread should work */
		if (newPriority > runningThreadPriority) {

			/* Can make other thread able to run, trigger context switch,
			 * set PendSV to pending */
			SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

		} else {
			/* When increasing the priority, no need to trigger context
			 * switch because the running thread is always the highest priority
			 * ready thread  */
		}



	} else {
		/* Do nothing, it has the same priority */
	}

}
#endif
