#pragma once
#include "UISizer.h"

/// <summary>
/// Default sizer which sets the bounds of its UIElement relative to its parent
/// using the size and position of the UIElement.
/// </summary>
class UILocalSizer : public UISizer
{
public:
	virtual ~UILocalSizer() { }

	const bool isControllingChildren();

private:
	static const bool controllingChildren = false;
};

