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
class TestSimple3D:public Test
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
public:
	TestSimple3D();
	~TestSimple3D();

	void OnRender() override;
	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
};
}