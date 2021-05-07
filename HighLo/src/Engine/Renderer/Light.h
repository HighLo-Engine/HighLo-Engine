#pragma once
#include "Engine/Math/HLMath.h"

namespace highlo
{
	struct Light
	{
		glm::vec3 Color = glm::vec3(1, 1, 1);
		glm::vec3 Position = glm::vec3(0.0f, 10.0f, -10.0f);
		float Intensity = 1000.0f;
	};
}
