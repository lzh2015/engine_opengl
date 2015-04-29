#include "input.h"

Input& Input::GetInstance() {
	static Input instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

Input::Input()
{
	Initialize();
}

Input::~Input()
{
}


void Input::Initialize()
{
	int i;
	

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}


void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

bool Input::MouseButton(unsigned int key)
{
	return m_mouse[key];
}

void Input::MouseMove(int x, int y)
{
	m_vMouse.x = x;
	m_vMouse.y = y;
	//cout << m_vMouse.x << m_vMouse.y << endl;
}

void Input::MouseClick(int key, int state, int x, int y)
{
	m_mouse[key] = state;
	m_vMouse.x = x;
	m_vMouse.y = y;
}

glm::vec2 & Input::GetMousePosition()
{
	return m_vMouse;
}
