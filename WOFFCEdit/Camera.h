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

public:
	Camera();

	void handleInput(InputCommands const& commands, const float deltaTime);
	bool moveTowards(const SceneObject* obj);

	DirectX::SimpleMath::Vector3 screenToWorld(HWND hwnd, int width, int height, DirectX::SimpleMath::Matrix worldMatrix);

	DirectX::SimpleMath::Matrix createViewMatrix();

	DirectX::SimpleMath::Vector3 getPosition() const;
	DirectX::SimpleMath::Vector3 getLookAt() const;
	DirectX::SimpleMath::Vector3 getUp() const;

	std::wstring getDebugPosition() const;
};
