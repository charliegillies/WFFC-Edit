#pragma once

class SceneObject;
struct InputCommands;

/*
#include "Command.h"
// The command that represents a transform change
// and allows redo/undo of that edit.
class TransformEdit : public Command {
private:
	enum {
		SCALE, ROTATE, TRANSLATE
	} mode;

public:
	// Beginning x/y/z
	float bx, by, bz;
	// Final x/y/z
	float fx, fy, fz;
};
*/

// This is the tool that handles the different forms of manipulation
// that can be performed by the editor according to editor mode
class ManipulationTool {
private:


public:
	ManipulationTool();

	bool rotate(const InputCommands* input, SceneObject* selected);
	bool scale(const InputCommands* input, SceneObject* selected);
	bool translate(const InputCommands* input, SceneObject* selected);
};