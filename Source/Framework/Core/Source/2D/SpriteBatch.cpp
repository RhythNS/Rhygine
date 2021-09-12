#include <2D/SpriteBatch.h>
#include <2D/TextureRegion.h>
#include <ECS/Bindable/BasicShader.h>
#include <ECS/Bindable/CombinedShader.h>
#include <ECS/Bindable/VertBuffer.h>
#include <Core/Gfx.h>
#include <2D/OrthographicCamera.h>

#include <algorithm>

SpriteBatch::SpriteBatch(SortMode sortMode, bool alphaBlending) :
	sortMode(sortMode),
	alphaBlending(alphaBlending),
	indexBuffer(std::make_unique<IndexBufferUS>(0, startingSize * 6, 0)),
	vertBuffer(std::make_unique<VertBuffer<VertexPosRhyColorUV>>(VertexPosRhyColorUV(), startingSize * 4, 0)),
	primitiveTopology(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
	sampler(std::make_unique<Sampler>(0)),
	vertShader(std::make_unique<VertShader>(L"Assets\\Shader\\SpriteBatchVert.cso")),
	pixShader(std::make_unique<PixShader>(L"Assets\\Shader\\SpriteBatchPixel.cso")),
	constantVert(std::make_unique<ConstantVS<SpriteBatch::WorldPos>>(&constantBuffer, 0)),
	blendState(std::make_unique<BlendStateBindable>()),
	noDepthBuffer(std::make_unique<StencilStates>(StencilStates::GetNoDepthTest()))
{

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = {
		{ "position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "texCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	inputLayout = std::make_unique<InputLayout>(inputLayoutDesc, vertShader->GetBlob());
}

void SpriteBatch::Begin(OrthographicCamera* _camera)
{
	assert(drawing == false);
	drawing = true;

	currentBufferCount = 0;
	camera = _camera;
}

void SpriteBatch::Draw(Texture* texture, float texX, float texY, float texWidth, float texHeight, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color)
{
	assert(drawing == true);
	assert(texture);

	GrowArray();

	sprites[currentBufferCount++] = {
		position.x, position.y, position.z, width, height, rotation,
		texX, texY, texWidth, texHeight, texture, color
	};
}

void SpriteBatch::Draw(TextureRegion* texture, RhyM::Vec3 position, float width, float height, float rotation, RhyC::color color)
{
	assert(drawing == true);
	assert(texture->texture);

	GrowArray();

	sprites[currentBufferCount++] = {
		position.x, position.y, position.z, width, height, rotation,
		texture->x, texture->y, texture->width, texture->height, texture->texture, color
	};
}

void SpriteBatch::Draw(TextureRegion* texture, float x, float y, float z, float width, float height, float rotation, RhyC::color color)
{
	assert(drawing == true);
	assert(texture->texture);

	GrowArray();

	sprites[currentBufferCount++] = {
		x, y, z, width, height, rotation,
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

	// Disable warning about arithmetic overflow
#pragma warning(push)
#pragma warning(disable : 26451)

	std::vector<VertexPosRhyColorUV> verts(currentBufferCount * 4);
	std::vector<unsigned short> indexes(currentBufferCount * 6);

	// Go over each sprite in the buffer and put them to the verts and indexes list.
	for (int i = 0; i < currentBufferCount; i++)
	{
		auto* sprite = sortedSprites[i];
		int atVert = i * 4;

		// TODO: Rotation not yet implemented.

		verts[atVert].color = sprite->color;
		verts[atVert + 1].color = sprite->color;
		verts[atVert + 2].color = sprite->color;
		verts[atVert + 3].color = sprite->color;

		verts[atVert].pos.x = sprite->worldPosition.x;
		verts[atVert].pos.y = sprite->worldPosition.y;
		verts[atVert].pos.z = sprite->worldPosition.z;

		verts[atVert + 1].pos.x = sprite->worldPosition.x + sprite->worldSize.width;
		verts[atVert + 1].pos.y = sprite->worldPosition.y;
		verts[atVert + 1].pos.z = sprite->worldPosition.z;

		verts[atVert + 2].pos.x = sprite->worldPosition.x;
		verts[atVert + 2].pos.y = sprite->worldPosition.y + sprite->worldSize.height;
		verts[atVert + 2].pos.z = sprite->worldPosition.z;

		verts[atVert + 3].pos.x = sprite->worldPosition.x + sprite->worldSize.width;
		verts[atVert + 3].pos.y = sprite->worldPosition.y + sprite->worldSize.height;
		verts[atVert + 3].pos.z = sprite->worldPosition.z;

		int texWidth = sprite->texture->GetWidth();
		int texHeight = sprite->texture->GetHeight();
		float uvWidth = sprite->textureSize.width / texWidth;
		float uvHeight = sprite->textureSize.height / texHeight;
		float u = sprite->texturePosition.x / texWidth;
		float v = sprite->texturePosition.y / texHeight;

		verts[atVert].u = u;
		verts[atVert].v = v + uvHeight;

		verts[atVert + 1].u = u + uvWidth;
		verts[atVert + 1].v = v + uvHeight;

		verts[atVert + 2].u = u;
		verts[atVert + 2].v = v;

		verts[atVert + 3].u = u + uvWidth;
		verts[atVert + 3].v = v;

		int atIndex = i * 6;
		indexes[atIndex] = atVert;
		indexes[atIndex + 1] = atVert + 2;
		indexes[atIndex + 2] = atVert + 1;
		indexes[atIndex + 3] = atVert + 2;
		indexes[atIndex + 4] = atVert + 3;
		indexes[atIndex + 5] = atVert + 1;
	}
#pragma warning(pop) // end disable warning

	// Both lists are now populated, lets bind all needed bindables.
	if (alphaBlending)
		blendState->Bind();
	if (noDepthBlending)
		noDepthBuffer->Bind();
	else
		Gfx::GetInstance()->ClearDepth();

	// Update the vertex, index and constant buffer
	vertBuffer->UpdateVerts(verts);
	indexBuffer->UpdateIndexes(indexes);
	constantBuffer.transform = camera->GetOrthoMatrix();
	constantVert->SetAndUpdate(&constantBuffer);

	indexBuffer->Bind();
	vertBuffer->Bind();
	primitiveTopology->Bind();
	sampler->Bind();
	vertShader->Bind();
	pixShader->Bind();
	inputLayout->Bind();
	constantVert->Bind();

	// iterate over the sprite list again and make a draw call every time the next sprite uses a
	// different texture.
	int lastSameTexture = 0;
	int currentDraw = 1;
	while (currentDraw < currentBufferCount)
	{
		if (sortedSprites[lastSameTexture]->texture != sortedSprites[currentDraw]->texture)
		{
			DrawBatch(lastSameTexture, currentDraw);
			lastSameTexture = currentDraw;
		}
		++currentDraw;
	}
	// Draw the last batch.
	DrawBatch(lastSameTexture, currentBufferCount);

	// Unbind if needed.
	if (alphaBlending)
		blendState->UnBind();
	if (noDepthBlending)
		noDepthBuffer->UnBind();
}

inline void SpriteBatch::GrowArray()
{
	// Do we need to grow the array?
	if (currentBufferCount + 1 < maxBufferSize)
		return;

	maxBufferSize *= 2;
	
	// reset the old buffers
	indexBuffer.reset();
	vertBuffer.reset();

	// create the new buffers
	indexBuffer = std::make_unique<IndexBufferUS>(0, maxBufferSize * 6, 0);
	vertBuffer = std::make_unique<VertBuffer<VertexPosRhyColorUV>>(VertexPosRhyColorUV(), maxBufferSize * 4, 0);

	// resize the vectors
	sprites.resize(maxBufferSize);
	sortedSprites.resize(maxBufferSize);
}

inline void SpriteBatch::Sort()
{
	CopySpritesToSorted();

	switch (sortMode)
	{
	case SpriteBatch::SortMode::ZBased:
		std::sort(sortedSprites.begin(), sortedSprites.begin() + currentBufferCount,
			[](Sprite* first, Sprite* second) -> bool
			{
				return first->worldPosition.z > second->worldPosition.z;
			}
		);

		break;

	case SpriteBatch::SortMode::TextureBased:
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
	sortedSprites[from]->texture->Bind();
	Gfx::GetInstance()->DrawIndexed((to - from) * 6, from * 6);
}
