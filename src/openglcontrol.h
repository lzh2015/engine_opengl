#ifndef _OPENGLCONTROL_H_
#define _OPENGLCONTROL_H_

#pragma once

#include "global.h"
//solve the error: 'ptrdiff_t' has not been declared issue
#include <cstddef>

class OpenGLControl
{
public:
	OpenGLControl();
	//OpenGLControl(const OpenGLControl&);
	~OpenGLControl();

	bool InitializeExtensions(HWND);
	bool InitializeOpenGL(HWND, int, int);
	void Shutdown(HWND);

	void InitScene(float, float, float, float);
	void ClearScene();
	void EndScene();

	void ResizeOpenGLViewportFull();
	void SetProjectionMatrix(float fFOV, float fAspectRatio, float fNear, float fFar);
	const glm::mat4 & GetProjectionMatrix() const;

	void GetVideoCardInfo(char*);
private:
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	glm::mat4 m_projection;
	char m_videoCardDescription[128];
	int m_iViewportWidth, m_iViewportHeight;//viewport parameters
	bool LoadExtensionList();
};

#endif
