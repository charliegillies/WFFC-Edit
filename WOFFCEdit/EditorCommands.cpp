#include "EditorCommands.h"

#include "ToolMain.h"

AddNewSceneObjectCommand::AddNewSceneObjectCommand(ToolMain * tool) : m_tool(tool) {}

void AddNewSceneObjectCommand::execute(bool asRedo)
{
	m_tool->createNewSceneObject();
}
void AddNewSceneObjectCommand::undo()
{

}
std::wstring AddNewSceneObjectCommand::get_label()
{
	return L"Added new scene object";
}

