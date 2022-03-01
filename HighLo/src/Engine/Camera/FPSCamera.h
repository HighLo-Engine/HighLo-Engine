// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"

namespace highlo
{
	class FPSCamera : public Camera
	{
	public:

		HLAPI FPSCamera();
		HLAPI FPSCamera(const glm::mat4 &projectionMatrix);

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
