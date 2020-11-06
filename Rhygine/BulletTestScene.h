#pragma once
#include "Scene.h"

#include "bullet\btBulletDynamicsCommon.h"

/// <summary>
/// For testing Bullet and physics related stuff.
/// </summary>
class BulletTestScene : public Scene
{
public:
	void InnerInit();
};
