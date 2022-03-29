#include "Renderer.h"
#include <iostream>

void GLClearErr() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLPrintErr(const char* functionCall, const char* file, int line) {
	bool isOk = true;
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL error" << error << " " << functionCall << " " << file << ":" << line << std::endl;
		isOk = false;
	}

	return isOk;
}

void Renderer::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_POINTS, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
