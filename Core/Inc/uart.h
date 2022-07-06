/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    uart.h
 * @brief   This file contains all the function prototypes for
 *          the uart.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>  //for va_list var arg functions
#include <string.h>
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
// ADDED
#define HAL_UART_MAX_RCV_DELAY 5000
#define HAL_UART_MAX_TX_DELAY 100
#define HAL_UART_BUFFER_LEN 1

/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);
void string_to_uart(const char *fmt, ...);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
