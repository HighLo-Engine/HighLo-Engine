#include "HighLoPch.h"
#include "HLFrustum.h"

namespace highlo
{
	void HLFrustum::Init(const glm::mat4& mat)
	{
		Planes[HL_FRUSTUM_RIGHT].Normal.x = mat[0][3] - mat[0][0];
		Planes[HL_FRUSTUM_RIGHT].Normal.y = mat[1][3] - mat[1][0];
		Planes[HL_FRUSTUM_RIGHT].Normal.z = mat[2][3] - mat[2][0];
		Planes[HL_FRUSTUM_RIGHT].Distance = mat[3][3] - mat[3][0];

		Planes[HL_FRUSTUM_LEFT].Normal.x = mat[0][3] + mat[0][0];
		Planes[HL_FRUSTUM_LEFT].Normal.y = mat[1][3] + mat[1][0];
		Planes[HL_FRUSTUM_LEFT].Normal.z = mat[2][3] + mat[2][0];
		Planes[HL_FRUSTUM_LEFT].Distance = mat[3][3] + mat[3][0];

		Planes[HL_FRUSTUM_DOWN].Normal.x = mat[0][3] + mat[0][1];
		Planes[HL_FRUSTUM_DOWN].Normal.y = mat[1][3] + mat[1][1];
		Planes[HL_FRUSTUM_DOWN].Normal.z = mat[2][3] + mat[2][1];
		Planes[HL_FRUSTUM_DOWN].Distance = mat[3][3] + mat[3][1];

		Planes[HL_FRUSTUM_UP].Normal.x = mat[0][3] - mat[0][1];
		Planes[HL_FRUSTUM_UP].Normal.y = mat[1][3] - mat[1][1];
		Planes[HL_FRUSTUM_UP].Normal.z = mat[2][3] - mat[2][1];
		Planes[HL_FRUSTUM_UP].Distance = mat[3][3] - mat[3][1];

		Planes[HL_FRUSTUM_FAR].Normal.x = mat[0][3] - mat[0][2];
		Planes[HL_FRUSTUM_FAR].Normal.y = mat[1][3] - mat[1][2];
		Planes[HL_FRUSTUM_FAR].Normal.z = mat[2][3] - mat[2][2];
		Planes[HL_FRUSTUM_FAR].Distance = mat[3][3] - mat[3][2];

		Planes[HL_FRUSTUM_NEAR].Normal.x = mat[0][3] + mat[0][2];
		Planes[HL_FRUSTUM_NEAR].Normal.y = mat[1][3] + mat[1][2];
		Planes[HL_FRUSTUM_NEAR].Normal.z = mat[2][3] + mat[2][2];
		Planes[HL_FRUSTUM_NEAR].Distance = mat[3][3] + mat[3][2];

		// Normalize all plane normals
		for (int i = 0; i < 6; i++)
			Planes[i].Normalize();
	}
	
	bool HLFrustum::SphereIntersection(const glm::vec3& center, float radius)
	{
		for (int i = 0; i < 6; i++)
		{
			if (glm::dot(center, Planes[i].Normal) + Planes[i].Distance + radius <= 0)
				return false;
		}

		return true;
	}
	
	int HLFrustum::AABBIntersection(const AABB& aabb)
	{
		int ret = HL_INSIDE;
		glm::vec3 vmin, vmax;

		for (int i = 0; i < 6; ++i)
		{
			// X axis 
			if (Planes[i].Normal.x > 0) {
				vmin.x = aabb.Min.x;
				vmax.x = aabb.Max.x;
			}
			else
			{
				vmin.x = aabb.Max.x;
				vmax.x = aabb.Min.x;
			}

			// Y axis 
			if (Planes[i].Normal.y > 0) {
				vmin.y = aabb.Min.y;
				vmax.y = aabb.Max.y;
			}
			else
			{
				vmin.y = aabb.Max.y;
				vmax.y = aabb.Min.y;
			}
			
			// Z axis 
			if (Planes[i].Normal.z > 0) {
				vmin.z = aabb.Min.z;
				vmax.z = aabb.Max.z;
			}
			else
			{
				vmin.z = aabb.Max.z;
				vmax.z = aabb.Min.z;
			}

			if (glm::dot(Planes[i].Normal, vmin) + Planes[i].Distance > 0)
				return HL_OUTSIDE;

			if (glm::dot(Planes[i].Normal, vmax) + Planes[i].Distance >= 0)
				ret = HL_INTERSECTING;
		}

		return ret;
	}
}