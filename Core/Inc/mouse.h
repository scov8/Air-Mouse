/*
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

#ifndef __MOUSE_H__
#define __MOUSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <uart.h>
#include "mouse.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "fatfs.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

typedef struct {
    uint8_t button;
    int8_t mouse_x;
    int8_t mouse_y;
    int8_t wheel;
} mouseHID;

void mouse_reset(void);
void mouse_reset_btn(void);
void mouse_left_click(void);
void mouse_right_click(void);
void mouse_move(int8_t mouse_x, int8_t mouse_y);
void mouse_scroll(int8_t wheel);
void mouse_macro1(void);
void mouse_macro2(void);
void mouse_macro3(void);

#ifdef __cplusplus
}
#endif

#endif /* __MOUSE_H__ */
