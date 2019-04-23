#include "EditorCommands.h"

#include "ToolMain.h"

AddNewSceneObjectCommand::AddNewSceneObjectCommand(ToolMain * tool) : m_tool(tool) {}

void AddNewSceneObjectCommand::execute(bool asRedo)
{
	if (asRedo) {
		m_sceneObject = m_tool->insertSceneObject(std::move(m_sceneObject));
	}
	else {
		m_sceneObject = m_tool->createNewSceneObject();
	}
}
void AddNewSceneObjectCommand::undo()
{
	// Attempt to remove the scene object that we added
	bool removed = m_tool->removeSceneObject(m_sceneObject);
	if (!removed) {
		// TODO: Debug log an error!
	}
}
std::wstring AddNewSceneObjectCommand::get_label()
{
	return L"Added new scene object";
}

