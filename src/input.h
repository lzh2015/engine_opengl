#ifndef _Input_H_
#define _Input_H_

#include <glm/glm.hpp>
#include <iostream>
using namespace std;

#define MOUSE_LEFT_BUTTON		0
#define MOUSE_MIDDLE_BUTTON		1
#define MOUSE_RIGHT_BUTTON		2

class Input
{
public:
	static Input& GetInstance();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);
	bool IsKeyDown(unsigned int key);
	bool MouseButton(unsigned int key);
	void MouseMove(int x, int y);
	void MouseClick(int key, int state, int x, int y);
	glm::vec2 & GetMousePosition();
private:
	Input();
	~Input();
	Input(Input const&);  // Don't Implement
	void operator=(Input const&); // Don't implement
	void Initialize();
	bool m_keys[256];
	bool m_mouse[3];
	glm::vec2 m_vMouse;
};

#endif
