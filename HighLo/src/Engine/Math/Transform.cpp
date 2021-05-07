#include "HighLoPch.h"
#include "Transform.h"

namespace highlo
{
	Transform Transform::OriginTransform = Transform::FromPosition({ 0, 0, 0 });

	Transform& Transform::Translate(const glm::vec3 &translation)
	{
		m_Position += translation;
		m_Transform = glm::translate(m_Transform, translation);
		return *this;
	}

	Transform& Transform::Scale(const glm::vec3 &scale)
	{
		m_Scale *= scale;
		m_Transform = glm::scale(m_Transform, scale);
		return *this;
	}

	Transform& Transform::Scale(float scale)
	{
		m_Scale *= glm::vec3(scale, scale, scale);
		m_Transform = glm::scale(m_Transform, {scale, scale, scale});
		return *this;
	}

	Transform& Transform::Rotate(float angle, const glm::vec3 &axis)
	{
		glm::vec3 rotation_axis_value = axis * angle;
		m_Rotation = glm::normalize(m_Rotation * glm::quat(rotation_axis_value));

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::mat4_cast(m_Rotation);

		return *this;
	}

	void Transform::SetPosition(const glm::vec3& position)
	{
		m_Position = position;

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::mat4_cast(m_Rotation);
	}

	void Transform::SetRotation(const glm::vec3& rotation)
	{
		float cy = cos(glm::radians(rotation.x) * 0.5f);
		float sy = sin(glm::radians(rotation.x) * 0.5f);
		float cp = cos(glm::radians(rotation.y) * 0.5f);
		float sp = sin(glm::radians(rotation.y) * 0.5f);
		float cr = cos(glm::radians(rotation.z) * 0.5f);
		float sr = sin(glm::radians(rotation.z) * 0.5f);
		m_Rotation.w = cy * cp * cr + sy * sp * sr;
		m_Rotation.x = cy * cp * sr - sy * sp * cr;
		m_Rotation.y = sy * cp * sr + cy * sp * cr;
		m_Rotation.z = sy * cp * cr - cy * sp * sr;

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::mat4_cast(m_Rotation);
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	void Transform::SetScale(float scale)
	{
		m_Scale = glm::vec3(scale, scale, scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::mat4_cast(m_Rotation);
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