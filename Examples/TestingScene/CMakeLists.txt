add_executable (
	TestingScene WIN32
	Include/Bindables/TextureChanger.h
	Include/Components/AwakeRigidbodies.h
	Include/Components/BulletShowcaser.h
	Include/Components/ChangeSceneTest.h
	Include/Components/CoroutineTestComponent.h
	Include/Components/InfoDisplayer.h
	Include/Components/ParallelPrint.h
	Include/Components/RinModelShower.h
	Include/Components/RotateAround.h
	Include/Components/SpriteBatchTester.h
	Include/Components/TestLitPlateComponent.h
	Include/Components/TestParallelAsUpdate.h
	Include/Components/TestParallelMaker.h
	Include/Components/TestPhysicsMover.h
	Include/Components/UITester.h
	Include/Factories/TestBasicLit.h
	Include/Factories/TestCrystal.h
	Include/Factories/TestFloor.h
	Include/Factories/TestLight.h
	Include/Factories/TestLitPlate.h
	Include/Factories/TestModel.h
	Include/Factories/TestModelLoader.h
	Include/Factories/TestNeptuneLoader.h
	Include/Factories/TestPyramid.h
	Include/Factories/TestTexture.h
	Include/Opt/OptBasic.h
	Include/Opt/OptBatch.h
	Include/Opt/OptModel.h
	Include/Opt/OptNep.h
	Include/Present/PresentResources.h
	Include/Present/PresentVNManager.h
	Include/Present/VNCharacter.h
	Include/Present/VNOpener.h
	Include/Present/VNTextbox.h
	Include/Scenes/BulletTestScene.h
	Include/Scenes/LightTestScene.h
	Include/Scenes/MemoryLeakTestScene.h
	Include/Scenes/NewFeatureTestScene.h
	Include/Scenes/OptScene.h
	Include/Scenes/PresentScene.h
	Include/Scenes/TestScene.h
	Source/Bindables/TextureChanger.cpp
	Source/Components/AwakeRigidbodies.cpp
	Source/Components/BulletShowcaser.cpp
	Source/Components/ChangeSceneTest.cpp
	Source/Components/CoroutineTestComponent.cpp
	Source/Components/InfoDisplayer.cpp
	Source/Components/ParallelPrint.cpp
	Source/Components/RinModelShower.cpp
	Source/Components/RotateAround.cpp
	Source/Components/SpriteBatchTester.cpp
	Source/Components/TestLitPlateComponent.cpp
	Source/Components/TestParallelAsUpdate.cpp
	Source/Components/TestParallelMaker.cpp
	Source/Components/TestPhysicsMover.cpp
	Source/Components/UITester.cpp
	Source/Factories/TestBasicLit.cpp
	Source/Factories/TestCrystal.cpp
	Source/Factories/TestFloor.cpp
	Source/Factories/TestLight.cpp
	Source/Factories/TestLitPlate.cpp
	Source/Factories/TestModel.cpp
	Source/Factories/TestModelLoader.cpp
	Source/Factories/TestNeptuneLoader.cpp
	Source/Factories/TestPyramid.cpp
	Source/Factories/TestTexture.cpp
	Source/Opt/OptBasic.cpp
	Source/Opt/OptBatch.cpp
	Source/Opt/OptModel.cpp
	Source/Opt/OptNep.cpp
	Source/Present/PresentResources.cpp
	Source/Present/PresentVNManager.cpp
	Source/Present/VNCharacter.cpp
	Source/Present/VNOpener.cpp
	Source/Present/VNTextbox.cpp
	Source/Scenes/BulletTestScene.cpp
	Source/Scenes/LightTestScene.cpp
	Source/Scenes/MemoryLeakTestScene.cpp
	Source/Scenes/NewFeatureTestScene.cpp
	Source/Scenes/OptScene.cpp
	Source/Scenes/PresentScene.cpp
	Source/Scenes/TestScene.cpp
	Source/WinMain.cpp
)

target_include_directories (TestingScene PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (TestingScene PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	TestingScene
	LINK_PUBLIC RhyCore
	LINK_PUBLIC CommonComponents
	LINK_PUBLIC BulletPhysics
	LINK_PUBLIC ImGUI
	LINK_PUBLIC Model3D
)

RHY_COPY_ASSETS (TestingScene ${CMAKE_CURRENT_SOURCE_DIR})
