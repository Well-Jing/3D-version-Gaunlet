#pragma once
#include <glm.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Shader.h"

struct ffVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	float m_boneWeightArr[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	int m_boneIdArr[4] = { -1, -1, -1, -1 };
};

struct ffTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class ffMesh
{
public:
	ffMesh(float vertices[]);
	ffMesh(std::vector<ffVertex> vertices, std::vector<unsigned int> indices, std::vector<ffTexture> textures);
	std::vector<ffVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ffTexture> textures;
	void Draw(Shader* shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

