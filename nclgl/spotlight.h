
#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Spotlight {
public:
	Spotlight() {};
	Spotlight(const Vector3& position, const Vector4& colour, const Vector3& lookingat, float cosangle, float radius) {
		this->position = position;
		this->colour = colour;
		this->lookingat = lookingat;
		this->cosangle = cosangle;
		this->radius = radius;
		midDir = position -lookingat ;
	}

	~Spotlight(void) {};

	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }

	Vector3 Getlookingat() const { return lookingat; }
	void Setlookingat(const Vector3& val) { lookingat = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }

	Vector3 GetmidDir() const { return midDir; }
	void SetmidDir(const Vector3& val) { midDir = val; }

	float GetRadius()const { return radius; }
	void SetRadius(float val) { radius = val; }

	float Getcosangle()const { return cosangle; }
	void Setcosangle(float val) { cosangle = val; }

	void UpdatespotLight(float dt);
	void UpdateShadow(float dt);
protected:
	Vector3 position;
	float cosangle;
	Vector4 colour;
	Vector3 lookingat;
	float radius;
	Vector3 midDir;
};