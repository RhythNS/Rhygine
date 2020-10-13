#pragma once
#include "VertIndexes.h"

class ShapeFactory
{
public:
	template <class Vertex, class Index>
	static VertIndexes<Vertex, Index> GetCube();

	template <class Vertex, class Index>
	static VertIndexes<Vertex, Index> GetNormalCube();

	template <class Vertex, class Index>
	static VertIndexes<Vertex, Index> GetPyramid();

	template <class Vertex, class Index>
	static VertIndexes<Vertex, Index> GetNormalPyramid();
};

