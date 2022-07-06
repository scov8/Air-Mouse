/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include <dynamic_queue.h>
#include "main.h"

#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdarg.h>  //for va_list var arg functions
#include <stdio.h>
#include <string.h>

#include "ds3231.h"
#include "gpio.h"
#include "i2c.h"
#include "mouse.h"
#include "mpu6050.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SD_SPI_HANDLE hspi1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
SPI_HandleTypeDef hspi1;
extern MPU6050 mpu6050_1;
extern MPU6050 mpu6050_2;
int left = 0;
char buffer[25];
char stamp[255];
char stamp1[255];
char moveString[255];
TQueue q;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;

/* Definitions for moveMouse */
osThreadId_t moveMouseHandle;
const osThreadAttr_t moveMouse_attributes = {
    .name = "moveMouse",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for rightLeftAlt */
osThreadId_t rightLeftAltHandle;
const osThreadAttr_t rightLeftAlt_attributes = {
    .name = "rightLeftAlt",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for zoom */
osThreadId_t zoomHandle;
const osThreadAttr_t zoom_attributes = {
    .name = "zoom",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for sdWrite */
osThreadId_t sdWriteHandle;
const osThreadAttr_t sdWrite_attributes = {
    .name = "sdWrite",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for resetMouse */
osThreadId_t resetMouseHandle;
const osThreadAttr_t resetMouse_attributes = {
    .name = "resetMouse",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* Definitions for readAll */
osThreadId_t readAllHandle;
const osThreadAttr_t readAll_attributes = {
    .name = "readAll",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI1_Init(void);
void move(void *argument);
void right_left_alt(void *argument);
void zoom_mouse(void *argument);
void sd(void *argument);
void reset_mouse(void *argument);
void read_all(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief  This function save the data to be written on sd into a queue
 * @param event   String which represent the event type
 */
void MPU6050_to_sd(char *event) {
    time_to_string(buffer);
    queueAdd(&q, buffer);

    queueAdd(&q, event);

    snprintf(stamp, 255, "accel1: (%.3f,%.3f,%.3f);gyro1: (%.3f,%.3f,%.3f); ",
             mpu6050_1.ax, mpu6050_1.ay, mpu6050_1.az, mpu6050_1.gx,
             mpu6050_1.gy, mpu6050_1.gz);

    queueAdd(&q, stamp);

    snprintf(stamp1, 255,
             "accel2: (%.3f,%.3f,%.3f);gyro2: (%.3f,%.3f,%.3f); \n",
             mpu6050_2.ax, mpu6050_2.ay, mpu6050_2.az, mpu6050_2.gx,
             mpu6050_2.gy, mpu6050_2.gz);

    queueAdd(&q, stamp1);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_FATFS_Init();
    MX_USART2_UART_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    /* USER CODE BEGIN 2 */
    // set_time(0, 44, 18, 7, 2, 7, 22);
    HAL_GPIO_WritePin(GPIOB, ledY_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ledR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, ledB_Pin, GPIO_PIN_SET);
    sd_init();
    HAL_GPIO_WritePin(GPIOB, ledB_Pin, GPIO_PIN_RESET);
    sd_unmount();
    MPU6050_init();
    HAL_GPIO_WritePin(GPIOB, ledR_Pin, GPIO_PIN_RESET);
    sd_write("\n\n\n\n******** START LOG ********\n");
    q = queueCreate();

    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of moveMouse */
    moveMouseHandle = osThreadNew(move, NULL, &moveMouse_attributes);

    /* creation of rightLeftAlt */
    rightLeftAltHandle =
        osThreadNew(right_left_alt, NULL, &rightLeftAlt_attributes);

    /* creation of zoom */
    zoomHandle = osThreadNew(zoom_mouse, NULL, &zoom_attributes);

    /* creation of sdWrite */
    sdWriteHandle = osThreadNew(sd, NULL, &sdWrite_attributes);

    /* creation of resetMouse */
    resetMouseHandle = osThreadNew(reset_mouse, NULL, &resetMouse_attributes);

    /* creation of readAll */
    readAllHandle = osThreadNew(read_all, NULL, &readAll_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }

    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_move */
/**
 * @brief  Function to manage movement of the mouse
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_move */
void move(void *argument) {
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        if ((mpu6050_1.ax > 0.10 || mpu6050_1.ax < -0.10) &&
            (mpu6050_1.ay > 0.10 || mpu6050_1.ay < -0.10)) {
            mouse_move(mpu6050_1.ay * 30, -mpu6050_1.ax * 60);
            // string_to_uart("move");
            // string_to_uart("\n \r");
            snprintf(moveString, 255, "MOVE TO: (%.2f, %.2f) - ",
                     mpu6050_1.ay * 30, -mpu6050_1.ax * 60);
            MPU6050_to_sd(moveString);
        } else if (mpu6050_1.ax > 0.10 || mpu6050_1.ax < -0.10) {
            mouse_move(0, -mpu6050_1.ax * 60);
            // string_to_uart("move");
            // string_to_uart("\n \r");
            snprintf(moveString, 255, "MOVE TO: (%.2f, %.2f) - ",
                     mpu6050_1.ay * 30, -mpu6050_1.ax * 60);
            MPU6050_to_sd(moveString);
        } else if (mpu6050_1.ay > 0.10 || mpu6050_1.ay < -0.10) {
            mouse_move(mpu6050_1.ay * 30, 0);
            // string_to_uart("move");
            // string_to_uart("\n \r");
            snprintf(moveString, 255, "MOVE TO: (%.2f, %.2f) - ",
                     mpu6050_1.ay * 30, -mpu6050_1.ax * 60);
            MPU6050_to_sd(moveString);
        }
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_right_left_alt */
/**
 * @brief Function to manage left click, right click and alt-tab
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_right_left_alt */
void right_left_alt(void *argument) {
    /* USER CODE BEGIN right_left_alt */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        if (mpu6050_2.ax > 0.5) {
            mouse_right_click();
            // string_to_uart("right");
            // string_to_uart("\n \r");
            MPU6050_to_sd("RIGHT CLICK - ");
            while (mpu6050_2.ax > 0.5) {
            }
        }
        if (mpu6050_2.ax < -0.5) {
            mouse_left_click();
            // string_to_uart("left");
            // string_to_uart("\n \r");
            MPU6050_to_sd("LEFT CLICK - ");
            while (mpu6050_2.ax < -0.5) {
            }
            left++;
        }
        if (mpu6050_1.angle_y > 80 && mpu6050_2.angle_y > 80 &&
            mpu6050_2.gx > 80 && mpu6050_1.gx > 80) {
            mouse_macro1();
            // string_to_uart("alt-tab");
            // string_to_uart("\n \r");
            MPU6050_to_sd("ALT-TAB - ");
        }
    }
    /* USER CODE END right_left_alt */
}

/* USER CODE BEGIN Header_zoom_mouse */
/**
 * @brief Function to manage zoom-in and zoom-out
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_zoom_mouse */
void zoom_mouse(void *argument) {
    /* USER CODE BEGIN zoom_mouse */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        if (left > 0) {
            osDelay(1500);
            if (left == 4) {  // zoom-out
                mouse_macro3();
                // string_to_uart("zoom-");
                // string_to_uart("\n \r");
                MPU6050_to_sd("ZOOM-OUT - ");
            }
            if (left == 3) {  // zoom-in
                mouse_macro2();
                // string_to_uart("zoom+");
                // string_to_uart("\n \r");
                MPU6050_to_sd("ZOOM-IN - ");
            }
            left = 0;
        }
    }
    /* USER CODE END zoom_mouse */
}

/* USER CODE BEGIN Header_sd */
/**
 * @brief Function for writing on sd card from a queue
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_sd */
void sd(void *argument) {
    /* USER CODE BEGIN sd */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        while (!queueIsEmpty(&q)) {
            sd_write(queueRemove(&q));
        }
    }
    /* USER CODE END sd */
}

/* USER CODE BEGIN Header_reset_mouse */
/**
 * @brief Function to reset mouse click
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_reset_mouse */
void reset_mouse(void *argument) {
    /* USER CODE BEGIN reset_mouse */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        if (mpu6050_2.ax < 0.5 && mpu6050_2.ax > -0.5) mouse_reset_btn();
    }
    /* USER CODE END reset_mouse */
}

/* USER CODE BEGIN Header_read_all */
/**
 * @brief Function to read current accelerometers and gyros values
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_read_all */
void read_all(void *argument) {
    /* USER CODE BEGIN read_all */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
        MPU6050_convert();
    }
    /* USER CODE END read_all */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM5 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM5) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
