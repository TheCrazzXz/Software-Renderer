#pragma once

#ifndef __CUBE_H__
#define __CUBE_H__

#include "maths/maths.h"
#include "sh/Drawer.h"

float flat_plane_vertices_counter_clockwise[] = 
{
	// Bottom
	-1, -1, 1,
	-1, -1, -1,
	1, -1, -1,
	1, -1, 1
};
float flat_plane_vertices[] = 
{
	// Bottom
	-1, -1, 1,
	1, -1, 1,
	1, -1, -1,
	-1, -1, -1
};
float flat_plane_vertices_old[] = 
{
	-1, 1, 1,
	1, 1, 1,
	1, -1, 1,
	-1, -1, 1
};
float flat_plane_vertices_old2[] = 
{
	-1, 1, 0,
	1, 1, 0,
	1, -1, 0,
	-1, -1, 0
};

float cube_vertices[] = {
		// Front
		-1, 1, -1,
		-1, -1, -1,
		1, -1, -1,
		1, 1, -1,

		// Back
		-1, 1, 1, 
		-1, -1, 1,
		1, -1, 1, 
		1, 1, 1,

		// Left
		-1, 1, -1,
		-1, -1, -1,
		-1, -1, 1, 
		-1, 1, 1,

		// Right
		1, 1, -1,
		1, -1, -1,
		1, -1, 1, 
		1, 1, 1,

		// Bottom
		-1, -1, 1,
		-1, -1, -1,
		1, -1, -1,
		1, -1, 1,

		// Top
		-1, 1, 1,
		-1, 1, -1,
		1, 1, -1,
		1, 1, 1,
	};

void renderShape(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, float* vertices, float vertices_size)
{
	Matrix4 transformationMatrix = createTransformationMatrix(position, rotation, scale);
	beginDraw(DRAWER_RENDER_RECTANGLES_LINES, pixelBuffer);
	for(int i = 0 ; i < vertices_size / sizeof(float) / 3 ; i += 1)
	{
		Vector4 currentVertice = Vec4_create(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2], 1);
		Vector4 tranformedCurrentVertice4 = Matrix4_multiplyVector(&transformationMatrix, currentVertice);
		tranformedCurrentVertice4 = Matrix4_multiplyVector(viewMatrix, tranformedCurrentVertice4);

		Vector4 projectedCurrentVertice = projectVertex(tranformedCurrentVertice4);
	    
		//printf("Pixel re : %f\n", );

		if((i % 2))
		{
			drawDataColor(pixelRGB(255, 0, 0));
		}
		else if((i % 3))
		{
			drawDataColor(pixelRGB(0, 255, 0));
		}
		else
		{
			drawDataColor(pixelRGB(0, 0, 255));
		}
	    
		drawDataVertex4(projectedCurrentVertice.x, projectedCurrentVertice.y, projectedCurrentVertice.z, projectedCurrentVertice.w);

		
	}
	endDraw();
}

void cube(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer)
{
	renderShape(position, rotation, scale, viewMatrix, pixelBuffer, cube_vertices, sizeof(cube_vertices));
}
void flatPlane(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer)
{
	renderShape(position, rotation, scale, viewMatrix, pixelBuffer, flat_plane_vertices, sizeof(flat_plane_vertices));
}
	

#endif