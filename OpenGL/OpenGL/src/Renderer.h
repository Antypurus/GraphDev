#pragma once

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

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);