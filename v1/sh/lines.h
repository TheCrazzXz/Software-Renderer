#pragma once

#ifndef __LINES_H__
#define __LINES_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "interpolation.h"

void plotLine(float x0, float y0, float x1, float y1, PixelBuffer* pixelBuffer);

void plotLineLow(float x0, float y0, float x1, float y1, PixelBuffer* pixelBuffer)
{
	float dx = x1 - x0;
    float dy = y1 - y0;
    float yi = 1;
    if(dy < 0)
    {
    	yi = -1;
        dy = -dy;
    }
    
    float D = (2 * dy) - dx;
    float y = y0;
   	for(float x = x0 ; x <= x1 ; x++)
   	{
		renderInterpolateAtPosition((int)x, (int)y, pixelBuffer);

        if(D > 0)
        {
        	y = y + yi;
            D = D + (2 * (dy - dx));
        }
           
        else
        {
            D = D + 2*dy;
        }
   	}
        
}
    

void plotLineHigh(float x0, float y0, float x1, float y1, PixelBuffer* pixelBuffer)
{
	float dx = x1 - x0;
    float dy = y1 - y0;
    float xi = 1;
    if(dx < 0)
    {
    	xi = -1;
        dx = -dx;
    }
    
    float D = (2 * dx) - dy;
    float x = x0;

    for(float y = y0 ; y < y1 ; y++)
    {
    	renderInterpolateAtPosition((int)x, (int)y, pixelBuffer);
        if(D > 0)
        {
        	x = x + xi;
            D = D + (2 * (dx - dy));
        }
            
        else
        {
            D = D + 2*dx;
        }
    }
        
}
    

void plotLine(float x0, float y0, float x1, float y1, PixelBuffer* pixelBuffer)
{
	if((abs(y1 - y0)) < (abs(x1 - x0)))
    {
    	if(x0 > x1)
    	{
    		
    		plotLineLow(x1, y1, x0, y0, pixelBuffer);
    	}
        else
        {
            plotLineLow(x0, y0, x1, y1, pixelBuffer);
        }
    }
        
    else
    {
    	if(y0 > y1)
        {
            plotLineHigh(x1, y1, x0, y0, pixelBuffer);
        }
        else
        {
        	plotLineHigh(x0, y0, x1, y1 , pixelBuffer);
            
        }
    }
}

#endif