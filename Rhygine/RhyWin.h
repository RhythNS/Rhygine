// Includer for including windows and dxd classes.

// Defining this before include <Window.h>, fixes an error with assimp
// https://github.com/assimp/assimp/issues/2271
#ifndef NOMINMAX
#define NOMINMAX
#endif

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
