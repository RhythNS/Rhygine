#include "TestScene.h"
#include "World.h"
#include "Components.h"

void Rhygine::TestScene::OnLoad(World& t_world)
{
	EntityId id = t_world.CreateEntity(0);
	t_world.AddComponent<Transform>(id, nullptr);
	//t_world.AddComponent<MeshRenderer>(id, nullptr);
}
