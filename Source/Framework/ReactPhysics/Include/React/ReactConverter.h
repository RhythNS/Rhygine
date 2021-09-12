#pragma once
#include <RhyMath.h>
#include <React/Ray.h>
#include <React/RaycastInfo.h>
#include <React/Collider/ReactCollider.h>
#include <React/ReactBodyBase.h>
#include <reactphysics3d/mathematics/Ray.h>
#include <reactphysics3d/mathematics/Ray.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/collision/RaycastInfo.h>

namespace RhyReact
{
	static inline RhyM::Quat QuatReactToRhy(reactphysics3d::Quaternion quaternion)
	{
		return RhyM::Quat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

	static inline reactphysics3d::Quaternion QuatRhyToReact(RhyM::Quat quat)
	{
		return reactphysics3d::Quaternion(quat.x, quat.y, quat.z, quat.w);
	}

	static inline RhyM::Vec3 Vec3ReactToRhy(reactphysics3d::Vector3 vector3)
	{
		return RhyM::Vec3(vector3.x, vector3.y, vector3.z);
	}

	static inline reactphysics3d::Vector3 Vec3RhyToReact(RhyM::Vec3 vec3)
	{
		return reactphysics3d::Vector3(vec3.x, vec3.y, vec3.z);
	}

	static inline reactphysics3d::Transform TransformRhyToReact(RhyM::Vec3 pos, RhyM::Quat quat)
	{
		return reactphysics3d::Transform(Vec3RhyToReact(pos), QuatRhyToReact(quat));
	}

	static inline void TransformReactToRhy(reactphysics3d::Transform trans, RhyM::Vec3& outPos, RhyM::Quat& outQuat)
	{
		outPos = Vec3ReactToRhy(trans.getPosition());
		outQuat = QuatReactToRhy(trans.getOrientation());
	}

	static inline RhyReact::Ray RayReactToRhy(reactphysics3d::Ray ray)
	{
		return RhyReact::Ray{ Vec3ReactToRhy(ray.point1), Vec3ReactToRhy(ray.point2), ray.maxFraction };
	}

	static inline reactphysics3d::Ray RayRhyToReact(RhyReact::Ray ray)
	{
		return reactphysics3d::Ray{ Vec3RhyToReact(ray.origin), Vec3RhyToReact(ray.endPoint), ray.maxFraction };
	}

	static inline RhyReact::RaycastInfo RaycastInfoReactToRhy(reactphysics3d::RaycastInfo& info)
	{
		return RhyReact::RaycastInfo{ Vec3ReactToRhy(info.worldPoint), Vec3ReactToRhy(info.worldNormal), info.hitFraction, info.meshSubpart,
			info.triangleIndex, static_cast<ReactBodyBase*>(info.body->getUserData()), static_cast<ReactCollider*>(info.collider->getUserData()) };
	}
}
