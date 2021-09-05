#include <Task/TaskManager.h>
#include <ECS/Stage.h>
#include "Worker.h"
#include <ECS/ParallelUpdatable.h>

#include <RhyWin.h>

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

	// Get one parralel, if there are no parallels, we do not need to start all the other threads.
	ParallelUpdatable* parallel = GetParallel();
	if (!parallel)
		return;

	// Start other therads.
	for (auto& worker : workers)
		worker->Play();

	// Also work on getting through each parallel update
	do
	{
		parallel->ParallelUpdate();
		parallel = GetParallel();
	} while (parallel);

	// Wait for all other threads to stop working
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
	// This must be synchronized
	std::unique_lock<std::mutex> lock(l);

	// If there are oneshots available, just return the one shot.
	if (atOneShot < oneShots.size())
		return oneShots[atOneShot++];

	// Continue iterating throught the gameObjects to find parallel updates.
	while (true)
	{
		// Did we reach the end of the linked list?
		if (!currentGameObject)
			return nullptr;

		// Check to see if every parallel of the current object has been done
		if (currentGameObject->parallels.size() <= currentParallel)
		{
			// Reset and continue on the next gameObject
			currentGameObject = currentGameObject->next;
			currentParallel = 0;
			continue;
		}

		// No parallel found on this object. Look at the next one.
		return currentGameObject->parallels[currentParallel++];
	}
}

