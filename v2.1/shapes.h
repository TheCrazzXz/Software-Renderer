#pragma once

#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "maths/maths.h"
#include "sh/Drawer.h"

// as triangles
float flat_plane_vertices[] = 
{

	-1, -1, -1,
	1, -1, 1,
	-1, -1, 1,

	-1, -1, -1,
	1, -1, -1,
	1, -1, 1
};

float triangle32_vertices[] = {
    0.0f, 1.0f, 0.0 ,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,
	0.0f, -1.0f, -1.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f,
	0.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f
};

// as triangles
float cube_vertices[] = {
	 

	// Top
	-1, 1, -1,
	1, 1, 1,
	-1, 1, 1,

	-1, 1, -1,
	1, 1, -1,
	1, 1, 1,

	// Bottom
	-1, -1, -1,
	1, -1, 1,
	-1, -1, 1,

	-1, -1, -1,
	1, -1, -1,
	1, -1, 1,

	// Left
	-1, 1, 1,
	-1, 1, -1,
	-1, -1, -1,

	-1, 1, 1,
	-1, -1, -1,
	-1, -1, 1,

	// Right
	1, 1, 1,
	1, 1, -1,
	1, -1, -1,

	1, 1, 1,
	1, -1, -1,
	1, -1, 1,

	// Back
	-1, 1, -1,
	1, -1, -1,
	-1, -1, -1,

	-1, 1, -1,
	1, 1, -1,
	1, -1, -1,
	

	// Front
	-1, 1, 1,
	1, -1, 1,
	-1, -1, 1,

	-1, 1, 1,
	1, 1, 1,
	1, -1, 1
		
	};
char fun_offset = 0;
void renderShape(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, float* vertices, float vertices_size, Uint32 color)
{
	//fun_offset += 1;
	Matrix4 transformationMatrix = createTransformationMatrix(position, rotation, scale);
	beginDraw(DRAWER_RENDER_TRIANGLES, pixelBuffer);
	drawDataColor(color);
	for(int i = 0 ; i < vertices_size / sizeof(float) / 3 ; i += 1)
	{

		Vector4 currentVertice = Vec4_create(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2], 1);
		Vector4 tranformedCurrentVertice4 = Matrix4_multiplyVector(&transformationMatrix, currentVertice);
		tranformedCurrentVertice4 = Matrix4_multiplyVector(viewMatrix, tranformedCurrentVertice4);

		Vector4 projectedCurrentVertice = projectVertex(tranformedCurrentVertice4);
	    
		//printf("Pixel re : %f\n", );
	    
		drawDataVertex4(projectedCurrentVertice.x, projectedCurrentVertice.y, projectedCurrentVertice.z, projectedCurrentVertice.w);

		
	}
	endDraw();
}

void cube(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, Uint32 color)
{
	renderShape(position, rotation, scale, viewMatrix, pixelBuffer, cube_vertices, sizeof(cube_vertices), color);
}
void flatPlane(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, Uint32 color)
{
	renderShape(position, rotation, scale, viewMatrix, pixelBuffer, flat_plane_vertices, sizeof(flat_plane_vertices), color);
}
void triangle3d(Vector3 position, Vector3 rotation, Vector3 scale, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, Uint32 color)
{
	renderShape(position, rotation, scale, viewMatrix, pixelBuffer, triangle32_vertices, sizeof(triangle32_vertices), color);
}
	

#endif