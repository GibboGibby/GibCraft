#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Window.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <glm/ext.hpp>

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

	std::array<Vertex, 6> starVertices = {

		Vertex{{0.0f, 0.8f, 0.0f}, {1.0f,0.0f,1.0f,1.0f}}, // Top
		Vertex{{-0.3f, -0.8f, 0.0f}, {1.0f,1.0f,1.0f,1.0f}}, // BotLeft
		Vertex{{0.3f, -0.8f, 0.0f}, {0.0f,0.0f,0.0f,1.0f}}, // BotRight
		Vertex{{-0.6f, 0.3f, 0.0f}, {0.0f,1.0f,0.0f,1.0f}}, // TopLeft
		Vertex{{0.6f, 0.3f, 0.0f}, {1.0f,0.0f,0.0f,1.0f}}, // TopRight
		Vertex{{0.0f, -0.3f, 0.0f}, {0.0f,0.0f,1.0f,1.0f}}, // Mid

	};

	GLuint starIndices[] =
	{
		0, 1, 5, 
		0, 2, 5, 
		3, 4, 5, 
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


	std::array<Vertex, 8> cubeVertices = {
		Vertex{{0.5f, 0.5f, 0.5f}, {0.0f,1.0f,0.0f, 1.0f}}, // Front Top Right - 0
		Vertex{{0.5f, 0.5f, -0.5f}, {1.0f,0.0f,0.0f, 1.0f}}, // Back Top Right - 1
		Vertex{{-0.5f, 0.5f, -0.5f}, {1.0f,0.0f,0.0f, 1.0f}}, // Back Top Left - 2
		Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f,1.0f,0.0f, 1.0f}}, // Front Top Left - 3
		Vertex{{0.5f, -0.5f, 0.5f}, {0.0f,1.0f,0.0f, 1.0f}}, // Front Bottom Right - 4
		Vertex{{0.5f, -0.5f, -0.5f}, {1.0f,0.0f,0.0f, 1.0f}}, // Back Bottom Right - 5
		Vertex{{-0.5f, -0.5f, -0.5f}, {1.0f,0.0f,0.0f, 1.0f}}, // Back Bottom Left - 6
		Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f,1.0f,0.0f, 1.0f}}, // Front Bottom Left - 7
	};

	GLuint cubeIndices[]{
		0,3,1,
		0, 3, 4,
		7, 3, 4,
		0, 1, 4,
		5, 4, 1
	};

	GLuint cubeIndicesReversed[]{
		0,3,1,
		2,3,1,
		5,4,1,
		0,1,4,
		7,3,4,
		0,3,4
		
	};

	GLuint squareHollowIndices[]
	{
		0,1, 1, 2, 2, 3, 3, 0
	};

	struct SimpleObject
	{
		glm::vec3 pos;
	};
	glm::vec3 scale(1.0f,1.0f,1.0f);
	float rotation = 0;
	SimpleObject obj{glm::vec3(0.0f,0.0f,0.0f)};
	//glm::vec3 position;

	glm::mat4 transform(1);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, obj.pos);

	SimpleObject camera{ glm::vec3(0.0f,0.0f,5.0f) };
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = glm::lookAt(camera.pos, center, up);

	

	
	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();

	//VBO VBO1(vertices, sizeof(vertices));
	//EBO EBO1(indices, sizeof(indices));

	
	//VBO VBO1(squareVertex.data(), sizeof(squareVertex));
	//EBO EBO1(squareIndices, sizeof(squareIndices));
	VBO VBO1(cubeVertices.data(), sizeof(cubeVertices));
	EBO EBO1(cubeIndicesReversed, sizeof(cubeIndicesReversed));
	
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 7 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	VAO1.LinkAttrib(VBO1, 1, 4, GL_FLOAT, 7 * sizeof(float), (void*)offsetof(Vertex, colour));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	float angle = 0;
	float yAngle = 0;
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

		float moveSpeed = 1.0f;
		if (Input::GetKey(GLFW_KEY_A))
		{
			obj.pos += glm::vec3(-1.0f, 0.0f, 0.0f) * moveSpeed / 2.0f;
		}

		if (Input::GetKey(GLFW_KEY_D))
		{
			obj.pos += glm::vec3(1.0f, 0.0f, 0.0f) * moveSpeed / 2.0f;
		}

		if (Input::GetKey(GLFW_KEY_W))
		{
			obj.pos += glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed / 2.0f;
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			obj.pos += glm::vec3(0.0f, -1.0f, 0.0f) * moveSpeed / 2.0f;
		}

		if (Input::GetKey(GLFW_KEY_RIGHT))
		{
			angle += 1.0f * moveSpeed;
		}
		if (Input::GetKey(GLFW_KEY_LEFT))
		{
			angle += -1.0f * moveSpeed;
		}

		if (Input::GetKey(GLFW_KEY_UP))
		{
			yAngle += 1.0f * moveSpeed;
		}
		if (Input::GetKey(GLFW_KEY_DOWN))
		{
			yAngle += -1.0f * moveSpeed;
		}

		float fov = 45.f;
		float near = 0.01f;
		float far = 10000.0f;
		float windowAspect = ((float)window.Width() / (float)window.Height());
		glm::mat4 projection = glm::perspective(fov, windowAspect, near, far);

		//camera.pos.x = 2.0f * glm::sin(glm::pi<float>() * 2 * angle / 360);
		//camera.pos.z = 2.0f * glm::cos(glm::pi<float>() * 2 * angle / 360);

		glm::mat4 newViewMatrix = glm::lookAt(camera.pos, center, up);

		glm::mat4 newTransform(1);
		newTransform = glm::scale(newTransform, scale);
		newTransform = glm::rotate(newTransform, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		newTransform = glm::rotate(newTransform, glm::radians(yAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		newTransform = glm::translate(newTransform, obj.pos); // Movement is in local space not global
		// Need to update 

		// Set the background colour
		glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		// Clears the screen using the set colour
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();

		GLint transformLoc = glGetUniformLocation(shaderProgram.ID, "uTransform");
		GLint projectionLoc = glGetUniformLocation(shaderProgram.ID, "uProjection");
		GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "uView");

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(newTransform));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(newViewMatrix));



		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

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