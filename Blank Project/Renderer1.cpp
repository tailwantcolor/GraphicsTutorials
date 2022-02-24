#include "Renderer1.h"

Renderer1::Renderer1(Window& parent) :OGLRenderer(parent) {

	water = new Water(TEXTUREDIR"noise1.png");

	camera = new Camera(-10.0f, 270.0f, Vector3(1000.0f, 100.0f, 1000.0f));

	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	waterShader = new Shader("TexturedVertex.glsl", "texturedfragment.glsl");
	if (!waterShader->LoadSuccess()) { return; }


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	waterRotate = 0.0f;
	waterCycle = 0.0f;

}

 Renderer1::~Renderer1(void) {
	 delete camera;
	 delete waterShader;
}

 void Renderer1::UpdateScene(float dt) {
	 camera->UpdateCamera(dt);

 }

 void Renderer1::RenderScene() {
	 DrawWater();
 }

 void Renderer1::DrawWater() {
	 glViewport(0, 0, width / 2, height / 2);
	 BindShader(waterShader);
	 UpdateShaderMatrices();
	 glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "diffuseTex"), 0);
	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, waterTex);
	 water->Draw();
 }