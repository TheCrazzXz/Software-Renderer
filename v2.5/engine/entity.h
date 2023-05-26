#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_RENDERING_DISTANCE 50
#define RENDER_DISTANCE_CHECK true

#include "model.h"
#include "../graphical_library/maths/vec.h"
#include "../graphical_library/sh/Drawer.h"
#include "../graphical_library/maths/maths.h"

struct Entity
{
	Model model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Uint32 color_start;
	Uint32 color_end;
};

bool checkCubeCollision(Vector3 e1_pos, Vector3 e1_scale, Vector3 e2_pos, Vector3 e2_scale)
{
	if(fabs(e1_pos.x - e2_pos.x) < (e1_scale.x + e2_scale.x) * 2.0f)
	{
		if(fabs(e1_pos.y - e2_pos.y) < e1_scale.y + e2_scale.y)
		{
			if(fabs(e1_pos.z - e2_pos.z) / 2.0f < (e1_scale.z + e2_scale.z) + 1)
			{
				return true;
			}	
		}		
	}
	return false;
}

struct Entity createEntity(Model model, Vector3 position, Vector3 rotation, Vector3 scale, Uint32 color_start, Uint32 color_end)
{
	struct Entity entity;

	entity.model = model;
	entity.position = position;
	entity.rotation = rotation;
	entity.scale = scale;
	entity.color_start = color_start;
	entity.color_end = color_end;

	return entity;
}
Matrix4 currentTransformationMatrix;
Matrix4 currentViewMatrix;
struct Entity* currentEntity;

RenderVertexData entity_vertex_shader(Vector3 vertex, ulong i)
{
	Vector4 currentVertice = Vec4_create(vertex.x, vertex.y, vertex.z, 1);
	Vector4 tranformedCurrentVertice4 = Matrix4_multiplyVector(&currentTransformationMatrix, currentVertice);
	tranformedCurrentVertice4 = Matrix4_multiplyVector(&currentViewMatrix, tranformedCurrentVertice4);

	Vector4 projectedCurrentVertice = projectVertex(tranformedCurrentVertice4);

	Uint32 color;

	if(i % 2 == 0)
	{
		color = currentEntity->color_start;
	}
	else
	{
		color = currentEntity->color_end;
	}

	RenderVertexData renderVertexData = { projectedCurrentVertice, color };
	return renderVertexData;
    
}

void Entity_render(struct Entity* entity, Matrix4* viewMatrix, PixelBuffer* pixelBuffer, Vector3* camera )
{
	currentViewMatrix = *viewMatrix;
	if(Vectors3_distance(&entity->position, camera) <= MAX_RENDERING_DISTANCE || (RENDER_DISTANCE_CHECK == false))
	{
		currentEntity = entity;
		currentTransformationMatrix = createTransformationMatrix(entity->position, entity->rotation, entity->scale);
		shRenderVertices(DRAWER_RENDER_TRIANGLES, entity->model.vertices, entity->model.vertices_length, pixelBuffer, entity_vertex_shader);
	}
	
}

void Entity_render2D(struct Entity* entity, PixelBuffer* pixelBuffer)
{
		Matrix4 transformationMatrix = createTransformationMatrix(entity->position, entity->rotation, entity->scale);
		shBeginDraw(DRAWER_RENDER_TRIANGLES, pixelBuffer);
		
		for(int i = 0 ; i < entity->model.vertices_length / 2 ; i += 1)
		{

			Vector4 currentVertice = Vec4_create(entity->model.vertices[2 * i], entity->model.vertices[2 * i + 1], 0, 1);
			Vector4 tranformedCurrentVertice4 = Matrix4_multiplyVector(&transformationMatrix, currentVertice);
			//printf("Pixel re : %f\n", );
		    
			if(i % 2 == 0)
			{
				shDrawDataColor(entity->color_end);
			}
			else
			{
				shDrawDataColor(entity->color_start);
			}

			shDrawDataVertex2(tranformedCurrentVertice4.x, tranformedCurrentVertice4.y);

			
		}
		shEndDraw();
	
}

typedef struct Entity Entity;

#endif