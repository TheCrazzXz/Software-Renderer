#pragma once

#ifndef __VERTEX_DRAWER_H__
#define __VERTEX_DRAWER_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"

void draw2DVertices(float* positions, ulong positionsLength, PixelBuffer* pixelBuffer)
{
	for(int i = 0 ; i < positionsLength ; i += 1 * 4)
	{
		Vector2 currentVertex = Vec2_create((int) (pixelBuffer->width * (positions[i] + 1) / 2), (int) (pixelBuffer->height * (positions[i+1] + 1) / 2));
		Vector2 nextVertex = Vec2_create((int) (pixelBuffer->width * (positions[i+2] + 1) / 2), (int) (pixelBuffer->height * (positions[i+3] + 1) / 2));

		contextVertex0.position = currentVertex;
		contextVertex1.position = nextVertex;

		//printVector2(&currentVertex);
		//printVector2(&nextVertex);

		Vector2 directorVector = Vec2_create(nextVertex.x - currentVertex.x, nextVertex.y - currentVertex.y);

		float dx = abs(directorVector.x);
		float dy = abs(directorVector.y);

		float lastEquation = 0;
		
		float x1 = currentVertex.x;
		float x2 = nextVertex.x;

		float y1 = currentVertex.y;
		float y2 = nextVertex.y;

 		plotLine(x1, y1, x2, y2, pixelBuffer);
	    
	}
}




void drawEachVertex(Vector* verticesVector, PixelBuffer* pixelBuffer)
{
	for(ulong i = 0 ; i < verticesVector->length ; i++)
	{
		struct DrawVertexData* vertex = Vector_element(verticesVector, i);

		planPixel(vertex->position.x, vertex->position.y, vertex->color, pixelBuffer);
	}
}

#endif