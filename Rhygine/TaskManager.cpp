#include "TaskManager.h"
#include "Stage.h"
#include "Worker.h"
#include "ParallelUpdatable.h"

#include "RhyWin.h"

TaskManager::TaskManager(int numberOfThreads) : workers(numberOfThreads)
{
	for (int i = 0; i < numberOfThreads; i++)
		workers[i] = new Worker(this);
}

TaskManager::~TaskManager()
{
	for (auto& worker : workers)
		delete worker;
}

void TaskManager::AddOneShot(ParallelUpdatable* oneShot)
{
	oneShots.push_back(oneShot);
}

void TaskManager::Work(Stage* stage)
{
	currentGameObject = stage->GetFront();
	currentParallel = atOneShot = 0;

	for (auto& worker : workers)
		worker->Play();

	while (true)
	{
		ParallelUpdatable* parallel = GetParallel();
		if (parallel)
			parallel->Update();
		else
			break;
	}

	bool workerStillWorking = true;
	while (workerStillWorking)
	{
		workerStillWorking = false;
		for (auto& worker : workers)
		{
			if (worker->IsExecuting())
			{
				workerStillWorking = true;
				break;
			}
		}
	}

	oneShots.clear();
}

ParallelUpdatable* TaskManager::GetParallel()
{
	std::unique_lock<std::mutex> lock(l);

	if (atOneShot < oneShots.size())
		return oneShots[atOneShot++];

	while (true)
	{
		if (!currentGameObject)
			return nullptr;

		if (currentGameObject->parallels.size() <= currentParallel)
		{
			currentGameObject = currentGameObject->next;
			currentParallel = 0;
			continue;
		}

		return currentGameObject->parallels[currentParallel++];
	}
}

