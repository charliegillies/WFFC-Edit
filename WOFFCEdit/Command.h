#pragma once

#include <string>

// Defines a command that can be performed in the editor
// and done/undone by the history. The pattern for this is outlined
// in game prog. patterns: 
// http://gameprogrammingpatterns.com/command.html
struct Command {
	virtual void execute(bool asRedo) = 0;
	virtual void undo() = 0;
	virtual std::wstring get_label() = 0;
};
