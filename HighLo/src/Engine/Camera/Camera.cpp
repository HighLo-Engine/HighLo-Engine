#include "HighLoPch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Application/HLApplication.h"

namespace highlo
{
	Camera::Camera()
	{
		SetProjectionInternal(1280, 720);
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = glm::radians(verticalFOV);
		m_PerspectiveNear = nearClip;
		m_PerspecticeFar = farClip;
		SetProjectionInternal(HLApplication::Get().GetWindow().GetWidth(), HLApplication::Get().GetWindow().GetHeight());
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		SetProjectionInternal(HLApplication::Get().GetWindow().GetWidth(), HLApplication::Get().GetWindow().GetHeight());
	}

	void Camera::SetViewportSize(uint32 width, uint32 height)
	{
		SetProjectionInternal(width, height);
	}

	void Camera::GetDirectionVectors(glm::vec3 &front, glm::vec3 &up, glm::vec3 &left)
	{
		float sx, sy, sz, cx, cy, cz, theta;

		// rotation angle about X-axis (pitch)
		theta = glm::radians(m_Pitch);
		sx = Sin(theta);
		cx = Cos(theta);

		// rotation angle about Y-axis (yaw)
		theta = glm::radians(m_Yaw);
		sy = Sin(theta);
		cy = Cos(theta);

		// rotation angle about Z-axis (roll)
		theta = glm::radians(m_Roll);
		sz = Sin(theta);
		cz = Cos(theta);

		left.x = cy * cz;
		left.y = sx * sy * cz + cx * sz;
		left.z = -cx * sy * cz + sx * sz;

		up.x = -cy * sz;
		up.y = -sx * sy * sz + cx * cz;
		up.z = cx * sy * sz + sx * cz;

		front.x = sy;
		front.y = -sx * cy;
		front.z = cx * cy;
	}

	void Camera::OnWindowResize(uint32 width, uint32 height)
	{
		SetProjectionInternal(width, height);
	}

	void Camera::SetProjectionInternal(uint32 width, uint32 height)
	{
		switch (m_ProjectionType)
		{
			case ProjectionType::Perspective:
			{
				m_ProjectionMatrix = glm::perspectiveFov(m_PerspectiveFOV, (float) width, (float) height, m_PerspectiveNear, m_PerspecticeFar);
				break;
			}

			case ProjectionType::Orthographic:
			{
				float aspect = (float) width / (float) height;
				float w = m_OrthographicSize * aspect;
				float h = m_OrthographicSize;
				m_ProjectionMatrix = glm::ortho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f);
				break;
			}
		}
	}
}