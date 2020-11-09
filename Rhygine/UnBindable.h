#pragma once

/// <summary>
/// Unbinds a bindable. Bindables may want to inherit from this, if they
/// need to be unbound.
/// </summary>
class UnBindable
{
public:
	virtual void UnBind() = 0;
};
