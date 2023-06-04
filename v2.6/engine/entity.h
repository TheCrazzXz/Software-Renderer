#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_RENDERING_DISTANCE 10
#define RENDER_DISTANCE_CHECK false

#include "model.h"
#include "../graphical_library/maths/vec.h"
#include "../graphical_library/graphics/Drawer.h"
#include "../graphical_library/graphics/DataDrawing.h"
#include "../graphical_library/maths/maths.h"
#include "../graphical_library/maths/projection.h"

#include "../graphical_library/graphics/drawContext.h"

struct Entity
{
	Model model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector4 color_0;
	Vector4 color_1;
	Vector4 color_2;
};

bool checkCubeCollision(Vector3 e1_pos, Vector3 e1_scale, Vector3 e2_pos, Vector3 e2_scale)
{
	if(fabs(e1_pos.x - e2_pos.x) < (e1_scale.x + e2_scale.x))
	{
		if(fabs(e1_pos.y - e2_pos.y) < e1_scale.y + e2_scale.y)
		{
			if(fabs(e1_pos.z - e2_pos.z) < (e1_scale.z + e2_scale.z))
			{
				return true;
			}	
		}		
	}
	return false;
}

struct Entity createEntity(Model model, Vector3 position, Vector3 rotation, Vector3 scale, Vector4 color_0)
{
	struct Entity entity;

	entity.model = model;
	entity.position = position;
	entity.rotation = rotation;
	entity.scale = scale;
	entity.color_0 = color_0;
	//entity.color_1 = color_1;
	//entity.color_2 = color_2;

	return entity;
}
Matrix4 currentTransformationMatrix;
Matrix4 currentViewMatrix;
struct Entity* currentEntity;

InputVertexData entity_vertex_shader(Vector3 vertex, ulong i)
{
	Vector4 currentVertice = Vec4_create(vertex.x, vertex.y, vertex.z, 1);
	Vector4 tranformedCurrentVertice4 = Matrix4_multiplyVector(&currentTransformationMatrix, currentVertice);
	tranformedCurrentVertice4 = Matrix4_multiplyVector(&currentViewMatrix, tranformedCurrentVertice4);

	Vector4 projectedCurrentVertice = projectVertex(tranformedCurrentVertice4);

	Vector4 color;

	/*
		--- --- --- ---
	*/
	//printf("in shader\n");
	/*if(Vectors3_equals_copy(vertex, Vec3_create(-1, 1, -1)))
	{
		color = 0xff0000ff;
	}
	else if(Vectors3_equals_copy(vertex, Vec3_create(1, 1, -1)))
	{
		color = 0xff00ff00;
	}
	else if(Vectors3_equals_copy(vertex, Vec3_create(-1, -1, -1)))
	{
		color = 0xffff0000;
	}
	else if(Vectors3_equals_copy(vertex, Vec3_create(1, -1, -1)))
	{
		color = 0xffffffff;
	}*/

	/*if(i % 2 == 0)
	{
		color = currentEntity->color_1;
	}
	
	else if(i % 3 == 0)
	{
		color = currentEntity->color_2;
	}
	else
	{
		color = currentEntity->color_0;
	}*/
	color = currentEntity->color_0;
	

	InputVertexData InputVertexData = { projectedCurrentVertice, color };
	return InputVertexData;
    
}

void Entity_render(struct Entity* entity, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, Vector3* camera )
{
	currentViewMatrix = *viewMatrix;
	if(Vectors3_distance(&entity->position, camera) <= MAX_RENDERING_DISTANCE || (RENDER_DISTANCE_CHECK == false))
	{

		currentEntity = entity;
		currentTransformationMatrix = createTransformationMatrix(entity->position, entity->rotation, entity->scale);
		shDrawVertices(DRAWER_DRAW_TRIANGLES, entity->model.vertices, entity->model.vertices_length, pixelBuffer, entity_vertex_shader);
	}
	
}

typedef struct Entity Entity;

#endif