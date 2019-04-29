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

void TerrainManipulator::sculpt(DisplayChunk* chunk, const Camera* camera, const InputCommands* input)
{
	bool raise = input->leftMouse != ClickState::UP;
	bool lower = raise && input->shift;
	//if (!raise && !lower) return;

	// hard coding the numbers as a test!
	//Matrix mat;
	//auto r = camera->screenToWorld(1235, 657, mat);

	// Form a ray from the cameras position & direction
	Ray ray(camera->getPosition(), camera->getDirection());

	// then check if the ray intersects the plane!
	float distance;
	if (ray.Intersects(p, distance)) {
		const float scalar = chunk->getResolutionScale();

		// calculate the intersection point on the plane
		Vector3 point = ray.position + (ray.direction * distance);
		// terrain is positioned from -TERRAINRES/2:TERRAINRES/2. Set back!
		int x = std::round(point.z / scalar) + (TERRAINRESOLUTION)/2;
		int y = std::round(point.x / scalar) + (TERRAINRESOLUTION)/2;

		if (x >= 0 && x < TERRAINRESOLUTION && y >= 0 && y < TERRAINRESOLUTION) {
			// in bounds! convert coordinates to 1d.. 
			int index = (TERRAINRESOLUTION * x) + y;
			if (lower)
				chunk->LowerCoordinateHeight(index);
			else if (raise)
				chunk->RaiseCoordinateHeight(index);
		}
	}

}


