#include "Model3DModule.h"
#include "ModelLoader.h"

Model3DModule::~Model3DModule()
{
	delete modelLoader;
}

ModelLoader* Model3DModule::GetModelLoader()
{
	return modelLoader;
}

void Model3DModule::Setup()
{
	modelLoader = new ModelLoader();
}
