#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::OnInitialize()
{


	FileSystem::StartWatching();
}

void LevelEditor::OnUpdate(Timestep timestep)
{
}

void LevelEditor::OnShutdown()
{
	FileSystem::StopWatching();


}

void LevelEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(LevelEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(LevelEditor::OnMouseButtonPressedEvent));
}

void LevelEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString rendererAPI = "OpenGL"; // TODO: get this from api
	HLString title = sceneName + " - HighLo Engine - " + " Renderer: " + rendererAPI;
	GetWindow().SetTitle(title);
}

bool LevelEditor::OnKeyPressedEvent(KeyPressedEvent &e)
{
	return false;
}

bool LevelEditor::OnMouseButtonPressedEvent(MouseButtonPressedEvent &e)
{
	return false;
}
