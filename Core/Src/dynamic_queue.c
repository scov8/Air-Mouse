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
#include <assert.h>
#include <dynamic_queue.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Function to create a dynamic array
 *
 * @param length The initial length of the array
 * @return TArray
 */
TArray arrayCreate(int length) {
    TArray a;
    int size = length + C_EXP;
    a.items = malloc(size * sizeof(TInfo));
    assert(a.items != NULL);  // in case of error ends the program
    a.length = length;
    a.size = size;
    return a;
}

/**
 * @brief Function to destroy the dynamic array
 *
 * @param a pointer to the dynamic array to be deleted
 */
void arrayDestroy(TArray *a) {
    free(a->items);
    a->items = NULL;
    a->length = 0;
    a->size = 0;
}

/**
 * @brief Function that allows to resize the dynamic array
 *
 * @param a The dynamic array
 * @param length The new length of the dynamic array
 */
void arrayResize(TArray *a, int length) {
    if (length > a->size || length < a->size - C_RED) {
        int size = length + C_EXP;
        a->items = realloc(a->items, size * sizeof(TInfo));
        assert(a->items != NULL);  // in case of error ends the program
        a->size = size;
    }
    a->length = length;
}

/**
 * @brief Function to create the dynamic array
 *
 * @return return the queue
 */
TQueue queueCreate() {
    TQueue q;
    q.n = q.f = q.b = 0;
    q.a = arrayCreate(0);
    return q;
}

/**
 * @brief Function for the destruction of the array
 *
 * @param q the pointer of queue
 */
void queueDestroy(TQueue *q) {
    q->n = q->f = q->b = 0;
    arrayDestroy(&q->a);
}

/**
 * @brief Function that allows to add a value in the queue
 *
 * @param q is the pointer of queue
 * @param x is the value that we want add
 */
void queueAdd(TQueue *q, TInfo x) {
    if (q->n == q->a.length) {  // need to resize the array
        int len = q->a.length;
        arrayResize(&q->a, len * 2 + 1);
        if (q->n > 0 && q->f >= q->b) {  // need to move the last elements
            int n = len - q->f;          // number of the elements to move
            for (int i = 1; i <= n; i++)
                q->a.items[q->a.length - i] = q->a.items[len - i];
            q->f = q->a.length - n;  // update the front
        }
    }
    q->a.items[q->b] = x;
    q->b = (q->b + 1) % q->a.length;
    q->n++;
}

/**
 * @brief Function that remove and return the element at the top of the queue
 *
 * @param q The queue
 * @return The top element of the queue
 */
TInfo queueRemove(TQueue *q) {
    TInfo x = q->a.items[q->f];
    q->f = (q->f + 1) % q->a.length;
    q->n--;
    return x;
}

/**
 * @brief Function that return the value on the first position
 *
 * @param q the pointer of the queue
 * @return The top element of the queue
 */
TInfo queueFront(TQueue *q) { return q->a.items[q->f]; }

/**
 * @brief Function that check of the queue is empty or not
 *
 * @param q the pointer of the queue
 * @return 1 if the queue is empty otherwise 0
 */
int queueIsEmpty(TQueue *q) { return q->n == 0; }
