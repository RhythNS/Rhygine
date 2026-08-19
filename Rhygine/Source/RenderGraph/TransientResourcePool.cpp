#include "TransientResourcePool.h"
#include "IDevice.h"
#include "RHIDescriptors.h"

#include <algorithm>

namespace Rhygine
{
	namespace
	{
		inline void HashCombine(uint64_t& seed, uint64_t value)
		{
			seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
		}

		uint64_t HashTextureDesc(const TextureDesc& desc)
		{
			uint64_t seed = 0;
			HashCombine(seed, static_cast<uint64_t>(desc.width));
			HashCombine(seed, static_cast<uint64_t>(desc.height));
			HashCombine(seed, static_cast<uint64_t>(desc.depth));
			HashCombine(seed, static_cast<uint64_t>(desc.mipLevels));
			HashCombine(seed, static_cast<uint64_t>(desc.arrayLayers));
			HashCombine(seed, static_cast<uint64_t>(desc.format));
			HashCombine(seed, static_cast<uint64_t>(desc.type));
			HashCombine(seed, static_cast<uint64_t>(desc.usage));
			HashCombine(seed, static_cast<uint64_t>(desc.sampleCount));
			HashCombine(seed, static_cast<uint64_t>(desc.aspect));
			return seed;
		}

		uint64_t HashBufferDesc(const BufferDesc& desc)
		{
			uint64_t seed = 0;
			HashCombine(seed, desc.size);
			HashCombine(seed, static_cast<uint64_t>(desc.usage));
			HashCombine(seed, static_cast<uint64_t>(desc.cpuAccess));
			return seed;
		}
	}

	TransientResourcePool::TransientResourcePool(IDevice* t_device)
		: m_device(t_device)
	{
	}

	TransientResourcePool::~TransientResourcePool()
	{
		if (m_device)
		{
			for (Texture* tex : m_allTextures)
			{
				m_device->DestroyTexture(tex);
			}
			for (Buffer* buf : m_allBuffers)
			{
				m_device->DestroyBuffer(buf);
			}
		}

		m_allTextures.clear();
		m_allBuffers.clear();
		m_freeTextures.clear();
		m_freeBuffers.clear();
		m_acquiredTextures.clear();
		m_acquiredBuffers.clear();
		m_textureHashes.clear();
		m_bufferHashes.clear();
	}

	Texture* TransientResourcePool::AcquireTexture(const TextureDesc& t_desc)
	{
		const uint64_t hash = HashTextureDesc(t_desc);
		auto it = m_freeTextures.find(hash);
		if (it != m_freeTextures.end() && !it->second.empty())
		{
			Texture* tex = it->second.back();
			it->second.pop_back();
			m_acquiredTextures.push_back(tex);
			return tex;
		}

		if (!m_device)
		{
			return nullptr;
		}

		Texture* tex = m_device->CreateTexture(t_desc);
		if (tex)
		{
			m_allTextures.push_back(tex);
			m_acquiredTextures.push_back(tex);
			m_textureHashes[tex] = hash;
		}
		return tex;
	}

	Buffer* TransientResourcePool::AcquireBuffer(const BufferDesc& t_desc)
	{
		const uint64_t hash = HashBufferDesc(t_desc);
		auto it = m_freeBuffers.find(hash);
		if (it != m_freeBuffers.end() && !it->second.empty())
		{
			Buffer* buf = it->second.back();
			it->second.pop_back();
			m_acquiredBuffers.push_back(buf);
			return buf;
		}

		if (!m_device)
		{
			return nullptr;
		}

		Buffer* buf = m_device->CreateBuffer(t_desc);
		if (buf)
		{
			m_allBuffers.push_back(buf);
			m_acquiredBuffers.push_back(buf);
			m_bufferHashes[buf] = hash;
		}
		return buf;
	}

	void TransientResourcePool::ReleaseAll()
	{
		for (Texture* tex : m_acquiredTextures)
		{
			auto it = m_textureHashes.find(tex);
			if (it != m_textureHashes.end())
			{
				m_freeTextures[it->second].push_back(tex);
			}
		}
		m_acquiredTextures.clear();

		for (Buffer* buf : m_acquiredBuffers)
		{
			auto it = m_bufferHashes.find(buf);
			if (it != m_bufferHashes.end())
			{
				m_freeBuffers[it->second].push_back(buf);
			}
		}
		m_acquiredBuffers.clear();
	}

	void TransientResourcePool::GarbageCollect()
	{
		if (!m_device)
		{
			m_freeTextures.clear();
			m_freeBuffers.clear();
			return;
		}

		for (auto& [hash, list] : m_freeTextures)
		{
			for (Texture* tex : list)
			{
				m_device->DestroyTexture(tex);
				std::erase(m_allTextures, tex);
				m_textureHashes.erase(tex);
			}
		}
		m_freeTextures.clear();

		for (auto& [hash, list] : m_freeBuffers)
		{
			for (Buffer* buf : list)
			{
				m_device->DestroyBuffer(buf);
				std::erase(m_allBuffers, buf);
				m_bufferHashes.erase(buf);
			}
		}
		m_freeBuffers.clear();
	}
}
