#include "Camera.h"

#include "InputCommands.h"
#include <string>

typedef DirectX::SimpleMath::Vector3 Vector3;

Camera::Camera()
{
	//functional
	m_moveSpeed = 0.30;
	m_camRotRate = 3.0;

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

void Camera::handleInput(InputCommands const & input)
{
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	if (input.rotateRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (input.rotateLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (input.forward) {
		m_camPosition += m_camLookDirection * m_moveSpeed;
	}
	if (input.back) {
		m_camPosition -= m_camLookDirection * m_moveSpeed;
	}
	if (input.right) {
		m_camPosition += m_camRight * m_moveSpeed;
	}
	if (input.left) {
		m_camPosition -= m_camRight * m_moveSpeed;
	}
	if (input.moveDown) {
		m_camPosition -= Vector3::UnitY * m_moveSpeed;
	}
	if (input.moveUp) {
		m_camPosition += Vector3::UnitY * m_moveSpeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
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
	return L"Cam X: " + std::to_wstring(m_camPosition.x) + L"Cam Z: " + std::to_wstring(m_camPosition.z);
}
