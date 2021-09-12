#include <React/ReactBodyBase.h>
#include <Core/Log.h>
#include <React/ReactConverter.h>
#include <React/ReactPhysicsModule.h>
#include <ECS/Components/Transform.h>

using namespace RhyReact;

ReactBodyBase::~ReactBodyBase()
{
	for (int i = 0; i < colliders.size(); i++)
	{
		InnerRemoveCollider(colliders[i].collider);

		if (colliders[i].isOwned == true)
			delete colliders[i].collider;
	}

	colliders.clear();
}

void ReactBodyBase::Move(RhyM::Vec3 position, RhyM::Quat quat)
{
	assert(collisionBody);
	reactphysics3d::Transform trans = TransformRhyToReact(position, quat);
	collisionBody->setTransform(trans);
}

void ReactBodyBase::GetPositionAndRotation(RhyM::Vec3& position, RhyM::Quat& quat) const
{
	assert(collisionBody);
	TransformReactToRhy(collisionBody->getTransform(), position, quat);
}

void ReactBodyBase::SetPositionAndRotation(const RhyM::Vec3& position, const RhyM::Quat& quat)
{
	assert(collisionBody);
	collisionBody->setTransform(TransformRhyToReact(position, quat));
}

bool ReactBodyBase::TestPointInside(const RhyM::Vec3& worldPoint) const
{
	assert(collisionBody);
	return collisionBody->testPointInside(Vec3RhyToReact(worldPoint));
}

bool ReactBodyBase::Raycast(const Ray& ray, RaycastInfo& raycastInfo)
{
	assert(collisionBody);
	reactphysics3d::Ray reactRay = RayRhyToReact(ray);
	reactphysics3d::RaycastInfo info;
	if (collisionBody->raycast(reactRay, info))
	{
		raycastInfo = RaycastInfoReactToRhy(info);
		return true;
	}
	return false;
}

RhyM::Vec3 ReactBodyBase::GetWorldPoint(const RhyM::Vec3& localPoint) const
{
	assert(collisionBody);
	return Vec3ReactToRhy(collisionBody->getWorldPoint(Vec3RhyToReact(localPoint)));
}

RhyM::Vec3 ReactBodyBase::GetWorldVector(const RhyM::Vec3& localVector) const
{
	assert(collisionBody);
	return Vec3ReactToRhy(collisionBody->getWorldVector(Vec3RhyToReact(localVector)));
}

RhyM::Vec3 ReactBodyBase::GetLocalPoint(const RhyM::Vec3& worldPoint) const
{
	assert(collisionBody);
	return Vec3ReactToRhy(collisionBody->getLocalPoint(Vec3RhyToReact(worldPoint)));
}

RhyM::Vec3 ReactBodyBase::GetLocalVector(const RhyM::Vec3& worldVector) const
{
	assert(collisionBody);
	return Vec3ReactToRhy(collisionBody->getLocalVector(Vec3RhyToReact(worldVector)));
}

bool ReactBodyBase::AddCollider(ReactCollider* collider, const RhyM::Vec3& position, const RhyM::Quat& quat, const bool transferOwnerShip)
{
	if (collider->collider != nullptr)
	{
		LOG_ERROR("Tried to add a collider that was already attached to a ReactBody!");
		return false;
	}

	collider->collider = AddCollider(collider->GetCollisionShape(), position, quat);
	collider->collider->setUserData(collider);
	collider->attachedTo = this;
	colliders.push_back({ collider, transferOwnerShip });
	return true;
}

int ReactBodyBase::GetColliderNum()
{
	return colliders.size();
}

ReactCollider* ReactBodyBase::GetColliderAt(int index)
{
	assert(index > -1 && index < colliders.size());
	return colliders[index].collider;
}

Transform* ReactBodyBase::GetTransform()
{
	return transform;
}

void ReactBodyBase::RemoveCollider(ReactCollider* collider)
{
	assert(collisionBody && collider);

	for (int i = 0; i < colliders.size(); i++)
	{
		if (colliders[i].collider == collider)
		{
			InnerRemoveCollider(collider);
			
			if (colliders[i].isOwned == true)
				delete collider;

			colliders.erase(colliders.begin() + i);
			return;
		}
	}

	LOG_ERROR("Could not remove collider. Not found in list!");
}

reactphysics3d::PhysicsWorld* ReactBodyBase::GetWorld()
{
	return ReactPhysicsModule::instance->world;
}

reactphysics3d::PhysicsCommon* ReactBodyBase::GetCommon()
{
	return &ReactPhysicsModule::instance->common;
}

reactphysics3d::Collider* ReactBodyBase::AddCollider(reactphysics3d::CollisionShape* collisionShape, const RhyM::Vec3& position, const RhyM::Quat& quat)
{
	assert(collisionBody && collisionShape);
	return collisionBody->addCollider(collisionShape, TransformRhyToReact(position, quat));
}

void ReactBodyBase::InnerRemoveCollider(ReactCollider* toRemove)
{
	assert(collisionBody && toRemove);
	collisionBody->removeCollider(toRemove->collider);
	toRemove->attachedTo = nullptr;
	if (toRemove->isShared == false)
		toRemove->Release(&ReactPhysicsModule::instance->common);
	toRemove->collider = nullptr;
}
