/*
# This library was developed from a library on this link:
# https://01001000.xyz/2020-08-09-Tutorial-STM32CubeIDE-SD-card/
#
# Course: Embedded Systems 2021/2022
#
# Lecturer: Francesco Moscato	fmoscato@unisa.it
# Lecturer: Vincenzo Carletti	vcarletti@unisa.it
#
# Group:
# Cirillo Benedetto 0622701741  b.cirillo6@studenti.unisa.it
# D'Amato Stefano  	0622701723	s.damato16@studenti.unisa.it
# Scovotto Luigi    0622701702  l.scovotto1@studenti.unisa.it
# Tortora Francesco 0622701700  f.tortora21@studenti.unisa.it
#
# Copyright (C) 2022 - All Rights Reserved
# This file is part of AirMouse.
#
# Requirements: Use “fingers” to reproduce mouse movements to a PC
#
# AirMouse is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# AirMouse is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with AirMouse.  If not, see <http://www.gnu.org/licenses/>.
#
# You can find the complete project on GitHub:
# https://github.com/torx2a/airMouse
*/

#include "sd.h"

#include <uart.h>

#define MSG_SIZE 100

FATFS FatFs;   // Fatfs handle
FIL fil;       // File handle
FRESULT fres;  // Result after operations
BYTE readBuf[MSG_SIZE];

/**
 * @brief Function that initialize the sd (mount the volume)
 */
void sd_init(void) {
    // Open the file system
    fres = f_mount(&FatFs, "", 1);  // 1=mount now
    if (fres != FR_OK) {
        // string_to_uart("\n\r errore mount");
        // HAL_NVIC_SystemReset(); //system reset
        HAL_GPIO_WritePin(
            GPIOB, ledB_Pin,
            GPIO_PIN_SET);  // in caso di errore accendiamo il led blu e rosso
        HAL_GPIO_WritePin(GPIOB, ledR_Pin, GPIO_PIN_SET);
    }
}

/**
 * @brief Function for reading the file "log.txt"
 */
char* sd_read(void) {
    // Let's try to open file "log.txt"
    fres = f_open(&fil, "log.txt", FA_READ);
    if (fres != FR_OK) {
        HAL_GPIO_WritePin(GPIOB, ledR_Pin,
                          GPIO_PIN_SET);  // in caso di errore accendiamo il led
                                          // rosso e giallo
        HAL_GPIO_WritePin(GPIOB, ledY_Pin, GPIO_PIN_SET);
    }

    // We can either use f_read OR f_gets to get data out of files
    // f_gets is a wrapper on f_read that does some string formatting for us
    TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
    if (rres != 0) {
        // string_to_uart("Read string from 'log.txt'");
    } else {
        HAL_GPIO_WritePin(
            GPIOB, ledR_Pin,
            GPIO_PIN_SET);  // in caso di errore accendiamo il led rosso
    }

    // Close the file
    f_close(&fil);

    return readBuf;
}

/**
 * @brief Function for writing the file "log.txt"
 * @param string String to be written on the file
 */
void sd_write(char* string) {
    sd_init();
    fres = f_open(&fil, "log.txt", FA_WRITE | FA_OPEN_APPEND);
    if (fres != FR_OK) {
        // string_to_uart("Errore in open");
        HAL_GPIO_WritePin(GPIOB, ledR_Pin,
                          GPIO_PIN_SET);  // in caso di errore accendiamo il led
                                          // rosso e giallo
        HAL_GPIO_WritePin(GPIOB, ledY_Pin, GPIO_PIN_SET);
    }
    int string_length = strlen(string);
    strncpy((char*)readBuf, string, string_length);
    UINT bytesWrote;
    fres = f_write(&fil, readBuf, string_length, &bytesWrote);
    if (fres != FR_OK) {
        HAL_GPIO_WritePin(GPIOB, ledB_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, ledR_Pin,
                          GPIO_PIN_SET);  // in caso di errore accendiamo il led
                                          // rosso, giallo e rosso
        HAL_GPIO_WritePin(GPIOB, ledY_Pin, GPIO_PIN_SET);
    }
    f_sync(&fil);
    f_close(&fil);
    sd_unmount();
}

/**
 * @brief Function for unmounting the volume
 */
void sd_unmount(void) {
    // De-mount the drive
    f_mount(NULL, "", 0);
}
