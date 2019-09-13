#pragma once
#include<queue>
#include<mutex>
#include<memory>
#include<condition_variable>
using namespace std;
template<class T>
class ThreadSafeQueue
{
private:
	queue<T*> m_queue;
	mutex m_mutex;
	condition_variable m_condition;
public:
	ThreadSafeQueue() {}
	~ThreadSafeQueue() {}
	void Enqueue(T* request)
	{
		lock_guard<mutex> lk(m_mutex);
		m_queue.push(move(request));
		m_condition.notify_one();
	}
	T* Dequeue()
	{
		lock_guard<mutex> lk(m_mutex);
		if (m_queue.empty())
		{
			return nullptr;
		}
		T* request = move(m_queue.front());
		m_queue.pop();
		return request;
	}
	size_t size()
	{
		lock_guard<mutex> lk(m_mutex);
		return m_queue.size();
	}
};