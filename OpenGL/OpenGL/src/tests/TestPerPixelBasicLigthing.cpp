#include "TestPerPixelBasicLigthing.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

Test::TestPerPixelBasicLigthing::TestPerPixelBasicLigthing()
{
	glEnable(GL_DEPTH_TEST);

	float positions[] = {
		-50.0f, -50.0f,  50.0f, 0.0f, 0.0f,   //0
		 50.0f, -50.0f,  50.0f, 1.0f, 0.0f,   //1
		 50.0f,  50.0f,  50.0f, 1.0f, 1.0f,   //2
		-50.0f,  50.0f,  50.0f, 0.0f, 1.0f,	  //3

		-50.0f, -50.0f, -50.0f, 0.0f, 1.0f,   //4
		 50.0f, -50.0f, -50.0f, 1.0f, 1.0f,   //5
		 50.0f,  50.0f, -50.0f, 1.0f, 0.0f,   //6
		-50.0f,  50.0f, -50.0f, 0.0f, 0.0f	  //7
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1,
	};

	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GlCall(glEnable(GL_BLEND));

	va = new VertexArray;
	vb = new VertexBuffer(positions, 8 * 5 * sizeof(float));

	layout = new VertexBufferLayout;
	layout->Push<float>(3);
	layout->Push<float>(2);
	va->addBuffer(*vb, *layout);

	ib = new IndexBuffer(indices, 36);

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 10000.0f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(480.0f, 270.0f, 500.0f),		// the position of your camera, in world space
		glm::vec3(480.0f, 270.0f, 0.0f),		// where you want to look at, in world space
		glm::vec3(0.0f, 1.0f, 0.0f)				// probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);

	this->proj = proj;
	this->view = view;

	textureShader = new Shader("res/shaders/PPBL_Texture.shader");
	colorShader = new Shader("res/shaders/PPBL_Color.shader");

	texture = new Texture("res/textures/adromeda.png");
	texture->Bind();

	textureShader->Bind();
	textureShader->SetUniform1i("u_Texture", 0);

	vb->Unbind();
	ib->Unbind();
	textureShader->Unbind();
	colorShader->Unbind();
	va->Unbind();
	texture->Unbind();

	this->translation = glm::vec3(200, 200, 0);

	this->rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
}

Test::TestPerPixelBasicLigthing::~TestPerPixelBasicLigthing()
{
	delete va;
	delete vb;
	delete layout;
	delete ib;
	delete textureShader;
	delete colorShader;
	delete texture;
	delete renderer;
}

void Test::TestPerPixelBasicLigthing::OnUpdate(float deltaTime)
{

}

void Test::TestPerPixelBasicLigthing::OnRender()
{
	renderer->Clear();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		model = glm::rotate(model, glm::radians(this->rotationAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 mvp = proj * view * model;

		if (this->mvp != mvp || textureCache != useTexture)
		{
			this->mvp = mvp;
			textureCache = useTexture;

			if (useTexture)
			{
				textureShader->SetUniformMat4f("u_MVP", mvp);
			}
			else
			{
				colorShader->SetUniformMat4f("u_MVP", mvp);
			}
		}
	}

	/* Draw A Triangle By issuing draw call to buffer */
	if(useTexture)
	{
		renderer->Draw(*va, *ib, *textureShader);
	}
	else
	{
		renderer->Draw(*va, *ib, *colorShader);
	}
}

void Test::TestPerPixelBasicLigthing::OnImGuiRender()
{
	textureCache = useTexture;

	//Translation Controlls
	ImGui::SliderFloat("Translation X axys", &translation.x, 0.0f, 960.0f);
	ImGui::SliderFloat("Translation Y axys", &translation.y, 0.0f, 540.0f);
	ImGui::SliderFloat("Translation Z axys", &translation.z, 0.0f, 100.0f);

	ImGui::Separator();

	//Rotation Controlls
	ImGui::SliderFloat("X Angle", &rotationAngle.x, 0.0f, 360.0f);
	ImGui::SliderFloat("Y Angle", &rotationAngle.y, 0.0f, 360.0f);
	ImGui::SliderFloat("Z Angle", &rotationAngle.z, 0.0f, 360.0f);

	ImGui::Separator();

	//texture usage controll
	ImGui::Checkbox("Use Texture", &useTexture);

	if(useTexture != textureCache)
	{
		if(useTexture)
		{
			textureShader->Bind();
			texture->Bind();
		}
		else
		{
			colorShader->Bind();
			texture->Unbind();
		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
