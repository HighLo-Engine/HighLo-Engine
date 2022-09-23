#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace utils
{
	static glm::vec2 GetVector2(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale };
	}

	static glm::vec3 GetVector3(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale };
	}

	static glm::vec4 GetVector4(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.4f * scale };
	}

	static glm::mat2 GetMatrix2(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.4f * scale };
	}

	static glm::mat3 GetMatrix3(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.4f * scale, 1.4f * scale, 6.4f * scale, 32.5f * scale, 5.3f * scale, 6.4f * scale };
	}

	static glm::mat4 GetMatrix4(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.4f * scale, 1.4f * scale, 6.4f * scale, 32.5f * scale, 5.3f * scale, 6.4f * scale, 4.2f * scale, 63.0f * scale, 74.4f * scale, 42.4f * scale, 42.5f * scale, 6.3f * scale, 74.4f * scale };
	}

	static glm::quat GetQuaternion(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.4f * scale };
	}
}

namespace utils::json
{

}

namespace utils::xml
{

}

namespace utils::yaml
{

}

