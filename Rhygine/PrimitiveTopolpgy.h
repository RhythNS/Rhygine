#pragma once
#include "Bindable.h"

/// <summary>
/// Representation of a primitive topology.
/// </summary>
class PrimitiveTopology : public Bindable
{
public:
	/// <summary>
	/// Constructs a primitive topology.
	/// </summary>
	PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopolpgy);

	void Bind();
	
	/// <summary>
	/// Gets the topology.
	/// </summary>
	D3D11_PRIMITIVE_TOPOLOGY* GetTopology();
private:
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};

