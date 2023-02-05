#include "HighLoPch.h"
#include "AssimpMeshLoader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	namespace utils
	{
		static glm::mat4 Mat4FromAssimp(const aiMatrix4x4 &matrix)
		{
			glm::mat4 result;
			// a,b,c,d in assimp is the row - 1,2,3,4 is the column
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;
		}

		static FileSystemPath GetTexturePath(const FileSystemPath &rootPath, const HLString &newPath)
		{
			FileSystemPath parent = rootPath.ParentPath();
			parent /= newPath;
			return parent;
		}
	}

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		virtual void write(const char *msg) override
		{
			HL_CORE_ERROR("Assimp Error: {0}", msg);
		}
	};

	AssimpMeshLoader::AssimpMeshLoader(const FileSystemPath &filePath, const Ref<Shader> &shader)
		: m_FilePath(filePath)
	{
		LogStream::Initialize();

		m_Importer = UniqueRef<Assimp::Importer>::Create();
		const aiScene *scene = m_Importer->ReadFile(*filePath.String(), m_MeshImportFlags);
		if (!scene || !scene->HasMeshes())
			return;

		m_Scene = scene;
		m_IsAnimated = scene->mAnimations != nullptr;
		m_InverseTransform = glm::inverse(utils::Mat4FromAssimp(scene->mRootNode->mTransformation));

		if (scene->mAnimations)
		{
			m_TicksPerSecond = (float)scene->mAnimations[0]->mTicksPerSecond;
			m_AnimationDuration = (float)scene->mAnimations[0]->mDuration;
		}
		else
		{
			m_TicksPerSecond = -1.0f;
			m_AnimationDuration = -1.0f;
		}

		m_BoundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		m_BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		uint32 vertexCount = 0;
		uint32 indexCount = 0;

		m_Submeshes.reserve(scene->mNumMeshes);
		for (uint32 i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh *mesh = scene->mMeshes[i];

			Mesh &submesh = m_Submeshes.emplace_back();
			submesh.BaseVertex = vertexCount;
			submesh.BaseIndex = indexCount;
			submesh.MaterialIndex = mesh->mMaterialIndex;
			submesh.VertexCount = mesh->mNumVertices;
			submesh.IndexCount = mesh->mNumFaces * 3;
			submesh.MeshName = mesh->mName.C_Str();

			vertexCount += submesh.VertexCount;
			indexCount += submesh.IndexCount;

			HL_ASSERT(mesh->HasPositions(), "Mesh requires to have vertex positions!");
			HL_ASSERT(mesh->HasNormals(), "Mesh requires to have normal positions!");

			// Load Vertices
			if (m_IsAnimated)
			{
				for (uint32 j = 0; j < mesh->mNumVertices; ++j)
				{
					AnimatedVertex vertex;
					vertex.Position = { mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z };
					vertex.Normal = { mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z };

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z };
						vertex.BiNormal = { mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z };
					}

					if (mesh->HasTextureCoords(0))
					{
						vertex.TexCoord = { mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y };
					}

					m_AnimatedVertices.push_back(vertex);
				}
			}
			else
			{
				auto &aabb = submesh.BoundingBox;
				aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
				aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				for (uint32 j = 0; j < mesh->mNumVertices; ++j)
				{
					Vertex vertex;
					vertex.Position = { mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z };
					vertex.Normal = { mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z };
					
					aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
					aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
					aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
					aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
					aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
					aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z };
						vertex.BiNormal = { mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z };
					}

					if (mesh->HasTextureCoords(0))
					{
						vertex.TexCoord = { mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y };
					}

					m_StaticVertices.push_back(vertex);
				}
			}

			// Load Indices
			for (uint32 j = 0; j < mesh->mNumFaces; ++j)
			{
				HL_ASSERT(mesh->mFaces[j].mNumIndices == 3);
				VertexIndex index = { mesh->mFaces[j].mIndices[0], mesh->mFaces[j].mIndices[1], mesh->mFaces[j].mIndices[2] };
				m_Indices.push_back(index);

				if (!m_IsAnimated)
				{
					m_TriangleCache[i].emplace_back(m_StaticVertices[index.P1 + submesh.BaseVertex],
													m_StaticVertices[index.P2 + submesh.BaseVertex],
													m_StaticVertices[index.P3 + submesh.BaseVertex]);
				}
			}

			// Load Animation
			TraverseNodes(scene->mRootNode);

			// update correct bounding box
			for (const auto &submesh : m_Submeshes)
			{
				AABB transformedSubmeshAABB = submesh.BoundingBox;
				glm::vec3 min = glm::vec3(submesh.WorldTransform.GetTransform() * glm::vec4(transformedSubmeshAABB.Min, 1.0f));
				glm::vec3 max = glm::vec3(submesh.WorldTransform.GetTransform() * glm::vec4(transformedSubmeshAABB.Max, 1.0f));

				m_BoundingBox.Min.x = glm::min(m_BoundingBox.Min.x, min.x);
				m_BoundingBox.Min.y = glm::min(m_BoundingBox.Min.y, min.y);
				m_BoundingBox.Min.z = glm::min(m_BoundingBox.Min.z, min.z);

				m_BoundingBox.Max.x = glm::max(m_BoundingBox.Max.x, max.x);
				m_BoundingBox.Max.y = glm::max(m_BoundingBox.Max.y, max.y);
				m_BoundingBox.Max.z = glm::max(m_BoundingBox.Max.z, max.z);
			}

			// Load Single bones
			if (m_IsAnimated)
			{
				for (uint32 i = 0; i < scene->mNumMeshes; ++i)
				{
					aiMesh *mesh = scene->mMeshes[i];
					Mesh &submesh = m_Submeshes[i];

					for (uint32 j = 0; j < mesh->mNumBones; ++j)
					{
						aiBone *bone = mesh->mBones[j];
						HLString boneName(bone->mName.data);
						int32 boneIndex = 0;

						if (m_BoneMapping.find(boneName) == m_BoneMapping.end())
						{
							boneIndex = m_BoneCount;
							++m_BoneCount;
							BoneInfo bi;
							m_BoneInfos.push_back(bi);
							m_BoneInfos[boneIndex].BoneOffset.SetTransform(utils::Mat4FromAssimp(bone->mOffsetMatrix));
							m_BoneMapping[boneName] = boneIndex;
						}
						else
						{
							boneIndex = m_BoneMapping[boneName];
						}

						for (uint32 j = 0; j < bone->mNumWeights; ++j)
						{
							int32 id = submesh.BaseVertex + bone->mWeights[j].mVertexId;
							float weight = bone->mWeights[j].mWeight;
							m_AnimatedVertices[id].AddBone(boneIndex, weight);
						}
					}
				}
			}

			// Load Materials
			Ref<Texture2D> whiteTex = Renderer::GetWhiteTexture();
			if (scene->HasMaterials())
			{
				m_Textures.resize(scene->mNumMaterials);
				m_Materials.resize(scene->mNumMaterials);

				for (uint32 i = 0; i < scene->mNumMaterials; ++i)
				{
					auto aiMaterial = scene->mMaterials[i];
					auto aiMaterialName = aiMaterial->GetName();

					auto mi = Material::Create(shader, aiMaterialName.data);
					m_Materials[i] = mi;

					aiString aiTexPath;
					uint32 textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);

					glm::vec3 diffuseColor(0.0f);
					float emission = 0.0f;
					aiColor3D aiColor, aiEmission;
					float shininess, metalness, roughness;

					// Load fallback colors

					// Diffuse and emission
					if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
						diffuseColor = { aiColor.r, aiColor.g, aiColor.b };

					if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == AI_SUCCESS)
						emission = aiEmission.r;

					mi->Set("u_MaterialUniforms.DiffuseColor", diffuseColor);
					mi->Set("u_MaterialUniforms.Emission", emission);

					// Shininess and metalness
					if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
						shininess = 80.0f;

					if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
						metalness = 0.0f;

					// Roughness
					roughness = 1.0f - glm::sqrt(shininess / 100.0f);

					// Load real texture maps

					// Diffuse
					bool hasDiffuseMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
					bool useFallback = !hasDiffuseMap;

					if (hasDiffuseMap)
					{
						FileSystemPath texturePath = utils::GetTexturePath(filePath, aiTexPath.data);
						HL_CORE_TRACE("Loading Diffuse Map: {0}", *texturePath);

						// SRGB Texture
						Ref<Texture2D> tex = Texture2D::LoadFromFile(texturePath);
						tex->GetSpecification().Format = TextureFormat::SRGB;
						if (tex->IsLoaded())
						{
							m_Textures[i] = tex;
							mi->Set("u_DiffuseTexture", tex);
							mi->Set("u_MaterialUniforms.DiffuseColor", glm::vec3(1.0f));
						}
						else
						{
							HL_CORE_ERROR("Could not load texture: {0}", **texturePath);
							m_Textures[i] = whiteTex;
							useFallback = true;
						}
					}

					if (useFallback)
					{
						mi->Set("u_DiffuseTexture", whiteTex);
					}

					// Normal
					bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
					useFallback = !hasNormalMap;

					if (hasNormalMap)
					{
						FileSystemPath texturePath = utils::GetTexturePath(filePath, aiTexPath.data);
						HL_CORE_TRACE("Loading Normal Map: {0}", **texturePath);

						Ref<Texture2D> tex = Texture2D::LoadFromFile(texturePath);
						if (tex->IsLoaded())
						{
							m_Textures.push_back(tex);
							m_NormalMaps.push_back(tex);
							mi->Set("u_NormalTexture", tex);
							mi->Set("u_MaterialUniforms.UseNormalMap", true);
						}
						else
						{
							HL_CORE_ERROR("Could not load texture: {0}", **texturePath);
							useFallback = true;
						}
					}

					if (useFallback)
					{
						mi->Set("u_NormalTexture", whiteTex);
						mi->Set("u_MaterialUniforms.UseNormalMap", false);
					}

					// Roughness
					bool hasRoughnessMap = aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS;
					useFallback = !hasRoughnessMap;

					if (hasRoughnessMap)
					{
						FileSystemPath texturePath = utils::GetTexturePath(filePath, aiTexPath.data);
						HL_CORE_TRACE("Loading Roughness Map: {0}", **texturePath);

						Ref<Texture2D> tex = Texture2D::LoadFromFile(texturePath);
						if (tex->IsLoaded())
						{
							m_Textures.push_back(tex);
							mi->Set("u_RoughnessTexture", tex);
							mi->Set("u_MaterialUniforms.Roughness", 1.0f);
						}
						else
						{
							HL_CORE_ERROR("Could not load texture: {0}", **texturePath);
							useFallback = true;
						}
					}

					if (useFallback)
					{
						mi->Set("u_RoughnessTexture", whiteTex);
						mi->Set("u_MaterialUniforms.Roughness", roughness);
					}

					// Metalness
					bool metalnessTextureFound = false;
					for (uint32 j = 0; j < aiMaterial->mNumProperties; ++j)
					{
						auto property = aiMaterial->mProperties[j];
						if (property->mType == aiPTI_String)
						{
							uint32 strLength = *(uint32*)property->mData;
							HLString str(property->mData + 4, strLength);
							HLString key = property->mKey.data;

							if (key == "$raw.ReflectionFactor|file")
							{
								FileSystemPath texturePath = utils::GetTexturePath(filePath, str);
								Ref<Texture2D> tex = Texture2D::LoadFromFile(texturePath);
								if (tex->IsLoaded())
								{
									metalnessTextureFound = true;
									m_Textures.push_back(tex);
									mi->Set("u_MetalnessTexture", tex);
									mi->Set("u_MaterialUniforms.Metalness", 1.0f);
								}
								else
								{
									HL_CORE_ERROR("Could not load texture: {0}", **texturePath);
								}
							}
						}
					}

					useFallback = !metalnessTextureFound;
					if (useFallback)
					{
						mi->Set("u_MetalnessTexture", whiteTex);
						mi->Set("u_MaterialUniforms.Metalness", metalness);
					}
				}
			}
			else
			{
				auto mi = Material::Create(shader, "HighLo-Default-Material");
				mi->Set("u_MaterialUniforms.DiffuseColor", glm::vec3(0.8f));
				mi->Set("u_MaterialUniforms.Emission", 0.0f);
				mi->Set("u_MaterialUniforms.Metalness", 0.0f);
				mi->Set("u_MaterialUniforms.Roughness", 0.8f);
				mi->Set("u_MaterialUniforms.UseNormalMap", false);

				mi->Set("u_AlbedoTexture", whiteTex);
				mi->Set("u_MetalnessTexture", whiteTex);
				mi->Set("u_RoughnessTexture", whiteTex);
				m_Materials.push_back(mi);
			}

			if (m_IsAnimated)
			{
				m_Layout = BufferLayout::GetAnimatedShaderLayout();
				m_VertexBuffer = VertexBuffer::Create(m_AnimatedVertices.data(), (uint32)(m_AnimatedVertices.size() * sizeof(AnimatedVertex)));
			}
			else
			{
				m_Layout = BufferLayout::GetStaticShaderLayout();
				m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32)(m_StaticVertices.size() * sizeof(Vertex)));
			}

			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32)(m_Indices.size() * sizeof(VertexIndex)));
		}
	}

	AssimpMeshLoader::~AssimpMeshLoader()
	{
	}
	
	void AssimpMeshLoader::ManipulateBoneTransform(float time, std::vector<glm::mat4> &outTransforms)
	{
		ReadNodeHierarchy(time, m_Scene->mRootNode, glm::mat4(1.0f));
		m_BoneTransforms.resize(m_BoneCount);
		for (uint32 i = 0; i < m_BoneCount; ++i)
			m_BoneTransforms[i] = m_BoneInfos[i].BoneTransform.GetTransform();

		outTransforms.resize(m_BoneCount);
		for (uint32 i = 0; i < m_BoneCount; ++i)
			outTransforms[i] = m_BoneInfos[i].BoneTransform.GetTransform();
	}

	void AssimpMeshLoader::BoneTransform(float time)
	{
		ReadNodeHierarchy(time, m_Scene->mRootNode, glm::mat4(1.0f));
		m_BoneTransforms.resize(m_BoneCount);
		for (uint32 i = 0; i < m_BoneCount; ++i)
			m_BoneTransforms[i] = m_BoneInfos[i].BoneTransform.GetTransform();
	}
	
	void AssimpMeshLoader::ReadNodeHierarchy(float animTime, const aiNode *node, const glm::mat4 &parentTransform)
	{
		HLString name(node->mName.data);
		const aiAnimation *anim = m_Scene->mAnimations[0];
		glm::mat4 nodeTransform(utils::Mat4FromAssimp(node->mTransformation));
		const aiNodeAnim *nodeAnim = FindNodeAnim(anim, name);
		glm::mat4 transform;

		if (nodeAnim)
		{
			glm::vec3 translation = InterpolateTranslation(animTime, nodeAnim);
			glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

			glm::quat rotation = InterpolateRotation(animTime, nodeAnim);
			glm::mat4 rotationMat = glm::toMat4(rotation);

			glm::vec3 scale = InterpolateScale(animTime, nodeAnim);
			glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));

			nodeTransform = translationMat * rotationMat * scaleMat;
		}

		transform = parentTransform * nodeTransform;

		if (m_BoneMapping.find(name) != m_BoneMapping.end())
		{
			uint32 boneIndex = m_BoneMapping[name];
			m_BoneInfos[boneIndex].BoneTransform.SetTransform(m_InverseTransform * transform * m_BoneInfos[boneIndex].BoneOffset.GetTransform());
		}

		for (uint32 i = 0; i < node->mNumChildren; ++i)
		{
			ReadNodeHierarchy(animTime, node->mChildren[i], transform);
		}
	}
	
	void AssimpMeshLoader::TraverseNodes(aiNode *node, const glm::mat4 &parentTransform, uint32 level)
	{
		glm::mat4 localTransform = utils::Mat4FromAssimp(node->mTransformation);
		glm::mat4 transform = parentTransform * localTransform;
		m_NodeMap[node].resize(node->mNumMeshes);

		for (uint32 i = 0; i < node->mNumMeshes; ++i)
		{
			uint32 mesh = node->mMeshes[i];
			auto &submesh = m_Submeshes[mesh];
			submesh.NodeName = node->mName.C_Str();
			submesh.WorldTransform.SetTransform(transform);
			submesh.LocalTransform.SetTransform(localTransform);
			m_NodeMap[node][i] = mesh;
		}

		for (uint32 i = 0; i < node->mNumChildren; ++i)
		{
			TraverseNodes(node->mChildren[i], transform, level + 1);
		}
	}
	
	const aiNodeAnim *AssimpMeshLoader::FindNodeAnim(const aiAnimation *anim, const HLString &name)
	{
		for (uint32 i = 0; i < anim->mNumChannels; ++i)
		{
			const aiNodeAnim *nodeAnim = anim->mChannels[i];
			if (HLString(nodeAnim->mNodeName.data) == name)
			{
				return nodeAnim;
			}
		}

		return nullptr;
	}
	
	uint32 AssimpMeshLoader::FindPosition(float animTime, const aiNodeAnim *anim)
	{
		for (uint32 i = 0; i < anim->mNumPositionKeys; ++i)
		{
			if (animTime < (float)anim->mPositionKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	
	uint32 AssimpMeshLoader::FindScale(float animTime, const aiNodeAnim *anim)
	{
		for (uint32 i = 0; i < anim->mNumScalingKeys; ++i)
		{
			if (animTime < (float)anim->mScalingKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	
	uint32 AssimpMeshLoader::FindRotation(float animTime, const aiNodeAnim *anim)
	{
		for (uint32 i = 0; i < anim->mNumRotationKeys; ++i)
		{
			if (animTime < (float)anim->mRotationKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	
	glm::vec3 AssimpMeshLoader::InterpolateTranslation(float animTime, const aiNodeAnim *anim)
	{
		// No interpolation necessary for single value
		if (anim->mNumPositionKeys == 1)
		{
			auto v = anim->mPositionKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32 positionIndex = FindPosition(animTime, anim);
		uint32 nextPositionIndex = positionIndex + 1;
		HL_ASSERT(nextPositionIndex < anim->mNumPositionKeys);

		float deltaTime = (float)(anim->mPositionKeys[nextPositionIndex].mTime - anim->mPositionKeys[positionIndex].mTime);
		float factor = (animTime - (float)anim->mPositionKeys[positionIndex].mTime) / deltaTime;
		
		// Factor must be between 0 and 1
		HL_ASSERT(factor <= 1.0f);
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const aiVector3D &start = anim->mPositionKeys[positionIndex].mValue;
		const aiVector3D &end = anim->mPositionKeys[nextPositionIndex].mValue;
		aiVector3D delta = end - start;

		auto result = start + factor * delta;
		return { result.x, result.y, result.z };
	}
	
	glm::vec3 AssimpMeshLoader::InterpolateScale(float animTime, const aiNodeAnim *anim)
	{
		// No interpolation necessary for single value
		if (anim->mNumScalingKeys == 1)
		{
			auto v = anim->mScalingKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32 scaleIndex = FindScale(animTime, anim);
		uint32 nextScaleIndex = scaleIndex + 1;
		HL_ASSERT(nextScaleIndex < anim->mNumScalingKeys);

		float deltaTime = (float)(anim->mScalingKeys[nextScaleIndex].mTime - anim->mScalingKeys[scaleIndex].mTime);
		float factor = (animTime - (float)anim->mScalingKeys[scaleIndex].mTime) / deltaTime;

		// Factor must be between 0 and 1
		HL_ASSERT(factor <= 1.0f);
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const auto &start = anim->mScalingKeys[scaleIndex].mValue;
		const auto &end = anim->mScalingKeys[nextScaleIndex].mValue;
		auto delta = end - start;

		auto result = start + factor * delta;
		return { result.x, result.y, result.z };
	}
	
	glm::quat AssimpMeshLoader::InterpolateRotation(float animTime, const aiNodeAnim *anim)
	{
		// No interpolation necessary for single value
		if (anim->mNumRotationKeys == 1)
		{
			auto v = anim->mRotationKeys[0].mValue;
			return glm::quat(v.w, v.x, v.y, v.z);
		}

		uint32 rotationIndex = FindRotation(animTime, anim);
		uint32 nextRotationIndex = rotationIndex + 1;
		HL_ASSERT(nextRotationIndex < anim->mNumRotationKeys);

		float deltaTime = (float)(anim->mRotationKeys[nextRotationIndex].mTime - anim->mRotationKeys[rotationIndex].mTime);
		float factor = (animTime - (float)anim->mRotationKeys[rotationIndex].mTime) / deltaTime;

		// Factor must be between 0 and 1
		HL_ASSERT(factor <= 1.0f);
		factor = glm::clamp(factor, 0.0f, 1.0f);

		const aiQuaternion &start = anim->mRotationKeys[rotationIndex].mValue;
		const aiQuaternion &end = anim->mRotationKeys[nextRotationIndex].mValue;
		
		aiQuaternion result = aiQuaternion();
		aiQuaternion::Interpolate(result, start, end, factor);
		result = result.Normalize();
		return glm::quat(result.w, result.x, result.y, result.z);
	}
}

