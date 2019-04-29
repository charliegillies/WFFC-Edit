#include "Camera.h"

#include "InputCommands.h"
#include "SceneObject.h"
#include <string>

typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector2 Vector2;
typedef DirectX::SimpleMath::Matrix Matrix;

Camera::Camera()
{
	// movement and camera rotation speed
	m_moveSpeed = 4.0;
	m_camRotRate = 10.0f;

	// initial camera position
	m_camPosition = Vector3(0.0f, 3.7f, -3.5f);

	m_camLookDirection = Vector3::Zero;
	m_camOrientation = Vector3::Zero;
	m_camLookAt = Vector3::Zero;
	m_camRight = Vector3::Zero;
}

void Camera::handleInput(InputCommands const & input, const float deltaTime)
{
	// no input focus? don't process the commands.
	if (input.lostFocus) return;

	static const float shift_speed_mult = 1.5f;
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	float moveSpeed = m_moveSpeed;
	if (input.shift) moveSpeed *= shift_speed_mult;
	moveSpeed *= deltaTime;

	float rotateSpeed = m_camRotRate;
	if (input.shift) rotateSpeed *= shift_speed_mult;
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
	if (input.rightMouse != ClickState::UP) {
		Vector2 mVelocity = Vector2(input.mouseVelocityX, input.mouseVelocityY);
		mVelocity.Normalize();
		mVelocity = mVelocity * rotateSpeed * 12.5f;
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

DirectX::SimpleMath::Vector3 Camera::screenToWorld(HWND hwnd, int width, int height, DirectX::SimpleMath::Matrix worldMatrix)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);
	Vector3 cursorPosition = Vector3(p.x, p.y, 1.0f);

	// Convert the coordinates to normalised device coordinates (-1:1)
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	cursorPosition.x = (cursorPosition.x / hw) - 1.0f;
	cursorPosition.y = (cursorPosition.y / hh) - 1.0f;

	// Calculate the position of the cursor in our 3D world
	auto cursorWorld = Matrix::CreateTranslation(
		m_camPosition.x - (cursorPosition.x * (1.0f / hw)), 
		m_camPosition.y - (cursorPosition.y * (1.0f / hh)), 
		m_camPosition.z - (cursorPosition.x * (1.0f / hw)));
	Matrix dir = worldMatrix * cursorWorld;
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
	return L"X: " + std::to_wstring(m_camPosition.x) 
		+ L" Y: " + std::to_wstring(m_camPosition.y) 
		+ L" Z: " + std::to_wstring(m_camPosition.z);
}
