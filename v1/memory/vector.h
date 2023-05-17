#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

typedef uint32_t Uint32;

#include "memutils.h"



struct Vector {
  ulong length;
  ulong elementSize;
  void *data;
};

void printVector(struct Vector *vec) {
  printf("Vector : length=%lu, elementSize=%lu, data=%p\n", vec->length,
         vec->elementSize, vec->data);
}

struct Vector Vector_create(ulong elementSize) {
  struct Vector vector;
  vector.elementSize = elementSize;
  vector.length = 0;
  vector.data = NULL;

  return vector;
};

ulong Vector_Size(struct Vector *vec) { return vec->length * vec->elementSize; }

void *Vector_element(struct Vector *vec, ulong index) {
  return (void *)((ulong)vec->data + vec->elementSize * index);
}

void Vector_AllocNewSpaceForElement(struct Vector *vec) {
  if (vec->length == 0) {
    vec->data = malloc(vec->elementSize);
  } else {
    ulong newsize = Vector_Size(vec) + vec->elementSize;
    vec->data = realloc(vec->data, newsize);
  }
  vec->length++;
}

void Vector_Push_back(struct Vector *vec, void *data) {
  Vector_AllocNewSpaceForElement(vec);
  void *address =
      (void *)((ulong)vec->data + Vector_Size(vec) - vec->elementSize);
  memcpy(address, data, vec->elementSize);
}

void Vector_Delete(struct Vector *vec) { free(vec->data); }

typedef struct Vector Vector;

#endif