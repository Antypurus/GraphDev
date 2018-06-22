#pragma once
#include "Test.h"

namespace Test
{
	class TestPerPixelBasicLigthing : public Test
	{
	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
