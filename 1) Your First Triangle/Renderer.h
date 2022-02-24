#pragma once

#include "../NCLGL/OGLRenderer.h"
#include "../NCLGL/Camera.h"
#include "ParticleEmitter.h"	//A new class!

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	Shader* particleShader;
	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void	SetShaderParticleSize(float f);	//And a new setter

	ParticleEmitter* emitter;	//A single particle emitter
	Camera* camera;		//And the camera we're used to by now...
};
