#include <GL/glew.h>
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../VertexBufferLayout.h"
#include "TestDuplicateSquareRender.h"

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

	vb = VertexBuffer(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.addBuffer(vb, layout);

	ib = IndexBuffer(indices, 6);

	proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	shader = Shader("res/shaders/Basic.shader");

	texture = Texture("res/textures/adromeda.png");
	texture.Bind();
	shader->SetUniform1i("u_Texture", 0);

	vb.Unbind();
	ib.Unbind();
	shader->Unbind();
	va.Unbind();
}

void Test::TestDuplicateSquareRender::OnImGuiRender()
{
}

void Test::TestDuplicateSquareRender::OnUpdate(float deltaTime)
{
	ImGui::Separator();

	//Display some text (you can use a format string too)
	ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
	ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test::TestDuplicateSquareRender::OnRender()
{
	renderer.Clear();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
		glm::mat4 mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
	}
	renderer.Draw(va, ib, *shader);

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
		glm::mat4 mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
	}
	renderer.Draw(va, ib, *shader);
}