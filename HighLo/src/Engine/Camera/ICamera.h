#pragma once
#include "Engine/Math/HLMath.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/HLTime.h"

namespace highlo
{
	class ICamera : public IsSharedReference
	{
	public:
		HLAPI ICamera() = default;
		HLAPI virtual ~ICamera() = default;

		HLAPI virtual void Update() = 0;
		HLAPI virtual void OnWindowResize(uint32 width, uint32 height) = 0;
		HLAPI virtual void OnEvent(Event& e) = 0;

		HLAPI const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		HLAPI const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		HLAPI glm::mat4 GetViewProjectionMatrix() { return m_ProjectionMatrix * m_ViewMatrix; }
		HLAPI void GetViewProjectionMatrix(glm::mat4& out) { MatrixMulSSE(m_ProjectionMatrix, m_ViewMatrix, out); }

		HLAPI const glm::vec3& GetPosition()	 const { return m_Position; }
		HLAPI const glm::vec3 GetYawPitchRoll()  const { return glm::vec3(m_Yaw, m_Pitch, m_Roll); }

		HLAPI const float GetYaw()   const { return m_Yaw; }
		HLAPI const float GetPitch() const { return m_Pitch; }
		HLAPI const float GetRoll()  const { return m_Roll; }

		HLAPI void SetPosition(const glm::vec3& position);
		HLAPI void SetYawPitchRoll(const glm::vec3& orientation);

		HLAPI void SetYaw(float yaw);
		HLAPI void SetPitch(float pitch);
		HLAPI void SetRoll(float roll);

		HLAPI void InvertPitch();

		HLAPI float GetExposure() const { return m_Exposure; }
		HLAPI float &GetExposure() { return m_Exposure; }

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

	protected:
		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;
		float m_Exposure = 0.0f;
	};
}
