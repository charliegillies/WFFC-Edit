#pragma once

#include "Command.h"

// We define the history commands here, as it helps with
// the compile time of other objects just needing to include
// the history header, instead of HistoryCommands

class AddNewSceneObjectCommand : public Command {
private:
	class ToolMain* m_tool;

public:
	AddNewSceneObjectCommand(ToolMain* tool);

	// Inherited via Command
	virtual void execute(bool asRedo) override;
	virtual void undo() override;
	virtual std::wstring get_label() override;
};
