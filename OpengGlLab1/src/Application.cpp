#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define COLOR_STEP 0.15f

struct Vertex {
	float coordinates[2];
	float color[4];
	float pointSize;
};

std::vector<Vertex> vertices;
float r_step = COLOR_STEP;
float r = 1;
float g_step = COLOR_STEP;
float g = 1;
float b_step = COLOR_STEP;
float b = 1;

float point_size = 3;
float point_size_step = 0.2;

VertexBuffer GetVb(std::vector<Vertex> vertices) {
	if (vertices.size() != 0) {
		return VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	}
	return VertexBuffer(nullptr, 0);
}

IndexBuffer GetIb(std::vector<Vertex> vertices) {
	if (vertices.size() == 0) {
		return IndexBuffer(nullptr, 0);
	}
	std::vector<unsigned int> indexes;
	for (int i = 0; i < vertices.size(); i++)
	{
		indexes.push_back(i);
	}

	return IndexBuffer(&indexes[0], vertices.size());
}

int sign(double val) {
	return (val > 0) - (val < 0);
}

void CalculateColorChanel(float& color, float& delta) {
	if (color > 1.0)
		delta = -COLOR_STEP;
	if (color < 0.0)
		delta = COLOR_STEP;
	color += delta;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		float x = -1.0 + 2.0 * (double)xpos / WINDOW_WIDTH;
		float y = 1.0 - 2.0 * (double)ypos / WINDOW_HEIGHT;
		Vertex v = { {x, y}, {r, g, b, 1}, point_size };
		vertices.push_back(v);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && vertices.size() >= 1)
		vertices.resize(vertices.size() - 1);
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		CalculateColorChanel(r, r_step);
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		CalculateColorChanel(g, g_step);
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		CalculateColorChanel(b, b_step);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	point_size = std::max(0.0f, point_size + point_size_step * sign(yoffset));
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	GLenum glewRes = glewInit();
	if (glewRes != GLEW_OK) {
		return -1;
	}

	{
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		Shader shader("res/shaders/Basic.shader");

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(1, true);

		Renderer renderer(GL_POINTS);
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			IndexBuffer ib = GetIb(vertices);
			VertexArray va;
			VertexBuffer vb = GetVb(vertices);
			va.AddBuffer(vb, layout);

			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}