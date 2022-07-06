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

#include <uart.h>
#include "mouse.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "fatfs.h"


mouseHID mousehid = {0, 0, 0, 0};

extern USBD_HandleTypeDef hUsbDeviceFS;

/**
 * @brief Function to initialize/reset mouse
 */
void mouse_reset(void) {
    mousehid.mouse_x = 0;
    mousehid.mouse_y = 0;
    mousehid.wheel = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
}

/**
 * @brief Function to reset the click button
 */
void mouse_reset_btn(void) {
    mousehid.button = 0;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
}

/**
 * @brief Function to do the left click
 */
void mouse_left_click(void) {
    if (mousehid.button != 1) {
        mousehid.button = 1;
        USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
        osDelay(50);
    }
}

/**
 * @brief Function to do the right click
 */
void mouse_right_click(void) {
    if (mousehid.button != 2) {
        mousehid.button = 2;
        USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
        osDelay(50);
    }
}

/**
 * @brief Function to make the movement of the mouse
 * @param mouse_x Value of movement on x axis
 * @param mouse_y Value of movement on y axis
 */
void mouse_move(int8_t mouse_x, int8_t mouse_y) {
    mousehid.mouse_x = mouse_x;
    mousehid.mouse_y = mouse_y;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
    mouse_reset();
    osDelay(50);
}

/**
 * @brief Function to scroll on a page
 * @param wheel Value of scroll
 */
void mouse_scroll(int8_t wheel) {
    mousehid.wheel = wheel;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
    mouse_reset();
    osDelay(50);
}

/**
 * @brief Function to send middle click
 */
void mouse_macro1(void) {
    mousehid.button = 4;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
    mouse_reset();
    osDelay(50);
}

/**
 * @brief Function to send middle and right click simultaneously
 */
void mouse_macro2(void) {
    mousehid.button = 6;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
    mouse_reset();
    osDelay(50);
}

/**
 * @brief Function to send middle and left click simultaneously
 */
void mouse_macro3(void) {
    mousehid.button = 5;
    USBD_HID_SendReport(&hUsbDeviceFS, &mousehid, sizeof(mousehid));
    osDelay(50);
    mouse_reset();
    osDelay(50);
}
