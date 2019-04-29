#pragma once

class DisplayChunk;
class Camera;
struct InputCommands;

class TerrainManipulator {
public:
	void flatten(DisplayChunk* chunk);
	void manipulate(DisplayChunk* chunk, Camera* camera, const InputCommands* input);

};
