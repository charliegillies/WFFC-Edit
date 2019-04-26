#pragma once
// Charlie Gillies
// 23-04-2019

#include <vector>
struct Command;

// The history is a list of the actions/commands performed by the user
// within the editor. We want the user to be able to use undo
// and redo items within the history.
class History {
private:
	std::vector<Command*> m_commandHistory;
	int m_index;

public:
	History();

	void log(Command* cmd);
	bool undo();
	bool redo();

	int num_commands();

	std::wstring get_current_cmd_label();
};
