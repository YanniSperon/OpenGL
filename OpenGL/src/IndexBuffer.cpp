#include "IndexBuffer.h"
#include "GLErrorManager.h"
#include "GL/glew.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	// & is used for the memory address
	GLCall(glGenBuffers(1, &m_RendererID));
	// create buffers^ then set \/ as active
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

	// next we are creating the bufferdata to give opengl the data we are working
	// with and the size of it. Since we created a buffer, but did not actually put
	// anything in it
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	// set buffer as active
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	// set buffer as 0 to deactivate
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
