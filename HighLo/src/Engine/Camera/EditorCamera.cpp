// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "EditorCamera.h"

#include <glm/gtx/quaternion.hpp>
#include "Engine/Core/Input.h"

namespace highlo
{
	EditorCamera::EditorCamera(const glm::mat4 &projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
		m_Rotation = glm::vec3(90.0f, 0.0f, 0.0f);
		m_FocalPoint = glm::vec3(0.0f);

		glm::vec3 position = { -5.0f, 5.0f, 5.0f };
		m_Distance = glm::distance(position, m_FocalPoint);

		m_Yaw = 3.0f * HL_PI / 4.0f;
		m_Pitch = HL_PI / 4.0f;

		UpdateCameraView();
	}

	void EditorCamera::Focus(const glm::vec3 &focusPoint)
	{
		m_FocalPoint = focusPoint;
		if (m_Distance > m_MinFocusDistance)
		{
			float distance = m_Distance - m_MinFocusDistance;
			MouseZoom(distance / ZoomSpeed());
			UpdateCameraView();
		}
	}

	void EditorCamera::Update()
	{
		const glm::vec2 &mouse = { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (m_Active)
		{
			if (Input::IsKeyPressed(HL_KEY_LEFT_ALT))
			{
				if (Input::IsMouseButtonPressed(HL_MOUSE_BUTTON_LEFT))
					MouseRotate(delta);
				else if (Input::IsMouseButtonPressed(HL_MOUSE_BUTTON_RIGHT))
					MouseZoom(delta.y);
				else if (Input::IsMouseButtonPressed(HL_MOUSE_BUTTON_MIDDLE))
					MousePan(delta);
			}
		}

		UpdateCameraView();
	}

	void EditorCamera::OnWindowResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	void EditorCamera::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HL_BIND_EVENT_FUNCTION(EditorCamera::OnMouseScrolled));
	}

	void EditorCamera::SetProjection(const glm::mat4 &projection)
	{
		m_ProjectionMatrix = projection;
	}

	glm::vec3 EditorCamera::GetUpDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::UpdateCameraView()
	{
		m_Position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		m_Rotation = glm::eulerAngles(orientation) * (180.0f / HL_PI);
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent &e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateCameraView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2 &delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2 &delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		float speed = RotationSpeed();
		m_Yaw += yawSign * delta.x * speed;
		m_Pitch += delta.y * speed;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition()
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}

