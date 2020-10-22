#pragma once

/// <summary>
/// Interface for classes to implement the Update function. Update is
/// called before objects are drawn. Should be used to advance an object
/// in the simulation.
/// </summary>
class Updatable
{
public:
	virtual void Update() {}
};
