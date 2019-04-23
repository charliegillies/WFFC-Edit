#include "History.h"
#include "Command.h"

void History::log(Command* cmd)
{
	// Execute the command, but not as a redo, as this is the first
	// time the command is being executed in the history
	cmd->execute(false);
	m_commandHistory.push_back(cmd);
}

std::wstring History::get_top_cmd_label()
{
	Command* top = m_commandHistory.size() > 0 ? m_commandHistory.back() : nullptr;
	return top == nullptr ? L"" : top->get_label();
}
