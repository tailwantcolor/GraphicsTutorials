#include  "Renderer.h"

Renderer::Renderer(Window &parent) :OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();

	matrixShader = new Shader("MatrixVertex.glsl", "colourFragment.glsl");

	camera = new Camera();// how to instantiate

	if (!matrixShader->LoadSuccess()) {
		return;
	}

	init = true;

	SwitchToOrthographic();
}

Renderer::~Renderer(void) {
	delete triangle;
	delete matrixShader;
	delete camera;
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, fov);

}void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(matrixShader);

	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);//what is the function and what is the location

	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);

	for (int i = 0; i < 3; ++i) {
		Vector3 temPos = position;
		temPos.z += (i * 500.0f);
		temPos.x -= (i * 100.0f);
		temPos.y -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(temPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);

		triangle->Draw();
	}
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}