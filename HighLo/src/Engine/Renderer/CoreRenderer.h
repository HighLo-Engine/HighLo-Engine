#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Engine/Camera/ICamera.h"
#include "Engine/Math/Transform.h"

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
		HLAPI static void DrawCube(const Transform& transform, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCube(const Transform& transform, Ref<Material> material);

		HLAPI static void DrawSphere(const glm::vec3 &position, float size = 1.0f, float rotation = 0.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawSphere(const glm::vec3 &position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);
		HLAPI static void DrawSphere(const Transform& transform, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawSphere(const Transform& transform, Ref<Material> material);

		HLAPI static void DrawCapsule(const glm::vec3& position, float size = 1.0f, float rotation = 0.0f, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCapsule(const glm::vec3& position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);
		HLAPI static void DrawCapsule(const Transform& transform, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCapsule(const Transform& transform, Ref<Material> material);

		HLAPI static void DrawMesh(const Ref<Mesh>& mesh, const Transform& transform);

	private:
		// Internal helper functions
		static void RenderVertexArray(Ref<VertexArray>& va);
		static Ref<Shader> SelectMaterialShader(Ref<Material> material, bool animated);
		static void SetObjectDataBuffer(Ref<Shader> shader, const Transform& transform);
		static void RenderMesh(Ref<Mesh> mesh, Ref<Material> material, const Transform& transform);

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
