set ( 
	SHADER_VERT_FILES
	Shaders/ReactDebugVert.hlsl
)
set ( 
	SHADER_PIX_FILES
	Shaders/ReactDebugPix.hlsl
)

foreach (VS ${SHADER_VERT_FILES})
	RHY_COMPILE_VS (VS_OUT ${VS})
	list (APPEND CC_VS_OUT ${VS_OUT})
endforeach ()

foreach (PS ${SHADER_PIX_FILES})
	RHY_COMPILE_PS (PS_OUT ${PS})
	list (APPEND CC_PS_OUT ${PS_OUT})
endforeach ()

add_library (
	ReactPhysics
	Include/React/Collider/BoxCollider.h
	Include/React/Collider/CapsuleCollider.h
	Include/React/Collider/ConcaveCollider.h
	Include/React/Collider/ConcaveMeshCollider.h
	Include/React/Collider/ConvexCollider.h
	Include/React/Collider/ConvexMeshCollider.h
	Include/React/Collider/ConvexPolyhedronCollider.h
	Include/React/Collider/HeightFieldCollider.h
	Include/React/Collider/ReactCollider.h
	Include/React/Collider/SphereCollider.h
	Include/React/CollisionBody.h
	Include/React/Ray.h
	Include/React/RaycastInfo.h
	Include/React/ReactBodyBase.h
	Include/React/ReactConverter.h
	Include/React/ReactDebugRenderer.h
	Include/React/ReactLogger.h
	Include/React/ReactPhysicsModule.h
	Include/React/RigidBody.h
	Include/RhyReact.h
	Source/React/Collider/BoxCollider.cpp
	Source/React/Collider/CapsuleCollider.cpp
	Source/React/Collider/ConcaveCollider.cpp
	Source/React/Collider/ConcaveMeshCollider.cpp
	Source/React/Collider/ConvexCollider.cpp
	Source/React/Collider/ConvexMeshCollider.cpp
	Source/React/Collider/ConvexPolyhedronCollider.cpp
	Source/React/Collider/HeightFieldCollider.cpp
	Source/React/Collider/ReactCollider.cpp
	Source/React/Collider/SphereCollider.cpp
	Source/React/CollisionBody.cpp
	Source/React/ReactBodyBase.cpp
	Source/React/ReactDebugRenderer.cpp
	Source/React/ReactLogger.cpp
	Source/React/ReactPhysicsModule.cpp
	Source/React/RigidBody.cpp
    ${CC_VS_OUT}
    ${CC_PS_OUT}
)

target_include_directories (ReactPhysics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (ReactPhysics PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	ReactPhysics 
	LINK_PUBLIC reactphysics3d
	LINK_PUBLIC RhyCore
)
