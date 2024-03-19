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
	 * ���������� � ������ ������� ����� ������. ��� ����
	 * ������������� �������, � ����� ��������� ��������
	 * �������������� �������� ����������
	 */
	void push(std::function<void()> funct);
	/*
	 * ����� pop ��������� � ��������, ���� �� �������
	 * ����������� �� �������� ����������. ��� �����������
	 * �������� ���������� ������ ����������� �� �������
	 */
	std::function<void()> pop();
protected:
	std::queue<std::function<void()>> _queue;	// ����� ��� �������� �����
	mutable std::mutex _mt;							// ���������� ����������
	std::condition_variable_any _notification;		// �����������
};

class thread_pool
{
public:
	thread_pool();
	~thread_pool();
	/** �������� �� ��������� ������ �  ��������� ��. ����
	 *  ����� ���������� ������������ ������� ��� ����������
	**/
	void work();

	/** �������� � ������� ������
	 *  ����� ���������� ������������ ������� ��� ����������
	**/
	void submit(std::function<void()> funct);
	//	void submit(std::packaged_task<void(void)>&);

protected:
	std::vector<std::thread> _pool;					// ������ �������
	safe_queue* _squeue = nullptr;					// ������� �����
};