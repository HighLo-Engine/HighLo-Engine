#include "HighLoPch.h"
#include "OrthographicCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Core/HLInput.h"
#include "Engine/Core/HLTime.h"
#include "Engine/Math/Transform.h"

namespace highlo
{
	OrthographicCamera::OrthographicCamera(float width, float height)
	{
		float aspectRatio = width / height;
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjectionMatrix();
	}
	
	void OrthographicCamera::Update()
	{
		auto timestep = Time::GetTimestep();

		if (Input::IsKeyPressed(HL_KEY_A))
		{
			m_Position.x -= cos(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
			m_Position.y -= sin(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HL_KEY_D))
		{
			m_Position.x += cos(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
			m_Position.y += sin(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(HL_KEY_W))
		{
			m_Position.x += -sin(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
			m_Position.y +=  cos(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HL_KEY_S))
		{
			m_Position.x -= -sin(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
			m_Position.y -=  cos(glm::radians(m_Rotation)) * m_TranslationSpeed * timestep;
		}

		if (Input::IsKeyPressed(HL_KEY_Q))
		{
			m_Rotation += m_RotationSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HL_KEY_E))
		{
			m_Rotation -= m_RotationSpeed * timestep;
		}

		if (m_Rotation > 180.0f)
		{
			m_Rotation -= 360.0f;
		}
		else if (m_Rotation <= -180.0f)
		{
			m_Rotation += 360.0f;
		}

		m_TranslationSpeed = m_ZoomLevel;
	}
	
	void OrthographicCamera::OnWindowResize(uint32 width, uint32 height)
	{
		float aspectRatio = width / height;
		m_ProjectionMatrix = glm::ortho(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
	}
	
	void OrthographicCamera::OnEvent(Event &e)	
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(HL_BIND_EVENT_FUNCTION(OrthographicCamera::OnWindowResizeInternal));
		dispatcher.Dispatch<MouseScrolledEvent>(HL_BIND_EVENT_FUNCTION(OrthographicCamera::OnMouseScrolledInternal));
	}

	Ref<OrthographicCamera> OrthographicCamera::Create(float width, float height)
	{
		return Ref<OrthographicCamera>::Create(width, height);
	}
	
	void OrthographicCamera::UpdateProjectionMatrix()
	{
		float aspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::ortho(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
	}
	
	void OrthographicCamera::UpdateViewMatrix()
	{
		Transform transform;
		transform.FromPosition(m_Position).Rotate(m_Rotation, { 0, 0, 1 });

		m_ViewMatrix = glm::inverse(transform.GetTransform());
	}

	bool OrthographicCamera::OnWindowResizeInternal(WindowResizeEvent &e)
	{
		OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;
	}
	
	bool OrthographicCamera::OnMouseScrolledInternal(MouseScrolledEvent &e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		UpdateProjectionMatrix();
		return false;
	}
}