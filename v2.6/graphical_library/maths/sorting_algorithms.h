#pragma once

#ifndef __SORTING_ALGORITHMS__
#define __SORTING_ALGORITHMS__

#include "../memory/memutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec.h"
#include <string.h>

float intervertFloats(float *value1, float *value2) {
  float value_1_save = *value1;
  *value1 = *value2;
  *value2 = value_1_save;
}

/*
        min() : Returns the smallest (min) value of values

*/
float min(float *values, ulong values_length) {
  float candidate = values[0];
  for (ulong i = 1; i < values_length; i++) {
    if (values[i] < candidate) {
      candidate = values[i];
    }
  }
  return candidate;
}

/*
        min() : Returns the smallest (min) value of values

*/
ulong minIdx(float *values, ulong values_length) {
  ulong idx = 0;
  for (ulong i = 1; i < values_length; i++) {
    if (values[i] < values[idx]) {
      idx = i;
    }
  }
  return idx;
}



/*
        mostIdx() : Returns the index of the element which most corresponds to a function

*/
ulong mostIdx(void *values, ulong values_length, ulong elementSize,
              bool (*func)(void *, void *)) {
  ulong idx = 0;
  for (ulong i = 1; i < values_length; i++) {
    if (func((void *)((ulong)values + i * elementSize),
             (void *)((ulong)values + idx * elementSize))) {
      idx = i;
    }
  }
  return idx;
}

/*
        max() : Returns the index of the biggest (max) value of values

*/
float max(float *values, ulong values_length) {
  float candidate = values[0];
  for (ulong i = 1; i < values_length; i++) {
    if (values[i] > candidate) {
      candidate = values[i];
    }
  }
  return candidate;
}
float max2(float v1, float v2)
{
	return (v1 < v2) ? v2 : v1;
}
float min2(float v1, float v2)
{
	return (v1 > v2) ? v2 : v1;
}

/*
        max() : Returns the index of the biggest (max) value of values

*/
ulong maxIdx(float *values, ulong values_length) {
  ulong idx = 0;
  for (ulong i = 1; i < values_length; i++) {
    if (values[i] > values[idx]) {
      idx = i;
    }
  }
  return idx;
}

void sortIncreasing(float *values, ulong values_length) {
  for (ulong i = 0; i < values_length - 1; i++) {
    float *values_afer_i = &(values[i]);
    ulong values_after_i_len = values_length - i - 1;

    ulong b_idx = minIdx(values_afer_i, values_after_i_len);

    if (values_afer_i[b_idx] < values[i]) {
      intervertFloats(&values[i], &values_afer_i[b_idx]);
    }
  }
}

void sortDecreasing(float *values, ulong values_length) {
  for (ulong i = 0; i < values_length - 1; i++) {
    float *values_afer_i = &(values[i]);
    ulong values_after_i_len = values_length - i - 1;

    ulong b_idx = maxIdx(values_afer_i, values_after_i_len);

    if (values_afer_i[b_idx] > values[i]) {
      intervertFloats(&values[i], &values_afer_i[b_idx]);
    }
  }
}

/*
  sorts values with a function : 
  (see mostIdx)
*/

void sortFunc(void *values, ulong values_length, ulong elementSize,
              bool (*func)(void *, void *)) {
  for (ulong i = 0; i < values_length - 1; i++) {
    void *values_afer_i =
        (void *)((ulong)values + i * elementSize + elementSize);

    ulong values_after_i_len = values_length - i - 1 ;


    ulong b_idx = mostIdx(values_afer_i, values_after_i_len, elementSize, func);

    void *candidate = (void *)((ulong)values_afer_i + b_idx * elementSize);

    void *current = (void *)((ulong)values + i * elementSize);

    if (func(candidate, current)) {
      void *candidate_save = malloc(elementSize);
      memcpy(candidate_save, candidate, elementSize);

      memcpy(candidate, current, elementSize);
      memcpy(current, candidate_save, elementSize);

      free(candidate_save);
    }
  }
}

#endif