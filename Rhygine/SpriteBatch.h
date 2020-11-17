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
/// <summary>
/// Collects all images that are drawn in a frame. At the end of a frame, it sorts them and then
/// uploads them to Direct3D in a single call, while trying to use as few draw calls as possible.
/// </summary>
class SpriteBatch
{
public:
	enum class SortMode {
		ZBased,
		TextureBased,
		None
	};

	/// <summary>
	/// Standard Constructor.
	/// </summary>
	/// <param name="sortMode">How the sprites are sorted at the end of the frame.</param>
	/// <param name="alphaBlending">Wheter to use alphaBlending or not.</param>
	SpriteBatch(SortMode sortMode = SortMode::None, bool alphaBlending = true);

	/// <summary>
	/// Should be called at the start of a frame, bevor adding any sprites and after End has already been called.
	/// </summary>
	/// <param name="camera">The camera for position and rotation data.</param>
	void Begin(OrthographicCamera* camera);
	/// <summary>
	/// Draws a texture partially onto the screen.
	/// </summary>
	/// <param name="texture">A reference to the texture.</param>
	/// <param name="texX">The x coordinate of the rectangle that is used to draw the partial texture.</param>
	/// <param name="texY">The y coordinate of the rectangle that is used to draw the partial texture.</param>
	/// <param name="texWidth">The width of the rectangle that is used to draw the partial texture.</param>
	/// <param name="texHeight">The height of the rectangle that is used to draw the partial texture.</param>
	/// <param name="position">The position in world space where the sprite is being drawn to.</param>
	/// <param name="width">The width of the sprite that is drawn.</param>
	/// <param name="height">The height of the sprite that is drawn.</param>
	/// <param name="rotation">The rotation in rad.</param>
	/// <param name="color">The color of the sprite.</param>
	void Draw(Texture* texture, float texX, float texY, float texWidth, float texHeight, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	/// <summary>
	/// Draws a texture region onto the screen.
	/// </summary>
	/// <param name="texture"A reference to the texture region.</param>
	/// <param name="position">The position in world space where the sprite is being drawn to.</param>
	/// <param name="width">The width of the sprite that is drawn.</param>
	/// <param name="height">The height of the sprite that is drawn.</param>
	/// <param name="rotation">The rotation in rad.</param>
	/// <param name="color">The color of the sprite.</param>
	void Draw(TextureRegion* texture, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color);
	/// <summary>
	/// Draws a texture region onto the screen.
	/// </summary>
	/// <param name="texture">A reference to the texture region.</param>
	/// <param name="x">The x coordinate of the sprite.</param>
	/// <param name="y">The y coordinate of the sprite.</param>
	/// <param name="z">The z coordinate of the sprite.</param>
	/// <param name="width">The width of the sprite that is drawn.</param>
	/// <param name="height">The height of the sprite that is drawn.</param>
	/// <param name="rotation">The rotation in rad.</param>
	/// <param name="color">The color of the sprite.</param>
	void Draw(TextureRegion* texture, float x, float y, float z, float width, float height, float rotation, RhyC::color color);
	/// <summary>
	/// Draws all sprites onto the screen.
	/// </summary>
	void End();

	/// <summary>
	/// Wheter alpha blending should be used.
	/// </summary>
	bool alphaBlending;
	/// <summary>
	/// Wheter the z buffer is turned off when drawing sprites.
	/// </summary>
	bool noDepthBlending = true;
	SortMode sortMode = SortMode::None;
private:
	/// <summary>
	/// Information of the sprites that should be drawn onto screen.
	/// </summary>
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

		Sprite() :
			worldPosition({ 0,0,0 }),
			worldSize({ 0,0 }),
			rotation(0),
			texturePosition({ 0,0 }),
			textureSize({ 0,0 }),
			texture(nullptr),
			color(0)
		{}

	};

	struct WorldPos {
		DirectX::XMMATRIX transform;
	};

	/// <summary>
	/// Grows the backing array, vertex buffer and index buffer if needed.
	/// </summary>
	inline void GrowArray();

	/// <summary>
	/// Sorts all sprites in the sortedSprites list.
	/// </summary>
	inline void Sort();
	/// <summary>
	/// Copies all sprites to sortedSprites.
	/// </summary>
	inline void CopySpritesToSorted();

	/// <summary>
	/// Draws a batch of sprites with the same texture.
	/// </summary>
	/// <param name="from">The inclusive from index of sortedSprites.</param>
	/// <param name="to">The exclusive to index of sortedSprites.</param>
	inline void DrawBatch(int from, int to);

	OrthographicCamera* camera;
	WorldPos constantBuffer;
	
	// ------ All required bindables ------
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
	// ------------------------------------

	std::vector<Sprite> sprites{ startingSize, Sprite() };
	std::vector<Sprite*> sortedSprites{ startingSize, nullptr };
	int currentBufferCount = 0;
	int maxBufferSize = startingSize;
	bool drawing = false;

	/// <summary>
	/// The starting size of the sprite array.
	/// </summary>
	static const int startingSize = 50;
};
