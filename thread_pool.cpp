#include "thread_pool.h"

/*----------------------->> safe_queue <<-----------------------*/
void safe_queue::push(std::function<void()> funct)
{
	{
		std::unique_lock<std::mutex> lock(_mt);
		_queue.push(funct);
	}
	_notification.notify_one();
}

std::function<void(void)> safe_queue::pop()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(_mt);
		_notification.wait(lock, [&] { return !(_queue.empty()); });
		std::function<void()> res = _queue.front();
		lock.unlock();
		return res;
	}
}

/*----------------------->> thread_pool <<-----------------------*/
thread_pool::thread_pool()
{
	_squeue = new safe_queue;
}

thread_pool::~thread_pool()
{
	delete _squeue;
}

void work()
{
}

void thread_pool::submit(std::function<void(void)> funct)
{
	_squeue->push(funct);
}