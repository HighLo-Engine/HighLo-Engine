#pragma once
#include "ICamera.h"

namespace highlo
{
	class FPSCamera : public ICamera
	{
	public:
		HLAPI FPSCamera(float zNear = 0.1f, float zFar = 2000.0f, float fov = 90.0f);

		// Inherited via Camera
		HLAPI virtual void Update() override;
		HLAPI virtual void OnWindowResize(unsigned int width, unsigned int height) override;

	private:
		float m_Speed = 0.01f;
		float m_RotationSpeed = 0.48f;
		float m_zNear, m_zFar, m_Fov;

		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		// Inherited via Camera
		virtual void UpdateProjectionMatrix() override;
		virtual void UpdateViewMatrix() override;
		virtual void OnEvent(Event& e) override {}
	};
}