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

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define MPU6050_ADDR_1 0XD0
#define MPU6050_ADDR_2 0XD2
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define GYRO_XOUT_H_REG 0x43
#define TEMP_OUT_H_REG 0x41
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

typedef struct {
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
    double angle_x;
    double angle_y;
    double angle_z;
    double offset_gx;
    double offset_gy;
    double offset_gz;
    double offset_ax;
    double offset_ay;
    double offset_az;

} MPU6050;

void MPU6050_read_accel(double* Ax1, double* Ay1, double* Az1, double* Ax2,
                        double* Ay2, double* Az2);
void MPU6050_read_gyro(double* Gx1, double* Gy1, double* Gz1, double* Gx2,
                       double* Gy2, double* Gz2);
void MPU6050_read(void);
void MPU6050_to_uart_single(MPU6050 mpu6050);
void MPU6050_to_uart(void);
void MPU6050_calibration(void);
void MPU6050_convert(void);
void MPU6050_init(void);

#endif /* INC_MPU6050_H_ */
