#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

#include "../graphical_library/memory/memutils.h"

struct Model
{
	float* vertices;
	ulong vertices_length;
};

struct Model createModel(float* vertices, ulong vertices_length)
{
	if(vertices == NULL)
	{
		printf("<createModel> Error : Vertices is a NULL pointer\n");
		exit(-1);
	}
	if(vertices_length == 0)
	{
		printf("<createModel> Error : Can't be no vertices\n");
	}
	struct Model model;
	model.vertices = vertices;
	model.vertices_length = vertices_length;
	return model;
}

typedef struct Model Model;

#endif