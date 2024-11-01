#include "Windows/Windows.h"
#include "Core/EntryPoint.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	using namespace Rhygine;

	int exitCode = EntryPoint::WinRun(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return exitCode;
}
