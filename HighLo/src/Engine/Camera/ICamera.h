#pragma once
#include "Engine/Math/HLMath.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/HLTime.h"

namespace highlo
{
	class ICamera : public IsSharedReference
	{
	public:
		HLAPI virtual void Update() = 0;
		HLAPI virtual void OnWindowResize(uint32 width, uint32 height) = 0;
		HLAPI virtual void OnEvent(Event& e) = 0;

		HLAPI const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		HLAPI const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		HLAPI const glm::vec3& GetPosition()	 const { return m_Position; }
		HLAPI const glm::vec3 GetYawPitchRoll()  const { return glm::vec3(m_Yaw, m_Pitch, m_Roll); }

		HLAPI const float GetYaw()   const { return m_Yaw; }
		HLAPI const float GetPitch() const { return m_Pitch; }
		HLAPI const float GetRoll()  const { return m_Roll; }

		HLAPI void SetPosition(const glm::vec3& position);
		HLAPI void SetYawPitchRoll(const glm::vec3& orientation);

		HLAPI void Translate(const glm::vec3& translation);
		HLAPI void Rotate(const glm::vec3& rotation);

		HLAPI void SetYaw(float yaw);
		HLAPI void SetPitch(float pitch);
		HLAPI void SetRoll(float roll);

		HLAPI void InvertPitch();

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		virtual void UpdateProjectionMatrix() = 0;
		virtual void UpdateViewMatrix() = 0;

	protected:
		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;
	};
}
