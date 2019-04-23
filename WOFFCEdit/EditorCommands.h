#pragma once

#include "Command.h"
#include "SceneObject.h"

// We define the history commands here, as it helps with
// the compile time of other objects just needing to include
// the history header, instead of HistoryCommands

class AddNewSceneObjectCommand : public Command {
private:
	class ToolMain* m_tool;
	SceneObject m_sceneObject;

public:
	AddNewSceneObjectCommand(ToolMain* tool);

	// Inherited via Command
	virtual void execute(bool asRedo) override;
	virtual void undo() override;
	virtual std::wstring get_label() override;
};
