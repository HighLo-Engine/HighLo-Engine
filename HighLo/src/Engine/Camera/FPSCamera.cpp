// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FPSCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Engine/Window/Window.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"

namespace highlo
{
	FPSCamera::FPSCamera()
	{
		m_ViewportWidth = Window::Get().GetWidth();
		m_ViewportHeight = Window::Get().GetHeight();
		UpdateCameraView();

		m_Yaw = 3.0f * HL_PI / 4.0f;
		m_Pitch = HL_PI / 4.0f;

		UpdateCameraView();
	}

	FPSCamera::FPSCamera(const glm::mat4 &projectionMatrix)
	{
		m_ViewportWidth = Window::Get().GetWidth();
		m_ViewportHeight = Window::Get().GetHeight();
		m_ProjectionMatrix = projectionMatrix;

		m_Yaw = 3.0f * HL_PI / 4.0f;
		m_Pitch = HL_PI / 4.0f;

		UpdateCameraView();
	}
	
	void FPSCamera::Update(Timestep ts)
	{
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_W))
		{
			m_Position += GetForwardDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_W))
		{
			m_Position += GetForwardDirection() * MovementSpeed() * ts;
		}
		
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_S))
		{
			m_Position -= GetForwardDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_S))
		{
			m_Position -= GetForwardDirection() * MovementSpeed() * ts;
		}

		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_D))
		{
			m_Position += GetRightDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_D))
		{
			m_Position += GetRightDirection() * MovementSpeed() * ts;
		}

		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_A))
		{
			m_Position -= GetRightDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_A))
		{
			m_Position -= GetRightDirection() * MovementSpeed() * ts;
		}

		if (Input::IsKeyPressed(HL_KEY_LEFT))
		{
			m_Yaw += -(RotationSpeed() * ts);
		}
		else if (Input::IsKeyPressed(HL_KEY_RIGHT))
		{
			m_Yaw += RotationSpeed() * ts;
		}

		UpdateCameraView();
	}
	
	void FPSCamera::OnWindowResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateCameraView();
	}
	
	void FPSCamera::SetViewportSize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateCameraView();
	}
	
	glm::vec3 FPSCamera::GetUpDirection()
	{
		return glm::normalize(glm::cross(GetRightDirection(), GetForwardDirection()));
	}
	
	glm::vec3 FPSCamera::GetRightDirection()
	{
		return glm::normalize(glm::cross(GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	
	glm::vec3 FPSCamera::GetForwardDirection()
	{
		glm::vec3 front;
		front.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		front.y = sinf(glm::radians(m_Pitch));
		front.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		return glm::normalize(front);
	}
	
	void FPSCamera::UpdateCameraView()
	{
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians(90.0f), (float) m_ViewportWidth, (float) m_ViewportHeight, 0.001f, 10000.0f);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetForwardDirection(), GetUpDirection());
	}
	
	float FPSCamera::MovementSpeed() const
	{
		return 0.01f;
	}
	
	float FPSCamera::RotationSpeed() const
	{
		return 0.08f;
	}

	float FPSCamera::RunningSpeed() const
	{
		return 0.025f;
	}
}
