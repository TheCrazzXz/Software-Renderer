#pragma once

#ifndef __VERTEX_DRAWER_H__
#define __VERTEX_DRAWER_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "clipping/line_clipping.h"

#include "lines.h"
#include "drawContext.h"

#define LINE_CLIPPING true

bool horizontalLineOutOfScreen(float x0, float x1, float y)
{
	if(y < -1 || y > 1)
	{
		return 1;
	}

	if((x0 > 1) && (x1 > 1))
	{
		return 1;
	}
	
	if((x0 < -1) && (x1 < -1))
	{
		return 1;
	}
	return 0;
}

/*
	Clip line if clipping if line clipping is enabled and convert convert coordinates to screen coords

*/
int treatLineAndGetScreenCoords(float x0, float x1, float y0, float y1, PixelBuffer* pixelBuffer, Vector2* Out_CurrentVertex, Vector2* Out_NextVertex, bool clip)
{
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
		return -1;
	}

	if(clip)
	{
		if(liang_barsky_clipper(xmin, ymin, xmax, ymax, x0, y0, x1, y1, &xn0, &yn0, &xn1, &yn1) != 1)
		{
			return -2;
		}
	}
	*Out_CurrentVertex = planeCoordinatesToScreenCoordinates(Vec2_create(xn0, yn0), pixelBuffer);
	*Out_NextVertex = planeCoordinatesToScreenCoordinates(Vec2_create(xn1, yn1), pixelBuffer);

	return 1;
}

void drawUnicolorHorizontalLineAcceleratedDraw(float x0, float x1, float y, Vector4 uniformColor, struct PixelBuffer *pixelBuffer)
{
	Vector2 currentVertex;
	Vector2 nextVertex;

	if(horizontalLineOutOfScreen(x0, x1, y))
	{
		return;
	}

	x0 = clampFloat(x0, -1, 1);
	x1 = clampFloat(x1, -1, 1);
	y = clampFloat(y, -1, 1);

	if(treatLineAndGetScreenCoords(x0, x1, y, y, pixelBuffer, &currentVertex, &nextVertex, false) == 1)
	{
		unicolorHorizontalLineAcceleratedDraw((int)currentVertex.x, (int)nextVertex.x, (int) currentVertex.y, vec4UnitToPixelColor(uniformColor), pixelBuffer);
	}
}

void drawInterpolatedHorizontalLineAcceleratedDraw(float x0, float x1, float y, VerticesState vstate, struct PixelBuffer *pixelBuffer)
{
	Vector2 currentVertex;
	Vector2 nextVertex;
	
	if(horizontalLineOutOfScreen(x0, x1, y))
	{
		return;
	}

	x0 = clampFloat(x0, -1, 1);
	x1 = clampFloat(x1, -1, 1);
	y = clampFloat(y, -1, 1);

	if(treatLineAndGetScreenCoords(x0, x1, y, y, pixelBuffer, &currentVertex, &nextVertex, false) == 1)
	{
		interpolatedHorizontalLineAcceleratedDraw(currentVertex.x, nextVertex.x, currentVertex.y, vstate, pixelBuffer);
	}
}

void connect2Vertices(float x0, float y0, float x1, float y1, VerticesState vstate, PixelBuffer* pixelBuffer)
{
	Vector2 currentVertex;
	Vector2 nextVertex;

	if(treatLineAndGetScreenCoords(x0, x1, y0, y1, pixelBuffer, &currentVertex, &nextVertex, true) == 1)
	{
		plotLineScreen(currentVertex.x, currentVertex.y, nextVertex.x, nextVertex.y, vstate, pixelBuffer);
	}
	
	
	    
}
void drawTrianglePoints(DrawVertexData *triangle, VerticesState vstate, PixelBuffer* pixelBuffer)
{

	for(ulong i = 0 ; i < 3 ; i++)
	{
		DrawVertexData* vertex = &triangle[i];
		NDCpixel(vertex->position.x, vertex->position.y, vec4UnitToPixelColor(interpolateColourN(vertex->position, vstate.allVertices, vstate.allVertices_length)), pixelBuffer);
	}
		
}

#endif