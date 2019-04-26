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
		if (rotator == inactive) {
			rotator = processing;
			x = rotation.x; y = rotation.z; z = rotation.z;
		}

		rotation += change;
		selected->setRotation(rotation.x, rotation.y, rotation.z);
		return true;
	}

	if (rotator == processing) {
		rotator = inactive;
		history->log(TransformEdit::Rotate(selected, x, y, z, rotation.x, rotation.y, rotation.z));
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
		scale += change;
		selected->setScale(scale.x, scale.y, scale.z);
		return true;
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
		position += movement;
		selected->setPosition(position.x, position.y, position.z);
		return true;
	}
	return false;
}

TransformEdit * TransformEdit::Rotate(SceneObject * obj, float x, float y, float z, float x1, float y1, float z1)
{
	auto edit = new TransformEdit();
	edit->obj = obj;
	edit->mode = ROTATE;
	edit->bx = x; edit->by = y; edit->bz = z;
	edit->fx = x1; edit->fy = y1; edit->fz = z1;
	return edit;
}

void TransformEdit::execute(ToolMain* tool, bool asRedo)
{
	obj->setRotation(fx, fy, fz);
	tool->setDirty(true);
}

void TransformEdit::undo(ToolMain* tool)
{
	obj->setRotation(bx, by, bz);
	tool->setDirty(true);
}

std::wstring TransformEdit::get_label()
{
	return L"Edited transform";
}
