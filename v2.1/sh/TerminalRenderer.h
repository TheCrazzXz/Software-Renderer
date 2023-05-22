/*
	If you desire to render a pixel buffer directly into the terminal rather than SDL there's this

*/

#pragma once

#ifndef __TERMINAL_RENDERER_H_
#define __TERMINAL_RENDERER_H_

#include <stdbool.h>
#include <string.h>
#include "PixelBuffer.h"

/*
	ROUGE BLEU
	BLUE BLUE

*/

void parseColor(Uint32 pixel, char* output, ulong output_max_size)
{
	const Uint32 BLUE_COMPONENT = (char)((pixel & 0x000000ff));
	const Uint32 GREEN_COMPONENT = (char)((pixel & 0x0000ff00) >> 8);
	const Uint32 RED_COMPONENT = (char)((pixel & 0x00ff0000) >> 16);

	if((RED_COMPONENT) == (char)0xFF) // Red component
	{
		printf("red");
		strncpy(output, "\033[1;31mA\033[0m", output_max_size);
	}
	else if((GREEN_COMPONENT) == (char)0xFF) // Green component
	{
		printf("green");
		strncpy(output, "\033[1;32mA\033[0m", output_max_size);
	}
	else if((BLUE_COMPONENT) == (char)0xFF) // Blue component
	{
		printf("blue");
		strncpy(output, "\033[1;34mA\033[0m", output_max_size);
	}
	else
	{
		strncpy(output, "A", output_max_size);
	}
}

void renderPixelBufferTerminal(PixelBuffer* pixelBuffer, bool colored)
{
	char coloredText[64];
	// Loop through each pixel from 0 to the width
	for(int j = 0 ; j < pixelBuffer->height ; j++) // Y
	{
		// Loop through each pixel from 0 to the height
		for(int i = 0 ; i < pixelBuffer->width ; i++) // X
		{
			// Pointer to the current pixel
			Uint32 pixel = (pixelBuffer->buffer[j*pixelBuffer->width + i]);

			// If pixel isn't black then render
			if(pixel != 0xff000000)
			{
				if(colored == true)
				{
					parseColor(pixel, coloredText, 64);
					printf("%s", coloredText);
				}
				else
				{
					printf("A");
				}
				
			}
			// If pixel is black just render a space
			else
			{
				printf(" ");
			}
			
		}
		// End of Y line
		printf("\n");
	}
}

#endif