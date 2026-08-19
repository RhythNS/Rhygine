#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <memory>

namespace Rhygine
{
	class Archetype;
	class World;
	struct Transform;
	struct RenderPacketBuilder;

	typedef uint32_t SceneId;

	struct EntityId
	{
		uint32_t index;
		uint32_t generation;

		EntityId(uint32_t t_index, uint32_t t_generation) : index(t_index), generation(t_generation) {}

		static EntityId Invalid() { return { UINT32_MAX, 0 }; }
		bool operator==(const EntityId& other) const { return index == other.index && generation == other.generation; }
	};

	struct EntityRecord
	{
		uint32_t generation;
		Archetype* archetype;
		size_t rowInArchetype;
		SceneId scene;
		bool markedForDeletion;
	};

	typedef uint32_t TypeId;
	
	enum class TypeKind
	{
		Component,
		Behaviour,
		Service,
		Task
	};

	struct TypeInfo
	{
		TypeId id;
		std::string name;
		TypeKind kind;

		size_t size;
		size_t alignment;
		
		using ConstructFn = void(*)(void*);
		using DestructFn = void(*)(void*);
		using MoveConstructFn = void(*)(void* t_dst, void* t_src);
		using AttachFn = void(*)(EntityId t_id, void* t_data, World& t_world);
		using DetachFn = void(*)(EntityId t_id, void* t_data, World& t_world);
		using ExtractorFn = void(*)(const void* t_component, const Transform* t_transform, RenderPacketBuilder& t_builder);
		using FactoryFn = std::unique_ptr<void, void(*)(void*)>(*)(void* t_args);

		ConstructFn construct;
		DestructFn destruct;
		MoveConstructFn moveConstruct;
		AttachFn attach;
		DetachFn detach;
		ExtractorFn extractor;
		FactoryFn factory;
	};
}
