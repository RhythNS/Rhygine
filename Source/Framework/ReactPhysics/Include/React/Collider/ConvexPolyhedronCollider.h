#pragma once

#include <React/Collider/ConvexCollider.h>
#include <reactphysics3d/collision/HalfEdgeStructure.h>
#include <RhyMath.h>

namespace reactphysics3d { class ConvexPolyhedronShape; class ConvexShape; }

namespace RhyReact
{
	class ConvexPolyhedronCollider : public ConvexCollider
	{
	public:
		virtual unsigned int GetNumFaces();

		virtual const reactphysics3d::HalfEdgeStructure::Face& GetFace(unsigned int faceIndex);

		virtual unsigned int GetNumVertices();

		virtual reactphysics3d::HalfEdgeStructure::Vertex GetVertex(unsigned int vertexIndex);

		virtual RhyM::Vec3 GetVertexPosition(unsigned int vertexIndex);

		virtual RhyM::Vec3 GetFaceNormal(unsigned int faceIndex);

		virtual unsigned int GetNumHalfEdges();

		virtual const reactphysics3d::HalfEdgeStructure::Edge& GetHalfEdge(unsigned int edgeIndex);

	protected:
		virtual void Release(reactphysics3d::PhysicsCommon* common) = 0;

		virtual reactphysics3d::CollisionShape* GetCollisionShape() = 0;

		virtual reactphysics3d::ConvexPolyhedronShape* GetConvexPolyhedronShape() = 0;

		virtual reactphysics3d::ConvexShape* GetConvexShape() override;
	};
}
