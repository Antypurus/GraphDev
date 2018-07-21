#pragma once
#include "Test.h"
#include "glm/glm.hpp"

class VertexArray;
class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;
class Shader;
class Texture;
class Renderer;

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
	public:
		DirectionalLightTest();
		~DirectionalLightTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
