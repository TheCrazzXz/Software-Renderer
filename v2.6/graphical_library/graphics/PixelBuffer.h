#pragma once

#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#include <stdint.h>

#define PIXEL_COLOR_UNIT_COLOR_TO_SCREEN_COLOR(u) (char)(255 * u)

typedef uint32_t Uint32;

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <time.h> 

#define PIXEL_IS_IN_THE_SCREEN 1

#define PIXEL_IS_LEFT_BEFORE_THE_SCREEN -1
#define PIXEL_IS_RIGHT_AFTER_THE_SCREEN -2

#define PIXEL_IS_ABOVE_THE_SCREEN -3
#define PIXEL_IS_BELOW_THE_SCREEN -4


#include "../memory/memutils.h"
#include "../maths/vec.h"
struct PixelBuffer
{
	int width;
	int height;
	Uint32* buffer;
};

Uint32 pixelRGB(char r, char g, char b)
{
	Uint32 pixel = 0;
	pixel = (pixel & 0xFFFFFF00) |  b;
	pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
	pixel = (pixel & 0xFF00FFFF) | ((uint32_t)r << 16);
	pixel = (pixel & 0x00FFFFFF) | ((uint32_t)255 << 24);
	return pixel;
}
Uint32 pixelRBGA(char r, char g, char b, char a)
{
	Uint32 pixel = 0;
	pixel = (pixel & 0xFFFFFF00) |  b;
	pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
	pixel = (pixel & 0xFF00FFFF) | ((uint32_t)r << 16);
	pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
	return pixel;
}

/* 
	a   x
	- = -
	1   255

	x = 255 a / 1 = 255a
*/

Uint32 vec4UnitToPixelColor(Vector4 colorVector)
{
	return pixelRGB(PIXEL_COLOR_UNIT_COLOR_TO_SCREEN_COLOR(colorVector.x), PIXEL_COLOR_UNIT_COLOR_TO_SCREEN_COLOR(colorVector.y), PIXEL_COLOR_UNIT_COLOR_TO_SCREEN_COLOR(colorVector.z));
}



struct PixelBuffer createPixelBuffer(int width, int height, Uint32 color)
{
	struct PixelBuffer pixelBuffer;

	pixelBuffer.buffer = (Uint32*) malloc(sizeof(Uint32) * width * height);

	memsetu32(pixelBuffer.buffer, color, width * height);

	pixelBuffer.width = width;
	pixelBuffer.height = height;

	return pixelBuffer;
}

void resizePixelBuffer(int width, int height, Uint32 color, struct PixelBuffer* pixelBuffer)
{
	pixelBuffer->buffer = (Uint32*) realloc(pixelBuffer->buffer, sizeof(Uint32) * width * height);

	memsetu32(pixelBuffer->buffer, color, width * height);

	pixelBuffer->width = width;
	pixelBuffer->height = height;

}

void clearColor(struct PixelBuffer* pixelBuffer, Uint32 color)
{
	memsetu32(pixelBuffer->buffer, color, pixelBuffer->width * pixelBuffer->height);
}

bool isInBoundsPlan(float x, float y)
{
	return (x > -1 && x < 1 && y > -1 && y < 1);
}
bool isInBoundsScreen(int x, int y, struct PixelBuffer* pixelBuffer)
{
	return (x >= 0 && x <= pixelBuffer->width && y >= 0 && y <= pixelBuffer->height);
}

int isInBoundsScreenWidth(int x, struct PixelBuffer* pixelBuffer)
{
	if(x < 0)
	{
		return PIXEL_IS_LEFT_BEFORE_THE_SCREEN;
	}
	else if(x > pixelBuffer->width)
	{
		return PIXEL_IS_RIGHT_AFTER_THE_SCREEN;
	}
	else
	{
		return PIXEL_IS_IN_THE_SCREEN;
	}
}
int isInBoundsScreenHeight(int y, struct PixelBuffer* pixelBuffer)
{
	if(y < 0)
	{
		return PIXEL_IS_BELOW_THE_SCREEN;
	}
	else if(y > pixelBuffer->height)
	{
		return PIXEL_IS_ABOVE_THE_SCREEN;
	}
	else
	{
		return PIXEL_IS_IN_THE_SCREEN;
	}
}
/* Draws a strait horizontal line (y = x) with one and only one color for all the pixels*/
/* x0, x1 and y are screen coordinates */
void unicolorHorizontalLineAcceleratedDraw(int x0, int x1, int y, Uint32 uniformColor, struct PixelBuffer* pixelBuffer)
{
	
	/* x0 --------------- x1 */
	if(x1 > x0)
	{

		memsetu32(&pixelBuffer->buffer[y * pixelBuffer->width + x0], uniformColor, x1 - x0);
	}
	/* x1 -------------- x0*/
	else
	{
		memsetu32(&pixelBuffer->buffer[y * pixelBuffer->width + x1], uniformColor, x0 - x1);
	}

}

Vector2 screenCoordinatesToPlaneCoordinates(Vector2 vector, struct PixelBuffer* pixelBuffer)
{
	Vector2 converted = { 2 * vector.x / pixelBuffer->width - 1, 2 * vector.y / pixelBuffer->height - 1};
	return converted;
}
Vector2 planeCoordinatesToScreenCoordinates(Vector2 vector, struct PixelBuffer* pixelBuffer)
{
	Vector2 converted = { (float)((int) (pixelBuffer->width * (vector.x + 1) / 2)), (float)((int) (pixelBuffer->height * (vector.y + 1) / 2))};
	return converted;
}

void planeCoordinatesToScreenCoordinates2(float x, float y, int *nx, int *ny,  struct PixelBuffer* pixelBuffer)
{
	*nx = (int) (pixelBuffer->width * (x + 1) / 2),
	*ny = (int) (pixelBuffer->height * (y + 1) / 2);
}

bool pixelAtPosition(int x, int y, Uint32 color, struct PixelBuffer* pixelBuffer)
{
	if(isInBoundsScreen(x, y, pixelBuffer))
	{
		pixelBuffer->buffer[y*pixelBuffer->width + x] = color;
		return true;
	}
	return false;
	
}



/*
	NDCpixel :
	x and y must € [-1, 1]

*/

bool NDCpixel(float x, float y, Uint32 color, struct PixelBuffer* pixelBuffer)
{
	int pixelPosX = (int) (pixelBuffer->width * (x + 1) / 2);
	int pixelPosY = (int) (pixelBuffer->height * (y + 1) / 2);
	
	return pixelAtPosition(pixelPosX, pixelPosY, color, pixelBuffer);
}


typedef struct PixelBuffer PixelBuffer;

#endif