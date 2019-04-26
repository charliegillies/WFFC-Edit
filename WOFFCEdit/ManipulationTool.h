#pragma once

class SceneObject;
struct InputCommands;

// This is the tool that handles the different forms of manipulation
// that can be performed by the editor according to editor mode
class ManipulationTool {

public:
	ManipulationTool();

	bool rotate(const InputCommands* input, SceneObject* selected);
	bool scale(const InputCommands* input, SceneObject* selected);
	bool translate(const InputCommands* input, SceneObject* selected);
};