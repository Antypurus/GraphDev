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

	va = new VertexArray;
	vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));

	layout = new VertexBufferLayout;
	layout->Push<float>(2);
	layout->Push<float>(2);
	va->addBuffer(*vb, *layout);

	ib = new IndexBuffer(indices, 6);

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	this->proj = proj;
	this->view = view;

	shader = new Shader("res/shaders/Basic.shader");

	texture = new Texture("res/textures/adromeda.png");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	vb->Unbind();
	ib->Unbind();
	shader->Unbind();
	va->Unbind();

	this->translationA = glm::vec3(200, 200, 0);
	this->translationB = glm::vec3(400, 200, 0);

	Renderer renderer;
}

Test::TestDuplicateSquareRender::~TestDuplicateSquareRender()
{
	delete va;
	delete vb;
	delete layout;
	delete ib;
	delete shader;
	delete texture;
	delete renderer;
}

void Test::TestDuplicateSquareRender::OnImGuiRender()
{
	//Display some text (you can use a format string too)
	ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
	ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test::TestDuplicateSquareRender::OnUpdate(float deltaTime)
{
}

void Test::TestDuplicateSquareRender::OnRender()
{
	renderer->Clear();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
		glm::mat4 mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
	}

	/* Draw A Triangle By issuing draw call to buffer */
	renderer->Draw(*va, *ib, *shader);


	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
		glm::mat4 mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
	}

	renderer->Draw(*va, *ib, *shader);
}