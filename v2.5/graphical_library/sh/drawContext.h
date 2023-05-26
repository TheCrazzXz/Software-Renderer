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
	Uint32 color;
	float depth;
};	
struct RenderVertexData
{
	Vector4 position;
	Uint32 color;
};
typedef struct RenderVertexData RenderVertexData;


struct DrawVertexData contextVertex0;
struct DrawVertexData contextVertex1;
struct DrawVertexData contextVertex2;

Uint32 contextNextVertexColor = 0xffffffff;
PixelBuffer* contextPixelBuffer;
char contextDrawType;

Vector contextVerticesVector;

#endif