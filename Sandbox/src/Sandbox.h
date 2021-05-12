#pragma once

#include <HighLo.h>
using namespace highlo;

#include <Engine/ECS/RenderSystem.h>

class Sandbox : public HLApplication
{
public:
	void OnInitialize() override;
	void OnUpdate(Timestep timestep) override;
	void OnShutdown() override;
	void OnEvent(Event& e) override;
	void OnImGuiRender() override;
	void OnResize(uint32 width, uint32 height) override;

private:
	Ref<RenderSystem> m_RenderSystemRef;

	Ref<Scene> m_Scene;
};
