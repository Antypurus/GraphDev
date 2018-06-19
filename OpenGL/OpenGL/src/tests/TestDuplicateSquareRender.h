#pragma once

#include "Test.h"

namespace Test
{
	class TestDuplicateSquareRender:public Test
	{
	private:
	public:
		TestDuplicateSquareRender();
		void OnImGuiRender() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
	};
}
