#include "FileMenuDemo.h"

void FileMenuDemo::OnInitialize()
{
	GetWindow().CenterWindow();

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Scene", "", 1, [=](FileMenu *menu, MenuItem *item) { HL_WARN("Clicked New Scene"); });
	fileMenu->AddMenuItem("Open Scene", "", 2, [=](FileMenu *menu, MenuItem *item) { HL_WARN("Clicked Open Scene"); });

	fileMenu->AddSeparator();

	Ref<FileMenu> zoomMenu = FileMenu::Create("Zoom");
	zoomMenu->AddMenuItem("200%", "", 3, [=](FileMenu *menu, MenuItem *item) { HL_WARN("Clicked 200%"); });
	zoomMenu->AddMenuItem("100%", "", 4, [=](FileMenu *menu, MenuItem *item) { HL_WARN("Clicked 100%"); });
	zoomMenu->AddMenuItem("50%", "", 5, [=](FileMenu *menu, MenuItem *item) { HL_WARN("Clicked 50%"); });

	fileMenu->AddSubMenu(zoomMenu);

	m_Menubar = MenuBar::Create();
	m_Menubar->AddMenu(fileMenu);
	GetWindow().Maximize();
	GetWindow().SetMenuBar(m_Menubar);
}

void FileMenuDemo::OnUpdate(Timestep timestep)
{

}

void FileMenuDemo::OnShutdown()
{

}

void FileMenuDemo::OnEvent(Event &e)
{
	m_Menubar->OnEvent(e);
}

void FileMenuDemo::OnUIRender(Timestep timestep)
{

}

void FileMenuDemo::OnResize(uint32 width, uint32 height)
{

}

