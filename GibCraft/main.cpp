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
#include "Engine/MeshThread.h"
#include "Engine/GenThread.h"
#include <iomanip>

#include "Engine/ImGUI/imgui.h"
#include "Engine/ImGUI/imgui_impl_glfw.h"
#include "Engine/ImGUI/imgui_impl_opengl3.h"

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

	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);


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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);

	ImGui_ImplOpenGL3_Init("#version 460");

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

	
	float maxFPS = 1.0f / 144.0f;
	
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

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(NewVertex), (void*)offsetof(NewVertex, pos));
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
	int crosshairWidth;
	int crosshairHeight;
	int crosshairChannels;

	uint8_t* pixels = stbi_load("resources\\textures\\oak_planks.png", &texWidth, &texHeight, &numChannels, 0);
	uint8_t* dirtPixels = stbi_load("resources\\textures\\dirt.png", &texWidth, &texHeight, &numChannels, 0);
	uint8_t* textureAtlasPixels = stbi_load("resources\\textures\\atlas.png", &atlasWidth, &atlasHeight, &atlasNumChannels, 0);
	uint8_t* crosshairPixels = stbi_load("resources\\textures\\crosshair.png", &crosshairWidth, &crosshairHeight, &crosshairChannels, 0);

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


	/*
	uint32_t crosshairID;
	glGenTextures(1, &crosshairID);
	glBindTexture(GL_TEXTURE_2D, crosshairID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat, crosshairWidth, crosshairHeight, border, format, type, crosshairPixels);
	
	*/
	stbi_image_free(pixels);
	stbi_image_free(dirtPixels);
	stbi_image_free(textureAtlasPixels);
	//stbi_image_free(crosshairPixels);

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

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, crosshairID);

	//glActiveTexture(GL_TEXTURE0 + textureSlot);

	

	shaderProgram.UploadInt("uTexture", textureSlot);

	float fov = 52.5f;
	float nearPlane = 0.01f;
	float farPlane = 10000.0f;
	float windowAspect = ((float)window.Width() / (float)window.Height());

	Renderer* renderer = new Renderer();
	std::shared_ptr<FPSCamera> camera = std::make_shared<FPSCamera>(fov, windowAspect, nearPlane, farPlane, 0.25f);

	inputMan->ResetMousePositionDelta(window.GetWindow());
	
	World world(1254125, glm::vec2(0,0), "Gaming");
	world.UpdatePlayerPosition(camera->GetPosition());

	camera->SetPosition(glm::vec3(0.0f, 120.0f, 0.0f));
	std::cout << "size of block in bytes - " << sizeof(Block) << std::endl;
	std::cout << "size of blocktype in bytes - " << sizeof(BlockType) << std::endl;


	world.InitWorld();
	//world.CreateWorldGenThread(camera);
	GenThread* genThread = new GenThread(world.m_WorldChunks, camera, 1254125, 10, 14);
	MeshThread* meshThread = new MeshThread(world.m_WorldChunks, camera, 1254125, 10, 14);

	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	bool disable = true;

	//int bytes = sizeof(world.RetrieveChunkFromMap(0, 0)->pChunkContentsPtr->data());
	int bytes = sizeof(Block) * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
	std::cout << "size of chunk contents ptr data - " << bytes << std::endl;
	//world.Init();
	
	std::chrono::high_resolution_clock::time_point lastFrameStart = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point lastFrameFinish = std::chrono::high_resolution_clock::now();
	float baseMoveSpeed = 4.0f;
	float moveSpeedScalar = 3.0f;

	float sens = camera->GetSensitivity();
	float f = fov;
	int counter = 0;
	
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();
		//int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(frameStartTime - lastFrameFinish).count();
		//std::cout << "milliseconds number - " << milliseconds << std::endl;
		//float deltaTime = (float)milliseconds / 1000.0f;
		float deltaTime = std::chrono::duration<float>(frameStartTime - lastFrameStart).count();
		//int mil = std::chrono::duration_cast<std::chrono::milliseconds>(frameStartTime - lastFrameFinish).count();
		//std::cout << "milliseconds difference - " << mil << std::endl;
		std::cout << "deltaTime - " << deltaTime << std::fixed << std::setprecision(3) << std::endl;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("fov", &f, 40.0f, 120.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("sensitivity", &sens, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("base move speed", &baseMoveSpeed, 1.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("sprint multiplier", &moveSpeedScalar, 2.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		camera->SetFov(f);
		camera->SetSensitivity(sens);
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

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

		if (Input::GetKeyDown(GLFW_KEY_F3))
		{
			disable = !disable;
			if (disable) glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		//float moveSpeed = 4.0f;
		float moveSpeed = baseMoveSpeed;
		if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
		{
			moveSpeed *= 3.0f;
		}

		if (Input::GetKey(GLFW_KEY_A))
		{
			//camera->SetPosition(camera->GetPosition() + (camera->GetFront() * (glm::vec3(-1.0f, 0.0f, 0.0f) * -moveSpeed)));
			camera->SetPosition(camera->GetPosition() + (camera->GetRight() * -moveSpeed * deltaTime));
		}

		if (Input::GetKey(GLFW_KEY_D))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetRight() * moveSpeed * deltaTime));
		}

		if (Input::GetKey(GLFW_KEY_W))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetFront() * moveSpeed * deltaTime));
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			camera->SetPosition(camera->GetPosition() + (camera->GetFront() * -moveSpeed * deltaTime));
		}

		if (Input::GetKey(GLFW_KEY_Q))
		{
			camera->SetPosition(camera->GetPosition() + (glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed * deltaTime));
		}
		if (Input::GetKey(GLFW_KEY_E))
		{
			camera->SetPosition(camera->GetPosition() + (glm::vec3(0.0f, -1.0f, 0.0f) * moveSpeed * deltaTime));
		}


		//shaderProgram.Activate();
		glm::vec2 mousePosDelta = Input::MousePositionDelta();
		//
		//  << "This is the mouse delta: x - " << mousePosDelta.x << "   y - " << mousePosDelta.y << std::endl;
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

		if (Input::GetKeyDown(GLFW_KEY_1)) camera->blockInHand = BlockType::DIRT;
		if (Input::GetKeyDown(GLFW_KEY_2)) camera->blockInHand = BlockType::GRASS;
		if (Input::GetKeyDown(GLFW_KEY_3)) camera->blockInHand = BlockType::STONE;
		if (Input::GetKeyDown(GLFW_KEY_4)) camera->blockInHand = BlockType::COBBLESTONE;
		if (Input::GetKeyDown(GLFW_KEY_5)) camera->blockInHand = BlockType::OAK_PLANKS;
		if (Input::GetKeyDown(GLFW_KEY_6)) camera->blockInHand = BlockType::GLASS;
		if (Input::GetKeyDown(GLFW_KEY_7)) camera->blockInHand = BlockType::SAND;


		if (Input::GetKeyDown(GLFW_KEY_P))
		{
			std::shared_ptr<Chunk> thing = world.RetrieveChunkFromMap(0,0);
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

		
		world.UpdateAddToChunks();
		//world.Update();
		world.UpdateViewFrustum(camera);
		world.RenderWorld(camera);

		world.UpdateFramePause();

		
		/*
		renderer->StartChunkRendering(camera, 5);
		renderer->RenderChunk(chunk);
		renderer->EndChunkRendering();
		*/

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window.GetWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		// Swaps the front and back buffer as we always draw to the back buffer and swap when done
		glfwSwapBuffers(window.GetWindow());

		window.UpdateViewport();
		// Gets all the events from GLFW
		inputMan->UpdatePrevInput();
		inputMan->UpdateMouse(window.GetWindow());
		glfwPollEvents();
		lastFrameFinish = std::chrono::high_resolution_clock::now();
		lastFrameStart = frameStartTime;
		auto fpsMax = std::chrono::duration<float>(maxFPS);
		auto miliseconds = std::chrono::duration<float>(lastFrameFinish - frameStartTime);
		//std::cout << "END OF FRAME\nMiliseconds count - " << miliseconds.count() << "\nfpsMax - " << maxFPS << std::endl;
		//float miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(lastFrameFinish - frameStartTime).count() / 1000.0f;
		if (miliseconds.count() < maxFPS)
		{
			std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(fpsMax - miliseconds));
			//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
	shaderProgram.Delete();

	
	Input::Release();
	glfwDestroyWindow(window.GetWindow());
	glfwTerminate();

}