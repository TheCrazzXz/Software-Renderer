#pragma once


#ifndef __FILLING_TRIANGLE_ALGORITHM_H__
#define __FILLING_TRIANGLE_ALGORITHM_H__

#include "shapeDrawing.h"
#include "../maths/vec.h"
#include "vertexProcessor.h"
#include "drawContext.h"

bool isTriangleOutOfScreen(DrawVertexData* v1, DrawVertexData* v2, DrawVertexData* v3)
{
  if(v1->position.x > 1 && v2->position.x > 1 && v3->position.x > 1)
  {
    return 1;
  }

  if(v1->position.x < -1 && v2->position.x < -1 && v3->position.x < -1)
  {
    return 1;
  }

  if(v1->position.y > 1 && v2->position.y > 1 && v3->position.y > 1)
  {
    return 1;
  }

  if(v1->position.y < -1 && v2->position.y < -1 && v3->position.y < -1)
  {
    return 1;
  }

  return 0;
}

Vector2 getIntersectionPoint(float h, Vector2 point0, Vector2 point1 )
{

  float m = (float)(point0.y - point1.y) / (float)(point0.x - point1.x);

  Vector2 output;

  output.x = (h - point0.y) / m + point0.x;
  output.y = h;

  return output;
}

// where v1 is the top vertex
void drawBottomFlatTriangle(DrawVertexData* v1, DrawVertexData* v2, DrawVertexData* v3, VerticesState vstate)
{
  if(isTriangleOutOfScreen(v1, v2, v3))
	{
		return;
	}
  for(float y = v1->position.y ; y >= v2->position.y ; y -= 1.0f / (float)contextPixelBuffer->height)
  {
    Vector2 intersectionPoint0 = getIntersectionPoint(y, v1->position, v2->position);
    Vector2 intersectionPoint1 = getIntersectionPoint(y, v1->position, v3->position);
    if(vstate.allVerticesHaveSameColor)
    {
      drawUnicolorHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, v1->color, contextPixelBuffer);
    }
    else if(vstate.useOtherColor)
    {
      drawUnicolorHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, vstate.otherColor, contextPixelBuffer);
    }
    else
    {
      drawInterpolatedHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, vstate, contextPixelBuffer);
    }
  }
}
// where v1 and v2 are the top vertices
void drawTopFlatTriangle(DrawVertexData* v1, DrawVertexData* v2, DrawVertexData* v3, VerticesState vstate)
{
  if(isTriangleOutOfScreen(v1, v2, v3))
	{
		return;
	}
  for(float y = v1->position.y ; y >= v3->position.y ; y -= 1.0f / (float)contextPixelBuffer->height)
  {
    Vector2 intersectionPoint0 = getIntersectionPoint(y, v1->position, v3->position);

    Vector2 intersectionPoint1 = getIntersectionPoint(y, v2->position, v3->position);
  
    if(vstate.allVerticesHaveSameColor)
    {
      drawUnicolorHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, v1->color, contextPixelBuffer);
    }
    else if(vstate.useOtherColor)
    {
      drawUnicolorHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, vstate.otherColor, contextPixelBuffer);
    }
    else
    {
      drawInterpolatedHorizontalLineAcceleratedDraw(intersectionPoint0.x, intersectionPoint1.x, intersectionPoint0.y, vstate, contextPixelBuffer);
    }
   
  }
}

#endif
