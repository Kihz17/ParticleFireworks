#pragma once

#include "GLCommon.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "World.h"
#include "Fireworks.h"
#include "PhysicsUtils.h"

const float windowWidth = 1200;
const float windowHeight = 640;
bool editMode = true;

ShaderManager gShaderManager;

Camera camera(windowHeight, windowWidth);
float moveSpeed = 1.1f;

int currentEditIndex = 0;

bool onePressed = false;
bool twoPressed = false;
bool threePressed = false;
bool fourPressed = false;

static bool copyFile(std::string src, std::string dest)
{
	std::ifstream ifSrc(src, std::ios::binary);
	std::ofstream ofDest(dest, std::ios::binary);
	ofDest << ifSrc.rdbuf();
	return ifSrc && ofDest;
}

static bool doesFileExist(std::string path)
{
	std::ifstream ifs(path);
	return ifs.good();
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Movement controls
	if (!editMode)
	{
		if (key == GLFW_KEY_W)
		{
			camera.position += camera.direction * moveSpeed;
		}
		if (key == GLFW_KEY_A)
		{
			// Rotate our camera's direction 90 degrees to the left
			glm::vec3 left;
			constexpr float theta = glm::radians(90.0f);
			left.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
			left.y = 0.0f;
			left.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
			camera.position += left * moveSpeed;
		}
		if (key == GLFW_KEY_S)
		{
			camera.position -= camera.direction * moveSpeed;
		}
		if (key == GLFW_KEY_D)
		{
			// Rotate our camera's direction 90 degrees to the right
			glm::vec3 right;
			constexpr float theta = glm::radians(-90.0f);
			right.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
			right.y = 0.0f;
			right.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
			camera.position += right * moveSpeed;
		}
		if (key == GLFW_KEY_SPACE)
		{
			camera.position.y += moveSpeed;
		}
	}

	if (key == GLFW_KEY_1)
	{
		if (action == GLFW_PRESS)
		{
			onePressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			onePressed = false;
		}
	}
	if (key == GLFW_KEY_2)
	{
		if (action == GLFW_PRESS)
		{
			twoPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			twoPressed = false;
		}
	}
	if (key == GLFW_KEY_3)
	{
		if (action == GLFW_PRESS)
		{
			threePressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			threePressed = false;
		}
	}
	if (key == GLFW_KEY_4)
	{
		if (action == GLFW_PRESS)
		{
			fourPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			fourPressed = false;
		}
	}

	// Toggle cursor view
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		editMode = !editMode;
		int cursorOption = editMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, cursorOption);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!editMode)
	{
		camera.MoveCamera(xpos, ypos);
	}
}

bool InitializerShaders();

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Initialize our window
	window = glfwCreateWindow(windowWidth, windowHeight, "Midterm", NULL, NULL);

	// Make sure the window initialized properly
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback); // Tell GLFW where our key callbacks are
	glfwSetCursorPosCallback(window, mouse_callback); // Tell GLFW where our mouse callback is

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Give glad this process ID
	glfwSwapInterval(1);

	if (!InitializerShaders())
	{
		return -1;
	}

	// Setup world
	World* world = new World();
	std::vector<Firework*> fireworks;

	// Load in scene
	std::stringstream ss;
	ss << SOLUTION_DIR << "Extern\\assets\\models\\box.obj";
	Model ground(ss.str());
	ss.str("");

	ss << SOLUTION_DIR << "Extern\\assets\\models\\SoccerBall.obj";
	Model sphere(ss.str());
	ss.str("");
	
	CompiledShader shader = *gShaderManager.pGetShaderProgramFromFriendlyName("Shader#1");

	camera.position = glm::vec3(150.0f, 25.0f, 0.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f) - camera.position;
	camera.direction = cameraDirection;

	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;

	float previousTime = static_cast<float>(glfwGetTime());

	// Our actual render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// FPS TITLE
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				glfwSetWindowTitle(window, newTitle.c_str());

	
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}

		float ratio;
		int width, height;
		glm::mat4 projection;
		glm::mat4 view;

		glfwGetFramebufferSize(window, &width, &height); // Assign width and height to our window width and height
		ratio = width / (float)height;

		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST); // Enables the Depth Buffer, which decides which pixels will be drawn based on their depth (AKA don't draw pixels that are behind other pixels)

		glViewport(0, 0, width, height); // Specifies the transformation of device coords to window coords 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the buffers

		view = camera.GetViewMatrix();
		projection = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);

		glUseProgram(shader.ID);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matView"), 1, GL_FALSE, glm::value_ptr(view)); // Assign new view matrix
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matProjection"), 1, GL_FALSE, glm::value_ptr(projection)); // Assign projection

		if (onePressed)
		{
			Firework* firework = new BaseFireworkNormal(glm::vec3(getRandom(-5.0, 5.0), 0.0f, getRandom(-5.0, 5.0)));
			fireworks.push_back(firework);
			world->AddParticle(firework);
			firework->SetVelocity(glm::vec3(0.0f, 30.0f, 0.0f));
			onePressed = false;
		}
		if (twoPressed)
		{
			Firework* firework = new ConeFirework(glm::vec3(getRandom(-5.0, 5.0), 0.0f, getRandom(-5.0, 5.0)));
			fireworks.push_back(firework);
			world->AddParticle(firework);
			firework->SetVelocity(glm::vec3(0.0f, 30.0f, 0.0f));
			twoPressed = false;
		}
		if (threePressed)
		{
			Firework* firework = new RotatingFirework(glm::vec3(getRandom(-5.0, 5.0), 0.0f, getRandom(-5.0, 5.0)));
			fireworks.push_back(firework);
			world->AddParticle(firework);
			firework->SetVelocity(glm::vec3(0.0f, 30.0f, 0.0f));
			threePressed = false;
		}

		std::vector<Firework*> fireworksToAdd; // Represents a batch of the next fireworks to add to the world

		// Validate all registered particles
		for (std::vector<Firework*>::iterator it = fireworks.begin(); it != fireworks.end();)
		{
			Firework* firework = *it;
			if (firework->IsValid())
			{
				it++;
			}
			else
			{ // We are no longer valid

				// Generate particles for the next stage
				std::vector<Firework*> newFireworks = firework->BeginNextStage();

				// Register the newly generated particles for the next stage
				if (!newFireworks.empty())
				{
					for (std::vector<Firework*>::iterator newFireworkIt = newFireworks.begin(); newFireworkIt != newFireworks.end(); newFireworkIt++)
					{
						fireworksToAdd.push_back(*newFireworkIt); // Flag this firework to be added to the world
					}
				}

				it = fireworks.erase(it);
				world->RemoveParticle(firework);
				delete firework; // We are now forever done with this particle, remove it from memory
			}
		}

		// Add the new fireworks to the world
		for (Firework* f : fireworksToAdd)
		{
			fireworks.push_back(f);
			world->AddParticle(f);
		}
		fireworksToAdd.clear();

		// Integrate all particles in the world
		world->Tick(deltaTime);

		// Safety, mostly for first frame
		if (deltaTime == 0.0f)
		{
			deltaTime = 0.03f;
		}

		// Render the models
		ground.Draw(shader, glm::vec3(-10.0f, -1.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 0.2f, 10.0f));

		glm::vec3 position(0.0f);
		for (Firework* f : fireworks)
		{
			f->GetPosition(position);
			sphere.Draw(shader, position, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5f));
		}

		// Show what we've drawn
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (Firework* f : fireworks)
	{
		world->RemoveParticle(f);
		delete f;
	}

	delete world;

	glfwDestroyWindow(window); // Clean up the window

	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}


bool InitializerShaders()
{
	std::stringstream ss;

	// "Normal" Shader
	Shader vertexShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\vertexShader.glsl";
	vertexShader.fileName = ss.str();
	ss.str("");

	Shader fragmentShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\fragmentShader.glsl";
	fragmentShader.fileName = ss.str();
	ss.str("");

	bool success = gShaderManager.createProgramFromFile("Shader#1", vertexShader, fragmentShader);
	if (success)
	{
		std::cout << "Shaders compiled OK" << std::endl;
	}
	else
	{
		std::cout << "Error making shaders: " << std::endl;
		std::cout << gShaderManager.getLastError() << std::endl;
		return 1;
	}

	return success;
}