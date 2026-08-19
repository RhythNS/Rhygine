#include "SingleConcurrentQueue.h"

template<typename T>
inline void Rhygine::SingleConcurrentQueue<T>::Push(const T& t_item)
{
	m_queue.enqueue(t_item);
	m_count.fetch_add(1, std::memory_order_release);
}

template<typename T>
void Rhygine::SingleConcurrentQueue<T>::Peek(T& t_item) const
{
	m_queue.peek(t_item);
}

template<typename T>
bool Rhygine::SingleConcurrentQueue<T>::TryPop(T& t_item)
{
	if (m_queue.try_dequeue(t_item))
	{
		m_count.fetch_sub(1, std::memory_order_release);
		return true;
	}
	return false;
}

template<typename T>
void Rhygine::SingleConcurrentQueue<T>::Pop(T& t_item)
{
	m_queue.dequeue(t_item);
	m_count.fetch_sub(1, std::memory_order_release);
}

template<typename T>
size_t Rhygine::SingleConcurrentQueue<T>::GetCount() const
{
	return m_count.load(std::memory_order_acquire);
}
