#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine/Input/Input.h"
#include "Engine/Window.h"
#include "Engine/ShaderClass.h"
#include "Engine/VAO.h"
#include "Engine/VBO.h"
#include "Engine/EBO.h"
#include <glm/ext.hpp>
#include "Engine/Cube.h"
#include "Engine/TesselatedPlane.h"

//#include "Engine/CubeRenderer.h"
#include "Engine/Renderer.h"
#include "Engine/FPSCamera.h"

#include "Engine/World.h"
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

	struct NewVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	NewVertex newVertexesSquare[]
	{
		{{0.5f,0.5f,0.0f},{0.0f,0.0f,1.0f,1.0f}},
		{{0.5f,-0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f}},
		{{-0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f,1.0f}},
		{{-0.5f,0.5f,0.0f},{1.0f,0.0f,1.0f,1.0f}}
	};


	/*
	std::array<Vertex, 6> starVertices = {

		Vertex{{0.0f, 0.8f, 0.0f}, {1.0f,0.0f,1.0f,1.0f}}, // Top
		Vertex{{-0.3f, -0.8f, 0.0f}, {1.0f,1.0f,1.0f,1.0f}}, // BotLeft
		Vertex{{0.3f, -0.8f, 0.0f}, {0.0f,0.0f,0.0f,1.0f}}, // BotRight
		Vertex{{-0.6f, 0.3f, 0.0f}, {0.0f,1.0f,0.0f,1.0f}}, // TopLeft
		Vertex{{0.6f, 0.3f, 0.0f}, {1.0f,0.0f,0.0f,1.0f}}, // TopRight
		Vertex{{0.0f, -0.3f, 0.0f}, {0.0f,0.0f,1.0f,1.0f}}, // Mid

	};
	*/

	GLuint starIndices[] =
	{
		0, 1, 5, 
		0, 2, 5, 
		3, 4, 5, 
	};

	/*
	std::array<Vertex, 4> squareVertex = {
		Vertex{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		Vertex{{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		Vertex{{-0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f,1.0f}},
		Vertex{{-0.5f,0.5f,0.0f}, {1.0f,0.0f,1.0f,1.0f}}
	};
	*/
	GLuint squareIndices[]
	{
		0,1,2,
		3, 0, 2

	};

	/*
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
	*/

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

	SimpleObject oldcamera{ glm::vec3(0.0f,0.0f,5.0f) };
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = glm::lookAt(oldcamera.pos, center, up);

	

	
	Shader shaderProgram("shaders/vertex/default.vert", "shaders/fragment/default.frag");
	Shader newShaderProgram("shaders/vertex/basic.vert", "shaders/fragment/basic.frag");
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	VAO1.Bind();
	VBO1.Bind();
	EBO1.Bind();
	VBO1.BufferData(sizeof(newVertexesSquare), newVertexesSquare, GL_STATIC_DRAW);
	EBO1.BufferData(sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(NewVertex), (void*)offsetof(NewVertex,pos));
	VAO1.LinkAttrib(VBO1, 1, 4, GL_FLOAT, sizeof(NewVertex), (void*)offsetof(NewVertex, color));
	
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Cube cube;
	//CubeRenderer cubeRenderer;
	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 planePos = glm::vec3(50.0f, 0.0f, 0.0f);

	
	TesselatedPlane plane(3, 50);

	float angle = 0;
	float yAngle = 0;
	//Game loop (techincally)
	bool fullscreen = false;

	float amplitude = 1.0f;
	float frequency = 0.5f;
	glClearDepth(1.0f);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	//glMatrixMode(GL_PROJECTION);

	int texWidth;
	int texHeight;
	int numChannels;
	int atlasWidth;
	int atlasHeight;
	int atlasNumChannels;

	uint8_t* pixels = stbi_load("resources\\textures\\oak_planks.png", &texWidth, &texHeight, &numChannels, 0);
	uint8_t* dirtPixels = stbi_load("resources\\textures\\dirt.png", &texWidth, &texHeight, &numChannels, 0);
	uint8_t* textureAtlasPixels = stbi_load("resources\\textures\\atlas.png", &atlasWidth, &atlasHeight, &atlasNumChannels, 0);

	uint32_t woodenPlanksID;
	glGenTextures(1, &woodenPlanksID);
	glBindTexture(GL_TEXTURE_2D, woodenPlanksID);

	// Texture Parameters
	bool pixelated = true;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

	uint32_t mipLevel = 0;
	uint32_t internalFormat = GL_RGBA32F;
	uint32_t width = texWidth;
	uint32_t height = texHeight;
	uint32_t border = 0;
	uint32_t format = GL_RGB;
	uint32_t type = GL_UNSIGNED_BYTE;

	
	(GL_TEXTURE_2D, mipLevel, internalFormat, width, height, border, format, type, pixels);

	
	uint32_t dirtID;
	glGenTextures(1, &dirtID);
	glBindTexture(GL_TEXTURE_2D, dirtID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat, width, height, border, format, type, dirtPixels);

	uint32_t texAtlasID;
	glGenTextures(1, &texAtlasID);
	glBindTexture(GL_TEXTURE_2D, texAtlasID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat, atlasWidth, atlasHeight, border, format, type, textureAtlasPixels);

	
	stbi_image_free(pixels);
	stbi_image_free(dirtPixels);
	stbi_image_free(textureAtlasPixels);

	/*
	int textureSlot = 0;
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, woodenPlanksID);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dirtID);
	
	*/
	int textureSlot = 0;
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, texAtlasID);

	shaderProgram.UploadInt("uTexture", textureSlot);

	float fov = 45.f;
	float near = 0.01f;
	float far = 10000.0f;
	float windowAspect = ((float)window.Width() / (float)window.Height());

	Renderer* renderer = new Renderer();
	FPSCamera* camera = new FPSCamera(fov, windowAspect, near, far, 0.25f);

	inputMan->ResetMousePositionDelta(window.GetWindow());
	
	World world(1254125, glm::vec2(0,0), "Gaming");
	world.UpdatePlayerPosition(camera->GetPosition());
	world.Update();
	//world.CreateWorldGenThread();
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//world.Init();
	
	
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
		float moveSpeed = 1.0f;
		if (Input::GetKey(GLFW_KEY_A))
		{
			//camera->SetPosition(camera->GetPosition() + (camera->GetFront() * (glm::vec3(-1.0f, 0.0f, 0.0f) * -moveSpeed)));
			camera->SetPosition(camera->GetPosition() + (camera->GetRight() * -moveSpeed));
		}

		if (Input::GetKey(GLFW_KEY_D))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetRight() * moveSpeed));
		}

		if (Input::GetKey(GLFW_KEY_W))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetFront() * moveSpeed));
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetFront() * -moveSpeed));
		}

		if (Input::GetKey(GLFW_KEY_Q))
		{
			camera->SetPosition(camera->GetPosition() + (glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed * 0.3f));
		}
		if (Input::GetKey(GLFW_KEY_E))
		{
			camera->SetPosition(camera->GetPosition() + (glm::vec3(0.0f, -1.0f, 0.0f) * moveSpeed *0.3f));
		}


		//shaderProgram.Activate();
		glm::vec2 mousePosDelta = Input::MousePositionDelta();
		//std::cout << "This is the mouse delta: x - " << mousePosDelta.x << "   y - " << mousePosDelta.y << std::endl;
		camera->UpdateOnMouseMovement(Input::mousePosition.x, Input::mousePosition.y);


		
		// Set the background colour
		glClearColor(250.0f / 255.0f, 119.0f / 255.0f, 110.0f / 255.0f, 1.0f);
		// Clears the screen using the set colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*
		VAO1.Bind();
		newShaderProgram.Activate();
		glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, (void*)0);
		VAO1.Unbind();
		*/
		//cube.DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), shaderProgram);
		//plane.DrawPlane(glm::vec3(0.0f, 0.0f, 0.0f), shaderProgram);

		glUseProgram(0);

		/*
		if (chunk->pMeshState == ChunkMeshState::Unbuilt)
			chunk->Construct(chunkForward, chunkBack, chunkLeft, chunkRight);
		*/

		if (Input::GetKeyDown(GLFW_KEY_P))
		{
			Chunk* thing = world.RetrieveChunkFromMap(0,0);
			for (int x = 0; x < CHUNK_SIZE_X; x++)
			{ 
				for (int y = 0; y < CHUNK_SIZE_Y; y++)
				{
					for (int z = 0; z < CHUNK_SIZE_Z; z++)
					{
						thing->SetBlock(BlockType::AIR, glm::vec3(x, y, z));
						thing->pMeshState = ChunkMeshState::Unbuilt;
					}
				}
			}
		}
		camera->Refresh();
		world.UpdatePlayerPosition(camera->GetPosition());
		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
		{
			world.Raycast(false, camera);
		}

		if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_2))
		{
			world.Raycast(true, camera);
		}

		
		
		world.Update();
		//world.RenderSingleChunk(0, 0, camera);
		//world.RenderSingleChunk(1, 0, camera);
		//world.RenderSingleChunk(0, 1, camera);
		world.RenderWorld(camera);

		world.UpdateFramePause();

		
		/*
		renderer->StartChunkRendering(camera, 5);
		renderer->RenderChunk(chunk);
		renderer->EndChunkRendering();
		*/


		// Swaps the front and back buffer as we always draw to the back buffer and swap when done
		glfwSwapBuffers(window.GetWindow());

		window.UpdateViewport();
		// Gets all the events from GLFW
		inputMan->UpdatePrevInput();
		inputMan->UpdateMouse(window.GetWindow());
		glfwPollEvents();
	}

	VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
	shaderProgram.Delete();


	Input::Release();
	glfwDestroyWindow(window.GetWindow());
	glfwTerminate();
}