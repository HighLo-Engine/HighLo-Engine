#pragma once
#include "Engine/ECS/Entity.h"
#include "Engine/Math/HLFrustum.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Renderer/Light.h"

namespace highlo
{
	class Scene : public IsSharedReference
	{
	public:
		HLAPI Scene(const HLString& name = "Scene1");

		HLString Name;
		Ref<Camera> Camera;
		Light SceneLight;

		HLAPI void Update(Timestep ts);

		HLAPI void AddEntity(Entity entity);

	private:
		glm::mat4 m_FrustumInitMatrix;
		HLFrustum m_CameraFrustum;

		std::vector<Entity> m_Entities;
		std::map<EntityID, RenderComponent*> m_RenderComponentMap;
	};
}
