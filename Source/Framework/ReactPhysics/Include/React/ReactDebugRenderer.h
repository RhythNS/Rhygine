#pragma once
#include <RhyBindables.h>
#include <ECS/Bindable/Verticies.h>

#include <vector>
#include <memory>

namespace reactphysics3d { class DebugRenderer; struct Vector3; }

class Stage;
class Bindable;

namespace RhyReact
{
	class ReactDebugRenderer
	{
	public:
		void Init(Stage* stage);

		void Draw(reactphysics3d::DebugRenderer& debugRenderer);

	private:
		void PushVertex(const reactphysics3d::Vector3& pos, const unsigned int& color);

		struct WorldPos {
			DirectX::XMMATRIX transform;
		};
		WorldPos worldBuffer{};

		Stage* stage = nullptr;
		std::vector<std::unique_ptr<Bindable>> bindables;
		ConstantVS<WorldPos>* worldConstant = nullptr;
		std::vector<VertexPosRhyColor> verts;
	};
}
