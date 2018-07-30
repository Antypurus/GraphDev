#pragma once
#include "Test.h"
#include "glm/glm.hpp"

class VertexArray;
class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;
class Texture;
class Renderer;
class Shader;

namespace  Test
{
	class MultipassForwardRenderingTest : public Test
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

		//Lighthing Controlls
		float AmbientLightIntensity[3] = { 1.0f,1.0f,1.0f };
	public:
		MultipassForwardRenderingTest();
		~MultipassForwardRenderingTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
