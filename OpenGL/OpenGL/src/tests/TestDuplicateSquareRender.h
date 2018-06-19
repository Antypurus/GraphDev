#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"

namespace Test
{
	class TestDuplicateSquareRender:public Test
	{
	private:
		//general requirements
		VertexArray va;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader* shader;
		Texture texture;
		Renderer renderer;

		//specific requirements
		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 translationA = glm::vec3(200, 200, 0);
		glm::vec3 translationB = glm::vec3(400, 200, 0);
	public:
		TestDuplicateSquareRender();
		void OnImGuiRender() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
	};
}
