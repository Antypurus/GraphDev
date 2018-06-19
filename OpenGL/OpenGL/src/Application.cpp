#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Graphics Development Window", NULL, NULL);
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
			100.0f, 100.0f, 0.0f, 0.0f,   //0
			200.0f, 100.0f, 1.0f, 0.0f,   //1
			200.0f, 200.0f, 1.0f, 1.0f,   //2
			100.0f, 200.0f, 0.0f, 1.0f	//3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GlCall(glEnable(GL_BLEND));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.addBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/adromeda.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		va.Unbind();


		float r = 0.0f;
		float increment = 0.05f;

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.8f, 0.8f, 1.0f);

			/* Draw A Triangle By issuing draw call to buffer */
			renderer.Draw(va, ib, shader);

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
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
