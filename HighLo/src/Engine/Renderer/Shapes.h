#pragma once

#include "Mesh.h"

namespace highlo
{
	namespace Shapes
	{
		namespace _3D
		{
			extern MeshData Cube;
			extern MeshData Plane;
			extern MeshData Sphere;
		}

		namespace _2D
		{
			extern MeshData Quad;
			extern MeshData Triangle;
		}

		void InitializeBasicShapes();
	}
}
