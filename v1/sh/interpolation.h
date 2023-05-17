#pragma once

#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

#include "drawContext.h"

void renderInterpolateAtPosition(int x, int y, PixelBuffer* pixelBuffer)
{
	Vector2 point = Vec2_create(x, y);

	float verticesDist = Vectors2_distance(&contextVertex0.position, &contextVertex1.position);

	float Vert0Interpolant = Vectors2_distance(&point, &contextVertex0.position) / verticesDist;
	float Vert1Interpolant = Vectors2_distance(&point, &contextVertex1.position) / verticesDist;

	int color_x = (int)(COLOR_RED(contextVertex0.color) * Vert0Interpolant + COLOR_RED(contextVertex1.color) * Vert1Interpolant);
	int color_y = (int)(COLOR_GREEN(contextVertex0.color) * Vert0Interpolant + COLOR_GREEN(contextVertex1.color) * Vert1Interpolant);
	int color_z = (int)(COLOR_BLUE(contextVertex0.color) * Vert0Interpolant + COLOR_BLUE(contextVertex1.color) * Vert1Interpolant);

	pixelAtPosition((int)x, (int)y, pixelRGB(color_x, color_y, color_z), pixelBuffer);
}

#endif