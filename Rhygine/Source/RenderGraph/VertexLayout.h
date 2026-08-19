#pragma once

#include <vector>
#include <cstdint>
#include "RHIDescriptors.h"

namespace Rhygine
{
	struct VertexLayout
	{
		std::vector<VertexInputAttribute> attributes;
		std::vector<VertexInputBinding> bindings;
		uint64_t hash = 0;

		VertexLayout();
		VertexLayout(std::vector<VertexInputAttribute> t_attributes, std::vector<VertexInputBinding> t_bindings);

		void ComputeHash();

		bool operator==(const VertexLayout& other) const;

		static VertexLayout PositionColor();
		static VertexLayout Standard3D();
		static VertexLayout Empty();
	};
}
