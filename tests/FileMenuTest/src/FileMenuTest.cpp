#include "FileMenuTest.h"

void FileMenuTest::OnInitialize()
{
	GetWindow().CenterWindow();

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", 1, true);
	fileMenu->AddMenuItem("Open Scene", 2, true);

	fileMenu->AddMenuSeperator();

	Ref<FileMenu> zoomMenu = FileMenu::Create("Zoom");
	zoomMenu->AddMenuItem("200%", 3, true);
	zoomMenu->AddMenuItem("100%", 4, true);
	zoomMenu->AddMenuItem("50%", 5, true);

	fileMenu->AddSubMenu(zoomMenu);

	Ref<MenuBar> menubar = MenuBar::Create();
	menubar->AddMenu(fileMenu);
	GetWindow().SetMenuBar(menubar);
}

void FileMenuTest::OnUpdate(Timestep timestep)
{
}

void FileMenuTest::OnShutdown()
{
}

void FileMenuTest::OnEvent(Event &e)
{
	HL_INFO(*e.ToString());
}

