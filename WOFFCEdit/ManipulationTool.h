#pragma once

class SceneObject;
class InputCommands;

// This is the tool that handles the different forms of manipulation
// that can be performed by the editor according to editor mode
class ManipulationTool {

public:
	bool rotate(const InputCommands* input, SceneObject* selected);
	bool scale(const InputCommands* input, SceneObject* selected);
	bool translate(const InputCommands* input, SceneObject* selected);
};