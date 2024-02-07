#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <GLFW/glfw3.h>
#include "GibMaths.h"
#include <stdexcept>
#include <array>
class Window {
private:
	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;

	std::array<int, 2> wndPos{ 0,0 };
	std::array<int, 2> wndSize{ 0,0 };
	std::array<int, 2> vpSize{ 0,0 };
	bool updateViewport = true;

	int _width;
	int _height;

	bool isFullscreen;

	void Resize(int cx, int cy);
public:
	void Init(int width, int height, const char* name);
	static void CallbackResize(GLFWwindow* windwow, int cx, int cy);
	//void MainLoop();
	bool IsFullscreen();
	void SetFullScreen(bool fullscreen);

	GLFWwindow* GetWindow();
	void UpdateViewport();

	void Close();
	int Width();
	int Height();
};

#endif // !WINDOW_CLASS_H
