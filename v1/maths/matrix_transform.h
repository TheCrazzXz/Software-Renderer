#pragma once

#ifndef __MATRIX_TRANSFORM_H__
#define __MATRIX_TRANSFORM_H__

#include "matrix4.h"
#include <math.h> 

/*
	getTranslationMatrix4 : Get the 4x4 translation matrix by vector

*/

struct Matrix4 getTranslationMatrix4(struct Vec3 vector)
{
	Matrix4 translationMatrix4 = getIdentityMatrix4();
	*Matrix4_element(&translationMatrix4, 3, 0) = vector.x;
	*Matrix4_element(&translationMatrix4, 3, 1) = vector.y;
	*Matrix4_element(&translationMatrix4, 3, 2) = vector.z;
	return translationMatrix4;
}

/*
	Matrix4_translate : Translates a 4x4 matrix

	Notice : It should be an identity matrix or a transformed identity matrix

*/

struct Matrix4 Matrix4_translate(struct Matrix4* matrix, struct Vec3 vector)
{
	// Allocate a 4x4 matrix
	Matrix4 translationMatrix = getTranslationMatrix4(vector);
	Matrix4 result = Matrix4_multiply(&translationMatrix, matrix);
	return result;

}

/*
	getScaleMatrix4 : Get the 4x4 scale matrix by vector

*/

struct Matrix4 getScaleMatrix4(struct Vec3 vector)
{
	Matrix4 scaleMatrix4 = getIdentityMatrix4();
	*Matrix4_element(&scaleMatrix4, 0, 0) = vector.x;
	*Matrix4_element(&scaleMatrix4, 1, 1) = vector.y;
	*Matrix4_element(&scaleMatrix4, 2, 2) = vector.z;
	return scaleMatrix4;
}

/*
	Matrix4_scale : Scales a 4x4 matrix

	Notice : It should be an identity matrix or a transformed identity matrix

*/

struct Matrix4 Matrix4_scale(struct Matrix4* matrix, struct Vec3 vector)
{
	// Allocate a 4x4 matrix
	Matrix4 scaleMatrix = getScaleMatrix4(vector);
	Matrix4 result = Matrix4_multiply(&scaleMatrix, matrix);
	return result;


}
/*

	getRotationXMatrix4 : Returns rotation around the X axis (concerned axis are then Y and Z) by angle in randians

*/
struct Matrix4 getRotationXMatrix4(float angle)
{
	Matrix4 rotationXMatrix4 = getIdentityMatrix4();
	*Matrix4_element(&rotationXMatrix4, 1, 1) = cos(angle);
	*Matrix4_element(&rotationXMatrix4, 1, 2) = sin(angle);
	*Matrix4_element(&rotationXMatrix4, 2, 1) = sin(-angle);
	*Matrix4_element(&rotationXMatrix4, 2, 2) = cos(angle);
	return rotationXMatrix4;
}

/*

	getRotationYMatrix4 : Returns rotation around the Y axis (concerned axis are then X and Z) by angle in randians

*/
struct Matrix4 getRotationYMatrix4(float angle)
{
	Matrix4 rotationYMatrix4 = getIdentityMatrix4();
	*Matrix4_element(&rotationYMatrix4, 0, 0) = cos(angle);
	*Matrix4_element(&rotationYMatrix4, 0, 2) = sin(-angle);
	*Matrix4_element(&rotationYMatrix4, 2, 0) = sin(angle);
	*Matrix4_element(&rotationYMatrix4, 2, 2) = cos(angle);
	return rotationYMatrix4;
}

/*

	getRotationZMatrix4 : Returns rotation around the Z axis (concerned axis are then X and Y) by angle in randians

*/
struct Matrix4 getRotationZMatrix4(float angle)
{
	Matrix4 rotationZMatrix4 = getIdentityMatrix4();
	*Matrix4_element(&rotationZMatrix4, 0, 0) = cos(angle);
	*Matrix4_element(&rotationZMatrix4, 0, 1) = sin(angle);
	*Matrix4_element(&rotationZMatrix4, 1, 0) = sin(-angle);
	*Matrix4_element(&rotationZMatrix4, 1, 1) = cos(angle);
	return rotationZMatrix4;
}

/*
	Matrix4_rotate : Rotates a 4x4 matrix by angle in radians
	The axis vector corresponds the axis in which it should be transformed :

	Notice : It should be an identity matrix or a transformed identity matrix

*/

struct Matrix4 Matrix4_rotate(struct Matrix4* matrix, struct Vec3 axisVector, float angle)
{
	// Allocate a 4x4 matrix
	Matrix4 rotationMatrix = getIdentityMatrix4();

	if(axisVector.x == 1) // Rotate matrix around the x axis
	{
		Matrix4 rotationXMatrix = getRotationXMatrix4(angle);
		rotationMatrix = Matrix4_multiply(&rotationXMatrix, &rotationMatrix);
	}
	if(axisVector.y == 1) // Rotate matrix around the y axis
	{
		Matrix4 rotationYMatrix = getRotationYMatrix4(angle);
		rotationMatrix = Matrix4_multiply(&rotationYMatrix, &rotationMatrix);
	}
	if(axisVector.z == 1) // Rotate matrix around the z axis
	{
		Matrix4 rotationZMatrix = getRotationZMatrix4(angle);
		rotationMatrix = Matrix4_multiply(&rotationZMatrix, &rotationMatrix);
	}


	Matrix4 result = Matrix4_multiply(&rotationMatrix, matrix);
	return result;


}


#endif