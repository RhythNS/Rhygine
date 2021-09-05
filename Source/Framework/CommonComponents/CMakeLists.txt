set ( 
	SHADER_VERT_FILES
	Shaders/BasicLitVert.hlsl
	Shaders/BasicVert.hlsl
	Shaders/PhongTexNormalMappingVert.hlsl
	Shaders/PhongTexVert.hlsl
	Shaders/TexVert.hlsl
	Shaders/ToonVert.hlsl
)
set ( 
	SHADER_PIX_FILES
	Shaders/BasicLitPix.hlsl
	Shaders/BasicPix.hlsl
	Shaders/PhongTexNormalMappingPix.hlsl
	Shaders/PhongTexPix.hlsl
	Shaders/TexPix.hlsl
	Shaders/ToonPix.hlsl
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
	CommonComponents
	Include/CC/Shaders/BasicTextureLit.h
	Include/CC/Shaders/TexLitNormalMapShader.h
	Include/CC/Shaders/TexLitShader.h
	Include/CC/Shaders/ToonShader.h
	Include/CC/DeleteWhenBelow.h
	Include/CC/TestLightComponent.h
	Include/CC/ShapeFactory.h
	Source/CC/Shaders/BasicTextureLit.cpp
	Source/CC/Shaders/TexLitNormalMapShader.cpp
	Source/CC/Shaders/TexLitShader.cpp
	Source/CC/Shaders/ToonShader.cpp
	Source/CC/DeleteWhenBelow.cpp
	Source/CC/TestLightComponent.cpp
    ${CC_VS_OUT}
    ${CC_PS_OUT}
	)
	
target_include_directories (CommonComponents PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (CommonComponents PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	CommonComponents 
	LINK_PUBLIC RhyCore
	)
	