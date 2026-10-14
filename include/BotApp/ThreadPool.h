#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
	using Task = std::function<void()>;

private:
	std::vector<std::thread> threads_;
	std::queue<Task> tasks_;
	std::mutex tasksMutex_;
	std::condition_variable cv_;
	bool running_{ true };

public:
	// Количество потоков по умолчанию равно количеству рабочих потоков ПК
	explicit ThreadPool(size_t numberThreads =
		std::thread::hardware_concurrency());
	~ThreadPool();

	// Удаление копирования
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// Добавление новой задачи
	void addTask(ThreadPool::Task task);
};

#endif // !THREADPOOL_H
