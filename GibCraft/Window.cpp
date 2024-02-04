#include "Window.h"

void Window::Init(int width, int height, const char* name)
{
	window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Error initializing window");
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, Window::CallbackResize);

	monitor = glfwGetPrimaryMonitor();
	glfwGetWindowSize(window, &wndSize[0], &wndSize[1]);
	glfwGetWindowPos(window, &wndPos[0], &wndSize[1]);
	updateViewport = true;
}

void Window::CallbackResize(GLFWwindow* window, int cx, int cy)
{
	void* ptr = glfwGetWindowUserPointer(window);
	if (Window* windowPtr = static_cast<Window*>(ptr))
		windowPtr->Resize(cx, cy);
}

void Window::Resize(int cx, int cy)
{
	updateViewport = true;
}



bool Window::IsFullscreen()
{
	return glfwGetWindowMonitor(window) != nullptr;
}

void Window::SetFullScreen(bool fullscreen)
{
	if (IsFullscreen() == fullscreen) return;

	if (fullscreen)
	{
		glfwGetWindowPos(window, &wndPos[0], &wndPos[1]);
		glfwGetWindowSize(window, &wndSize[0], &wndSize[1]);

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, wndPos[0], wndPos[1], wndSize[0], wndSize[1], 0);
	}
	updateViewport = true;
}

GLFWwindow* Window::GetWindow()
{
	return window;
}

void Window::UpdateViewport()
{
	if (updateViewport)
	{
		glfwGetFramebufferSize(window, &vpSize[0], &vpSize[1]);
		glViewport(0, 0, vpSize[0], vpSize[1]);
		updateViewport = false;
	}
}

void Window::Close()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
