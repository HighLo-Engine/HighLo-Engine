#pragma once

#include "Engine/Camera/ICamera.h"

namespace highlo
{
	class OrthographicCamera : public ICamera
	{
	public:

		HLAPI OrthographicCamera(float width, float height);
		HLAPI void SetProjection(float width, float height);


		HLAPI virtual void Update() override;
		HLAPI virtual void OnWindowResize(uint32 width, uint32 height) override;
		HLAPI virtual void OnEvent(Event &e) override;

		HLAPI static Ref<OrthographicCamera> Create(float width, float height);

	private:

		virtual void UpdateProjectionMatrix() override;
		virtual void UpdateViewMatrix() override;

		bool OnWindowResizeInternal(WindowResizeEvent &e);
		bool OnMouseScrolledInternal(MouseScrolledEvent &e);

		float m_TranslationSpeed = 3.0f;
		float m_RotationSpeed = 50.0f;

		float m_Rotation = 0.0f;
		float m_ZoomLevel = 1.0f;
		float m_ViewportWidth = 0.0f;
		float m_ViewportHeight = 0.0f;
	};
}