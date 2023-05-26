#pragma once

#ifndef __LINE_CLIPPING_H__
#define __LINE_CLIPPING_H__


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

#endif