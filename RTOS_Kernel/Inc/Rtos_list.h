/********************************************************************
 **
 **
 ** file    		Rtos_list.h
 ** author  		Ahmed Wasfy
 ** email   		ahmedessam0128@gmail.com
 ** date    		Dec 3, 2021
 ** Target  		STM32F4291-discoveryboard
 ** Version     	1.0.0
 **
 ** brief   		Rtos_list.h
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

#ifndef INC_RTOS_LIST_H_
#define INC_RTOS_LIST_H_

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
 * @brief 	its the item list object that carry the thread and exist in one of /
 * 		    Two lists (Ready ,  blocked)
 */
typedef struct ListItem_t {
	uint32_t itemValue; /**< Item value used for sorting in non ready lists */
	struct ListItem_t *pNext; /* ptr to the next thread in list */
	struct ListItem_t *pPrev; /* ptr to the prev thread in list */
	/*******Question *********/
	void *pThread; /* ptr to the thread of the item */
	void *pList; /* ptr to the list of the item  */
} RTOS_listItem_t;

/**
 * @brief 	item end is the dummy item only in its list and doesn't point to /
 * 			any thread and the last item in any list
 */
typedef struct {
	uint32_t itemValue; /**< Item value used for sorting in non ready lists */
	RTOS_listItem_t *pNext; /* ptr to the next thread in list */
	RTOS_listItem_t *pPrev; /* ptr to the prev thread in list */
} RTOS_listEnd_t;

/**
 * @brief 	its the list blueprint of threads
 */
typedef struct {
	uint32_t numOfItems; /* it's the number of items inside the list without/
	 taking the item end into consideration*/

	RTOS_listItem_t *pIndex; /* its the ptr of the first item in the list 	  /
	 (after sorting either by priority or by time ) */

	RTOS_listEnd_t listEnd; /* it's the last item of the List (list ENd object) */
} RTOS_list_t;

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
 * @brief   Initialize thread list
 * @note
 * @param   RTOS_list_t *
 * @retval  None
 */
void RTOS_listInit(RTOS_list_t *pList);

/**
 * @brief
 * @note
 * @param   List to add to , item to be added
 * @retval  void
 */

void RTOS_listInsertEnd(RTOS_list_t *plist, RTOS_listItem_t *pNewItem);

/**
 * @brief
 * @note
 * @param   List which we want to remove from
 * @retval  void
 */
void RTOS_listRemove(RTOS_listItem_t *pRemovedItem);


/**
 * @brief   Insert thread item by priority order
 * @note
 * @param   RTOS_list_t *, RTOS_listItem_t *
 * @retval  None
 */
void RTOS_listInsert(RTOS_list_t *pList, RTOS_listItem_t *pNewItem);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_RTOS_LIST_H_ */
