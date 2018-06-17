#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static void GLClearError();
static bool GLLogCall();

#define DEBUG 1

#define ASSERT(x) if(!(x)) __debugbreak();

#if DEBUG
#define GlCall(x) \
	GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__));
#else
#define GlCall(x) \
	x;
#endif

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]" << error <<":"<<function<<" File:"<<file<<":"<<line<<'\n';
		return false;
	}
	return true;
}

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

	/* Check If GLEW was properly initialized */
	if (glewInit() != GLEW_OK)
	{
		printf("Error Initializing GLEW\n");
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << "\n";

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

	unsigned int buffer;//contains the id of the generated buffer
	GlCall(glGenBuffers(1, &buffer));//generate 1 buffer
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));//bind the buffer for usage
	GlCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW)); //sets the data that is being used
	
	GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));
	GlCall(glEnableVertexAttribArray(0));

	unsigned int indexBufferObject;//contains the id of the generated buffer
	GlCall(glGenBuffers(1, &indexBufferObject));//generate 1 buffer
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));//bind the buffer for usage
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); //sets the data that is being used

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GlCall(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GlCall(glClear(GL_COLOR_BUFFER_BIT));

		/* Draw A Triangle By issuing draw call to buffer */
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr));
		
		/* Swap front and back buffers */
		GlCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GlCall(glfwPollEvents());
	}
	
	GlCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}