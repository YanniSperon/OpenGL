#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_NAME "Windows"
	#define ASSERT(x) if (!(x)) __debugbreak();
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#elif defined(__APPLE__) && defined(__MACH__)
	#define PLATFORM_NAME "Apple"
	#define ASSERT(x) if (!(x)) throw "OpenGL Error, please contact software developer";
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall())
#elif defined(__linux__)
	#define PLATFORM_NAME "Linux"
	#define ASSERT(x) if (!(x)) throw "OpenGL Error, please contact software developer";
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall())
#else
	#define PLATFORM_NAME "Unknown"
	#define ASSERT(x) if (!(x)) throw "OpenGL Error, please contact software developer";
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall())
#endif
