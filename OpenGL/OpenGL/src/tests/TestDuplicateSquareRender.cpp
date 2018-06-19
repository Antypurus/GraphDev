#include "imgui/imgui.h"
#include "TestDuplicateSquareRender.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include "../VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"

class Texture;

Test::TestDuplicateSquareRender::TestDuplicateSquareRender()
{
	float positions[] = {
		-50.0f, -50.0f, 0.0f, 0.0f,   //0
		50.0f, -50.0f, 1.0f, 0.0f,   //1
		50.0f,  50.0f, 1.0f, 1.0f,   //2
		-50.0f,  50.0f, 0.0f, 1.0f	//3
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

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	Shader shader("res/shaders/Basic.shader");

	Texture texture("res/textures/adromeda.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
	va.Unbind();

	Renderer renderer;
}

void Test::TestDuplicateSquareRender::OnImGuiRender()
{
}

void Test::TestDuplicateSquareRender::OnUpdate(float deltaTime)
{
	ImGui::Separator();

	//Display some text (you can use a format string too)
	//ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
	//ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test::TestDuplicateSquareRender::OnRender()
{

}