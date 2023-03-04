// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-15) Added SetTransform function to set the transform matrix directly
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/Core/Core.h"
#include "Math.h"

namespace highlo
{
	/// <summary>
	/// Represents the object local transform in shaders
	/// </summary>
	struct TransformVertexData
	{
		glm::vec4 Row0;
		glm::vec4 Row1;
		glm::vec4 Row2;
	};

	class Transform
	{
	public:

		static Transform OriginTransform;

		HLAPI Transform() = default;
		HLAPI Transform(const glm::mat4 &transform);

		HLAPI static Transform Identity();

		HLAPI Transform &Translate(const glm::vec3 &translation);
		HLAPI Transform &Scale(const glm::vec3 &scale);
		HLAPI Transform &Scale(float scale);
		HLAPI Transform &Rotate(float angle, const glm::vec3 &axis);
		HLAPI Transform &Rotate(const glm::quat &rotation);

		HLAPI static Transform FromPosition(const glm::vec3 &position);
		HLAPI static Transform FromRotation(const glm::vec3 &rotation);
		HLAPI static Transform FromScale(const glm::vec3 &scale);

		HLAPI void SetPosition(const glm::vec3 &position);
		HLAPI void SetRotation(const glm::vec3 &rotation);
		HLAPI void SetScale(const glm::vec3 &scale);
		HLAPI void SetScale(float scale);
		HLAPI void SetTransform(const glm::mat4 &transform);

		HLAPI static void Decompose(const glm::mat4 &transform, Transform &outTransform);

		HLAPI inline glm::vec3 &GetPosition() {	return m_Position; }
		HLAPI inline const glm::vec3 &GetPosition() const {	return m_Position; }
		HLAPI inline glm::vec3 &GetScale() { return m_Scale; }
		HLAPI inline const glm::vec3 &GetScale() const { return m_Scale; }
		HLAPI inline glm::vec3 &GetRotation() { return m_Rotation; }
		HLAPI inline const glm::vec3 &GetRotation() const { return m_Rotation; }
		HLAPI inline const glm::quat GetRotationAsQuat() const { return glm::quat(m_Rotation); }
		HLAPI inline const glm::mat4 &GetTransform() const { return m_Transform; }

		HLAPI bool operator==(const Transform &other) const
		{
			// this is faster than just comparing the transform because a transform has 16 comparisons and this has just 9
			return m_Position == other.m_Position && m_Scale == other.m_Scale && m_Rotation == other.m_Rotation;
		}

		HLAPI bool operator!=(const Transform &other) const
		{
			return !(*this == other);
		}

	private:

		glm::vec3 m_Position  = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale     = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_Rotation  = { 0.0f, 0.0f, 0.0f };
		glm::mat4 m_Transform = glm::mat4(1.0f);
	};
}

