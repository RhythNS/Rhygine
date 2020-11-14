#pragma once
#include "UISizer.h"

class UILocalSizer : public UISizer
{
	const bool isControllingChildren();

private:
	static const bool controllingChildren = false;
};

