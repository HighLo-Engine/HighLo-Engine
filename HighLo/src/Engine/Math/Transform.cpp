#include "HighLoPch.h"
#include "Transform.h"

namespace highlo
{
	void Transform::Translate(const glm::vec3 &translation)
	{
		m_Transform = glm::translate(m_Transform, translation);
	}

	void Transform::Scale(const glm::vec3 &scale)
	{
		m_Transform = glm::scale(m_Transform, scale);
	}

	void Transform::Rotate(float angle, const glm::vec3 &axis)
	{
		float radians = glm::radians(angle);

		const float sinHalfAngle = sin(radians / 2.0f);
		const float cosHalfAngle = cos(radians / 2.0f);

		m_Rotation.x = axis.x * sinHalfAngle;
		m_Rotation.y = axis.y * sinHalfAngle;
		m_Rotation.z = axis.z * sinHalfAngle;
		m_Rotation.w = cosHalfAngle;

		m_Transform = glm::mat4_cast(m_Rotation) * m_Transform;
	}
}