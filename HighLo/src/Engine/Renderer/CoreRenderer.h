#pragma once

#include "Material.h"
#include "Mesh.h"
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

		HLAPI static void DrawCube(const glm::vec3& position, float size = 1.0f, float rotation = 0.0f, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCube(const glm::vec3& position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);

		HLAPI static void DrawSphere(const glm::vec3 &position, float size = 1.0f, float rotation = 0.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawSphere(const glm::vec3 &position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);

		HLAPI static void DrawCapsule(const glm::vec3& position, float size = 1.0f, float rotation = 0.0f, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCapsule(const glm::vec3& position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);

	private:
		struct SceneData
		{
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::vec3 m_CameraPosition;
			float	  m_Padding01;
		};

		static SceneData s_SceneData;

	private:
		static Ref<Material> s_DefaultMaterial;
		static Ref<Mesh>	 s_CubeMesh;
		static Ref<Mesh>	 s_SphereMesh;
		static Ref<Mesh>	 s_CapsuleMesh;
	};
}
