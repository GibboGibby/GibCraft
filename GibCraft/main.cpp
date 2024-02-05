#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Window.h"
#include "ShaderClass.h"

int main()
{
	// Checks to see if glfw can initialize
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW");
		return -1;
	}
	
	// Specifies GLFW settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	Window window;
	window.Init(1280, 720, "Gibcraft (Minecraft Clone)");


	// Checks to see if OpenGL is loaded
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize glad.\n");
		return -1;
	}

	// Tells which pixels to draw to. (All of the screen)
	// Define Input
	Input* inputMan = Input::Instance();
	glfwSetKeyCallback(window.GetWindow(), inputMan->KeyCallback);
	glfwSetMouseButtonCallback(window.GetWindow(), inputMan->MouseButtonCallback);
	glfwSetScrollCallback(window.GetWindow(), inputMan->ScrollCallback);

	//Shader shader("default.vert", "default.frag");
	//shader.Activate();

	//Game loop (techincally)
	bool fullscreen = false;
	while (!glfwWindowShouldClose(window.GetWindow()))
	{

		if (Input::GetKeyDown(GLFW_KEY_F1))
		{
			fullscreen = !fullscreen;
			window.SetFullScreen(fullscreen);
		}

		if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
		{
			window.Close();
			return -1;
		}

		if (Input::MouseScrollDelta().y > 0)
		{
			std::cout << "Mouse scrolled up" << std::endl;
		}

		if (Input::MouseScrollDelta().y < 0)
		{
			std::cout << "Mouse scrolled down" << std::endl;
		}

		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			std::cout << "Left mouse button down" << std::endl;
		}
		if (Input::GetMouseButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
		{
			std::cout << "Left mouse button up" << std::endl;
		}

		
		// Set the background colour
		glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		// Clears the screen using the set colour
		glClear(GL_COLOR_BUFFER_BIT);
		// Swaps the front and back buffer as we always draw to the back buffer and swap when done
		glfwSwapBuffers(window.GetWindow());

		// Gets all the events from GLFW
		inputMan->UpdatePrevInput();
		inputMan->UpdateMouse(window.GetWindow());
		glfwPollEvents();
	}

	Input::Release();
	glfwTerminate();
}