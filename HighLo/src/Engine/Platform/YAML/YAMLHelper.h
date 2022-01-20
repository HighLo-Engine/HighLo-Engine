#pragma once

#include "Engine/Utils/StringUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#include <yaml-cpp/yaml.h>

namespace highlo::utils
{
	static YAML::Node Vec2ToYAMLNode(const glm::vec2 &v)
	{
		YAML::Node node;
		node.push_back(v.x);
		node.push_back(v.y);
		return node;
	}

	static YAML::Node Vec3ToYAMLNode(const glm::vec3 &v)
	{
		YAML::Node node;
		node.push_back(v.x);
		node.push_back(v.y);
		node.push_back(v.z);
		return node;
	}

	static YAML::Node Vec4ToYAMLNode(const glm::vec4 &v)
	{
		YAML::Node node;
		node.push_back(v.x);
		node.push_back(v.y);
		node.push_back(v.z);
		node.push_back(v.w);
		return node;
	}

	static YAML::Node Mat2ToYAMLNode(const glm::mat2 &mat)
	{
		YAML::Node node;
		node.push_back(mat[0][0]);
		node.push_back(mat[0][1]);
		node.push_back(mat[1][0]);
		node.push_back(mat[1][1]);
		return node;
	}

	static YAML::Node Mat3ToYAMLNode(const glm::mat3 &mat)
	{
		YAML::Node node;
		node.push_back(mat[0][0]);
		node.push_back(mat[0][1]);
		node.push_back(mat[0][2]);

		node.push_back(mat[1][0]);
		node.push_back(mat[1][1]);
		node.push_back(mat[1][2]);

		node.push_back(mat[2][0]);
		node.push_back(mat[2][1]);
		node.push_back(mat[2][2]);
		return node;
	}
		
	static YAML::Node Mat4ToYAMLNode(const glm::mat4 &mat)
	{
		YAML::Node node;
		node.push_back(mat[0][0]);
		node.push_back(mat[0][1]);
		node.push_back(mat[0][2]);
		node.push_back(mat[0][3]);

		node.push_back(mat[1][0]);
		node.push_back(mat[1][1]);
		node.push_back(mat[1][2]);
		node.push_back(mat[1][3]);

		node.push_back(mat[2][0]);
		node.push_back(mat[2][1]);
		node.push_back(mat[2][2]);
		node.push_back(mat[2][3]);

		node.push_back(mat[3][0]);
		node.push_back(mat[3][1]);
		node.push_back(mat[3][2]);
		node.push_back(mat[3][3]);
		return node;
	}

	static YAML::Node QuatToYAMLNode(const glm::quat &quat)
	{
		YAML::Node node;
		node.push_back(quat.w);
		node.push_back(quat.x);
		node.push_back(quat.y);
		node.push_back(quat.z);
		return node;
	}

	static bool YAMLNodeToVec2(const YAML::Node &node, glm::vec2 *outVector)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;

		(*outVector).x = node[0].as<float>();
		(*outVector).y = node[1].as<float>();
		return true;
	}

	static bool YAMLNodeToVec3(const YAML::Node &node, glm::vec3 *outVector)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		(*outVector).x = node[0].as<float>();
		(*outVector).y = node[1].as<float>();
		(*outVector).z = node[2].as<float>();
		return true;
	}

	static bool YAMLNodeToVec4(const YAML::Node &node, glm::vec4 *outVector)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		(*outVector).x = node[0].as<float>();
		(*outVector).y = node[1].as<float>();
		(*outVector).z = node[2].as<float>();
		(*outVector).w = node[3].as<float>();
		return true;
	}

	static bool YAMLNodeToMat2(const YAML::Node &node, glm::mat2 *outMatrix)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		(*outMatrix)[0][0] = node[0].as<float>();
		(*outMatrix)[0][1] = node[1].as<float>();
		(*outMatrix)[1][0] = node[2].as<float>();
		(*outMatrix)[1][1] = node[3].as<float>();
		return true;
	}

	static bool YAMLNodeToMat3(const YAML::Node &node, glm::mat3 *outMatrix)
	{
		if (!node.IsSequence() || node.size() != 9)
			return false;

		(*outMatrix)[0][0] = node[0].as<float>();
		(*outMatrix)[0][1] = node[1].as<float>();
		(*outMatrix)[0][2] = node[2].as<float>();

		(*outMatrix)[1][0] = node[3].as<float>();
		(*outMatrix)[1][1] = node[4].as<float>();
		(*outMatrix)[1][2] = node[5].as<float>();

		(*outMatrix)[2][0] = node[6].as<float>();
		(*outMatrix)[2][1] = node[7].as<float>();
		(*outMatrix)[2][2] = node[8].as<float>();
		return true;
	}

	static bool YAMLNodeToMat4(const YAML::Node &node, glm::mat4 *outMatrix)
	{
		if (!node.IsSequence() || node.size() != 16)
			return false;

		(*outMatrix)[0][0] = node[0].as<float>();
		(*outMatrix)[0][1] = node[1].as<float>();
		(*outMatrix)[0][2] = node[2].as<float>();
		(*outMatrix)[0][3] = node[3].as<float>();

		(*outMatrix)[1][0] = node[4].as<float>();
		(*outMatrix)[1][1] = node[5].as<float>();
		(*outMatrix)[1][2] = node[6].as<float>();
		(*outMatrix)[1][3] = node[7].as<float>();

		(*outMatrix)[2][0] = node[8].as<float>();
		(*outMatrix)[2][1] = node[9].as<float>();
		(*outMatrix)[2][2] = node[10].as<float>();
		(*outMatrix)[2][3] = node[11].as<float>();

		(*outMatrix)[3][0] = node[12].as<float>();
		(*outMatrix)[3][1] = node[13].as<float>();
		(*outMatrix)[3][2] = node[14].as<float>();
		(*outMatrix)[3][3] = node[15].as<float>();
		return true;
	}

	static bool YAMLNodeToQuat(const YAML::Node &node, glm::quat *outQuat)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		(*outQuat).w = node[0].as<float>();
		(*outQuat).x = node[1].as<float>();
		(*outQuat).y = node[2].as<float>();
		(*outQuat).z = node[3].as<float>();
		return true;
	}
}

