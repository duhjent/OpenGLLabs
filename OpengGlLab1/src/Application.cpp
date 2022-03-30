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
#include "Vertex.h"
#include "ApplicationState.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		float x = -1.0 + 2.0 * (double)xpos / WINDOW_WIDTH;
		float y = 1.0 - 2.0 * (double)ypos / WINDOW_HEIGHT;
		ApplicationState::GetInstance().AddVertexOnCoordinates(x, y);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		ApplicationState::GetInstance().UndoLastVertice();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		ApplicationState::GetInstance().ClearVertices();
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		ApplicationState::GetInstance().ColorChanged(ColorChannel::RED);
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		ApplicationState::GetInstance().ColorChanged(ColorChannel::GREEN);
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		ApplicationState::GetInstance().ColorChanged(ColorChannel::BLUE);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ApplicationState::GetInstance().SizeChanged(yoffset);
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

	unsigned int glewRes = glewInit();
	if (glewRes != GLEW_OK) {
		return -1;
	}

	{
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		Shader shader("res/shaders/Basic.shader");

		VertexBufferLayout layout = Vertex::GetLayout();

		Renderer renderer(GL_POINTS);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			VertexArray va;
			IndexBuffer ib = GetIb(ApplicationState::GetInstance().GetVertices());
			VertexBuffer vb = GetVb(ApplicationState::GetInstance().GetVertices());
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