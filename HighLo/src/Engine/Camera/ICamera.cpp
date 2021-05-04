#include "HighLoPch.h"
#include "ICamera.h"

namespace highlo
{
	void ICamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void ICamera::SetYawPitchRoll(const glm::vec3& orientation)
	{
		m_Yaw = orientation.x;
		m_Pitch = orientation.y;
		m_Roll = orientation.z;
		UpdateViewMatrix();
	}

	void ICamera::Translate(const glm::vec3& translation)
	{
		m_Position += translation;
		UpdateViewMatrix();
	}

	void ICamera::Rotate(const glm::vec3& rotation)
	{
		m_Yaw += rotation.x;
		m_Pitch += rotation.y;
		m_Roll += rotation.z;
		UpdateViewMatrix();
	}

	void ICamera::SetYaw(float yaw)
	{
		m_Yaw = yaw;
		UpdateViewMatrix();
	}

	void ICamera::SetPitch(float pitch)
	{
		m_Pitch = pitch;
		UpdateViewMatrix();
	}

	void ICamera::SetRoll(float roll)
	{
		m_Roll = roll;
		UpdateViewMatrix();
	}

	void ICamera::InvertPitch()
	{
		m_Pitch = -m_Pitch;
		UpdateViewMatrix();
	}
}
