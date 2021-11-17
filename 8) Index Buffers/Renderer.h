#pragma once
#include "../nclgl/OGLRenderer.h"
#include "SceneNode.h"

class HeightMap;
class Camera;

class Renderer:public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;
protected:
	void DrawNode(SceneNode* n);

	HeightMap* heightMap;
	Shader* shader;
	Shader* shader_cube;
	Camera* camera;
	GLuint terrainTex;
	SceneNode* root;
	Mesh* cube;
	
};