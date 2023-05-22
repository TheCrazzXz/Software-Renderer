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
#define LINE_CLIPPING true

// returns if line should be drawn and if it should be, clips it to xn1, yn1, xn2, yn2
// input : float xmin, float ymin, float xmax, float ymax, float x1, float y1, float x2, float y2

// output : float* xn1, float* yn1, float* xn2, float* yn2
int liang_barsky_clipper(float xmin, float ymin, float xmax, float ymax,
                          float x1, float y1, float x2, float y2,
                          float* xn1, float* yn1, float* xn2, float* yn2) {
  // defining variables
  float p1 = -(x2 - x1);
  float p2 = -p1;
  float p3 = -(y2 - y1);
  float p4 = -p3;

  float q1 = x1 - xmin;
  float q2 = xmax - x1;
  float q3 = y1 - ymin;
  float q4 = ymax - y1;

  float posarr[5], negarr[5];
  int posind = 1, negind = 1;
  posarr[0] = 1;
  negarr[0] = 0;

  if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) {
      return -1;
  }
  if (p1 != 0) {
    float r1 = q1 / p1;
    float r2 = q2 / p2;
    if (p1 < 0) {
      negarr[negind++] = r1; // for negative p1, add it to negative array
      posarr[posind++] = r2; // and add p2 to positive array
    } else {
      negarr[negind++] = r2;
      posarr[posind++] = r1;
    }
  }
  if (p3 != 0) {
    float r3 = q3 / p3;
    float r4 = q4 / p4;
    if (p3 < 0) {
      negarr[negind++] = r3;
      posarr[posind++] = r4;
    } else {
      negarr[negind++] = r4;
      posarr[posind++] = r3;
    }
  }

  float rn1, rn2;
  rn1 = max(negarr, negind); // maximum of negative array
  rn2 = min(posarr, posind); // minimum of positive array

  if (rn1 > rn2)  { // reject
    return -2;
  }

  *xn1 = x1 + p2 * rn1;
  *yn1 = y1 + p4 * rn1; // computing new points

  *xn2 = x1 + p2 * rn2;
  *yn2 = y1 + p4 * rn2;

  return 1;
}

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
    
    int xmin = 0;
    int xmax = pixelBuffer->width;

    int ymin = 0;
    int ymax = pixelBuffer->height;

    float xn0;
    float xn1;

    float yn0;
    float yn1;

    if(LINE_CLIPPING)
    {
        if(liang_barsky_clipper(xmin, ymin, xmax, ymax, x0, y0, x1, y1, &xn0, &yn0, &xn1, &yn1) != 1)
        {
            return;
        }
    }

    

	if((fabs(yn1 - yn0)) < (fabs(xn1 - xn0)))
    {
        /*
            xn1 ---- xn0
        */
        if(xn0 > xn1)
        {
            if((xn0 > pixelBuffer->width || xn1 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineLow(xn1, yn1, xn0, yn0, false, pixelBuffer);
        }
        else
        {
            if((xn1 > pixelBuffer->width || xn0 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineLow(xn0, yn0, xn1, yn1, true, pixelBuffer);
        }
    }
        
    else
    {
        if(yn0 > yn1)
        {
            if((yn0 > pixelBuffer->height || yn1 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineHigh(xn1, yn1, xn0, yn0, pixelBuffer);
        }
        else
        {
            if( (yn1 > pixelBuffer->height || yn0 < 0) && ENTIRE_LINE_CLIPPING)
            {
                return;
            }
            plotLineHigh(xn0, yn0, xn1, yn1 , pixelBuffer);
            
        }
    }
}

#endif