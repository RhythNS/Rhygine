// Includer for including stb image classes.

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif 

#ifdef _DEBUG
#define STBI_FAILURE_USERMSG
#endif // DEBUG

#ifndef _DEBUG
#define STBI_NO_FAILURE_STRINGS
#endif // !DEBUG

// Ignore imgui compiler warnings
#pragma warning(push)
#pragma warning(disable : 6387 6308 26451 6011 28182 6262 26819)
#include <stb_image.h>
#pragma warning(pop)
