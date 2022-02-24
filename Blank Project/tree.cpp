#include "tree.h"

Tree::Tree(Mesh* tree) {
	tree1 = new SceneNode(tree, Vector4(1, 0, 0, 1));
	tree1->SetModelScale(Vector3(10, 15, 5));
	tree1->SetTransform(Matrix4::Translation(Vector3(3000, 450, 5000)) * Matrix4::Scale(Vector3(10, 10, 10)));
	//body->SetTexture(texture= SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	AddChild(tree1);

	tree2 = new SceneNode(tree, Vector4(0, 1, 0, 1));
	tree2->SetModelScale(Vector3(5, 5, 5));
	tree2->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	tree2->AddChild(tree1);

	tree3 = new SceneNode(tree, Vector4(0, 1, 0, 1));
	tree3->SetModelScale(Vector3(3, -18, 3));
	tree3->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	tree3->AddChild(tree1);
}