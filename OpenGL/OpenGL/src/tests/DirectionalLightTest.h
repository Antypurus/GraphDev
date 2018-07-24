#pragma once
#include "Test.h"
#include "glm/glm.hpp"
#include <string>
#include "../Shader.h"

class VertexArray;
class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;
class Texture;
class Renderer;

struct BaseLight
{
	glm::vec3 color;
	float intensity;

	void sendToShader(const std::string& name,Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f(name + ".color", color.x, color.y, color.z);
		shader.SetUniform1f(name + ".intensity", intensity);
	}
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;

	void sendToShader(const std::string& name, Shader& shader)
	{
		shader.Bind();
		shader.SetUniform1f(name + ".constant", constant);
		shader.SetUniform1f(name + ".linear", linear);
		shader.SetUniform1f(name + ".exponent", exponent);
	}
};

struct PointLight
{
	BaseLight base;
	glm::vec3 position;
	Attenuation attenuation;
	float range;
	int activated = 1;

	void sendToShader(const std::string& name,Shader& shader)
	{
		shader.Bind();
		shader.SetUniform3f(name + ".base.color", base.color.x, base.color.y, base.color.z);
		shader.SetUniform1f(name + ".base.intensity", base.intensity);
		shader.SetUniform3f(name + ".position", position.x, position.y, position.z);
		attenuation.sendToShader(name + ".atten", shader);
		shader.SetUniform1f(name + ".range", range);
		shader.SetUniform1i(name + ".activated", activated);
	}
};

struct SpotLight
{
	PointLight pointLight;
	glm::vec3 direction;
	float cutoff;

	void sendToShader(const std::string& name, Shader& shader)
	{
		direction = normalize(direction);
		shader.Bind();
		pointLight.sendToShader(name + ".pointLight", shader);
		shader.SetUniform3f(name + ".direction", direction.x, direction.y, direction.z);
		shader.SetUniform1f(name + ".cutoff", cutoff);
	}
};

namespace Test
{
	class DirectionalLightTest:public Test
	{
	private:
		VertexArray * va;
		VertexBuffer* vb;
		VertexBufferLayout* layout;
		IndexBuffer* ib;
		Shader* shader;
		Texture* texture;
		Renderer* renderer;

		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 translation;

		glm::vec3 rotationAngle;
		float scale = 1.0f;

		//caching mechanisms
		bool textureCache = false;
		glm::mat4 mvp;

		float rotation = 0;

		//lighting
		float ambientColor[3] = { 1.0f,1.0f,1.0f };
		float intensity = 10.0f;
		float specularIntensity = 0.0f;
		float specularExponent = 0.0f;

		Attenuation atten = { 1.0f,0.0f,0.0f };
		float lightRange = 10000.0f;

		float spotLightDirection[3] = { 0.0f,0.0f,-1.0f };
		float cuttoff = 0.0f;
	public:
		DirectionalLightTest();
		~DirectionalLightTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
