#pragma once
#include <ECS/GameObject.h>

#include <vector>
#include <mutex>

class Stage;
class Worker;
class ParallelUpdatable;
class TaskManager
{
public:
	TaskManager(int numberOfThreads);
	~TaskManager();

	void AddOneShot(ParallelUpdatable* oneShot);
	void Work(Stage* stage);
	ParallelUpdatable* GetParallel();

private:
	std::vector<ParallelUpdatable*> oneShots;
	std::vector<Worker*> workers;
	GameObject* currentGameObject = nullptr;
	int currentParallel = 0, atOneShot = 0;
	std::mutex l;
};
