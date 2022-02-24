#pragma once

#include "../nclgl/OGLRenderer.h"
#include"../nclgl/HeightMap.h"

class Camera;
class Mesh;

class Renderer :public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void UpdateScene(float dt) override;
	void RenderScene() override;

protected:
	void DrawShadowScene();
	void DrawMainScene();
	void DrawShadowBox();
	void Drawpain();

	HeightMap* heightMap;

	GLuint shadowTex;
	GLuint shadowFBO;
	GLuint shadowFBOA[6];
	GLuint shadowbox;
	//GLuint shadow_cubeTex[6];
	GLuint axis[6];

	GLuint sceneDiffuse;
	GLuint sceneBump;
	float sceneTime;

	Shader* sceneShader;
	Shader* shadowShader;
	Shader* shadowpoint;
	Shader* pointShader;

	vector<Mesh*> sceneMeshes;
	vector<Matrix4> sceneTransforms;

	Camera* camera;
	Light* light;

	Matrix4 shadowmatrix_time[6];
};