#pragma once

#include <GL/glew.h>
#include <iostream>

static void GLClearError();
static bool GLLogCall();

#define DEBUG 1

#define ASSERT(x) if(!(x)) __debugbreak();

#if DEBUG
#define GlCall(x) \
	GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__));
#else
#define GlCall(x) \
	x;
#endif

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]" << error << ":" << function << " File:" << file << ":" << line << '\n';
		return false;
	}
	return true;
}

class Renderer
{
public:
	
};
