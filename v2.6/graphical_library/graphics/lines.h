#pragma once

#ifndef __LINES_H__
#define __LINES_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "../maths/sorting_algorithms.h"
#include "interpolation.h"

#include <assert.h>

#define ENTIRE_LINE_CLIPPING false

void plotLineScreen(float x0, float y0, float x1, float y1, VerticesState vstate, PixelBuffer* pixelBuffer);



void plotLineLowScreen(float x0, float y0, float x1, float y1, VerticesState vstate, PixelBuffer* pixelBuffer)
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
        if(vstate.useOtherColor)
        {
            pixelAtPosition((int)x, (int)y, vec4UnitToPixelColor(vstate.otherColor), pixelBuffer);
        }
        else
        {
            pixelInterpolateScreen(x, y, vstate, pixelBuffer);
        }
       

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
    

void plotLineHighScreen(float x0, float y0, float x1, float y1, VerticesState vstate, PixelBuffer* pixelBuffer)
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
        if(vstate.useOtherColor)
        {
            pixelAtPosition(x, y, vec4UnitToPixelColor(vstate.otherColor), pixelBuffer);
        }
        else
        {
            pixelInterpolateScreen((int)x, (int)y, vstate, pixelBuffer);
        }

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

void plotLineScreen(float x0, float y0, float x1, float y1, VerticesState vstate, PixelBuffer* pixelBuffer)
{
	if((fabs(y1 - y0)) < (fabs(x1 - x0)))
    {
        /*
            x1 ---- x0
        */
        if(x0 > x1)
        {
            if((x0 > pixelBuffer->width || x1 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineLowScreen(x1, y1, x0, y0, vstate, pixelBuffer);
        }
        else
        {
            if((x1 > pixelBuffer->width || x0 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineLowScreen(x0, y0, x1, y1, vstate, pixelBuffer);
        }
    }
        
    else
    {
        if(y0 > y1)
        {
            if((y0 > pixelBuffer->height || y1 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineHighScreen(x1, y1, x0, y0, vstate, pixelBuffer);
        }
        else
        {
            if( (y1 > pixelBuffer->height || y0 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineHighScreen(x0, y0, x1, y1 , vstate, pixelBuffer);
            
        }
    }
}

#endif