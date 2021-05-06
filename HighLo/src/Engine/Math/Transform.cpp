#include "HighLoPch.h"
#include "Transform.h"

namespace highlo
{
	Transform& Transform::Translate(const glm::vec3 &translation)
	{
		m_Transform = glm::translate(m_Transform, translation);
		return *this;
	}

	Transform& Transform::Scale(const glm::vec3 &scale)
	{
		m_Transform = glm::scale(m_Transform, scale);
		return *this;
	}

	Transform& Transform::Scale(float scale)
	{
		m_Transform = glm::scale(m_Transform, {scale, scale, scale});
		return *this;
	}

	Transform& Transform::Rotate(float angle, const glm::vec3 &axis)
	{
		float radians = glm::radians(angle);

		const float sinHalfAngle = sin(radians / 2.0f);
		const float cosHalfAngle = cos(radians / 2.0f);

		m_Rotation.x = axis.x * sinHalfAngle;
		m_Rotation.y = axis.y * sinHalfAngle;
		m_Rotation.z = axis.z * sinHalfAngle;
		m_Rotation.w = cosHalfAngle;

		m_Transform = glm::mat4_cast(m_Rotation) * m_Transform;

		return *this;
	}
	
	Transform Transform::FromPosition(const glm::vec3& position)
	{
		Transform transform;
		transform.Translate(position);
		return transform;
	}

	Transform Transform::FromRotation(const glm::vec3& rotation)
	{
		Transform transform;
		transform.Rotate(rotation.x, { 1, 0, 0 });
		transform.Rotate(rotation.y, { 0, 1, 0 });
		transform.Rotate(rotation.z, { 0, 0, 1 });
		return transform;
	}
	
	Transform Transform::FromScale(const glm::vec3& scale)
	{
		Transform transform;
		transform.Scale(scale);
		return transform;
	}
}