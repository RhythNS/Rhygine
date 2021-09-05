set ( 
	SHADER_VERT_FILES
	Shaders/BulletDebugVert.hlsl
)
set ( 
	SHADER_PIX_FILES
	Shaders/BulletDebugPix.hlsl
)

foreach (VS ${SHADER_VERT_FILES})
	RHY_COMPILE_VS (VS_OUT ${VS})
	list (APPEND CC_VS_OUT ${VS_OUT})
endforeach ()

foreach (PS ${SHADER_PIX_FILES})
	RHY_COMPILE_PS (PS_OUT ${PS})
	list (APPEND CC_PS_OUT ${PS_OUT})
endforeach ()

add_library(
	BulletPhysics
	Include/Bullet/BulletConverter.h
	Include/Bullet/BulletDebugDraw.h
	Include/Bullet/Physics.h
	Include/Bullet/RigidBody.h
	Include/RhyBullet.h
	Source/Bullet/BulletDebugDraw.cpp
	Source/Bullet/Physics.cpp
	Source/Bullet/RigidBody.cpp
	)
	
target_include_directories (
	BulletPhysics
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include"
	PUBLIC ${BULLET_INCLUDE_DIRS}/
)

target_include_directories (BulletPhysics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (BulletPhysics PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	BulletPhysics 
	LINK_PUBLIC ${BULLET_LIBRARY}
	LINK_PUBLIC RhyCore
	)
