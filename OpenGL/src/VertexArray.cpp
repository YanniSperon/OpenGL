#include "VertexArray.h"
#include "GLErrorManager.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));

	//unsigned int vao;
	//GLCall(glGenVertexArrays(1, &vao));
	//GLCall(glBindVertexArray(vao)); ----- when we bind a vertex array and a buffer,
	// nothing links them together, but when we use vertex attrib pointer, we are
	// setting the 0 position in the current vertex array. Same with index buffer
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	// once buffer is bound, we can tell opengl what the layout of our buffer is.
	//GLCall(glEnableVertexAttribArray(0));
	// index we want to   \/   ^ enable
	//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	// this is the starting index, amount of indexes in each pair, valuetype, normalize?
	// , amount of bytes between each pair of values (stride), and finally offset between each
	// attribute (only needed if you store multiple values like texture data too

	// dynamic is changed values (updated every frame) and drawn often

	// we are storing the cache (buffer) with the variable name buffer to access it.
	// it returns an integer which is used like a key in a dictionary

	// first we create them with glGenBuffers(size, &key) then we bind it to the
	// currently used buffer with glBindBuffers(type(enum), key)
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const unsigned int count)
{
	VertexBufferLayout layout;
	layout.Push<float>(count);

	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	// once buffer is bound, we can tell opengl what the layout of our buffer is.
	//GLCall(glEnableVertexAttribArray(0));
	// index we want to   \/   ^ enable
	//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	// this is the starting index, amount of indexes in each pair, valuetype, normalize?
	// , amount of bytes between each pair of values (stride), and finally offset between each
	// attribute (only needed if you store multiple values like texture data too

	// dynamic is changed values (updated every frame) and drawn often

	// we are storing the cache (buffer) with the variable name buffer to access it.
	// it returns an integer which is used like a key in a dictionary

	// first we create them with glGenBuffers(size, &key) then we bind it to the
	// currently used buffer with glBindBuffers(type(enum), key)
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
