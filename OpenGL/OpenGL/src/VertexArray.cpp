#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
	GlCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto & elements = layout.getElements();
	unsigned int offset = 0;
	for(size_t i = 0;i<elements.size();++i)
	{
		const auto& element = elements[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count*VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}
