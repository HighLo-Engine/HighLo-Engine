// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "ICamera.h"

namespace highlo
{
	class Camera : public ICamera
	{
	public:

		enum class ProjectionType
		{
			Perspective = 0, Orthographic = 1
		};

		HLAPI Camera();
		HLAPI virtual ~Camera() = default;

		HLAPI void SetPerspective(float verticalFOV = 45.0f, float nearClip = 0.01f, float farClip = 10000.0f);
		HLAPI void SetOrthographic(float size = 10.0f, float nearClip = -1.0f, float farClip = 1.0f);
		HLAPI void SetViewportSize(uint32 width, uint32 height);

		HLAPI inline void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = glm::radians(fov); }
		HLAPI inline void SetPerspectiveNearPlane(float nearPlane) { m_PerspectiveNear = nearPlane; }
		HLAPI inline void SetPerspectiveFarPlane(float farPlane) { m_PerspecticeFar = farPlane; }

		HLAPI inline void SetOrthographicSize(float size) { m_OrthographicSize = size; }
		HLAPI inline void SetOrthographicNearPlane(float nearPlane) { m_OrthographicNear = nearPlane; }
		HLAPI inline void SetOrthographicFarPlane(float farPlane) { m_OrthographicFar = farPlane; }

		HLAPI inline float GetPerspectiveFOV() const { return glm::degrees(m_PerspectiveFOV); }
		HLAPI inline float GetPerspectiveNearPlane() const { return m_PerspectiveNear; }
		HLAPI inline float GetPerspectiveFarPlane() const { return m_PerspecticeFar; }

		HLAPI inline float GetOrthographicSize() const { return m_OrthographicSize; }
		HLAPI inline float GetOrthographicNearPlane() const { return m_OrthographicNear; }
		HLAPI inline float GetOrthographicFarPlane() const { return m_OrthographicFar; }

		HLAPI virtual void GetDirectionVectors(glm::vec3 &front, glm::vec3 &up, glm::vec3 &left);

		HLAPI virtual void Update() override {}
		HLAPI virtual void OnWindowResize(uint32 width, uint32 height) override;
		HLAPI virtual void OnEvent(Event &e) override {}

	private:

		void SetProjectionInternal(uint32 width, uint32 height);

		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspecticeFar = 10000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
	};
}