#pragma once

#ifndef __LINE_CLIPPING_H__
#define __LINE_CLIPPING_H__

typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

// ASSUME THAT xmax, xmin, ymax and ymin are global constants.

OutCode ComputeOutCode(double x, double y, float xmin, float xmax, float ymin, float ymax)
{
	OutCode code = INSIDE;  // initialised as being inside of clip window

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool CohenSutherlandLineClip(float* x0, float* y0, float *x1, float* y1, float xmin, float xmax, float ymin, float ymax)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode((*x0), (*y0), xmin, xmax, ymin, ymax);
	OutCode outcode1 = ComputeOutCode((*x1), (*y1), xmin, xmax, ymin, ymax);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

			// Now find the intersection point;
			// use formulas:
			//   slope = ((*y1) - (*y0)) / ((*x1) - (*x0))
			//   x = (*x0) + (1 / slope) * (ym - (*y0)), where ym is ymin or ymax
			//   y = (*y0) + slope * (xm - (*x0)), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = (*x0) + ((*x1) - (*x0)) * (ymax - (*y0)) / ((*y1) - (*y0));
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = (*x0) + ((*x1) - (*x0)) * (ymin - (*y0)) / ((*y1) - (*y0));
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = (*y0) + ((*y1) - (*y0)) * (xmax - (*x0)) / ((*x1) - (*x0));
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = (*y0) + ((*y1) - (*y0)) * (xmin - (*x0)) / ((*x1) - (*x0));
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				*x0 = x;
				*y0 = y;
				outcode0 = ComputeOutCode((*x0), (*y0), xmin, xmax, ymin, ymax);
			} else {
				*x1 = x;
				*y1 = y;
				outcode1 = ComputeOutCode((*x1), (*y1), xmin, xmax, ymin, ymax);
			}
		}
	}
	return accept;
}

bool lineClip(float* x0, float* y0, float *x1, float* y1, float xmin, float xmax, float ymin, float ymax)
{
	float x[2];
	float y[2];
  	if(!((*x0) < xmin && (*x1) < xmin) && !((*x0) > xmax && (*x1) > xmax))
  	{
  		if(!((*y0) < ymin && (*y1) < ymin) && !((*y0) > ymax && (*y1) > ymax))
	    {
	      x[0] = (*x0);
	      y[0] = (*y0);
	      x[1] = (*x1);
	      y[1] = (*y1);
	      int i = 1;
	      do
	      {
	        if(x[i] < xmin)
	        {
	          x[i] = xmin;
	          y[i] = (((*y1) - (*y0))/((*x1) - (*x0))) * (xmin - (*x0)) + (*y0);
	        }
	        else if(x[i] > xmax)
	        {
	          x[i] = xmax;
	          y[i] = (((*y1) - (*y0))/((*x1) - (*x0))) * (xmax - (*x0)) + (*y0);
	        }
	        if(y[i] < ymin)
	        {
	          y[i] = ymin;
	          x[i] = (((*x1) - (*x0))/((*y1) - (*y0)))*(ymin - (*y0)) + (*x0);
	        }
	        else if(y[i] > ymax)
	        {
	          y[i] = ymax;
	          x[i] = (((*x1) - (*x0))/((*y1) - (*y0))) * (ymax - (*y0)) + (*x0);
	        }
	        i++;
	      }
	      while(i <= 1);
	      
	      if(!(x[0] < xmin && x[1] < xmin) && !(x[0] > xmax && x[1] > xmax))
	      {
	      	(*x0) = x[0];
	      	(*x1) = x[1];

	      	(*y0) = y[0];
	      	(*y1) = y[1];
	      }
	      else
	      {
	      	return false;
	      }
	  	}
	}
}

#endif