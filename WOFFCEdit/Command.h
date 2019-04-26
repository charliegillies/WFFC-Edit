#pragma once

#include <string>

class ToolMain;

// Defines a command that can be performed in the editor
// and done/undone by the history. The pattern for this is outlined
// in game prog. patterns: http://gameprogrammingpatterns.com/command.html
struct Command {
	// Performs the execution of the command.
	// Optional boolean flag to indicate if the execution
	// is a redo, or the first execution. Use this for edge cases.
	virtual void execute(ToolMain* tool, bool asRedo) = 0;
	
	// The opposite of execute - tidy up/remove the effect that
	// was performed inside of execute()
	virtual void undo(ToolMain* tool) = 0;
	
	// Get the label of the editor command.
	// This is a string identifier that shows up in the editor
	// history, so the user is aware of what undo/redo was done.
	virtual std::wstring get_label() = 0;
};
