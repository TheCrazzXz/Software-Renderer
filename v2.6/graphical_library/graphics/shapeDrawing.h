#pragma once

#include "clipping/line_clipping.h"
#include "drawContext.h"
#ifndef __SHAPE_DRAWING_H__
#define __SHAPE_DRAWING_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "lines.h"
#include "vertexConnector.h"
#include "interpolation.h"
#include "vertexProcessor.h"
#include "../maths/polygon_clipping.h"
#include "../maths/sorting_algorithms.h"
#include "../maths/triangle.h"
#include "triangleFillingAlgorithm.h"

#define TRIANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING false
#define RECTANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING true

#define DRAWER_ENABLE_TRIANGLE_DEPTH_TEST false

#define DRAWER_DRAW_LEVEL_FILL 0
#define DRAWER_DRAW_LEVEL_WIREFRAME 1
#define DRAWER_DRAW_LEVEL_POINTS 2
#define DRAWER_DRAW_LEVEL_FILL_AND_WIREFRAME 3

#define DRAWER_INTERPOLATE_EACH_SHAPE 0
#define DRAWER_INTERPOLATE_EACH_DATA 1

#define DRAWER_COLOR_WHEN_FILL_AND_WIREFRAME Vec4_create(1, 1, 1, 1)

Uint32 drawer_interpolation_mode = DRAWER_INTERPOLATE_EACH_DATA;

Uint32 draw_level = DRAWER_DRAW_LEVEL_FILL;


	/*
		A     B
		*-----*
		|     |
		|     |
		*-----*
		C     D
	*/


void drawRectangles(Vector* verticesVector, PixelBuffer* pixelBuffer, bool fill)
{
	Vector2 screen_square[] = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
	for(ulong i = 0 ; i < verticesVector->length / 4 ; i++)
	{

		DrawVertexData square[] = {
			*(DrawVertexData*) Vector_element(verticesVector, 4 * i), 
			*(DrawVertexData*) Vector_element(verticesVector, 4 * i + 1), 
			*(DrawVertexData*) Vector_element(verticesVector, 4 * i + 2), 
			*(DrawVertexData*) Vector_element(verticesVector, 4 * i + 3), 
		};


		Vector2 center = { (square[0].position.x + square[1].position.x) / 2.0f, (square[0].position.y + square[2].position.y) / 2.0f};
		//printf("Center : \n");
		printVector2(&center);
		NDCpixel(center.x, center.y, 0xffffffff, pixelBuffer);

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
	}
}


bool triangleFillingVerticesYAscendingSort(void* candidate, void* currentBest)
{
	return ((DrawVertexData*)(candidate))->position.y > ((DrawVertexData*)(currentBest))->position.y;
}
void fillTriangle(DrawVertexData *triangle, VerticesState vstate, PixelBuffer* pixelBuffer)
{
	if(isTriangleOutOfScreen(&triangle[0], &triangle[1], &triangle[2]))
	{
		return;
	}
	// sorting triangles Y position from bigger to lower 
	sortFunc(triangle, 3, sizeof(DrawVertexData), triangleFillingVerticesYAscendingSort);

	if(triangle[1].position.y == triangle[2].position.y)
	{
		drawBottomFlatTriangle(&triangle[0], &triangle[1], &triangle[2], vstate);
	}
	else if(triangle[0].position.y == triangle[1].position.y)
	{

		drawTopFlatTriangle(&triangle[0], &triangle[1], &triangle[2], vstate);
	}
	else
	{
		float m = (triangle[2].position.y - triangle[0].position.y) / (triangle[2].position.x - triangle[0].position.x);
		Vector2 v4_pos = { ( triangle[1].position.y - triangle[0].position.y ) / m + triangle[0].position.x, triangle[1].position.y};

		DrawVertexData vert4 = { v4_pos, Vec4_create(1, 1, 1, 1) };
		drawBottomFlatTriangle(&triangle[0], &triangle[1], &vert4, vstate);
		drawTopFlatTriangle(&triangle[1], &vert4, &triangle[2], vstate);
	}
}

// OLD TRIANGLE FILLING ALGORITHM : 1 fps
/*
void fillTriangle(DrawVertexData *triangle, DrawVertexData* allVertices, ulong allVertices_length, PixelBuffer* pixelBuffer)
{
	float x_coords[] = {triangle[0].position.x, triangle[1].position.x, triangle[2].position.x};
	float y_coords[] = {triangle[0].position.y, triangle[1].position.y, triangle[2].position.y};
	
	float xmin = min(x_coords, sizeof(x_coords) / sizeof(float));
	float xmax = max(y_coords, sizeof(x_coords) / sizeof(float));
	float ymin = min(y_coords, sizeof(y_coords) / sizeof(float));
	float ymax = max(y_coords, sizeof(y_coords) / sizeof(float));

	float adv_x = 1.0f / (float)pixelBuffer->width;
	float adv_y = 1.0f / (float)pixelBuffer->height;

	for(float x = xmin ; x < xmax ; x += adv_x)
	{
		for(float y = xmin ; y < xmax ; y += adv_x)
		{
			Vector2 point = {x, y};
			if(PointInTriangle(&point, &triangle[0].position, &triangle[1].position, &triangle[2].position))
			{
				NDCpixel(x, y, interpolateColourN(point, allVertices, allVertices_length), pixelBuffer);
			}
		}
	}
    
}
*/


void drawTriangles(Vector* trianglesVec, VerticesState vstate)
{
	VerticesState wireframe_box_vstate;
	wireframe_box_vstate = makeVerticesStateOtherColor(DRAWER_COLOR_WHEN_FILL_AND_WIREFRAME);
	for(ulong i = 0 ; i < trianglesVec->length ; i++)
	{
		DrawVertexData* triangle = (DrawVertexData*)(Vector_element(trianglesVec, i));
		

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


		if(drawer_interpolation_mode == DRAWER_INTERPOLATE_EACH_SHAPE)
		{
			vstate.allVertices = triangle;
			vstate.allVertices_length = 3; // number of vertices for a triangle
			vstate.allVerticesHaveSameColor =  !checkIfVerticesColorIsDifferent(vstate.allVertices, vstate.allVertices_length);
		}

		if(draw_level == DRAWER_DRAW_LEVEL_WIREFRAME)
		{
			connectTriangle(triangle, vstate, contextPixelBuffer);
		}
		else if(draw_level == DRAWER_DRAW_LEVEL_FILL_AND_WIREFRAME)
		{
			fillTriangle(triangle, vstate, contextPixelBuffer);
			
			
			connectTriangle(triangle, wireframe_box_vstate, contextPixelBuffer);

		}
		else if(draw_level == DRAWER_DRAW_LEVEL_POINTS)
		{
			drawTrianglePoints(triangle, vstate, contextPixelBuffer);
			
		}
		else
		{
			fillTriangle(triangle, vstate, contextPixelBuffer);
		}
	}
}

#endif