#include "Light.h"
#include "Matrix4.h"

class Matrix4;
void Light::UpdateLight(float dt) {
	position = Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)) * position;
}

void Light::UpdateShadow(float dt) {
	float sceneTime = 0.0f;
	sceneTime += dt;
	position = Matrix4::Translation(Vector3(sin(sceneTime*5), 0.0f, sin(sceneTime*5))) * position;
}