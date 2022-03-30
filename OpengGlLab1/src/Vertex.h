#pragma once

#include "VertexBufferLayout.h"

struct Vertex {
	float coordinates[2];
	float color[4];
	float pointSize;

	static VertexBufferLayout GetLayout() {
		auto layout = new VertexBufferLayout();
		layout->Push<float>(2);
		layout->Push<float>(4);
		layout->Push<float>(1);

		return *layout;
	}
};
