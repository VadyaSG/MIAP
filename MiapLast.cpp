#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

template<typename T>
class Safe_queue
{
private:
	std::queue<T> _queue;
	mutable std::mutex _mutex;
	std::condition_variable _cond_var;
	bool is_stop = false;
public:
	void push(T val)
	{
		std::lock_guard<std::mutex>lock(_mutex);
		_queue.push(std::move(val));
		_cond_var.notify_one();
	}

	bool pop(T& val)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cond_var.wait(lock, [this] {return !_queue.empty() || is_stop; });
		if (is_stop && _queue.empty())
		{
			return false;
		}

		val = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	void stop_queue()
	{
		std::lock_guard<std::mutex>lock(_mutex);
		is_stop = true;
		_cond_var.notify_all();
	}
};

class Thread_pool
{
private:
	std::vector<std::thread> _threads;
	Safe_queue < std::function<void()>> _task_queue;

	void work()
	{
		while (true)
		{
			std::function<void()> task;
			if (_task_queue.pop(task))
			{
				task();
			}
			else
			{
				break;
			}
		}
	}

public:
	Thread_pool()
	{
		unsigned int num_threads = std::thread::hardware_concurrency();
		for (unsigned int i = 0; i < num_threads; ++i)
		{
			_threads.emplace_back(&Thread_pool::work, this);
		}
	}

	~Thread_pool()
	{
		_task_queue.stop_queue();
		for (auto& thread : _threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	void submit(std::function<void()>task)
	{
		_task_queue.push(std::move(task));
	}
};

void func1()
{
	std::cout << "task 1 in thread: " << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void func2()
{
	std::cout << "task 2 in thread: " << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main()
{

	Thread_pool pool;
	for (int i = 0; i < 5; ++i) 
	{
		pool.submit(func1);
		pool.submit(func2);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "complited" << std::endl;

	return 0;
}