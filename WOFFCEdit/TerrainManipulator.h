#pragma once

class DisplayChunk;
class Camera;
struct InputCommands;

// Utility for manipulating the terrain, the 'DisplayChunk'
class TerrainManipulator {
public:
	void flatten(DisplayChunk* chunk);
	void sculpt(DisplayChunk* chunk, const Camera* camera, const InputCommands* input);
};
