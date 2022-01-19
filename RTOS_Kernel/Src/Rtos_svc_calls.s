/********************************************************************
**
**
** file    		Rtos_svc_calls.s
** author  		Ahmed Wasfy
** email   		ahmedessam0128@gmail.com
** date    		Dec 3, 2021
** Target  		STM32F4291-discoveryboard
** Version     	1.0.0
**
** brief   		Rtos_svc_calls.s
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



/* directives used for linker */
/**unified syntex
  *cpu cortex m4
  *soft floating point
  *thumb instructions
*/
.syntax unified
.cpu cortex-m4
.thumb

/* global directive, it is used for the linker to make
   global in all files and can be called in .c files */
.global RTOS_SVC_schedulerStart
.global RTOS_SVC_threadCreate

.global RTOS_SVC_mutexCreate
.global RTOS_SVC_mutexLock
.global RTOS_SVC_mutexRelease

.global RTOS_SVC_semaphoreCreate
.global RTOS_SVC_semaphoreTake
.global RTOS_SVC_semaphoreGive

.global RTOS_SVC_mailboxCreate
.global RTOS_SVC_mailboxWrite
.global RTOS_SVC_mailboxRead

.global RTOS_SVC_threadDelay
.global RTOS_SVC_threadDestroy

.global RTOS_SVC_threadPrioritySet

/*  switch to the text section / code segment */
.text

/* define RTOS_SVC_schedulerStart as a function  */
.type RTOS_SVC_schedulerStart, %function
RTOS_SVC_schedulerStart:
    svc 0   				/* trigger Supervisor call and pass 0 for it */
    /* between these two instruction  the procesor went to the svc INTERRUPT handler */
    /* take the Special registers and put them into the stack along with the SVC parameter*/
    bx lr					/* link register carry the program counter of the next instruction after returning from the function so here we jump back from
    							from this function  */

.type RTOS_SVC_threadCreate, %function
RTOS_SVC_threadCreate:
    svc 1
    bx lr

.type RTOS_SVC_mutexCreate, %function
RTOS_SVC_mutexCreate:
    svc 2
    bx lr

.type RTOS_SVC_mutexLock, %function
RTOS_SVC_mutexLock:
    svc 3
    bx lr

.type RTOS_SVC_mutexRelease, %function
RTOS_SVC_mutexRelease:
    svc 4
    bx lr

.type RTOS_SVC_semaphoreCreate, %function
RTOS_SVC_semaphoreCreate:
    svc 5
    bx lr

.type RTOS_SVC_semaphoreTake, %function
RTOS_SVC_semaphoreTake:
    svc 6
    bx lr

.type RTOS_SVC_semaphoreGive, %function
RTOS_SVC_semaphoreGive:
    svc 7
    bx lr

.type RTOS_SVC_mailboxCreate, %function
RTOS_SVC_mailboxCreate:
    svc 8
    bx lr

.type RTOS_SVC_mailboxWrite, %function
RTOS_SVC_mailboxWrite:
    svc 9
    bx lr

.type RTOS_SVC_mailboxRead, %function
RTOS_SVC_mailboxRead:
    svc 10
    bx lr

.type RTOS_SVC_threadDelay, %function
RTOS_SVC_threadDelay:
	svc 11
	bx lr

.type RTOS_SVC_threadDestroy, %function
RTOS_SVC_threadDestroy:
	svc 12
	bx lr

.type RTOS_SVC_threadPrioritySet, %function
RTOS_SVC_threadPrioritySet:
    svc 13
    bx lr


