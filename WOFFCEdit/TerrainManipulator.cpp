#include "TerrainManipulator.h"
#include "DisplayChunk.h"

void TerrainManipulator::flatten(DisplayChunk* chunk)
{
	// TODO: 
	// Record this change to the history, so it can be reversed.
	chunk->Flatten();
}

void TerrainManipulator::manipulate(DisplayChunk * chunk, Camera * camera, InputCommands * input)
{

}


