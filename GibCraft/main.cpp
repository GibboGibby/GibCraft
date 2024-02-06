#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Window.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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

	GLfloat vertices[]
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	 0.8f, 0.3f, 0.02f, 1.0f, // Lower left
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	 0.8f,0.3f, 0.02f, 1.0f, // Lower right
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f,0.6f,0.32f, 1.0f, // Up
		-0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,	 0.9f, 0.45f, 0.17f, 1.0f,// Inner left
		 0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,	 0.9f, 0.45f, 0.17f, 1.0f,// Inner right
		 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, 	 0.8f, 0.3f, 0.02f, 1.0f// Inner down
	};

	GLuint indices[] = {
		0,3,5,
		3,2,4,
		5,4,1
	};

	GLfloat squareVerts[]
	{
		0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,-0.5f,0.0f,		0.0f,1.0f,0.0f,1.0f,
		-0.5f,0.5f,0.0f,		1.0f,0.0f,1.0f,1.0f
	};

	std::array<Vertex, 4> squareVertex = {
		Vertex{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		Vertex{{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		Vertex{{-0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f,1.0f}},
		Vertex{{-0.5f,0.5f,0.0f}, {1.0f,0.0f,1.0f,1.0f}}
	};
	GLuint squareIndices[]
	{
		0,1,2,
		3, 0, 2

	};

	GLuint squareHollowIndices[]
	{
		0,1, 1, 2, 2, 3, 3, 0
	};

	
	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();

	//VBO VBO1(vertices, sizeof(vertices));
	//EBO EBO1(indices, sizeof(indices));

	
	VBO VBO1(squareVertex.data(), sizeof(squareVertex));
	EBO EBO1(squareHollowIndices, sizeof(squareHollowIndices));
	
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 7 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	VAO1.LinkAttrib(VBO1, 1, 4, GL_FLOAT, 7 * sizeof(float), (void*)offsetof(Vertex, colour));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


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

		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Left mouse button down" << std::endl; 
		}
		if (Input::GetMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Left mouse button up" << std::endl;
		}

		
		// Set the background colour
		glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		// Clears the screen using the set colour
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

		// Swaps the front and back buffer as we always draw to the back buffer and swap when done
		glfwSwapBuffers(window.GetWindow());

		window.UpdateViewport();
		// Gets all the events from GLFW
		inputMan->UpdatePrevInput();
		inputMan->UpdateMouse(window.GetWindow());
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	Input::Release();
	glfwDestroyWindow(window.GetWindow());
	glfwTerminate();
}