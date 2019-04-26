#pragma once

class History;
class SceneObject;
struct InputCommands;

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

	SceneObject* obj;

	static TransformEdit* Rotate(SceneObject* obj, float x, float y, float z, float x1, float y1, float z1);

	// Inherited via Command
	virtual void execute(ToolMain* tool, bool asRedo) override;
	virtual void undo(ToolMain* tool) override;
	virtual std::wstring get_label() override;
};

// This is the tool that handles the different forms of manipulation
// that can be performed by the editor according to editor mode
class ManipulationTool {
private:
	enum {
		inactive, processing, stopped
	} rotator, scalar, translator;

	float x, y, z;

public:
	ManipulationTool();

	bool rotate(const InputCommands* input, SceneObject* selected, History* history);
	bool scale(const InputCommands* input, SceneObject* selected, History* history);
	bool translate(const InputCommands* input, SceneObject* selected, History* history);
};