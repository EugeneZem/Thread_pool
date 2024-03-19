#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>
#include <iostream>

class safe_queue
{
public:
	safe_queue() {};
	safe_queue(const safe_queue& other) = delete;
	safe_queue& operator=(const safe_queue&) = delete;
	~safe_queue() {};
	/*
	 * «аписывает в начало очереди новую задачу. ѕри этом
	 * захватываетс€ мьютекс, а после окончани€ операции
	 * нотифицируетс€ условна€ переменна€
	 */
	void push(std::function<void()> funct);
	/*
	 * ћетод pop находитс€ в ожидании, пока не прийдет
	 * уведомление на условную переменную. ѕри нотификации
	 * условной переменной данные считываютс€ из очереди
	 */
	std::function<void()> pop();
protected:
	std::queue<std::function<void()>> _queue;	// ћесто дл€ хранени€ задач
	mutable std::mutex _mt;							// –еализаци€ блокировки
	std::condition_variable_any _notification;		// ”ведемлени€
};

class thread_pool
{
public:
	thread_pool();
	~thread_pool();
	/** ¬ыбирает из очередную задачу и  исполн€ет ее. Ётот
	 *  метод передаетс€ конструктору потоков дл€ исполнени€
	**/
	void work();

	/** ѕомещает в очередь задачу
	 *  метод передаетс€ конструктору потоков дл€ исполнени€
	**/
	void submit(std::function<void()> funct);
	//	void submit(std::packaged_task<void(void)>&);

protected:
	std::vector<std::thread> _pool;					// ¬ектор потоков
	safe_queue* _squeue = nullptr;					// ќчередь задач
};