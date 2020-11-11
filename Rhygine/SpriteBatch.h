#pragma once
#include "RhyMath.h"
#include "RhyColor.h"
#include "RhyBindables.h"
#include "Verticies.h"

#include <vector>

class Camera;
class TextureRegion;
class SpriteBatch
{
public:
	enum class SortMode {
		ZBased,
		TextureBased,
		None
	};
	enum class DrawMode {
		Perspective,
		Orthographic
	};

	SpriteBatch(SortMode sortMode = SortMode::ZBased, DrawMode drawMode = DrawMode::Orthographic, int initialCapacity = startingSize);

	void Begin(Camera* camera);
	void Draw(Texture* texture, float texX, float texY, float texWidth, float texHeight, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	void Draw(TextureRegion* texture, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	void End();

	bool alphaBlending;
	SortMode sortMode = SortMode::TextureBased;
	DrawMode drawMode = DrawMode::Orthographic;
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

	inline void GrowArray();

	inline void Sort();
	inline void CopySpritesToSorted();

	inline void DrawBatch(int from, int to);

	Camera* camera;

	std::unique_ptr<IndexBufferUS> indexBuffer;
	std::unique_ptr<VertBuffer<VertexPosColorUV>> vertBuffer;
	std::unique_ptr<PrimitiveTopology> primitiveTopology;
	std::unique_ptr<Sampler> sampler;
	std::unique_ptr<CombinedShader> combinedShader;

	std::vector<Sprite> sprites{ startingSize, Sprite() };
	std::vector<Sprite*> sortedSprites{ startingSize, nullptr };
	int currentBufferCount = 0;
	int maxBufferSize = startingSize;
	bool drawing = false;

	static const int startingSize = 50;
};
