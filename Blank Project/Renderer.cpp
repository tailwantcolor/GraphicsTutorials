


#include "Renderer.h"

#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"
#include "water.h"
#include "../nclgl/Window.h"

#define SHADOWSIZE 2048

int a = 0;
bool camera_auto = true;

Renderer::Renderer(Window& parent) :OGLRenderer(parent) {
	
	
	heightMap = new HeightMap(TEXTUREDIR"noise1.png");
	quad = Mesh::GenerateQuad();
	Vector3 dimensions = heightMap->GetHeightmapSize();
	water = new Water(TEXTUREDIR"noise1.png");

	cube = Mesh::LoadFromMeshFile("offsetCubeY.msh");
	sphere = Mesh::LoadFromMeshFile("Sphere.msh");
	cylinder = Mesh::LoadFromMeshFile("Cylinder.msh");
	root = new SceneNode();
	root->AddChild(new CubeRobot(cube,cylinder,sphere));
	root1 = new SceneNode();
	for (int i = 0; i < 5; ++i) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation(Vector3(2000, 1000.0f, 300.0f + 100 * i)));
		s->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
		s->SetMesh(sphere);
		s->SetTexture(waterTex);
		root1->AddChild(s);
	}

	

	man = Mesh::LoadFromMeshFile("Role_T.msh");
	anim = new MeshAnimation("Role_T.anm");
	material = new MeshMaterial("Role_T.mat");

	tree_alone = Mesh::LoadFromMeshFile("Palm_Tree.msh");
	tower_body = Mesh::LoadFromMeshFile("Cylinder.msh");


	camera = new Camera(-30.0f, 235.0f, Vector3(2000.0f,1000.0f,2000.0f));
	camera_land1 = new Camera(-30.0f, 270.0f, Vector3(3000.0f, 1000.0f, 5000.0f));
	camera_land2 = new Camera(-30.0f, 270.0f, Vector3(5000.0f, 1000.0f, 3000.f));
	camera_land3 = new Camera(-30.0f,270.0f,Vector3(2000.0f,1000.0f,2000.0f));
	camera_land4 = new Camera(-30.0f, 270.0f, Vector3(5000.0f, 1000.0f, 5000.0f));

	light = new Light(Vector3(dimensions.x*0.3f, 10000.0f, dimensions.z*0.0f), Vector4(1, 1, 1, 1), 25000.0f);
	spotlight = new Spotlight(Vector3(dimensions.x*0.6f,500.0f,dimensions.z*0.6f), Vector4(1, 1, 1, 1),Vector3(dimensions.x * 0.54f+80.0f,20.0f, dimensions.z * 0.54f+80.0f),0.9f,20000.0f);
	parlight = new Parlight(Vector3(0.0f, 1000.0f, 0.0f), Vector4(1, 1, 1, 1), Vector3(dimensions.x * 0.5f, 0.0f, dimensions.y * 0.5f));


	sceneShader = new Shader("shadowscenevert.glsl", "shadowscenefrag.glsl");
	shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	sceneNodeShader = new Shader("SceneNodeVert.glsl", "SceneNodeFrag.glsl");//no need
	manshadowShader = new Shader("SkinningVertexshadow.glsl", "shadowFrag.glsl");
	mansceneShader = new Shader("SkinningVertex.glsl", "texturedFragment.glsl");
	waterShader = new Shader("waterVertex.glsl", "waterFrag.glsl");
	towerShader = new Shader("towerVertex.glsl", "waterFrag.glsl");


	/*particleShader = new Shader("particleVertex.glsl",
		"particleFragment.glsl",
		"particleGeometry.glsl");*/
	
	//if (!shadowpoint->LoadSuccess() || !pointShader->LoadSuccess()) { return; }
	if (!sceneShader->LoadSuccess() || !shadowShader->LoadSuccess()||!skyboxShader->LoadSuccess()) { return; }
	if (!sceneNodeShader->LoadSuccess()) { return; }
	if (!manshadowShader->LoadSuccess()) { return; }
	if (!mansceneShader->LoadSuccess()) { return; }
	if (!waterShader->LoadSuccess()) { return; }
	if (!towerShader->LoadSuccess()) { return; }

	/*if (!particleShader->LoadSuccess()) { return; }*/

	//A new particle emitter!
	//emitter = new ParticleEmitter();

	//Generate a man
	for (int i = 0; i < man->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}


	//FBO and generate own texture
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//second FBO and generate texture
	glGenTextures(1, &BuildshadowTex);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &BuildshadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, BuildshadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, BuildshadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//spot FBO and generate texture
	glGenTextures(1, &BuildshadowTex_spot);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex_spot);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &BuildshadowFBO_spot);
	glBindFramebuffer(GL_FRAMEBUFFER, BuildshadowFBO_spot);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, BuildshadowTex_spot, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//texture
	sceneDiffuse = SOIL_load_OGL_texture(TEXTUREDIR"yanye.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sceneBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	waterBump = SOIL_load_OGL_texture(TEXTUREDIR"waterbump.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	alone = SOIL_load_OGL_texture(TEXTUREDIR"tree_alone.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	aloneBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3 TEST.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cube_r = SOIL_load_OGL_texture(TEXTUREDIR"lizi.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cube_b = SOIL_load_OGL_texture(TEXTUREDIR"qingniao.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	wave = SOIL_load_OGL_texture(TEXTUREDIR"wave5.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	
	SetTextureRepeating(sceneDiffuse, true);
	SetTextureRepeating(sceneBump, true);
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(waterBump, true);
	SetTextureRepeating(alone, true);
	SetTextureRepeating(aloneBump, true);
	SetTextureRepeating(cube_r, true);
	SetTextureRepeating(cube_b, true);
	SetTextureRepeating(wave, true);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	waterRotate = 0.0f;
	waterCycle = 0.0f;
	
	currentFrame = 0;
	frameTime = 0.0f;
	scenetime = 0.0f;
	scenetimesin = 0.0f;

	shadowparmat = Matrix4::Orthographic(20.0f, 15000.0f, -5000.0f, 5000.0f, -5000.0f, 5000.0f) * Matrix4::BuildViewMatrix(parlight->GetPosition(), Vector3(dimensions.x * 0.5f, 0.0f, dimensions.z * 0.5f));
	shadowspotmat = Matrix4::Perspective(1, 2000, 1, 45.0f)* Matrix4::BuildViewMatrix(spotlight->GetPosition(), spotlight->Getlookingat());

	init = true;
}

Renderer::~Renderer(void) {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	
	//to do delete texture and fbo

	//delete emitter;
	delete sphere;
	delete cylinder;
	delete cube;
	delete water;
	
	delete heightMap;
	delete camera;
	delete sceneShader;
	delete shadowShader;
	delete skyboxShader;
	delete reflectShader;
	delete manshadowShader;
	delete mansceneShader;
	delete sceneNodeShader;
	delete waterShader;
	delete towerShader;
	delete light;
	delete root;
	delete root1;
	delete spotlight;
	delete parlight;
	delete camera_land1;
	delete camera_land2;
	delete camera_land3;
	delete camera_land4;
	//delete root1;
}
void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(100*dt);
	waterRotate += dt * 2.0f; // 2 degrees a second
    waterCycle += dt * 0.25f;
	root->Update(dt);
	//root1->Update(dt);
	//emitter->Update(1000 * dt);
	//light->UpdateLight(dt);
	
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}

	//camera->Updateauto(10*dt);

}

void Renderer::Update2scene(float dt) {
	/*camera->UpdateCamera(100 * dt);
	camera_land1->UpdateCamera(100*dt);
	camera_land2->UpdateCamera(100 * dt);
	camera_land3->UpdateCamera(100 * dt);
	camera_land4->UpdateCamera(100 * dt);*/

	waterRotate += dt * 2.0f; // 2 degrees a second
	waterCycle += dt * 0.25f;
	root->Update(dt);
	//root1->Update(dt);
	//emitter->Update(1000 * dt);
	light->UpdateLight(dt);

	frameTime -= dt;
	scenetime += dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_4)) {
		camera_auto = true;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_5)) {
		camera_auto = false;
	}

	if (camera_auto) {
		camera->Updateauto(dt);
		camera_land1->Updateauto(dt);
		camera_land2->Updateauto(dt);
		camera_land3->Updateauto(dt);
		camera_land4->Updateauto(dt);
	}

	if (!camera_auto) {
		camera->UpdateCamera(100 * dt);
		camera_land1->UpdateCamera(100 * dt);
		camera_land2->UpdateCamera(100 * dt);
		camera_land3->UpdateCamera(100 * dt);
		camera_land4->UpdateCamera(100 * dt);
	}
	spotlight->UpdatespotLight(dt);

	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}
	
	/*if ((int)scenetime % 3 == 1) {
		scenetimesin = 3.0f * sin(scenetime);
	}
	else if((int)scenetime%3 == 2)
		scenetimesin = 3.0f * sin(-scenetime);*/
	//scenetimesin = log(exp(scenetime))*pow(scenetime,1/2)*sin(scenetime/5.0f);
	if ((int)scenetime % 10 < 5)
		scenetimesin += dt;
	if ((int)scenetime % 10 >= 5)
		scenetimesin -= dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
		a = 1;
	}
	
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
		a = 0;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
		a = 2;
	}
	

	if (a==1) {
		RenderScene();
	}
	else if (a==0) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		DrawBuildingshadow();
		DrawBuildingshadow_spot();
		DrawWave();
		DrawBuilding();
		
	}
	else if (a == 2) {
		//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		DrawShadowScene();
		//DrawShadowScene();
		glViewport(0, 0, width / 2, height / 2);
		
		DrawSkybox();
		viewMatrix = camera_land1->BuildViewMatrix();
		DrawMainScene();
		DrawWater();
		//glViewport(0, 0, width / 2, height / 2);
		glViewport(width/2, 0, width / 2, height / 2);
		
		DrawSkybox();
		viewMatrix = camera_land2->BuildViewMatrix();
		DrawMainScene();
		DrawWater();
		glViewport(0, height/2, width / 2, height / 2);
		
		DrawSkybox();
		viewMatrix = camera_land3->BuildViewMatrix();
		DrawMainScene();
		DrawWater();
		glViewport(width/2, height / 2, width / 2, height / 2);
		
		DrawSkybox();
		viewMatrix = camera_land4->BuildViewMatrix();
		DrawMainScene();
		DrawWater();
	}
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	DrawShadowScene();
	viewMatrix=camera->BuildViewMatrix();
	DrawMainScene();
	DrawWater();
	//DrawCubeRobot(root);	
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	BindShader(skyboxShader);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
    quad->Draw();
    glDepthMask(GL_TRUE);
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);
	Vector3 dim = heightMap->GetHeightmapSize();
	// light direction
	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(dim.x,0.0f,dim.z*0.5f));//发散光的第二个Vector3会有问题 试着改一下方向
	//projMatrix = Matrix4::Perspective(100, 10000, 1, 90.0f);
	projMatrix = Matrix4::Orthographic(20.0f, 15000.0f, -5000.0f, 5000.0f, -5000.0f, 5000.0f);
	shadowMatrix = projMatrix * viewMatrix; // uesd later;

	
		modelMatrix.ToIdentity();
		UpdateShaderMatrices();
		heightMap->Draw();

		UpdateShaderMatrices();
		DrawNodeShadow(root);

		//UpdateShaderMatrices();
		//DrawNodeShadow(root1);

		//tree_alone render shadowmap
		Matrix4 model_alone = Matrix4::Translation(Vector3(3000, 160, 5000)) * Matrix4::Scale(Vector3(200, 200, 200));
		modelMatrix = model_alone;
		UpdateShaderMatrices();
		tree_alone->Draw();

		//change shader
		UpdateShaderMatrices();
		Drawmanshadow();

		
		
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);
}





void Renderer::DrawMainScene() {
	BindShader(sceneShader);
	SetShaderLight(*light);
	//viewMatrix = camera->BuildViewMatrix();
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
	
	   
		modelMatrix.ToIdentity();
		UpdateShaderMatrices();
		heightMap->Draw();

		glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
		glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
		glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_r);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, aloneBump);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowTex);

		DrawCubeRobot(root,0);
		//DrawCubeRobot(root1, 0);
		//DrawCubeRobot(root1);

		//tree_alone render shadowscene
		/*Matrix4 model_alone = Matrix4::Translation(Vector3(3000, 180, 5000)) * Matrix4::Scale(Vector3(200, 200, 200));
		modelMatrix = model_alone;
		UpdateShaderMatrices();
		tree_alone->Draw();*/

		DrawTree();

		UpdateShaderMatrices();
		Drawmanscene();


		
		//BindShader(NULL);

}


void Renderer::Drawprojlightscene() {
	BindShader(sceneShader);
	SetShaderLight(*light);
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
	//Vector3 dim = heightMap->GetHeightmapSize();
	//Vector3 lightdirection = light->GetPosition() - Vector3(dim.x, 0.0f, dim.z);
	//glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "lightDir"), 1, (float*)lightdirection);
	glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sceneBump);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

}



//to do: make it dark in night 
void Renderer::DrawWater() {
		BindShader(reflectShader);
		glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "bumpTex"), 1);
		glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
		//glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "shadowTex"), 3);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterBump);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, shadowTex);
		Vector3 hSize = heightMap->GetHeightmapSize();
		modelMatrix = Matrix4::Translation(Vector3(hSize.x * 0.5f, hSize.y * 0.5f, hSize.z * 0.5f)) * Matrix4::Scale(hSize * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));
		textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(5, 5, 5)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
		projMatrix= Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
		UpdateShaderMatrices();
		//SetShaderLight(*light);// lighting in this shader!
		quad->Draw();
	}

void Renderer::DrawCubeRobot(SceneNode*n,int a) {
	
	if (n->GetMesh()) {
		if (a <= 2) {
			Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
			modelMatrix = model;
			UpdateShaderMatrices();
			n->Draw(*this);
		}
		if (a > 2) {
			Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
			modelMatrix = model;
			UpdateShaderMatrices();
			glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
			glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
			glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
			glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cube_b);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, aloneBump);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, shadowTex);
			n->Draw(*this);
		}
	}

	/*for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
	
		DrawCubeRobot(*i);
	}*/
	for (int i = 0; i < n->children.size(); ++i) {
		DrawCubeRobot(n->children[i], i);
		
	}

}

void Renderer::DrawNodeShadow(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		modelMatrix = model;
		UpdateShaderMatrices();
		n->Draw(*this);
	}

	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNodeShadow(*i);

	}
}

void Renderer::Drawmanshadow() {
	BindShader(manshadowShader);
	Matrix4 model = Matrix4::Translation(Vector3(4000, 270, 4000)) * Matrix4::Scale(Vector3(100, 100, 100));
	modelMatrix = model;//if change the location change it!!!!
	UpdateShaderMatrices();
	vector<Matrix4> frameMatrices;
	const Matrix4* invBindPose = man->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);

	for (unsigned int i = 0; i < man->GetJointCount(); ++i) {
		frameMatrices.emplace_back((frameData[i] * invBindPose[i]));
	}

	int j = glGetUniformLocation(manshadowShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());
	for (int i = 0; i < man->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		man->DrawSubMesh(i);
	}
};

void Renderer::Drawmanscene() {
	BindShader(mansceneShader);
	glUniform1i(glGetUniformLocation(mansceneShader->GetProgram(), "diffuseTex"), 0);
	Matrix4 model =Matrix4::Translation(Vector3(4000,270,4000))* Matrix4::Scale(Vector3(100, 100, 100));
	modelMatrix = model;//if change the location change it!!!!
	//viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	vector<Matrix4> frameMatrices;
	const Matrix4* invBindPose = man->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);

	for (unsigned int i = 0; i < man->GetJointCount(); ++i) {
		frameMatrices.emplace_back((frameData[i] * invBindPose[i]));
	}

	int j = glGetUniformLocation(mansceneShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());
	for (int i = 0; i < man->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		man->DrawSubMesh(i);
	}

	
}

void Renderer::DrawTree() {
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, alone);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, aloneBump);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	Matrix4 model_alone = Matrix4::Translation(Vector3(3000, 160, 5000)) * Matrix4::Scale(Vector3(200, 200, 200));
    modelMatrix = model_alone;
    UpdateShaderMatrices();
	tree_alone->Draw();

}

void Renderer::DrawWave() {
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width , height );
	BindShader(waterShader);
	//auto a = (*spotlight);
	SetShaderSpotLight(*spotlight);
	SetShaderParLight(*parlight);
	modelMatrix.ToIdentity();
	viewMatrix=camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	textureMatrix=Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle))* Matrix4::Scale(Vector3(5, 5, 5))* Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));;
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, aloneBump);
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "waveTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wave);
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "shadowTex"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex);
	glUniform1i(glGetUniformLocation(waterShader->GetProgram(), "shadowTex_spot"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex_spot);
	glUniform1f(glGetUniformLocation(waterShader->GetProgram(), "time"), scenetimesin);
	glUniform3fv(glGetUniformLocation(waterShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniformMatrix4fv(glGetUniformLocation(waterShader->GetProgram(), "shadowparmat"), 1, false, shadowparmat.values);
	glUniformMatrix4fv(glGetUniformLocation(waterShader->GetProgram(), "shadowspotmat"), 1, false, shadowspotmat.values);

	water->Draw();
}

void Renderer::DrawBuilding() {
	
	BindShader(towerShader);
	SetShaderParLight(*parlight);
	SetShaderSpotLight(*spotlight);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glUniform1i(glGetUniformLocation(towerShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(towerShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(towerShader->GetProgram(), "shadowTex"), 2);
	glUniform1i(glGetUniformLocation(towerShader->GetProgram(), "shadowTex_spot"), 3);
	glUniform3fv(glGetUniformLocation(towerShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneDiffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, aloneBump);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, BuildshadowTex_spot);
	glUniformMatrix4fv(glGetUniformLocation(towerShader->GetProgram(), "shadowparmat"), 1, false, shadowparmat.values);
	glUniformMatrix4fv(glGetUniformLocation(towerShader->GetProgram(), "shadowspotmat"), 1, false, shadowspotmat.values);

	Vector3 dim = water->GetWaterSize();
	Matrix4 model_tower = Matrix4::Translation(Vector3(dim.x * 0.6f, 240, dim.z * 0.6f)) * Matrix4::Scale(Vector3(500, 1000, 500));
	modelMatrix = model_tower;
	UpdateShaderMatrices();
	tower_body->Draw();

}

void Renderer::DrawBuildingshadow() {
	glBindFramebuffer(GL_FRAMEBUFFER, BuildshadowFBO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);
	Vector3 dim = water->GetWaterSize();
	// light direction
	viewMatrix = Matrix4::BuildViewMatrix(parlight->GetPosition(), Vector3(dim.x*0.5f, 0.0f, dim.z * 0.5f));//发散光的第二个Vector3会有问题 试着改一下方向
	//projMatrix = Matrix4::Perspective(100, 10000, 1, 90.0f);
	projMatrix = Matrix4::Orthographic(20.0f, 15000.0f, -5000.0f, 5000.0f, -5000.0f, 5000.0f);
	shadowMatrix = projMatrix * viewMatrix; // uesd later;

	Matrix4 model_tower = Matrix4::Translation(Vector3(dim.x * 0.6f, 240, dim.z * 0.6f)) * Matrix4::Scale(Vector3(500, 1000, 500));
	modelMatrix = model_tower;
	UpdateShaderMatrices();
	tower_body->Draw();


	// combine matrix
	//viewMatrix = Matrix4::BuildViewMatrix(spotlight->GetPosition(), spotlight->Getlookingat());
	//projMatrix = Matrix4::Perspective(1, 2000, 1, 45.0f);
	//shadowMatrix = projMatrix * viewMatrix;
	//UpdateShaderMatrices();
	//tower_body->Draw();
	//heightMap->Draw();

	//UpdateShaderMatrices();
	//DrawNodeShadow(root);

	//UpdateShaderMatrices();
	//DrawNodeShadow(root1);

	//tree_alone render shadowmap
	//Matrix4 model_alone = Matrix4::Translation(Vector3(3000, 160, 5000)) * Matrix4::Scale(Vector3(200, 200, 200));
	//modelMatrix = model_alone;
	//UpdateShaderMatrices();
	//tree_alone->Draw();

	//change shader
	//UpdateShaderMatrices();
	//Drawmanshadow();



	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);
}

void Renderer::DrawBuildingshadow_spot() {
	glBindFramebuffer(GL_FRAMEBUFFER, BuildshadowFBO_spot);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);
	Vector3 dim = water->GetWaterSize();
	// light direction
	//viewMatrix = Matrix4::BuildViewMatrix(parlight->GetPosition(), Vector3(dim.x * 0.5f, 0.0f, dim.z * 0.5f));//发散光的第二个Vector3会有问题 试着改一下方向
	//projMatrix = Matrix4::Perspective(100, 10000, 1, 90.0f);
	//projMatrix = Matrix4::Orthographic(20.0f, 15000.0f, -5000.0f, 5000.0f, -5000.0f, 5000.0f);
	//shadowMatrix = projMatrix * viewMatrix; // uesd later;

	//Matrix4 model_tower = Matrix4::Translation(Vector3(dim.x * 0.6f, 240, dim.z * 0.6f)) * Matrix4::Scale(Vector3(500, 1000, 500));
	//modelMatrix = model_tower;
	//UpdateShaderMatrices();
	//tower_body->Draw();


	// spot matrix
	viewMatrix = Matrix4::BuildViewMatrix(spotlight->GetPosition(), spotlight->Getlookingat());
	projMatrix = Matrix4::Perspective(1, 2000, 1, 45.0f);
	Matrix4 model_tower = Matrix4::Translation(Vector3(dim.x * 0.6f, 240, dim.z * 0.6f)) * Matrix4::Scale(Vector3(500, 1000, 500));
	modelMatrix = model_tower;
	//shadowMatrix = projMatrix * viewMatrix;
	UpdateShaderMatrices();
	tower_body->Draw();
	//heightMap->Draw();

	//UpdateShaderMatrices();
	//DrawNodeShadow(root);

	//UpdateShaderMatrices();
	//DrawNodeShadow(root1);

	//tree_alone render shadowmap
	//Matrix4 model_alone = Matrix4::Translation(Vector3(3000, 160, 5000)) * Matrix4::Scale(Vector3(200, 200, 200));
	//modelMatrix = model_alone;
	//UpdateShaderMatrices();
	//tree_alone->Draw();

	//change shader
	//UpdateShaderMatrices();
	//Drawmanshadow();



	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);
}