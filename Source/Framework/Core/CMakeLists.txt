set (RHY_LOG_LEVEL 2 CACHE STRING "spdlog log level")

message ("Log level is:  ${RHY_LOG_LEVEL}")
add_compile_definitions (RHY_LOG_LEVEL=${RHY_LOG_LEVEL})

set ( 
	SHADER_VERT_FILES
	Shaders/SpriteBatchVert.hlsl
)
set ( 
	SHADER_PIX_FILES
	Shaders/SpriteBatchPixel.hlsl
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
	RhyCore
	Include/2D/Animation.h
	Include/2D/MonoFont.h
	Include/2D/OrthographicCamera.h
	Include/2D/SpriteBatch.h
	Include/2D/TextureGenerator.h
	Include/2D/TextureRegion.h
	Include/Core/Module/IWin32MessageHandler.h
	Include/Core/Module/Module.h
	Include/Core/Module/PhysicsModule.h
	Include/Core/Gfx.h
	Include/Core/Log.h
	Include/Core/Scene.h
	Include/Core/Tickable.h
	Include/Core/Time.h
	Include/Core/Window.h
	Include/ECS/Bindable/BasicShader.h
	Include/ECS/Bindable/Bindable.h
	Include/ECS/Bindable/BlendStateBindable.h
	Include/ECS/Bindable/CombinedShader.h
	Include/ECS/Bindable/ConstantBuffer.h
	Include/ECS/Bindable/Drawer.h
	Include/ECS/Bindable/DrawerUpdatable.h
	Include/ECS/Bindable/GeoShader.h
	Include/ECS/Bindable/IndexBuffer.h
	Include/ECS/Bindable/InputLayout.h
	Include/ECS/Bindable/PixShader.h
	Include/ECS/Bindable/PrimitiveTopolpgy.h
	Include/ECS/Bindable/Sampler.h
	Include/ECS/Bindable/StencilStates.h
	Include/ECS/Bindable/Texture.h
	Include/ECS/Bindable/UnBindable.h
	Include/ECS/Bindable/VertBuffer.h
	Include/ECS/Bindable/Verticies.h
	Include/ECS/Bindable/VertIndexes.h
	Include/ECS/Bindable/VertShader.h
	Include/ECS/Components/Camera.h
	Include/ECS/Components/CameraMover.h
	Include/ECS/Components/Transform.h
	Include/ECS/Coroutine/Coroutine.h
	Include/ECS/Coroutine/WaitForSeconds.h
	Include/ECS/Coroutine/YieldInstruction.h
	Include/ECS/Coroutine/Yielder.h
	Include/ECS/Component.h
	Include/ECS/Drawable.h
	Include/ECS/GameObject.h
	Include/ECS/GameObjectFactory.h
	Include/ECS/LateUpdatable.h
	Include/ECS/ParallelUpdatable.h
	Include/ECS/Stage.h
	Include/ECS/Updatable.h
	Include/Input/Keys.h
	Include/Input/Mouse.h
	Include/Math/Matrix3x3.hpp
	Include/Math/Quaternion.hpp
	Include/Math/Random.h
	Include/Math/Rect.h
	Include/Math/SpaceConverter.h
	Include/Math/Vector2.hpp
	Include/Math/Vector2Int.hpp
	Include/Math/Vector3.hpp
	Include/Task/TaskManager.h
	Include/UI/Elements/UIButton.h
	Include/UI/Elements/UIContainer.h
	Include/UI/Elements/UIFont.h
	Include/UI/Elements/UIImage.h
	Include/UI/Elements/UIRootElement.h
	Include/UI/Sizers/UIFitToParentRelativeSizer.h
	Include/UI/Sizers/UIFitToParentSizer.h
	Include/UI/Sizers/UIHoriGroupSizer.h
	Include/UI/Sizers/UIKeepWidthSizer.h
	Include/UI/Sizers/UILocalSizer.h
	Include/UI/Sizers/UIRootSizer.h
	Include/UI/Sizers/UIVertGroupSizer.h
	Include/UI/UIChildSizer.h
	Include/UI/UIElement.h
	Include/UI/UIOwnSizer.h
	Include/RhyBindables.h
	Include/RhyColor.h
	Include/RhyCoroutine.h
	Include/RhyException.h
	Include/RhyMath.h
	Include/RhySTB.h
	Include/RhyWin.h
	Source/2D/MonoFont.cpp
	Source/2D/OrthographicCamera.cpp
	Source/2D/SpriteBatch.cpp
	Source/2D/TextureGenerator.cpp
	Source/Core/Module/IWin32MessageHandler.cpp
	Source/Core/Module/Module.cpp
	Source/Core/Module/PhysicsModule.cpp
	Source/Core/Gfx.cpp
	Source/Core/Log.cpp
	Source/Core/Scene.cpp
	Source/Core/Time.cpp
	Source/Core/Window.cpp
	Source/ECS/Bindable/BasicShader.cpp
	Source/ECS/Bindable/Bindable.cpp
	Source/ECS/Bindable/BlendStateBindable.cpp
	Source/ECS/Bindable/CombinedShader.cpp
	Source/ECS/Bindable/Drawer.cpp
	Source/ECS/Bindable/GeoShader.cpp
	Source/ECS/Bindable/InputLayout.cpp
	Source/ECS/Bindable/PixShader.cpp
	Source/ECS/Bindable/PrimitiveTopolpgy.cpp
	Source/ECS/Bindable/Sampler.cpp
	Source/ECS/Bindable/StencilStates.cpp
	Source/ECS/Bindable/Texture.cpp
	Source/ECS/Bindable/VertShader.cpp
	Source/ECS/Components/Camera.cpp
	Source/ECS/Components/CameraMover.cpp
	Source/ECS/Components/Transform.cpp
	Source/ECS/Coroutine/Coroutine.cpp
	Source/ECS/Coroutine/Generator.h
	Source/ECS/Coroutine/WaitForSeconds.cpp
	Source/ECS/Component.cpp
	Source/ECS/GameObject.cpp
	Source/ECS/Stage.cpp
	Source/Input/Keys.cpp
	Source/Input/Mouse.cpp
	Source/Math/Rect.cpp
	Source/Math/SpaceConverter.cpp
	Source/Task/TaskManager.cpp
	Source/Task/Worker.cpp
	Source/Task/Worker.h
	Source/UI/Elements/UIButton.cpp
	Source/UI/Elements/UIFont.cpp
	Source/UI/Elements/UIImage.cpp
	Source/UI/Elements/UIRootElement.cpp
	Source/UI/Sizers/UIFitToParentRelativeSizer.cpp
	Source/UI/Sizers/UIFitToParentSizer.cpp
	Source/UI/Sizers/UIHoriGroupSizer.cpp
	Source/UI/Sizers/UIKeepWidthSizer.cpp
	Source/UI/Sizers/UIRootSizer.cpp
	Source/UI/Sizers/UIVertGroupSizer.cpp
	Source/UI/UIChildSizer.cpp
	Source/UI/UIElement.cpp
	Source/UI/UIOwnSizer.cpp
    ${CC_VS_OUT}
    ${CC_PS_OUT}
	)

target_include_directories (RhyCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (RhyCore PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	RhyCore
	LINK_PUBLIC stb
	LINK_PUBLIC spdlog
)
