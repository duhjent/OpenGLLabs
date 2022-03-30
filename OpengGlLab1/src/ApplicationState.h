#pragma once
#include <vector>

#include "Vertex.h"

struct ColorState {
	float red;
	float green;
	float blue;
};

enum ColorChannel {
	RED,
	GREEN,
	BLUE
};

class ApplicationState {
private:
	ApplicationState();
	ApplicationState(ApplicationState const&);
	std::vector<Vertex> m_Vertices;
	float m_PointSize;
	ColorState m_ColorState;
	ColorState m_ColorDelta;
public:
	static ApplicationState& GetInstance()
	{
		static ApplicationState instance;
		return instance;
	}
	std::vector<Vertex> GetVertices() const { return m_Vertices; };
	void AddVertexOnCoordinates(float xpos, float ypos);
	void ColorChanged(ColorChannel channel);
	void SizeChanged(float direction);
	void ClearVertices();
	void UndoLastVertice();
};
