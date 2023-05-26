#pragma once

#ifndef __FILLING_TRIANGLE_ALGORITHM_H__
#define __FILLING_TRIANGLE_ALGORITHM_H__

#include "../maths/vec.h"
#include "vertexDrawer.h"
#include "drawContext.h"

Vector2 getIntersectionPoint(float h, Vector2 point0, Vector2 point1 )
{

  float m = (float)(point0.y - point1.y) / (float)(point0.x - point1.x);

  Vector2 output;

  output.x = (h - point0.y) / m + point0.x;
  output.y = h;

  return output;
}

// where v1 is the top vertex
void drawBottomFlatTriangle(struct DrawVertexData* v1, struct DrawVertexData* v2, struct DrawVertexData* v3)
{
  for(float y = v1->position.y ; y >= v2->position.y ; y -= 1.0f / (float)contextPixelBuffer->height)
  {
    vertices2Color(v2->color, v3->color);

    Vector2 intersectionPoint0 = getIntersectionPoint(y, v1->position, v2->position);
    //planPixel(intersectionPoint0.x, intersectionPoint0.y, 0xffffffff, pixelBuffer);

    Vector2 intersectionPoint1 = getIntersectionPoint(y, v1->position, v3->position);
    //planPixel(intersectionPoint1.x, intersectionPoint1.y, 0xffffffff, pixelBuffer);

    float positions[] = { intersectionPoint0.x, intersectionPoint0.y, intersectionPoint1.x, intersectionPoint1.y };
    //float positions[] = { xmin, ymin, xmax, ymax };
    draw2DVertices(positions, 4, contextPixelBuffer);
  }
}
// where v1 and v2 are the top vertices
void drawTopFlatTriangle(struct DrawVertexData* v1, struct DrawVertexData* v2, struct DrawVertexData* v3)
{
  for(float y = v1->position.y ; y >= v3->position.y ; y -= 1.0f / (float)contextPixelBuffer->height)
  {
    vertices2Color(v2->color, v3->color);

    Vector2 intersectionPoint0 = getIntersectionPoint(y, v1->position, v3->position);
    //planPixel(intersectionPoint0.x, intersectionPoint0.y, 0xffffffff, pixelBuffer);

    Vector2 intersectionPoint1 = getIntersectionPoint(y, v2->position, v3->position);
    //planPixel(intersectionPoint1.x, intersectionPoint1.y, 0xffffffff, pixelBuffer);

    float positions[] = { intersectionPoint0.x, intersectionPoint0.y, intersectionPoint1.x, intersectionPoint1.y };
    //float positions[] = { xmin, ymin, xmax, ymax };
    draw2DVertices(positions, 4, contextPixelBuffer);
  }
}

#endif