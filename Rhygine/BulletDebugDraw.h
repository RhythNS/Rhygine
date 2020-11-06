#pragma once
#include "Gameobject.h"
#include "Verticies.h"
#include "bullet\btBulletDynamicsCommon.h"
#include "RhyBindables.h"

#include <vector>
#include <memory>

class Stage;
class Bindable;

/// <summary>
/// Debug drawer for a bullet world. For more info check out the bullet
/// documentation.
/// </summary>
class BulletDebugDraw : public btIDebugDraw
{
public:
	void Init(Stage* stage);

	void setDefaultColors(const DefaultColors& colors);

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	void reportErrorWarning(const char* warningString);

	void draw3dText(const btVector3& location, const char* textString);

	void setDebugMode(int debugMode);

	int getDebugMode() const;

	void clearLines();

	void flushLines();
private:
	struct WorldPos {
		DirectX::XMMATRIX transform;
	};
	WorldPos worldBuffer;

	int debugMode = 0;
	Stage* stage;
	std::vector<std::unique_ptr<Bindable>> bindables;
	ConstantVS<WorldPos>* worldConstant;
	std::vector<VertexPosColor> verts;
};
