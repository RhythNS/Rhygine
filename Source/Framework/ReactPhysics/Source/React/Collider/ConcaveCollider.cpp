#include <React/Collider/ConcaveCollider.h>
#include <React/ReactConverter.h>
#include <ECS/Components/Transform.h>

using namespace RhyReact;

reactphysics3d::TriangleRaycastSide ConcaveCollider::GetRaycastTestType()
{
	return GetConcaveShape()->getRaycastTestType();
}

void ConcaveCollider::SetRaycastTestType(reactphysics3d::TriangleRaycastSide testType)
{
	GetConcaveShape()->setRaycastTestType(testType);
}

const RhyM::Vec3 ConcaveCollider::GetScale()
{
	return Vec3ReactToRhy(GetConcaveShape()->getScale());
}

void ConcaveCollider::SetScale(const RhyM::Vec3& scale)
{
	GetTransform()->SetWorldScale(scale);
	GetConcaveShape()->setScale(Vec3RhyToReact(scale));
}
