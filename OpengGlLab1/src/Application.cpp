#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

std::vector<float> vertices;

VertexBuffer GetVb(std::vector<float> vertices) {
	float* positions = &vertices[0];

	VertexBuffer vb(positions, vertices.size() * sizeof(float));
	return vb;
}

IndexBuffer GetIb(std::vector<float> vertices) {
	unsigned int size = vertices.size();
	std::vector<unsigned int> indexes;
	for (int i = 0; i < size; i++)
	{
		indexes.push_back(i);
	}

	return IndexBuffer(&indexes[0], size);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Mouse pressed at " << xpos << " " << ypos << std::endl;
		vertices.push_back(xpos / WINDOW_WIDTH);
		vertices.push_back(ypos / WINDOW_HEIGHT);
	}
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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSwapInterval(1);

	GLenum glewRes = glewInit();
	if (glewRes != GLEW_OK) {
		return -1;
	}

	{
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.5f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);
		vertices.push_back(0.5f);

		Shader shader("res/shaders/Basic.shader");

		IndexBuffer ib = GetIb(vertices);

		VertexArray va;
		VertexBuffer vb = GetVb(vertices);
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		float r = 0.0f;
		float step = 0.05f;
		Renderer renderer;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			if (r > 1.0)
				step = -0.05f;
			if (r < 0.0)
				step = 0.05f;
			r += step;

			//IndexBuffer ib = GetIb(vertices);

			//VertexArray va;
			//VertexBuffer vb = GetVb(vertices);
			//VertexBufferLayout layout;
			//layout.Push<float>(2);
			//va.AddBuffer(vb, layout);

			renderer.Draw(va, ib, shader);
			shader.SetUniform4f("u_Color", r, 0.5, 1.0, 1.0);
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}