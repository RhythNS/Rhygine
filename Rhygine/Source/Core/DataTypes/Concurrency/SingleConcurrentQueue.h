#pragma once

#include <atomic>
#include <readerwriterqueue.h>

namespace Rhygine
{
	/// <summary>
	/// Only one thread can push to the queue, and only one thread can pop from the queue.
	/// </summary>
	template<typename T>
	class SingleConcurrentQueue
	{
	public:
		SingleConcurrentQueue() = default;
		~SingleConcurrentQueue() = default;

		void Push(const T& t_item);
		void Peek(T& t_item) const;
		bool TryPop(T& t_item);
		void Pop(T& t_item);

		size_t GetCount() const;

	private:
		moodycamel::ReaderWriterQueue<T> m_queue;
		std::atomic<size_t> m_count{ 0 };
	};
}
