/********************************************************************
**
**
** file    		Rtos_asm.s
** author  		Ahmed Wasfy
** email   		ahmedessam0128@gmail.com
** date    		Dec 3, 2021
** Target  		STM32F4291-discoveryboard
** Version     	1.0.0
**
** brief   		Rtos_asm.s
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


.syntax unified
.cpu cortex-m4
.thumb


.global RTOS_SVC_Handler
.global RTOS_PendSV_Handler

.text

.type RTOS_SVC_Handler, %function
RTOS_SVC_Handler:
    tst lr, #4                /* Test bit 2 of the link register to find current stack */
    						  /* Anding 4(0100) with the link register knowing that it contains now the EXC_RETURN of the previous
    						     So in last  byte if it uses MSP it would be 9(1001) else if it uses PSP it is D(1101) so
    						     The test is true and zero flag unset if it is PSP , test is false and zero flag set if it's MSP */
    ite eq                    /* eq when zero flag is set */
    						  /* means if its MSP then test will set zero flag then eq flag is set,
    						  	  else if its PSP then test won't set zero flag then eq flag is unset */
    mrseq r0, msp             /* msp when bit 2 is cleared */
    						  /* if eq flag is set then its MSP */
    mrsne r0, psp             /* msp when bit 2 is set */
    						  /* if eq flag is unset then its PSP */
    ldr r1,=svcEXEReturn      /* Load the address of svcEXEReturn in r1 */
    str lr,[r1]               /* Store lr in svcEXEReturn */
    						  /* in previous 2 steps we load address of EXC_return and put it in r1
    						  	 then put the value of link register (which already carry the EXC_RETURN of the function before the exception) inside EXC_RETURN*/
    bl RTOS_SVC_Handler_main  /* Run C part of SVC Handler */

    ldr r1,=svcEXEReturn      /* Load the address of svcEXEReturn in r1 */
    ldr lr,[r1]               /* Load lr with updated svcEXEReturn value */
    						  /* this sequence to return the link register value that we saved in EXC_RETURN to exit the exception normally */
    bx lr                     /* Return */



.type RTOS_PendSV_Handler, %function
RTOS_PendSV_Handler:
    /* ******************** */
    /* Save current context */
    /* ******************** */
    mrs r1, psp               /* Store psp in r1 */
    tst lr, #0x10             /* Test bit 4. if zero, need to stack floating point regs */
    it eq                     /* eq means zero flag is set */
    vstmdbeq r1!, {s16-s31}   /* save floating point registers */
    mov r2, lr                /* Store lr in r2 */
    mrs r3, control           /* Store control in r3 */
    stmdb r1!,{r2-r11}        /* Store multiple registers (r2 to r11).
                                 Decrement address before each access. ! for write back */
                              /* Note we already saved the r2 and r3 automatically but now we just mocking
                               * the r2 and r3 to save the exceptionReturn and control register */
    bl RTOS_threadGetRunning  /* Get current running thread location */
    str r1,[r0]               /* Store the stack pointer for the current thread  */
    						  /* First we get the Current running thread and by default the return of
    						   * get running is saved in r0 which is the address of the struct of current thread
    						   * and the first element in this struct of the thread is the stack pointer
    						   * So we update the stack pointer to the new one after we saved context */
    /* ******************** */
    /* Load next context    */
    /* ******************** */
    mov r0, #1                   /* Put 1 in r0 */
    msr basepri, r0              /* Disable interrupts with priority 1 and greater */
    bl RTOS_threadSwitchRunning  /* Change current running thread */
    mov r0, #0                   /* Put 0 in r0 */
    msr basepri, r0              /* Enable interrupts */
    bl RTOS_threadGetRunning     /* Get current running thread location */
    ldr r1,[r0]                  /* Get its value which is the stack pointer */
    ldmia r1!,{r2-r11}           /* Load multiple registers (r2 to r11).
                                    Increment address after each access. ! for write back */
    mov lr, r2                   /* Load lr with value from r2 */
    msr control, r3              /* Load control with value from r3 */
    isb                          /* Instruction Synchronization Barrier, recommended after control change */
    tst lr, #0x10                /* Test bit 4. If zero, need to unstack floating point regs */
    it eq                        /* eq means zero flag is set */
    vldmiaeq r1!, {s16-s31}      /* Load floating point registers */
    msr psp, r1                  /* set psp to the stack pointer of the new current task */
    bx lr                        /* Return */
