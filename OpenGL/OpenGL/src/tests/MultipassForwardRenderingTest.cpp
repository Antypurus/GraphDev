#include "MultipassForwardRenderingTest.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "imgui/imgui.h"

Test::MultipassForwardRenderingTest::MultipassForwardRenderingTest()
{
	glEnable(GL_DEPTH_TEST);

	float positions[] = {
		// { Pos.x, Pos.y, Pos.z} {Text.u, Text.v} {Norm.x, Norm.y, Norm.z}
		//front face
		-50.0f, -50.0f,  50.0f, 0.0f, 0.0f,  0.0f,0.0f,1.0f,  //0
		50.0f, -50.0f,  50.0f, 1.0f, 0.0f,  0.0f,0.0f,1.0f,  //1
		50.0f,  50.0f,  50.0f, 1.0f, 1.0f,  0.0f,0.0f,1.0f,  //2
		-50.0f,  50.0f,  50.0f, 0.0f, 1.0f,	 0.0f,0.0f,1.0f,  //3

															  //back face
															  -50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 0.0f,0.0f,-1.0f,  //4
															  50.0f, -50.0f, -50.0f, 1.0f, 1.0f, 0.0f,0.0f,-1.0f,  //5
															  50.0f,  50.0f, -50.0f, 1.0f, 0.0f, 0.0f,0.0f,-1.0f,  //6
															  -50.0f,  50.0f, -50.0f, 0.0f, 0.0f, 0.0f,0.0f,-1.0f,  //7

																													//top face
																													-50.0f,  50.0f, -50.0f, 0.0f, 1.0f,  0.0f,1.0f,0.0f, //8
																													50.0f,  50.0f, -50.0f, 1.0f, 1.0f,  0.0f,1.0f,0.0f, //9
																													50.0f,  50.0f,  50.0f, 1.0f, 0.0f,  0.0f,1.0f,0.0f, //10
																													-50.0f,  50.0f,  50.0f, 0.0f, 0.0f,  0.0f,1.0f,0.0f, //11

																																										 //bottom face
																																										 -50.0f, -50.0f, -50.0f, 0.0f, 1.0f,  0.0f,-1.0f,0.0f,  //12
																																										 50.0f, -50.0f, -50.0f, 1.0f, 1.0f,  0.0f,-1.0f,0.0f,  //13
																																										 50.0f, -50.0f,  50.0f, 1.0f, 0.0f,  0.0f,-1.0f,0.0f,  //14
																																										 -50.0f, -50.0f,  50.0f, 0.0f, 0.0f,  0.0f,-1.0f,0.0f,  //15

																																																								//left face
																																																								-50.0f, -50.0f, -50.0f, 0.0f, 1.0f,  -1.0f,0.0f,0.0f,  //16
																																																								-50.0f, -50.0f,  50.0f, 1.0f, 1.0f,  -1.0f,0.0f,0.0f,  //17
																																																								-50.0f,  50.0f,  50.0f, 1.0f, 0.0f,  -1.0f,0.0f,0.0f,  //18
																																																								-50.0f,  50.0f, -50.0f, 0.0f, 0.0f,  -1.0f,0.0f,0.0f,  //19

																																																																					   //right face
																																																																					   50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 1.0f,0.0f,0.0f,   //20
																																																																					   50.0f, -50.0f,  50.0f, 1.0f, 1.0f, 1.0f,0.0f,0.0f,   //21
																																																																					   50.0f,  50.0f,  50.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f,   //22
																																																																					   50.0f,  50.0f, -50.0f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,   //23
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		7, 6, 5,
		5, 4, 7,
		//top
		10,9,11,
		11,9,8,
		//bottom
		14,15,13,
		15,12,13,
		//left
		18,19,16,
		18,16,17,
		//right
		22,20,21,
		22,23,20
	};

	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GlCall(glEnable(GL_BLEND));

	va = new VertexArray;
	vb = new VertexBuffer(positions, 24 * 8 * sizeof(float));

	layout = new VertexBufferLayout;
	layout->Push<float>(3);//vertex positions
	layout->Push<float>(2);//texture coordinates
	layout->Push<float>(3);//normal coordinates
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

	ambientShader = new Shader("res/shaders/Forward Shaders/Forward-Ambient.shader");
	directionalLightShader = new Shader("res/shaders/Forward Shaders/Forward-Directional.shader");

	texture = new Texture("res/textures/adromeda.png");
	texture->Bind();

	ambientShader->Bind();
	ambientShader->SetUniform1i("u_Texture", 0);

	directionalLightShader->Bind();
	directionalLightShader->SetUniform1i("u_Texture", 0);

	vb->Unbind();
	ib->Unbind();
	ambientShader->Unbind();
	va->Unbind();
	//texture->Unbind();

	this->translation = glm::vec3(200, 200, 0);
	this->rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
}

Test::MultipassForwardRenderingTest::~MultipassForwardRenderingTest()
{
	delete va;
	delete vb;
	delete layout;
	delete ib;
	delete ambientShader;
	delete texture;
	delete renderer;
	delete directionalLightShader;
}

void Test::MultipassForwardRenderingTest::OnUpdate(float deltaTime)
{
}

void Test::MultipassForwardRenderingTest::OnRender()
{
	renderer->Clear();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		model = glm::rotate(model, glm::radians(this->rotationAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->rotationAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));

		glm::vec3 camVec = glm::vec3(0.0f, 0.0f, -500.0f);

		glm::vec4 lookingPos = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0.0f, 1.0f, 0.0f))*glm::vec4(camVec, 0.0f);

		view = glm::lookAt(
			glm::vec3(480.0f, 270.0f, 500.0f),													// the position of your camera, in world space
			glm::vec3(480.0f + lookingPos.x, 270.0f + lookingPos.y, 500.0f + lookingPos.z),		// where you want to look at, in world space
			glm::vec3(0.0f, 1.0f, 0.0f)															// probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be grea too
		);

		glm::mat4 mvp = proj * view * model;
		ambientShader->SetUniformMat4f("u_MVP", mvp);
		ambientShader->SetUniform3f("AmbientIntensity", AmbientLightIntensity[0], AmbientLightIntensity[1], AmbientLightIntensity[2]);
		
		directionalLightShader->SetUniformMat4f("u_MVP", mvp);
		directionalLightShader->SetUniformMat4f("u_Model", model);
		directionalLightShader->SetUniform3f("eyePos", 480.0f, 270.0f, 500.0f);
		directionalLight.sendToShader("u_DirectionalLight", *directionalLightShader);
		directionalLightShader->SetUniform1f("specularIntensity", specularIntensity);
		directionalLightShader->SetUniform1f("specularExponent", specularDampening);
	}

	renderer->Draw(*va, *ib, *ambientShader);

	GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_ONE, GL_ONE));
	GlCall(glDepthMask(GL_FALSE));
	GlCall(glDepthFunc(GL_EQUAL));

	renderer->Draw(*va, *ib, *directionalLightShader);

	GlCall(glDepthFunc(GL_LESS));
	GlCall(glDepthMask(GL_TRUE));
	GlCall(glDisable(GL_BLEND));
}

void Test::MultipassForwardRenderingTest::OnImGuiRender()
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

	ImGui::Separator();

	ImGui::SliderFloat("Scale", &scale, 0.1f, 100.0f);

	ImGui::Separator();

	//camera controll
	ImGui::SliderFloat("Horizontal Rotation", &rotation, -360.0f, 360.0f);

	ImGui::Separator();

	//Light Controll - Ambient

	ImGui::ColorPicker3("Ambient Light Color", &AmbientLightIntensity[0]);

	ImGui::Separator();

	//Light Controll - Directional
	ImGui::SliderFloat("Diffuse Light Intensity", &directionalLight.base.intensity, 0.0f, 100.0f);
	ImGui::SliderFloat3("Specular Light Direction", &directionalLight.direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Specular Light Intensity", &specularIntensity,0.0f,100.0f);
	ImGui::SliderFloat("Specular Light Dampening", &specularDampening, 0.0f, 100.0f);


	ImGui::Separator();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
