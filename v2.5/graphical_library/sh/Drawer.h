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
#include "../maths/polygon_clipping.h"
#include "../maths/sorting_algorithms.h"
#include "../maths/triangle.h"
#include "shapeDrawing.h"
#include "Rendering.h"
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

	contextVerticesVector = Vector_create(sizeof(struct DrawVertexData));
	contextPixelBuffer = pixelBuffer;
	
	contextDrawType = type;
}   
void shDrawDataColor(Uint32 color)
{
	contextNextVertexColor = color;
}

void shDrawDataVertex2(float positionX, float positionY)
{
	struct DrawVertexData vertexData = { {positionX, positionY}, contextNextVertexColor };
	Vector_Push_back(&contextVerticesVector, &vertexData);
}
void shDrawDataVertex4(float positionX, float positionY, float positionZ, float positionW)
{
	//printf("w : %f\n", positionW);
	// if w is negative then just don't draw 
	if(positionW < 0)
	{
		return;
	}
	positionZ /= positionW;
	//printf("z : %f\n", positionZ);
	//printf("w=%f, z=%f\n", positionW, positionZ);
	int halfWidth = contextPixelBuffer->width / 2;
	int halfHeight = contextPixelBuffer->height / 2;

	struct DrawVertexData vertexData = { {positionX / positionW, positionY / positionW}, contextNextVertexColor };

	//vertexData.position = planeCoordinatesToScreenCoordinates(vertexData.position, contextPixelBuffer);

	/*     /
	    |-/---|
		|/    |
		/     |
	   /|-----|


	*/

	//vertexData.position.x = (vertexData.position.x * 1.0f / (2.0f * positionW));
    //vertexData.position.y = (vertexData.position.y * 1.0f / (2.0f * positionW));

    //Vector2 screen_coords = planeCoordinatesToScreenCoordinates(vertexData.position, contextPixelBuffer);

    vertexData.position.x = (vertexData.position.x * (float)contextPixelBuffer->width) / (2.0f * positionW) + halfWidth;
    vertexData.position.y = (vertexData.position.y * (float)contextPixelBuffer->height) / (2.0f * positionW) + halfHeight;

    

    vertexData.position = screenCoordinatesToPlaneCoordinates(vertexData.position, contextPixelBuffer);

    

    /*if(isinf(vertexData.position.x) && vertexData.position.x <= -FLT_MAX && isinf(vertexData.position.y) && vertexData.position.y <= -FLT_MAX)
    {
    	
    	printf("stopping\n");
    	return;
    }*/
    //printf("Vertex : [%f, %f] -> [%f ; %f]\n", screen_coords.x, screen_coords.y, vertexData.position.x, vertexData.position.y);

    //vertexData.position = screenCoordinatesToPlaneCoordinates(vertexData.position, contextPixelBuffer);

    

    Vector_Push_back(&contextVerticesVector, &vertexData);
    //Vector_Push_back(&contextVerticesVector, &vertexData2);

	
}

void shEndDraw()
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
	else if(contextDrawType == DRAWER_RENDER_RECTANGLES_LINES)
	{
		if((contextVerticesVector.length % 4))
		{
			drawerError("In mode DRAWER_RENDER_RECTANGLES there should be n packs of 4 vertices, representing each square");
		}
		drawRectangles(&contextVerticesVector, contextPixelBuffer, false);
		
	}
	else if(contextDrawType == DRAWER_RENDER_TRIANGLES)
	{
		drawTriangles(&contextVerticesVector, contextPixelBuffer);
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

/*
	draw2D() : Draws pixels from vertices in 2D

*/


#endif