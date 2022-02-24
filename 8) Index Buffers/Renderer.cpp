#include "Renderer.h"
#include"../nclgl/Camera.h"
#include "HeightMap.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"

Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	heightMap = new HeightMap(TEXTUREDIR"noise.png");
	camera = new Camera(-40, 270, Vector3());

	cube = Mesh::LoadFromMeshFile("offsetCubeY.msh");
	root = new SceneNode();
	root->AddChild(new CubeRobot(cube));
	Vector3 dimensions = heightMap->GetHeightmapSize();
	camera->SetPosition(dimensions * Vector3(0.5, 2, 0.5));

	shader = new Shader("TexturedVertex.glsl", "texturedfragment.glsl");

	shader_cube = new Shader("SceneVertex.glsl", "SceneFragment.glsl");

	if (!shader->LoadSuccess()) {
		return;
	}

	if (!shader_cube->LoadSuccess()) {
		return;
	}

	terrainTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);//4->0 ±¨´í

	if (!terrainTex) {
		return;
	}

	SetTextureRepeating(terrainTex, true);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	init =true;
}

Renderer::~Renderer(void) {
	delete heightMap;
	delete camera;
	delete shader;
	delete root;
	delete cube;
	delete shader_cube;
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(dt);
}

void Renderer::RenderScene() {
	//glPolygonMode(GL_FRONT, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	heightMap->Draw();
	BindShader(shader_cube);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(shader_cube->GetProgram(), "diffuseTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	DrawNode(root);

}

void Renderer::DrawNode(SceneNode* n) {
	//const Vector4 a= Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(shader_cube->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(shader_cube->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(shader_cube->GetProgram(), "useTexture"), 2);//
		n->Draw(*this);
	}

	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}