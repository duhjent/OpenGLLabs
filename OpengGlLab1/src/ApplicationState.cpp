#include <algorithm>

#include "ApplicationState.h"
#include "Vertex.h"

static int sign(double val) {
	return (val > 0) - (val < 0);
}

ApplicationState::ApplicationState() {
	m_ColorState = { 1, 1, 1 };
	m_ColorDelta = { 0.15f, 0.15f, 0.15 };
	m_PointSize = 1;
}

void ApplicationState::AddVertexOnCoordinates(float xpos, float ypos) {
	Vertex v = { {xpos, ypos}, {m_ColorState.red, m_ColorState.green, m_ColorState.blue, 1}, m_PointSize };
	m_Vertices.push_back(v);
}

void ApplicationState::ColorChanged(ColorChannel channel) {
	float* color = &m_ColorState.red;
	float* delta = &m_ColorDelta.red;
	switch (channel) {
		//case ColorChannel::RED:
		//	color = &m_ColorState.red; delta = &m_ColorDelta.red; break;
	case ColorChannel::GREEN:
		color = &m_ColorState.green; delta = &m_ColorDelta.green; break;
	case ColorChannel::BLUE:
		color = &m_ColorState.blue; delta = &m_ColorDelta.blue; break;
	}
	if (*color > 1.0)
		*delta = -0.15;
	if (*color < 0.0)
		*delta = 0.15;
	*color += *delta;
}

void ApplicationState::SizeChanged(float direction) {
	m_PointSize = std::max(0.0f, m_PointSize + 0.2f * sign(direction));
}

void ApplicationState::ClearVertices()
{
	m_Vertices.clear();
}

void ApplicationState::UndoLastVertice()
{
	if (m_Vertices.size() >= 1) {
		m_Vertices.resize(m_Vertices.size() - 1);
	}
}
