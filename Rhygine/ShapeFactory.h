#pragma once
#include "VertIndexes.h"
#include "Verticies.h"

class ShapeFactory
{
public:
	template <class Index>
	static VertIndexes<VertexPosColor, Index> GetCubePosColor()
	{
		return VertIndexes<VertexPosColor, Index>
			(
				{ 
					{-0.5f, -0.5f, -0.5f, 0,0,0,255},
					{ 0.5f, -0.5f, -0.5f, 0,0,0,255},
					{-0.5f,  0.5f, -0.5f, 0,0,0,255},
					{ 0.5f,  0.5f, -0.5f, 0,0,0,255},
					{-0.5f, -0.5f,  0.5f, 0,0,0,255},
					{ 0.5f, -0.5f,  0.5f, 0,0,0,255},
					{-0.5f,  0.5f,  0.5f, 0,0,0,255},
					{ 0.5f,  0.5f,  0.5f, 0,0,0,255} 
				},
				{
					0,2,3, 0,3,1,
					4,6,2, 4,2,0,
					5,7,6, 5,6,4,
					1,3,7, 1,7,5,
					2,6,3, 6,7,3,
					0,1,4, 1,5,4
				}
			);
	}

	template <class Index>
	static VertIndexes<VertexPosUV, Index> GetCubePosUV();

	template <class Index>
	static VertIndexes<VertexPosNormalUV, Index> GetCubePosNormalUV();

	template <class Index>
	static VertIndexes<VertexPosNormalColor, Index> GetCubePosNormalColor();
};
