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

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#define DS3231_ADDRESS 0xD0

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "i2c.h"
#include <uart.h>

typedef struct {
    int16_t seconds;
    int16_t minutes;
    int16_t hour;
    int16_t dayofweek;
    int16_t dayofmonth;
    int16_t month;
    int16_t year;
} DS3231Time;

uint8_t decToBcd(int16_t val);
int16_t bcdToDec(uint8_t val);
void set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom,
              uint8_t month, uint8_t year);
void get_time(void);
float get_temp(void);
void force_temp_conv(void);
void time_to_uart(void);
void time_to_string(char* buffer);

#endif /* INC_DS3231_H_ */
