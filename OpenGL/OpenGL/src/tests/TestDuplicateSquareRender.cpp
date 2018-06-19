#include "imgui/imgui.h"
#include "TestDuplicateSquareRender.h"

Test::TestDuplicateSquareRender::TestDuplicateSquareRender()
{

}

void Test::TestDuplicateSquareRender::OnImGuiRender()
{
}

void Test::TestDuplicateSquareRender::OnUpdate(float deltaTime)
{
	ImGui::Separator();

	//Display some text (you can use a format string too)
	//ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
	//ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test::TestDuplicateSquareRender::OnRender()
{

}