#pragma once

#ifndef __MATHS_H__
#define __MATHS_H__

#include "vec.h"
#include "matrix4.h"
#include "matrix_transform.h"

/*
	pointInRectangle : Checks if a point is included in a rectangle formed by 4 elements of vertices
	Vertices length should be 4
	Notice : Vertices have to be passed count clockwise

*/

bool pointInRectangle(Vector2 vertices[], Vector2 point)
{
	Vector2 center = { (vertices[1].x  + vertices[2].x) / 2.0f, (vertices[0].y + vertices[1].y) / 2.0f};

	for(ulong i = 0 ; i < 4 ; i++)
	{
		Vector2 vertexToCenter = { center.x - vertices[i].x, center.y - vertices[i].y };
		Vector2 vertexToPoint = { point.x - vertices[i].x, point.y - vertices[i].y };

		//printf("vertexToPoint\n");
		//printVector2(&vertexToPoint);

		Vector2 vertexToCenter_unit = Vec2_unit(&vertexToCenter);
		Vector2 vertexToPoint_unit = Vec2_unit(&vertexToPoint);

		if(!Vectors2_equals(&vertexToCenter_unit, &vertexToPoint_unit))
		{
			return false;
		}

	}	
	return true;
}

Matrix4 createTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale)
{
	Matrix4 transformationMatrix = getIdentityMatrix4();

	transformationMatrix = Matrix4_translate(&transformationMatrix, translation);

	Vector3 zAxis = {0, 0, 1};
	Vector3 yAxis = {0, 1, 0};
	Vector3 xAxis = {1, 0, 0};
	transformationMatrix = Matrix4_rotate(&transformationMatrix, zAxis, DEGREE_TO_RADIANS(rotation.z));
	transformationMatrix = Matrix4_rotate(&transformationMatrix, yAxis, DEGREE_TO_RADIANS(rotation.y));
	transformationMatrix = Matrix4_rotate(&transformationMatrix, xAxis, DEGREE_TO_RADIANS(rotation.x));
	
	transformationMatrix = Matrix4_scale(&transformationMatrix, scale);

	//printMatrix4(&transformationMatrix);

	return transformationMatrix;
}
Matrix4 createViewMatrix(Vector3 cameraPosition, float yaw, float pitch)
{
	Matrix4 viewMatrix = getIdentityMatrix4();

	Vector3 yAxis = {0, 1, 0};
	Vector3 xAxis = {1, 0, 0};

	
	Vector3 zAxis = {0, 0, 1};

	Vector3 negativeCameraPosition = Vec3_mulNumber_get(&cameraPosition, -1.0f);
	
	viewMatrix = Matrix4_rotate(&viewMatrix, xAxis, (float) DEGREE_TO_RADIANS(pitch));
	viewMatrix = Matrix4_rotate(&viewMatrix, yAxis, (float) DEGREE_TO_RADIANS(yaw));

	viewMatrix = Matrix4_translate(&viewMatrix, negativeCameraPosition);


	//Vector3 negativeCameraPosition = Vec3_mulNumber_get(&cameraPosition, -1.0f);
	//viewMatrix = Matrix4_translate(&viewMatrix, cameraPosition);


	/*Vector3 negativeCameraPosition = Vec3_mulNumber_get(&cameraPosition, -1.0f);
	viewMatrix = Matrix4_translate(&viewMatrix, negativeCameraPosition);
	viewMatrix = Matrix4_translate(&viewMatrix, negativeCameraPosition);*/

	return viewMatrix;
}


#endif