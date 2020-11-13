#pragma once
#include "RhyMath.h"
#include "RhyColor.h"
#include "RhyBindables.h"
#include "Verticies.h"
#include "BlendStateBindable.h"
#include "StencilStates.h"

#include <vector>

class OrthographicCamera;
class TextureRegion;
class SpriteBatch
{
public:
	enum class SortMode {
		ZBased,
		TextureBased,
		None
	};

	SpriteBatch(SortMode sortMode = SortMode::ZBased, bool alphaBlending = false);

	void Begin(OrthographicCamera* camera);
	void Draw(Texture* texture, float texX, float texY, float texWidth, float texHeight, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	void Draw(TextureRegion* texture, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	void End();

	bool alphaBlending;
	bool noDepthBlending = true;
	SortMode sortMode = SortMode::TextureBased;
private:
	struct Sprite
	{
	public:
		struct {
			float x, y, z;
		} worldPosition;
		struct {
			float width, height;
		} worldSize;
		float rotation;
		struct {
			float x, y;
		} texturePosition;
		struct {
			float width, height;
		} textureSize;
		Texture* texture;
		RhyC::color color;

		Sprite(
			float x, float y, float z,
			float width, float height,
			float rotation,
			float texX, float texY,
			float texWidth, float texHeight,
			Texture* texture,
			RhyC::color color) :
			worldPosition({ x, y, z }),
			worldSize({ width, height }),
			rotation(rotation),
			texturePosition({ texX, texY }),
			textureSize({ texWidth,texHeight }),
			texture(texture),
			color(color)
		{}

		Sprite() {}

	};

	struct WorldPos {
		DirectX::XMMATRIX transform;
	};

	inline void GrowArray();

	inline void Sort();
	inline void CopySpritesToSorted();

	inline void DrawBatch(int from, int to);

	OrthographicCamera* camera;
	WorldPos constantBuffer;

	std::unique_ptr<IndexBufferUS> indexBuffer;
	std::unique_ptr<VertBuffer<VertexPosColorUV>> vertBuffer;
	std::unique_ptr<PrimitiveTopology> primitiveTopology;
	std::unique_ptr<Sampler> sampler;
	std::unique_ptr<VertShader> vertShader;
	std::unique_ptr<PixShader> pixShader;
	std::unique_ptr<InputLayout> inputLayout;
	std::unique_ptr<ConstantVS<WorldPos>> constantVert;
	std::unique_ptr<BlendStateBindable> blendState;
	std::unique_ptr<StencilStates> noDepthBuffer;

	std::vector<Sprite> sprites{ startingSize, Sprite() };
	std::vector<Sprite*> sortedSprites{ startingSize, nullptr };
	int currentBufferCount = 0;
	int maxBufferSize = startingSize;
	bool drawing = false;

	static const int startingSize = 50;
};
