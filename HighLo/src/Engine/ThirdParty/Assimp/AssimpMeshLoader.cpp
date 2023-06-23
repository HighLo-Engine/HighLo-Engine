// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssimpMeshLoader.h"

#include "AssimpUtils.h"
#include "AssimpAnimationImporter.h"
#include "AssimpLogStream.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Engine/Renderer/Renderer.h"
#include "Engine/Assets/AssetManager.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#define DEBUG_PRINT_ALL_PROPS 1

namespace highlo
{
	static const uint32 s_MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		//		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
		aiProcess_ValidateDataStructure |   // Validation
		aiProcess_GlobalScale;              // e.g. convert cm to m for fbx import (and other formats where cm is native)

	AssimpMeshImporter::AssimpMeshImporter(const FileSystemPath &path)
		: m_Path(path)
	{
		AssimpLogStream::Initialize();
	}

	Ref<MeshFile> AssimpMeshImporter::ImportToMeshSource()
	{
		Ref<MeshFile> meshSource = Ref<MeshFile>::Create();

		HL_CORE_INFO("Loading mesh: {0}", **m_Path);

		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene *scene = importer.ReadFile(m_Path.String().C_Str(), s_MeshImportFlags);
		if (!scene /* || !scene->HasMeshes()*/)  // note: scene can legit contain no meshes (e.g. it could contain an armature, an animation, and no skin (mesh)))
		{
			HL_CORE_ERROR("Failed to load mesh file: {0}", **m_Path);
			meshSource->SetFlag(AssetFlag::Invalid);
			return nullptr;
		}

		meshSource->m_Skeleton = ::highlo::ImportSkeleton(scene);
		HL_CORE_INFO("Skeleton {0} found in mesh file '{1}'", meshSource->HasSkeleton() ? "" : "not", **m_Path);
		if (meshSource->HasSkeleton())
		{
			const auto animationNames = ::highlo::GetAnimationNames(scene);
			meshSource->m_Animations.reserve(std::size(animationNames));
			for (const auto &animationName : animationNames)
			{
				meshSource->m_Animations.emplace_back(::highlo::ImportAnimation(scene, animationName, *meshSource->m_Skeleton));
			}
		}

		// If no meshes in the scene, there's nothing more for us to do
		if (scene->HasMeshes())
		{
			uint32 vertexCount = 0;
			uint32 indexCount = 0;

			meshSource->m_BoundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
			meshSource->m_BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

			meshSource->m_Submeshes.reserve(scene->mNumMeshes);
			for (unsigned m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh *mesh = scene->mMeshes[m];

				Mesh &submesh = meshSource->m_Submeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.VertexCount = mesh->mNumVertices;
				submesh.IndexCount = mesh->mNumFaces * 3;
				submesh.MeshName = mesh->mName.C_Str();

				vertexCount += mesh->mNumVertices;
				indexCount += submesh.IndexCount;

				HL_ASSERT(mesh->HasPositions(), "Meshes require positions.");
				HL_ASSERT(mesh->HasNormals(), "Meshes require normals.");

				// Vertices
				auto &aabb = submesh.BoundingBox;
				aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
				aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex;
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
					aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
					aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
					aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
					aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
					aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
					aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.BiNormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					meshSource->m_Vertices.push_back(vertex);
				}

				// Indices
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					HL_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
					Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
					meshSource->m_Indices.push_back(index);

					meshSource->m_TriangleCache[m].emplace_back(meshSource->m_Vertices[index.V1 + submesh.BaseVertex], meshSource->m_Vertices[index.V2 + submesh.BaseVertex], meshSource->m_Vertices[index.V3 + submesh.BaseVertex]);
				}
			}

#if MESH_DEBUG_LOG
			HL_CORE_INFO_TAG("Mesh", "Traversing nodes for scene '{0}'", filename);
			Utils::PrintNode(scene->mRootNode, 0);
#endif

			MeshNode &rootNode = meshSource->m_Nodes.emplace_back();
			TraverseNodes(meshSource, scene->mRootNode, 0);

			for (const auto &submesh : meshSource->m_Submeshes)
			{
				AABB transformedSubmeshAABB = submesh.BoundingBox;
				glm::vec3 min = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Min, 1.0f));
				glm::vec3 max = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Max, 1.0f));

				meshSource->m_BoundingBox.Min.x = glm::min(meshSource->m_BoundingBox.Min.x, min.x);
				meshSource->m_BoundingBox.Min.y = glm::min(meshSource->m_BoundingBox.Min.y, min.y);
				meshSource->m_BoundingBox.Min.z = glm::min(meshSource->m_BoundingBox.Min.z, min.z);
				meshSource->m_BoundingBox.Max.x = glm::max(meshSource->m_BoundingBox.Max.x, max.x);
				meshSource->m_BoundingBox.Max.y = glm::max(meshSource->m_BoundingBox.Max.y, max.y);
				meshSource->m_BoundingBox.Max.z = glm::max(meshSource->m_BoundingBox.Max.z, max.z);
			}
		}

		// Bones
		if (meshSource->HasSkeleton())
		{
			meshSource->m_BoneInfluences.resize(meshSource->m_Vertices.size());
			for (uint32 m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh *mesh = scene->mMeshes[m];
				Mesh &submesh = meshSource->m_Submeshes[m];

				if (mesh->mNumBones > 0)
				{
					submesh.IsRigged = true;
					for (uint32 i = 0; i < mesh->mNumBones; i++)
					{
						aiBone *bone = mesh->mBones[i];
						bool hasNonZeroWeight = false;
						for (size_t j = 0; j < bone->mNumWeights; j++)
						{
							if (bone->mWeights[j].mWeight > 0.000001f)
							{
								hasNonZeroWeight = true;
							}
						}
						if (!hasNonZeroWeight)
							continue;

						// Find bone in skeleton
						uint32 boneIndex = meshSource->m_Skeleton->GetBoneIndex(bone->mName.C_Str());
						if (boneIndex == Skeleton::NullIndex)
						{
							HL_CORE_ERROR("Could not find mesh bone '{}' in skeleton!", bone->mName.C_Str());
						}

						uint32 boneInfoIndex = ~0;
						for (size_t j = 0; j < meshSource->m_BoneInfo.size(); ++j)
						{
							// note: Same bone could influence different submeshes (and each will have different transforms in the bind pose).
							//       Hence the need to differentiate on submesh index here.
							if ((meshSource->m_BoneInfo[j].BoneIndex == boneIndex) && (meshSource->m_BoneInfo[j].SubMeshIndex == m))
							{
								boneInfoIndex = static_cast<uint32>(j);
								break;
							}
						}
						if (boneInfoIndex == ~0)
						{
							boneInfoIndex = static_cast<uint32>(meshSource->m_BoneInfo.size());
							meshSource->m_BoneInfo.emplace_back(glm::inverse(submesh.Transform), utils::Mat4FromAIMatrix4x4(bone->mOffsetMatrix), m, boneIndex);
						}

						for (size_t j = 0; j < bone->mNumWeights; j++)
						{
							int VertexID = submesh.BaseVertex + bone->mWeights[j].mVertexId;
							float Weight = bone->mWeights[j].mWeight;
							meshSource->m_BoneInfluences[VertexID].AddBoneData(boneInfoIndex, Weight);
						}
					}
				}
			}

			for (auto &boneInfluence : meshSource->m_BoneInfluences)
			{
				boneInfluence.NormalizeWeights();
			}
		}

		// Materials
		Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();
		if (scene->HasMaterials())
		{
			HL_MESH_LOG("---- Materials - {0} ----", **path);

			meshSource->m_Materials.resize(scene->mNumMaterials);

			for (uint32 i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();
				Ref<Material> mi = Material::Create(Renderer::GetShaderLibrary()->Get("HighLoPBR_Static"), aiMaterialName.data);
				meshSource->m_Materials[i] = mi;

				HL_MESH_LOG("  {0} (Index = {1})", aiMaterialName.data, i);
				aiString aiTexPath;
				uint32 textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
				HL_MESH_LOG("    TextureCount = {0}", textureCount);

				glm::vec3 diffuseColor(0.8f);
				float emission = 0.0f;
				aiColor3D aiColor, aiEmission;
				if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
					diffuseColor = { aiColor.r, aiColor.g, aiColor.b };

				if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == AI_SUCCESS)
					emission = aiEmission.r;

				mi->Set("u_MaterialUniforms.DiffuseColor", diffuseColor);
				mi->Set("u_MaterialUniforms.Emission", emission);

				float roughness, metalness;
				if (aiMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) != aiReturn_SUCCESS)
					roughness = 0.5f; // Default value

				if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
					metalness = 0.0f;

				HL_MESH_LOG("    COLOR = {0}, {1}, {2}", aiColor.r, aiColor.g, aiColor.b);
				HL_MESH_LOG("    ROUGHNESS = {0}", roughness);
				HL_MESH_LOG("    METALNESS = {0}", metalness);
				bool hasDiffuseMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
				bool fallback = !hasDiffuseMap;
				if (hasDiffuseMap)
				{
					AssetHandle textureHandle = 0;
					TextureSpecification spec;
					spec.DebugName = aiTexPath.C_Str();
					spec.Properties.SRGB = true;
					if (auto aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = TextureFormat::RGBA;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;

						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, 1, 1, aiTexEmbedded->pcData);
					//	Ref<Texture2D> &texture = AssetManager::Get()->GetAsset<Texture2D>(textureHandle);
					//	texture->SetData(aiTexEmbedded->pcData, sizeof(aiTexel) * spec.Width * spec.Height);
					}
					else
					{
						FileSystemPath parentPath = m_Path.ParentPath();
						parentPath /= HLString(aiTexPath.data);
						HL_MESH_LOG("    Diffuse map path = {0}", **parentPath);

						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, parentPath);
					}

					Ref<Texture2D> texture = AssetManager::Get()->GetAsset<Texture2D>(textureHandle);
					if (texture && texture->IsLoaded())
					{
						mi->Set("u_DiffuseTexture", texture);
						mi->Set("u_MaterialUniforms.DiffuseColor", glm::vec3(1.0f));
					}
					else
					{
						HL_CORE_ERROR("Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					HL_MESH_LOG("    No diffuse map");
					mi->Set("u_DiffuseTexture", whiteTexture);
				}

				// Normal maps
				bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
				fallback = !hasNormalMap;
				if (hasNormalMap)
				{
					AssetHandle textureHandle = 0;

					TextureSpecification spec;
					spec.DebugName = aiTexPath.C_Str();
					if (const aiTexture *aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = TextureFormat::RGB;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;
						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, Buffer(aiTexEmbedded->pcData, 1));
					}
					else
					{

						FileSystemPath parentPath = m_Path.ParentPath();
						parentPath /= HLString(aiTexPath.data);
						HLString texturePath = parentPath.String();
						HL_MESH_LOG("    Normal map path = {0}", *texturePath);
						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, texturePath);
					}

					Ref<Texture2D> texture = AssetManager::Get()->GetAsset<Texture2D>(textureHandle);
					if (texture && texture->IsLoaded())
					{
						mi->Set("u_NormalTexture", texture);
						mi->Set("u_MaterialUniforms.UseNormalMap", true);
					}
					else
					{
						HL_CORE_ERROR("    Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					HL_MESH_LOG("    No normal map");
					mi->Set("u_NormalTexture", whiteTexture);
					mi->Set("u_MaterialUniforms.UseNormalMap", false);
				}

				// Roughness map
				bool hasRoughnessMap = aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS;
				fallback = !hasRoughnessMap;
				if (hasRoughnessMap)
				{
					AssetHandle textureHandle = 0;
					TextureSpecification spec;
					spec.DebugName = aiTexPath.C_Str();
					if (const aiTexture *aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = TextureFormat::RGB;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;
						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, Buffer(aiTexEmbedded->pcData, 1));
					}
					else
					{
						auto parentPath = m_Path.ParentPath();
						parentPath /= HLString(aiTexPath.data);
						HLString texturePath = parentPath.String();
						HL_MESH_LOG("    Roughness map path = {0}", texturePath);
						textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, texturePath);
					}

					Ref<Texture2D> texture = AssetManager::Get()->GetAsset<Texture2D>(textureHandle);
					if (texture && texture->IsLoaded())
					{
						mi->Set("u_RoughnessTexture", texture);
						mi->Set("u_MaterialUniforms.Roughness", 1.0f);
					}
					else
					{
						HL_CORE_ERROR("    Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					HL_MESH_LOG("    No roughness map");
					mi->Set("u_RoughnessTexture", whiteTexture);
					mi->Set("u_MaterialUniforms.Roughness", roughness);
				}

#if 0
				// Metalness map (or is it??)
				if (aiMaterial->Get("$raw.ReflectionFactor|file", aiPTI_String, 0, aiTexPath) == AI_SUCCESS)
				{
					FileSystemPath parentPath = m_Path.ParentPath();
					parentPath /= HLString(aiTexPath.data);
					HLString texturePath = parentPath.String();

					Ref<Texture2D> texture = Texture2D::LoadFromFile(texturePath);
					if (texture->IsLoaded())
					{
						HL_MESH_LOG("    Metalness map path = {0}", texturePath);
						mi->Set("u_MetalnessTexture", texture);
						mi->Set("u_MetalnessTexToggle", 1.0f);
					}
					else
					{
						HL_CORE_ERROR("Could not load texture: {0}", texturePath);
					}
				}
				else
				{
					HL_MESH_LOG("    No metalness texture");
					mi->Set("u_Metalness", metalness);
				}
#endif

				bool metalnessTextureFound = false;
				for (uint32 p = 0; p < aiMaterial->mNumProperties; p++)
				{
					auto prop = aiMaterial->mProperties[p];

#if DEBUG_PRINT_ALL_PROPS
					HL_MESH_LOG("Material Property:");
					HL_MESH_LOG("  Name = {0}", prop->mKey.data);
					HL_MESH_LOG("  Type = {0}", prop->mType);
					HL_MESH_LOG("  Size = {0}", prop->mDataLength);
					float data = *(float *)prop->mData;
					HL_MESH_LOG("  Value = {0}", data);

					switch (prop->mSemantic)
					{
					case aiTextureType_NONE:
						HL_MESH_LOG("  Semantic = aiTextureType_NONE");
						break;
					case aiTextureType_DIFFUSE:
						HL_MESH_LOG("  Semantic = aiTextureType_DIFFUSE");
						break;
					case aiTextureType_SPECULAR:
						HL_MESH_LOG("  Semantic = aiTextureType_SPECULAR");
						break;
					case aiTextureType_AMBIENT:
						HL_MESH_LOG("  Semantic = aiTextureType_AMBIENT");
						break;
					case aiTextureType_EMISSIVE:
						HL_MESH_LOG("  Semantic = aiTextureType_EMISSIVE");
						break;
					case aiTextureType_HEIGHT:
						HL_MESH_LOG("  Semantic = aiTextureType_HEIGHT");
						break;
					case aiTextureType_NORMALS:
						HL_MESH_LOG("  Semantic = aiTextureType_NORMALS");
						break;
					case aiTextureType_SHININESS:
						HL_MESH_LOG("  Semantic = aiTextureType_SHININESS");
						break;
					case aiTextureType_OPACITY:
						HL_MESH_LOG("  Semantic = aiTextureType_OPACITY");
						break;
					case aiTextureType_DISPLACEMENT:
						HL_MESH_LOG("  Semantic = aiTextureType_DISPLACEMENT");
						break;
					case aiTextureType_LIGHTMAP:
						HL_MESH_LOG("  Semantic = aiTextureType_LIGHTMAP");
						break;
					case aiTextureType_REFLECTION:
						HL_MESH_LOG("  Semantic = aiTextureType_REFLECTION");
						break;
					case aiTextureType_UNKNOWN:
						HL_MESH_LOG("  Semantic = aiTextureType_UNKNOWN");
						break;
					}
#endif

					if (prop->mType == aiPTI_String)
					{
						uint32 strLength = *(uint32*)prop->mData;
						HLString str(prop->mData + 4, strLength);

						HLString key = prop->mKey.data;
						if (key == "$raw.ReflectionFactor|file")
						{
							AssetHandle textureHandle = 0;
							TextureSpecification spec;
							spec.DebugName = str;
							if (const aiTexture *aiTexEmbedded = scene->GetEmbeddedTexture(*str))
							{
								spec.Format = TextureFormat::RGB;
								spec.Width = aiTexEmbedded->mWidth;
								spec.Height = aiTexEmbedded->mHeight;
								textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, Buffer(aiTexEmbedded->pcData, 1));
							}
							else
							{
								FileSystemPath parentPath = m_Path.ParentPath();
								parentPath /= str;
								HLString texturePath = parentPath.String();
								HL_MESH_LOG("    Metalness map path = {0}", *texturePath);
								textureHandle = AssetManager::Get()->CreateMemoryOnlyRendererAsset<Texture2D>(spec, texturePath);
							}

							Ref<Texture2D> texture = AssetManager::Get()->GetAsset<Texture2D>(textureHandle);
							if (texture && texture->IsLoaded())
							{
								metalnessTextureFound = true;
								mi->Set("u_MetalnessTexture", texture);
								mi->Set("u_MaterialUniforms.Metalness", 1.0f);
							}
							else
							{
								HL_CORE_ERROR("    Could not load texture: {0}", *str);
							}
							break;
						}
					}
				}

				fallback = !metalnessTextureFound;
				if (fallback)
				{
					HL_MESH_LOG("    No metalness map");
					mi->Set("u_MetalnessTexture", whiteTexture);
					mi->Set("u_MaterialUniforms.Metalness", metalness);

				}
			}
			HL_MESH_LOG("------------------------");
		}
		else
		{
			Ref<Material> mi = Material::Create(Renderer::GetShaderLibrary()->Get("HighLoPBR_Static"), "HighLo-Default");
			mi->Set("u_MaterialUniforms.DiffuseColor", glm::vec3(0.8f));
			mi->Set("u_MaterialUniforms.Emission", 0.0f);
			mi->Set("u_MaterialUniforms.Metalness", 0.0f);
			mi->Set("u_MaterialUniforms.Roughness", 0.8f);
			mi->Set("u_MaterialUniforms.UseNormalMap", false);

			mi->Set("u_DiffuseTexture", whiteTexture);
			mi->Set("u_MetalnessTexture", whiteTexture);
			mi->Set("u_RoughnessTexture", whiteTexture);
			meshSource->m_Materials.push_back(mi);
		}

		if (meshSource->m_Vertices.size())
		{
			meshSource->m_VertexBuffer = VertexBuffer::Create(meshSource->m_Vertices.data(), (uint32)(meshSource->m_Vertices.size() * sizeof(Vertex)));
		}

		if (meshSource->HasSkeleton())
		{
			meshSource->m_BoneInfluenceBuffer = VertexBuffer::Create(meshSource->m_BoneInfluences.data(), (uint32)(meshSource->m_BoneInfluences.size() * sizeof(BoneInfluence)));
		}

		if (meshSource->m_Indices.size())
		{
			meshSource->m_IndexBuffer = IndexBuffer::Create(meshSource->m_Indices.data(), (uint32)(meshSource->m_Indices.size() * sizeof(Index)));
		}

		return meshSource;
	}
	
	bool AssimpMeshImporter::ImportSkeleton(UniqueReference<Skeleton> &skeleton)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene *scene = importer.ReadFile(m_Path.String().C_Str(), s_MeshImportFlags);
		if (!scene)
		{
			HL_CORE_ERROR("Failed to load mesh file: {0}", **m_Path);
			return false;
		}

		skeleton = ::highlo::ImportSkeleton(scene);
		return true;
	}
	
	bool AssimpMeshImporter::ImportAnimations(const uint32 animationIndex, const Skeleton &skeleton, std::vector<UniqueReference<Animation>> &animations)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene *scene = importer.ReadFile(m_Path.String().C_Str(), s_MeshImportFlags);
		if (!scene)
		{
			HL_CORE_ERROR("Failed to load mesh file: {0}", **m_Path);
			return false;
		}

		const auto animationNames = GetAnimationNames(scene);
		// m_Animations.reserve(std::size(animationNames));
		for (const auto &animationName : animationNames)
		{
			animations.emplace_back(ImportAnimation(scene, animationName, skeleton));
		}

		return true;
	}
	
	bool AssimpMeshImporter::IsCompatibleSkeleton(const uint32 animationIndex, const Skeleton &skeleton)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene *scene = importer.ReadFile(m_Path.String().C_Str(), s_MeshImportFlags);
		if (!scene)
		{
			HL_CORE_ERROR("Failed to load mesh file: {0}", **m_Path);
			return false;
		}

		if (scene->mNumAnimations <= animationIndex)
		{
			return false;
		}

		const auto animationNames = GetAnimationNames(scene);
		if (animationNames.empty())
		{
			return false;
		}

		const aiAnimation *anim = scene->mAnimations[animationIndex];

		std::unordered_map<HLString, uint32> boneIndices;
		for (uint32 i = 0; i < skeleton.GetNumBones(); ++i)
		{
			boneIndices.emplace(skeleton.GetBoneName(i), i);
		}

		std::set<std::tuple<uint32, aiNodeAnim *>> validChannels;
		for (uint32 channelIndex = 0; channelIndex < anim->mNumChannels; ++channelIndex)
		{
			aiNodeAnim *nodeAnim = anim->mChannels[channelIndex];
			auto it = boneIndices.find(nodeAnim->mNodeName.C_Str());
			if (it != boneIndices.end())
			{
				validChannels.emplace(it->second, nodeAnim);
			}
		}

		// It's hard to decide whether or not the animation is "valid" for the given skeleton.
		// For example an animation does not necessarily contain channels for all bones.
		// Conversely, some channels in the animation might not be for bones.
		// So, you cannot simply check number of valid channels == number of bones
		// And you cannot simply check number of invalid channels == 0
		// For now, I've just decided on a simple number of valid channels must be at least 1
		return validChannels.size() > 0;
	}
	
	uint32 AssimpMeshImporter::GetAnimationCount()
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene *scene = importer.ReadFile(m_Path.String().C_Str(), s_MeshImportFlags);
		if (!scene)
		{
			HL_CORE_ERROR("Failed to load mesh file: {0}", **m_Path);
			return false;
		}

		return (uint32)scene->mNumAnimations;
	}
	
	void AssimpMeshImporter::TraverseNodes(Ref<MeshFile> &meshSource, void *assimpNode, uint32 nodeIndex, const glm::mat4 &parentTransform, uint32 level)
	{
		aiNode *aNode = (aiNode *)assimpNode;

		MeshNode &node = meshSource->m_Nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = utils::Mat4FromAIMatrix4x4(aNode->mTransformation);

		glm::mat4 transform = parentTransform * node.LocalTransform;
		for (uint32 i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32 submeshIndex = aNode->mMeshes[i];
			auto &submesh = meshSource->m_Submeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		// HL_MESH_LOG("{0} {1}", LevelToSpaces(level), node->mName.C_Str());

		uint32 parentNodeIndex = (uint32)meshSource->m_Nodes.size() - 1;
		node.Children.resize(aNode->mNumChildren);
		for (uint32 i = 0; i < aNode->mNumChildren; i++)
		{
			MeshNode &child = meshSource->m_Nodes.emplace_back();
			uint32 childIndex = (uint32)(meshSource->m_Nodes.size() - 1);
			child.Parent = parentNodeIndex;
			meshSource->m_Nodes[nodeIndex].Children[i] = childIndex;
			TraverseNodes(meshSource, aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}
}

