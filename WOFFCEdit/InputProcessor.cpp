#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
	ZeroMemory(&m_inputCommands, sizeof(InputCommands));
}

InputCommands* InputProcessor::getCommands()
{
	return &m_inputCommands;
}

const InputCommands& InputProcessor::tick()
{
	if (!m_inputCommands.ctrl) {
		// Map actions if their appropriate keybinds are down
		// and the control modifier is NOT down
		m_inputCommands.forward = m_keyArray['W'];
		m_inputCommands.back = m_keyArray['S'];
		m_inputCommands.left = m_keyArray['A'];
		m_inputCommands.right = m_keyArray['D'];
		m_inputCommands.rotateLeft = m_keyArray['Q'];
		m_inputCommands.rotateRight = m_keyArray['E'];
		m_inputCommands.down = m_keyArray['Z'];
		m_inputCommands.up = m_keyArray['X'];

		m_inputCommands.space = m_keyArray[VK_SPACE];
	}
	else {
		// Ctrl modifier is down, so check shortcuts!
		m_inputCommands.undo = m_keyArray['Z'] && !m_lastKeyArray['Z'];
		m_inputCommands.redo = m_keyArray['Y'] && !m_lastKeyArray['Y'];
		m_inputCommands.save = m_keyArray['S'] && !m_lastKeyArray['S'];
		m_inputCommands.wireframe = m_keyArray['W'] && !m_lastKeyArray['W'];
		m_inputCommands.duplicate = m_keyArray['D'] && !m_lastKeyArray['D'];
	}

	// Copy the keys from the this input frame
	// to the last input frame, so we can compare
	for (int i = 0; i < NUM_KEYS; i++)
		m_lastKeyArray[i] = m_keyArray[i];

	// calculate the mouse velocity from current mouse and last mouse position
	m_inputCommands.mouseVelocityX = (float)m_inputCommands.mouseX - m_lastMouseX;
	m_inputCommands.mouseVelocityY = (float)m_inputCommands.mouseY - m_lastMouseY;
	m_lastMouseX = m_inputCommands.mouseX;
	m_lastMouseY = m_inputCommands.mouseY;

	if (m_lastInputCommands.leftMouse == ClickState::DOWN && m_inputCommands.leftMouse == ClickState::DOWN) {
		m_inputCommands.leftMouse = ClickState::HELD;
	}
	if (m_lastInputCommands.rightMouse == ClickState::DOWN && m_inputCommands.rightMouse == ClickState::DOWN) {
		m_inputCommands.rightMouse = ClickState::HELD;
	}
	m_lastInputCommands = m_inputCommands;

	return m_inputCommands;
}

void InputProcessor::process_msg(const MSG * msg)
{
	switch (msg->message) {
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		m_inputCommands.mouseX = GET_X_LPARAM(msg->lParam);
		m_inputCommands.mouseY = GET_Y_LPARAM(msg->lParam);
		break;

	case WM_LBUTTONDOWN:
		m_inputCommands.leftMouse = ClickState::DOWN;
		break;
	case WM_LBUTTONUP:
		m_inputCommands.leftMouse = ClickState::UP;
		break;

	case WM_RBUTTONDOWN:
		m_inputCommands.rightMouse = ClickState::DOWN;
		break;
	case WM_RBUTTONUP:
		m_inputCommands.rightMouse = ClickState::UP;
		break;
	}

	m_inputCommands.ctrl = m_keyArray[VK_CONTROL];
	m_inputCommands.shift = m_keyArray[VK_SHIFT];
}
