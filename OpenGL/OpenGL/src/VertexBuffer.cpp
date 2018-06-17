#include "VertexBuffer.h"
#include <GL/glew.h> 
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned size)
{
	GlCall(glGenBuffers(1, &m_RendererID));//generate 1 buffer
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//bind the buffer for usage
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //sets the data that is being used
}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//bind the buffer for usage
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//unbind the buffer
}
