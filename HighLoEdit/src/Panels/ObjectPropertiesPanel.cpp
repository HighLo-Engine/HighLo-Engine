#include "ObjectPropertiesPanel.h"

void ObjectPropertiesPanel::Initialize(uint32 width, uint32 height)
{
}

void ObjectPropertiesPanel::Destroy()
{
}

void ObjectPropertiesPanel::Update(Timestep ts)
{
}

void ObjectPropertiesPanel::Render(Timestep ts)
{
	ImGuiRenderer::StartViewport("Object Properties");


	ImGuiRenderer::EndViewport();
}

void ObjectPropertiesPanel::OnEvent(Event &e)
{
}

void ObjectPropertiesPanel::OnResize(uint32 width, uint32 height)
{
}
