// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Meshes/StaticModel.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Camera/ICamera.h"

namespace highlo
{
	class Skybox : public IsSharedReference
	{
	public:

		HLAPI Skybox();

		HLAPI static Ref<Skybox> Create();

		HLAPI inline const Ref<Texture3D> &GetTexture() const { return m_Texture; }
		HLAPI inline void SetTexture(Ref<Texture3D> texture) { m_Texture = texture; }
		HLAPI void Render(ICamera &camera);


	private:
		Ref<Texture3D> m_Texture;
		Ref<Shader> m_Shader;
		Ref<StaticModel> m_Mesh;

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
