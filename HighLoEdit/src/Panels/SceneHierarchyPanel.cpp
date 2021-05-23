#include "SceneHierarchyPanel.h"

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
	ImGuiRenderer::StartViewport("Scene Hierarchy");
	ImGuiRenderer::EndViewport();
}

void SceneHierarchyPanel::OnEvent(Event &e)
{
}

void SceneHierarchyPanel::OnResize(uint32 width, uint32 height)
{
}

