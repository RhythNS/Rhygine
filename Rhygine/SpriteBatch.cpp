#include "SpriteBatch.h"
#include "TextureRegion.h"
#include "BasicShader.h"
#include "Gfx.h"

#include <algorithm>

SpriteBatch::SpriteBatch(SortMode sortMode, DrawMode drawMode, int initialCapacity) :
	indexBuffer(std::make_unique<IndexBufferUS>(0, initialCapacity * 4, 0)),
	vertBuffer(std::make_unique<VertBuffer<VertexPosColorUV>>(VertexPosColorUV(), initialCapacity * 6, 0)),
	primitiveTopology(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
	sampler(std::make_unique<Sampler>(0))
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	combinedShader = std::make_unique<BasicShader>(L"SpriteBatchPixel.hlsl", L"SpriteBatchVert.hlsl", &inputLayoutDesc);
}

void SpriteBatch::Begin(Camera* _camera)
{
	assert(drawing == false);
	drawing = true;

	currentBufferCount = 0;
	camera = _camera;
}

void SpriteBatch::Draw(Texture* texture, float texX, float texY, float texWidth, float texHeight, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color)
{
	assert(drawing == true);

	GrowArray();

	sprites[currentBufferCount++] = {
		position.m_floats[0], position.m_floats[1], position.m_floats[2], width, height, rotation,
		texX, texY, texWidth, texHeight, texture, color
	};
}

void SpriteBatch::Draw(TextureRegion* texture, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color)
{
	assert(drawing == true);

	GrowArray();

	sprites[currentBufferCount++] = {
		position.m_floats[0], position.m_floats[1], position.m_floats[2], width, height, rotation,
		texture->x, texture->y, texture->width, texture->height, texture->texture, color
	};
}

void SpriteBatch::End()
{
	assert(drawing == true);
	drawing = false;

	if (currentBufferCount == 0)
		return;

	Sort();

	std::vector<VertexPosColorUV> verts;
	verts.reserve(currentBufferCount * 4);
	std::vector<unsigned short> indexes;
	indexes.reserve(currentBufferCount * 6);

	for (size_t i = 0; i < currentBufferCount; i++)
	{
		auto* sprite = sortedSprites[i];
		verts[i].color = sprite->color;
		verts[i + 1].color = sprite->color;
		verts[i + 2].color = sprite->color;
		verts[i + 3].color = sprite->color;

		verts[i].pos.x = sprite->worldPosition.x;
		verts[i].pos.y = sprite->worldPosition.y;

		verts[i + 1].pos.x = sprite->worldPosition.x + sprite->worldSize.width;
		verts[i + 1].pos.y = sprite->worldPosition.y;

		verts[i + 2].pos.x = sprite->worldPosition.x;
		verts[i + 2].pos.y = sprite->worldPosition.y + sprite->worldSize.height;

		verts[i + 3].pos.x = sprite->worldPosition.x + sprite->worldSize.width;
		verts[i + 3].pos.y = sprite->worldPosition.y + sprite->worldSize.height;

		int texWidth = sprite->texture->GetWidth();
		int texHeight = sprite->texture->GetWidth();
		float uvWidth = sprite->textureSize.width / texWidth;
		float uvHeight = sprite->textureSize.height / texHeight;
		float u = sprite->texturePosition.x / texWidth;
		float v = sprite->texturePosition.y / texHeight;

		verts[i].u = u;
		verts[i].v = v;

		verts[i + 1].u = u + uvWidth;
		verts[i + 1].v = v;

		verts[i + 2].u = u;
		verts[i + 2].v = v + uvHeight;

		verts[i + 3].u = u + uvWidth;
		verts[i + 3].v = v + uvHeight;

		indexes[i * 6] = i;
		indexes[i * 6 + 1] = i + 2;
		indexes[i * 6 + 2] = i + 1;
		indexes[i * 6 + 3] = i + 2;
		indexes[i * 6 + 4] = i + 3;
		indexes[i * 6 + 5] = i + 1;
	}

	vertBuffer->UpdateVerts(verts);
	indexBuffer->UpdateIndexes(indexes);

	indexBuffer->Bind();
	vertBuffer->Bind();
	primitiveTopology->Bind();
	sampler->Bind();
	combinedShader->Bind();
	
	int lastSameTexture = 0;
	int currentDraw = 1;
	while (currentDraw < currentBufferCount)
	{
		if (sortedSprites[lastSameTexture] != sortedSprites[currentDraw])
		{
			DrawBatch(lastSameTexture, currentDraw);
			lastSameTexture = currentDraw;
		}
		++currentDraw;
	}
	DrawBatch(lastSameTexture, currentBufferCount);
}

inline void SpriteBatch::GrowArray()
{
	if (currentBufferCount + 1 < maxBufferSize)
		return;

	maxBufferSize *= 2;

	indexBuffer.reset();
	vertBuffer.reset();

	indexBuffer = std::make_unique<IndexBufferUS>(0, maxBufferSize * 4, 0);
	vertBuffer = std::make_unique<VertBuffer<VertexPosColorUV>>(VertexPosColorUV(), maxBufferSize * 6, 0);

	sprites.resize(maxBufferSize);
	sortedSprites.resize(maxBufferSize);
}

inline void SpriteBatch::Sort()
{
	switch (sortMode)
	{
	case SpriteBatch::SortMode::ZBased:
		CopySpritesToSorted();
		std::sort(sortedSprites.begin(), sortedSprites.begin() + currentBufferCount,
			[](Sprite* first, Sprite* second) -> bool
			{
				return first->worldPosition.z > second->worldPosition.z;
			}
		);

		break;

	case SpriteBatch::SortMode::TextureBased:
		CopySpritesToSorted();
		std::sort(sortedSprites.begin(), sortedSprites.begin() + currentBufferCount,
			[](Sprite* first, Sprite* second) -> bool
			{
				return first->texture > second->texture;
			}
		);

		break;

	case SpriteBatch::SortMode::None:
		return;

	default:
		assert(false);
	}
}

inline void SpriteBatch::CopySpritesToSorted()
{
	for (int i = 0; i < currentBufferCount; i++)
	{
		sortedSprites[i] = &sprites[i];
	}
}

inline void SpriteBatch::DrawBatch(int from, int to)
{
	Gfx::GetInstance()->DrawIndexed((to - from) * 6, from * 6);
}
