add_library(
	Model3D
	Include/Model3D/Bone.h
	Include/Model3D/Mesh.h
	Include/Model3D/Model3DModule.h
	Include/Model3D/ModelLoader.h
	Include/Model3D/SkeletonMesh.h
	Include/RhyAssimp.h
	Source/Model3D/Bone.cpp
	Source/Model3D/Mesh.cpp
	Source/Model3D/Model3DModule.cpp
	Source/Model3D/ModelLoader.cpp
	Source/Model3D/SkeletonMesh.cpp
	)
	
target_include_directories (Model3D PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (Model3D PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	Model3D 
	LINK_PUBLIC assimp
	LINK_PUBLIC RhyCore
	)
	