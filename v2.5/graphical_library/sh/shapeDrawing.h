#pragma once

#ifndef __SHAPE_DRAWING_H__
#define __SHAPE_DRAWING_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "lines.h"
#include "vertexConnector.h"
#include "interpolation.h"
#include "vertexDrawer.h"
#include "../maths/polygon_clipping.h"
#include "../maths/sorting_algorithms.h"
#include "../maths/triangle.h"
#include "Rendering.h"
#include "triangleFillingAlgorithm.h"
	/*
		A     B
		*-----*
		|     |
		|     |
		*-----*
		C     D
	*/


void fillSquare(struct DrawVertexData square[], PixelBuffer* pixelBuffer)
{
	Vector2 squareVertices[] = { 
		square[0].position,
		square[1].position,
		square[2].position,
		square[3].position

	};

	for(ulong y = 0 ; y < pixelBuffer->height ; y++)
	{
		for(ulong x = 0 ; x < pixelBuffer->width ; x++)
		{
			Vector2 pixelScreen = {x, y};
			Vector2 point = screenCoordinatesToPlaneCoordinates(pixelScreen, pixelBuffer);
			if(pointInRectangle(squareVertices, point))
			{

				renderInterpolateAtPosition(x, y, pixelBuffer);
			}
		}
	}
}

/*Vector2 getIntersectionPoint(float h, Vector2 point0, Vector2 point1 )
{

	float m = (float)(point0.y - point1.y) / (float)(point0.x - point1.x);

	Vector2 output;

	output.x = (h - point0.y) / m + point0.x;
	output.y = h;

	return output;
}*/
void drawRectangles(Vector* verticesVector, PixelBuffer* pixelBuffer, bool fill)
{
	Vector2 screen_square[] = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
	for(ulong i = 0 ; i < verticesVector->length / 4 ; i++)
	{

		struct DrawVertexData square[] = {
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 1), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 2), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 3), 
		};


		Vector2 center = { (square[0].position.x + square[1].position.x) / 2.0f, (square[0].position.y + square[2].position.y) / 2.0f};
		//printf("Center : \n");
		printVector2(&center);
		planPixel(center.x, center.y, 0xffffffff, pixelBuffer);

		if(RECTANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING)
		{
			if(!isInBoundsPlan(square[0].position.x, square[0].position.y) 
			&& !isInBoundsPlan(square[1].position.x, square[1].position.y) 
			&& !isInBoundsPlan(square[2].position.x, square[2].position.y)
			&& !isInBoundsPlan(square[3].position.x, square[3].position.y) 
			)
			{
				//printf("OUT OF BOUNDS\n");
				continue;

			}
		}

		connectSquare(square, pixelBuffer);
		if(fill)
		{
			fillSquare(square, pixelBuffer);
		}
	}
}

float x_advance = 1.0f;
float y_advance = 1.0f;

bool triangleFillingVerticesYAscendingSort(void* candidate, void* currentBest)
{
	return ((struct DrawVertexData*)(candidate))->position.y > ((struct DrawVertexData*)(currentBest))->position.y;
}
void fillTriangle(struct DrawVertexData *triangle, PixelBuffer* pixelBuffer)
{
	// sorting triangles Y position from bigger to lower 

	sortFunc(triangle, 3, sizeof(struct DrawVertexData), triangleFillingVerticesYAscendingSort);
	if(triangle[1].position.y == triangle[2].position.y)
	{
		drawBottomFlatTriangle(&triangle[0], &triangle[1], &triangle[2]);
	}
	else if(triangle[0].position.y == triangle[1].position.y)
	{
		drawTopFlatTriangle(&triangle[0], &triangle[1], &triangle[2]);
	}
	else
	{
		float m = (triangle[2].position.y - triangle[0].position.y) / (triangle[2].position.x - triangle[0].position.x);
		Vector2 v4_pos = { ( triangle[1].position.y - triangle[0].position.y ) / m + triangle[0].position.x, triangle[1].position.y};

		struct DrawVertexData vert4 = { v4_pos, interpolate2(v4_pos.x, v4_pos.y, &triangle[0], &triangle[2]) };

		drawBottomFlatTriangle(&triangle[0], &triangle[1], &vert4);
		drawTopFlatTriangle(&triangle[1], &vert4, &triangle[2]);
	}
}
void drawTriangles(Vector* verticesVector, PixelBuffer* pixelBuffer)
{
	
	Vector2 screen_square[] = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
	for(ulong i = 0 ; i < verticesVector->length / 3 ; i++)
	{
		struct DrawVertexData triangle[] = {
			*(struct DrawVertexData*) Vector_element(verticesVector, 3 * i), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 3 * i + 1), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 3 * i + 2), 
		};

		if(TRIANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING)
		{
			if(!isInBoundsPlan(triangle[0].position.x, triangle[0].position.y) 
			&& !isInBoundsPlan(triangle[1].position.x, triangle[1].position.y) 
			&& !isInBoundsPlan(triangle[2].position.x, triangle[2].position.y)
			)
			{
				//printf("OUT OF BOUNDS\n");
				continue;

			}
		}

		connectTriangle(triangle, pixelBuffer);
		//fillTriangle(triangle, pixelBuffer);
	}
}

#endif