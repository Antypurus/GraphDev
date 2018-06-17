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

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader&shader)const;
	void Clear()const;
};