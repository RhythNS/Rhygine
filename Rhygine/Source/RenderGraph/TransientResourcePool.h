#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

namespace Rhygine
{
	class IDevice;
	class Texture;
	class Buffer;
	struct TextureDesc;
	struct BufferDesc;

	class TransientResourcePool
	{
	public:
		TransientResourcePool(IDevice* t_device);
		~TransientResourcePool();

		Texture* AcquireTexture(const TextureDesc& t_desc);
		Buffer* AcquireBuffer(const BufferDesc& t_desc);

		void ReleaseAll();
		void GarbageCollect();

		[[nodiscard]] size_t GetTotalAllocatedTextureCount() const { return m_allTextures.size(); }
		[[nodiscard]] size_t GetTotalAllocatedBufferCount() const { return m_allBuffers.size(); }
		[[nodiscard]] size_t GetAcquiredTextureCount() const { return m_acquiredTextures.size(); }
		[[nodiscard]] size_t GetAcquiredBufferCount() const { return m_acquiredBuffers.size(); }

	private:
		IDevice* m_device = nullptr;

		std::unordered_map<uint64_t, std::vector<Texture*>> m_freeTextures;
		std::unordered_map<uint64_t, std::vector<Buffer*>>  m_freeBuffers;

		std::vector<Texture*> m_allTextures;
		std::vector<Buffer*>  m_allBuffers;

		std::vector<Texture*> m_acquiredTextures;
		std::vector<Buffer*>  m_acquiredBuffers;

		std::unordered_map<Texture*, uint64_t> m_textureHashes;
		std::unordered_map<Buffer*, uint64_t>  m_bufferHashes;
	};
}
