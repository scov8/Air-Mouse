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
#include "main.h"

#define C_EXP 10  // costante di espansione
#define C_RED 20  // costante di riduzione

typedef char *TInfo;

struct SArray {
    int length;    // lunghezza array
    int size;      // dimensione allocata (>= length)
    TInfo *items;  // puntatore agli elementi dell'array
};
typedef struct SArray TArray;

struct SQueue {
    int n;  // numero di elementi
    int f;  // indice front
    int b;  // indice back
    TArray a;
};
typedef struct SQueue TQueue;

TArray arrayCreate(int length);
void arrayDestroy(TArray *a);
void arrayResize(TArray *a, int length);
TQueue queueCreate();
void queueDestroy(TQueue *q);
void queueAdd(TQueue *q, TInfo x);
TInfo queueRemove(TQueue *q);
TInfo queueFront(TQueue *q);
int queueIsEmpty(TQueue *q);
