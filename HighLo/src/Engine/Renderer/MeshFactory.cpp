#include "HighLoPch.h"
#include "MeshFactory.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace highlo
{
	Ref<Mesh> MeshFactory::CreateCube(const glm::vec3 &size)
	{
		std::vector<Vertex> vertices;
		vertices.resize(8);

		std::vector<int32> indices;
		indices.resize(36);

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
		indices[ 0] = 0;
		indices[ 1] = 1;
		indices[ 2] = 2;
		indices[ 3] = 2;
		indices[ 4] = 3;
		indices[ 5] = 0;

		indices[ 6] = 1;
		indices[ 7] = 5;
		indices[ 8] = 6;
		indices[ 9] = 6;
		indices[10] = 2;
		indices[11] = 1;

		indices[12] = 7;
		indices[13] = 6;
		indices[14] = 5;
		indices[15] = 5;
		indices[16] = 4;
		indices[17] = 7;

		indices[18] = 4;
		indices[19] = 0;
		indices[20] = 3;
		indices[21] = 3;
		indices[22] = 7;
		indices[23] = 4;

		indices[24] = 4;
		indices[25] = 5;
		indices[26] = 1;
		indices[27] = 1;
		indices[28] = 0;
		indices[29] = 4;

		indices[30] = 3;
		indices[31] = 2;
		indices[32] = 6;
		indices[33] = 6;
		indices[34] = 7;
		indices[35] = 3;

		MeshData data;
		data.m_Vertices = vertices;
		data.m_Indices = indices;

		return Mesh::Create(data);
	}

	Ref<Mesh> MeshFactory::CreateSphere(float radius)
	{
		constexpr float latitudeBands = 30;
		constexpr float longitudeBands = 30;

		std::vector<Vertex> vertices;
		std::vector<int32> indices;

		// Generate vertices
		for (float latitude = 0.0f; latitude <= latitudeBands; ++latitude)
		{
			float theta = latitude * (float)M_PI / latitudeBands;
			float sinTheta = glm::sin(theta);
			float cosTheta = glm::cos(theta);

			for (float longitude = 0.0f; longitude <= longitudeBands; ++longitude)
			{
				float phi = longitude * 2.0f * (float)M_PI / longitudeBands;
				float sinPhi = glm::sin(phi);
				float cosPhi = glm::cos(phi);

				Vertex vertex;
				vertex.Normal = { cosPhi * sinTheta, cosTheta, sinPhi * sinTheta };
				vertex.Position = { radius * vertex.Normal.x, radius * vertex.Normal.y, radius * vertex.Normal.z };
				vertices.push_back(vertex);
			}
		}

		// Generate indices
		for (uint32 latitude = 0; latitude <= latitudeBands; ++latitude)
		{
			for (uint32 longitude = 0; longitude <= longitudeBands; ++longitude)
			{
				uint32 first = (uint32)((latitude * (longitudeBands + 1)) + longitude);
				uint32 second = (uint32)(first + longitudeBands + 1);

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);
				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		MeshData data;
		data.m_Vertices = vertices;
		data.m_Indices = indices;

		return Mesh::Create(data);
	}

	Ref<Mesh> MeshFactory::CreateCapsule(float radius, float height)
	{
		std::vector<Vertex> vertices;
		std::vector<int32> indices;

		constexpr int32 segments = 36;
		constexpr int32 pointCount = segments + 1;

		float pointsX[pointCount];
		float pointsY[pointCount];
		float pointsZ[pointCount];
		float pointsR[pointCount];

		float calcH = 0.0f;
		float calcV = 0.0f;

		for (int i = 0; i < pointCount; i++)
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
		for (int32 y = 0; y < segments + 1; y++)
		{
			for (int32 x = 0; x < segments; x++)
			{
				indices.push_back(((y + 0) * (segments + 1)) + x + 0);
				indices.push_back(((y + 1) * (segments + 1)) + x + 0);
				indices.push_back(((y + 1) * (segments + 1)) + x + 1);

				indices.push_back(((y + 0) * (segments + 1)) + x + 1);
				indices.push_back(((y + 0) * (segments + 1)) + x + 0);
				indices.push_back(((y + 1) * (segments + 1)) + x + 1);
			}
		}

		MeshData data;
		data.m_Vertices = vertices;
		data.m_Indices = indices;

		return Mesh::Create(data);
	}
}