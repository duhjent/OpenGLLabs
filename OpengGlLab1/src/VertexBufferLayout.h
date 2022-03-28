#pragma once
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;

	unsigned int GetSizeOfType() const {
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: 4;
		case GL_UNSIGNED_BYTE: 1;
		}
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, false });
		m_Stride += sizeof(float) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_Stride += sizeof(unsigned int) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
		m_Stride += sizeof(unsigned char) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
	inline const unsigned int GetStride() const& { return m_Stride; };
};
