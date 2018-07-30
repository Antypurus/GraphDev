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

namespace Test
{
	struct BaseLight
	{
		glm::vec3 color;
		float intensity;

		void sendToShader(const std::string& name, Shader& shader)
		{
			shader.Bind();
			shader.SetUniform3f(name + ".color", color.x, color.y, color.z);
			shader.SetUniform1f(name + ".intensity", intensity);
		}
	};

	struct DirectionalLight
	{
		BaseLight base;
		glm::vec3 direction;

		void sendToShader(const std::string& name, Shader& shader)
		{
			shader.Bind();
			shader.SetUniform3f(name + ".base.color", base.color.x, base.color.y, base.color.z);
			shader.SetUniform1f(name + ".base.intensity", base.intensity);
			shader.SetUniform3f(name + ".direction", direction.x, direction.y, direction.z);
		}
	};
}

namespace  Test
{
	class MultipassForwardRenderingTest : public Test
	{
	private:
		VertexArray * va;
		VertexBuffer* vb;
		VertexBufferLayout* layout;
		IndexBuffer* ib;

		Shader* ambientShader;
		Shader* directionalLightShader;

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

		//Lighthing Controlls
		float AmbientLightIntensity[3] = { 1.0f,1.0f,1.0f };

		DirectionalLight directionalLight = { { glm::vec3(1.0f,1.0f,1.0f),1.0f },glm::vec3(-1,1,-1) };

		float specularIntensity = 10.0f;
		float specularDampening = 1.0f;

		bool useAmbientShader = true;
		bool useDirectionalShader = true;
	public:
		MultipassForwardRenderingTest();
		~MultipassForwardRenderingTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
