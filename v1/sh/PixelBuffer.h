#pragma once

#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#include <stdint.h>

typedef uint32_t Uint32;

#include <stdlib.h>
#include <stdio.h>

#include "../memory/memutils.h"

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




struct PixelBuffer createPixelBuffer(int width, int height, Uint32 color)
{
	struct PixelBuffer pixelBuffer;

	pixelBuffer.buffer = malloc(sizeof(Uint32) * width * height);

	memsetu32(pixelBuffer.buffer, color, width * height);

	pixelBuffer.width = width;
	pixelBuffer.height = height;

	return pixelBuffer;
}

void clearColor(struct PixelBuffer* pixelBuffer, Uint32 color)
{
	memsetu32(pixelBuffer->buffer, color, pixelBuffer->width * pixelBuffer->height);
}

void pixelAtPosition(int x, int y, Uint32 color, struct PixelBuffer* pixelBuffer)
{
	if(x > 0 && x < pixelBuffer->width && y > 0 && y < pixelBuffer->height)
	{
		pixelBuffer->buffer[y*pixelBuffer->width + x] = color;
	}
	
}

/*
	planPixel :
	x and y must € [-1, 1]

*/

void planPixel(float x, float y, Uint32 color, struct PixelBuffer* pixelBuffer)
{
	int pixelPosX = (int) (pixelBuffer->width * (x + 1) / 2);
	int pixelPosY = (int) (pixelBuffer->height * (y + 1) / 2);
	
	pixelAtPosition(pixelPosX, pixelPosY, color, pixelBuffer);
}


typedef struct PixelBuffer PixelBuffer;

#endif