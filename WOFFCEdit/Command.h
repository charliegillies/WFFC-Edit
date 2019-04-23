#pragma once

#include <string>

// Defines a command that can be performed in the editor
// and done/undone by the history.
struct Command {
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual std::wstring get_label() = 0;
};
