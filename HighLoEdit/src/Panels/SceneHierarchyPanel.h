#pragma once

#include "Panel.h"

class SceneHierarchyPanel : public Panel
{
public:

	using HierarchyChangedFn = std::function<void(Entity)>;

	SceneHierarchyPanel(const Ref<Scene> &scene);

	void SetContext(const Ref<Scene> &scene);
	void SetSelected(Entity entity);
	void SetSelectionChangedCallback(const HierarchyChangedFn &func) { m_SelectionChangedCallback = func; }
	void SetEntityDeletedCallback(const HierarchyChangedFn &func) { m_EntityDeletedCallback = func; }

	virtual void Initialize(uint32 width, uint32 height) override;
	virtual void Destroy() override;
	virtual void Update(Timestep ts) override;
	virtual void Render(Timestep ts) override;
	virtual void OnEvent(Event &e) override;
	virtual void OnResize(uint32 width, uint32 height) override;

	virtual bool IsMouseOver() const override { return m_IsOver; }
	virtual bool IsSelected() const override { return m_IsSelected; }

private:

	void DrawEntityNode(Entity entity);
	void DrawMeshNode(const Ref<Mesh> &mesh, uint32 &imguiMeshID);
	void DrawComponents(Entity entity);

	bool m_IsOver = false;
	bool m_IsSelected = false;

	Ref<Scene> m_Context;
	Entity m_SelectedEntity;

	HierarchyChangedFn m_SelectionChangedCallback;
	HierarchyChangedFn m_EntityDeletedCallback;
};

