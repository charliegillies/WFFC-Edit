#include "History.h"
#include "Command.h"

History::History() : m_index(0)
{
}

void History::log(Command* cmd)
{
	if (m_commandHistory.size() > 0) {
		// Delete all recorded history between index+1:size
		for (int i = m_index + 1, right = m_commandHistory.size(); i < right; i++) {
			Command* cmd = m_commandHistory.at(i);
			delete cmd;
		}
		// now remove those elements from the vector
		// TODO: Remove to STL to tidy this up.
		for (int i = m_index + 1, right = m_commandHistory.size(); i < right; i++) {
			m_commandHistory.pop_back();
		}
	}

	// Execute the command, but not as a redo, as this is the first
	// time the command is being executed in the history
	cmd->execute(false);
	m_commandHistory.push_back(cmd);

	// Move the index to be equal to the 
	m_index = m_commandHistory.size() - 1;
}

bool History::undo()
{
	if (m_commandHistory.size() == 0) return false;
	// First check. Can we go back?
	if ((m_index - 1) < -1) return false;

	// Undo the last command that was executed
	Command* last = m_commandHistory[m_index];
	last->undo();
	// and move the index back one
	m_index -= 1;

	return true;
}

bool History::redo()
{
	if (m_commandHistory.size() == 0) return false;
	// First check. Can we go forward?
	if ((m_index + 1) >= m_commandHistory.size()) return false;

	m_index += 1;
	Command* next = m_commandHistory[m_index];
	next->execute(true);
	return true;
}

int History::num_commands()
{
	return m_commandHistory.size();
}

std::wstring History::get_current_cmd_label()
{
	// wrap the index so it does not go out of bounds
	int index = m_index;
	if (index < 0) index = 0;

	Command* current = m_commandHistory.size() > 0 ? m_commandHistory[index] : nullptr;
	return current == nullptr ? L"" : current->get_label();
}
