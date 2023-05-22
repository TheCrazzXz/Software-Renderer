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
		if(!isnormal(positions[0]) || !isnormal(positions[1]) || !isnormal(positions[2]) || !isnormal(positions[3]))
		{
			
		}
		Vector2 currentVertex = planeCoordinatesToScreenCoordinates(Vec2_create(positions[i], positions[i + 1]), pixelBuffer);
		Vector2 nextVertex = planeCoordinatesToScreenCoordinates(Vec2_create(positions[i+2], positions[i + 3]), pixelBuffer);



		contextVertex0.position = currentVertex;
		contextVertex1.position = nextVertex;

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