#include "HighLoPch.h"
#include "Shapes.h"

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
	}
}
