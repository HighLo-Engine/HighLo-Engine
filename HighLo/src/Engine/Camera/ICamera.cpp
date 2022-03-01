// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ICamera.h"

namespace highlo
{
	void ICamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	void ICamera::SetYawPitchRoll(const glm::vec3& orientation)
	{
		m_Yaw = orientation.x;
		m_Pitch = orientation.y;
		m_Roll = orientation.z;
	}

	void ICamera::SetYaw(float yaw)
	{
		m_Yaw = yaw;
	}

	void ICamera::SetPitch(float pitch)
	{
		m_Pitch = pitch;
	}

	void ICamera::SetRoll(float roll)
	{
		m_Roll = roll;
	}

	void ICamera::InvertPitch()
	{
		m_Pitch = -m_Pitch;
	}
}
