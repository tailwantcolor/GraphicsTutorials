#include "CubeRobot.h"

CubeRobot::CubeRobot(Mesh* cube) {
	//SetMesh(cube);
	//Uncomment if you want a local origin marker!

	SceneNode* body = new SceneNode(cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0))/**Matrix4::Scale(Vector3(20,20,20))*/);
	AddChild(body);

	head = new SceneNode(cube, Vector4(0, 1, 0, 1));
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	body->AddChild(head);

	leftArm = new SceneNode(cube, Vector4(0, 1, 0, 1));
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1));
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftLeg);

	SceneNode* rightleg = new SceneNode(cube, Vector4(0, 0, 1, 1));
	rightleg->SetModelScale(Vector3(3, -17.5, 3));
	rightleg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightleg);

	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	//rightleg->SetBoundingRadius(rightleg->GetWorldTransform().GetScalingVector().Length()/sqrt(2));
	rightleg->SetBoundingRadius(18.0f);
}

void CubeRobot::Update(float dt) {
	transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));
	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	SceneNode::Update(dt);
}

//如果scale从world Transform里面就有 应该要去存worldtransform的矩阵与自己的transform的scale矩阵然后GetScalingVector()去求长度s
