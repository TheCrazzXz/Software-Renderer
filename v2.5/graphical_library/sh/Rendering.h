#pragma once

#ifndef __RENDERING_H__
#define __RENDERING_H__

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

#define TRIANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING false
#define RECTANGLE_DRAWING_OUT_OF_BOUNDS_CHECKING true

#define RENDER_ENABLE_TRIANGLE_CULLING false

bool wireframeMode = false;

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

void shSetWireframeMode(bool enabled)
{
	wireframeMode = enabled;
}

bool processRenderVertexData(struct RenderVertexData* renderVertexData, struct DrawVertexData* outputDrawVertexData)
{

	renderVertexData->position.z /= renderVertexData->position.w;
	//printVector4(&renderVertexData->position);
	//printf("w=%f, z=%f\n", positionW, positionZ);

	int halfWidth = contextPixelBuffer->width / 2;
	int halfHeight = contextPixelBuffer->height / 2;

	//struct DrawVertexData vertexData;
	struct DrawVertexData vertexData = { 
		{
			renderVertexData->position.x / renderVertexData->position.w, 
			renderVertexData->position.y / renderVertexData->position.w
		},
	 renderVertexData->color};

    outputDrawVertexData->position.x = (vertexData.position.x * (float)contextPixelBuffer->width) / (2.0f * renderVertexData->position.w) + halfWidth;
    outputDrawVertexData->position.y = (vertexData.position.y * (float)contextPixelBuffer->height) / (2.0f * renderVertexData->position.w) + halfHeight;

    outputDrawVertexData->color = vertexData.color;
    outputDrawVertexData->depth = renderVertexData->position.z;
    outputDrawVertexData->position = screenCoordinatesToPlaneCoordinates(outputDrawVertexData->position, contextPixelBuffer);


    // if w is negative then just don't draw 
	if(renderVertexData->position.w <= 0)
	{
		return false;
	}

    return true;

}

// in building...
bool cullingDepthSortFunc(void* candidate, void* currentBest)
{
	return ((struct DrawVertexData*)(candidate))[0].depth > ((struct DrawVertexData*)(currentBest))[0].depth;
}

/*
	Renders a list of vertices of type defined bu vertices and vertices_len 

	vertexShader : A function that will be executed for each vertex of the draw, passed arguments to it is the position of the current vertex
	It has to return the new 4d position and the color through the RenderVertexData struct
	Perspective division will be automaticly done
*/

void shRenderVertices(char type, float* vertices, ulong vertices_len, PixelBuffer* pixelBuffer, RenderVertexData (*vertexShader)(Vector3, ulong))
{
	Vector verticesVec = Vector_create(sizeof(struct DrawVertexData));
	ulong vertexCounter = 0;
	contextPixelBuffer = pixelBuffer;
	if(type == DRAWER_RENDER_TRIANGLES)
	{
		Vector trianglesVec = Vector_create(sizeof(struct DrawVertexData) * 3);
		if(vertices_len % 9 != 0)
		{
			printf("<shRenderVertices:DRAWER_RENDER_TRIANGLES> Error : Data should be backs of 3d triangles (3 vertices of 3 dimensions per triangle)\n");
			exit(-1);
		}

		for(ulong i = 0 ; i < vertices_len / 3 ; i++)
		{
			ulong currLine = 3 * i;
			Vector3 position = { vertices[ currLine], vertices[ currLine + 1 ], vertices[ currLine + 2]  };
			RenderVertexData VertexInClipSpace = vertexShader(position, vertexCounter);
			struct DrawVertexData data;

			if(processRenderVertexData(&VertexInClipSpace, &data) == false)
			{
			}
			else
			{
				vertexCounter++;
				Vector_Push_back(&verticesVec, &data);
			}
			
		}

		for(ulong i = 0 ; i < verticesVec.length / 3 ; i++)
		{
			struct DrawVertexData triangle[] = {
				*(struct DrawVertexData*) Vector_element(&verticesVec, 3 * i), 
				*(struct DrawVertexData*) Vector_element(&verticesVec, 3 * i + 1), 
				*(struct DrawVertexData*) Vector_element(&verticesVec, 3 * i + 2), 
			};
			Vector_Push_back(&trianglesVec, &triangle);
		}

		// in building
		if(RENDER_ENABLE_TRIANGLE_CULLING)
		{
			sortFunc(trianglesVec.data, trianglesVec.length, 3 * sizeof(struct DrawVertexData), cullingDepthSortFunc);
		}
		
		for(ulong i = 0 ; i < trianglesVec.length ; i++)
		{
			struct DrawVertexData* triangle = (Vector_element(&trianglesVec, i));
			//printVector3(&position);
			
			/*if(stopped >= 3)
			{
				continue;
			}*/

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
			if(wireframeMode)
			{
				connectTriangle(triangle, contextPixelBuffer);
			}
			else
			{
				fillTriangle(triangle, pixelBuffer);
			}
			
		}
		Vector_Delete(&trianglesVec);


	}
	else
	{
		printf("<shRenderVertices> Error : Type is not supported or doesn't exist\n");
		exit(-1);
	}
	Vector_Delete(&verticesVec);
	
}



#endif