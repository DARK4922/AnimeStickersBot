#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numberThreads)
{
	if (numberThreads == 0) { numberThreads = 1; }

	threads_.reserve(numberThreads);
	
	for (size_t index{}; index < numberThreads; ++index)
	{
		threads_.emplace_back([this]()
			{
				while (true)
				{
					Task task;

					{
						std::unique_lock<std::mutex> lock(tasksMutex_);

						cv_.wait(lock, [this]()
							{
								return !tasks_.empty() || !running_;
							});

						// Проверка на завершение работы
						if (!running_ && tasks_.empty()) { return; }

						// Получение задачи
						task = std::move(tasks_.front());
						tasks_.pop();
					}

					// Выполнение задачи
					task();
				}
			});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::lock_guard<std::mutex> lock(tasksMutex_);
		running_ = false;
	}

	cv_.notify_all();
	// Ожидание завершения всех потоков
	for (auto& thread : threads_)
	{
		if (thread.joinable()) { thread.join(); }
	}
}

void ThreadPool::addTask(ThreadPool::Task task)
{
	{
		std::lock_guard<std::mutex> lock(tasksMutex_);

		if (!running_) { return; }

		tasks_.push(std::move(task));
	}

	// Будем один поток
	cv_.notify_one();
}