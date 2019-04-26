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
	return false;
}

bool ManipulationTool::scale(const InputCommands * input, SceneObject * selected)
{
	return false;
}

bool ManipulationTool::translate(const InputCommands * input, SceneObject * selected)
{
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
