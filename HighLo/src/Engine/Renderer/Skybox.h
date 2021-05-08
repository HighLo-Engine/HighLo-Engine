#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Engine/Camera/ICamera.h"

namespace highlo
{
	class Skybox : public IsSharedReference
	{
	public:
		HLAPI static Ref<Skybox> Create();

		HLAPI inline const Ref<Texture>& GetTexture() const { return m_Texture; }
		HLAPI inline void SetTexture(Ref<Texture3D> texture) { m_Texture = texture; }
		HLAPI void Render(ICamera& camera);

	private:
		Skybox();

	private:
		Ref<Texture3D> m_Texture;
		Ref<Shader> m_Shader;
		Ref<Mesh> m_Mesh;

		void CreateMesh();
		void CreateShader();

	private:
		struct VS_SceneData
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

		VS_SceneData m_SceneData;
	};
}
