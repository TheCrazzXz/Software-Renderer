#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>


typedef uint32_t Uint32;

typedef unsigned long ulong ;

struct Vector {
  ulong length;
  ulong elementSize;
  void *data;
  bool deleted;
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
  vector.deleted = false;

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

void Vector_setMemory(struct Vector *vec, void *data, float dataLength, float elementSize, bool freePreviousData) {
  if (freePreviousData) {
    if (vec->data == NULL) {
      printf(
          "<Vector_setMemory> Error : Trying to free a NULL data for vector\n");
      exit(-1);
    }
    free(vec->data);
  }
  vec->deleted = false;
  vec->data = malloc(dataLength * elementSize);
  memcpy(vec->data, data, dataLength * elementSize);
  vec->elementSize = elementSize;
  vec->length = dataLength;
}

struct Vector Vector_copy(struct Vector *vec) {
  struct Vector vector;

  Vector_setMemory(&vector, vec->data, vec->length, vec->elementSize, false);
  return vector;
}

void Vector_clear(struct Vector* vec)
{
  if(vec->length != 0)
  {
    free(vec->data);
    vec->length = 0;
  }
  
  
}

void Vector_Delete(struct Vector *vec)
{ 
  if(!vec->deleted)
  {
    free(vec->data);
    vec->deleted = true;
  }
  else
  {
    printf("Error : <Vector_Delete> Trying to delete an already deleted vector\n");
    exit(-1);
  }
}

typedef struct Vector Vector;

#endif