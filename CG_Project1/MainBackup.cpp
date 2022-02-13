//#define GLEW_STATIC
//#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#include "Shader.h"
//#include "Camera.h"
//#include "Material.h"
//#include "LightDirectional.h"
//#include "LightPoint.h"
//#include "LightSpot.h"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//using namespace std;
//
//#pragma region Model Data
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
//
//glm::vec3 cubePositions[] = {
//  glm::vec3(0.0f,  0.0f,  0.0f),
//  glm::vec3(2.0f,  5.0f, -15.0f),
//  glm::vec3(-1.5f, -2.2f, -2.5f),
//  glm::vec3(-3.8f, -2.0f, -12.3f),
//  glm::vec3(2.4f, -0.4f, -3.5f),
//  glm::vec3(-1.7f,  3.0f, -7.5f),
//  glm::vec3(1.3f, -2.0f, -2.5f),
//  glm::vec3(1.5f,  2.0f, -2.5f),
//  glm::vec3(1.5f,  0.2f, -1.5f),
//  glm::vec3(-1.3f,  1.0f, -1.5f)
//};
//
//float lightSourceVertices[] = {
//	-0.5f, -0.5f, -0.5f,
//	0.5f, -0.5f, -0.5f,
//	0.5f, 0.5f, -0.5f,
//	0.5f, 0.5f, -0.5f,
//	-0.5f, 0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//
//	-0.5f, -0.5f, 0.5f,
//	0.5f, -0.5f, 0.5f,
//	0.5f, 0.5f, 0.5f,
//	0.5f, 0.5f, 0.5f,
//	-0.5f, 0.5f, 0.5f,
//	-0.5f, -0.5f, 0.5f,
//
//	-0.5f, 0.5f, 0.5f,
//	-0.5f, 0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//	-0.5f, -0.5f, -0.5f,
//	-0.5f, -0.5f, 0.5f,
//	-0.5f, 0.5f, 0.5f,
//
//	0.5f, 0.5f, 0.5f,
//	0.5f, 0.5f, -0.5f,
//	0.5f, -0.5f, -0.5f,
//	0.5f, -0.5f, -0.5f,
//	0.5f, -0.5f, 0.5f,
//	0.5f, 0.5f, 0.5f,
//
//	-0.5f, -0.5f, -0.5f,
//	0.5f, -0.5f, -0.5f,
//	0.5f, -0.5f, 0.5f,
//	0.5f, -0.5f, 0.5f,
//	-0.5f, -0.5f, 0.5f,
//	-0.5f, -0.5f, -0.5f,
//
//	-0.5f, 0.5f, -0.5f,
//	0.5f, 0.5f, -0.5f,
//	0.5f, 0.5f, 0.5f,
//	0.5f, 0.5f, 0.5f,
//	-0.5f, 0.5f, 0.5f,
//	-0.5f, 0.5f, -0.5f
//};
//
//glm::vec3 lightPositions[] = {
//  glm::vec3(10.0f,  10.0f,  5.0f)
//};
//#pragma endregion
//
//#pragma region Camera Declaration
//// Initialize cmera class
////Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
//Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
//#pragma endregion
//
//#pragma region LightDeclaration
////LightDirectional light = LightDirectional(glm::vec3(10.0f, 10.0f, -5.0f), glm::vec3(0, glm::radians(45.0f), 0));
////LightPoint light = LightPoint(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0, glm::radians(45.0f), 0), glm::vec3(10.0f, 10.0f, 10.0f));
//LightSpot light = LightSpot(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.9, 0.55);
//#pragma endregion
//
//#pragma region Camera Operation Funtions
//double previous_xPos, previous_yPos;
//bool first_initialise_mouse = true;
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, true);
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//	{
//		camera.SpeedZ = 1.0;
//	}
//	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//	{
//		camera.SpeedZ = -1.0;
//	}
//	else
//	{
//		camera.SpeedZ = 0;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//	{
//		camera.SpeedX = 1.0;
//	}
//	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//	{
//		camera.SpeedX = -1.0;
//	}
//	else
//	{
//		camera.SpeedX = 0;
//	}
//}
//
//void mouse_callback(GLFWwindow* window, double xPos, double yPos)
//{
//	if (first_initialise_mouse == true)
//	{
//		previous_xPos = xPos;
//		previous_yPos = yPos;
//		first_initialise_mouse = false;
//	}
//
//	double Delta_x = xPos - previous_xPos;
//	double Delta_y = yPos - previous_yPos;
//
//	previous_xPos = xPos;
//	previous_yPos = yPos;
//
//	//cout << Delta_x << "   " << Delta_y << endl;
//	camera.ProcessMouseMovement(Delta_x, Delta_y);
//}
//#pragma endregion
//
//#pragma region Texture Loading Function
//unsigned int LoadTextureImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot)
//{
//	unsigned int TexBuffer;
//	glGenTextures(1, &TexBuffer);
//
//	glActiveTexture(GL_TEXTURE0 + textureSlot);
//
//	//cout << "before" <<  filename << "Slot:  " << TexBuffer << endl;
//
//	glBindTexture(GL_TEXTURE_2D, TexBuffer);
//	int width, height, nrChannel;
//	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
//	if (data)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		cout << "Texture image " << filename << " load failed." << endl;
//	}
//	stbi_image_free(data);
//
//	//cout << filename << "Slot:  " << TexBuffer << endl;
//
//	return TexBuffer;
//}
//#pragma endregion
//
//int main2()
//{
//#pragma region Open a Window
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "My openGL game", NULL, NULL);
//
//	if (window == NULL)
//	{
//		cout << "Open window failed." << endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	glewExperimental = true;
//	if (glewInit() != GLEW_OK)
//	{
//		cout << "Init GlEW failed." << endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	glViewport(0, 0, 800, 600);
//	glEnable(GL_DEPTH_TEST);
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//#pragma endregion
//
//#pragma region Init Shader Program
//	Shader* testShader = new Shader("VertexShader.vert", "FragmentShader.frag");
//#pragma endregion 
//
//#pragma region Init Material
//	Material* myMaterial = new Material(
//		testShader,
//		glm::vec3(1.0f, 1.0f, 1.0f), // ambient 
//		LoadTextureImageToGPU("container2.png", GL_RGBA, GL_RGBA, testShader->DIFFUSE), // diffuse
//		LoadTextureImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, testShader->SPECULAR), // spectural
//		LoadTextureImageToGPU("matrix_changed2.jpg", GL_RGB, GL_RGB, testShader->EMISSION), // emission
//		32.0f); // shininess
//#pragma endregion
//
//#pragma region Init and Load Models to VBO, VAO
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);*/
//	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(2);*/
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(3);
//	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(4);
//
//	/*unsigned int VBO_light;
//	glGenBuffers(1, &VBO_light);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_light);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(lightSourceVertices), lightSourceVertices, GL_STATIC_DRAW);
//
//	unsigned int VAO_light;
//	glGenVertexArrays(1, &VAO_light);
//	glBindVertexArray(VAO_light);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);*/
//#pragma endregion
//
//#pragma region Init and Load Texture;
//	//stbi_set_flip_vertically_on_load(true);
//	//unsigned int TexBufferA;
//	//TexBufferA = LoadTextureImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);
//	//unsigned int TexBufferB;
//	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
//#pragma endregion
//
//#pragma region Prepare MVP matrices
//	glm::mat4 trans;
//	glm::mat4 modelMat;
//	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
//	glm::mat4 viewMat;
//	glm::mat4 projMat;
//	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
//#pragma endregion
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// this lines will be necessary for some tricks
//		//float timevalue = glfwGetTime();
//		//trans = glm::rotate(trans, glm::radians(0.3f), glm::vec3(1.0f, 1.0f, 0.0f));
//
//		// Process Input
//		processInput(window);
//
//		// Clear Screen
//		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		viewMat = camera.GetViewMatrix();
//
//		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
//		{
//			// Set Model Matrix
//			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
//
//			// Set View and Projection Matrices here if you want
//
//			// Set Material -> Textures
//			//glActiveTexture(GL_TEXTURE0);
//			//glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
//			//glActiveTexture(GL_TEXTURE1);
//			//glBindTexture(GL_TEXTURE_2D, myMaterial->spectural);
//			// Set Matrial -> Shader Program
//			testShader->use();
//			// Set Material -> Uniforms
//			glUniform1i(glGetUniformLocation(testShader->ID, "ourTexture"), 0);
//			glUniform1i(glGetUniformLocation(testShader->ID, "ourFace"), 1);
//			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
//			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
//			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
//			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
//			glUniform3f(glGetUniformLocation(testShader->ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
//			glUniform3f(glGetUniformLocation(testShader->ID, "objectColor"), 0.5f, 0.5f, 0.5f);
//			glUniform3f(glGetUniformLocation(testShader->ID, "ambientColor"), 0.2f, 0.2f, 0.2f);
//			glUniform3f(glGetUniformLocation(testShader->ID, "lightPos"), light.position.x, light.position.y, light.position.z);
//			glUniform3f(glGetUniformLocation(testShader->ID, "lightColor"), light.color.x, light.color.y, light.color.z);
//			glUniform3f(glGetUniformLocation(testShader->ID, "lightDirUniform"), light.direction.x, light.direction.y, light.direction.z);
//			/*glUniform1f(glGetUniformLocation(testShader->ID, "lightPoint.constant"), light.constant);
//			glUniform1f(glGetUniformLocation(testShader->ID, "lightPoint.linear"), light.linear);
//			glUniform1f(glGetUniformLocation(testShader->ID, "lightPoint.quadratic"), light.quadratic);*/
//			glUniform1f(glGetUniformLocation(testShader->ID, "lightSpot.cosInner"), light.cosInner);
//			glUniform1f(glGetUniformLocation(testShader->ID, "lightSpot.cosOuter"), light.cosOuter);
//
//			// Material
//			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
//			//myMaterial->shader->SetUniform1i("material.diffuse", myMaterial->diffuse);
//			myMaterial->shader->SetUniform1i("material.diffuse", testShader->DIFFUSE);
//			myMaterial->shader->SetUniform1i("material.specular", testShader->SPECULAR);
//			myMaterial->shader->SetUniform1i("material.emission", testShader->EMISSION);
//			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);
//
//			//cout << "slot0: " << 0 << "  myMaterial->diffuse: " << myMaterial->diffuse << endl;
//			//cout << "slot1: " << 1 << "  myMaterial->specular: " << myMaterial->specular << endl;
//
//			// Set Model
//			glBindVertexArray(VAO);
//
//			// Drawcall
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		}
//
//		GLfloat curSizeLine = 10;
//		//    glGetFloatv(GL_LINE_WIDTH_RANGE,sizesLine);
//		//    glGetFloatv(GL_LINE_WIDTH_GRANULARITY,&stepLine);
//		glLineWidth(curSizeLine);
//		glBegin(GL_LINES);
//		glColor3f(1.0, 0.0, 0.0);
//		glVertex3f(1.0f, 0.0f, -30.0f);
//		glVertex3f(0.0f, 0.0f, 30.0f);
//		glEnd();
//
//		// Clean up, prepare for next render loop
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//		camera.UpdateCameraPos();
//	}
//
//	// Exit Program
//	glfwTerminate();
//	return 0;
//}