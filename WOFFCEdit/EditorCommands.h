#pragma once

#include "Command.h"
#include "SceneObject.h"

// We define the history commands here, as it helps with
// the compile time of other objects just needing to include
// the history header, instead of HistoryCommands

class ToolMain;

class AddNewSceneObjectCommand : public Command {
private:
	ToolMain* m_tool;
	SceneObject m_sceneObject;

public:
	AddNewSceneObjectCommand();

	// Inherited via Command
	virtual void execute(ToolMain* tool, bool asRedo) override;
	virtual void undo(ToolMain* tool) override;
	virtual std::wstring get_label() override;
};

class ChangeSelectionCommand : public Command {
private:
	ToolMain* m_tool;
	int m_newSelectionId, m_lastSelectionId;

public:
	ChangeSelectionCommand(int newId, int lastId);

	// Inherited via Command
	virtual void execute(ToolMain* tool, bool asRedo) override;
	virtual void undo(ToolMain* tool) override;
	virtual std::wstring get_label() override;
};

class DeleteSceneObjectCommand : public Command {
private:
	SceneObject m_object;

public:
	DeleteSceneObjectCommand(const SceneObject* obj);

	// Inherited via Command
	virtual void execute(ToolMain * tool, bool asRedo) override;
	virtual void undo(ToolMain * tool) override;
	virtual std::wstring get_label() override;
};
