#pragma once

#include "PixelBuffer.h"
#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

#define INTERPOLATION_VERTICES_ALL_EQUALS_CHECK true

#include <stdbool.h>

#include "drawContext.h"

bool checkIfVerticesColorIsDifferent(DrawVertexData* vertices, ulong vertices_len)
{
	bool oneVertexColorIsDiffrent = false;
	for(ulong i = 0 ; i < vertices_len ; i++)
	{
		if(!Vectors4_equals(&vertices[i].color, &vertices[0].color))
		{
			oneVertexColorIsDiffrent = true;
			break;
		}
	}
	return oneVertexColorIsDiffrent;
}



/*
	Interpolates vertices.length vertices

*/
Vector4 interpolateColourN(Vector2 point, DrawVertexData* vertices, ulong vertices_len)
{
	if(checkIfVerticesColorIsDifferent(vertices, vertices_len) == false)
	{
		return vertices[0].color;
	}
	Vector4 color;

	float colors_total_w = 0;

	for(ulong i = 0 ; i < vertices_len ; i++)
	{
		// weight of the current vertex
		float w = 1.0f / Vectors2_distance(&point, &vertices[i].position);
		//float w = 1.0f / vertices[i].weight;

		colors_total_w += w;

		color.x += w * vertices[i].color.x;
		color.y += w * vertices[i].color.y;
		color.z += w * vertices[i].color.z;
		color.w += w * vertices[i].color.w;

		
	}
	// dividing all color components by the sum of all the weights
	color.x /= colors_total_w;
	color.y /= colors_total_w;
	color.z /= colors_total_w;
	color.w /= colors_total_w;

	color.x = clampInt(color.x, 0, 255);
	color.y = clampInt(color.y, 0, 255);
	color.z = clampInt(color.z, 0, 255);
	color.w = clampInt(color.w, 0, 255);

	return color;

}

void interpolatedHorizontalLineAcceleratedDraw(int x0, int x1, int y, VerticesState vstate, struct PixelBuffer* pixelBuffer)
{
	unsigned long count = abs(x1 - x0);

	Uint32 *buf;
	Uint32* originalBuf;

	/* x0 --------------- x1 */
	if(x1 > x0)
	{
		buf = &pixelBuffer->buffer[y * pixelBuffer->width + x0];
		originalBuf = buf;
		while(count--) *buf++ = vec4UnitToPixelColor(interpolateColourN(screenCoordinatesToPlaneCoordinates(Vec2_create(x0 + buf - originalBuf, y), pixelBuffer), vstate.allVertices, vstate.allVertices_length));
	}
	/* x1 -------------- x0*/
	else
	{
		buf = &pixelBuffer->buffer[y * pixelBuffer->width + x1];
		originalBuf = buf;
		while(count--) *buf++ = vec4UnitToPixelColor(interpolateColourN(screenCoordinatesToPlaneCoordinates(Vec2_create(x1 + buf - originalBuf, y), pixelBuffer), vstate.allVertices, vstate.allVertices_length));
	}
}


bool pixelInterpolateScreen(int x, int y, VerticesState vstate, PixelBuffer* pixelBuffer)
{
	if(vstate.allVerticesHaveSameColor)
	{
		return pixelAtPosition(x, y, vec4UnitToPixelColor(vstate.allVertices[0].color), pixelBuffer);
	}
	Vector2 planeCoords = screenCoordinatesToPlaneCoordinates(Vec2_create(x, y), pixelBuffer);
	return pixelAtPosition(x, y, vec4UnitToPixelColor(interpolateColourN(planeCoords, vstate.allVertices, vstate.allVertices_length)), pixelBuffer);
}

bool pixelInterpolateNDC(float x, float y, VerticesState vstate, PixelBuffer* pixelBuffer)
{
	Vector2 screenCoords = screenCoordinatesToPlaneCoordinates(Vec2_create(x, y), pixelBuffer);
	pixelInterpolateScreen(screenCoords.x, screenCoords.y, vstate, pixelBuffer);
}

#endif