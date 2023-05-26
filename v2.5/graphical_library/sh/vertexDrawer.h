#pragma once

#ifndef __VERTEX_DRAWER_H__
#define __VERTEX_DRAWER_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "clipping/line_clipping.h"

#define LINE_CLIPPING true

void draw2DVertices(float* positions, ulong positionsLength, PixelBuffer* pixelBuffer)
{
	for(int i = 0 ; i < positionsLength ; i += 1 * 4)
	{
		float x0 = positions[i];
		float y0 = positions[i + 1];

		float x1 = positions[i + 2];
		float y1 = positions[i + 3];

		/*if(!isInBoundsPlan(x0, y0) || !isInBoundsPlan(x1, y1))
		{
			printf("line : %f -> %f\t%f -> %f \n", x0, x1, y0, y1);
		}*/
		

		int xmin = -1;
	    int xmax = 1;

	    int ymin = -1;
	    int ymax = 1;

	    float xn0 = x0;
	    float xn1 = x1;

	    float yn0 = y0;
	    float yn1 = y1;

	    if(isnan(x0) || isnan(x1) || isnan(y0) || isnan(y1))
	    {
	    	continue;
	    }

	    if(LINE_CLIPPING)
	    {
	        if(liang_barsky_clipper(xmin, ymin, xmax, ymax, x0, y0, x1, y1, &xn0, &yn0, &xn1, &yn1) != 1)
	        {
	            continue;
	        }
	    }

	    Vector2 currentVertex = planeCoordinatesToScreenCoordinates(Vec2_create(xn0, yn0), pixelBuffer);
		Vector2 nextVertex = planeCoordinatesToScreenCoordinates(Vec2_create(xn1, yn1), pixelBuffer);

		contextVertex0.position = currentVertex;
		contextVertex1.position = nextVertex;

 		plotLine(currentVertex.x, currentVertex.y, nextVertex.x, nextVertex.y, pixelBuffer);
	    
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