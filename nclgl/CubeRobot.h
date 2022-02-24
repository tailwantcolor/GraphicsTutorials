#pragma once
#include "../nclgl/SceneNode.h"

class CubeRobot : public SceneNode {
public:
	CubeRobot(Mesh* cube, Mesh* cylinder, Mesh* sphere);
	//CubeRobot(Mesh* cube, Mesh* cyclinder, Mesh* sphere);
	~CubeRobot(void) {};
	void Update(float dt) override;
protected:
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
	SceneNode* head1;
	SceneNode* leftArm1;
	SceneNode* rightArm1;
	//SceneNode* body;
};



