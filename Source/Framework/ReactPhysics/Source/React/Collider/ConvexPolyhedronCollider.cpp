#include <React/Collider/ConvexPolyhedronCollider.h>
#include <reactphysics3d/collision/shapes/ConvexPolyhedronShape.h>
#include <React/ReactConverter.h>

using namespace RhyReact;

unsigned int ConvexPolyhedronCollider::GetNumFaces()
{
	return GetConvexPolyhedronShape()->getNbFaces();
}

const reactphysics3d::HalfEdgeStructure::Face& ConvexPolyhedronCollider::GetFace(unsigned int faceIndex)
{
	return GetConvexPolyhedronShape()->getFace(faceIndex);
}

unsigned int ConvexPolyhedronCollider::GetNumVertices()
{
	return GetConvexPolyhedronShape()->getNbVertices();
}

reactphysics3d::HalfEdgeStructure::Vertex ConvexPolyhedronCollider::GetVertex(unsigned int vertexIndex)
{
	return GetConvexPolyhedronShape()->getVertex(vertexIndex);
}

RhyM::Vec3 ConvexPolyhedronCollider::GetVertexPosition(unsigned int vertexIndex)
{
	return Vec3ReactToRhy(GetConvexPolyhedronShape()->getVertexPosition(vertexIndex));
}

RhyM::Vec3 ConvexPolyhedronCollider::GetFaceNormal(unsigned int faceIndex)
{
	return Vec3ReactToRhy(GetConvexPolyhedronShape()->getFaceNormal(faceIndex));
}

unsigned int ConvexPolyhedronCollider::GetNumHalfEdges()
{
	return GetConvexPolyhedronShape()->getNbHalfEdges();
}

const reactphysics3d::HalfEdgeStructure::Edge& ConvexPolyhedronCollider::GetHalfEdge(unsigned int edgeIndex)
{
	return GetConvexPolyhedronShape()->getHalfEdge(edgeIndex);
}

reactphysics3d::ConvexShape* ConvexPolyhedronCollider::GetConvexShape()
{
	return GetConvexPolyhedronShape();
}

