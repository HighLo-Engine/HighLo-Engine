#include "HighLoPch.h"
#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/HLCore.h"

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

	static ParserMesh ProcessMesh(aiMesh* mesh)
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

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
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

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			auto& face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		MeshData data;
		data.m_Vertices = vertices;
		data.m_Indices = indices;

		parser_mesh.EngineMesh = Mesh::Create(data);
		parser_mesh.xMin = xMin;
		parser_mesh.xMax = xMax;
		parser_mesh.yMin = yMin;
		parser_mesh.yMax = yMax;
		parser_mesh.zMin = zMin;
		parser_mesh.zMax = zMax;

		return parser_mesh;
	}

	static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<ParserMesh>& mesh_list)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ParserMesh parser_mesh = ProcessMesh(mesh);

			mesh_list.push_back(parser_mesh);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene, mesh_list);
	}

	Model AssimpLoader::LoadStatic(const HLString& filepath, bool bShouldApplyCorrectionMatrix)
	{
		s_ShouldApplyCorrectionMatrix = bShouldApplyCorrectionMatrix;

		Assimp::Importer importer;
		auto* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!scene)
		{
			HL_CORE_ERROR("AssimpLoader> Failed to load " + filepath);
			HL_CORE_ERROR(importer.GetErrorString());
			return Model();
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

		for (auto& mesh : parser_meshes)
		{
			if (mesh.xMax > xMax) xMax = mesh.xMax;
			if (mesh.xMin < xMin) xMin = mesh.xMin;

			if (mesh.yMax > yMax) yMax = mesh.yMax;
			if (mesh.yMin < yMin) yMin = mesh.yMin;

			if (mesh.zMax > zMax) zMax = mesh.zMax;
			if (mesh.zMin < zMin) zMin = mesh.zMin;

			engine_meshes.push_back(mesh.EngineMesh);
		}

		auto model = Model(engine_meshes);
		//model.DefaultBoundingBoxData = BoundingBoxData{ xMax, xMin, yMax, yMin, zMax, zMin };

		HL_CORE_INFO("AssimpLoader> [+] Loaded {0} [+]", filepath.C_Str());
		return model;
	}

}
