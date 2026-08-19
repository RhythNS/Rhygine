#pragma once

#include <atomic>
#include <cstdint>
#include <utility>

#include "RenderPacket.h"

namespace Rhygine
{
	class TripleBufferedPackets
	{
	public:
		void Produce(RenderPacket&& t_packet)
		{
			uint32_t writeSlot = m_writeIndex.load(std::memory_order_relaxed);
			m_slots[writeSlot] = std::move(t_packet);
			m_latestReady.store(writeSlot, std::memory_order_release);
			m_writeIndex.store((writeSlot + 1) % SLOT_COUNT, std::memory_order_relaxed);
		}

		void Produce(const RenderPacket& t_packet)
		{
			RenderPacket copy = t_packet;
			Produce(std::move(copy));
		}

		const RenderPacket& ConsumeLatest()
		{
			uint32_t latest = m_latestReady.exchange(UINT32_MAX, std::memory_order_acquire);
			if (latest != UINT32_MAX)
			{
				m_readIndex = latest;
			}
			return m_slots[m_readIndex];
		}

		[[nodiscard]] bool HasNewPacket() const
		{
			return m_latestReady.load(std::memory_order_relaxed) != UINT32_MAX;
		}

	private:
		static constexpr uint32_t SLOT_COUNT = 3;
		RenderPacket m_slots[SLOT_COUNT];
		
		std::atomic<uint32_t> m_writeIndex{0};
		std::atomic<uint32_t> m_latestReady{UINT32_MAX};

		uint32_t m_readIndex = 0; // Only touched by render thread
	};
}
