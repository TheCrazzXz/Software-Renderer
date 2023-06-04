#pragma once

#ifndef __DRAW_CONTEXT_H__
#define __DRAW_CONTEXT_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"

struct DrawVertexData
{
	Vector2 position;
	Vector4 color;
	float depth;
	float weight;
};

struct InputVertexData
{
	Vector4 position;
	Vector4 color;
};


struct VerticesState
{
	struct DrawVertexData* allVertices;
	ulong allVertices_length;
	bool allVerticesHaveSameColor;
	bool useOtherColor;
	Vector4 otherColor;

};

struct VerticesState makeVerticesState(struct DrawVertexData* allVertices,
	ulong allVertices_length,
	bool allVerticesHaveSameColor)
{
	struct VerticesState out;

	out.allVertices = allVertices;
	out.allVertices_length = allVertices_length;
	out.allVerticesHaveSameColor = allVerticesHaveSameColor;
	out.useOtherColor = false;
	out.otherColor = Vec4_create(0, 0, 0, 0);
	
	return out;
}
struct VerticesState makeVerticesStateOtherColor(Vector4 otherColor)
{
	struct VerticesState out;

	out.allVertices = NULL;
	out.allVertices_length = 0;
	out.allVerticesHaveSameColor = false;
	out.useOtherColor = true;
	out.otherColor = otherColor;

	return out;	
};


typedef struct VerticesState VerticesState;
typedef struct DrawVertexData DrawVertexData;
typedef struct InputVertexData InputVertexData;

struct DrawVertexData contextVertex0;
struct DrawVertexData contextVertex1;
struct DrawVertexData contextVertex2;

Uint32 contextNextVertexColor = 0xffffffff;
bool contextUseOnlyNextVertexColorBypass = false;
PixelBuffer* contextPixelBuffer;
char contextDrawType;

Vector contextVerticesVector;

Vector* contextDrawingVertices;

#endif