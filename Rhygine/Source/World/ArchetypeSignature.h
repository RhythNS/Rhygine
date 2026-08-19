#pragma once

#include <vector>
#include <initializer_list>
#include <functional>

#include "WorldDescriptors.h"

namespace Rhygine
{
	struct ArchetypeSignature
	{
	public:
		ArchetypeSignature()
		{
			m_hash = ComputeHash();
		}

		ArchetypeSignature(std::vector<TypeId> t_componentTypes)
		{
			std::sort(t_componentTypes.begin(), t_componentTypes.end());
			m_componentTypes = std::move(t_componentTypes);
			m_hash = ComputeHash();
		}

		ArchetypeSignature(TypeId t_componentType)
			: m_componentTypes{ t_componentType }
		{
			m_hash = ComputeHash();
		}

		ArchetypeSignature(std::initializer_list<TypeId> t_componentTypes)
			: ArchetypeSignature(std::vector<TypeId>(t_componentTypes))
		{}

		[[nodiscard]] const std::vector<TypeId>& GetComponentTypes() const noexcept { return m_componentTypes; }
		[[nodiscard]] bool Contains(TypeId t_componentType) const noexcept
		{
			return std::binary_search(m_componentTypes.begin(), m_componentTypes.end(), t_componentType);
		}

		[[nodiscard]] ArchetypeSignature operator+(TypeId t_componentType) const
		{
			std::vector<TypeId> newTypes = m_componentTypes;
			if (Contains(t_componentType) == false)
			{
				newTypes.push_back(t_componentType);
			}
			return ArchetypeSignature(std::move(newTypes));
		}
		[[nodiscard]] ArchetypeSignature operator-(TypeId t_componentType) const
		{
			std::vector<TypeId> newTypes = m_componentTypes;
			newTypes.erase(std::remove(newTypes.begin(), newTypes.end(), t_componentType), newTypes.end());
			return ArchetypeSignature(std::move(newTypes));
		}

		[[nodiscard]] auto begin() const noexcept { return m_componentTypes.begin(); }
		[[nodiscard]] auto end() const noexcept { return m_componentTypes.end(); }
		[[nodiscard]] size_t size() const noexcept { return m_componentTypes.size(); }
		[[nodiscard]] bool empty() const noexcept { return m_componentTypes.empty(); }

		[[nodiscard]] std::size_t GetHash() const noexcept { return m_hash; }
		bool operator==(const ArchetypeSignature& other) const = default;

		static const ArchetypeSignature& Empty() { static ArchetypeSignature empty; return empty; }

	private:
		std::size_t ComputeHash() const noexcept
		{
			std::size_t h = 0;
			for (auto id : m_componentTypes)
			{
				h ^= std::hash<TypeId>{}(id)+0x9e3779b9 + (h << 6) + (h >> 2);
			}
			return h;
		}

		std::vector<TypeId> m_componentTypes;
		std::size_t m_hash{ 0 };
	};
}

template<>
struct std::hash<Rhygine::ArchetypeSignature>
{
	std::size_t operator()(const Rhygine::ArchetypeSignature& signature) const noexcept
	{
		return signature.GetHash();
	}
};
