#pragma once
// Charlie Gillies
// 23-04-2019

#include <vector>
struct Command;
class ToolMain;

// The history is a list of the actions/commands performed by the user
// within the editor. We want the user to be able to use undo
// and redo items within the history.
class History {
private:
	std::vector<Command*> m_commandHistory;
	int m_index;
	ToolMain* m_tool;
	bool m_isDirty;

public:
	History(ToolMain* tool);

	void log(Command* cmd);
	bool undo();
	bool redo();

	bool isDirty() const;
	void setDirty(bool);

	int num_commands();

	std::wstring get_current_cmd_label();
};
