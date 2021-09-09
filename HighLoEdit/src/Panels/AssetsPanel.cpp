#include "AssetsPanel.h"

void AssetsPanel::Initialize(uint32 width, uint32 height)
{
	m_Popup = PopupMenu::Create("MainPopup");
	m_Popup->AddMenuItem("Create Folder", 1, [=](PopupMenuItem item) { HL_TRACE("Creating folder"); });
	m_Popup->AddSeparator();
	m_Popup->AddMenuItem("Test", 2, [=](PopupMenuItem item) { HL_TRACE("TEST"); });
}

void AssetsPanel::Destroy()
{
}

void AssetsPanel::Update(Timestep ts)
{
}

void AssetsPanel::Render(Timestep ts)
{
	UI::BeginViewport("Assets");
	m_IsSelected = UI::IsWindowFocused();
	m_IsHovered = UI::IsWindowHovered();

	if (m_IsHovered)
		UI::DrawPopupMenu(m_Popup);



	UI::EndViewport();
}

void AssetsPanel::OnEvent(Event &e)
{
}

void AssetsPanel::OnResize(uint32 width, uint32 height)
{
}

