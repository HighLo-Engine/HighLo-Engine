// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Camera.h"

namespace highlo
{
	class EditorCamera : public Camera
	{
	public:

		HLAPI EditorCamera() = default;
		HLAPI EditorCamera(const glm::mat4 &projectionMatrix);

		HLAPI void SetActive(bool active = true) { m_Active = active; }
		HLAPI bool GetActive() const { return m_Active; }

		HLAPI void Focus(const glm::vec3 &focusPoint);
		HLAPI virtual void Update(Timestep ts) override;
		HLAPI virtual void OnWindowResize(uint32 width, uint32 height) override;
		HLAPI virtual void OnEvent(Event &e) override;

		HLAPI inline float GetDistance() const { return m_Distance; }
		HLAPI inline void SetDistance(float distance) { m_Distance = distance; }
		HLAPI void SetProjection(const glm::mat4 &projection);

		HLAPI glm::vec3 GetUpDirection();
		HLAPI glm::vec3 GetRightDirection();
		HLAPI glm::vec3 GetForwardDirection();
		HLAPI glm::quat GetOrientation() const;

	private:

		void UpdateCameraView();
		bool OnMouseScrolled(MouseScrolledEvent &e);

		void MousePan(const glm::vec2 &delta);
		void MouseRotate(const glm::vec2 &delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition();

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:

		glm::vec3 m_Rotation, m_FocalPoint;

		bool m_Panning, m_Rotating;
		bool m_Active = false;

		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;

		float m_Distance;
		float m_MinFocusDistance = 100.0f;
		uint32 m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}
