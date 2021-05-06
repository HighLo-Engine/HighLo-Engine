#include "HighLoPch.h"
#include "Shapes.h"

#include <vector>

namespace highlo
{
	namespace Shapes
	{
		namespace _3D
		{
			MeshData Cube = MeshData
			{
				{
					Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
					Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
					Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1, 1), glm::vec3(0, 0, -1)),
					Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1, 0), glm::vec3(0, 0, -1)),

					Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
					Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
					Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1, 1), glm::vec3(0, 0, 1)),
					Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(0, 0, 1)),

					Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
					Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(1, 0, 0)),

					Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
					Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),
					Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
					Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),

					Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(0, 1, 0)),

					Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0, 0), glm::vec3(0, -1, 0)),
					Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
				},

				{
					3, 1, 0,
					2, 1, 3,
					7, 5, 4,
					6, 5, 7,
					11, 9, 8,
					10, 9, 11,
					15, 13, 12,
					14, 13, 15,
					19, 17, 16,
					18, 17, 19,
					23, 21, 20,
					22, 21, 23
				}
			};

			MeshData Plane = MeshData
			{
				{
					Vertex(glm::vec3(-0.5f, 0, 0.5f), glm::vec2(0, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(-0.5f, 0, -0.5f), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, 0, 0.5f), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, 0, -0.5f), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
				},
				{
					0, 1, 2,
					2, 1, 3
				}
			};

			MeshData Sphere = MeshData{ {}, {} };
		}

		namespace _2D
		{
			MeshData Quad = MeshData
			{
				{
					Vertex(glm::vec3(-0.5f, 0.5f, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(-0.5f, -0.5f, 0), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, 0.5f, 0), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, 0), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
				},
				{
					0, 1, 2,
					2, 1, 3
				}
			};

			MeshData Triangle = MeshData
			{
				{
					Vertex(glm::vec3(-0.5f, -0.5f, 0),	glm::vec2(0, 1), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0, 0.5f, 0),		glm::vec2(1, 0), glm::vec3(0, 1, 0)),
					Vertex(glm::vec3(0.5f, -0.5f, 0),	glm::vec2(1, 1), glm::vec3(0, 1, 0)),
				},
				{
					0, 1, 2
				}
			};
		}

		MeshData CreateSphereMeshData()
		{
			const float PI = 3.14159265359f;
			float sectorCount = 36;
			float stackCount = 18;
			float radius = 1.0f;
			MeshData data;
			
			float x, y, z, xy;                              // vertex position
			float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
			float s, t;                                     // vertex texCoord


			float sectorStep = 2 * PI / sectorCount;
			float stackStep = PI / stackCount;
			float sectorAngle, stackAngle;

			for (int i = 0; i <= stackCount; ++i)
			{
				stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
				xy = radius * cosf(stackAngle);             // r * cos(u)
				z = radius * sinf(stackAngle);              // r * sin(u)

				// add (sectorCount+1) vertices per stack
				// the first and last vertices have same position and normal, but different tex coords
				for (int j = 0; j <= sectorCount; ++j)
				{
					Vertex vertex;

					sectorAngle = j * sectorStep;           // starting from 0 to 2pi

					// vertex position (x, y, z)
					x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
					y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
					vertex.position.x = x;
					vertex.position.y = y;
					vertex.position.z = z;

					// normalized vertex normal (nx, ny, nz)
					nx = x * lengthInv;
					ny = y * lengthInv;
					nz = z * lengthInv;
					vertex.normal.x = nx;
					vertex.normal.y = ny;
					vertex.normal.z = nz;

					// vertex tex coord (s, t) range between [0, 1]
					s = (float)j / sectorCount;
					t = (float)i / stackCount;
					vertex.uv.x = s;
					vertex.uv.y = t;

					data.m_Vertices.push_back(vertex);
				}
			}

			int k1, k2;
			for (int i = 0; i < stackCount; ++i)
			{
				k1 = i * ((int)sectorCount + 1);     // beginning of current stack
				k2 = k1 + (int)sectorCount + 1;      // beginning of next stack

				for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
				{
					// 2 triangles per sector excluding first and last stacks
					// k1 => k2 => k1+1
					if (i != 0)
					{
						data.m_Indices.push_back(k1);
						data.m_Indices.push_back(k2);
						data.m_Indices.push_back(k1 + 1);
					}

					// k1+1 => k2 => k2+1
					if (i != (stackCount - 1))
					{
						data.m_Indices.push_back(k1 + 1);
						data.m_Indices.push_back(k2);
						data.m_Indices.push_back(k2 + 1);
					}
				}
			}

			return data;
		}

		void InitializeBasicShapes()
		{
			_3D::Sphere = CreateSphereMeshData();
		}
	}
}
