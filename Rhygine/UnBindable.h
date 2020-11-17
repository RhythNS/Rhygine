#pragma once

/// <summary>
/// Unbinds a bindable. Bindables may want to inherit from this, if they
/// need to be unbound after they have been drawn.
/// </summary>
class UnBindable
{
public:
	virtual ~UnBindable() { }
	virtual void UnBind() = 0;
};
