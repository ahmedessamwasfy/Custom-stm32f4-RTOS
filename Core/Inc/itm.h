/*******************************************************************************
 * @file    itm.h
 * @author  Ahmed Eldeep
 * @email   ahmed@almohandes.org
 * @website http://almohandes.org/stm32
 * @date    29.08.2018
 *
 * @brief   Basic functionality of ITM
 * @note
 *
@verbatim
Copyright (C) Almohandes.org, 2018

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
@endverbatim
*******************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __INC_ITM_H_
#define __INC_ITM_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"

/**
 * @addtogroup stm32_examples
 * @{
 */

/**
 * @addtogroup itm
 * @{
 */

/**
 * @defgroup itm_exported_typedefs
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup itm_exported_defines
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup itm_exported_macros
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup itm_exported_constants
 * @{
 */

/**
 * @}
 */

/**
 * @defgroup itm_exported_functions
 * @{
 */
/**
 * @brief   ITM_Printf
 * @note
 * @param   str, length
 * @retval  None
 */
void ITM_Printf(char *str);

/**
 * @brief   SendChar
 * @note
 * @param   port, ch
 * @retval
 */
void ITM_SendChar_Port(uint8_t port, uint8_t ch);

/**
 * @brief   ITM_Printf_Port
 * @note
 * @param   port, str, length
 * @retval  None
 */
void ITM_Printf_Port(uint8_t port, char *str);

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /*__INC_ITM_H_ */
