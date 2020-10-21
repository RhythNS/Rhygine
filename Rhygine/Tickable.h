#pragma once

/// <summary>
/// Interface for classes to implement the Tick function. Tick is called
/// at the start of new frame before Update is being called.
/// </summary>
class Tickable {
public: 
	virtual void Tick() = 0;
};