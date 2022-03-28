#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& fileName) : m_FileLocation(fileName)
{
	m_ShaderSource = ParseShader();
	m_RendererId = CreateShader();
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererId, "u_Color"));
	if (location == -1) {
		std::cerr << "Shader has incorrect uniform: " << name << std::endl;
	}
	m_LocationCache[name] = location;
	return location;
}

ShaderSource Shader::ParseShader()
{
	enum class ShaderType
	{
		None = -1, Vertex = 0, Fragment = 1
	};

	std::ifstream stream(m_FileLocation);

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

unsigned int Shader::CreateShader()
{
	unsigned int program = glCreateProgram();
	unsigned int vertexId = CompileShahader(GL_VERTEX_SHADER, m_ShaderSource.VertexShader);
	unsigned int fragmentId = CompileShahader(GL_FRAGMENT_SHADER, m_ShaderSource.FragmentShader);

	glAttachShader(program, vertexId);
	glAttachShader(program, fragmentId);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return program;
}

unsigned int Shader::CompileShahader(unsigned int type, std::string& code)
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
