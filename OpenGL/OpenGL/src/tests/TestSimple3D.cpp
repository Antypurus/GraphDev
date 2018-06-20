#include "TestSimple3D.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace Test
{
TestSimple3D::TestSimple3D()
{
	float positions[] = {
		-50.0f, -50.0f, 00.0f, 0.0f, 0.0f,   //0
		 50.0f, -50.0f, 00.0f, 1.0f, 0.0f,   //1
		 50.0f,  50.0f, 00.0f, 1.0f, 1.0f,   //2
		-50.0f,  50.0f, 00.0f, 0.0f, 1.0f	//3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GlCall(glEnable(GL_BLEND));

	va = new VertexArray;
	vb = new VertexBuffer(positions, 4 * 5 * sizeof(float));

	layout = new VertexBufferLayout;
	layout->Push<float>(3);
	layout->Push<float>(2);
	va->addBuffer(*vb, *layout);

	ib = new IndexBuffer(indices, 6);

	//glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 960.0f / 540.0f, 0.1f, 10000.0f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(200.0f, 200.0f,200.0f),			// the position of your camera, in world space
		glm::vec3(200.0f, 200.0f, 50.0f),		// where you want to look at, in world space
		glm::vec3(0.0f, 1.0f, 0.0f)			// probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);

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

	this->translation = glm::vec3(200, 200, 0);

	this->rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
}

TestSimple3D::~TestSimple3D()
{
	delete va;
	delete vb;
	delete layout;
	delete ib;
	delete shader;
	delete texture;
	delete renderer;
}

void TestSimple3D::OnRender()
{
	renderer->Clear();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		model = glm::rotate(model, glm::radians(this->rotationAngle.x),  glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.y),  glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.z),  glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
	}

	/* Draw A Triangle By issuing draw call to buffer */
	renderer->Draw(*va, *ib, *shader);
}

void TestSimple3D::OnUpdate(float deltaTime)
{
}

void TestSimple3D::OnImGuiRender()
{
	//Translation Controlls
	ImGui::SliderFloat("Translation X axys", &translation.x, 0.0f, 960.0f);
	ImGui::SliderFloat("Translation Y axys", &translation.y, 0.0f, 540.0f);
	ImGui::SliderFloat("Translation Z axys", &translation.z, 0.0f, 100.0f);

	ImGui::Separator();

	//Rotation Controlls
	ImGui::SliderFloat("X Angle", &rotationAngle.x, 0.0f, 360.0f);
	ImGui::SliderFloat("Y Angle", &rotationAngle.y, 0.0f, 360.0f);
	ImGui::SliderFloat("Z Angle", &rotationAngle.z, 0.0f, 360.0f);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
}