#include "graphics.h"
#include "global.h"
#include "shader.h"

Graphics::Graphics():
	m_ShaderResource (ShaderResource::GetInstance()),
	m_SceneManager(SceneManager::GetInstance()),
	m_Sky(Sky::GetInstance()),
	m_Camera(ViewCamera::GetInstance()),
	m_LightResource(LightResource::GetInstance())

{
	m_OpenGL = 0;
}


//Graphics::Graphics(const Graphics& other)
//{
//}


Graphics::~Graphics()
{
}


bool Graphics::Initialize(OpenGLControl* OpenGL, HWND hwnd)
{
	bool result;

	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	m_OpenGL->InitScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = m_ShaderResource.Initialize();
	if(!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the shaders.", L"Error", MB_OK);
		return false;
	}

	result = m_LightResource.Initialize();
	if(!result)
	{
		MessageBoxW(hwnd, L"Could not initialize light resources.", L"Error", MB_OK);
		return false;
	}

	result = m_Sky.Initialize();
	if(!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky.", L"Error", MB_OK);
		return false;
	}

	result = m_SceneManager.Initialize();
	if(!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sceneManager.", L"Error", MB_OK);
		return false;
	}

	Timer::GetInstance().Start();

	m_Camera.SetType(ViewCamera::FREE);

	return true;
}


void Graphics::Shutdown()
{
	m_ShaderResource.Delete();

	m_LightResource.Delete();

	m_Sky.Delete();

	m_SceneManager.Release();

	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;

	return;
}


bool Graphics::Frame()
{
	CheckInput();

	bool result;
	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

void Graphics::CheckInput()
{
	if(Input::GetInstance().IsKeyDown(A_KEY))
	{
		m_Camera.rotateViewY(3.0f * Timer::GetInstance().GetElapsedTime());
		m_LightResource.Update();
	}

	if(Input::GetInstance().IsKeyDown(D_KEY))
	{
		m_Camera.rotateViewY(-3.0f * Timer::GetInstance().GetElapsedTime());
		m_LightResource.Update();
	}

	if(Input::GetInstance().IsKeyDown(W_KEY))
	{
		m_Camera.move(Timer::GetInstance().GetElapsedTime());
		m_LightResource.Update();
	}

	if(Input::GetInstance().IsKeyDown(S_KEY))
	{
		m_Camera.move(-Timer::GetInstance().GetElapsedTime());
		m_LightResource.Update();
	}

	if(Input::GetInstance().IsKeyDown(F1_KEY))
	{
		m_LightResource.OnKeyboard(F1_KEY);
	}

	if(Input::GetInstance().IsKeyDown(F2_KEY))
	{
		m_LightResource.OnKeyboard(F2_KEY);
	}

	if(Input::GetInstance().IsKeyDown(SPACE_KEY))
	{
		switch(m_Camera.GetType())
		{
			case ViewCamera::FREE:		m_Camera.SetType(ViewCamera::DRIVEN);	break;
			case ViewCamera::DRIVEN:	m_Camera.SetType(ViewCamera::FREE);		break;
		};
	}

//	if((GetAsyncKeyState(A_KEY)>>15)&1)
//	{
//		m_Camera.rotateViewY(3.0f * Timer::GetInstance().GetElapsedTime());
//	}
//
//	if((GetAsyncKeyState(D_KEY)>>15)&1)
//	{
//		m_Camera.rotateViewY(-3.0f * Timer::GetInstance().GetElapsedTime());
//	}
//
//	if((GetAsyncKeyState(W_KEY)>>15)&1)
//	{
//		m_Camera.move(Timer::GetInstance().GetElapsedTime());
//	}
//
//	if((GetAsyncKeyState(S_KEY)>>15)&1)
//	{
//		m_Camera.move(-Timer::GetInstance().GetElapsedTime());
//	}

	glm::vec2 pos = Input::GetInstance().GetMousePosition();
	static glm::vec2 lastPos = pos;

	if(Input::GetInstance().MouseButton(MOUSE_LEFT_BUTTON))
	{
		m_Camera.RotateView((lastPos.x-pos.x)*0.005, (lastPos.y-pos.y)*0.005);
		m_LightResource.Update();
	}
	lastPos = pos;
}

bool Graphics::Render()
{
	// Clear the buffers to begin the scene.
	m_OpenGL->ClearScene();

	m_SceneManager.Idle(Timer::GetInstance().GetElapsedTime());

	//off-screen rendering
	m_SceneManager.PreRender();

	m_SceneManager.Render();

	// Present the rendered scene to the screen.
	m_OpenGL->EndScene();

	//time per frame
	Timer::GetInstance().Tick();

	return true;
}

void Graphics::OnKeyboard(unsigned int input)
{
	m_SceneManager.OnKeyboard(input);
}
