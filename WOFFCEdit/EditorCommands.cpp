#include "EditorCommands.h"

#include "ToolMain.h"

// Add new scene object command implementation .. 
AddNewSceneObjectCommand::AddNewSceneObjectCommand(ToolMain * tool) : m_tool(tool) {}

void AddNewSceneObjectCommand::execute(bool asRedo)
{
	if (asRedo) {
		m_sceneObject = m_tool->getGraph()->insertSceneObject(std::move(m_sceneObject));
	}
	else {
		m_sceneObject = m_tool->getGraph()->createNewSceneObject();
	}
}
void AddNewSceneObjectCommand::undo()
{
	// Attempt to remove the scene object that we added
	bool removed = m_tool->getGraph()->removeSceneObject(m_sceneObject);
	if (!removed) {
		// TODO: Debug log an error!
	}
}
std::wstring AddNewSceneObjectCommand::get_label()
{
	return L"Added new scene object";
}


// Change selection command implementation .. 
ChangeSelectionCommand::ChangeSelectionCommand(int newId, int lastId, ToolMain* tool)
	: m_lastSelectionId(lastId), m_newSelectionId(newId), m_tool(tool) {}

void ChangeSelectionCommand::execute(bool asRedo)
{
	m_tool->setSelectionID(m_newSelectionId);
}

void ChangeSelectionCommand::undo()
{
	m_tool->setSelectionID(m_lastSelectionId);
}

std::wstring ChangeSelectionCommand::get_label()
{
	return L"Selected object: " + std::to_wstring(m_newSelectionId);
}
