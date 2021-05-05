#include "FileMenuTest.h"

void FileMenuTest::OnInitialize()
{
	GetWindow().CenterWindow();

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", 1, [=]() { HL_WARN("Clicked New Scene"); });
	fileMenu->AddMenuItem("Open Scene", 2, [=]() { HL_WARN("Clicked Open Scene"); });

	fileMenu->AddMenuSeperator();

	Ref<FileMenu> zoomMenu = FileMenu::Create("Zoom");
	zoomMenu->AddMenuItem("200%", 3, [=]() { HL_WARN("Clicked 200%"); });
	zoomMenu->AddMenuItem("100%", 4, [=]() { HL_WARN("Clicked 100%"); });
	zoomMenu->AddMenuItem("50%", 5, [=]() { HL_WARN("Clicked 50%"); });

	fileMenu->AddSubMenu(zoomMenu);

	m_Menubar = MenuBar::Create();
	m_Menubar->AddMenu(fileMenu);
	GetWindow().SetMenuBar(m_Menubar);
}

void FileMenuTest::OnUpdate(Timestep timestep)
{
}

void FileMenuTest::OnShutdown()
{
}

void FileMenuTest::OnEvent(Event &e)
{
	m_Menubar->OnEvent(e);
}

