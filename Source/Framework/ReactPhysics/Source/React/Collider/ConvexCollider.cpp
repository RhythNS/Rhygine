#include <React/Collider/ConvexCollider.h>
#include <reactphysics3d/collision/shapes/ConvexShape.h>

float RhyReact::ConvexCollider::GetMargin()
{
	return GetConvexShape()->getMargin();
}
