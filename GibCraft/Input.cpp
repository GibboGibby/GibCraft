#include "Input.h"


Input* Input::instance = NULL;

Vec2 Input::mousePosition = { 0,0 };

Input* Input::Instance()
{
	if (instance == NULL)
		instance = new Input();

	return instance;
}

void Input::Release()
{
	
	delete instance;
	instance = NULL;
}


Input::Input()
{
	memset(keys, 0, 348);
	memset(prevKeys, 0, 348);
}

Input::~Input()
{
	delete[] keys;
	delete[] prevKeys;
}

bool Input::GetKey(int key)
{
	//return (instance->keyMap[key] == GLFW_REPEAT) || (instance->keyMap[key]  == GLFW_PRESS);
	return instance->keys[key];
}

bool Input::GetKeyDown(int key)
{
	return instance->keys[key] && !instance->prevKeys[key];
}

bool Input::GetKeyUp(int key)
{
	return !instance->keys[key] && instance->prevKeys[key];
}

/*
Vec2 Input::MousePosition()
{
	return Input::mousePosition;
}
*/



void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	instance->keys[key] = (action == GLFW_RELEASE) ? false : true;
}

void Input::UpdatePrevInput()
{
	memcpy(prevKeys, keys, 348);
}

void Input::UpdateMouse(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	Input::mousePosition = { static_cast<float>(xpos), static_cast<float>(ypos) };
}