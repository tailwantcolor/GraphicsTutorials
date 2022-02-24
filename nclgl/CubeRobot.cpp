#include "CubeRobot.h"

CubeRobot::CubeRobot(Mesh* cube,Mesh* cylinder,Mesh* sphere) {
	//SetMesh(cube);
	//Uncomment if you want a local origin marker!
	/*SceneNode* trans = new SceneNode(cube, Vector4(1, 0, 0, 1));
	trans->SetModelScale(Vector3(0, 0, 0));
	trans->SetTransform(Matrix4::Translation(Vector3(4000, 35, 4000)) * Matrix4::Scale(Vector3(20, 20, 20)));
	AddChild(trans);*/

	SceneNode* body = new SceneNode(cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(3000, 450, 3000))*Matrix4::Scale(Vector3(10,10,10)));
	//body->SetTexture(texture= SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
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


	SceneNode* body1 = new SceneNode(cube, Vector4(1, 0, 0, 1));
	body1->SetModelScale(Vector3(10, 15, 5));
	body1->SetTransform(Matrix4::Translation(Vector3(3500, 550, 5500)) * Matrix4::Scale(Vector3(10, 10, 10)));
	//body->SetTexture(texture= SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	AddChild(body1);

	head1 = new SceneNode(sphere, Vector4(0, 1, 0, 1));
	head1->SetModelScale(Vector3(5, 5, 5));
	head1->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	body1->AddChild(head1);

	leftArm1 = new SceneNode(cylinder, Vector4(0, 1, 0, 1));
	leftArm1->SetModelScale(Vector3(3, -18, 3));
	leftArm1->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body1->AddChild(leftArm1);

	rightArm1 = new SceneNode(cylinder, Vector4(0, 0, 1, 1));
	rightArm1->SetModelScale(Vector3(3, -18, 3));
	rightArm1->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body1->AddChild(rightArm1);

	SceneNode* leftLeg1 = new SceneNode(cube, Vector4(0, 0, 1, 1));
	leftLeg1->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg1->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body1->AddChild(leftLeg1);

	SceneNode* rightleg1 = new SceneNode(cube, Vector4(0, 0, 1, 1));
	rightleg1->SetModelScale(Vector3(3, -17.5, 3));
	rightleg1->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body1->AddChild(rightleg1);


	/*SceneNode* hat = new SceneNode(cube, Vector4(0, 0, 1, 1));
	hat->SetModelScale(Vector3(2, 2, 2));
	hat->SetTransform(Matrix4::Translation(Vector3(0, 1, 0)));
	head->AddChild(hat);*/

	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	//rightleg->SetBoundingRadius(rightleg->GetWorldTransform().GetScalingVector().Length()/sqrt(2));
	rightleg->SetBoundingRadius(18.0f);


}
//CubeRobot::CubeRobot(Mesh* cube, Mesh* cylinder, Mesh* sphere) {
//	SceneNode* body1 = new SceneNode(cube, Vector4(1, 0, 0, 1));
//	body1->SetModelScale(Vector3(10, 15, 5));
//	body1->SetTransform(Matrix4::Translation(Vector3(5000, 450, 5000)) * Matrix4::Scale(Vector3(10, 10, 10)));
//	//body->SetTexture(texture= SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
//	AddChild(body1);
//
//	head1 = new SceneNode(sphere, Vector4(0, 1, 0, 1));
//	head1->SetModelScale(Vector3(5, 5, 5));
//	head1->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
//	body1->AddChild(head1);
//
//	leftArm1 = new SceneNode(cylinder, Vector4(0, 1, 0, 1));
//	leftArm1->SetModelScale(Vector3(3, -18, 3));
//	leftArm1->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
//	body1->AddChild(leftArm1);
//
//	rightArm1 = new SceneNode(cylinder, Vector4(0, 0, 1, 1));
//	rightArm1->SetModelScale(Vector3(3, -18, 3));
//	rightArm1->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
//	body1->AddChild(rightArm1);
//
//	SceneNode* leftLeg1 = new SceneNode(cube, Vector4(0, 0, 1, 1));
//	leftLeg1->SetModelScale(Vector3(3, -17.5, 3));
//	leftLeg1->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
//	body1->AddChild(leftLeg1);
//
//	SceneNode* rightleg1 = new SceneNode(cube, Vector4(0, 0, 1, 1));
//	rightleg1->SetModelScale(Vector3(3, -17.5, 3));
//	rightleg1->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
//	body1->AddChild(rightleg1);
//
//	body1->SetBoundingRadius(15.0f);
//	head->SetBoundingRadius(5.0f);
//
//	leftArm->SetBoundingRadius(18.0f);
//	rightArm->SetBoundingRadius(18.0f);
//
//	leftLeg->SetBoundingRadius(18.0f);
//	//rightleg->SetBoundingRadius(rightleg->GetWorldTransform().GetScalingVector().Length()/sqrt(2));
//	rightleg->SetBoundingRadius(18.0f);
//}

void CubeRobot::Update(float dt) {
	//transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));
	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	head1->SetTransform(head1->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)));
	leftArm1->SetTransform(leftArm1->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	rightArm1->SetTransform(rightArm1->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	SceneNode::Update(dt);
}

//如果scale从world Transform里面就有 应该要去存worldtransform的矩阵与自己的transform的scale矩阵然后GetScalingVector()去求长度s
