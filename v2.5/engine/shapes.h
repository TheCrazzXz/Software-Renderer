#pragma once

#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "../graphical_library/maths/maths.h"
#include "../graphical_library/sh/Drawer.h"

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

float square_2d[] =
{
	-1, 1, -1,
	1, -1, -1,
	-1, -1, -1,
	
	-1, 1, -1,
	1, 1, -1,
	1, -1, -1
};

Model square_2d_Model = { square_2d, sizeof(square_2d) / sizeof(float) };

Model flatPlaneModel = { flat_plane_vertices, sizeof(flat_plane_vertices) / sizeof(float) };

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
	1, -1, 1,
	};

Model cubeModel = { cube_vertices, sizeof(cube_vertices) / sizeof(float) };
char fun_offset = 0;
	

#endif