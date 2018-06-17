#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>

Shader::Shader(const std::string& filepath):m_Filepath(filepath)
{
	ShaderProgramSource source = ParseShader();
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
	GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if(m_UniformLocationCache.find(name)!=m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	GlCall(const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
	
	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader()
{
	std::ifstream file(m_Filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			if (type != ShaderType::NONE)
			{
				ss[(unsigned int)type] << line << '\n';
			}
		}
	}

	return { ss[0].str(),ss[1].str() };
}

unsigned Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GlCall(unsigned int program = glCreateProgram());
	GlCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader, "Vertex"));
	GlCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader, "Fragment"));

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

unsigned Shader::CompileShader(unsigned type, const std::string& source, const std::string& name)
{
	GlCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	/*Error Handling*/
	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)malloc(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));

		printf("Failed to compile %s shader. Error:%s", name.c_str(), message);
		GlCall(glDeleteShader(id));
		return 0;
	}

	return id;
}
