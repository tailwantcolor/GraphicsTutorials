#include "../nclgl/spotlight.h"
#include"../nclgl/Matrix4.h"
#include <iostream>
#include <cmath>
void Spotlight::UpdatespotLight(float dt) {
	Vector3 exist = Vector3((lookingat-position).x, (lookingat - position).y, (lookingat-position).z);
	//Vector3 trans = Matrix4::Rotation(30.0f*dt,Vector3(0,1,0)) * exist;
	//Vector3 trans_xz = Vector3(trans.x, 0.0f, trans.y);
	//lookingat = Vector3(position.x,lookingat.y,position.y) + trans_xz;
	//lookingat = Matrix4::Rotation(15.0f * dt, Vector3(0, 1, 0)) * lookingat;
	//float a = exist.Length();
	//float arcsin = acos((lookingat - position).x / a);
	//float arccos = asin((lookingat - position).z / a);

    Vector4 trans = Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)) * Vector4(exist.x,exist.y,exist.z,1.0);

	lookingat = position + Vector3(trans.x, trans.y, trans.z);
	//lookingat = Vector3(a*cos(3.0f*dt+arcsin)+position.x,lookingat.y,a*sin(3.0f*dt+arccos)+position.y);
	midDir = position - lookingat;
}