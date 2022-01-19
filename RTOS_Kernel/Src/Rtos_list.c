/********************************************************************
 **
 **
 ** file    		Rtos_list.c
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_list.c
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
 * @brief   List end item value
 */
#define LIST_END_ITEM_VALUE                     ((uint32_t) 0xFFFFFFFFu)

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
 * @brief   Initialize thread list
 * @note
 * @param   RTOS_list_t *
 * @retval  None
 */
void RTOS_listInit(RTOS_list_t *pList) {
	/* Check input parameters */
	ASSERT_param(NULL != pList);

	/* Currently no threads in the list, so the index pointing to the end */
	pList->pIndex = (RTOS_listItem_t*) &pList->listEnd;

	/* List is empty, so the end pointing to itself */
	pList->listEnd.pNext = (RTOS_listItem_t*) &pList->listEnd;
	pList->listEnd.pPrev = (RTOS_listItem_t*) &pList->listEnd;

	/* Set item value for the list end */
	pList->listEnd.itemValue = LIST_END_ITEM_VALUE;

	/* Number of threads in the list is zero */
	pList->numOfItems = 0;
}

/**
 * @brief
 * @note  	Our array is more like a stack (Last in first out)
 * @param   List to add to , item to be added
 * @retval  void
 */

void RTOS_listInsertEnd(RTOS_list_t *pList, RTOS_listItem_t *pNewItem) {

	/* put the index Next as the new item next  */
	pNewItem->pNext = pList->pIndex->pNext;

	/* put the index as the new item's previous   */
	pNewItem->pPrev = pList->pIndex;

	/* make the index next item previous pointer points to the new item */
	pList->pIndex->pNext->pPrev = pNewItem;

	/* make the index next pointer points to the new item */
	pList->pIndex->pNext = pNewItem;

	/* make the index of the list the new item */
	pList->pIndex = pNewItem;

	/* Set the list container for the new item */
	pNewItem->pList = (void*) pList;

	/* Increment number of items inside the list */
	pList->numOfItems++;
}

/**
 * @brief
 * @note
 * @param   List which we want to remove from
 * @retval  void
 */
void RTOS_listRemove(RTOS_listItem_t *pRemovedItem) {

	/* create a pointer to acess the list of the removed item */
	RTOS_list_t *pTempList = (RTOS_list_t*) pRemovedItem->pList;

	/* Previous of the next item will be the previous of removed item  */
	pRemovedItem->pNext->pPrev = pRemovedItem->pPrev;

	/* Next of the previous item will be the next of the removed item */
	pRemovedItem->pPrev->pNext = pRemovedItem->pNext;

	/* check if the removed item is the list index */
	if (pTempList->pIndex == pRemovedItem) {

		/* if true make the index the previous of the removed item */
		pTempList->pIndex = pRemovedItem->pPrev;
	} else {
		/* Do nothing its not the index */
	}

	/* remove the removed item from the current list */
	pRemovedItem->pList = NULL;

	/* Decrement the */
	pTempList->numOfItems--;

}

/**
 * @brief   Insert thread item by priority order
 * @note    first item after the endList item is the highest priority with lowest priority number
 * @param   RTOS_list_t *, RTOS_listItem_t *
 * @retval  None
 */
void RTOS_listInsert(RTOS_list_t *pList, RTOS_listItem_t *pNewItem) {

	/* Check input parameters */
	ASSERT_param(NULL != pList);
	ASSERT_param(NULL != pNewItem);

	/*TRY TO USE */

	/* Temp for the insert index */
	RTOS_listItem_t *pInsertIndex = (RTOS_listItem_t*) &pList->listEnd;

	/* Get insert index, find the high item value */
	/* less item value means heigher priority */
	while ((pInsertIndex->pNext != (RTOS_listItem_t*) &pList->listEnd)
			&& (pInsertIndex->pNext->itemValue <= pNewItem->itemValue)) {
		pInsertIndex = pInsertIndex->pNext;
	}

	/* Connect the new item with insert index */
	  pNewItem->pNext = pInsertIndex->pNext;
	  pNewItem->pPrev = pInsertIndex;
	  pInsertIndex->pNext->pPrev = pNewItem;
	  pInsertIndex->pNext = pNewItem;

	  /* Set the list container for the new item */
	  pNewItem->pList = (void *) pList;

	  /* Increment number of items in the list */
	  pList->numOfItems++;
}
