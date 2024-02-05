#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include "GibMaths.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include<map>


class Input
{
private:
	static Input* instance;
	

	bool* keys = new bool[348];
	bool* prevKeys = new bool[348];

	bool* mouseButtons = new bool[8];
	bool* prevMouseButtons = new bool[8];

	Vec2 mouseDelta = { 0,0 };

public:

	static Vec2 mousePosition;

	Input();
	~Input();

	static Input* Instance();
	static void Release();

	static bool GetKeyDown(int key);
	static bool GetKey(int key);
	static bool GetKeyUp(int key);

	static bool GetMouseButtonDown(int button);
	static bool GetMouseButton(int button);
	static bool GetMouseButtonUp(int button);

	//static Vec2 MousePosition();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow*, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow*, double xOffset, double yOffset);
	void UpdatePrevInput();
	void UpdateMouse(GLFWwindow* window);

	static Vec2 MouseScrollDelta();
	


};
#endif // !INPUT_CLASS_H
