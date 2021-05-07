#include "HighLoPch.h"
#include "FPSCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Window/Window.h"
#include "Engine/Core/HLInput.h"
#include "Engine/Utils/InputUtils.h"

namespace highlo
{
	static glm::vec3 s_FpsCameraWorldUp = glm::vec3(0, 1, 0);

	FPSCamera::FPSCamera(float zNear, float zFar, float fov)
		: m_zNear(zNear), m_zFar(zFar), m_Fov(fov)
	{
		m_WorldUp = glm::vec3(0, 1, 0);
		m_Front = glm::vec3(0, 0, 1);

		m_Yaw += 180.0f;

		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void FPSCamera::Update()
	{
		auto timestep = Time::GetTimestep();

		float frame_velocity = m_Speed * timestep;
		float frame_rotation_velocity = m_RotationSpeed;

		if (Input::IsKeyPressed(HL_KEY_LEFT))
			Rotate(glm::vec3(-frame_rotation_velocity, 0, 0));

		if (Input::IsKeyPressed(HL_KEY_RIGHT))
			Rotate(glm::vec3(frame_rotation_velocity, 0, 0));

		if (Input::IsKeyPressed(HL_KEY_W))
			m_Position += m_Front * frame_velocity;

		if (Input::IsKeyPressed(HL_KEY_S))
			m_Position -= m_Front * frame_velocity;

		if (Input::IsKeyPressed(HL_KEY_A))
			m_Position -= m_Right * frame_velocity;

		if (Input::IsKeyPressed(HL_KEY_D))
			m_Position += m_Right * frame_velocity;

		UpdateViewMatrix();
	}

	void FPSCamera::UpdateProjectionMatrix()
	{
		float aspect_ratio = (float)Window::Get().GetWidth() / (float)Window::Get().GetHeight();
		float y_scale = (float)((1.f / tanf(glm::radians(m_Fov / 2.f))) * aspect_ratio);
		float x_scale = y_scale / aspect_ratio;
		float frustum_length = m_zFar - m_zNear;

		m_ProjectionMatrix = glm::mat4(1.0f);
		m_ProjectionMatrix[0][0] = x_scale;
		m_ProjectionMatrix[1][1] = y_scale;
		m_ProjectionMatrix[2][2] = -((m_zFar + m_zNear) / frustum_length);
		m_ProjectionMatrix[2][3] = -1;
		m_ProjectionMatrix[3][2] = -((2 * m_zNear * m_zFar) / frustum_length);
		m_ProjectionMatrix[3][3] = 0;
	}

	void FPSCamera::UpdateViewMatrix()
	{
		glm::vec3 front;
		front.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		front.y = sinf(glm::radians(m_Pitch));
		front.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(m_Front, s_FpsCameraWorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void FPSCamera::OnWindowResize(uint32 width, uint32 height)
	{
		UpdateProjectionMatrix();
	}
}

