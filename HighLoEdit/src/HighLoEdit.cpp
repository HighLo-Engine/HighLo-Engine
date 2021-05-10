#include "HighLoEdit.h"

HighLoEditor::HighLoEditor()
{
}

HighLoEditor::~HighLoEditor()
{
}

void HighLoEditor::OnInitialize()
{


	FileSystem::StartWatching();
}

void HighLoEditor::OnUpdate(Timestep timestep)
{
}

void HighLoEditor::OnShutdown()
{
	FileSystem::StopWatching();


}

void HighLoEditor::OnEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnKeyPressedEvent));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HL_BIND_EVENT_FUNCTION(HighLoEditor::OnMouseButtonPressedEvent));
}

void HighLoEditor::UpdateWindowTitle(const HLString &sceneName)
{
	HLString rendererAPI = "OpenGL"; // TODO: get this from api
	HLString title = sceneName + " - HighLo Engine - " + " Renderer: " + rendererAPI;
	GetWindow().SetTitle(title);
}

bool HighLoEditor::OnKeyPressedEvent(const KeyPressedEvent &e)
{
	return false;
}

bool HighLoEditor::OnMouseButtonPressedEvent(const MouseButtonPressedEvent &e)
{
	return false;
}
