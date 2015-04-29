#ifndef _ENGINESYSTEM_H_
#define _ENGINESYSTEM_H_

// PRE-PROCESSING DIRECTIVES
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "openglcontrol.h"
#include "input.h"
#include "graphics.h"

#include <iostream>
using namespace std;

class EngineSystem
{
public:
	EngineSystem();
	~EngineSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	OpenGLControl* m_OpenGL;
	Graphics* m_Graphics;
};

#endif
