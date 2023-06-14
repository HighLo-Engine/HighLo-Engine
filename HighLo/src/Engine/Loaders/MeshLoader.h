// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-21) initial release
//

#pragma once

#include "Engine/Core/FileSystemPath.h"
#include "Engine/Math/AABB.h"

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Meshes/Mesh.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Graphics/Texture.h"

namespace highlo
{
	/// <summary>
	/// Interface to import a model or an animation of any type
	/// </summary>
	class MeshLoader : public IsSharedReference
	{
	public:

		HLAPI virtual ~MeshLoader() {}

		HLAPI virtual const Ref<VertexBuffer> &GetVertexBuffer() const = 0;
		HLAPI virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;
		HLAPI virtual const BufferLayout &GetLayout() const = 0;

		// Materials
		HLAPI virtual const std::vector<Ref<Material>> &GetMaterials() const = 0;
		HLAPI virtual const std::vector<Ref<Texture2D>> &GetTextures() const = 0;
		HLAPI virtual const std::vector<Ref<Texture2D>> &GetNormalMaps() const = 0;

		// Anim
		HLAPI virtual const glm::mat4 &GetInverseTransform() const = 0;
		HLAPI virtual const std::vector<glm::mat4> &GetBoneTransforms() const = 0;
		HLAPI virtual const std::unordered_map<uint32, std::vector<Triangle>> &GetTriangleCache() const = 0;
		HLAPI virtual const std::vector<Mesh> &GetSubmeshes() const = 0;

		HLAPI virtual uint32 GetBoneCount() const = 0;
		HLAPI virtual const std::unordered_map<HLString, uint32> &GetBoneMappings() const = 0;
		HLAPI virtual const std::vector<BoneInfo> &GetBoneInfos() const = 0;
		HLAPI virtual bool IsAnimated() const = 0;
		HLAPI virtual const FileSystemPath &GetFilePath() const = 0;
		HLAPI virtual const AABB &GetBoundingBox() const = 0;

		HLAPI virtual const std::vector<Vertex> &GetStaticVertices() const = 0;
		HLAPI virtual const std::vector<AnimatedVertex> &GetAnimatedVertices() const = 0;
		HLAPI virtual const std::vector<VertexIndex> &GetIndices() const = 0;

		HLAPI virtual float GetTicksPerSecond() const = 0;
		HLAPI virtual float GetAnimationDuration() const = 0;

		HLAPI virtual void ManipulateBoneTransform(float time, std::vector<glm::mat4> &outTransforms) = 0;

		HLAPI static Ref<MeshLoader> Create(const FileSystemPath &filePath, const Ref<Shader> &shader);
	};
}

