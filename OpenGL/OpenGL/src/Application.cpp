#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream file(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0,FRAGMENT =1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while(getline(file,line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if(line.find("vertex")!=std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if(line.find("fragment") != std::string::npos)
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

static unsigned int CompileShader(unsigned int type, const std::string& source,const std::string& name)
{
	GlCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	/*Error Handling*/
	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if(result==GL_FALSE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)malloc(length*sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		
		printf("Failed to compile %s shader. Error:%s", name.c_str(), message);
		GlCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader,const std::string& fragmentShader)
{
	GlCall(unsigned int program = glCreateProgram());
	GlCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader,"Vertex"));
	GlCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader,"Fragment"));
	
	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));
	
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

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
	glfwSwapInterval(1);//syncronizes with refresh rate

	/* Check If GLEW was properly initialized */
	if (glewInit() != GLEW_OK)
	{
		printf("Error Initializing GLEW\n");
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << "\n";
	{
		float positions[] = {
			-0.5f, -0.5f,   //0
			 0.5f, -0.5f,   //1
			 0.5f,  0.5f,   //2
			-0.5f,  0.5f	//3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.addBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

		GlCall(glUseProgram(shader));
		GlCall(int location = glGetUniformLocation(shader, "u_Color"));
		//ASSERT(location == -1); //for some reason is always -1 on my computer
		GlCall(glUniform4f(location, 0.2f, 0.8f, 0.8f, 1.0f));

		GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GlCall(glUseProgram(0));


		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			GlCall(glUseProgram(shader));
			GlCall(glUniform4f(location, r, 0.8f, 0.8f, 1.0f));

			va.Bind();
			ib.Bind();

			/* Draw A Triangle By issuing draw call to buffer */
			GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}
			r += increment;

			/* Swap front and back buffers */
			GlCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GlCall(glfwPollEvents());
		}

		GlCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}