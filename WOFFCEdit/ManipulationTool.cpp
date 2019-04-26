#include "ManipulationTool.h"

#include "InputCommands.h"
#include "SceneObject.h"
#include "ToolMain.h"
#include "History.h"

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

ManipulationTool::ManipulationTool()
{
	translator = inactive;
	rotator = inactive;
	scalar = inactive;
	x = 0; y = 0; z = 0;
}

bool ManipulationTool::rotate(const InputCommands* input, SceneObject* selected, History* history)
{
	// Manipulate object rotation according to input
	Vector3 rotation = { selected->rotX, selected->rotY, selected->rotZ };
	Vector3 change = Vector3::Zero;

	if (input->left) {
		change += Vector3::Left;
	}
	if (input->right) {
		change += Vector3::Right;
	}
	if (input->forward) {
		change += Vector3::Forward;
	}
	if (input->back) {
		change += Vector3::Backward;
	}
	if (input->down) {
		change += Vector3::Down;
	}
	if (input->up) {
		change += Vector3::Up;
	}

	if (change.Length() > 0.0f) {
		// if the manipulator hasn't been used, cache the original position
		// for the undo history
		if (rotator == inactive) {
			rotator = processing;
			x = rotation.x; y = rotation.y; z = rotation.z;
		}

		rotation += change;
		selected->setRotation(rotation.x, rotation.y, rotation.z);
		return true;
	}

	if (rotator == processing) {
		rotator = inactive;
		history->log(TransformEdit::Rotate(selected->ID, x, y, z, rotation.x, rotation.y, rotation.z));
	}

	return false;
}

bool ManipulationTool::scale(const InputCommands* input, SceneObject* selected, History* history)
{
	// Manipulate scale according to the input
	Vector3 scale = { selected->scaX, selected->scaY, selected->scaZ };
	Vector3 change = Vector3::Zero;

	if (input->left) {
		change += Vector3::Left;
	}
	if (input->right) {
		change += Vector3::Right;
	}
	if (input->forward) {
		change += Vector3::Forward;
	}
	if (input->back) {
		change += Vector3::Backward;
	}
	if (input->down) {
		change += Vector3::Down;
	}
	if (input->up) {
		change += Vector3::Up;
	}

	if (change.Length() > 0.0f) {
		// if the manipulator hasn't been used, cache the original position
		// for the undo history
		if (scalar == inactive) {
			scalar = processing;
			x = scale.x; y = scale.y; z = scale.z;
		}

		scale += change;
		selected->setScale(scale.x, scale.y, scale.z);
		return true;
	}

	if (scalar == processing) {
		scalar = inactive;
		history->log(TransformEdit::Scale(selected->ID, x, y, z, scale.x, scale.y, scale.z));
	}
	return false;
}

bool ManipulationTool::translate(const InputCommands * input, SceneObject * selected, History* history)
{
	// Manipulate position according to the input
	Vector3 position = { selected->posX, selected->posY, selected->posZ };
	Vector3 movement = Vector3::Zero;

	if (input->left) {
		movement += Vector3::Left;
	}
	if (input->right) {
		movement += Vector3::Right;
	}
	if (input->forward) {
		movement += Vector3::Forward;
	}
	if (input->back) {
		movement += Vector3::Backward;
	}
	if (input->down) {
		movement += Vector3::Down;
	}
	if (input->up) {
		movement += Vector3::Up;
	}

	if (movement.Length() > 0.0f) {
		// if the manipulator hasn't been used, cache the original position
		// for the undo history
		if (translator == inactive) {
			translator = processing;
			x = position.x; y = position.y; z = position.z;
		}
		position += movement;
		selected->setPosition(position.x, position.y, position.z);
		return true;
	}

	if (translator == processing) {
		translator = inactive;
		history->log(TransformEdit::Translate(selected->ID, x, y, z, position.x, position.y, position.z));
	}
	return false;
}

void TransformEdit::setValues(SceneObject * obj, float x, float y, float z)
{
	switch (mode) {
	case ROTATE:
		obj->setRotation(x, y, z);
		break;
	case TRANSLATE:
		obj->setPosition(x, y, z);
		break;
	case SCALE:
		obj->setScale(x, y, z);
		break;
	}
}

TransformEdit * TransformEdit::Rotate(int objId, float x, float y, float z, float x1, float y1, float z1)
{
	auto edit = new TransformEdit();
	edit->objID = objId;
	edit->mode = ROTATE;
	edit->bx = x; edit->by = y; edit->bz = z;
	edit->fx = x1; edit->fy = y1; edit->fz = z1;
	return edit;
}

TransformEdit * TransformEdit::Scale(int objId, float x, float y, float z, float x1, float y1, float z1)
{
	auto edit = new TransformEdit();
	edit->objID = objId;
	edit->mode = SCALE;
	edit->bx = x; edit->by = y; edit->bz = z;
	edit->fx = x1; edit->fy = y1; edit->fz = z1;
	return edit;
}

TransformEdit * TransformEdit::Translate(int objId, float x, float y, float z, float x1, float y1, float z1)
{
	auto edit = new TransformEdit();
	edit->objID = objId;
	edit->mode = TRANSLATE;
	edit->bx = x; edit->by = y; edit->bz = z;
	edit->fx = x1; edit->fy = y1; edit->fz = z1;
	return edit;
}

void TransformEdit::execute(ToolMain* tool, bool asRedo)
{
	SceneObject* target = tool->getGraph()->getObjectById(objID);
	if (target == nullptr) return;

	setValues(target, fx, fy, fz);
	tool->setDirty(true);
}

void TransformEdit::undo(ToolMain* tool)
{
	SceneObject* target = tool->getGraph()->getObjectById(objID);
	if (target == nullptr) return;

	setValues(target, bx, by, bz);
	tool->setDirty(true);
}

std::wstring TransformEdit::get_label()
{
	return L"Edited transform";
}
