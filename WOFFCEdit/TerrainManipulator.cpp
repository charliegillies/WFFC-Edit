#include "TerrainManipulator.h"

#include <d3d11.h>
#include <SimpleMath.h>

#include "InputCommands.h"
#include "DisplayChunk.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

Plane p(0, 1, 0, 0);

void TerrainManipulator::flatten(DisplayChunk* chunk)
{
	// TODO: 
	// Record this change to the history, so it can be reversed.
	chunk->Flatten();
}

void TerrainManipulator::manipulate(DisplayChunk * chunk, Camera * camera, const InputCommands * input)
{
	// Form a ray from the cameras position & direction
	Ray ray(camera->getPosition(), camera->getDirection());

	// then check if the ray intersects the plane!
	float distance;
	if (ray.Intersects(p, distance)) {
		// calculate the intersection point on the plane
		Vector3 point = ray.position + (ray.direction * distance);
		// the terrain is positioned from -64 to 64 (TERRAINRESOLUTION/2)
		// so check if our point is in range?
		int x = static_cast<int>(point.x / 4.0f) + (TERRAINRESOLUTION)/2;
		int y = static_cast<int>(point.z / 4.0f) + (TERRAINRESOLUTION)/2;

		if (x >= 0 && x < TERRAINRESOLUTION && y >= 0 && y < TERRAINRESOLUTION) {
			// in bounds! convert coordinates to 1d.. 
			int index = (TERRAINRESOLUTION * x) + y;
			chunk->SetCoordinateHeight(index, 50);
		}
	}

}


