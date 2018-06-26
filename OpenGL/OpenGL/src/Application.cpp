#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include "Renderer.h"
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "tests/TestDuplicateSquareRender.h"
#include "tests/TestClearColor.h"
#include "tests/TestSimple3D.h"
#include "tests/TestPerPixelBasicLigthing.h"

bool drawGui = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS)
	{
		drawGui = !drawGui;
	}
}

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

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	if(cursor!=NULL)
	{
		glfwSetCursor(window, cursor);
		printf("Cursor created and set\n");
	}
	else
	{
		printf("Failed to create cursor\n");
	}

	std::cout << glGetString(GL_VERSION) << "\n";
	{
		Test::Test* currentTest = new Test::TestPerPixelBasicLigthing;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			currentTest->OnRender();

			ImGui_ImplGlfwGL3_NewFrame();

			{

				if(drawGui)
				{
				if (ImGui::BeginMainMenuBar()) {

					if (ImGui::BeginMenu("Test Cases"))
					{
						if (ImGui::MenuItem("Clear Color Test"))
						{
							delete currentTest;
							currentTest = new Test::TestClearColor;
						}
						if (ImGui::MenuItem("Duplicate Square Test"))
						{
							delete currentTest;
							currentTest = new Test::TestDuplicateSquareRender;
						}
						if (ImGui::MenuItem("Simple 3D Test"))
						{
							delete currentTest;
							currentTest = new Test::TestSimple3D;
						}
						if (ImGui::MenuItem("Basic Per Pixel Lighing Test"))
						{
							delete currentTest;
							currentTest = new Test::TestPerPixelBasicLigthing;
						}
						ImGui::EndMenu();
					}

					ImGui::EndMainMenuBar();
				}

				currentTest->OnImGuiRender();
				}
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
			glfwSetKeyCallback(window, key_callback);
		}

		delete currentTest;
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyCursor(cursor);
	glfwTerminate();

	return 0;
}
