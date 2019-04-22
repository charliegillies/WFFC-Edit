#pragma once

enum class MouseState {
	UP = 0, DOWN = 1, HELD = 2
};

struct InputCommands {
	// Keyboard input command results
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotateRight, rotateLeft;
	bool moveDown, moveUp;

	// Mouse input - right/left.
	MouseState LeftMouse;
	MouseState RightMouse;


};
