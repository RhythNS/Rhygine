#pragma once
#include "UISizer.h"
#include <vector>

class UIVertPercentageSizer : public UISizer
{
public:
	UIVertPercentageSizer();
	UIVertPercentageSizer(std::vector<float> percentages);
	virtual ~UIVertPercentageSizer() { }

	const bool isControllingChildren();

	void ResizeChildren(RhyM::Vec2 currentWorldScale);
	void UpdatePositionChildren();

	std::vector<float> percentages;

private:
	static const bool controllingChildren = true;

};
