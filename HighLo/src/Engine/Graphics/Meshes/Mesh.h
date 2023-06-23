// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Math/AABB.h"

#include <glm/glm.hpp>

#define NUM_ATTRIBS 5

namespace highlo
{
	/// <summary>
	/// Represents a non-animated Vertex position
	/// </summary>
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 BiNormal;
		glm::vec2 TexCoord;

		HLAPI bool operator==(const Vertex &other) const
		{
			return
				Position == other.Position &&
				Normal == other.Normal &&
				Tangent == other.Tangent &&
				BiNormal == other.BiNormal &&
				TexCoord == other.TexCoord;
		}

		HLAPI bool operator!=(const Vertex &other) const
		{
			return !(*this == other);
		}
	};

	/// <summary>
	/// Represents a animated Vertex position
	/// </summary>
	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 BiNormal;
		glm::vec2 TexCoord;

		uint32 BoneIds[4] = { 0, 0, 0, 0 };
		float BoneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		HLAPI bool operator==(const AnimatedVertex &other) const
		{
			return 
				Position == other.Position &&
				Normal == other.Normal &&
				Tangent == other.Tangent &&
				BiNormal == other.BiNormal &&
				TexCoord == other.TexCoord &&

				BoneIds[0] == other.BoneIds[0] &&
				BoneIds[1] == other.BoneIds[1] &&
				BoneIds[2] == other.BoneIds[2] &&
				BoneIds[3] == other.BoneIds[3] &&

				BoneWeights[0] == other.BoneWeights[0] &&
				BoneWeights[1] == other.BoneWeights[1] &&
				BoneWeights[2] == other.BoneWeights[2] &&
				BoneWeights[3] == other.BoneWeights[3];
		}

		HLAPI bool operator!=(const AnimatedVertex &other) const
		{
			return !(*this == other);
		}

		void AddBone(uint32 id, float weight)
		{
			for (uint32 i = 0; i < 4; ++i)
			{
				if (BoneWeights[i] == 0.0f)
				{
					BoneIds[i] = id;
					BoneWeights[i] = weight;
					return;
				}
			}

			HL_CORE_WARN("Vertex has more than four bones affecting it! Extra information will be discarded. (ID={0}, Weight={1})", id, weight);
		}
	};

	/// <summary>
	/// Represents a single bone
	/// </summary>
	struct VertexBone
	{
		uint32 Ids[4];
		float Weights[4];

		VertexBone()
		{
			memset(Ids, 0, sizeof(Ids));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBone(uint32 id, float weight)
		{
			for (uint32 i = 0; i < 4; ++i)
			{
				if (Weights[i] == 0.0f)
				{
					Ids[i] = id;
					Weights[i] = weight;
					return;
				}
			}

			HL_ASSERT(false, "Too many bones!");
		}
	};

	/// <summary>
	/// Represents the bone transformation
	/// </summary>
	struct BoneInfo
	{
		glm::mat4 SubMeshInverseTransform;
		glm::mat4 InverseBindPose;
		uint32 SubMeshIndex;
		uint32 BoneIndex;

		BoneInfo() = default;
		BoneInfo(glm::mat4 subMeshInverseTransform, glm::mat4 inverseBindPose, uint32 subMeshIndex, uint32 boneIndex)
			: SubMeshInverseTransform(subMeshInverseTransform)
			, InverseBindPose(inverseBindPose)
			, SubMeshIndex(subMeshIndex)
			, BoneIndex(boneIndex)
		{
		}
	};

	/// <summary>
	/// Represents a single Triangle in a mesh
	/// </summary>
	struct Triangle
	{
		Vertex V0, V1, V2;

		Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2)
			: V0(v0), V1(v1), V2(v2)
		{
		}
	};

	struct MeshNode
	{
		uint32 Parent = 0xffffffff;
		std::vector<uint32> Children;
		std::vector<uint32> Submeshes;

		HLString Name;
		glm::mat4 LocalTransform;

		inline bool IsRoot() const { return Parent == 0xffffffff; }
	};

	struct BoneInfluence
	{
		uint32 BoneInfoIndices[4] = { 0, 0, 0, 0 };
		float Weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		void AddBoneData(uint32 boneInfoIndex, float weight)
		{
			if (weight < 0.0f || weight > 1.0f)
			{
				HL_CORE_WARN("Vertex bone weight is out of range. We will clamp it to [0, 1] (BoneID={0}, Weight={1})", boneInfoIndex, weight);
				weight = std::clamp(weight, 0.0f, 1.0f);
			}
			if (weight > 0.0f)
			{
				for (size_t i = 0; i < 4; i++)
				{
					if (Weights[i] == 0.0f)
					{
						BoneInfoIndices[i] = boneInfoIndex;
						Weights[i] = weight;
						return;
					}
				}

				// Note: when importing from assimp we are passing aiProcess_LimitBoneWeights which automatically keeps only the top N (where N defaults to 4)
				//       bone weights (and normalizes the sum to 1), which is exactly what we want.
				//       So, we should never get here.
				HL_CORE_WARN("Vertex has more than four bones affecting it, extra bone influences will be discarded (BoneID={0}, Weight={1})", boneInfoIndex, weight);
			}
		}

		void NormalizeWeights()
		{
			float sumWeights = 0.0f;
			for (size_t i = 0; i < 4; i++)
			{
				sumWeights += Weights[i];
			}
			if (sumWeights > 0.0f)
			{
				for (size_t i = 0; i < 4; i++)
				{
					Weights[i] /= sumWeights;
				}
			}
		}
	};

	/// <summary>
	/// Represents a mesh inside of a model (this is basically a submesh)
	/// </summary>
	struct Mesh
	{
		uint32 BaseVertex;
		uint32 BaseIndex;
		uint32 MaterialIndex;
		uint32 IndexCount;
		uint32 VertexCount;

		glm::mat4 Transform{ 1.0f }; // World transform
		glm::mat4 LocalTransform{ 1.0f };
		AABB BoundingBox;

		HLString NodeName, MeshName;
		bool IsRigged = false;
	};

	struct Index
	{
		uint32 V1, V2, V3;
	};

	static_assert(sizeof(Index) == 3 * sizeof(uint32));
}

