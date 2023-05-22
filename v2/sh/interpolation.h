#pragma once

#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

#include <stdbool.h>

#include "drawContext.h"

bool renderInterpolateAtPosition(int x, int y, PixelBuffer* pixelBuffer)
{
	// if they have the same color no need to interpolate
	if(contextVertex0.color == contextVertex1.color)
	{
		return pixelAtPosition((int)x, (int)y, contextVertex0.color, pixelBuffer);
	}

	Vector2 point = Vec2_create(x, y);

	float verticesDist = Vectors2_distance(&contextVertex0.position, &contextVertex1.position);

	float Vert0Interpolant = Vectors2_distance(&point, &contextVertex0.position) / verticesDist;
	float Vert1Interpolant = Vectors2_distance(&point, &contextVertex1.position) / verticesDist;

	int color_x = (int)(COLOR_RED(contextVertex0.color) * Vert0Interpolant + COLOR_RED(contextVertex1.color) * Vert1Interpolant);
	int color_y = (int)(COLOR_GREEN(contextVertex0.color) * Vert0Interpolant + COLOR_GREEN(contextVertex1.color) * Vert1Interpolant);
	int color_z = (int)(COLOR_BLUE(contextVertex0.color) * Vert0Interpolant + COLOR_BLUE(contextVertex1.color) * Vert1Interpolant);

	color_x = clampInt(color_x, 0, 255);
	color_y = clampInt(color_y, 0, 255);
	color_z = clampInt(color_z, 0, 255);

	return pixelAtPosition((int)x, (int)y, pixelRGB(color_x, color_y, color_z), pixelBuffer);
}
bool renderInterpolate3AtPosition(int x, int y, PixelBuffer* pixelBuffer)
{
	Vector2 point = Vec2_create(x, y);

	float verticesDist = Vectors2_distance(&contextVertex0.position, &contextVertex1.position);
	float verticesDist2 = Vectors2_distance(&contextVertex0.position, &contextVertex2.position);

	float Vert0Interpolant = Vectors2_distance(&point, &contextVertex0.position) / verticesDist;
	float Vert1Interpolant = Vectors2_distance(&point, &contextVertex1.position) / verticesDist;
	float Vert2Interpolant = Vectors2_distance(&point, &contextVertex2.position) / verticesDist;

	Vert2Interpolant = 0;

	int color_x = (int)(COLOR_RED(contextVertex0.color) * Vert0Interpolant + COLOR_RED(contextVertex1.color) * Vert1Interpolant + COLOR_RED(contextVertex2.color) * Vert2Interpolant);
	int color_y = (int)(COLOR_GREEN(contextVertex0.color) * Vert0Interpolant + COLOR_GREEN(contextVertex1.color) * Vert1Interpolant + COLOR_GREEN(contextVertex2.color) * Vert2Interpolant);
	int color_z = (int)(COLOR_BLUE(contextVertex0.color) * Vert0Interpolant + COLOR_BLUE(contextVertex1.color) * Vert1Interpolant + COLOR_BLUE(contextVertex2.color) * Vert2Interpolant);

	color_x = clampInt(color_x, 0, 255);
	color_y = clampInt(color_y, 0, 255);
	color_z = clampInt(color_z, 0, 255);


	return pixelAtPosition((int)x, (int)y, pixelRGB(color_x, color_y, color_z), pixelBuffer);
}

#endif