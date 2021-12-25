#pragma once

#include "Engine/Core/FileSystemPath.h"
#include "Engine/Renderer/Shaders/Shader.h"

#include "Engine/Renderer/Meshes/Mesh.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Math/AABB.h"

namespace highlo
{
	/// <summary>
	/// Interface to import a model or an animation of any type
	/// </summary>
	class MeshLoader : public IsSharedReference
	{
	public:

		virtual ~MeshLoader() {}

		virtual const Ref<VertexBuffer> &GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;
		virtual const BufferLayout &GetLayout() const = 0;

		// Materials
		virtual const std::vector<Ref<Material>> &GetMaterials() const = 0;
		virtual const std::vector<Ref<Texture2D>> &GetTextures() const = 0;
		virtual const std::vector<Ref<Texture2D>> &GetNormalMaps() const = 0;

		// Anim
		virtual const glm::mat4 &GetInverseTransform() const = 0;
		virtual const std::vector<glm::mat4> &GetBoneTransforms() const = 0;
		virtual const std::unordered_map<uint32, std::vector<Triangle>> &GetTriangleCache() const = 0;
		virtual const std::vector<Mesh> &GetSubmeshes() const = 0;

		virtual uint32 GetBoneCount() const = 0;
		virtual const std::unordered_map<HLString, uint32> &GetBoneMappings() const = 0;
		virtual const std::vector<BoneInfo> &GetBoneInfos() const = 0;
		virtual bool IsAnimated() const = 0;
		virtual const FileSystemPath &GetFilePath() const = 0;
		virtual const AABB &GetBoundingBox() const = 0;

		virtual const std::vector<Vertex> &GetStaticVertices() const = 0;
		virtual const std::vector<AnimatedVertex> &GetAnimatedVertices() const = 0;
		virtual const std::vector<VertexIndex> &GetIndices() const = 0;

		virtual float GetTicksPerSecond() const = 0;
		virtual float GetAnimationDuration() const = 0;

		virtual void ManipulateBoneTransform(float time, std::vector<glm::mat4> &outTransforms) = 0;

		static Ref<MeshLoader> Create(const FileSystemPath &filePath, const Ref<Shader> &shader);
	};
}

