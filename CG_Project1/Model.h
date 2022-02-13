#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//struct Texture {
//	unsigned int id;
//	string type;
//	string path;  // we store the path of the texture to compare with other textures
//};

class Model
{
public:
	Model() = default;
	Model(std::string path);
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	void Draw(Shader *shader);



private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);

};

