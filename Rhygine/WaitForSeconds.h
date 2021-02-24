#pragma once
#include "YieldInstruction.h"

class WaitForSeconds : public YieldInstruction
{
public:
	WaitForSeconds(float seconds);
	bool IsDone();
private:
	float timer = 0;
};
