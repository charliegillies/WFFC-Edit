#pragma once
// Charlie Gillies
// 23-04-2019

#include <vector>
class Command;

// The history is a list of the actions/commands performed by the user
// within the editor. We want the user to be able to use undo
// and redo items within the history.
class History {
private:
	std::vector<Command*> m_commandHistory;

public:
	void log(Command* cmd);

	std::wstring get_top_cmd_label();
};
