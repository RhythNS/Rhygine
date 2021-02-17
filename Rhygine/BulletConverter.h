#pragma once
#include "Physics.h"
#include "RhyMath.h"

static btQuaternion QuatToBullet(RhyM::Quat toConvert)
{
	return btQuaternion(toConvert.x, toConvert.y, toConvert.z, toConvert.w);
}

static btVector3 Vec3ToBullet(RhyM::Vec3 toConvert)
{
	return btVector3(toConvert.x, toConvert.y, toConvert.z);
}

static RhyM::Quat QuatToRhy(btQuaternion toConvert)
{
	return RhyM::Quat(toConvert.x(), toConvert.y(), toConvert.z(), toConvert.w());
}

static RhyM::Vec3 Vec3ToRhy(btVector3 toConvert)
{
	return RhyM::Vec3(toConvert.m_floats[0], toConvert.m_floats[1], toConvert.m_floats[2]);
}
