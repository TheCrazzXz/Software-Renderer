#pragma once

#ifndef __VEC_H__
#define __VEC_H__

#include <stdio.h>
#include <math.h>

#include <stdbool.h>

struct Vec2
{
	float x;
	float y; 
};

struct Vec2 Vec2_create(float x, float y)
{
	struct Vec2 vector;
	vector.x = x;
	vector.y = y;
	return vector;
}

/*
	Vector2_add : Returns a new vector : vector1 + vector2 (or vector2 + vector1)

*/

struct Vec2 Vectors2_add(struct Vec2* vector1, struct Vec2* vector2)
{
	struct Vec2 result = { vector2->x + vector1->x, vector2->y + vector1->y};
	return result;
}

/*
	Vector2_sub : Returns a new vector : vector1 - vector2 (or -vector2 + vector1)

*/

struct Vec2 Vectors2_sub(struct Vec2* vector1, struct Vec2* vector2)
{
	struct Vec2 result = { vector1->x - vector2->x, vector1->y - vector2->y};
	return result;
}

/*
	Vectors2_distance : Returns the distance between two vectors :
	d = sqrt((x1 - x2)² + (y1 - y2)²)

*/

float Vectors2_distance(struct Vec2* vector1, struct Vec2* vector2)
{
	return sqrt(
			(vector1->x - vector2->x) * (vector1->x - vector2->x) /* (deltaX)² */
		+	(vector1->y - vector2->y) * (vector1->y - vector2->y) /* (deltaY)² */	
	);
}
float Vectors2_dot(struct Vec2* vector1, struct Vec2* vector2)
{
	return vector1->x * vector2->x + vector1->y + vector2->y;
}
/*
	Vec2_mulNumber_get : Returns a new vector : number * vector where number is real

*/

struct Vec2 Vec2_mulNumber_get(struct Vec2* vector, float number)
{
	struct Vec2 result = { vector->x * number, vector->y * number};
	return result;
}

/*
	Vec2_mulNumber : Sets vector to : number * vector where number is real

*/

void Vec2_mulNumber(struct Vec2* vector, float number)
{
	vector->x *= number;
	vector->y *= number;
}

/*
	Vec2_divNumber_get : Returns a new vector : vector / number where number is real

*/

struct Vec2 Vec2_divNumber_get(struct Vec2* vector, float number)
{
	struct Vec2 result = { vector->x / number, vector->y / number};
	return result;
}

/*
	Vec2_divNumber : Sets vector to : vector / number where number is real

*/

void Vec2_divNumber(struct Vec2* vector, float number)
{
	vector->x /= number;
	vector->y /= number;
}

/*
	Vec2_length() : Returns the length / magnitude / norm of a vector :

	||v|| = sqrt(x[v]² + y[v]²) : Pythagorus theorem

*/

float Vec2_length(struct Vec2* vector)
{
	return sqrt(vector->x * vector->x + vector->y * vector->y);
}

/*
	Vec2_unit() : Returns a new vector whose x and y components are always between -1 or 1
	x and y components would hold signs of respective axis of the passed in parameter vector
	Sign : Either +1 or -1

*/

struct Vec2 Vec2_unit(struct Vec2* vector)
{
	struct Vec2 vec = { vector->x / abs(vector->x), vector->y / abs(vector->y) };
	return vec;
}
bool Vectors2_equals(struct Vec2* vec1, struct Vec2* vec2 )
{
	return vec1->x == vec2->x && vec1->y == vec2->y;
}

void printVector2(struct Vec2* vector)
{
	printf("<Vector2 : %f ; %f>\n", vector->x, vector->y);
}

typedef struct Vec2 Vector2; 

struct Vec3
{
	float x;
	float y; 
	float z;
};

struct Vec3 Vec3_create(float x, float y, float z)
{
	struct Vec3 vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

/*
	Vec3_mulNumber_get : Returns a new vector : number * vector where number is real

*/

struct Vec3 Vec3_mulNumber_get(struct Vec3* vector, float number)
{
	struct Vec3 result = { vector->x * number, vector->y * number, vector->z * number};
	return result;
}

/*
	Vec3_mulNumber : Sets vector to : number * vector where number is real

*/

void Vec3_mulNumber(struct Vec3* vector, float number)
{
	vector->x *= number;
	vector->y *= number;
	vector->z *= number;
}

void printVector3(struct Vec3* vector)
{
	printf("<Vector3 : %f ; %f ; %f>\n", vector->x, vector->y, vector->z);
}


typedef struct Vec3 Vector3; 

struct Vec4
{
	float x;
	float y; 
	float z;
	float w;
};

struct Vec4 Vec4_create(float x, float y, float z, float w)
{
	struct Vec4 vector;
	vector.x = x;
	vector.y = y;
	vector.z = z;
	vector.w = w;
	return vector;
}

/*
	Vec4_forceConvertToVec3() : Creates a new vec3 with vector.x, vector.y and vector.z

*/

struct Vec3 Vec4_forceConvertToVec3(struct Vec4* vector)
{
	struct Vec3 outputVector = {vector->x, vector->y, vector->z};
	return outputVector;
}

void printVector4(struct Vec4* vector)
{
	printf("<Vector4 : %f ; %f ; %f ; %f>\n", vector->x, vector->y, vector->z, vector->w);
}

typedef struct Vec4 Vector4; 

#endif