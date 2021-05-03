#include "FileMenuTest.h"

void FileMenuTest::OnInitialize()
	{
	GetWindow().CenterWindow();

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", 1, true);
	fileMenu->AddMenuItem("Open Scene", 2, true);

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

