#pragma once


#ifndef __DRAWING_H__
#define __DRAWING_H__



/*
	DRAWER_DRAW_LINKED_ALL_VERTICES : Connects each single pixel with each single other pixel
	DRAWER_DRAW_STRIPS : Connects each pixel with the pixel, forming a strip
	DRAWER_DRAW_POINTS : Doesn't connect anything, only draws a point for each vertex
	DRAWER_DRAW_RECTANGLES_LINES : Connect each vertex of each rectangle
	DRAWER_DRAW_TRIANGLES : Connect each vertex of each triangle
*/

#define DRAWER_DRAW_LINKED_ALL_VERTICES (char)0
#define DRAWER_DRAW_STRIPS (char)1
#define DRAWER_DRAW_POINTS (char)2
#define DRAWER_DRAW_RECTANGLES_LINES (char)3
#define DRAWER_DRAW_TRIANGLES (char)4

#include "drawContext.h"
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



void shSetDrawLevel(Uint32 type)
{
	draw_level = type;
}

void shSetInterpolationMode(Uint32 type)
{
	drawer_interpolation_mode = type;
}

bool processInputVertexData(InputVertexData* InputVertexData, DrawVertexData* outputDrawVertexData)
{
	//printf("z = %f\n", InputVertexData->position.z);
	InputVertexData->position.z /= InputVertexData->position.w;
	//DrawVertexData vertexData;
	DrawVertexData vertexData = { 
		{
			InputVertexData->position.x / InputVertexData->position.w, 
			InputVertexData->position.y / InputVertexData->position.w
		},
	 InputVertexData->color, InputVertexData->position.z, InputVertexData->position.w};

	*outputDrawVertexData = vertexData;

    // if w is negative then just don't draw 
	if(InputVertexData->position.w <= 0)
	{
		return false;
	}

    return true;

}

// in building...
bool depthTestDepthSortFunc(void* candidate, void* currentBest)
{
	return ((DrawVertexData*)(candidate))[0].depth > ((DrawVertexData*)(currentBest))[0].depth;
}

/*
	Renders a list of vertices of type defined by vertices and vertices_len 

	vertexShader : A function that will be executed for each vertex of the draw, passed arguments to it is the position of the current vertex
	It has to return the new 4d position and the color through the InputVertexData struct
	Perspective division will be automaticly done
*/

void shDrawVertices(char type, float* vertices, ulong vertices_len, PixelBuffer* pixelBuffer, InputVertexData (*vertexShader)(Vector3, ulong))
{
	
	Vector verticesVec = Vector_create(sizeof(DrawVertexData));
	
	

	contextPixelBuffer = pixelBuffer;
	Uint32 vertexCounter = 0;
	if(type == DRAWER_DRAW_TRIANGLES)
	{
		Vector2 clipping_polygon[] = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
		Vector trianglesVec = Vector_create(sizeof(DrawVertexData) * 3);
		if(vertices_len % 9 != 0)
		{
			printf("<shDrawVertices:DRAWER_DRAW_TRIANGLES> Error : Data should be backs of 3d triangles (3 vertices of 3 dimensions per triangle)\n");
			exit(-1);
		}

		for(ulong i = 0 ; i < vertices_len / 3 ; i++)
		{
			const ulong currLine = 3 * i;
			Vector3 position = { vertices[ currLine], vertices[ currLine + 1 ], vertices[ currLine + 2]  };
			InputVertexData VertexInClipSpace = vertexShader(position, vertexCounter);
			DrawVertexData data;

			if(processInputVertexData(&VertexInClipSpace, &data) == false)
			{
			}
			else
			{
				Vector_Push_back(&verticesVec, &data);
				vertexCounter++;
			}
			
		}
		if(verticesVec.length >= 1)
		{
			VerticesState vstate;
			vstate = makeVerticesState((DrawVertexData*)verticesVec.data, verticesVec.length, false);
			if(!checkIfVerticesColorIsDifferent((DrawVertexData*)verticesVec.data, verticesVec.length))
			{
				vstate.allVerticesHaveSameColor = true;
			}

			for(ulong i = 0 ; i < verticesVec.length / 3 ; i++)
			{
				DrawVertexData triangle[] = {
					*(DrawVertexData*) Vector_element(&verticesVec, 3 * i), 
					*(DrawVertexData*) Vector_element(&verticesVec, 3 * i + 1), 
					*(DrawVertexData*) Vector_element(&verticesVec, 3 * i + 2), 
				};

				Vector_Push_back(&trianglesVec, &triangle);
				
			}

			// in building
			if(DRAWER_ENABLE_TRIANGLE_DEPTH_TEST)
			{
				sortFunc(trianglesVec.data, trianglesVec.length, 3 * sizeof(DrawVertexData), depthTestDepthSortFunc);
			}
			
			drawTriangles(&trianglesVec, vstate);
			Vector_Delete(&trianglesVec);
		}
		

	}
	else
	{
		printf("<shDrawVertices> Error : Type is not supported or doesn't exist\n");
		exit(-1);
	}
	Vector_Delete(&verticesVec);
}



#endif