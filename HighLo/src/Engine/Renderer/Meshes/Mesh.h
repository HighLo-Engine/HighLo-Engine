// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Math/AABB.h"

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
	/// Represents a Index per Vertex (indices for the submesh)
	/// </summary>
	struct VertexIndex
	{
		uint32 P1;
		uint32 P2;
		uint32 P3;
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
		Transform BoneOffset;
		Transform BoneTransform;
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

		Transform WorldTransform;
		Transform LocalTransform;
		AABB BoundingBox;

		HLString NodeName;
		HLString MeshName;
	};
}

