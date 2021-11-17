#include "Light.h"
#include "Matrix4.h"

class Matrix4;
void Light::UpdateLight(float dt) {
	position = Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)) * position;
}