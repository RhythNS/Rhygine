#pragma once
#include <vector>

/// <summary>
/// Helper class which holds a vertex and index vector.
/// </summary>
/// <typeparam name="Vertex">The vertex type.</typeparam>
/// <typeparam name="Index">The index type.</typeparam>
template<class Vertex, class Index>
class VertIndexes
{
public:
	std::vector<Vertex> verticies;
	std::vector<Index> indicies;
};
