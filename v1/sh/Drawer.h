#pragma once

#ifndef __DRAWER_H__
#define __DRAWER_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "lines.h"
#include "vertexConnector.h"
#include "interpolation.h"
#include "vertexDrawer.h"

#include <stdint.h>
#include <stdbool.h>

/*
	DRAWER_RENDER_LINKED_ALL_VERTICES : Connects each single pixel with each single other pixel
	DRAWER_RENDER_STRIPS : Connects each pixel with the pixel, forming a strip
	DRAWER_RENDER_POINTS : Doesn't connect anything, only draws a point for each vertex
	DRAWER_RENDER_RECTANGLES_LINES : Connect each vertex of each rectangle
	DRAWER_RENDER_RECTANGLES_FILL : Same as DRAWER_RENDER_RECTANGLES_LINES but fills every rectangle
*/

#define DRAWER_RENDER_LINKED_ALL_VERTICES (char)0
#define DRAWER_RENDER_STRIPS (char)1
#define DRAWER_RENDER_POINTS (char)2
#define DRAWER_RENDER_RECTANGLES_LINES (char)3
#define DRAWER_RENDER_RECTANGLES_FILL (char)4


typedef uint32_t Uint32;

void drawerError(char* msg)
{
	printf("Drawer Error : %s\n", msg);
	exit(-1);
}


	/*
		A     B
		*-----*
		|     |
		|     |
		*-----*
		C     D
	*/

Vector2 screenCoordinatesToPlaneCoordinates(Vector2 vector, PixelBuffer* pixelBuffer)
{
	Vector2 converted = { 2 * vector.x / pixelBuffer->width - 1, 2 * vector.y / pixelBuffer->height - 1};
	return converted;
}

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

void drawRectangles(Vector* verticesVector, PixelBuffer* pixelBuffer, bool fill)
{
	for(ulong i = 0 ; i < verticesVector->length / 4 ; i++)
	{
		struct DrawVertexData square[] = {
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 1), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 2), 
			*(struct DrawVertexData*) Vector_element(verticesVector, 4 * i + 3), 
		};
		connectSquare(square, pixelBuffer);
		if(fill)
		{
			fillSquare(square, pixelBuffer);
		}
	}
}
    
void beginDraw(char type, PixelBuffer* pixelBuffer)
{
	contextNextVertexColor = 0xffffffff;

	contextVerticesVector = Vector_create(sizeof(struct DrawVertexData));
	contextPixelBuffer = pixelBuffer;
	
	contextDrawType = type;
}   
void drawDataColor(Uint32 color)
{
	contextNextVertexColor = color;
}

void drawDataVertex2(float positionX, float positionY)
{
	struct DrawVertexData vertexData = { {positionX, positionY}, contextNextVertexColor };
	Vector_Push_back(&contextVerticesVector, &vertexData);
}
void drawDataVertex4(float positionX, float positionY, float positionZ, float positionW)
{

	struct DrawVertexData vertexData = { {positionX / positionW, positionY / positionW}, contextNextVertexColor };
	
	vertexData.position.x = (vertexData.position.x * 1 / (2.0f * positionW));
    vertexData.position.y = (vertexData.position.y * 1 / (2.0f * positionW));

    printf("%f ; %f\n", vertexData.position.x, vertexData.position.y);

	Vector_Push_back(&contextVerticesVector, &vertexData);
}

void endDraw()
{
	if(contextDrawType == DRAWER_RENDER_LINKED_ALL_VERTICES)
	{
		connectAllVertices(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_RENDER_STRIPS)
	{
		if((contextVerticesVector.length / 2) & 1  != 0)
		{
			drawerError("In mode DRAWER_RENDER_STRIPS there should be even number of vertices");
		}
		connectEachVertexWithNext(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_RENDER_POINTS)
	{
		drawEachVertex(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_RENDER_RECTANGLES_LINES || contextDrawType == DRAWER_RENDER_RECTANGLES_FILL)
	{
		if((contextVerticesVector.length % 4))
		{
			drawerError("In mode DRAWER_RENDER_RECTANGLES there should be n packs of 4 vertices, representing each square");
		}
		if(contextDrawType == DRAWER_RENDER_RECTANGLES_LINES)
		{
			drawRectangles(&contextVerticesVector, contextPixelBuffer, false);
		}
		else
		{

			drawRectangles(&contextVerticesVector, contextPixelBuffer, true);
		}
		
	}
	Vector_Delete(&contextVerticesVector);
}

/*
	draw2D() : Draws pixels from vertices in 2D

*/


#endif