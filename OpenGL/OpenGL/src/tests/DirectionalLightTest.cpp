#include "DirectionalLightTest.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

Test::DirectionalLightTest::DirectionalLightTest()
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

	shader = new Shader("res/shaders/Directional_Light.shader");
	shader->SetUniform3f("u_BaseColor", ambientColor[0], ambientColor[1], ambientColor[2]);

	texture = new Texture("res/textures/adromeda.png");
	texture->Bind();

	shader->Bind();
	shader->SetUniform1i("u_Texture", 0);

	vb->Unbind();
	ib->Unbind();
	shader->Unbind();
	va->Unbind();
	//texture->Unbind();

	this->translation = glm::vec3(200, 200, 0);
	this->rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
}

Test::DirectionalLightTest::~DirectionalLightTest()
{
	delete va;
	delete vb;
	delete layout;
	delete ib;
	delete shader;
	delete texture;
	delete renderer;
}

void Test::DirectionalLightTest::OnUpdate(float deltaTime)
{
}

void Test::DirectionalLightTest::OnRender()
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
		shader->SetUniformMat4f("u_MVP", mvp);
		shader->SetUniform3f("u_AmbientLight", ambientColor[0], ambientColor[1], ambientColor[2]);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniform3f("eyePos", 480.0f, 270.0f, 500.0f);
		shader->SetUniform1f("specularIntensity", specularIntensity);
		shader->SetUniform1f("specularExponent", specularExponent);

		PointLight light;
		light.base.intensity = intensity;
		light.base.color = glm::vec3(ambientColor[0], ambientColor[1], ambientColor[2]);
		light.position = glm::vec3(480.0f, 270.0f, 500.0f);

		light.attenuation = atten;
		light.range = lightRange;

		SpotLight spot;
		spot.pointLight = light;
		spot.direction = glm::vec3(spotLightDirection[0], spotLightDirection[1], spotLightDirection[2]);
		spot.cutoff = cuttoff;

		spot.sendToShader("spotLights[0]", *shader);
	}

	renderer->Draw(*va, *ib, *shader);
}

void Test::DirectionalLightTest::OnImGuiRender()
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

	//Lighting Controlls
	ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 1000.0f);
	ImGui::SliderFloat("Specular Intensity", &specularIntensity,0.0f,1000.0f);
	ImGui::SliderFloat("Specular Exponent", &specularExponent, 0.0f, 10.0f);

	ImGui::SliderFloat("Ligth Attenuation Constant", &atten.constant, 0.0f, 100.0f);
	ImGui::SliderFloat("Ligth Attenuation Linear", &atten.linear, 0.0f, 100.0f);
	ImGui::SliderFloat("Ligth Attenuation Exponent", &atten.exponent, 0.0f, 100.0f);

	ImGui::SliderFloat("Ligth Range", &lightRange, 0.0f, 10000.0f);

	ImGui::SliderFloat3("SpotLight Direction", &spotLightDirection[0], -1.0f, 1.0f);
	ImGui::SliderFloat("Spot Light cutoff", &cuttoff, -1.0f, 1.0f);

	ImGui::ColorPicker3("Ambient Light Color", ambientColor);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
