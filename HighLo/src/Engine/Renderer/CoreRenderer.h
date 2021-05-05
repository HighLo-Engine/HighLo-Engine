#pragma once
#include "Material.h"
#include "Shapes.h"
#include "Engine/Camera/ICamera.h"

namespace highlo
{
	class CoreRenderer
	{
	public:
		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void BeginScene(const ICamera& camera);
		HLAPI static void EndScene();

		HLAPI static void DrawCube(const glm::vec3& position, float size, const glm::vec3& color);
		HLAPI static void DrawCube(const glm::vec3& position, float size, Ref<Material> material);

	private:
		struct SceneData
		{
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::vec3 m_CameraPosition;
			float	  m_Padding01;
		};

		static SceneData m_SceneData;

	private:
		static Ref<Material> m_DefaultMaterial;
		static Ref<Mesh>	 m_CubeMesh;
	};
}
