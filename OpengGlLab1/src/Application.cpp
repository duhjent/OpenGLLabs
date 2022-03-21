#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearErr();\
	x;\
	ASSERT(GLPrintErr(#x, __FILE__, __LINE__));

static void GLClearErr() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLPrintErr(const char* functionCall, const char* file, int line) {
	if (GLenum error = glGetError())
	{
		std::cout << "OpenGL error" << error << " " << functionCall << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

struct ShaderSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

static ShaderSource ParseShader(std::string filePath)
{
	enum class ShaderType
	{
		None = -1, Vertex = 0, Fragment = 1
	};

	std::ifstream stream(filePath);

	std::string line;
	ShaderType type = ShaderType::None;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::Fragment;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[(int)ShaderType::Vertex].str(), ss[(int)ShaderType::Fragment].str() };
}

static unsigned int CompileShahader(unsigned int type, std::string& code)
{
	unsigned int id = glCreateShader(type);
	const char* codeCStr = code.c_str();
	glShaderSource(id, 1, &codeCStr, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char* message = (char*)alloca(len * sizeof(char));

		glGetShaderInfoLog(id, len, &len, message);
		std::cerr << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vertexId = CompileShahader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentId = CompileShahader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum glewRes = glewInit();
	if (glewRes != GLEW_OK) {
		return -1;
	}

	float positions[] = {
		0.0f, 0.0f,
		0.5f, 0.0f,
		0.0f, 0.5f,
		0.5f, 0.5f
	};

	unsigned int indexes[] = {
		0, 1, 2,
		1, 2, 3
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indexes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0);

	ShaderSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shaderProgram = CreateShader(source.VertexShader, source.FragmentShader);
	glUseProgram(shaderProgram);

	int location = glGetUniformLocation(shaderProgram, "u_Color");
	ASSERT(location != -1);

	float r = 0.0f;
	float step = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		if (r > 1.0)
			step = -0.05f;
		if (r < 0.0)
			step = 0.05f;
		r += step;

		glUniform4f(location, r, 0.5, 1.0, 1.0);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}