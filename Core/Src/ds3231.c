/*
# This library was provided by the teachers during the lessons
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

#include "ds3231.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <uart.h>

#include "i2c.h"
#include "sd.h"

DS3231Time ds3231time;

/**
 *   @brief convert normal decimal numbers to binary coded decimal
 *   @param val           decimal numbers
 */
uint8_t decToBcd(int16_t val) {
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

/**
 *   @brief convert binary coded decimal to normal decimal numbers
 *   @param val           binary coded decimal numbers
 */
int16_t bcdToDec(uint8_t val) {
    return (int16_t)((val / 16 * 10) + (val % 16));
}

/**
 *   @brief function to set time
 *   @param sec           seconds
 *   @param min           minutes
 *   @param hour          hours
 *   @param dow           days of week
 *   @param dom           days of month
 *   @param month         month
 *   @param year          year
 */
void set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom,
              uint8_t month, uint8_t year) {
    uint8_t set_time[7];
    set_time[0] = decToBcd(sec);
    set_time[1] = decToBcd(min);
    set_time[2] = decToBcd(hour);
    set_time[3] = decToBcd(dow);
    set_time[4] = decToBcd(dom);
    set_time[5] = decToBcd(month);
    set_time[6] = decToBcd(year);

    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

/**
 *   @brief Function that get the time and save it into a struct
 */
void get_time(void) {
    uint8_t get_time[7];
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
    ds3231time.seconds = bcdToDec(get_time[0]);
    ds3231time.minutes = bcdToDec(get_time[1]);
    ds3231time.hour = bcdToDec(get_time[2]);
    ds3231time.dayofweek = bcdToDec(get_time[3]);
    ds3231time.dayofmonth = bcdToDec(get_time[4]);
    ds3231time.month = bcdToDec(get_time[5]);
    ds3231time.year = bcdToDec(get_time[6]);
}

/**
 *   @brief function to get temperature
 */
float get_temp(void) {
    uint8_t temp[2];
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDRESS, 0x11, 1, temp, 2, 1000);
    return ((temp[0]) + (temp[1] >> 6) / 4.0);
}

/**
 *    @brief function to convert the temperature
 */
void force_temp_conv(void) {
    uint8_t status = 0;
    uint8_t control = 0;
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDRESS, 0x0F, 1, &status, 1,
                     100);  // read status register
    if (!(status & 0x04))   // if the BSY bit is not set
    {
        HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDRESS, 0x0E, 1, &control, 1,
                         100);  // read control register
        HAL_I2C_Mem_Write(
            &hi2c2, DS3231_ADDRESS, 0x0E, 1, (uint8_t*)(control | (0x20)), 1,
            100);  // write modified control register with CONV bit as'1'
    }
}

/**
 *    @brief Send current time to uart
 */
void time_to_uart(void) {
    get_time();
    char buffer[12] = {'\0', '\0', '\0', '\0', '\0', '\0',
                       '\0', '\0', '\0', '\0', '\0', '\0'};
    sprintf(buffer, "%02d:%02d:%02d", ds3231time.hour, ds3231time.minutes,
            ds3231time.seconds);
    uint8_t arr[255];

    memmove(arr, buffer, 255);
    HAL_UART_Transmit(&huart2, arr, sizeof(arr), 10);
}

/**
 * @brief Update the string buffer with the current time
 * @param buffer   String to be modified
 */
void time_to_string(char* buffer) {
    get_time();
    sprintf(buffer, "%02d-%02d-%02d %02d:%02d:%02d\n", ds3231time.dayofmonth,
            ds3231time.month, ds3231time.year, ds3231time.hour,
            ds3231time.minutes, ds3231time.seconds);
}
