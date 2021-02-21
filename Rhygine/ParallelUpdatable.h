#pragma once

/// <summary>
/// ParallelUpdatables are updated at the same time after each component got
/// updated and before late updatables where updated.
/// </summary>
class ParallelUpdatable
{
public:
	virtual void Update() = 0;
};
