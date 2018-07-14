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
	class TestPerPixelBasicLigthing : public Test
	{
	private:
		VertexArray * va;
		VertexBuffer* vb;
		VertexBufferLayout* layout;
		IndexBuffer* ib;
		Shader* textureShader;
		Shader* colorShader;
		Texture* texture;
		Renderer* renderer;

		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 translation;

		glm::vec3 rotationAngle;

		bool useTexture = false;
		
		//caching mechanisms
		bool textureCache = false;
		glm::mat4 mvp;

		//light
		glm::vec3 lightPosition;
		glm::vec3 lightColor;

		float rotation = 0;
	public:
		TestPerPixelBasicLigthing();
		~TestPerPixelBasicLigthing();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
