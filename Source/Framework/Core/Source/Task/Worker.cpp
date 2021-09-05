#include "Worker.h"
#include <ECS/ParallelUpdatable.h>
#include <Task/TaskManager.h>

Worker::Worker(TaskManager* taskManager) : taskManager(taskManager)
{
	thread = std::async(std::launch::async, [this] {
		Work();
		});
}

Worker::~Worker()
{
	if (!exit) Finalize();
	Wait();
}

void Worker::Finalize()
{
	auto l = std::unique_lock<std::mutex>(m);
	exit = true;
	cv.notify_one();
}
void Worker::Pause()
{
	auto l = std::unique_lock<std::mutex>(m);
	execute = false;
}
void Worker::Play()
{
	auto l = std::unique_lock<std::mutex>(m);
	execute = true;
	cv.notify_one();
}
void Worker::Wait()
{
	assert(exit);
	if (thread.valid())
		thread.get();
}

bool Worker::IsExecuting()
{
	return execute;
}

void Worker::Work()
{
	while (true) {
		bool done = false;
		{
			auto l = std::unique_lock<std::mutex>(m);
			cv.wait(l, [&] {
				return exit || execute;
				});
			done = exit; // have lock here
		}

		if (done)
			break;

		ParallelUpdatable* parallel = taskManager->GetParallel();
		if (parallel)
			parallel->ParallelUpdate();
		else
			execute = false;
	}
}
