#define GLEW_STATIC
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"
#include "ffMesh.h"
#include "ffModel.h"
#include "Character.h"
#include "IcePiton.h"
#include "Wall.h"
#include "Gem.h"
#include "Key.h"
#include "Gate.h"
#include "Cabin.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

int cameraMode = 1; // 0: third person view; 1: top down view;
bool shoot = false;
float lastShootTime = 0;


// renderQuad() renders a 1x1 XY quad in NDC
// This is used to debug shadow map 
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


#pragma region Model Data
//float vertices[] = {
//	// positions          // normals           // texture coords
//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
//};

float vertices_plane[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.0f, -0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  1.0f,
	 0.5f, -0.0f, -0.5f,  0.0f, 0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f, -0.0f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f, -0.0f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f,  0.0f,
	-0.5f, -0.0f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	-0.5f, -0.0f, -0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  1.0f,
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
}; 

float lightSourceVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

float vertices_ground[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f,   // top right
	 0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,   // bottom right
	 0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 100.0f    // top left 
};

unsigned int indices_ground[] = {
	0, 1, 2,
	2, 3, 0
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
#pragma endregion

#pragma region Camera Declaration
// Initialize camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 3.0f, 20.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region LightDeclaration
//                     default posititon glm::vec3(10.0f, 30.0f, 20.0f)
//                     + glm::vec3(0.0f, 0.0f, 100.0f) is an offset for rendering shadow (shadow in distance may be cut since the limitation of depth map)
LightDirectional lightD(glm::vec3(10.0f, 30.0f, 20.0f) + glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(glm::radians(110.0f), glm::radians(30.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
LightDirectional lightD1(glm::vec3(0.0f, 20.0f, 50.0f), glm::vec3(glm::radians(-215.0f), glm::radians(0.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
LightPoint lightP0(glm::vec3(15.0f, 15.0f, 15.0f), glm::vec3(0, glm::radians(45.0f), 0), glm::vec3(1.0f, 1.0f, 1.0f));
LightPoint lightP1(glm::vec3(0.0f, 5.0f, -15.0f), glm::vec3(0, glm::radians(45.0f), 0), glm::vec3(0.0f, 1.0f, 0.0f));
LightPoint lightP2(glm::vec3(0.0f, -5.0f, 5.0f), glm::vec3(0, glm::radians(45.0f), 0), glm::vec3(0.0f, 0.0f, 1.0f));
LightPoint lightP3(glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0, glm::radians(45.0f), 0), glm::vec3(0.0f, 1.0f, 1.0f));
LightSpot lightS(glm::vec3(0.0f, 5.0f, -3.0f), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(1.0f, 1.0f, 0.0f), 0.9, 0.85);
#pragma endregion

#pragma region Camera Operation Funtions
double previous_xPos, previous_yPos;
bool first_initialise_mouse = true;

void processInput_camera(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.SpeedZ = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.SpeedZ = -1.0;
	}
	else
	{
		camera.SpeedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.SpeedX = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.SpeedX = -1.0;
	}
	else
	{
		camera.SpeedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.SpeedY = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.SpeedY = -1.0;
	}
	else
	{
		camera.SpeedY = 0;
	}
}

void mouse_callback_camera(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos = xPos;
		previous_yPos = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos;
	double Delta_y = yPos - previous_yPos;

	previous_xPos = xPos;
	previous_yPos = yPos;

	//cout << Delta_x << "   " << Delta_y << endl;
	camera.ProcessMouseMovement(Delta_x, Delta_y);
}

void mouse_callback_camera_ThirdPersonView(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos = xPos;
		previous_yPos = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos;
	double Delta_y = yPos - previous_yPos;

	previous_xPos = xPos;
	previous_yPos = yPos;

	//cout << Delta_x << "   " << Delta_y << endl;
	camera.ProcessMouseMovement_ThirdPersonView(Delta_x, Delta_y);
}

void scroll_callback_camera_ThirdPersonView(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.fov -= (float)yoffset;
	if (camera.fov < 3.0f)
		camera.fov = 3.0f;
	if (camera.fov > 45.0f)
		camera.fov = 45.0f;
}

#pragma endregion

#pragma region Shoot icepiton
void ShootIcePiton(vector<IcePiton>& icePitons, Model icePiton_model, glm::vec3 position, glm::vec3 forward)
{
	IcePiton icePiton(
		/*Model(exePath.substr(0, exePath.find_last_of('\\')) + "\\IcePiton_sphere\\IcePiton.obj"),*/
		icePiton_model,
		position,     // position
		forward       // forward direction
	);

	icePitons.push_back(icePiton);
}

float processInput_shootIcePiton(GLFWwindow* window, Character& character, vector<IcePiton>& icePitons, Model icePiton_model, float time)
{
	float coolDownTime = 0.5;
	float currentTime = glfwGetTime();
	float elapsedTime = currentTime - time;
	//std::string path = exePath.substr(0, exePath.find_last_of('\\')) + "\\IcePiton_sphere\\IcePiton.obj";

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && elapsedTime >= coolDownTime)
	{
		ShootIcePiton(icePitons, icePiton_model, character.position, character.oriantation);
		shoot = true;
		lastShootTime = glfwGetTime();
		return currentTime;
	}
	else return time;
}

#pragma endregion

#pragma region Character Operation Funtions
double previous_xPos_character, previous_yPos_character;
bool first_initialise_mouse_character = true;

void processInput_character(GLFWwindow* window, Character& character)
{
	if (shoot)
	{
		character.speedForward = 0;
		character.speedRight = 0;
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		character.speedForward = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		character.speedForward = -1;
	}
	else
	{
		character.speedForward = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		character.speedRight = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		character.speedRight = -1;
	}
	else
	{
		character.speedRight = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		character.speedUp = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		character.speedUp = -1;
	}
	else
	{
		character.speedUp = 0;
	}
}

void mouse_callback_character(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos_character = xPos;
		previous_yPos_character = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos_character;
	double Delta_y = yPos - previous_yPos_character;

	previous_xPos_character = xPos;
	previous_yPos_character = yPos;

	//cout << Delta_x << "   " << Delta_y << endl;
	camera.ProcessMouseMovement(Delta_x, Delta_y);
}
#pragma endregion

#pragma region Texture Loading Function
unsigned int LoadTextureImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot)
{
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);

	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Texture image " << filename << " load failed." << endl;
	}
	stbi_image_free(data);

	//cout << filename << "Slot:  " << TexBuffer << endl;

	return TexBuffer;
}
#pragma endregion

#pragma region Skybox Loading Function
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
#pragma endregion

#pragma region Spawn Monster
float monsterSpawner(vector<Character>& monsters, Model monster_model, float time)
{
	int spawnCD = 10; // default 10
	int x_min = -30, x_max = 30;
	int z_min = -30, z_max = 30;

	float currentTime = glfwGetTime();
	//glm::vec3 position = glm::vec3((rand() % (x_max - x_min + 1)) + x_min, 0.0f, (rand() % (z_max - z_min + 1)) + z_min);

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(-30.0f, 0.0f, -30.0f));
	positions.push_back(glm::vec3(-30.0f, 0.0f, -70.0f));
	positions.push_back(glm::vec3(30.0f, 0.0f, -70.0f));
	positions.push_back(glm::vec3(60.0f, 0.0f, 20.0f));

	//float speed = rand() / double(RAND_MAX) * 0.4;

	if (currentTime - time >= spawnCD)
	{
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			Character monster_new(
				monster_model,
				positions[i],     // position
				glm::vec3(1.5f, 1.5f, 1.5f),      // scale
				rand() / double(RAND_MAX) * 0.3 + 0.1    // speed
			);
			monsters.push_back(monster_new);
		}
		

		return currentTime;
	}
	else
		return time;

}
#pragma endregion

#pragma region Switch Camera Mode
void processInput_switchCameraMode(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)  // Third person view
	{
		cameraMode = 1;
		camera.Position = glm::vec3(0.0f, 5.0f, -50.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)  // Top down view
		cameraMode = 2;
}
#pragma endregion

int main(int argc, char* argv[])
{
	std::string exePath = argv[0];
	//std::cout << exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\backpack.obj" << std::endl;

	// This is for fbx mdoel with animation, use this after you implemented aniamiton classes 
	//float _currentTime = glfwGetTime();
	//float _deltaTime = _currentTime - _lastTime;
	//_lastTime = _currentTime;

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "My openGL game", NULL, NULL);

	if (window == NULL)
	{
		cout << "Open window failed." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init GlEW failed." << endl;
		glfwTerminate();
		return -1;
	}

	//glfwSetCursorPosCallback(window, mouse_callback_camera);
	glfwSetCursorPosCallback(window, mouse_callback_camera_ThirdPersonView);
	glfwSetScrollCallback(window, scroll_callback_camera_ThirdPersonView);

	//glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

#pragma region Init Shader Program
	Shader* shader_character = new Shader("ffVertexShader_character.vert", "FragmentShader_character.frag");
	//Shader* shader_character = new Shader("VertexShader_character.vert", "FragmentShader_character.frag");
	Shader* shader_monster = new Shader("VertexShader_monster.vert", "FragmentShader_monster.frag");
	Shader* shader_icepiton = new Shader("VertexShader_icepiton.vert", "FragmentShader_icepiton.frag");
	Shader* shader_ground = new Shader("VertexShader_ground.vert", "FragmentShader_ground.frag");
	Shader* shader_LightSource = new Shader("VertexShader_LightSource.vert", "FragmentShader_LightSource.frag");
	Shader* shader_skybox = new Shader("VertexShader_skybox.vert", "FragmentShader_skybox.frag");
	Shader* shader_shadowMap = new Shader("VertexShader_shadowMap.vert", "FragmentShader_shadowMap.frag");
	Shader* shader_shadowDebug = new Shader("VertexShader_shadowDebug.vert", "FragmentShader_shadowDebug.frag");
	Shader* shader_wall = new Shader("VertexShader_wall.vert", "FragmentShader_wall.frag");
	Shader* shader_UI_item = new Shader("VertexShader_UI_item.vert", "FragmentShader_UI_item.frag");
	Shader* shader_endScene = new Shader("VertexShader_endScene.vert", "FragmentShader_endScene.frag");
	Shader* shader_star = new Shader("VertexShader_star.vert", "FragmentShader_star.frag");
#pragma endregion 

#pragma region Pass light to shaders;
	shader_character->use();
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	/*glUniform3f(glGetUniformLocation(shader_character->ID, "lightD1.pos"), lightD1.position.x, lightD1.position.y, lightD1.position.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightD1.color"), lightD1.color.x, lightD1.color.y, lightD1.color.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightD1.dirToLight"), lightD1.direction.x, lightD1.direction.y, lightD1.direction.z);*/

	/*glUniform3f(glGetUniformLocation(shader_character->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
	glUniform3f(glGetUniformLocation(shader_character->ID, "lightP0.dirToLight"), lightP0.direction.x, lightP0.direction.y, lightP0.direction.z);
	glUniform1f(glGetUniformLocation(shader_character->ID, "lightP0.constant"), lightP0.constant);
	glUniform1f(glGetUniformLocation(shader_character->ID, "lightP0.linear"), lightP0.linear);
	glUniform1f(glGetUniformLocation(shader_character->ID, "lightP0.quadratic"), lightP0.quadratic);*/

	shader_monster->use();
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	/*glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD1.pos"), lightD1.position.x, lightD1.position.y, lightD1.position.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD1.color"), lightD1.color.x, lightD1.color.y, lightD1.color.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightD1.dirToLight"), lightD1.direction.x, lightD1.direction.y, lightD1.direction.z);*/

	/*glUniform3f(glGetUniformLocation(shader_monster->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
	glUniform3f(glGetUniformLocation(shader_monster->ID, "lightP0.dirToLight"), lightP0.direction.x, lightP0.direction.y, lightP0.direction.z);
	glUniform1f(glGetUniformLocation(shader_monster->ID, "lightP0.constant"), lightP0.constant);
	glUniform1f(glGetUniformLocation(shader_monster->ID, "lightP0.linear"), lightP0.linear);
	glUniform1f(glGetUniformLocation(shader_monster->ID, "lightP0.quadratic"), lightP0.quadratic);*/

	shader_icepiton->use();
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	/*glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD1.pos"), lightD1.position.x, lightD1.position.y, lightD1.position.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD1.color"), lightD1.color.x, lightD1.color.y, lightD1.color.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightD1.dirToLight"), lightD1.direction.x, lightD1.direction.y, lightD1.direction.z);*/

	/*glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
	glUniform3f(glGetUniformLocation(shader_icepiton->ID, "lightP0.dirToLight"), lightP0.direction.x, lightP0.direction.y, lightP0.direction.z);
	glUniform1f(glGetUniformLocation(shader_icepiton->ID, "lightP0.constant"), lightP0.constant);
	glUniform1f(glGetUniformLocation(shader_icepiton->ID, "lightP0.linear"), lightP0.linear);
	glUniform1f(glGetUniformLocation(shader_icepiton->ID, "lightP0.quadratic"), lightP0.quadratic);*/

	shader_ground->use();
	glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	/*glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD1.pos"), lightD1.position.x, lightD1.position.y, lightD1.position.z);
	glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD1.color"), lightD1.color.x, lightD1.color.y, lightD1.color.z);
	glUniform3f(glGetUniformLocation(shader_ground->ID, "lightD1.dirToLight"), lightD1.direction.x, lightD1.direction.y, lightD1.direction.z);*/

	shader_wall->use();
	glUniform3f(glGetUniformLocation(shader_wall->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_wall->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_wall->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	shader_UI_item->use();
	glUniform3f(glGetUniformLocation(shader_UI_item->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_UI_item->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_UI_item->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

	shader_star->use();
	glUniform3f(glGetUniformLocation(shader_star->ID, "lightD.pos"), lightD.position.x, lightD.position.y, lightD.position.z);
	glUniform3f(glGetUniformLocation(shader_star->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
	glUniform3f(glGetUniformLocation(shader_star->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);
#pragma endregion

#pragma endregion

#pragma region Init Material
	Material* myMaterial = new Material(
		shader_character,
		glm::vec3(1.0f, 1.0f, 1.0f), // ambient 
		LoadTextureImageToGPU("container2.png", GL_RGBA, GL_RGBA, shader_character->DIFFUSE), // diffuse
		LoadTextureImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, shader_character->SPECULAR), // spectural
		LoadTextureImageToGPU("matrix_changed2.jpg", GL_RGB, GL_RGB, shader_character->EMISSION), // emission
		32.0f); // shininess

	Material* monsterMaterial = new Material(
		shader_monster,
		glm::vec3(1.0f, 1.0f, 1.0f), // ambient 
		LoadTextureImageToGPU("container2.png", GL_RGBA, GL_RGBA, shader_monster->DIFFUSE), // diffuse
		LoadTextureImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, shader_monster->SPECULAR), // spectural
		LoadTextureImageToGPU("matrix_changed2.jpg", GL_RGB, GL_RGB, shader_monster->EMISSION), // emission
		32.0f); // shininess

	Material* lightSourceMaterial = new Material(
		shader_LightSource,
		glm::vec3(1.0f, 1.0f, 1.0f), // ambient 
		LoadTextureImageToGPU("container2.png", GL_RGBA, GL_RGBA, shader_character->DIFFUSE), // diffuse
		LoadTextureImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, shader_character->SPECULAR), // spectural
		LoadTextureImageToGPU("matrix_changed2.jpg", GL_RGB, GL_RGB, shader_character->EMISSION), // emission
		32.0f); // shininess
#pragma endregion

#pragma region Init and Load Models to VBO, VAO

//Mesh cube(vertices);

//Model model(exePath.substr(0, exePath.find_last_of('\\')) + "\\Kamisato_obj_en\\Kamisato.obj");
	Character character(
		//Model(exePath.substr(0, exePath.find_last_of('\\')) + "\\Kamisato_obj_en\\Kamisato.obj"),
		ffModel(exePath.substr(0, exePath.find_last_of('\\')) + "\\Kamisato_fbx_en\\Kamisato_en.fbx"),
		glm::vec3(-2.0f, 0.0f, 0.0f),     // position
		//glm::vec3(4 * 0.027f, 4 * 0.027f, 4 * 0.027),      // scale
		glm::vec3(0.027f, 0.027f, 0.027),      // scale
		0.6                               // speed
	);

	Model monsterBody_model(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\body\\body.obj");
	vector<Character> monsters;
	Character monster_0(
		monsterBody_model,
		glm::vec3(30.0f, 0.0f, 30.0f),     // position
		glm::vec3(1.5f, 1.5f, 1.5f),      // scale
		0.2                               // speed
	);
	monsters.push_back(monster_0);
	Character monster_1(
		monsterBody_model,
		glm::vec3(-15.0f, 0.0f, 35.0f),     // position
		glm::vec3(1.5f, 1.5f, 1.5f),      // scale
		0.3                               // speed
	);
	monsters.push_back(monster_1);

	Character monster_3(
		monsterBody_model,
		glm::vec3(25.0f, 0.0f, -30.0f),     // position
		glm::vec3(1.5f, 1.5f, 1.5f),      // scale
		0.24                              // speed
	);
	monsters.push_back(monster_3);
	Character monster_4(
		monsterBody_model,
		glm::vec3(30.0f, 0.0f, -60.0f),     // position
		glm::vec3(1.5f, 1.5f, 1.5f),      // scale
		0.15                               // speed
	);
	monsters.push_back(monster_4);

	//Model monster_body(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\body\\body.obj");
	Model monster_righthand(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\right_hand\\right_hand.obj");
	Model monster_lefthand(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\left_hand\\left_hand.obj");
	Model monster_rightleg(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\right_leg\\right_leg.obj");
	Model monster_leftleg(exePath.substr(0, exePath.find_last_of('\\')) + "\\squareman_seperate\\left_leg\\left_leg.obj");

	vector<IcePiton> icePitons;
	Model icePiton_model(exePath.substr(0, exePath.find_last_of('\\')) + "\\IcePiton_sphere\\IcePiton.obj");
	
	Model heart(exePath.substr(0, exePath.find_last_of('\\')) + "\\heart\\heart.obj");

	vector<Key> keys;
	Model key(exePath.substr(0, exePath.find_last_of('\\')) + "\\key\\key.obj");
	Key newKey = Key(
		key,
		glm::vec3(35.0f, 2.0f, -72.0f)    // position
	);
	keys.push_back(newKey);

	vector<Gem> gems;
	Model gem(exePath.substr(0, exePath.find_last_of('\\')) + "\\gem\\gem.obj");
	Gem newGem = Gem(
		gem,
		glm::vec3(-36.0f, 1.0f, -71.0f)    // position
	);
	gems.push_back(newGem);

	newGem = Gem(
		gem,
		glm::vec3(36.0f, 1.0f, -100.0f)    // position
	);
	gems.push_back(newGem);

	newGem = Gem(
		gem,
		glm::vec3(37.0f, 1.0f, 37.0f)    // position
	);
	gems.push_back(newGem);

	newGem = Gem(
		gem,
		glm::vec3(-35.0f, 1.0f, -33.0f)    // position
	);
	gems.push_back(newGem);

	newGem = Gem(
		gem,
		glm::vec3(43.0f, 1.0f, -27.0f)    // position
	);
	gems.push_back(newGem);


	vector<Gate> gates;
	Model gate(exePath.substr(0, exePath.find_last_of('\\')) + "\\gate\\gate.obj");
	Gate newgate = Gate{
		gate,
		glm::vec3(52.5f, 0.0f, 39.0f),   // position
		glm::vec3(6.0f, 2.8f, 3.0f)      // scale
	};
	gates.push_back(newgate);


	vector<Cabin> cabins;
	Model cabin(exePath.substr(0, exePath.find_last_of('\\')) + "\\cabin2\\cabin.obj");
	Cabin newcabin = Cabin{
		cabin,
		glm::vec3(-29.0f, -0.3f, -27.0f),              // position
		//glm::vec3(0.00015f, 0.00035f, 0.00015f),      // scale
		glm::vec3(2.0f, 4.0f, 2.0f),
		glm::radians(90.0f),                          // rotate degree
		glm::vec3(0.0f, 1.0f, 0.0f)                   // rotate direction
	};
	cabins.push_back(newcabin);

	newcabin = Cabin{
		cabin,
		glm::vec3(-30.0f, -0.3f, -70.0f),              // position
		glm::vec3(2.0f, 4.0f, 2.0f),                  // scale
		glm::radians(90.0f),                          // rotate degree
		glm::vec3(0.0f, 1.0f, 0.0f)                   // rotate direction
	};
	cabins.push_back(newcabin);

	newcabin = Cabin{
		cabin,
		glm::vec3(30.0f, -0.3f, -70.0f),              // position
		glm::vec3(2.0f, 4.0f, 2.0f),                 // scale
		glm::radians(-90.0f),                          // rotate degree
		glm::vec3(0.0f, 1.0f, 0.0f)                   // rotate direction
	};
	cabins.push_back(newcabin);

	newcabin = Cabin{
		cabin,
		glm::vec3(60.0f, -0.3f, 20.0f),              // position
		glm::vec3(2.0f, 4.0f, 2.0f),                // scale
		glm::radians(-90.0f),                          // rotate degree
		glm::vec3(0.0f, 1.0f, 0.0f)                   // rotate direction
	};
	cabins.push_back(newcabin);

	vector<Wall> walls;
	Model wall(exePath.substr(0, exePath.find_last_of('\\')) + "\\wall\\wall.obj");
	Wall newWall = Wall(
		wall,
		glm::vec3(0.0f, 0.0f, 40.0f),    // position
		glm::vec3(40.0f, 6.0f, 1.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(0.0f, 0.0f, -120.0f),    // position
		glm::vec3(40.0f, 6.0f, 1.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(22.0f, 0.0f, -40.0f),    // position
		glm::vec3(18.0f, 6.0f, 1.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(-22.0f, 0.0f, -40.0f),    // position
		glm::vec3(18.0f, 6.0f, 1.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(52.2f, 0.0f, -30.0f),    // position
		glm::vec3(12.8f, 6.0f, 1.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(40.0f, 0.0f, -70.0f),    // position
		glm::vec3(1.0f, 6.0f, 50.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(40.0f, 0.0f, 16.0f),    // position
		glm::vec3(1.0f, 6.0f, 24.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(-40.0f, 0.0f, -40.0f),    // position
		glm::vec3(1.0f, 6.0f, 80.0f)     // scale
	);
	walls.push_back(newWall);

	newWall = Wall(
		wall,
		glm::vec3(65.0f, 0.0f, 5.0f),    // position
		glm::vec3(1.0f, 6.0f, 35.0f)     // scale
	);
	walls.push_back(newWall);

	Model star(exePath.substr(0, exePath.find_last_of('\\')) + "\\star\\star.obj");

	unsigned int VBO_ground;
	glGenBuffers(1, &VBO_ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ground), vertices_ground, GL_STATIC_DRAW);

	unsigned int VAO_ground;
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	unsigned int EBO_ground;
	glGenBuffers(1, &EBO_ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_ground), indices_ground, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	unsigned int VBO_LightSource;
	glGenBuffers(1, &VBO_LightSource);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LightSource);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightSourceVertices), lightSourceVertices, GL_STATIC_DRAW);

	unsigned int VAO_LightSource_Dir_1;
	glGenVertexArrays(1, &VAO_LightSource_Dir_1);
	glBindVertexArray(VAO_LightSource_Dir_1);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

#pragma endregion

#pragma region Init and Load Texture;
	stbi_set_flip_vertically_on_load(true);
	unsigned int TexBuffer_ground;
	TexBuffer_ground = LoadTextureImageToGPU("ground.jpg", GL_RGB, GL_RGB, 0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

#pragma region Init Skybox;
	vector<std::string> faces
	{
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\posx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\negx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\posy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\negy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\posz.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox_darksky\\negz.jpg"
		/*"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"*/
	};
	stbi_set_flip_vertically_on_load(false);
	unsigned int cubemapTexture = loadCubemap(faces);
#pragma endregion

#pragma region Init FBO for Shadow Map;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 2*4096, SHADOW_HEIGHT = 2*4096;
	
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

#pragma region Init Gameover Texture;
	std::string gameover_path = exePath.substr(0, exePath.find_last_of('\\')) + "\\gameover\\gameover_flip.jpg";
	unsigned int gameoverTex = LoadTextureImageToGPU(gameover_path.c_str() , GL_RGB, GL_RGB, 0);
#pragma endregion

#pragma region Init Congratulation Texture;
	std::string congratulation_path = exePath.substr(0, exePath.find_last_of('\\')) + "\\congratulation\\congratulation_flip.jpg";
	unsigned int congratulationTex = LoadTextureImageToGPU(congratulation_path.c_str(), GL_RGB, GL_RGB, 0);
#pragma endregion

#pragma region Prepare MVP matrices
//glm::mat4 scale;  glm::scale(transMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 trans;
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
	glm::mat4 viewMat;
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 300.0f);
#pragma endregion

	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 1.0f);
	float period_monster_limb = 1.0;
	float spawnTime = glfwGetTime();
	float shootTime = glfwGetTime();
	float _lastTime = 0;
	float congratulationTime = 0;
	bool congratulation = false;
	unsigned int animationNumber = 1; // 0: attack animation; 1: idle animation; 2: move animation;
	float shootAnimationDuration = 0.375;

	// Start drawing
	while (!glfwWindowShouldClose(window))
	{
		float timevalue = glfwGetTime();
		if (character.win && timevalue - congratulationTime >= 3.0f) congratulation = true;

		// These times are for animation
		float _currentTime = glfwGetTime();
		float _deltaTime = _currentTime - _lastTime;
		_lastTime = _currentTime;

		// Compute the time fot rotate monsters amrs and legs
		float time_rotate_monster = ((timevalue - floor(timevalue / period_monster_limb)*period_monster_limb) / period_monster_limb - 0.5) * 2;
		if (time_rotate_monster <= 0) time_rotate_monster = 2 * time_rotate_monster + 1;
		else time_rotate_monster = -2 * time_rotate_monster + 1;

		// Process Input
		//processInput_camera(window);

		shootTime = processInput_shootIcePiton(window, character, icePitons, icePiton_model, shootTime);
		processInput_character(window, character);
		processInput_switchCameraMode(window);

		if (cameraMode == 1)  // Third person view
		{
			character.updateDirection_camera(camera.Forward, camera.Right);
			camera.ThirdPersonView_LookAtCharacter(character);
		}
		else if (cameraMode == 2)  // Top down view
		{
			character.updateDirection_camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			camera.Position = character.position + glm::vec3(0.0f, 5.0f, 0.05f);
			camera.TopDownView_LookAtCharacter(character);
		}

		spawnTime = monsterSpawner(monsters, monsterBody_model, spawnTime);

#pragma region Monster-Chacter Collision

		bool mon_cha_collision = false;
		for (int j = 0; j < monsters.size(); j++)
		{
			glm::vec3 pos_character = glm::vec3(character.position.x, 0.0f, character.position.z);
			glm::vec3 pos_monster = glm::vec3(monsters[j].position.x, 0.0, monsters[j].position.z);
			float distance = glm::length(pos_character - pos_monster);

			if (distance <= (character.radius + monsters[j].radius) && !character.win)
			{
				monsters.erase(monsters.begin() + j);
				if (character.numLife > 0) character.numLife--;
				if (character.numLife <= 0) character.die = true;
				mon_cha_collision = true;
				break;
			}
		}
#pragma endregion

#pragma region Chacter-Gem Collision

		for (int j = 0; j < gems.size(); j++)
		{
			glm::vec3 pos_character = glm::vec3(character.position.x, 0.0f, character.position.z);
			glm::vec3 pos_monster = glm::vec3(gems[j].position.x, 0.0, gems[j].position.z);
			float distance = glm::length(pos_character - pos_monster);

			if (distance <= (character.radius + gems[j].radius))
			{
				gems.erase(gems.begin() + j);
				character.numGem += 1;
			}
		}
#pragma endregion

#pragma region Chacter-Key Collision

		for (int j = 0; j < keys.size(); j++)
		{
			glm::vec3 pos_character = glm::vec3(character.position.x, 0.0f, character.position.z);
			glm::vec3 pos_monster = glm::vec3(keys[j].position.x, 0.0, keys[j].position.z);
			float distance = glm::length(pos_character - pos_monster);

			if (distance <= (character.radius + keys[j].radius))
			{
				keys.erase(keys.begin() + j);
				character.numKey += 1;
			}
		}
#pragma endregion

#pragma region MonsterChacter-Wall Collision

		character.collision_x_p = false;
		character.collision_x_n = false;
		character.collision_z_p = false;
		character.collision_z_n = false;

		for (unsigned int i = 0; i < walls.size(); i++)
		{
			bool wall_collision = false;

			if (abs(character.position.x - walls[i].position.x) <= walls[i].x_length + 0.5
				&& abs(character.position.z - walls[i].position.z) <= walls[i].z_length + 0.5)
				wall_collision = true;

			if (wall_collision)
			{
				if (character.position.x < (walls[i].position.x - walls[i].x_length + 0.5 + 0.5))
					character.collision_x_n = true;
				else if (character.position.x > (walls[i].position.x + walls[i].x_length - 0.5 - 0.5))
					character.collision_x_p = true;

				if (character.position.z < (walls[i].position.z - walls[i].z_length + 0.5 + 0.5))
					character.collision_z_n = true;
				else if (character.position.z > (walls[i].position.z + walls[i].z_length - 0.5 - 0.5))
					character.collision_z_p = true;
			}
		}

		for (unsigned int j = 0; j < monsters.size(); j++)
		{
			monsters[j].collision_x_p = false;
			monsters[j].collision_x_n = false;
			monsters[j].collision_z_p = false;
			monsters[j].collision_z_n = false;
			for (unsigned int i = 0; i < walls.size(); i++)
			{
				bool wall_collision = false;

				if (abs(monsters[j].position.x - walls[i].position.x) <= walls[i].x_length + 0.5
					&& abs(monsters[j].position.z - walls[i].position.z) <= walls[i].z_length + 0.5)
					wall_collision = true;

				if (wall_collision)
				{
					if (monsters[j].position.x < (walls[i].position.x - walls[i].x_length + 0.5 + 0.5))
						monsters[j].collision_x_n = true;
					else if (monsters[j].position.x > (walls[i].position.x + walls[i].x_length - 0.5 - 0.5))
						monsters[j].collision_x_p = true;

					if (monsters[j].position.z < (walls[i].position.z - walls[i].z_length + 0.5 + 0.5))
						monsters[j].collision_z_n = true;
					else if (monsters[j].position.z > (walls[i].position.z + walls[i].z_length - 0.5 - 0.5))
						monsters[j].collision_z_p = true;
				}
			}
		}

#pragma endregion

#pragma region Chacter-Gate Collision

		for (unsigned int i = 0; i < gates.size(); i++)
		{
			bool gate_collision = false;

			if (abs(character.position.x - gates[i].position.x) <= gates[i].x_length + 0.5
				&& abs(character.position.z - gates[i].position.z) <= gates[i].z_length + 0.5)
				gate_collision = true;

			

			if (gate_collision)
			{
				if (character.numKey > 0)
				{
					gates.erase(gates.begin() + i);
					character.win = true;
					congratulationTime = glfwGetTime();
					break;
				}

				if (character.position.x < (gates[i].position.x - gates[i].x_length + 0.5 + 0.5))
					character.collision_x_n = true;
				else if (character.position.x > (gates[i].position.x + gates[i].x_length - 0.5 - 0.5))
					character.collision_x_p = true;

				if (character.position.z < (gates[i].position.z - gates[i].z_length + 0.5 + 0.5))
					character.collision_z_n = true;
				else if (character.position.z > (gates[i].position.z + gates[i].z_length - 0.5 - 0.5))
					character.collision_z_p = true;
			}
		}

#pragma endregion

#pragma region IcePiton Collision

		for (int i = 0; i < icePitons.size(); i++)
		{
			bool collision = false;

			if (icePitons[i].position.x > 1000 ||
				icePitons[i].position.x < -1000 ||
				icePitons[i].position.z > 1000 ||
				icePitons[i].position.z < -1000)
			{
				icePitons.erase(icePitons.begin() + i);
				break;
			}

			// Detect collision with monster
			for (int j = 0; j < monsters.size(); j++)
			{
				glm::vec3 pos_ice = glm::vec3(icePitons[i].position.x, 0.0f, icePitons[i].position.z);
				glm::vec3 pos_monster = glm::vec3(monsters[j].position.x, 0.0, monsters[j].position.z);
				float distance = glm::length(pos_ice - pos_monster);

				if (distance <= (icePitons[i].radius + monsters[j].radius))
				{
					monsters.erase(monsters.begin() + j);
					icePitons.erase(icePitons.begin() + i);
					//break;
					goto endOfLoop;
				}
			}

			// Detect collision with wall
			for (unsigned int j = 0; j < walls.size(); j++)
			{
				if (abs(icePitons[i].position.x - walls[j].position.x) <= walls[j].x_length + 0.5
					&& abs(icePitons[i].position.z - walls[j].position.z) <= walls[j].z_length + 0.5)
				{
					icePitons.erase(icePitons.begin() + i);
					//break;
					goto endOfLoop;
				}
			}
			endOfLoop:
				;
		}
#pragma endregion

#pragma region Create Shadow Map


		float near_plane = 0.0f, far_plane = 200.5f;
		glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane); 
		// Set the light position following the character, and add an offset to draw shadows in distance (some may be cut under the limitaion of depth map)
		glm::mat4 lightView = glm::lookAt(lightD.position + character.position , character.position + glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 lightView = glm::lookAt(lightD.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shader_shadowMap->use();
		shader_shadowMap->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		//glDisable(GL_CULL_FACE);

		float angle_character = acos(glm::dot(glm::normalize(displacement), character.initialDir));
		float clockwise = glm::dot(glm::cross(character.initialDir, displacement), glm::vec3(0.0f, 1.0f, 0.0f));
		if (!isnan(angle_character)) {
			if (clockwise >= 0) character.rotate = angle_character;
			else if (clockwise < 0) character.rotate = -angle_character;
		}

		if (shoot)
		{
			animationNumber = 0;
			if (_currentTime - lastShootTime >= shootAnimationDuration)
			{
				shoot = false;
				character.ffmodel.m_animators[animationNumber]->playAnimation();
			}
		}
		else if (character.speedForward || character.speedRight)
			animationNumber = 2;
		else
			animationNumber = 1;

		trans = glm::scale(glm::mat4(1.0f), character.scale);
		trans = glm::rotate(trans, character.rotate, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::translate(glm::mat4(1.0f), character.position);
		modelMat = modelMat * trans;

		shader_shadowMap->SetMatrix("model", modelMat);
		character.ffmodel.update(_deltaTime, animationNumber);
		character.ffmodel.Draw(shader_shadowMap, animationNumber);


		for (auto& monster : monsters)
		{
			monster.updateDirection_follow(glm::normalize(character.position - monster.position));

			float angle_monster = acos(glm::dot(glm::normalize(monster.forward), monster.initialDir));
			float clockwise_monster = glm::dot(glm::cross(monster.initialDir, monster.forward), glm::vec3(0.0f, 1.0f, 0.0f));
			if (!isnan(angle_monster)) {
				if (clockwise_monster >= 0) monster.rotate = angle_monster;
				else if (clockwise_monster < 0) monster.rotate = -angle_monster;
			}
		}

		for (auto& monster : monsters)
		{
			trans = glm::scale(glm::mat4(1.0f), monster.scale);
			trans = glm::rotate(trans, monster.rotate, glm::vec3(0.0f, 1.0f, 0.0f));
			/*modelMat = glm::translate(trans, monster.position);
			shader_shadowMap->SetMatrix("model", modelMat);*/

			glm::mat4 trans_righthand = glm::translate(trans, glm::vec3(-0.15f, 1.5f, 0.0f));
			trans_righthand = glm::rotate(trans_righthand, glm::radians(22.5f * time_rotate_monster), glm::vec3(0.0f, 1.0f, 0.0f));
			trans_righthand = glm::translate(trans_righthand, glm::vec3(0.15f, -1.5f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			modelMat = modelMat * trans_righthand;
			shader_shadowMap->SetMatrix("model", modelMat);
			//glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_righthand));
			monster_righthand.Draw(shader_shadowMap);

			glm::mat4 trans_lefthand = glm::translate(trans, glm::vec3(0.15f, 1.5f, 0.0f));
			trans_lefthand = glm::rotate(trans_lefthand, glm::radians(22.5f * time_rotate_monster), glm::vec3(0.0f, 1.0f, 0.0f));
			trans_lefthand = glm::translate(trans_lefthand, glm::vec3(-0.15f, -1.5f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			modelMat = modelMat * trans_lefthand;
			shader_shadowMap->SetMatrix("model", modelMat);
			//glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_lefthand));
			monster_lefthand.Draw(shader_shadowMap);

			glm::mat4 trans_rightleg = glm::translate(trans, glm::vec3(-0.26f, 0.83f, 0.0f));
			trans_rightleg = glm::rotate(trans_rightleg, glm::radians(45.0f * time_rotate_monster), glm::vec3(1.0f, 0.0f, 0.0f));
			trans_rightleg = glm::translate(trans_rightleg, glm::vec3(0.26f, -0.83f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			modelMat = modelMat * trans_rightleg;
			shader_shadowMap->SetMatrix("model", modelMat);
			//glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_rightleg));
			monster_rightleg.Draw(shader_shadowMap);

			glm::mat4 trans_leftleg = glm::translate(trans, glm::vec3(0.26f, 0.83f, 0.0f));
			trans_leftleg = glm::rotate(trans_leftleg, glm::radians(-45.0f * time_rotate_monster), glm::vec3(1.0f, 0.0f, 0.0f));
			trans_leftleg = glm::translate(trans_leftleg, glm::vec3(-0.26f, -0.83f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			modelMat = modelMat * trans_leftleg;
			shader_shadowMap->SetMatrix("model", modelMat);
			//glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_leftleg));
			monster_leftleg.Draw(shader_shadowMap);

			//glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			modelMat = modelMat * trans;
			shader_shadowMap->SetMatrix("model", modelMat);
			monster.model.Draw(shader_shadowMap);
		}

		for (int i = 0; i < icePitons.size(); i++)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
			modelMat = glm::translate(modelMat, icePitons[i].forward);
			modelMat = glm::translate(modelMat, icePitons[i].position);
			modelMat = modelMat * trans;
			shader_shadowMap->SetMatrix("model", modelMat);

			icePitons[i].model.Draw(shader_shadowMap);
		}

		for (int i = 0; i < walls.size(); i++)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(walls[i].scale.x, walls[i].scale.y, walls[i].scale.z));
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(walls[i].position.x, walls[i].position.y, walls[i].position.z));
			modelMat = modelMat * trans;
			shader_shadowMap->SetMatrix("model", modelMat);
			wall.Draw(shader_shadowMap);
		}

		for (unsigned int i = 0; i < cabins.size(); i++)
		{
			trans = glm::scale(glm::mat4(1.0f), cabins[i].scale);
			trans = glm::rotate(trans, cabins[i].rotation, cabins[i].rotation_dir);
			modelMat = glm::translate(glm::mat4(1.0f), cabins[i].position);
			modelMat = modelMat * trans;
			shader_shadowMap->SetMatrix("model", modelMat);
			cabin.Draw(shader_shadowMap);
		}

		for (unsigned int i = 0; i < gates.size(); i++)
		{
			trans = glm::scale(glm::mat4(1.0f), gates[i].scale);
			modelMat = glm::translate(glm::mat4(1.0f), gates[i].position);
			modelMat = modelMat * trans;
			shader_shadowMap->SetMatrix("model", modelMat);
			gate.Draw(shader_shadowMap);
		}

		//glCullFace(GL_BACK);

		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
		shader_shadowMap->SetMatrix("model", modelMat);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);
		
		//glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		glViewport(0, 0, 800, 600);

		// Clear Screen
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMat = camera.GetViewMatrix();

		// Shadow map debugger
		//shader_shadowDebug->use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//shader_shadowDebug->SetUniform1i("depthMap", 0);
		//renderQuad();

		
#pragma endregion

#pragma region Draw End Scene

		if (character.die)
		{
			shader_endScene->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gameoverTex);
			shader_shadowDebug->SetUniform1i("Texture", 0);
			renderQuad();
		}

		if (character.win && congratulation)
		{
			shader_endScene->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, congratulationTex);
			shader_shadowDebug->SetUniform1i("Texture", 0);
			renderQuad();
		}
#pragma endregion

#pragma region Draw character
		// Character

		shader_character->use();
		trans = glm::scale(glm::mat4(1.0f), character.scale);
		trans = glm::rotate(trans, character.rotate, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::translate(glm::mat4(1.0f), character.position);
		glUniformMatrix4fv(glGetUniformLocation(shader_character->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(glGetUniformLocation(shader_character->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_character->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_character->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		glUniform3f(glGetUniformLocation(shader_character->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(shader_character->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(shader_character->ID, "material.shininess"), 128.0f);
		shader_character->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shader_character->ID, "shadowMap"), 8);

		if (mon_cha_collision)
		{
			glUniform3f(glGetUniformLocation(shader_character->ID, "ambientColor"), 0.8f, 0.0f, 0.0f); // red flash to represent getting damage
			//character.ffmodel.update(_deltaTime, animationNumber);  // already updated when draw shadow map
			character.ffmodel.Draw(shader_character, animationNumber);
			glUniform3f(glGetUniformLocation(shader_character->ID, "ambientColor"), 0.0f, 0.0f, 0.0f);
		}
		else
		{
			glUniform3f(glGetUniformLocation(shader_character->ID, "ambientColor"), 0.0f, 0.0f, 0.0f);
			//character.ffmodel.update(_deltaTime, animationNumber);  // already updated when draw shadow map
			character.ffmodel.Draw(shader_character, animationNumber);
		}


#pragma endregion

#pragma region Draw Monster
		// Monster
		
		for (auto& monster : monsters)
		{
			shader_monster->use();
			trans = glm::scale(glm::mat4(1.0f), monster.scale);
			trans = glm::rotate(trans, monster.rotate, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), monster.position);
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_monster->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_monster->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_monster->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_monster->ID, "material.shininess"), 64.0f);
			shader_monster->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_monster->ID, "shadowMap"), 3);

			glm::mat4 trans_righthand = glm::translate(trans, glm::vec3(-0.15f, 1.5f, 0.0f));
			trans_righthand = glm::rotate(trans_righthand, glm::radians(22.5f * time_rotate_monster), glm::vec3(0.0f, 1.0f, 0.0f));
			trans_righthand = glm::translate(trans_righthand, glm::vec3(0.15f, -1.5f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_righthand));
			monster_righthand.Draw(shader_monster);

			glm::mat4 trans_lefthand = glm::translate(trans, glm::vec3(0.15f, 1.5f, 0.0f));
			trans_lefthand = glm::rotate(trans_lefthand, glm::radians(22.5f * time_rotate_monster), glm::vec3(0.0f, 1.0f, 0.0f));
			trans_lefthand = glm::translate(trans_lefthand, glm::vec3(-0.15f, -1.5f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_lefthand));
			monster_lefthand.Draw(shader_monster);

			glm::mat4 trans_rightleg = glm::translate(trans, glm::vec3(-0.26f, 0.83f, 0.0f));
			trans_rightleg = glm::rotate(trans_rightleg, glm::radians(45.0f * time_rotate_monster), glm::vec3(1.0f, 0.0f, 0.0f));
			trans_rightleg = glm::translate(trans_rightleg, glm::vec3(0.26f, -0.83f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_rightleg));
			monster_rightleg.Draw(shader_monster);

			glm::mat4 trans_leftleg = glm::translate(trans, glm::vec3(0.26f, 0.83f, 0.0f));
			trans_leftleg = glm::rotate(trans_leftleg, glm::radians(-45.0f * time_rotate_monster), glm::vec3(1.0f, 0.0f, 0.0f));
			trans_leftleg = glm::translate(trans_leftleg, glm::vec3(-0.26f, -0.83f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans_leftleg));
			monster_leftleg.Draw(shader_monster);

			glUniformMatrix4fv(glGetUniformLocation(shader_monster->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			monster.model.Draw(shader_monster);

			monster.updatePosition_follow(character.position);
		}

#pragma endregion

#pragma region Draw IcePiton
		for (int i = 0; i < icePitons.size(); i++)
		{
			shader_icepiton->use();
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
			modelMat = glm::translate(modelMat, icePitons[i].forward);
			modelMat = glm::translate(modelMat, icePitons[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_icepiton->ID, "material.shininess"), 64.0f);
			icePitons[i].model.Draw(shader_icepiton);

			icePitons[i].updatePosition();
		}
#pragma endregion

		for (unsigned int i = 0; i < cabins.size(); i++)
		{
			shader_icepiton->use();
			trans = glm::scale(glm::mat4(1.0f), cabins[i].scale);
			trans = glm::rotate(trans, cabins[i].rotation, cabins[i].rotation_dir);
			modelMat = glm::translate(glm::mat4(1.0f), cabins[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_icepiton->ID, "material.shininess"), 64.0f);
			shader_icepiton->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_icepiton->ID, "shadowMap"), 3);

			cabin.Draw(shader_icepiton);
		}

		for (unsigned int i = 0; i < keys.size(); i++)
		{
			shader_icepiton->use();
			trans = glm::scale(glm::mat4(1.0f), keys[i].scale);
			trans = glm::rotate(trans, glm::radians(130.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), keys[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_icepiton->ID, "material.shininess"), 64.0f);
			shader_icepiton->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_icepiton->ID, "shadowMap"), 3);

			keys[i].model.Draw(shader_icepiton);
		}

		for (unsigned int i = 0; i < gems.size(); i++)
		{
			shader_icepiton->use();
			trans = glm::scale(glm::mat4(1.0f), gems[i].scale);
			trans = glm::rotate(trans, glm::radians(45.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::translate(glm::mat4(1.0f), gems[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_icepiton->ID, "material.shininess"), 64.0f);
			shader_icepiton->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_icepiton->ID, "shadowMap"), 3);

			gems[i].model.Draw(shader_icepiton);
		}

		for (unsigned int i = 0; i < gates.size(); i++)
		{
			shader_icepiton->use();
			trans = glm::scale(glm::mat4(1.0f), gates[i].scale);
			modelMat = glm::translate(glm::mat4(1.0f), gates[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_icepiton->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_icepiton->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_icepiton->ID, "material.shininess"), 64.0f);
			shader_icepiton->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_icepiton->ID, "shadowMap"), 3);

			gate.Draw(shader_icepiton);
		}

		if (!character.die && !congratulation)
		{
			for (int i = 0; i < character.numKey; i++)
			{
				shader_UI_item->use();
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
				//trans = glm::rotate(trans, glm::radians(130.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.9f, -0.75f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UItransform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UImodelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				key.Draw(shader_UI_item);
			}

			for (int i = 0; i < character.numGem; i++)
			{
				shader_UI_item->use();
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.045f, 0.06f, 0.06f));
				//trans = glm::rotate(trans, glm::radians(130.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.9f, -0.93f, 0.0f) + glm::vec3(i * 0.1f, 0.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UItransform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UImodelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				trans = glm::rotate(trans, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
				glUniform1f(glGetUniformLocation(shader_UI_item->ID, "material.shininess"), 64.0f);
				gem.Draw(shader_UI_item);
			}

			for (int i = 0; i < character.numLife; i++)
			{
				shader_UI_item->use();
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.052f, 0.052f));
				//trans = glm::rotate(trans, glm::radians(130.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.92f, 0.90f, 0.0f) + glm::vec3(i * 0.075f, 0.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UItransform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "UImodelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_UI_item->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
				glUniform3f(glGetUniformLocation(shader_UI_item->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
				glUniform1f(glGetUniformLocation(shader_UI_item->ID, "material.shininess"), 64.0f);
				heart.Draw(shader_UI_item);
			}

		}
		if (congratulation)
		{
			for (int i = 0; i < 5; i++)
			{
				shader_star->use();
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.33f, 0.41f, 0.41f));
				//trans = glm::rotate(trans, glm::radians(130.0f * _currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.2f, 0.0f) + glm::vec3(i * 0.25f, 0.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "UItransform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "UImodelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				trans = glm::rotate(trans, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
				glUniformMatrix4fv(glGetUniformLocation(shader_star->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
				glUniform3f(glGetUniformLocation(shader_star->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
				glUniform3f(glGetUniformLocation(shader_star->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
				glUniform3f(glGetUniformLocation(shader_star->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
				glUniform1f(glGetUniformLocation(shader_star->ID, "material.shininess"), 64.0f);

				if (i < character.numGem)
					glUniform1i(glGetUniformLocation(shader_star->ID, "mark_star"), 1);
				else
					glUniform1i(glGetUniformLocation(shader_star->ID, "mark_star"), 0);
				star.Draw(shader_star);
			}
		}
#pragma region Draw Wall

		for (int i = 0; i < walls.size(); i++)
		{
			shader_wall->use();
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(walls[i].scale.x, walls[i].scale.y, walls[i].scale.z));
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(walls[i].position.x, walls[i].position.y, walls[i].position.z));
			glUniformMatrix4fv(glGetUniformLocation(shader_wall->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shader_wall->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_wall->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(shader_wall->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(shader_wall->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform3f(glGetUniformLocation(shader_wall->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(shader_wall->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
			glUniform1f(glGetUniformLocation(shader_wall->ID, "material.shininess"), 64.0f);
			glUniform1f(glGetUniformLocation(shader_wall->ID, "scale_x"), walls[i].scale.x);
			glUniform1f(glGetUniformLocation(shader_wall->ID, "scale_y"), walls[i].scale.y);
			glUniform1f(glGetUniformLocation(shader_wall->ID, "scale_z"), walls[i].scale.z);
			shader_wall->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glUniform1i(glGetUniformLocation(shader_wall->ID, "shadowMap"), 3);

			wall.Draw(shader_wall);

		}
#pragma endregion

#pragma region Draw ground
		// Ground
		shader_ground->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader_ground->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(glGetUniformLocation(shader_ground->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_ground->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_ground->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		glUniform3f(glGetUniformLocation(shader_ground->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(shader_ground->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(shader_ground->ID, "material.shininess"), 64.0f);
		shader_ground->SetMatrix("lightSpaceMatrix", lightSpaceMatrix);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_ground);
		glUniform1i(glGetUniformLocation(shader_ground->ID, "groundTexture_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shader_ground->ID, "shadowMap"), 1);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);

#pragma endregion

#pragma region Draw Directional Light;

		//shader_LightSource->use();
		////glm::vec3 lightPositions = glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z);
		//modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
		//glUniformMatrix4fv(glGetUniformLocation(shader_LightSource->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		//glUniformMatrix4fv(glGetUniformLocation(shader_LightSource->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformMatrix4fv(glGetUniformLocation(shader_LightSource->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(shader_LightSource->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		//glUniform3f(glGetUniformLocation(shader_LightSource->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		//glUniform3f(glGetUniformLocation(shader_LightSource->ID, "lightColor"), lightD.color.x, lightD.color.y, lightD.color.z);

		//glBindVertexArray(VAO_LightSource_Dir_1);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

#pragma endregion

#pragma region Draw Skybox;

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		shader_skybox->use();
		viewMat = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.04f, 0.0f));
		modelMat = glm::rotate(modelMat, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader_skybox->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_skybox->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(shader_skybox->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
#pragma endregion

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		displacement = character.updatePosition_key();

		if (cameraMode == 1)  // Third person view
			camera.UpdateCameraPos_ThirdPersonView(displacement);
		else if (cameraMode == 2)  // Top down view
			camera.UpdateCameraPos_ThirdPersonView(displacement);
		//camera.UpdateCameraPos();
	}

	// Exit Program
	glfwTerminate();
	return 0;
}