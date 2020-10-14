#include "TestLitPlate.h"
#include "TestLitPlateComponent.h"

void TestLitPlate::AddData(GameObject* toAddTo)
{
	toAddTo->AddComponent<TestLitPlateComponent>();
}
