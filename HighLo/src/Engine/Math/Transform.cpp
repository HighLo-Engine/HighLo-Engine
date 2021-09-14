// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Transform.h"

#include <glm/gtx/quaternion.hpp>

namespace highlo
{
	Transform Transform::OriginTransform = Transform::FromPosition({ 0, 0, 0 });

	Transform &Transform::Translate(const glm::vec3 &translation)
	{
		m_Position += translation;
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(m_Transform, translation);
		return *this;
	}

	Transform &Transform::Scale(const glm::vec3 &scale)
	{
		m_Scale *= scale;
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);
		m_Transform = glm::scale(m_Transform, scale);
		return *this;
	}

	Transform &Transform::Scale(float scale)
	{
		m_Scale *= glm::vec3(scale, scale, scale);
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);
		m_Transform = glm::scale(m_Transform, {scale, scale, scale});
		return *this;
	}

	Transform &Transform::Rotate(float angle, const glm::vec3 &axis)
	{
		glm::vec3 rotation_axis_value = axis * glm::radians(angle);
		glm::quat rotation = glm::normalize(glm::quat(m_Rotation) * glm::quat(rotation_axis_value));
		m_Rotation = glm::eulerAngles(rotation);

		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::mat4_cast(rotation);

		return *this;
	}

	Transform &Transform::Rotate(const glm::quat &rotation)
	{
		glm::mat4 rotationMatrix = glm::toMat4(rotation);

		glm::vec3 rot;
		highlo::DecomposeRotation(rotationMatrix, rot);
		m_Rotation += rot;

		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::scale(glm::mat4(1.0f), m_Scale) * rotationMatrix;

		return *this;
	}

	Transform Transform::FromPosition(const glm::vec3 &position)
	{
		Transform transform;
		transform.Translate(position);
		return transform;
	}

	Transform Transform::FromRotation(const glm::vec3 &rotation)
	{
		Transform transform;
		transform.Rotate(rotation.x, { 1, 0, 0 });
		transform.Rotate(rotation.y, { 0, 1, 0 });
		transform.Rotate(rotation.z, { 0, 0, 1 });
		return transform;
	}

	Transform Transform::FromScale(const glm::vec3 &scale)
	{
		Transform transform;
		transform.Scale(scale);
		return transform;
	}

	void Transform::SetPosition(const glm::vec3 &position)
	{
		m_Position = position;
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::toMat4(glm::quat(m_Rotation));
	}

	void Transform::SetRotation(const glm::vec3 &rotation)
	{
		m_Rotation = rotation;

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::toMat4(glm::quat(m_Rotation));
	}

	void Transform::SetScale(const glm::vec3 &scale)
	{
		m_Scale = scale;
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::toMat4(glm::quat(m_Rotation));
	}

	void Transform::SetScale(float scale)
	{
		m_Scale = glm::vec3(scale, scale, scale);
		//m_BoundingBox->SetTransform(m_Position, glm::eulerAngles(m_Rotation), m_Scale);

		m_Transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) * glm::toMat4(glm::quat(m_Rotation));
	}
	
	void Transform::Decompose(const glm::mat4 &transform, Transform &outTransform)
	{
		highlo::Decompose(transform, outTransform.m_Position, outTransform.m_Scale, outTransform.m_Rotation);
	}
}