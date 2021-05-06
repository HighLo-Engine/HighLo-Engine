#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace highlo
{
	class Transform
	{
	public:

		HLAPI Transform() = default;

		HLAPI void Translate(const glm::vec3 &translation);
		HLAPI void Scale(const glm::vec3 &scale);
		HLAPI void Rotate(float angle, const glm::vec3 &axis);

		HLAPI inline const glm::vec3 &GetPosition() {	return m_Position; }
		HLAPI inline const glm::vec3 &GetScale() { return m_Scale; }
		HLAPI inline const glm::quat &GetRotation() { return m_Rotation; }
		HLAPI inline const glm::mat4 &GetTransform() { return m_Transform; }

		HLAPI bool operator==(const Transform &other) const
		{
			// this is faster than just comparing the transform because a transform has 16 comparisons and this has just 10
			return m_Position == other.m_Position && m_Scale == other.m_Scale && m_Rotation == other.m_Rotation;
		}

		HLAPI bool operator!=(const Transform &other) const
		{
			return !(*this == other);
		}

	private:

		glm::vec3 m_Position  = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale     = { 1.0f, 1.0f, 1.0f };
		glm::quat m_Rotation  = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::mat4 m_Transform = glm::mat4(1.0f);

	};
}

