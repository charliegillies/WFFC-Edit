#pragma once

#include "InputCommands.h"

#include <afxext.h>

class InputProcessor {
private:
	static const int NUM_KEYS = 256;
	
	char m_keyArray[NUM_KEYS], m_lastKeyArray[NUM_KEYS];
	float m_lastMouseX, m_lastMouseY;

	InputCommands m_inputCommands;

public:
	const InputCommands& tick();
	void process_msg(const MSG* msg);
};
