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

#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/*
	DRAWER_RENDER_LINKED_ALL_VERTICES : Connects each single pixel with each single other pixel
	DRAWER_RENDER_STRIPS : Connects each pixel with the pixel, forming a strip
	DRAWER_RENDER_POINTS : Doesn't connect anything, only draws a point for each vertex
	DRAWER_RENDER_RECTANGLES_LINES : Connect each vertex of each rectangle
	DRAWER_RENDER_TRIANGLES : Connect each vertex of each triangle
*/

#define DRAWER_RENDER_LINKED_ALL_VERTICES (char)0
#define DRAWER_RENDER_STRIPS (char)1
#define DRAWER_RENDER_POINTS (char)2
#define DRAWER_RENDER_RECTANGLES_LINES (char)3
#define DRAWER_RENDER_TRIANGLES (char)4

#define TRIANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING true
#define RECTANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING true

int max_draw_time = 0; // The time that the object  

typedef uint32_t Uint32;
clock_t beforeAtBeginDraw;


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

bool lessClockWise(Vector2 a, Vector2 b, Vector2 center)
{
    if (a.x - center.x >= 0 && b.x - center.x < 0)
        return true;
    if (a.x - center.x < 0 && b.x - center.x >= 0)
        return false;
    if (a.x - center.x == 0 && b.x - center.x == 0) {
        if (a.y - center.y >= 0 || b.y - center.y >= 0)
            return a.y > b.y;
        return b.y > a.y;
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
    if (det < 0)
        return true;
    if (det > 0)
        return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
    int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
}

void clockwiseTriangleVertices(struct DrawVertexData triangle[3])
{
	Vector2 center;
	center.x = (triangle[0].position.x + triangle[1].position.x + triangle[2].position.x) / 3;
	center.y = (triangle[0].position.y + triangle[1].position.y + triangle[2].position.y) / 3;
	//planPixel(center.x, center.y, 0xffdeffff, contextPixelBuffer);


	/*for(ulong i = 0 ; i < 3 - 1 ; i++)
	{
		if(lessClockWise(triangle[i], triangle[i + 1]))
		{
			struct DrawVertexData temp = triangle[i];
			triangle[i] = triangle[i + 1];
			triangle[i + 1] = temp;
		}
	}*/

	printf("Triangle center : \n");
	printVector2(&center);


}

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
		printf("Center : \n");
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

void fillTriangle(struct DrawVertexData triangle[], PixelBuffer* pixelBuffer)
{
	/*struct DrawVertexData triangle[3];
	memcpy(triangle, triangleToFill, 3 * sizeof(struct DrawVertexData));
	Matrix4 transform = getIdentityMatrix4();
	Vector3 axis = {0, 0, 1};
	transform = Matrix4_rotate(&transform, axis, M_PI);

	for(ulong i = 0 ; i < 3 ; i++)
	{
		Vector4 pos = {triangle[i].position.x, triangle[i].position.y, 0, 1};
		pos = Matrix4_multiplyVector(&transform, pos);
		triangle[i].position.x = pos.x;
		triangle[i].position.y = pos.y;
	} */

	float vertices_x[] = { triangle[0].position.x, triangle[1].position.x, triangle[2].position.x };
	float vertices_y[] = { triangle[0].position.y, triangle[1].position.y, triangle[2].position.y };
	
	

	// defining aabb
	float xmin = min(vertices_x, sizeof(vertices_x) / sizeof(float));
	float xmax = max(vertices_x, sizeof(vertices_x) / sizeof(float));

	float ymin = min(vertices_y, sizeof(vertices_y) / sizeof(float));

	float ymax = max(vertices_y, sizeof(vertices_y) / sizeof(float));

	contextVertex0 = triangle[0];
	contextVertex1 = triangle[1];

	//printf("pixels to draw : %f\n",  ((ymax - ymin) * (float)pixelBuffer->height));

	//contextVertex2 = triangle[1];
	// Loop through each horizontal scanline (each y coordinate horizontal pixel list in aabb)
	for(float y = ymin ; y < ymax ; y += y_advance * (1.0f / (float) pixelBuffer->height))
	{
		Vector2 point_screen;
		// Loop though each pixel of the horizontal scanline
		//printf("x = %f\n", x);
		for(float x = xmin ; x < xmax ; x += x_advance * (1.0f / (float) pixelBuffer->width))
		{
			Vector2 point = { x, y };
			if(PointInTriangle(point, triangle[0].position, triangle[1].position, triangle[2].position))
			{
				
				point_screen = planeCoordinatesToScreenCoordinates(point, pixelBuffer);
				pixelAtPosition(point_screen.x, point_screen.y, contextVertex0.color, pixelBuffer);

				/*for(float x_cpy = point_screen.x ; x_cpy < point_screen.x + x_advance ; x_cpy++)
				{
					//pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + x_cpy)] = pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + point_screen.x)];
					for(float y_cpy = point_screen.y ; y_cpy < point_screen.y + y_advance ; y_cpy++)
					{
						if(isInBoundsScreen(x_cpy, y_cpy, pixelBuffer))
						{
							printf("x_cpy : %f, y_cpy : %f\n", x_cpy, y_cpy);
							pixelBuffer->buffer[(int)(y_cpy*pixelBuffer->width + x_cpy)] = pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + point_screen.x)];
						}
						
					}
				}*/

			}
			
		}
		

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
    
void beginDraw(char type, PixelBuffer* pixelBuffer)
{
	beforeAtBeginDraw = clock();
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
	// if w is negative then just don't draw 
	if(positionW < 0)
	{
		return;
	}
	positionZ /= positionW;
	//printf("w=%f, z=%f\n", positionW, positionZ);
	int halfWidth = contextPixelBuffer->width / 2;
	int halfHeight = contextPixelBuffer->height / 2;

	struct DrawVertexData vertexData = { {positionX / positionW, positionY / positionW}, contextNextVertexColor };

	//vertexData.position = planeCoordinatesToScreenCoordinates(vertexData.position, contextPixelBuffer);

	//vertexData.position.x = (vertexData.position.x * 1.0f / (2.0f * positionW));
    //vertexData.position.y = (vertexData.position.y * 1.0f / (2.0f * positionW));

    vertexData.position.x = (vertexData.position.x * (float)contextPixelBuffer->width) / (2.0f * positionW) + halfWidth;
    vertexData.position.y = (vertexData.position.y * (float)contextPixelBuffer->height) / (2.0f * positionW) + halfHeight;

    vertexData.position = screenCoordinatesToPlaneCoordinates(vertexData.position, contextPixelBuffer);

    

    /*if(isinf(vertexData.position.x) && vertexData.position.x <= -FLT_MAX && isinf(vertexData.position.y) && vertexData.position.y <= -FLT_MAX)
    {
    	printf("Vertex : [%f ; %f] -> [%f ; %f]\n", positionX, positionY, vertexData.position.x, vertexData.position.y);
    	printf("stopping\n");
    	return;
    }*/

    //vertexData.position = screenCoordinatesToPlaneCoordinates(vertexData.position, contextPixelBuffer);

    

    Vector_Push_back(&contextVerticesVector, &vertexData);
    //Vector_Push_back(&contextVerticesVector, &vertexData2);

	
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