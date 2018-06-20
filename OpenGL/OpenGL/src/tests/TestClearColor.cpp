#include "TestClearColor.h"
#include <GL/glew.h>
#include "../Renderer.h"
#include "imgui/imgui.h"

Test::TestClearColor::TestClearColor() :m_ClearColor{ 0.2f,0.3f,0.8f,1.0f }
{

}

Test::TestClearColor::~TestClearColor()
{
	GlCall(glClearColor(0, 0, 0, 1));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Test::TestClearColor::OnUpdate(float deltaTime)
{

}

void Test::TestClearColor::OnRender()
{
	GlCall(glClearColor(m_ClearColor[1], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Test::TestClearColor::OnImGuiRender()
{
	ImGui::ColorPicker4("Clear Color", m_ClearColor);
}
