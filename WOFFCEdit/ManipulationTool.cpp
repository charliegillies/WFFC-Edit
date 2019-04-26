#include "ManipulationTool.h"

#include "InputCommands.h"
#include "SceneObject.h"

#include <d3d11.h>
#include "SimpleMath.h"

using Vector3 = DirectX::SimpleMath::Vector3;

ManipulationTool::ManipulationTool()
{
}

bool ManipulationTool::rotate(const InputCommands * input, SceneObject * selected)
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
	if (input->moveDown) {
		change += Vector3::Down;
	}
	if (input->moveUp) {
		change += Vector3::Up;
	}

	if (change.Length() > 0.0f) {
		rotation += change;
		selected->setRotation(rotation.x, rotation.y, rotation.z);
		return true;
	}
	return false;
}

bool ManipulationTool::scale(const InputCommands * input, SceneObject * selected)
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
	if (input->moveDown) {
		change += Vector3::Down;
	}
	if (input->moveUp) {
		change += Vector3::Up;
	}

	if (change.Length() > 0.0f) {
		scale += change;
		selected->setScale(scale.x, scale.y, scale.z);
		return true;
	}
	return false;
}

bool ManipulationTool::translate(const InputCommands * input, SceneObject * selected)
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
	if (input->moveDown) {
		movement += Vector3::Down;
	}
	if (input->moveUp) {
		movement += Vector3::Up;
	}

	if (movement.Length() > 0.0f) {
		position += movement;
		selected->setPosition(position.x, position.y, position.z);
		return true;
	}
	return false;
}
