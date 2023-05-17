#pragma once

#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "matrix.h"
#include <stdbool.h>

bool createdProjectionMatrix = false;
Matrix4 projectionMatrix;

void createProjectionMatrix(float fov, float aspectRatio, float NEAR_Z, float FAR_Z) {
  projectionMatrix = getIdentityMatrix4();
  float f = 1.0f / tan( fov * 0.5f );
  m0(projectionMatrix) = f*aspectRatio;
  m5(projectionMatrix) = f;
  m10(projectionMatrix) = - (FAR_Z + NEAR_Z) / (FAR_Z - NEAR_Z);
  m11(projectionMatrix) = - 1.0f; /* this 'plugs' the old z into w */
  m14(projectionMatrix) = - (2.0f*NEAR_Z*FAR_Z) / (FAR_Z - NEAR_Z);
  m15(projectionMatrix) = 0.0f;

  createdProjectionMatrix = true;
}

Vector4 projectVertex(Vector4 vertex) {
  if (!createdProjectionMatrix) {
    printf("ERROR : PROJECTION MATRIX HASN'T BEEN CREATED YET\n");
    exit(-1);
  }
  Vector4 projectedVertex =
      Matrix4_multiplyVector(&projectionMatrix, vertex);

  return projectedVertex;
}
#endif