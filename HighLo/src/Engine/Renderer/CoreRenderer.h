// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-16) fixed indentations
//     - 1.0 (2021-09-14) initial release
//

/**
 * 
 * Idea for this file (@FlareCoding):
 * I like the idea of having a default-shape renderer, so maybe we could rewrite this?
 * In the current state this class would have to get a RenderPass as the render target
 * and then it would have to render the hole meshes into the RenderPass instead.
 * 
 * The rest could stay the same.
 * 
 */

#pragma once

#if 0

#include "Material.h"
#include "Mesh.h"
#include "Light.h"
#include "Engine/Camera/ICamera.h"
#include "Engine/Math/Transform.h"

namespace highlo
{
	class CoreRenderer
	{
	public:
		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void BeginScene(const ICamera &camera, const Light &light = Light());

		HLAPI static void DrawCube(const glm::vec3 &position, float size = 1.0f, float rotation = 0.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCube(const glm::vec3 &position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);
		HLAPI static void DrawCube(const Transform &transform, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCube(const Transform &transform, Ref<Material> material);

		HLAPI static void DrawSphere(const glm::vec3 &position, float size = 1.0f, float rotation = 0.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawSphere(const glm::vec3 &position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);
		HLAPI static void DrawSphere(const Transform &transform, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawSphere(const Transform &transform, Ref<Material> material);

		HLAPI static void DrawCapsule(const glm::vec3 &position, float size = 1.0f, float rotation = 0.0f, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCapsule(const glm::vec3 &position, Ref<Material> material, float size = 1.0f, float rotation = 0.0f);
		HLAPI static void DrawCapsule(const Transform &transform, const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));
		HLAPI static void DrawCapsule(const Transform &transform, Ref<Material> material);

		HLAPI static void DrawMesh(const Ref<Mesh> &mesh, const Transform &transform);
		HLAPI static void DrawRawMeshData(Ref<Mesh> &mesh);

	private:

		// Internal helper functions
		static void RenderVertexArray(Ref<VertexArray> &va);
		static Ref<Shader> SelectMaterialShader(Ref<Material> material, bool animated);
		static void SetObjectDataBuffer(Ref<Shader> shader, const Transform &transform);
		static void RenderMesh(Ref<Mesh> mesh, Ref<Material> material, const Transform &transform);

	private:
		struct SceneData
		{
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::vec3 m_CameraPosition;
			float	  m_Padding01;
			glm::vec3 m_LightPosition;
			float	  m_Padding02;
			glm::vec3 m_LightColor;
			float	  m_Padding03;
		};

		static SceneData s_SceneData;

	private:
		static Ref<Material> s_DefaultMaterial;
		static Ref<Mesh>	 s_CubeMesh;
		static Ref<Mesh>	 s_SphereMesh;
		static Ref<Mesh>	 s_CapsuleMesh;
	};
}

#endif
