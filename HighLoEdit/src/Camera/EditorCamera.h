#pragma once

#include <HighLo.h>
using namespace highlo;

class EditorCamera : public Camera
{
public:

	EditorCamera() = default;
	EditorCamera(const glm::mat4 &projectionMatrix);

	void Focus(const glm::vec3 &focusPoint);
	virtual void Update() override;
	virtual void OnWindowResize(uint32 width, uint32 height) override;
	virtual void OnEvent(Event &e) override;

	inline float GetDistance() const { return m_Distance; }
	inline void SetDistance(float distance) { m_Distance = distance; }
	void SetProjection(const glm::mat4 &projection);

	glm::vec3 GetUpDirection();
	glm::vec3 GetRightDirection();
	glm::vec3 GetForwardDirection();
	glm::quat GetOrientation() const;

private:

	void UpdateCameraView();
	bool OnMouseScrolled(MouseScrolledEvent &e);

	void MousePan(const glm::vec2 &delta);
	void MouseRotate(const glm::vec2 &delta);
	void MouseZoom(float delta);

	glm::vec3 CalculatePosition();

	std::pair<float, float> PanSpeed() const;
	float RotationSpeed() const;
	float ZoomSpeed() const;

private:

	glm::vec3 m_Rotation, m_FocalPoint;

	bool m_Panning, m_Rotating;

	glm::vec2 m_InitialMousePosition;
	glm::vec3 m_InitialFocalPoint, m_InitialRotation;

	float m_Distance;
	float m_MinFocusDistance = 100.0f;
	uint32 m_ViewportWidth = 1280, m_ViewportHeight = 720;
};