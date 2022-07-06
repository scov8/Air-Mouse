/*
# This library was developed from a library by Bulanov Konstantin
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

#include "mpu6050.h"

#include <math.h>
#include <uart.h>

#include "ds3231.h"
#include "gpio.h"
#include "i2c.h"
#include "sd.h"
#include "string.h"

MPU6050 mpu6050_1;
MPU6050 mpu6050_2;

double last_time = 0;
double angleGyroX1 = 0;
double angleGyroY1 = 0;
double angleGyroZ1 = 0;
double angleGyroX2 = 0;
double angleGyroY2 = 0;
double angleGyroZ2 = 0;

extern DS3231Time ds3231time;

/**
 *   @brief Read both accelerometers values
 *   @param Ax1 Pointer to accelerometer of device 1 on X axis
 *   @param Ay1 Pointer to accelerometer of device 1 on Y axis
 *   @param Az1 Pointer to accelerometer of device 1 on Z axis
 *   @param Ax2 Pointer to accelerometer of device 2 on X axis
 *   @param Ay2 Pointer to accelerometer of device 2 on Y axis
 *   @param Az2 Pointer to accelerometer of device 3 on Z axis
 */
void MPU6050_read_accel(double* Ax1, double* Ay1, double* Az1, double* Ax2,
                        double* Ay2, double* Az2) {
    uint8_t Rec_Data1[6];
    uint8_t Rec_Data2[6];
    // I read the 6 bytes from the coordinate register
    float divider = 16384.0;  // page 29 datasheet

    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_1, ACCEL_XOUT_H_REG, 1, Rec_Data1, 6,
                     1000);
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_2, ACCEL_XOUT_H_REG, 1, Rec_Data2, 6,
                     1000);

    *Ax1 = ((int16_t)(Rec_Data1[0] << 8 | Rec_Data1[1]) / divider) -
           mpu6050_1.offset_ax;
    *Ay1 = ((int16_t)(Rec_Data1[2] << 8 | Rec_Data1[3]) / divider) -
           mpu6050_1.offset_ay;
    *Az1 = ((int16_t)(Rec_Data1[4] << 8 | Rec_Data1[5]) / divider) -
           mpu6050_1.offset_az;

    *Ax2 = ((int16_t)(Rec_Data2[0] << 8 | Rec_Data2[1]) / divider) -
           mpu6050_2.offset_ax;
    *Ay2 = ((int16_t)(Rec_Data2[2] << 8 | Rec_Data2[3]) / divider) -
           mpu6050_2.offset_ay;
    *Az2 = ((int16_t)(Rec_Data2[4] << 8 | Rec_Data2[5]) / divider) -
           mpu6050_2.offset_az;
}

/**
 *   @brief  Read both gyroscopes values
 *   @param Gx1 Pointer to gyroscope of device 1 on X axis
 *   @param Gy1 Pointer to gyroscope of device 1 on Y axis
 *   @param Gz1 Pointer to gyroscope of device 1 on Z axis
 *   @param Gx2 Pointer to gyroscope of device 2 on X axis
 *   @param Gy2 Pointer to gyroscope of device 2 on Y axis
 *   @param Gz2 Pointer to gyroscope of device 3 on Z axis
 */
void MPU6050_read_gyro(double* Gx1, double* Gy1, double* Gz1, double* Gx2,
                       double* Gy2, double* Gz2) {
    uint8_t Rec_Data3[6];
    uint8_t Rec_Data4[6];
    // I read the 6 bytes from the coordinate register
    float divider = 131;  // page 31 datasheet

    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_1, GYRO_XOUT_H_REG, 1, Rec_Data3, 6,
                     1000);
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_2, GYRO_XOUT_H_REG, 1, Rec_Data4, 6,
                     1000);

    *Gx1 = ((int16_t)(Rec_Data3[0] << 8 | Rec_Data3[1]) / divider) -
           mpu6050_1.offset_gx;
    *Gy1 = ((int16_t)(Rec_Data3[2] << 8 | Rec_Data3[3]) / divider) -
           mpu6050_1.offset_gy;
    *Gz1 = ((int16_t)(Rec_Data3[4] << 8 | Rec_Data3[5]) / divider) -
           mpu6050_1.offset_gz;

    *Gx2 = ((int16_t)(Rec_Data4[0] << 8 | Rec_Data4[1]) / divider) -
           mpu6050_2.offset_gx;
    *Gy2 = ((int16_t)(Rec_Data4[2] << 8 | Rec_Data4[3]) / divider) -
           mpu6050_2.offset_gy;
    *Gz2 = ((int16_t)(Rec_Data4[4] << 8 | Rec_Data4[5]) / divider) -
           mpu6050_2.offset_gz;
}

/**
 *   @brief  Read both gyroscopes and accelerometers value
 */
void MPU6050_read(void) {
    MPU6050_read_accel(&(mpu6050_1.ax), &(mpu6050_1.ay), &(mpu6050_1.az),
                       &(mpu6050_2.ax), &(mpu6050_2.ay), &(mpu6050_2.az));
    MPU6050_read_gyro(&(mpu6050_1.gx), &(mpu6050_1.gy), &(mpu6050_1.gz),
                      &(mpu6050_2.gx), &(mpu6050_2.gy), &(mpu6050_2.gz));
}

/**
 *   @brief send to uart gyroscope and accelerometer values of a single MPU6050
 *   @param mpu6050 the mpu6050 device
 */
void MPU6050_to_uart_single(MPU6050 mpu6050) {
    char goDown[] = "\n \r ";

    char ax[10];
    char ay[10];
    char az[10];

    char anglex[10];
    char angley[10];
    char anglez[10];

    char gx[10];
    char gy[10];
    char gz[10];

    snprintf(ax, 10, "%f", mpu6050.ax);
    snprintf(ay, 10, "%f", mpu6050.ay);
    snprintf(az, 10, "%f", mpu6050.az);

    snprintf(gx, 10, "%f", mpu6050.gx);
    snprintf(gy, 10, "%f", mpu6050.gy);
    snprintf(gz, 10, "%f", mpu6050.gz);

    snprintf(anglex, 10, "%f", mpu6050.angle_x);
    snprintf(angley, 10, "%f", mpu6050.angle_y);
    snprintf(anglez, 10, "%f", mpu6050.angle_z);

    string_to_uart("ax=");
    string_to_uart(ax);
    string_to_uart(" ay=");
    string_to_uart(ay);
    string_to_uart(" az=");
    string_to_uart(az);
    string_to_uart(goDown);

    string_to_uart("gx=");
    string_to_uart(gx);
    string_to_uart(" gy=");
    string_to_uart(gy);
    string_to_uart(" gz=");
    string_to_uart(gz);
    string_to_uart(goDown);

    string_to_uart("anglex=");
    string_to_uart(anglex);
    string_to_uart(" angley=");
    string_to_uart(angley);
    string_to_uart(" anglez=");
    string_to_uart(anglez);
    string_to_uart(goDown);
}

/**
 *  @param  send to uart gyroscopes and accelerometers values of both MPU6050
 */
void MPU6050_to_uart(void) {
    char goDown[] = "\n \r ";

    string_to_uart(goDown);
    time_to_uart();
    string_to_uart(goDown);
    MPU6050_read();
    string_to_uart("ACC1");
    string_to_uart(goDown);
    MPU6050_to_uart_single(mpu6050_1);
    string_to_uart(goDown);
    uint8_t str2[255] = "ACC2";
    HAL_UART_Transmit(&huart2, str2, sizeof(str2), 10);
    string_to_uart(goDown);
    MPU6050_to_uart_single(mpu6050_2);
}

/**
 * @brief Function to set the home position for the gyroscopes and accelerometers
 * sensor
 */
void MPU6050_calibration(void) {
    double x1g = 0;
    double y1g = 0;
    double z1g = 0;

    double x2g = 0;
    double y2g = 0;
    double z2g = 0;

    double x1a = 0;
    double y1a = 0;
    double z1a = 0;

    double x2a = 0;
    double y2a = 0;
    double z2a = 0;

    for (int i = 0; i < 3000; i++) {
        MPU6050_read();
        x1g += mpu6050_1.gx;
        y1g += mpu6050_1.gy;
        z1g += mpu6050_1.gz;
        x2g += mpu6050_2.gx;
        y2g += mpu6050_2.gy;
        z2g += mpu6050_2.gz;
        x1a += mpu6050_1.ax;
        y1a += mpu6050_1.ay;
        z1a += mpu6050_1.az;
        x2a += mpu6050_2.ax;
        y2a += mpu6050_2.ay;
        z2a += mpu6050_2.az;
    }
    mpu6050_1.offset_gx = x1g / 3000;
    mpu6050_1.offset_gy = y1g / 3000;
    mpu6050_1.offset_gz = z1g / 3000;

    mpu6050_2.offset_gx = x2g / 3000;
    mpu6050_2.offset_gy = y2g / 3000;
    mpu6050_2.offset_gz = z2g / 3000;

    mpu6050_1.offset_ax = x1a / 3000;
    mpu6050_1.offset_ay = y1a / 3000;
    mpu6050_1.offset_az = z1a / 3000;

    mpu6050_2.offset_ax = x2a / 3000;
    mpu6050_2.offset_ay = y2a / 3000;
    mpu6050_2.offset_az = z2a / 3000;
}

/**
 * @brief From accelerometers and gyroscopes values calculate the angle on x,y and z
 * axis
 *
 */
void MPU6050_convert(void) {
    MPU6050_read();
    double interval = (HAL_GetTick() - last_time) * 0.001;

    double angleAccX1 = atan2(mpu6050_1.ay, sqrt(mpu6050_1.az * mpu6050_1.az +
                                                 mpu6050_1.ax * mpu6050_1.ax)) *
                        360 / 2.0 / 3.1415926535;
    double angleAccY1 = atan2(mpu6050_1.ax, sqrt(mpu6050_1.az * mpu6050_1.az +
                                                 mpu6050_1.ay * mpu6050_1.ay)) *
                        360 / -2.0 / 3.1415926535;

    double angleAccX2 = atan2(mpu6050_2.ay, sqrt(mpu6050_2.az * mpu6050_2.az +
                                                 mpu6050_2.ax * mpu6050_2.ax)) *
                        360 / 2.0 / 3.1415926535;
    double angleAccY2 = atan2(mpu6050_2.ax, sqrt(mpu6050_2.az * mpu6050_2.az +
                                                 mpu6050_2.ay * mpu6050_2.ay)) *
                        360 / -2.0 / 3.1415926535;

    angleGyroX1 += mpu6050_1.gx * interval;
    angleGyroY1 += mpu6050_1.gy * interval;
    angleGyroZ1 += mpu6050_1.gz * interval;

    mpu6050_1.angle_x =
        (0.98 * (mpu6050_1.angle_x + mpu6050_1.gx)) + (0.02 * angleAccX1);
    mpu6050_1.angle_y =
        (0.98 * (mpu6050_1.angle_y + mpu6050_1.gx)) + (0.02 * angleAccY1);
    mpu6050_1.angle_z = angleGyroZ1;

    angleGyroX2 += mpu6050_2.gx * interval;
    angleGyroY2 += mpu6050_2.gy * interval;
    angleGyroZ2 += mpu6050_2.gz * interval;

    mpu6050_2.angle_x =
        (0.98 * (mpu6050_2.angle_x + mpu6050_2.gx)) + (0.02 * angleAccX2);
    mpu6050_2.angle_y =
        (0.98 * (mpu6050_2.angle_y + mpu6050_2.gy)) + (0.02 * angleAccY2);
    mpu6050_2.angle_z = angleGyroZ2;

    last_time = HAL_GetTick();
}

/**
 *   @brief  Function to initialize the two MPU6050 devices
 */
void MPU6050_init(void) {
    uint8_t check1 = 0;
    uint8_t check2 = 0;
    uint8_t Data;
    char goDown[] = "\n \r ";

    // I check whether the devices are ready to function
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_1, WHO_AM_I_REG, 1, &check1, 1, 100);
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_2, WHO_AM_I_REG, 1, &check2, 1, 100);

    // when the device is available the return value is 104 (0x68)
    while (check1 != 104 || check2 != 104) {
        if (check1 != 104) {
            HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_1, WHO_AM_I_REG, 1, &check1,
                             1, 100);
            string_to_uart("Dev1 ERR");
            string_to_uart(goDown);
        }
        if (check2 != 104) {
            HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_2, WHO_AM_I_REG, 1, &check2,
                             1, 100);
            string_to_uart("Dev2 ERR");
            string_to_uart(goDown);
        }
    }

    if (check1 == 104) {
        string_to_uart("Dev1 READY");
        string_to_uart(goDown);
    }
    if (check2 == 104) {
        string_to_uart("Dev2 READY");
        string_to_uart(goDown);
    }

    Data = 0;
    // I active devices (page 40 datasheet)
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_1, PWR_MGMT_1_REG, 1, &Data, 1,
                      1000);
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_2, PWR_MGMT_1_REG, 1, &Data, 1,
                      1000);

    // I set the sampling frequency (page 12 datasheet)
    Data = 0x07;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_1, SMPLRT_DIV_REG, 1, &Data, 1,
                      1000);
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_2, SMPLRT_DIV_REG, 1, &Data, 1,
                      1000);

    // I set the accelerometer value (page 15 datasheet)
    // precision -/+ 2g
    Data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_1, ACCEL_CONFIG_REG, 1, &Data, 1,
                      1000);
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_2, ACCEL_CONFIG_REG, 1, &Data, 1,
                      1000);

    // I set the gyro value (page 14 datasheet)
    // precision -/+ 250°/s
    Data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_1, GYRO_CONFIG_REG, 1, &Data, 1,
                      1000);
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_2, GYRO_CONFIG_REG, 1, &Data, 1,
                      1000);

    HAL_GPIO_WritePin(GPIOB, ledY_Pin,
                      GPIO_PIN_RESET);  // indica che deve solo calibrare
    MPU6050_calibration();
    string_to_uart("Calibration ok");
    string_to_uart(goDown);
    last_time = HAL_GetTick();
}
