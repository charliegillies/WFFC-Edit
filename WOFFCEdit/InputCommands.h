#pragma once

enum class ClickState {
	UP = 0, DOWN = 1, HELD = 2
};

struct InputCommands {
	// Keyboard input command results
	bool forward, back, right, left;
	bool rotateRight, rotateLeft;
	bool down, up;
	bool space;

	// Input shortcuts for actions
	bool undo, redo, save, wireframe, duplicate;
	bool translate, rotate, scale, camera, edit;
	bool del;

	// Modifiers
	bool ctrl, shift;

	// Mouse input - right/left.
	ClickState leftMouse;
	ClickState rightMouse;

	// Mouse position 
	int mouseX, mouseY;
	// Mouse velocity
	float mouseVelocityX, mouseVelocityY;
};
