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

//#include"../nclgl/ParticleEmitter.h"

class HeightMap;

class Renderer1 : public OGLRenderer {
public:
	Renderer1(Window& parent);
	~Renderer1(void);
	void RenderScene()				override;
	void UpdateScene(float msec)	override;

	//Shader* particleShader;

protected:
	void DrawWater();
	/*Mesh* triangle;
	HeightMap* heightMap;
	Light* light;
	Mesh* quad;
	Mesh* cube;
	Mesh* sphere;
	Mesh* cylinder;
	Mesh* man;
	Mesh* tree_alone;*/

	Water* water;
	

	Camera* camera; // And the camera we're used to by now...
	/*SceneNode* root;
	SceneNode* root1;*/
	//ParticleEmitter* emitter; //A single particle emitter


	Shader* waterShader;
	/*Shader* basicShader;
	Shader* sceneShader;
	Shader* shadowShader;
	Shader* shader;
	Shader* skyboxShader;
	Shader* reflectShader;
	Shader* sceneNodeShader;
	Shader* manshadowShader;
	Shader* mansceneShader;
*/

	/*GLuint shadowFBO;
	GLuint shadowTex;
	GLuint cubeMap;
	GLuint waterTex;
	GLuint waterBump;
	GLuint alone;
	GLuint aloneBump;
	GLuint cube_r;
	GLuint cube_b;*/
	GLuint waterTex;


	/*GLuint sceneDiffuse;
	GLuint sceneBump;

	vector<GLuint> matTextures;*/

	float waterRotate;
	float waterCycle;
	int currentFrame;
	float frameTime;

};

