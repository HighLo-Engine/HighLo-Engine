// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MeshFactory.h"

#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	AssetHandle MeshFactory::CreateCube(const glm::vec3 &size)
	{
		std::vector<Vertex> vertices;
		vertices.resize(8);

		std::vector<VertexIndex> indices;
		indices.resize(12);

		// Generate Vertices
		vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
		vertices[1].Position = { size.x  / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
		vertices[2].Position = { size.x  / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
		vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
		vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
		vertices[5].Position = { size.x  / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
		vertices[6].Position = { size.x  / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
		vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

		// Generate normals
		vertices[0].Normal = { -1.0f, -1.0f,  1.0f };
		vertices[1].Normal = {  1.0f, -1.0f,  1.0f };
		vertices[2].Normal = {  1.0f,  1.0f,  1.0f };
		vertices[3].Normal = { -1.0f,  1.0f,  1.0f };
		vertices[4].Normal = { -1.0f, -1.0f, -1.0f };
		vertices[5].Normal = {  1.0f, -1.0f, -1.0f };
		vertices[6].Normal = {  1.0f,  1.0f, -1.0f };
		vertices[7].Normal = { -1.0f,  1.0f, -1.0f };

		// Generate indices
		indices[ 0] = { 0, 1, 2 };
		indices[ 1] = { 2, 3, 0 };
		indices[ 2] = { 1, 5, 6 };
		indices[ 3] = { 6, 2, 1 };
		indices[ 4] = { 7, 6, 5 };
		indices[ 5] = { 5, 4, 7 };
		indices[ 6] = { 4, 0, 3 };
		indices[ 7] = { 3, 7, 4 };
		indices[ 8] = { 4, 5, 1 };
		indices[ 9] = { 1, 0, 4 };
		indices[10] = { 3, 2, 6 };
		indices[11] = { 6, 7, 3 };

		AABB boundingBox = AABB(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec3(size.x / 2, size.y / 2, size.z / 2));

		AssetHandle handle = AssetManager::Get()->CreateMemoryOnlyAsset<MeshFile>(vertices, indices, boundingBox);
		Ref<MeshFile> result = AssetManager::Get()->GetAsset<MeshFile>(handle);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(result);
	}

	AssetHandle MeshFactory::CreateSphere(float radius)
	{
		const float PI = HL_PI;
		float sectorCount = 36;
		float stackCount = 18;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2.0f * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		std::vector<Vertex> vertices;
		std::vector<VertexIndex> indices;

		for (int32 i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2.0f - i * stackStep;     // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int32 j = 0; j <= sectorCount; ++j)
			{
				Vertex vertex;

				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertex.Position.x = x;
				vertex.Position.y = y;
				vertex.Position.z = z;

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertex.Normal.x = nx;
				vertex.Normal.y = ny;
				vertex.Normal.z = nz;

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				vertex.TexCoord.x = s;
				vertex.TexCoord.y = t;

				vertices.push_back(vertex);
			}
		}

		int32 k1, k2;
		for (int32 i = 0; i < stackCount; ++i)
		{
			k1 = i * ((int32)sectorCount + 1);     // beginning of current stack
			k2 = k1 + (int32)sectorCount + 1;      // beginning of next stack

			for (int32 j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1

				if (i != 0)
				{
					VertexIndex index;
					index.P1 = k1;
					index.P2 = k2;
					index.P3 = k1 + 1;
					indices.push_back(index);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					VertexIndex index;
					index.P1 = k1 + 1;
					index.P2 = k2;
					index.P3 = k2 + 1;
					indices.push_back(index);
				}
			}
		}

		AssetHandle handle = AssetManager::Get()->CreateMemoryOnlyAsset<MeshFile>(vertices, indices, glm::mat4(1.0f));
		Ref<MeshFile> result = AssetManager::Get()->GetAsset<MeshFile>(handle);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(result);
	}

	AssetHandle MeshFactory::CreateCapsule(float radius, float height)
	{
		std::vector<Vertex> vertices;
		std::vector<VertexIndex> indices;

		constexpr int32 segments = 36;
		constexpr int32 pointCount = segments + 1;

		float pointsX[pointCount];
		float pointsY[pointCount];
		float pointsZ[pointCount];
		float pointsR[pointCount];

		float calcH = 0.0f;
		float calcV = 0.0f;

		for (int32 i = 0; i < pointCount; i++)
		{
			float calcHRadians = glm::radians(calcH);
			float calcVRadians = glm::radians(calcV);

			pointsX[i] = glm::sin(calcHRadians);
			pointsZ[i] = glm::cos(calcHRadians);
			pointsY[i] = glm::cos(calcVRadians);
			pointsR[i] = glm::sin(calcVRadians);

			calcH += 360.0f / (float) segments;
			calcV += 180.0f / (float) segments;
		}

		float yOffset = (height - (radius * 2.0f)) * 0.5f;
		if (yOffset < 0.0f)
			yOffset = 0.0f;

		// Generate top vertices
		int32 top = (int32)glm::ceil(pointCount * 0.5f);
		for (int32 y = 0; y < top; y++)
		{
			for (int32 x = 0; x < pointCount; x++)
			{
				Vertex vertex;
				vertex.Position = glm::vec3(pointsX[x] * pointsR[y], pointsY[y] + yOffset, pointsZ[x] * pointsR[y]) * radius;
				vertices.push_back(vertex);
			}
		}

		// Generate bottom vertices
		int32 bottom = (int32)glm::floor(pointCount * 0.5f);
		for (int32 y = bottom; y < pointCount; y++)
		{
			for (int32 x = 0; x < pointCount; x++)
			{
				Vertex vertex;
				vertex.Position = glm::vec3(pointsX[x] * pointsR[y], -yOffset + pointsY[y], pointsZ[x] * pointsR[y]) * radius;
				vertices.push_back(vertex);
			}
		}

		// Generate indices
		for (int32 r = 0; r < segments + 1; r++)
		{
			for (int32 s = 0; s < segments; s++)
			{
				VertexIndex &index1 = indices.emplace_back();
				index1.P1 = (uint32) (r * segments + s + 1);
				index1.P2 = (uint32) (r * segments + s + 0);
				index1.P3 = (uint32) ((r + 1) * segments + s + 1);

				VertexIndex &index2 = indices.emplace_back();
				index2.P1 = (uint32) ((r + 1) * segments + s + 0);
				index2.P2 = (uint32) ((r + 1) * segments + s + 1);
				index2.P3 = (uint32) (r * segments + s);
			}
		}

		AssetHandle handle = AssetManager::Get()->CreateMemoryOnlyAsset<MeshFile>(vertices, indices, glm::mat4(1.0f));
		Ref<MeshFile> result = AssetManager::Get()->GetAsset<MeshFile>(handle);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(result);
	}
}

