// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#define MESH_DEBUG_LOG 0

#if MESH_DEBUG_LOG
#define HL_MESH_LOG(...) HL_CORE_TRACE(__VA_ARGS__)
#define HL_MESH_ERROR(...) HL_CORE_ERROR(__VA_ARGS__)
#else
#define HL_MESH_LOG(...)
#define HL_MESH_ERROR(...)
#endif

#include "Engine/Math/Math.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace highlo::utils
{
	glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4 &matrix)
	{
		glm::mat4 result;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
		result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
		result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
		result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
		return result;
	}

	void PrintNode(aiNode *node, size_t depth)
	{
		HL_MESH_LOG("{0:^{1}}{2} {{", "", depth * 3, node->mName.C_Str());
		++depth;
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transform = Mat4FromAIMatrix4x4(node->mTransformation);
		Math::Decompose(transform, translation, rotation, scale);
		rotation = glm::degrees(rotation);

		HL_MESH_LOG("{0:^{1}}translation: ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, translation.x, translation.y, translation.z);
		HL_MESH_LOG("{0:^{1}}rotation:    ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, rotation.x, rotation.y, rotation.z);
		HL_MESH_LOG("{0:^{1}}scale:       ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, scale.x, scale.y, scale.z);
		for (uint32 i = 0; i < node->mNumChildren; ++i)
		{
			PrintNode(node->mChildren[i], depth);
		}
		--depth;
		HL_MESH_LOG("{0:^{1}}}}", "", depth * 3);
	}
}

