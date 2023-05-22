#pragma once

#ifndef __LINES_H__
#define __LINES_H__

#include "PixelBuffer.h"
#include "../memory/vector.h"
#include "../maths/vec.h"
#include "../maths/maths.h"
#include "interpolation.h"

#include <assert.h>

#define LINE_CLIPPING false

void plotLine(float x0, float y0, float x1, float y1, PixelBuffer* pixelBuffer);

/*
    checkDirection :
    0 : <-
    1 : ->

*/

void plotLineLow(float x0, float y0, float x1, float y1, bool checkDirection, PixelBuffer* pixelBuffer)
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
    // just no, it would make the program do a huge loop
    if((y1 < 0 && x1 < 0) || (y0 < 0 && x0 < 0))
    {
        /*printf("<plotLine> didn't draw negative output\n");
        printf("\tx0 : %f, ", x0);
        printf("\ty0 : %f, ", y0);
        printf("\tx1 : %f, ", x1);
        printf("\ty1 : %f\n", y1);*/
        return;
    }

	if((abs(y1 - y0)) < (abs(x1 - x0)))
    {
        /*
            x1 ---- x0
        */
    	if(x0 > x1)
    	{
    		if((x0 > pixelBuffer->width || x1 < 0) && LINE_CLIPPING)
            {
                return;
            }
    		plotLineLow(x1, y1, x0, y0, false, pixelBuffer);
    	}
        else
        {
            if((x1 > pixelBuffer->width || x0 < 0) && LINE_CLIPPING)
            {
                return;
            }
            plotLineLow(x0, y0, x1, y1, true, pixelBuffer);
        }
    }
        
    else
    {
    	if(y0 > y1)
        {
            if((y0 > pixelBuffer->height || y1 < 0) && LINE_CLIPPING)
            {
                return;
            }
            plotLineHigh(x1, y1, x0, y0, pixelBuffer);
        }
        else
        {
            if( (y1 > pixelBuffer->height || y0 < 0) && LINE_CLIPPING)
            {
                return;
            }
        	plotLineHigh(x0, y0, x1, y1 , pixelBuffer);
            
        }
    }
}

#endif