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

public:

	static Vec2 mousePosition;

	Input();
	~Input();

	static Input* Instance();
	static void Release();

	static bool GetKeyDown(int key);
	static bool GetKey(int key);
	static bool GetKeyUp(int key);

	//static Vec2 MousePosition();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void UpdatePrevInput();
	void UpdateMouse(GLFWwindow* window);


};
#endif // !INPUT_CLASS_H
