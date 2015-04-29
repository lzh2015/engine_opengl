#ifndef _Graphics_H_
#define _Graphics_H_

#pragma once

#include "openglcontrol.h"
#include "shader.h"
#include "viewCamera.h"
#include "light.h"
#include "scene/sceneManager.h"
#include "sky.h"
#include "timer.h"
#include "input.h"

class Graphics
{
public:
	Graphics();
	//Graphics(const Graphics&);
	~Graphics();

	bool Initialize(OpenGLControl*, HWND);
	void Shutdown();
	bool Frame();
	void OnKeyboard(unsigned int input);
private:
	void CheckInput();
	bool Render();

private:
	OpenGLControl* m_OpenGL;
	ShaderResource & m_ShaderResource;
	SceneManager & m_SceneManager;
	Sky & m_Sky;
	ViewCamera & m_Camera;
	LightResource & m_LightResource;
};

#endif
