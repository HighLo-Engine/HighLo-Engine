// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FreeFlyCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Engine/Window/Window.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"

namespace highlo
{
	FreeFlyCamera::FreeFlyCamera()
	{
		m_ViewportWidth = Window::Get().GetWidth();
		m_ViewportHeight = Window::Get().GetHeight();
		UpdateCameraView();

		m_Yaw = 3.0f * HL_PI / 4.0f;
		m_Pitch = HL_PI / 4.0f;

		UpdateCameraView();
	}

	FreeFlyCamera::FreeFlyCamera(const glm::mat4 &projectionMatrix)
	{
		m_ViewportWidth = Window::Get().GetWidth();
		m_ViewportHeight = Window::Get().GetHeight();
		m_ProjectionMatrix = projectionMatrix;

		m_Yaw = 3.0f * HL_PI / 4.0f;
		m_Pitch = HL_PI / 4.0f;

		UpdateCameraView();
	}
	
	FreeFlyCamera::~FreeFlyCamera() {}
	
	void FreeFlyCamera::Update()
	{
		Timestep ts = Time::GetTimestep();

		// Move up
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_SPACE))
		{
			m_Position += GetUpDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_SPACE))
		{
			m_Position += GetUpDirection() * MovementSpeed() * ts;
		}

		// Move down
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
		{
			m_Position -= GetUpDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_LEFT_CONTROL))
		{
			m_Position -= GetUpDirection() * MovementSpeed() * ts;
		}

		// Move front
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_W))
		{
			m_Position += GetForwardDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_W))
		{
			m_Position += GetForwardDirection() * MovementSpeed() * ts;
		}

		// Move back
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_S))
		{
			m_Position -= GetForwardDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_S))
		{
			m_Position -= GetForwardDirection() * MovementSpeed() * ts;
		}

		// Move right
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_D))
		{
			m_Position += GetRightDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_D))
		{
			m_Position += GetRightDirection() * MovementSpeed() * ts;
		}

		// Move left
		if (Input::IsKeyPressed(HL_KEY_LEFT_SHIFT) && Input::IsKeyPressed(HL_KEY_A))
		{
			m_Position -= GetRightDirection() * RunningSpeed() * ts;
		}
		else if (Input::IsKeyPressed(HL_KEY_A))
		{
			m_Position -= GetRightDirection() * MovementSpeed() * ts;
		}

		// Rotate left
		if (Input::IsKeyPressed(HL_KEY_LEFT))
		{
			m_Yaw += -(RotationSpeed() * ts);
		}
		// Rotate right
		else if (Input::IsKeyPressed(HL_KEY_RIGHT))
		{
			m_Yaw += RotationSpeed() * ts;
		}

		// Rotate down
		if (Input::IsKeyPressed(HL_KEY_DOWN))
		{
			m_Pitch += -(RotationSpeed() * ts);
		}
		// Rotate up
		else if (Input::IsKeyPressed(HL_KEY_UP))
		{
			m_Pitch += RotationSpeed() * ts;
		}

		UpdateCameraView();
	}
	
	void FreeFlyCamera::OnWindowResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateCameraView();
	}
	
	void FreeFlyCamera::SetViewportSize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateCameraView();
	}
	
	glm::vec3 FreeFlyCamera::GetUpDirection()
	{
		return glm::normalize(glm::cross(GetRightDirection(), GetForwardDirection()));
	}
	
	glm::vec3 FreeFlyCamera::GetRightDirection()
	{
		return glm::normalize(glm::cross(GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	
	glm::vec3 FreeFlyCamera::GetForwardDirection()
	{
		glm::vec3 front;
		front.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		front.y = sinf(glm::radians(m_Pitch));
		front.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
		return glm::normalize(front);
	}
	
	void FreeFlyCamera::UpdateCameraView()
	{
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians(90.0f), (float) m_ViewportWidth, (float) m_ViewportHeight, 0.001f, 10000.0f);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetForwardDirection(), GetUpDirection());
	}
	
	float FreeFlyCamera::MovementSpeed() const
	{
		return 0.01f;
	}
	
	float FreeFlyCamera::RotationSpeed() const
	{
		return 0.16f;
	}
	
	float FreeFlyCamera::RunningSpeed() const
	{
		return 0.025f;
	}
}
