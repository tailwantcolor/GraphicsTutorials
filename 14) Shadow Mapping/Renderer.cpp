 #include "Renderer.h"

#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"

#define SHADOWSIZE 2048

Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	camera = new Camera(-30.0f, 315.0f, Vector3(-8.0f, 5000.0f, 8.0f));
	light = new Light(Vector3(-20.0f, 100000.0f, -10.0f), Vector4(1, 1, 1, 1), 2500000.0f);
	sceneShader = new Shader("shadowscenevert.glsl", "shadowscenefrag.glsl");
	shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
	shadowpoint = new Shader("shadowVert_p.glsl", "shadowFrag_p.glsl");
	pointShader = new Shader("shadowpointVert.glsl", "shadowpointFrag.glsl");
	//heightMap = new HeightMap(TEXTUREDIR"noise1.png");
	//if (!shadowpoint->LoadSuccess() || !pointShader->LoadSuccess()) { return; }

	//FBO and generate own texture
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE , SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*glGenFramebuffers(1, &shadowFBOA[0]);
	glGenFramebuffers(1, &shadowFBOA[1]);
	glGenFramebuffers(1, &shadowFBOA[2]);
	glGenFramebuffers(1, &shadowFBOA[3]);
	glGenFramebuffers(1, &shadowFBOA[4]);
	glGenFramebuffers(1, &shadowFBOA[5]);

	glGenTextures(1, &shadow_cubeTex[0]);
	glGenTextures(1, &shadow_cubeTex[1]);
	glGenTextures(1, &shadow_cubeTex[2]);
	glGenTextures(1, &shadow_cubeTex[3]);
	glGenTextures(1, &shadow_cubeTex[4]);
	glGenTextures(1, &shadow_cubeTex[5]);*/

	GLuint axis[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	glGenTextures(1, &shadowbox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowbox);
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(axis[i], 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenFramebuffers(1, &shadowFBOA[0]);

	//glGenTextures(1, &shadowbox);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, shadowbox);
	//for (int i = 0; i < 6; ++i) {

	//	/*glGenTextures(1, &shadow_cubeTex[i]);
	//	glBindTexture(GL_TEXTURE_2D, shadow_cubeTex[i]);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);*/

	//	
	////}

	////glGenTextures(1, &shadowbox);
	////glBindTexture(GL_TEXTURE_CUBE_MAP, shadowbox);
	////for ( int i=0 ; i < 6 ; ++i){
	//	//glGenTextures(1, &shadowbox);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowbox);
	//	//glBindTexture(GL_TEXTURE_2D, shadow_cubeTex[i]);
	//	glTexImage2D(axis[i], 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//	glGenTextures(1, &shadow_cubeTex[i]);
	//	glBindTexture(GL_TEXTURE_2D, shadow_cubeTex[i]);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//	glGenFramebuffers(1, &shadowFBOA[i]);
	//	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOA[i]);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, axis[i], shadow_cubeTex[i], 0);
	//	glDrawBuffer(GL_NONE);
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//}

	for (int i = 0; i < 6; ++i) {
		if (i == 0) { shadowmatrix_time[0] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(1.0f, 0.0f, 0.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
		else if (i == 1) { shadowmatrix_time[1] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(-1.0f, 0.0f, 0.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
		else if (i == 2) { shadowmatrix_time[2] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(0.0f, 1.0f, 0.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
		else if (i == 3) { shadowmatrix_time[3] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(0.0f, -1.0f, 0.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
		else if (i == 4) { shadowmatrix_time[4] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(0.0f, 0.0f, 1.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
		else if (i == 5) { shadowmatrix_time[5] = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(0.0f, 0.0f, -1.0f))* Matrix4::Perspective(1, 100, 1, 90.0f); }
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	sceneMeshes.emplace_back(Mesh::GenerateQuad());
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Sphere.msh"));
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Cylinder.msh"));
	sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Cone.msh"));
	//sceneMeshes.emplace_back(heightMap);

	sceneDiffuse = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sceneBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SetTextureRepeating(sceneDiffuse, true);
	SetTextureRepeating(sceneBump, true);
	glEnable(GL_DEPTH_TEST);

	sceneTransforms.resize(4);
	sceneTransforms[0] = Matrix4::Rotation(90, Vector3(1, 0, 0)) * Matrix4::Scale(Vector3(10, 10, 1));
	//sceneTransforms[4].ToIdentity();
	sceneTime = 0.0f;
	init = true;
}

Renderer::~Renderer(void) {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	//delete heightMap;
	//to do delete texture and fbo

	for (auto& i : sceneMeshes) { delete i; }

	delete camera;
	delete sceneShader;
	delete shadowShader;
	delete shadowpoint;
	delete pointShader;
	for (int i = 0; i < 6; ++i) {
		//glDeleteTextures(1, &shadow_cubeTex[i]);
		glDeleteFramebuffers(1, &shadowFBOA[i]);
	}
	

}
void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	sceneTime += dt;

	for (int i = 1; i < 4; ++i) {//skip the floor!
		Vector3 t = Vector3(-10 + (5 * i), 2.0f + sin(sceneTime * i), 0);
		sceneTransforms[i] = Matrix4::Translation(t) * Matrix4::Rotation(sceneTime * 10 * i, Vector3(1, 0, 0));
	}
	//light->UpdateLight(dt);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawShadowScene();
	//DrawShadowBox();
	DrawMainScene();
	//Drawpain();
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0.0f, 0, 0.0f));//发散光的第二个Vector3会有问题 试着改一下方向
    //projMatrix = Matrix4::Perspective(1, 100, 1, 90.0f);
	projMatrix = Matrix4::Orthographic(20.0f, 2000000.0f, -50.0f, 50.0f, -50.0f, 50.0f);
	shadowMatrix = projMatrix * viewMatrix; // uesd later;

	for (int i = 0; i < 4; ++i) {
		modelMatrix = sceneTransforms[i];
		UpdateShaderMatrices();
		sceneMeshes[i]->Draw();
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_CULL_FACE);
}


void Renderer::DrawShadowBox() {
	GLuint axis[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,GL_TEXTURE_CUBE_MAP_NEGATIVE_X ,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	BindShader(shadowpoint);
	for (int i = 0; i < 6;++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOA[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, axis[i], shadowbox, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		

		//viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0.0f, 0, 0.0f));
		if (i == 0) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition() + Vector3(1.0f, 0.0f, 0.0f)); }
		else if(i==1) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition()+Vector3(-1.0f, 0.0f, 0.0f)); }
		else if (i == 2) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition()+Vector3(0.0f, 1.0f, 0.0f)); }
		else if (i == 3) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition()+Vector3(0.0f, -1.0f, 0.0f)); }
		else if (i == 4) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition()+Vector3(0.0f, 0.0f, 1.0f)); }
		else if (i == 5) { viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), light->GetPosition()+Vector3(0.0f, 0.0f, -1.0f)); }
		projMatrix = Matrix4::Perspective(1, 100, 1, 90.0f);
		//projMatrix = Matrix4::Orthographic(20.0f, 20000.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		shadowMatrix = projMatrix * viewMatrix; // uesd later;

		for (int j = 0; j < 4; ++j) {
			modelMatrix = sceneTransforms[j];
			UpdateShaderMatrices();
			sceneMeshes[j]->Draw();
		}
	

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


void Renderer::DrawMainScene() {
	BindShader(sceneShader);
	SetShaderLight(*light);
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sceneBump);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (int i = 0; i < 4; ++i) {
		modelMatrix = sceneTransforms[i];
		UpdateShaderMatrices();
		sceneMeshes[i]->Draw();
	}

}

void Renderer::Drawpain() {
	BindShader(pointShader);
	SetShaderLight(*light);
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glUniform1i(glGetUniformLocation(pointShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(pointShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(pointShader->GetProgram(), "cubeTex"), 2);
	glUniform3fv(glGetUniformLocation(pointShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sceneBump);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowbox);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_posx"), 1, false, shadowmatrix_time[0].values);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_negx"), 1, false, shadowmatrix_time[1].values);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_posy"), 1, false, shadowmatrix_time[2].values);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_negy"), 1, false, shadowmatrix_time[3].values);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_posz"), 1, false, shadowmatrix_time[4].values);
	glUniformMatrix4fv(glGetUniformLocation(pointShader->GetProgram(), "shadowmatrix_negz"), 1, false, shadowmatrix_time[5].values);

	for (int i = 0; i < 4; ++i) {
		modelMatrix = sceneTransforms[i];
		UpdateShaderMatrices();
		sceneMeshes[i]->Draw();
	}
}