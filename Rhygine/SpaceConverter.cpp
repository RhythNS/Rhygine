#include "SpaceConverter.h"

RhyM::Vec3 RhyM::GetTangentFromTextureSpace(RhyM::Vec3 pos1, RhyM::Vec3 pos2, RhyM::Vec3 pos3, RhyM::Vec2 uv1, RhyM::Vec2 uv2, RhyM::Vec2 uv3)
{
	// taken from http://orwelldevcpp.blogspot.com/2011/03/normal-mapping-tutorial.html
	
	RhyM::Vec3 edge1 = pos2 - pos1;
	RhyM::Vec3 edge2 = pos3 - pos1;
	RhyM::Vec2 deltaUV1 = uv2 - uv1;
	RhyM::Vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	RhyM::Vec3 tangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	tangent.Normalize();

	return tangent;
}
