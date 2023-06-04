/*#pragma once

#include "drawContext.h"
#ifndef __DRAWER_H__
#define __DRAWER_H__

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
#include "shapeDrawing.h"
#include "DataDrawing.h"
//#include "triangleFillingAlgorithm.h"

#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>


int max_draw_time = 0; // The time that the object  

typedef uint32_t Uint32;
clock_t beforeAtBeginDraw;


void drawerError(char* msg)
{
	printf("Drawer Error : %s\n", msg);
	exit(-1);
}

    
void shBeginDraw(char type, PixelBuffer* pixelBuffer)
{
	beforeAtBeginDraw = clock();
	contextNextVertexColor = 0xffffffff;

	contextVerticesVector = Vector_create(sizeof(DrawVertexData));
	contextDrawingVertices = &contextVerticesVector;
	contextPixelBuffer = pixelBuffer;
	
	contextDrawType = type;
}   
void shDrawDataColor(Uint32 color)
{
	contextNextVertexColor = color;
}

void shDrawDataVertex2(float positionX, float positionY)
{
	DrawVertexData vertexData = { {positionX, positionY}, contextNextVertexColor };
	Vector_Push_back(&contextVerticesVector, &vertexData);
}
void shDrawDataVertex4(float positionX, float positionY, float positionZ, float positionW)
{
	InputVertexData InputVertexData = { {positionX, positionY, positionZ, positionW}, contextNextVertexColor };
	DrawVertexData outVert;
	processInputVertexData(&InputVertexData, &outVert);

    Vector_Push_back(&contextVerticesVector, &outVert);
	
}

void shEndDraw()
{
	VerticesState vstate = makeVerticesState((DrawVertexData*)contextVerticesVector.data, contextVerticesVector.length, false);
	if(contextDrawType == DRAWER_DRAW_LINKED_ALL_VERTICES)
	{
		connectAllVertices(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_DRAW_STRIPS)
	{
		if((contextVerticesVector.length / 2) & 1  != 0)
		{
			drawerError("In mode DRAWER_DRAW_STRIPS there should be even number of vertices");
		}
		connectEachVertexWithNext(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_DRAW_POINTS)
	{
		drawEachVertex(&contextVerticesVector, contextPixelBuffer);
	}
	else if(contextDrawType == DRAWER_DRAW_RECTANGLES_LINES)
	{
		if((contextVerticesVector.length % 4))
		{
			drawerError("In mode DRAWER_DRAW_RECTANGLES there should be n packs of 4 vertices, representing each square");
		}
		drawRectangles(&contextVerticesVector, contextPixelBuffer, false);
		
	}
	else if(contextDrawType == DRAWER_DRAW_TRIANGLES)
	{
		drawTriangles(&contextVerticesVector, vstate);
	}
	Vector_Delete(&contextVerticesVector);

	clock_t difference = clock() - beforeAtBeginDraw;
  	int msec = difference * 1000 / CLOCKS_PER_SEC;
  	//printf("It took %d ms to draw freakin triangles\n", msec);
  	if(msec > max_draw_time)
  	{
  		max_draw_time = msec;
  	}
}
*/
/*
	draw2D() : Draws pixels from vertices in 2D

*/


//#endif

