#pragma once
#include<string>
#include "../nclgl/Mesh.h"
//#include "Mesh.h"

class Water :public Mesh {
public:
	Water(const std::string& name);
	~Water(void) {};

	Vector3 GetWaterSize()const { return WaterSize; }
protected:
	Vector3 WaterSize;
};
