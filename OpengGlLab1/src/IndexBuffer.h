#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void UpdateData(const void* data, unsigned int count);

	inline unsigned int GetCount() const { return m_Count; }
};
