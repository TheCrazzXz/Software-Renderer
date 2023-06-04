#pragma once

#ifndef __POLYGON_CLIPPER_H__
#define __POLYGON_CLIPPER_H__

#include "../memory/vector.h"
#include "../graphics/drawContext.h"
#include "vec.h"
#include <stdbool.h>

bool isInside(Vector2 p1, Vector2 p2, Vector2 q) {
  float R = (p2.x - p1.x) * (q.y - p1.y) - (p2.y - p1.y) * (q.x - p1.x);
  return (R <= 0);
}

Vector2 compute_intersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
  float x;
  float y;

  float m1;
  float b1;

  float m2;
  float b2;

  // if first line is vertical
  if (p2.x - p1.x == 0) {
    x = p1.x;

    // slope and intercept of second line
    m2 = (p4.y - p3.y) / (p4.x - p3.x);
    b2 = p3.y - m2 * p3.x;

    // y-coordinate of intersection
    y = m2 * x + b2;
  }
  // if second line is vertical
  else if (p4.x - p3.x == 0) {
    x = p3.x;

    // slope and intercept of first line
    m1 = (p2.y - p1.y) / (p2.x - p1.x);
    b1 = p1.y - m1 * p1.x;

    // y-coordinate of intersection
    y = m1 * x + b1;
  }
  // if neither line is vertical
  else {
    m1 = (p2.y - p1.y) / (p2.x - p1.x);
    b1 = p1.y - m1 * p1.x;

    // slope and intercept of second line
    m2 = (p4.y - p3.y) / (p4.x - p3.x);
    b2 = p3.y - m2 * p3.x;

    // x-coordinate of intersection
    x = (b2 - b1) / (m1 - m2);

    // y-coordinate of intersection
    y = m1 * x + b1;
  }

  Vector2 intersection = {x, y};

  //printVector2(&intersection);

  return intersection;
}
Vector polygonClockWiseVerticesClip(Vector2 subject_polygon[], ulong subject_polygon_len,
            Vector2 clipping_polygon[], ulong clipping_polygon_len) {
  Vector final_polygon;
  Vector_setMemory(&final_polygon, subject_polygon, subject_polygon_len,
                   sizeof(Vector2), false);

  for (ulong i = 0; i < clipping_polygon_len; i++) {
    Vector nextPolygon = Vector_copy(&final_polygon);

    /*for(ulong a = 0 ; a < nextPolygon.length ; a++)
      {
        printVector2((Vector2*)Vector_element(&nextPolygon, a));
      }*/

    /*printf("clearing...\n");*/
    Vector_clear(&final_polygon);
    /*printf("ended clearing\n");*/

    // these two vertices define a line segment (edge) in the clipping
    // polygon. It is assumed that indices wrap around, such that if
    // i = 1, then i - 1 = K.
    
    // Accessing lopped, if i <= 0 then we would access the last element
    int index = clipping_polygon_len - 1;
    if(i > 0)
    {
      index = i - 1;
    }
    Vector2 c_edge_start = clipping_polygon[index];
    
    Vector2 c_edge_end = clipping_polygon[i];

    //printVector2(&c_edge_start);
    //printVector2(&c_edge_end);

    for (ulong j = 0; j < nextPolygon.length; j++) {
      // these two vertices define a line segment (edge) in the subject polygon
      
      // Accessing lopped, if j <= 0 then we would access the last element
      int index = nextPolygon.length - 1;
      if(j > 0)
      {
        index = j - 1;
      }


      Vector2 s_edge_start = *(Vector2 *)Vector_element(&nextPolygon, index);
      Vector2 s_edge_end = *(Vector2 *)Vector_element(&nextPolygon, j);

      

      if (isInside(c_edge_start, c_edge_end, s_edge_end)) {
        if (!isInside(c_edge_start, c_edge_end, s_edge_start)) {
          Vector2 intersection = compute_intersection(s_edge_start, s_edge_end,
                                                      c_edge_start, c_edge_end);
          Vector_Push_back(&final_polygon, &intersection);
        }
        Vector_Push_back(&final_polygon, &s_edge_end);

      }

      else if (isInside(c_edge_start, c_edge_end, s_edge_start)) {
        Vector2 intersection = compute_intersection(s_edge_start, s_edge_end,
                                                    c_edge_start, c_edge_end);
        Vector_Push_back(&final_polygon, &intersection);
      }
    }

    Vector_Delete(&nextPolygon);
  }

  return final_polygon;
}

#endif