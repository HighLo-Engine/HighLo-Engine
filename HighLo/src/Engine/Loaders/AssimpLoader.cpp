// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssimpLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/Core.h"

namespace highlo
{
	static const glm::mat4 CORRECTION_MATRIX = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	static bool s_ShouldApplyCorrectionMatrix = false;

	struct ParserMesh
	{
		Ref<Mesh> EngineMesh;
		float xMin, xMax;
		float yMin, yMax;
		float zMin, zMax;
	};

	static ParserMesh ProcessMesh(aiMesh *mesh)
	{
		ParserMesh parser_mesh;

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		float xMin, xMax, yMin, yMax, zMin, zMax;
		xMin = mesh->mVertices[0].x;
		xMax = mesh->mVertices[0].x;
		yMin = mesh->mVertices[0].y;
		yMax = mesh->mVertices[0].y;
		zMin = mesh->mVertices[0].z;
		zMax = mesh->mVertices[0].z;

		for (uint32 i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.UV.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.UV.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;

			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;

			vertex.Binormal.x = mesh->mBitangents[i].x;
			vertex.Binormal.y = mesh->mBitangents[i].y;
			vertex.Binormal.z = mesh->mBitangents[i].z;

			if (s_ShouldApplyCorrectionMatrix)
			{
				// Vertex data correction
				glm::vec4 corrected_position = CORRECTION_MATRIX * glm::vec4(vertex.Position.x, vertex.Position.y, vertex.Position.z, 1.0f);
				vertex.Position = glm::vec3(corrected_position.x, corrected_position.y, corrected_position.z);

				glm::vec4 corrected_normal = CORRECTION_MATRIX * glm::vec4(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z, 1.0f);
				vertex.Normal = glm::vec3(corrected_normal.x, corrected_normal.y, corrected_normal.z);

				glm::vec4 corrected_binormal = CORRECTION_MATRIX * glm::vec4(vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z, 1.0f);
				vertex.Binormal = glm::vec3(corrected_binormal.x, corrected_binormal.y, corrected_binormal.z);
			}

			vertices.push_back(vertex);

			if (vertex.Position.x > xMax) xMax = vertex.Position.x;
			if (vertex.Position.x < xMin) xMin = vertex.Position.x;

			if (vertex.Position.y > yMax) yMax = vertex.Position.y;
			if (vertex.Position.y < yMin) yMin = vertex.Position.y;

			if (vertex.Position.z > zMax) zMax = vertex.Position.z;
			if (vertex.Position.z < zMin) zMin = vertex.Position.z;
		}

		for (uint32 i = 0; i < mesh->mNumFaces; i++)
		{
			auto &face = mesh->mFaces[i];

			for (uint32 j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		MeshData data;
		data.Vertices = vertices;
		data.Indices = indices;

		parser_mesh.EngineMesh = Mesh::Create(data);
		parser_mesh.xMin = xMin;
		parser_mesh.xMax = xMax;
		parser_mesh.yMin = yMin;
		parser_mesh.yMax = yMax;
		parser_mesh.zMin = zMin;
		parser_mesh.zMax = zMax;

		return parser_mesh;
	}

	static void ProcessNode(aiNode *node, const aiScene *scene, std::vector<ParserMesh> &mesh_list)
	{
		for (uint32 i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			ParserMesh parser_mesh = ProcessMesh(mesh);

			mesh_list.push_back(parser_mesh);
		}

		for (uint32 i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene, mesh_list);
	}

	Ref<Model> AssimpLoader::LoadStatic(const HLString &filepath, bool bShouldApplyCorrectionMatrix)
	{
		s_ShouldApplyCorrectionMatrix = bShouldApplyCorrectionMatrix;

		Assimp::Importer importer;
		auto *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!scene)
		{
			HL_CORE_ERROR("AssimpLoader> Failed to load " + filepath);
			HL_CORE_ERROR(importer.GetErrorString());
			return Model::Create();
		}

		std::vector<ParserMesh> parser_meshes;
		std::vector<Ref<Mesh>> engine_meshes;

		ProcessNode(scene->mRootNode, scene, parser_meshes);

		float xMin, xMax, yMin, yMax, zMin, zMax;
		xMin = parser_meshes.at(0).xMin;
		xMax = parser_meshes.at(0).xMax;
		yMin = parser_meshes.at(0).yMin;
		yMax = parser_meshes.at(0).yMax;
		zMin = parser_meshes.at(0).zMin;
		zMax = parser_meshes.at(0).zMax;

		for (auto &mesh : parser_meshes)
		{
			if (mesh.xMax > xMax) xMax = mesh.xMax;
			if (mesh.xMin < xMin) xMin = mesh.xMin;

			if (mesh.yMax > yMax) yMax = mesh.yMax;
			if (mesh.yMin < yMin) yMin = mesh.yMin;

			if (mesh.zMax > zMax) zMax = mesh.zMax;
			if (mesh.zMin < zMin) zMin = mesh.zMin;

			engine_meshes.push_back(mesh.EngineMesh);
		}

		auto model = Model::Create(engine_meshes);
		model->BoundingBox = AABB({ xMax, xMin, yMax }, { yMin, zMax, zMin });

		HL_CORE_INFO("AssimpLoader> [+] Loaded {0} [+]", *filepath);
		return model;
	}

	// ------------------------------------------------------------------------------------------------------- //
	// ------------------------------------------------------------------------------------------------------- //
	// ------------------------------------------------------------------------------------------------------- //
	// ------------------------------------------------------------------------------------------------------- //
	// ------------------------------------------------------------------------------------------------------- //

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		std::vector<BoneTransform> KeyframeTransforms;
	};

	std::map<std::string, uint32> BoneMapping;
	std::vector<BoneInfo> BoneInfoList;

	static std::string CalculateBoneTransforms(Bone &RootBone, const aiScene *scene, uint32 animation_index, glm::mat4 GlobalInverseTransform, float &TicksPerSecond, float &AnimationDuration);
	static Bone ReadNodeHierarchy(aiAnimation *animation, aiNode *node);
	static const aiNodeAnim* FindNodeAnim(const aiAnimation *animation, const std::string &NodeName);

	static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

#pragma warning(push)
#pragma warning(disable: 4267)
	static void AddBoneData(std::vector<Vertex> &vertices, uint32 VertexID, uint32 BoneID, float Weight)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (vertices[VertexID].Weights[i] == 0.0f)
			{
				vertices[VertexID].JointIDs[i] = BoneID;
				vertices[VertexID].Weights[i] = Weight;
				return;
			}
		}

	}
#pragma warning(pop) 

	Ref<Mesh> AssimpLoader::LoadAnimated(const HLString &filepath, bool bShouldApplyCorrectionMatrix)
	{
		s_ShouldApplyCorrectionMatrix = bShouldApplyCorrectionMatrix;
		BoneMapping.clear();
		BoneInfoList.clear();

		Assimp::Importer importer;
		auto *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!scene)
		{
			HL_CORE_ERROR("AssimpLoader> Failed to load animated {0}", filepath);
			HL_CORE_ERROR(importer.GetErrorString());
			return nullptr;
		}

		if (!scene->mNumAnimations)
		{
			HL_CORE_ERROR("AssimpLoader> Failed to load animated {0} [NO ANIMATIONS FOUND]", filepath);
			return nullptr;
		}

		glm::mat4 InverseTransform = glm::inverse(aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));
		aiMesh *mesh = scene->mMeshes[0];

		std::vector<Vertex> vertices;
		std::vector<int32> indices;
		uint32 BoneCount = 0;

		// -----------------------------------------------//
		//				Reading the vertices              //
		// -----------------------------------------------//
		float xMin, xMax, yMin, yMax, zMin, zMax;
		xMin = mesh->mVertices[0].x;
		xMax = mesh->mVertices[0].x;
		yMin = mesh->mVertices[0].y;
		yMax = mesh->mVertices[0].y;
		zMin = mesh->mVertices[0].z;
		zMax = mesh->mVertices[0].z;

		for (uint32 i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x,   mesh->mVertices[i].y,   mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x,    mesh->mNormals[i].y,    mesh->mNormals[i].z };
			vertex.Tangent = { mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z };
			vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };

			if (mesh->HasTextureCoords(0))
				vertex.UV = { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y };

			vertices.push_back(vertex);

			if (vertex.Position.x > xMax) xMax = vertex.Position.x;
			if (vertex.Position.x < xMin) xMin = vertex.Position.x;

			if (vertex.Position.y > yMax) yMax = vertex.Position.y;
			if (vertex.Position.y < yMin) yMin = vertex.Position.y;

			if (vertex.Position.z > zMax) zMax = vertex.Position.z;
			if (vertex.Position.z < zMin) zMin = vertex.Position.z;
		}

		// -----------------------------------------------//
		//				Reading the indices               //
		// -----------------------------------------------//
		for (uint32 i = 0; i < mesh->mNumFaces; i++)
		{
			auto &face = mesh->mFaces[i];

			for (uint32 j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// -----------------------------------------------//
		//				  Reading the nodes               //
		// -----------------------------------------------//
		for (size_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone *bone = mesh->mBones[i];
			std::string BoneName(bone->mName.data);
			uint32 BoneIndex = 0;

			if (BoneMapping.find(BoneName) == BoneMapping.end())
			{
				// Allocate an index for a new bone
				BoneIndex = BoneCount;
				BoneCount++;
				BoneInfo bi;
				BoneInfoList.push_back(bi);
				BoneInfoList[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(bone->mOffsetMatrix);
				BoneMapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = BoneMapping[BoneName];
			}

			for (size_t j = 0; j < bone->mNumWeights; j++)
			{
				int32 VertexID = bone->mWeights[j].mVertexId;
				float Weight = bone->mWeights[j].mWeight;
				AddBoneData(vertices, VertexID, BoneIndex, Weight);
			}
		}

		// -----------------------------------------------//
		//        Calculating Bone Transformations        //
		// -----------------------------------------------//
		float TicksPerSecond = 0;
		float AnimationDuration = 0;

		glm::mat4 animation_correction_matrix = glm::mat4(1.0f);
		if (bShouldApplyCorrectionMatrix)
			animation_correction_matrix = CORRECTION_MATRIX;

		std::vector<Ref<Animation>> animations;
		for (uint32 i = 0; i < scene->mNumAnimations; i++)
		{
			Bone RootBone;
			std::string animation_name = CalculateBoneTransforms(RootBone, scene, i, InverseTransform, TicksPerSecond, AnimationDuration);

			auto animation = Animation::Create(animation_name, AnimationDuration, TicksPerSecond, InverseTransform, BoneCount, RootBone, animation_correction_matrix);
			animations.push_back(animation);
		}

		// -----------------------------------------------//
		//				   Finalizing Data                //
		// -----------------------------------------------//
		MeshData data;
		data.Vertices = vertices;
		data.Indices = indices;

		auto EngineMesh = Mesh::Create(data);
		EngineMesh->SetAnimation(animations[0]);

		/*model.DefaultBoundingBoxData = BoundingBoxData{ xMax, xMin, yMax, yMin, zMax, zMin };*/

		HL_CORE_INFO("AssimpLoader> [+] Loaded Animated {0} [+]", filepath);
		return EngineMesh;
	}

	std::string CalculateBoneTransforms(Bone &RootBone, const aiScene *scene, uint32 animation_index, glm::mat4 GlobalInverseTransform, float &TicksPerSecond, float &AnimationDuration)
	{
		TicksPerSecond = (float)scene->mAnimations[animation_index]->mTicksPerSecond;
		AnimationDuration = (float)scene->mAnimations[animation_index]->mDuration;

		RootBone = ReadNodeHierarchy(scene->mAnimations[animation_index], scene->mRootNode);
		return std::string(scene->mAnimations[animation_index]->mName.C_Str());
	}

	Bone ReadNodeHierarchy(aiAnimation *animation, aiNode *node)
	{
		std::string name(node->mName.data);
		const aiNodeAnim *NodeAnim = FindNodeAnim(animation, name);
		float keyframe_time = 0;

		Bone bone;
		bone.Name = name;

		if (NodeAnim)
		{
			if (BoneMapping.find(name) != BoneMapping.end())
			{
				for (uint32 animation_key = 0; animation_key < NodeAnim->mNumPositionKeys; animation_key++)
				{
					auto t = NodeAnim->mPositionKeys[animation_key].mValue;
					glm::vec3 vec = { t.x, t.y, t.z };

					auto q = NodeAnim->mRotationKeys[animation_key].mValue;
					q = q.Normalize();
					auto quat = glm::quat(q.w, q.x, q.y, q.z);

					uint32 BoneIndex = BoneMapping[name];
					bone.ID = BoneIndex;

					BoneTransform transform;
					transform.Translation = vec;
					transform.Rotation = quat;

					Keyframe keyframe;
					keyframe.Timestamp = (float)NodeAnim->mPositionKeys[animation_key].mTime;
					keyframe.Transform = transform;

					bone.Keyframes.push_back(keyframe);
					bone.OffsetMatrix = BoneInfoList[BoneIndex].BoneOffset;
				}
			}
		}

		for (uint32 i = 0; i < node->mNumChildren; i++)
		{
			if (bone.Keyframes.size() == 0)
				bone = ReadNodeHierarchy(animation, node->mChildren[i]);
			else
			{
				Bone child_bone = ReadNodeHierarchy(animation, node->mChildren[i]);
				if (child_bone.Keyframes.size() != 0)
					bone.Children.push_back(child_bone);
			}
		}

		return bone;
	}

	const aiNodeAnim *FindNodeAnim(const aiAnimation *animation, const std::string &NodeName)
	{
		for (uint32 i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim *nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == NodeName)
				return nodeAnim;
		}
		return nullptr;
	}

}
