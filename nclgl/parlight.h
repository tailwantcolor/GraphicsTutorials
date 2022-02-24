#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Parlight {
public:
	Parlight() {};
	Parlight(const Vector3& position, const Vector4& colour,const Vector3& lookingat) {
		this->position = position;
		this->colour = colour;
		this->lookingat = lookingat;
		direction = position - lookingat;
	}

	~Parlight(void) {};

	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }

	Vector3 GetLookingat() const { return lookingat; }
	void SetLookingat(const Vector3& val) { lookingat = val; }

	Vector3 GetDirection() const { return direction; }
	void SetDirection(const Vector3& val) { direction = val; }


	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }

	void UpdateParLight(float dt);
	void UpdateShadow(float dt);
protected:
	Vector3 position;
	Vector3 lookingat;
	Vector4 colour;
	Vector3 direction;
};