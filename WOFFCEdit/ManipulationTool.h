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

	// Beginning x/y/z
	float bx, by, bz;
	// Final x/y/z
	float fx, fy, fz;
	int objID;

	void setValues(SceneObject* obj, float x, float y, float z);

public:
	// Static factory methods for each transform tool
	static TransformEdit* Rotate(int objId, float x, float y, float z, float x1, float y1, float z1);
	static TransformEdit* Scale(int objId, float x, float y, float z, float x1, float y1, float z1);
	static TransformEdit* Translate(int objId, float x, float y, float z, float x1, float y1, float z1);

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
		inactive, processing
	} rotator, scalar, translator;

	float x, y, z;

public:
	ManipulationTool();

	bool rotate(const InputCommands* input, SceneObject* selected, History* history);
	bool scale(const InputCommands* input, SceneObject* selected, History* history);
	bool translate(const InputCommands* input, SceneObject* selected, History* history);
};