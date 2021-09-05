#pragma once
#include <Core/Module/Module.h>

class ModelLoader;

class Model3DModule : public Module
{
public:
	virtual ~Model3DModule();

	ModelLoader* GetModelLoader();

protected:
	virtual void Setup() override;

	ModelLoader* modelLoader;
};
