#include "HighLoPch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Window/Window.h"

namespace highlo
{
	Camera::Camera()
	{
		SetProjectionInternal(Window::Get().GetWidth(), Window::Get().GetHeight());
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = glm::radians(verticalFOV);
		m_PerspectiveNear = nearClip;
		m_PerspecticeFar = farClip;
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
	}

	void Camera::SetViewportSize(uint32 width, uint32 height)
	{
		SetProjectionInternal(width, height);
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