#include "HighLoPch.h"
#include "MeshFactory.h"

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
		data.Vertices = vertices;
		data.Indices = indices;

		return Mesh::Create(data);
	}

	Model MeshFactory::CreateCubeModel(const glm::vec3 &size)
	{
		Model model = Model({ CreateCube(size) });
		model.BoundingBox = AABB(glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2), glm::vec3(size.x / 2, size.y / 2, size.z / 2));
		return model;
	}

	Ref<Mesh> MeshFactory::CreateSphere(float radius)
	{
		const float PI = HL_PI;
		float sectorCount = 36;
		float stackCount = 18;
		MeshData data;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2.0f * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

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
				vertex.UV.x = s;
				vertex.UV.y = t;

				data.Vertices.push_back(vertex);
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
					data.Indices.push_back(k1);
					data.Indices.push_back(k2);
					data.Indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					data.Indices.push_back(k1 + 1);
					data.Indices.push_back(k2);
					data.Indices.push_back(k2 + 1);
				}
			}
		}

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
		data.Vertices = vertices;
		data.Indices = indices;

		return Mesh::Create(data);
	}
}