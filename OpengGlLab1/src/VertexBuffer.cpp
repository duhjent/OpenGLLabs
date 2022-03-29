#include "VertexBuffer.h"
#include "Renderer.h"

#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_RendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	int bindId;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bindId);
	std::cout << "glget constructor " << bindId << std::endl;
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind() const {
	int bindId;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bindId);
	std::cout << "glget " << bindId << std::endl;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
