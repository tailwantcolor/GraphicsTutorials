#include "../NCLGL/window.h"
#include "Renderer.h"
#include"Renderer1.h"
#include <iostream>

using namespace std;

int main()	{
	Window w("Make your own project!", 720, 540, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	//Renderer1 renderer1(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		///renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		//renderer.RenderScene();
		//renderer1.RenderScene();
		//cout << "input" << endl;
	
		renderer.Update2scene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.SwapBuffers();
	
		//renderer1.SwapBuffers();
		
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}
	return 0;
}