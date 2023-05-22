#pragma once

#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "matrix.h"
#include <stdbool.h>

bool createdProjectionMatrix = false;
Matrix projectionMatrix;

void createProjectionMatrix() {
  projectionMatrix = Matrix_create(3, 2);

  *Matrix_element(&projectionMatrix, 0, 0) = 1;
  *Matrix_element(&projectionMatrix, 1, 0) = 0;
  *Matrix_element(&projectionMatrix, 2, 0) = 0;

  *Matrix_element(&projectionMatrix, 0, 1) = 0;
  *Matrix_element(&projectionMatrix, 1, 1) = 1;
  *Matrix_element(&projectionMatrix, 2, 1) = 0;

  createdProjectionMatrix = true;
}

Vector2 projectVertex(Vector3 vertex3) {
  if (!createdProjectionMatrix) {
    printf("ERROR : PROJECTION MATRIX HASN'T BEEN CREATED YET\n");
    exit(-1);
  }
  Matrix vertexMatrix = vector3toMatrix(vertex3);
  Matrix projectedVertexMatrix =
      Matrix_multiply(&projectionMatrix, &vertexMatrix);

  Vector2 projectedVertex =
      Vec2_create(*Matrix_element(&projectedVertexMatrix, 0, 0),
                  *Matrix_element(&projectedVertexMatrix, 0, 1));

  Matrix_delete(&projectedVertexMatrix);

  return projectedVertex;
}

void deleteProjectionMatrix() { Matrix_delete(&projectionMatrix); }

#endif