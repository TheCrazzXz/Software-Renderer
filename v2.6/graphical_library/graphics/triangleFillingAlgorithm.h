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

struct EdgeEquation {
  float a;
  float b;
  float c;
  bool tie;
};

struct EdgeEquation EdgeEquation_create(const Vector2* v0, const Vector2* v1)
{
  struct EdgeEquation out;
  out.a = v0->y - v1->y;
  out.b = v1->x - v0->x;
  out.c = - (out.a * (v0->x + v1->x) + out.b * (v0->y + v1->y)) / 2;
  out.tie = out.a != 0 ? out.a > 0 : out.b > 0;
  return out;
}

/// Evaluate the edge equation for the given point.
float EdgeEquation_evaluate(struct EdgeEquation* edgeEquation, float x, float y)
{
  return edgeEquation->a * x + edgeEquation->b * y + edgeEquation->c;
}

/// Test for a given evaluated value.
bool EdgeEquation_testValue(struct EdgeEquation* edgeEquation, float v)
{
  return (v > 0 || v == 0 && edgeEquation->tie);
}

/// Test if the given point is inside the edge.
bool EdgeEquation_testPoint(struct EdgeEquation* edgeEquation, float x, float y)
{
  return EdgeEquation_testValue(edgeEquation, EdgeEquation_evaluate(edgeEquation, x, y));
}
typedef struct EdgeEquation EdgeEquation;

/*struct ParameterEquation {
  float a;
  float b;
  float c;
};
struct ParameterEquation ParameterEquation_create(

    float p0,
    float p1,
    float p2,
    const EdgeEquation *e0,
    const EdgeEquation *e1,
    const EdgeEquation *e2,
    float area)
  {
    float factor = 1.0f / (2.0f * area);

    a = factor * (p0 * e0.a + p1 * e1.a + p2 * e2.a);
    b = factor * (p0 * e0.b + p1 * e1.b + p2 * e2.b);
    c = factor * (p0 * e0.c + p1 * e1.c + p2 * e2.c);
  }

  /// Evaluate the parameter equation for the given point.
  float evaluate(float x, float y)
  {
    return a * x + b * y + c;
  }
*/

float sign (Vector2* p1, Vector2* p2, Vector2* p3)
{
    return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
}

bool PointInTriangle (Vector2* pt, Vector2* v1, Vector2* v2, Vector2* v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}


#endif