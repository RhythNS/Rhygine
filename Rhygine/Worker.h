#pragma once
#include <condition_variable>
#include <future>
#include <assert.h>
#include "ParallelUpdatable.h"
#include "TaskManager.h"

// https://stackoverflow.com/questions/40553609/approach-of-using-an-stdatomic-compared-to-stdcondition-variable-wrt-pausing
class Worker
{
public:
	Worker(TaskManager* taskManager);
	// move is not safe.  If you need this movable,
	// use unique_ptr<Worker>.
	Worker(Worker&&) = delete;
	~Worker();
	void Finalize();
	void Pause();
	void Play();
	void Wait();
	bool IsExecuting();
private:
	void Work();

	TaskManager* taskManager;
	std::mutex m;
	std::condition_variable cv;
	bool exit = false;
	bool execute = false;
	std::future<void> thread;
};
