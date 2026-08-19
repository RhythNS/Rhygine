#include "VertexLayout.h"

namespace Rhygine
{
	namespace
	{
		inline void HashCombine64(uint64_t& seed, uint64_t val)
		{
			seed ^= val + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
		}
	}

	VertexLayout::VertexLayout()
	{
		ComputeHash();
	}

	VertexLayout::VertexLayout(std::vector<VertexInputAttribute> t_attributes, std::vector<VertexInputBinding> t_bindings)
		: attributes(std::move(t_attributes))
		, bindings(std::move(t_bindings))
	{
		ComputeHash();
	}

	void VertexLayout::ComputeHash()
	{
		uint64_t h = 14695981039346656037ULL;

		for (const auto& b : bindings)
		{
			HashCombine64(h, b.binding);
			HashCombine64(h, b.stride);
			HashCombine64(h, b.perInstance ? 1ULL : 0ULL);
		}

		for (const auto& a : attributes)
		{
			HashCombine64(h, a.location);
			HashCombine64(h, a.binding);
			HashCombine64(h, static_cast<uint64_t>(a.format));
			HashCombine64(h, a.offset);
		}

		hash = h;
	}

	bool VertexLayout::operator==(const VertexLayout& other) const
	{
		if (hash != other.hash)
		{
			return false;
		}

		if (bindings.size() != other.bindings.size() || attributes.size() != other.attributes.size())
		{
			return false;
		}

		for (size_t i = 0; i < bindings.size(); ++i)
		{
			if (bindings[i].binding != other.bindings[i].binding ||
				bindings[i].stride != other.bindings[i].stride ||
				bindings[i].perInstance != other.bindings[i].perInstance)
			{
				return false;
			}
		}

		for (size_t i = 0; i < attributes.size(); ++i)
		{
			if (attributes[i].location != other.attributes[i].location ||
				attributes[i].binding != other.attributes[i].binding ||
				attributes[i].format != other.attributes[i].format ||
				attributes[i].offset != other.attributes[i].offset)
			{
				return false;
			}
		}

		return true;
	}

	VertexLayout VertexLayout::PositionColor()
	{
		VertexInputBinding binding{};
		binding.binding = 0;
		binding.stride = sizeof(float) * 6; // pos: 3, col: 3
		binding.perInstance = false;

		VertexInputAttribute posAttr{};
		posAttr.location = 0;
		posAttr.binding = 0;
		posAttr.format = Format::R32G32B32_FLOAT;
		posAttr.offset = 0;

		VertexInputAttribute colAttr{};
		colAttr.location = 1;
		colAttr.binding = 0;
		colAttr.format = Format::R32G32B32_FLOAT;
		colAttr.offset = sizeof(float) * 3;

		return VertexLayout({ posAttr, colAttr }, { binding });
	}

	VertexLayout VertexLayout::Standard3D()
	{
		VertexInputBinding binding{};
		binding.binding = 0;
		binding.stride = sizeof(float) * 8; // pos: 3, normal: 3, uv: 2
		binding.perInstance = false;

		VertexInputAttribute posAttr{};
		posAttr.location = 0;
		posAttr.binding = 0;
		posAttr.format = Format::R32G32B32_FLOAT;
		posAttr.offset = 0;

		VertexInputAttribute normAttr{};
		normAttr.location = 1;
		normAttr.binding = 0;
		normAttr.format = Format::R32G32B32_FLOAT;
		normAttr.offset = sizeof(float) * 3;

		VertexInputAttribute uvAttr{};
		uvAttr.location = 2;
		uvAttr.binding = 0;
		uvAttr.format = Format::R32G32_FLOAT;
		uvAttr.offset = sizeof(float) * 6;

		return VertexLayout({ posAttr, normAttr, uvAttr }, { binding });
	}

	VertexLayout VertexLayout::Empty()
	{
		return VertexLayout({}, {});
	}
}
