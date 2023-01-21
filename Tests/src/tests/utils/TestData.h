// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-26) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace testing::utils
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
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.5f * scale };
	}

	static glm::mat2 GetMatrix2(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.5f * scale };
	}

	static glm::mat3 GetMatrix3(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.5f * scale, 1.5f * scale, 6.4f * scale, 32.5f * scale, 5.3f * scale, 6.4f * scale };
	}

	static glm::mat4 GetMatrix4(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.5f * scale, 1.5f * scale, 6.4f * scale, 32.5f * scale, 5.3f * scale, 6.4f * scale, 4.5f * scale, 63.0f * scale, 74.4f * scale, 42.4f * scale, 42.5f * scale, 6.3f * scale, 74.4f * scale };
	}

	static glm::quat GetQuaternion(float scale = 1.0f)
	{
		return { 2.5f * scale, 6.8f * scale, 10.5f * scale, 12.5f * scale };
	}

	static std::vector<glm::vec2> GetVector2Array()
	{
		std::vector<glm::vec2> result;
		result.push_back(GetVector2());
		result.push_back(GetVector2(2.5f));
		result.push_back(GetVector2(10.0f));
		result.push_back(GetVector2(163.4f));
		return result;
	}

	static std::vector<glm::vec3> GetVector3Array()
	{
		std::vector<glm::vec3> result;
		result.push_back(GetVector3());
		result.push_back(GetVector3(2.5f));
		result.push_back(GetVector3(10.0f));
		result.push_back(GetVector3(163.4f));
		return result;
	}

	static std::vector<glm::vec4> GetVector4Array()
	{
		std::vector<glm::vec4> result;
		result.push_back(GetVector4());
		result.push_back(GetVector4(2.5f));
		result.push_back(GetVector4(10.0f));
		result.push_back(GetVector4(163.4f));
		return result;
	}

	static std::vector<glm::mat2> GetMatrix2Array()
	{
		std::vector<glm::mat2> result;
		result.push_back(GetMatrix2());
		result.push_back(GetMatrix2(2.5f));
		result.push_back(GetMatrix2(10.0f));
		result.push_back(GetMatrix2(163.4f));
		return result;
	}

	static std::vector<glm::mat3> GetMatrix3Array()
	{
		std::vector<glm::mat3> result;
		result.push_back(GetMatrix3());
		result.push_back(GetMatrix3(2.5f));
		result.push_back(GetMatrix3(10.0f));
		result.push_back(GetMatrix3(163.4f));
		return result;
	}

	static std::vector<glm::mat4> GetMatrix4Array()
	{
		std::vector<glm::mat4> result;
		result.push_back(GetMatrix4());
		result.push_back(GetMatrix4(2.5f));
		result.push_back(GetMatrix4(10.0f));
		result.push_back(GetMatrix4(163.4f));
		return result;
	}

	static std::vector<glm::quat> GetQuaternionArray()
	{
		std::vector<glm::quat> result;
		result.push_back(GetQuaternion());
		result.push_back(GetQuaternion(2.5f));
		result.push_back(GetQuaternion(10.0f));
		result.push_back(GetQuaternion(163.4f));
		return result;
	}
}

namespace testing::utils::json
{
	
}

namespace testing::utils::xml
{

}

namespace testing::utils::yaml
{

}

