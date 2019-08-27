#include "VertexBuffer.h"
#include "GLErrorManager.h"
#include "GL/glew.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// & is used for the memory address
	GLCall(glGenBuffers(1, &m_RendererID));
	// create buffers^ then set \/ as active
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

	// next we are creating the bufferdata to give opengl the data we are working
	// with and the size of it. Since we created a buffer, but did not actually put
	// anything in it
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	// set buffer as active
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	// set buffer as 0 to deactivate
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
