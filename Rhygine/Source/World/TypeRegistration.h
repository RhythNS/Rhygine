#pragma once

#include <vector>
#include <memory>
#include <string>
#include <type_traits>

#include "TypeRegistry.h"
#include "WorldDescriptors.h"
#include "Task.h"
#include "Service.h"
#include "Behaviour.h"

namespace Rhygine
{
	class World;
}

namespace Rhygine::detail
{
	using RegisterFn = void(*)(TypeRegistry&);
	using ParentChildFn = void(*)(TypeRegistry&);
	using DependencyFn = void(*)(TypeRegistry&);

	struct PendingRegistration
	{
		friend class World;

		std::string name;
		TypeKind kind;
		RegisterFn registerFn = nullptr;
		ParentChildFn parentChildFn = nullptr;
		DependencyFn dependencyFn = nullptr;

		static void AddPendingRegistration(const PendingRegistration& registration)
		{
			s_pendingRegistrations.push_back(registration);
		}

	private:
		static std::vector<PendingRegistration> s_pendingRegistrations;
	};

	template <typename Type, typename Dependency>
	constexpr DependencyFn MakeDependencyFn(Dependency*)
	{
		return [](TypeRegistry& registry)
			{
				registry.RegisterDependency(registry.GetId<Type>(), registry.GetId<Dependency>());
			};
	}

	template <typename Type, typename Dependency, typename... Rest>
	constexpr DependencyFn MakeDependencyFn(Dependency*, Rest*...)
	{
		return [](TypeRegistry& registry)
			{
				registry.RegisterDependency(registry.GetId<Type>(), registry.GetId<Dependency>());
				MakeDependencyFn<Type, Rest...>(static_cast<Rest*>(nullptr)...)(registry);
			};
	}

	template <typename Type, typename...Dependencies>
	constexpr DependencyFn BuildDependencyFn()
	{
		if constexpr (sizeof...(Dependencies) == 0)
		{
			return nullptr;
		}
		else
		{
			return MakeDependencyFn<Type>(static_cast<Dependencies*>(nullptr)...);
		}
	}
}

#define RHYGINE_COMPONENT(Type) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_COMPONENT_WITH_DEPENDENCIES(Type, ...) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_COMPONENT_WITH_PARENT(Type, ParentType) \
	struct Type##ParentRegistrationHelper { \
		Type##ParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentRegistrationHelper global_##Type##ParentRegistrationHelper; \

#define RHYGINE_COMPONENT_WITH_DEPENDENCIES_AND_PARENT(Type, ParentType, ...) \
	struct Type##ParentDependencyRegistrationHelper { \
		Type##ParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentDependencyRegistrationHelper global_##Type##ParentDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_ATTACH_DETACH(Type, AttachFn, DetachFn) \
	struct Type##AttachDetachRegistrationHelper { \
		Type##AttachDetachRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(nullptr, AttachFn, DetachFn); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##AttachDetachRegistrationHelper global_##Type##AttachDetachRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_ATTACH_DETACH_AND_DEPENDENCIES(Type, AttachFn, DetachFn, ...) \
	struct Type##AttachDetachDependencyRegistrationHelper { \
		Type##AttachDetachDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(nullptr, AttachFn, DetachFn); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##AttachDetachDependencyRegistrationHelper global_##Type##AttachDetachDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_ATTACH_DETACH_AND_PARENT(Type, AttachFn, DetachFn, ParentType) \
	struct Type##AttachDetachParentRegistrationHelper { \
		Type##AttachDetachParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(nullptr, AttachFn, DetachFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##AttachDetachParentRegistrationHelper global_##Type##AttachDetachParentRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_ATTACH_DETACH_AND_PARENT_AND_DEPENDENCIES(Type, AttachFn, DetachFn, ParentType, ...) \
	struct Type##AttachDetachParentDependencyRegistrationHelper { \
		Type##AttachDetachParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(nullptr, AttachFn, DetachFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##AttachDetachParentDependencyRegistrationHelper global_##Type##AttachDetachParentDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR(Type, ExtractorFn) \
	struct Type##ExtractorRegistrationHelper { \
		Type##ExtractorRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorRegistrationHelper global_##Type##ExtractorRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_DEPENDENCIES(Type, ExtractorFn, ...) \
	struct Type##ExtractorDependencyRegistrationHelper { \
		Type##ExtractorDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorDependencyRegistrationHelper global_##Type##ExtractorDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_PARENT(Type, ExtractorFn, ParentType) \
	struct Type##ExtractorParentRegistrationHelper { \
		Type##ExtractorParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorParentRegistrationHelper global_##Type##ExtractorParentRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_PARENT_AND_DEPENDENCIES(Type, ExtractorFn, ParentType, ...) \
	struct Type##ExtractorParentDependencyRegistrationHelper { \
		Type##ExtractorParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorParentDependencyRegistrationHelper global_##Type##ExtractorParentDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_DEPENDENCIES_AND_PARENT(Type, ExtractorFn, ParentType, ...) \
	RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_PARENT_AND_DEPENDENCIES(Type, ExtractorFn, ParentType, __VA_ARGS__)

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH(Type, ExtractorFn, AttachFn, DetachFn) \
	struct Type##ExtractorAttachDetachRegistrationHelper { \
		Type##ExtractorAttachDetachRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn, AttachFn, DetachFn); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorAttachDetachRegistrationHelper global_##Type##ExtractorAttachDetachRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH_AND_DEPENDENCIES(Type, ExtractorFn, AttachFn, DetachFn, ...) \
	struct Type##ExtractorAttachDetachDependencyRegistrationHelper { \
		Type##ExtractorAttachDetachDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn, AttachFn, DetachFn); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorAttachDetachDependencyRegistrationHelper global_##Type##ExtractorAttachDetachDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH_AND_PARENT(Type, ExtractorFn, AttachFn, DetachFn, ParentType) \
	struct Type##ExtractorAttachDetachParentRegistrationHelper { \
		Type##ExtractorAttachDetachParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn, AttachFn, DetachFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorAttachDetachParentRegistrationHelper global_##Type##ExtractorAttachDetachParentRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH_AND_PARENT_AND_DEPENDENCIES(Type, ExtractorFn, AttachFn, DetachFn, ParentType, ...) \
	struct Type##ExtractorAttachDetachParentDependencyRegistrationHelper { \
		Type##ExtractorAttachDetachParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Component; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterComponent<Type>(ExtractorFn, AttachFn, DetachFn); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ExtractorAttachDetachParentDependencyRegistrationHelper global_##Type##ExtractorAttachDetachParentDependencyRegistrationHelper;

#define RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH_AND_DEPENDENCIES_AND_PARENT(Type, ExtractorFn, AttachFn, DetachFn, ParentType, ...) \
	RHYGINE_COMPONENT_WITH_EXTRACTOR_AND_ATTACH_DETACH_AND_PARENT_AND_DEPENDENCIES(Type, ExtractorFn, AttachFn, DetachFn, ParentType, __VA_ARGS__)

#define RHYGINE_BEHAVIOUR(Type) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Behaviour; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterBehaviour<Type>(); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_BEHAVIOUR_WITH_DEPENDENCIES(Type, ...) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Behaviour; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterBehaviour<Type>(); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_BEHAVIOUR_WITH_PARENT(Type, ParentType) \
	struct Type##ParentRegistrationHelper { \
		Type##ParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Behaviour; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterBehaviour<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentRegistrationHelper global_##Type##ParentRegistrationHelper;

#define RHYGINE_BEHAVIOUR_WITH_DEPENDENCIES_AND_PARENT(Type, ParentType, ...) \
	struct Type##ParentDependencyRegistrationHelper { \
		Type##ParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Behaviour; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterBehaviour<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentDependencyRegistrationHelper global_##Type##ParentDependencyRegistrationHelper;

#define RHYGINE_SERVICE(Type) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Service; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterService<Type>(); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_SERVICE_WITH_DEPENDENCIES(Type, ...) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Service; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterService<Type>(); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_SERVICE_WITH_PARENT(Type, ParentType) \
	struct Type##ParentRegistrationHelper { \
		Type##ParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Service; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterService<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentRegistrationHelper global_##Type##ParentRegistrationHelper;

#define RHYGINE_SERVICE_WITH_DEPENDENCIES_AND_PARENT(Type, ParentType, ...) \
	struct Type##ParentDependencyRegistrationHelper { \
		Type##ParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Service; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterService<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentDependencyRegistrationHelper global_##Type##ParentDependencyRegistrationHelper;

#define RHYGINE_TASK(Type) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Task; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterTask<Type>(); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_TASK_WITH_DEPENDENCIES(Type, ...) \
	struct Type##RegistrationHelper { \
		Type##RegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Task; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterTask<Type>(); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##RegistrationHelper global_##Type##RegistrationHelper;

#define RHYGINE_TASK_WITH_PARENT(Type, ParentType) \
	struct Type##ParentRegistrationHelper { \
		Type##ParentRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Task; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterTask<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentRegistrationHelper global_##Type##ParentRegistrationHelper;

#define RHYGINE_TASK_WITH_DEPENDENCIES_AND_PARENT(Type, ParentType, ...) \
	struct Type##ParentDependencyRegistrationHelper { \
		Type##ParentDependencyRegistrationHelper() { \
			Rhygine::detail::PendingRegistration reg; \
			reg.name = typeid(Type).name(); \
			reg.kind = Rhygine::TypeKind::Task; \
			reg.registerFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterTask<Type>(); }; \
			reg.parentChildFn = [](Rhygine::TypeRegistry& registry) { registry.RegisterParentChild(registry.GetId<ParentType>(), registry.GetId<Type>()); }; \
			reg.dependencyFn = Rhygine::detail::BuildDependencyFn<Type, __VA_ARGS__>(); \
			Rhygine::detail::PendingRegistration::AddPendingRegistration(reg); \
		} \
	}; \
	static Type##ParentDependencyRegistrationHelper global_##Type##ParentDependencyRegistrationHelper;

