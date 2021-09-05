#pragma once

/// <summary>
/// Interface for classes to implement the Tick function. Tick is called
/// at the start of new frame before Update is being called.
/// EndTick is called at the end of a frame.
/// </summary>
class Tickable {
public:
	virtual ~Tickable() { }
	virtual void Tick() = 0;
	virtual void EndTick() {}
};
