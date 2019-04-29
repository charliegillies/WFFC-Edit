#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

struct InputCommands;
class SceneObject;

class Camera {
private:
	DirectX::SimpleMath::Vector3 m_camPosition;
	DirectX::SimpleMath::Vector3 m_camOrientation;
	DirectX::SimpleMath::Vector3 m_camLookAt;
	DirectX::SimpleMath::Vector3 m_camLookDirection;
	DirectX::SimpleMath::Vector3 m_camRight;
	float m_camRotRate;
	float m_moveSpeed;

	HWND m_hwnd;

public:
	Camera();

	void setHWND(HWND hwnd);
	void handleInput(InputCommands const& commands, const float deltaTime, const SceneObject* so);
	bool moveTowards(const SceneObject* obj, const float time);

	DirectX::SimpleMath::Vector3 screenToWorld(int width, int height, DirectX::SimpleMath::Matrix worldMatrix) const;

	DirectX::SimpleMath::Matrix createViewMatrix();

	DirectX::SimpleMath::Vector3 getDirection() const;
	DirectX::SimpleMath::Vector3 getPosition() const;
	DirectX::SimpleMath::Vector3 getLookAt() const;
	DirectX::SimpleMath::Vector3 getUp() const;

	std::wstring getDebugPosition() const;
};
