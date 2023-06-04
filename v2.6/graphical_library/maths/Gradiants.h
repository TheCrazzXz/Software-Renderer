#pragma once

#include "vec.h"
#ifndef __GRADIANTS_H__
#define __GRADIANTS_H__

#include "../graphics/drawContext.h"

struct Gradiants
{
    Vector4 color[3];
    Vector4 colorX_step;
    Vector4 colorY_step;
};

struct Gradiants createGradiants(DrawVertexData minYVertex, DrawVertexData midYVertex, DrawVertexData maxYVertex)
{
	struct Gradiants out;

	out.color[0] = minYVertex.color;
	out.color[1] = midYVertex.color;
	out.color[2] = maxYVertex.color;

	float oneOverdX = 1.0f /
		(((midYVertex.position.x - maxYVertex.position.x) *
		(minYVertex.position.y - maxYVertex.position.y)) -
		((minYVertex.position.x - maxYVertex.position.x) *
		(midYVertex.position.y - maxYVertex.position.y)));

	float oneOverdY = -oneOverdX;

	/*out.colorX_step =  
		(
			(
				(m_color[1].Sub(m_color[2])).Mul(
				(minYVertex.position.y - maxYVertex.position.y))
			).Sub(
				((m_color[0].Sub(m_color[2])).Mul(
				(midYVertex.position.y - maxYVertex.position.y)))
			)
		).Mul(oneOverdX);*/

	out.colorX_step = ( 
		Vector4_mulNumber_get_copy(
			Vectors4_sub_copy(
				Vector4_mulNumber_get_copy(Vectors4_sub_copy(out.color[1], out.color[2]), (minYVertex.position.y - maxYVertex.position.y)), 
				Vector4_mulNumber_get_copy(Vectors4_sub_copy(out.color[0], out.color[2]), (midYVertex.position.y - maxYVertex.position.y)) 
			), oneOverdX
		)
	);

	/*out.colorY_step =  
		(((m_color[1].Sub(m_color[2])).Mul(
		(minYVertex.position.x - maxYVertex.position.x))).Sub(
		((m_color[0].Sub(m_color[2])).Mul(
		(midYVertex.position.x - maxYVertex.position.x))))).Mul(oneOverdY);*/

	out.colorY_step = ( 
		Vector4_mulNumber_get_copy(
			Vectors4_sub_copy(
				Vector4_mulNumber_get_copy(Vectors4_sub_copy(out.color[1], out.color[2]), (minYVertex.position.x - maxYVertex.position.x)), 
				Vector4_mulNumber_get_copy(Vectors4_sub_copy(out.color[0], out.color[2]), (midYVertex.position.x - maxYVertex.position.x)) 
			), oneOverdY
		)
	);
	return out;
}


typedef struct Gradiants Gradiants;

#endif