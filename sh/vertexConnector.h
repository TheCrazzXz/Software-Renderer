#pragma once

#ifndef __VERTEX_CONNECTOR__
#define __VERTEX_CONNECTOR__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "drawContext.h"
#include "interpolation.h"
#include "vertexDrawer.h"
#include "../maths/line_clipping.h"



void vertices2Color(Uint32 vertex0Color, Uint32 vertex1Color)
{
	contextVertex0.color = vertex0Color;
	contextVertex1.color = vertex1Color;
}

void connectAllVertices(Vector* verticesVector, PixelBuffer* pixelBuffer)
{
	for(ulong i = 0 ; i < verticesVector->length ; i++)
	{
		struct DrawVertexData* vertex1 = Vector_element(verticesVector, i);
		for(ulong j = 0 ; j < verticesVector->length ; j++)
		{
			struct DrawVertexData* vertex2 = Vector_element(verticesVector, j);

			if(vertex1->position.x != vertex2->position.x && vertex1->position.y != vertex2->position.y)
			{
				vertices2Color(vertex1->color, vertex2->color);
				float positions[] = { vertex1->position.x, vertex1->position.y, vertex2->position.x, vertex2->position.y };
				draw2DVertices(positions, 4, pixelBuffer);
			}
		}
	}
}
void connectEachVertexWithNext(Vector* verticesVector, PixelBuffer* pixelBuffer)
{
	
	for(ulong i = 0 ; i < verticesVector->length / 2 ; i++)
	{
		struct DrawVertexData* vertex1 = Vector_element(verticesVector, 2 * i);
		struct DrawVertexData* vertex2 = Vector_element(verticesVector, 2 * i + 1);

		

		vertices2Color(vertex1->color, vertex2->color);
		float positions[] = { vertex1->position.x, vertex1->position.y, vertex2->position.x, vertex2->position.y };
		draw2DVertices(positions, 4, pixelBuffer);
	}
}

void connectSquare(struct DrawVertexData square[], PixelBuffer* pixelBuffer)
{

	for(ulong i = 0 ; i < 4 ; i++)
	{
		struct DrawVertexData* vertex1 = &square[i];
		int nextIndex = 0;
		if(i < 3)
		{
			nextIndex = i + 1;
		}
		struct DrawVertexData* vertex2 = &square[nextIndex];
		vertices2Color(vertex1->color, vertex2->color);
		float positions[] = { vertex1->position.x, vertex1->position.y, vertex2->position.x, vertex2->position.y };
		draw2DVertices(positions, 4, pixelBuffer);
		

		
	}
		
}

void connectTriangle(struct DrawVertexData triangle[], PixelBuffer* pixelBuffer)
{

	for(ulong i = 0 ; i < 3 ; i++)
	{
		struct DrawVertexData* vertex1 = &triangle[i];
		int nextIndex = 0;
		if(i < 2)
		{
			nextIndex = i + 1;
		}
		struct DrawVertexData* vertex2 = &triangle[nextIndex];
		vertices2Color(vertex1->color, vertex2->color);
		float positions[] = { vertex1->position.x, vertex1->position.y, vertex2->position.x, vertex2->position.y };
		draw2DVertices(positions, 4, pixelBuffer);
		

		
	}
		
}

#endif