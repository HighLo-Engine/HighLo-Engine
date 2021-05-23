#include "PopupMenuTest.h"

void PopupMenuTest::OnInitialize()
{
	GetWindow().CenterWindow();

	m_PopupMenu = PopupMenu::Create("Testing");
	m_PopupMenu->AddMenuItem("Test menu item", 1, [=]() { HL_WARN("Clicked first popup menu Item"); });
	m_PopupMenu->AddMenuItem("Test menu item 2", 2, [=]() { HL_WARN("Clicked second popup menu Item"); });
	m_PopupMenu->AddSeparator();
	m_PopupMenu->AddMenuItem("Test menu item 3", 3, [=]() { HL_WARN("Clicked third popup menu Item"); });

	Ref<PopupMenu> menu = PopupMenu::Create("Testing 2");
	menu->AddMenuItem("Testing2 Test", 1, [=]() { HL_WARN("Clicked first submenu popup menu Item"); });
	menu->AddMenuItem("Testing2 Test2", 2, [=]() { HL_WARN("Clicked second submenu popup menu Item"); });
	m_PopupMenu->AddSubMenu(menu);
}

void PopupMenuTest::OnUpdate(Timestep timestep)
{
	m_PopupMenu->Update();
}

void PopupMenuTest::OnShutdown()
{
}

void PopupMenuTest::OnEvent(Event &e)
{
	m_PopupMenu->OnEvent(e);
}

void PopupMenuTest::OnUIRender(Timestep timestep)
{
}

void PopupMenuTest::OnResize(uint32 width, uint32 height)
{
}

