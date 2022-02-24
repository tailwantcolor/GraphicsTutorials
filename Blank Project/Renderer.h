#pragma once
#include "../NCLGL/OGLRenderer.h"
#include"../nclgl/HeightMap.h"
#include"../nclgl/Light.h"
#include"../nclgl/Camera.h"
#include"../nclgl/SceneNode.h"
#include"../nclgl/CubeRobot.h"
#include"../nclgl/MeshMaterial.h"
#include"../nclgl/MeshAnimation.h"
#include"../nclgl/Mesh.h"
#include "water.h"
#include "../nclgl/spotlight.h"
#include "../nclgl/spotlight.h"
//#include"../nclgl/ParticleEmitter.h"

class HeightMap;

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;

	 void Update2scene(float msec);

	 //Shader* particleShader;

protected:
	void Drawshadow();
	void Drawscene();
	void DrawSkybox();
	void DrawWater();
	void DrawNode(SceneNode *n);
	void DrawShadowScene();
	void DrawMainScene();
	void DrawCubeRobot(SceneNode*n,int a);
	void DrawNodeShadow(SceneNode* n);
	void DrawTree();
	void Drawprojlightscene();
	void DrawBuilding();
	void DrawBuildingshadow();
	void DrawBuildingshadow_spot();
	/*void SetShaderParticleSize(float f){
		glUniform1f(glGetUniformLocation(particleShader->GetProgram(), "particleSize"), f);
	}*/
	void Drawmanshadow();
	void Drawmanscene();
	void DrawWave();
	
	


	Mesh*	triangle;
	HeightMap* heightMap;
	Water* water;
	Light* light;
	Spotlight* spotlight;
	Parlight* parlight;

	Mesh* quad;
	Mesh* cube;
	Mesh* sphere;
	Mesh* cylinder;
	Mesh* man;
	Mesh* tree_alone;
	Mesh* tower_body;

	MeshAnimation* anim;
	MeshMaterial* material;

	Camera* camera; // And the camera we're used to by now...
	Camera* camera_land1;
	Camera* camera_land2;
	Camera* camera_land3;
	Camera* camera_land4;


	SceneNode* root;
	SceneNode* root1;
	//ParticleEmitter* emitter; //A single particle emitter

	Shader* basicShader;
	Shader* sceneShader;
	Shader* shadowShader;
	Shader* shader;
	Shader* skyboxShader;
	Shader* reflectShader;
	Shader* sceneNodeShader;
	Shader* manshadowShader;
	Shader* mansceneShader;
	Shader* waterShader;
	Shader* towerShader;
	

	GLuint shadowFBO;
	GLuint shadowTex;
	GLuint BuildshadowFBO;
	GLuint BuildshadowTex;
	GLuint BuildshadowFBO_spot;
	GLuint BuildshadowTex_spot;
	GLuint cubeMap;
	GLuint waterTex;
	GLuint waterBump;
	GLuint alone;
	GLuint aloneBump;
	GLuint cube_r;
	GLuint cube_b;
	GLuint wave;


	GLuint sceneDiffuse;
	GLuint sceneBump;

	vector<GLuint> matTextures;

	float waterRotate;
	float waterCycle;
	int currentFrame;
	float frameTime;
	float scenetime;
	float scenetimesin;
	Matrix4 shadowspotmat;
	Matrix4 shadowparmat;
	
};
