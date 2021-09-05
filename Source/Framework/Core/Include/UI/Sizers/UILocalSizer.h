#pragma once
#include <UI/UIOwnSizer.h>

/// <summary>
/// Default sizer which sets the bounds of its UIElement relative to its parent
/// using the size and position of the UIElement.
/// </summary>
class UILocalSizer : public UIOwnSizer
{
public:
	virtual ~UILocalSizer() { }
};
