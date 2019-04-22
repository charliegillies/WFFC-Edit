#pragma once

enum class ClickState {
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
	ClickState LeftMouse;
	ClickState RightMouse;

	// Mouse position and mouse movement velocity
	int mouseX, mouseY;
	float mouseVelocityX, mouseVelocityY;
};
