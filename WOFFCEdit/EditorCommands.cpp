#include "EditorCommands.h"

#include "ToolMain.h"

// Add new scene object command implementation .. 
AddNewSceneObjectCommand::AddNewSceneObjectCommand() {
	m_sceneObject = SceneObject::CreatePrimitive();
}

AddNewSceneObjectCommand::AddNewSceneObjectCommand(SceneObject * so) {
	m_sceneObject = *so;
}

void AddNewSceneObjectCommand::execute(ToolMain* tool, bool asRedo)
{
	m_sceneObject = tool->getGraph()->insertSceneObject(std::move(m_sceneObject));
}

void AddNewSceneObjectCommand::undo(ToolMain* tool)
{
	// Attempt to remove the scene object that we added
	bool removed = tool->getGraph()->removeSceneObject(m_sceneObject);
	if (!removed) {
		// TODO: Debug log an error!
	}
}

std::wstring AddNewSceneObjectCommand::get_label()
{
	return L"Added new scene object";
}


// Change selection command implementation .. 
ChangeSelectionCommand::ChangeSelectionCommand(int newId, int lastId)
	: m_lastSelectionId(lastId), m_newSelectionId(newId) {}

void ChangeSelectionCommand::execute(ToolMain* tool, bool asRedo)
{
	tool->setSelectionID(m_newSelectionId);
}

void ChangeSelectionCommand::undo(ToolMain* tool)
{
	tool->setSelectionID(m_lastSelectionId);
}

std::wstring ChangeSelectionCommand::get_label()
{
	return L"Selected object: " + std::to_wstring(m_newSelectionId);
}

// Delete scene object command implementation 
DeleteSceneObjectCommand::DeleteSceneObjectCommand(const SceneObject * obj)
{
	m_object = *obj;
}

void DeleteSceneObjectCommand::execute(ToolMain * tool, bool asRedo)
{
	tool->getGraph()->removeSceneObject(m_object);
}

void DeleteSceneObjectCommand::undo(ToolMain * tool)
{
	m_object = tool->getGraph()->insertSceneObject(std::move(m_object));
}

std::wstring DeleteSceneObjectCommand::get_label()
{
	return L"Deleted scene object";
}
