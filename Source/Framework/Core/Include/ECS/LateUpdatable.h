#pragma once

/// <summary>
/// Interface for classes to implement the LateUpdate function. LateUpdate
/// is called after Update has been callen on all objects.
/// </summary>
class LateUpdatable
{
public:
	virtual ~LateUpdatable() { }
	virtual void LateUpdate() {}
};
