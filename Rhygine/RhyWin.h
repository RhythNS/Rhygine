// Defining this before include <Window.h>, fixes an error with assimp
// https://github.com/assimp/assimp/issues/2271
#define NOMINMAX

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>