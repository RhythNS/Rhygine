add_library(
	RhyWorld
	"TypeRegistry.h"
	"TypeRegistry.cpp"
	"Entity.h"
	"Entity.cpp"
	"Scene.h"
	"Scene.cpp"
	"World.h"
	"World.cpp"
	"WorldDescriptors.h"
	"Archetype.h"
	"Archetype.cpp"
	"ArchetypeSignature.h"
	"DeferredCommandBuffer.h"
	"DeferredCommandBuffer.cpp"
	"Service.h"
	"Behaviour.h"
	"Task.h"
	"Types.h"
	"Resource.h"
	"ResourceManager.h"
	"ResourceManager.cpp"
	"TypeRegistration.h"
	"TypeRegistration.cpp"
	"Components/Transform.h"
	"Components/Camera.h"
	"Components/Hierarchy.h"
	"Components/Hierarchy.cpp"
)

target_include_directories (RhyWorld PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyWorld
	PUBLIC RhyCore
	PUBLIC RhyRenderGraph
)
