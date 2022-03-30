#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearErr();\
	x;\
	ASSERT(GLPrintErr(#x, __FILE__, __LINE__));


void GLClearErr();
bool GLPrintErr(const char* functionCall, const char* file, int line);

class Renderer {
	unsigned int m_Mode;
public:
	Renderer(unsigned int mode);

	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
