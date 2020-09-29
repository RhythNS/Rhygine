// Defining this before include <Window.h>, fixes an error with assimp
// https://github.com/assimp/assimp/issues/2271
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>