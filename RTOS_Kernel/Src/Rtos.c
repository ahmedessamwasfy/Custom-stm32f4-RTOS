/********************************************************************
 **
 **
 ** file    		Rtos.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos.c
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

#include "../Inc/Rtos.h"

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
 * @brief   EXC_RETURN value used to return from SVC interrupts
 */
uint32_t svcEXEReturn;

/**
 * brief  
 */

/* ==================================================================== */
/* ========================== private data ============================ */
/* ==================================================================== */

/* Definition of private datatypes go here */

/**
 * @brief   Flag used to indicate the scheduler status
 */
static uint32_t schedulerRunning = 0;

/**
 * @brief   Thread and stack object for idle thread
 */
static RTOS_thread_t idleThread;
static RTOS_stack_t idleThreadStack;

/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */
static void idleThreadFunction(void);

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
static void idleThreadFunction(void) {
	while (1) {
		/*we can use it in logging or cleaning*/
	}
}

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
void RTOS_init(void) {

	/* Disable interrupts */
	__disable_irq();

	/* Enable double word stack alignment */
	/* because our stack in the threads are 64bit which is double word  */
	SCB->CCR |= SCB_CCR_STKALIGN_Msk;

	/* Configure and enable SysTick interrupts */
	ASSERT_param(0 == SysTick_Config(SYSTEM_CORE_CLOCK_HZ / SYS_TICK_RATE_HZ));

	/* Set priority group to 3
	 * bits[3:0] are the sub-priority,
	 * bits[7:4] are the preempt priority (0-15) */
	NVIC_SetPriorityGrouping(3);

	/* Set priority levels */
	/* SVCall can't be interrupted by any mean */
	NVIC_SetPriority(SVCall_IRQn, 0);
	NVIC_SetPriority(SysTick_IRQn, 1);

	/* Set PendSV to lowest possible priority */
	/* responsible for the context switching */
	NVIC_SetPriority(PendSV_IRQn, 0xFF);

	/* Enable SVC and PendSV interrupts */
	NVIC_EnableIRQ(PendSV_IRQn);
	NVIC_EnableIRQ(SVCall_IRQn);

	/* Disable all interrupts except SVC */
	__set_BASEPRI(1);

	/* Initialize thread module */
	RTOS_threadInitLists();

	/* Enable interrupts */
	__enable_irq();

}

/**
 * @brief   SVC handler main
 * @note	SVC_args are arguments given in the function so in SVC_Create thread
 * 			svc_args[0]=  pThread ,svc_args[1]=  pStack ,svc_args[2]=  priority ,svc_args[3]=  pFunction,
 * 			and svc_args[4]= ,svc_args[5]= ,svc_args[6]= the Stacked program counter
 * @param   uint32_t *
 * @retval  None
 */
void RTOS_SVC_Handler_main(uint32_t *svc_args) {

	/*
	 * Stack contains:
	 * r0, r1, r2, r3, r12, r14(LR), the return address and xPSR
	 * First argument (r0) is svc_args[0]
	 * Stacked R0 	 = svc_args[0]  (the First parameter in the SVC call)  (R0)
	 * Stacked R1 	 = svc_args[1]  (the Second parameter in the SVC call) (R1)
	 * Stacked R2 	 = svc_args[2]  (the Third parameter in the SVC call)  (R2)
	 * Stacked R3 	 = svc_args[3]  (the Fourth parameter in the SVC call) (R3)
	 * Stacked R12   = svc_args[4]  (the Fifth parameter in the SVC call)  (R12)
	 * Stacked LR 	 = svc_args[5]  (R14) (Link register)
	 * Stacked PC 	 = svc_args[6]  (PC) (Program counter)
	 * Stacked xPSR  = svc_args[7]	(R14) (Link register)
	 */

	/* Check input parameters */
	ASSERT_param(NULL != svc_args);

	uint8_t svc_number;

	/* Memory[(Stacked PC)-2] */
	/* program counter has been stacked when we entered the exception handler of the SVC
	 * So thought getting the program counter that has been stacked and get the 2 previous instruction we can extract the parameter
	 * of the SVC call trigger   */
	/*extracted from the instruction code*/
	svc_number = ((char*) svc_args[6])[-2];

	/* Temp variables */
	/* return value can be 0-> failed access,no block
	 * 					   1-> access granted
	 * 					   2-> failed access,no block */
	RTOS_return_t returnStatus;

	/* EnablePrivilegedMode */
	//__set_CONTROL(__get_CONTROL() & ~CONTROL_nPRIV_Msk);
	/* Check svc number */
	switch (svc_number) {

	case 0: //Scheduler start
		RTOS_schedulerStart();
		break;

	case 1: //thread create
		RTOS_threadCreate((RTOS_thread_t*) svc_args[0],
				(RTOS_stack_t*) svc_args[1], (uint32_t) svc_args[2],
				(void*) svc_args[3]);
		break;

	case 2:	//mutex Create
		/* svc_args[0] is the RTOS_mutex_t * & svc_args[1] is the initial value   */
		RTOS_mutexCreate((RTOS_mutex_t*) svc_args[0], (uint32_t) svc_args[1]);
		break;

	case 3:	//mutex Lock
		/* svc_args[0] is the RTOS_mutex_t * & svc_args[1] is the waiting flag   */
		returnStatus = RTOS_mutexLock((RTOS_mutex_t*) svc_args[0],
				(uint32_t) svc_args[1]);
		break;

	case 4:	//mutex Release
		/* svc_args[0] is the RTOS_mutex_t *  */
		RTOS_mutexRelease((RTOS_mutex_t*) svc_args[0]);

		break;

	case 5:
		RTOS_semaphoreCreate((RTOS_semaphore_t*) svc_args[0],
				(uint32_t) svc_args[1]);
		break;

	case 6:
		returnStatus = RTOS_semaphoreTake((RTOS_semaphore_t*) svc_args[0],
				(uint32_t) svc_args[1]);
		break;

	case 7:
		RTOS_semaphoreGive((RTOS_semaphore_t*) svc_args[0]);
		break;

	case 8:
		RTOS_mailboxCreate((RTOS_mailbox_t*) svc_args[0], (void*) svc_args[1],
				(uint32_t) svc_args[2], (uint32_t) svc_args[3]);
		break;

	case 9:
		returnStatus = RTOS_mailboxWrite((RTOS_mailbox_t*) svc_args[0],
				(uint32_t) svc_args[1], (const void* const ) svc_args[2]);
		break;

	case 10:
		returnStatus = RTOS_mailboxRead((RTOS_mailbox_t*) svc_args[0],
				(uint32_t) svc_args[1], (void* const ) svc_args[2]);
		break;

	case 11:
		RTOS_threadAddRunningToTimerList((uint32_t) svc_args[0]);
		break;

	case 12:
		RTOS_threadDestroy((RTOS_thread_t*) svc_args[0]);
		break;

	case 13:
		#if (1 == USE_PRIORITY_SET)
        RTOS_threadPrioritySet((uint32_t) svc_args[0]);
      #endif
		break;

	default:
		/* Not supported svc call */
		ASSERT_param(0);
		break;
	}

	switch (svc_number) {

	case 3:  //mutex		lock
	case 6:  //semaphore 	lock
	case 9:  //mailbox 		write
	case 10: //mailbox 		read
		/* Check return status */
		if (RTOS_CONTEXT_SWITCH_TRIGGERED == returnStatus) //we blocked the thread
				{
			/* Context switch was triggered, update program counter,
			 * when the context is restored the thread will try again */
			/** svc_args[6] is the stacked program counter and now we are in SVC call interrupt
			 * so we modify the stack pointer after we put the thread in waiting list
			 * because when he will return to the ready list and start running
			 **/
			/* note here we still in the context and TCB of the thread that blocked because
			 * it couldn't take the shared recourse */
			/* note here we decrease the program counter by 2bytes because we are using thumb if we
			 * are using arm instruction we should decrease 4bytes  */
			svc_args[6] = svc_args[6] - 2;
		} else //we didn't block the thread
		{
			/* No context switch was triggered, pass return value */

			svc_args[0] = returnStatus;
		}
		break;

	default:
		/* Handled above */
		break;
	}

}

/**
 * @brief   Start RTOS scheduler
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_schedulerStart(void) {
	/* create idle thread */
	RTOS_threadCreate(&idleThread, &idleThreadStack,
			(THREAD_PRIORITY_LEVELS - 1), idleThreadFunction);

	/* create pointer for the running thread */
	RTOS_thread_t *pRunningThread;

	/* update the running thread */
	RTOS_threadSwitchRunning();

	/* Get running thread */
	pRunningThread = RTOS_threadGetRunning();

	/* Set SVC interrupt return to the first thread */
	/* it put the stack pointer to the first register
	 * that being stack and responsible for the context switching
	 * which is the EXC_Return  */
	/* we put its value not its address */
	/* this value is passed to the linker register to tell him we start to work in thread mode */
	svcEXEReturn = MEM32_ADDRESS(pRunningThread->pStackPointer);

	/* Return to thread with PSP */
	/*Set the PSP register.
	 * The function sets the Process Stack Pointer (PSP) value using the instruction MSR.
	 * */
	/* make the process stack pointer to points on the R0 register because it's the
	 * last thing that the processor stacked before serve the exception so we make the
	 * Process Stack pointer points to the things that will be unstacked after returning */
	__set_PSP((uint32_t) (pRunningThread->pStackPointer + 10 * 4));

	/* Switch to use Process Stack, unprivileged state */
	__set_CONTROL(MEM32_ADDRESS((pRunningThread->pStackPointer + (1 << 2))));

	/* Execute ISB after changing CONTROL */
	/*Instruction Synchronization Barrier flushes the pipeline in the processor,
	 * so that all instructions following the ISB are fetched from cache or memory,
	 * after the instruction has been completed*/
	/*It ensures that the effects of context altering operations,
	 * executed before the ISB instruction are visible to the instructions
	 * fetched after the ISB.
	 * */
	__ISB();

	/* Flag scheduler is running */
	schedulerRunning = 1;

	/* Enable all interrupts */
	__set_BASEPRI(0);

}

/**
 * @brief   Returns scheduler running status
 * @note
 * @param   None
 * @retval  None
 */
uint32_t RTOS_isSchedulerRunning(void) {
	return schedulerRunning;
}

/**
 * @brief   SysTick Handler
 * @note
 * @param   None
 * @retval  None
 */
void RTOS_SysTick_Handler(void) {
	/* Trigger context switch, set PendSV to pending */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	/* Refresh the timer list */
	RTOS_threadRefreshTimerList();
}

/**
 * brief
 * note
 * param   void
 * retval  void
 */

