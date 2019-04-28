#include "Camera.h"

#include "InputCommands.h"
#include "SceneObject.h"
#include <string>

typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector2 Vector2;

Camera::Camera()
{
	//functional
	m_moveSpeed = 4.0;
	m_camRotRate = 10.0f;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}

void Camera::handleInput(InputCommands const & input, const float deltaTime)
{
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	float moveSpeed = m_moveSpeed;
	if (input.shift) moveSpeed *= 1.5f;
	moveSpeed *= deltaTime;

	float rotateSpeed = m_camRotRate;
	if (input.shift) rotateSpeed *= 1.5f;
	rotateSpeed *= deltaTime;

	if (input.rotateRight) {
		m_camOrientation.y -= m_camRotRate * rotateSpeed;
	}
	if (input.rotateLeft) {
		m_camOrientation.y += m_camRotRate * rotateSpeed;
	}

	// Allow right click & drag to change our camera rotation
	// this needs to be improved when we allow rotation around 
	// the other axis as well as the y axis.
	if (input.rightMouse == ClickState::DOWN) {
		Vector2 mVelocity = Vector2(input.mouseVelocityX, input.mouseVelocityY);
		mVelocity.Normalize();
		mVelocity = mVelocity * rotateSpeed;
		m_camOrientation += Vector3(0.0f, mVelocity.x, 0.0f);
	}

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (input.forward) {
		m_camPosition += m_camLookDirection * moveSpeed;
	}
	if (input.back) {
		m_camPosition -= m_camLookDirection * moveSpeed;
	}
	if (input.right) {
		m_camPosition += m_camRight * moveSpeed;
	}
	if (input.left) {
		m_camPosition -= m_camRight * moveSpeed;
	}
	if (input.down) {
		m_camPosition -= Vector3::UnitY * moveSpeed;
	}
	if (input.up) {
		m_camPosition += Vector3::UnitY * moveSpeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}

bool Camera::moveTowards(const SceneObject * obj)
{
	Vector3 target = Vector3(obj->posX, obj->posY, obj->posZ);
	Vector3 position = m_camPosition;

	// do not move closer than 1 unit
	if (Vector3::Distance(target, position) <= 1.0f) {
		return true;
	}

	// move towards .. 
	Vector3 delta = target - position;
	delta.Normalize();
	m_camPosition += delta * m_moveSpeed;

	// now rotate to look at it .. 


	return false;
}

DirectX::SimpleMath::Vector3 Camera::screenToWorld(HWND hwnd, float x, float y, int width, int height, DirectX::SimpleMath::Matrix worldMatrix)
{
	DirectX::SimpleMath::Vector3 cursorPosition;
	POINT p;
	if (GetCursorPos(&p)) {
		cursorPosition.x = static_cast<float>(p.x);
		cursorPosition.y = static_cast<float>(p.y);
	}
	if (ScreenToClient(hwnd, &p)) {
		cursorPosition.x = static_cast<float>(p.x);
		cursorPosition.y = static_cast<float>(p.y);
	}
	cursorPosition.z = 1.0f;

	DirectX::SimpleMath::Matrix dir;
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	// Calculate the current cursor position based on the screen width and height.
	cursorPosition.x = (cursorPosition.x / halfWidth) - 1.0f;
	cursorPosition.y = (cursorPosition.y / halfHeight) - 1.0f;

	// Calculate the position of the cursor in the 3D world and return it.
	DirectX::SimpleMath::Matrix position3D;
	position3D = position3D.CreateTranslation(m_camPosition.x - (cursorPosition.x * (1.0f / halfWidth)), m_camPosition.y - (cursorPosition.y * (1.0f / halfHeight)), m_camPosition.z - (cursorPosition.x * (1.0f / halfWidth)));
	dir = worldMatrix * position3D;
	return dir.Translation();
}

DirectX::SimpleMath::Matrix Camera::createViewMatrix()
{
	return DirectX::SimpleMath::Matrix::CreateLookAt(getPosition(), getLookAt(), getUp());
}

DirectX::SimpleMath::Vector3 Camera::getPosition() const
{
	return m_camPosition;
}

DirectX::SimpleMath::Vector3 Camera::getLookAt() const
{
	return m_camLookAt; 
}

DirectX::SimpleMath::Vector3 Camera::getUp() const
{
	return Vector3::UnitY;
}

std::wstring Camera::getDebugPosition() const
{
	return L"Cam X: " + std::to_wstring(m_camPosition.x) + L" Cam Z: " + std::to_wstring(m_camPosition.z);
}
