#include "SceneHierarchyPanel.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace editorutils
{
	static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4 &transform)
	{
		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		return { translation, orientation, scale };
	}
}

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene)
	: m_Context(scene)
{
}

void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene)
{
	m_Context = scene;
	m_SelectedEntity = {};
}

void SceneHierarchyPanel::SetSelected(Entity entity)
{
	m_SelectedEntity = entity;
	if (m_SelectionChangedCallback)
		m_SelectionChangedCallback(m_SelectedEntity);
}

void SceneHierarchyPanel::Initialize(uint32 width, uint32 height)
{
}

void SceneHierarchyPanel::Destroy()
{
}

void SceneHierarchyPanel::Update(Timestep ts)
{
}

void SceneHierarchyPanel::Render(Timestep ts)
{
	UI::BeginViewport("Scene Hierarchy");
	auto windowRect = UI::GetWindowRect();

	if (m_Context)
	{
		uint32 entityCount = 0, meshCount = 0;
		
	}

	UI::EndViewport();
}

void SceneHierarchyPanel::OnEvent(Event &e)
{
}

void SceneHierarchyPanel::OnResize(uint32 width, uint32 height)
{
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
}

void SceneHierarchyPanel::DrawMeshNode(const Ref<Mesh> &mesh, uint32 &imguiMeshID)
{
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
}

template<typename T, typename UIFunc>
static void DrawComponent(const HLString &name, Entity entity, UIFunc func, bool canBeRemoved = false)
{
	if (entity.HasComponent<T>())
	{
		auto &component = entity.GetComponent<T>();
		
	}
}