#pragma once

#include "Camera.h"

namespace highlo
{
	class FreeFlyCamera : public Camera
	{
	public:

		HLAPI FreeFlyCamera();
		HLAPI FreeFlyCamera(const glm::mat4 &projectionMatrix);
		HLAPI virtual ~FreeFlyCamera();

		HLAPI void Update() override;
		HLAPI void OnEvent(Event &e) override {}
		HLAPI void OnWindowResize(uint32 width, uint32 height) override;
		HLAPI void SetViewportSize(uint32 width, uint32 height);

		HLAPI glm::vec3 GetUpDirection();
		HLAPI glm::vec3 GetRightDirection();
		HLAPI glm::vec3 GetForwardDirection();

	private:

		void UpdateCameraView();
		float MovementSpeed() const;
		float RotationSpeed() const;
		float RunningSpeed() const;

	private:

		uint32 m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}