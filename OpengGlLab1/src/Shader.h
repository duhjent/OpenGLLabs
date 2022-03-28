#pragma once
#include <string>
#include <unordered_map>

struct ShaderSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader {
private:
	unsigned int m_RendererId;
	std::string m_FileLocation;
	std::unordered_map<std::string, int> m_LocationCache;
	ShaderSource m_ShaderSource;
public:
	Shader(const std::string& fileName);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	int GetUniformLocation(const std::string& name);
	ShaderSource ParseShader();
	unsigned int CreateShader();
	unsigned int CompileShahader(unsigned int type, std::string& code);
};
