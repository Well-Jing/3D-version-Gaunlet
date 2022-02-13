#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ffImage.h"
#include "ffMesh.h"
#include "Shader.h"
#include "ffBone.h"
#include "ffAnimation.h"
#include "ffAnimator.h"

#include "stb_image.h"

#define SINGLE_INSTANCE(className)	private:\
										static className* m_Instance;\
										className(const className& gw) = delete; \
										className& operator=(const className& ins) = delete;\
									public:\
										~className()\
										{\
											this->SINGLE_OVER();\
											delete m_Instance;\
										}\
										static className* getInstance()\
										{\
											if (m_Instance == nullptr)\
											{\
												m_Instance = new className(); \
											}\
											return m_Instance;\
										}\

#define SINGLE_INSTANCE_SET(className)  className*  className::m_Instance = nullptr;

class ffModel
{
public:
	ffModel() = default;
	ffModel(std::string path);
	std::vector<ffTexture> textures_loaded;
	std::vector<ffMesh> meshes;
	std::string directory;

	// skeleton animation
	//ffAnimation* m_animation;
	//ffAnimator* m_animator;
	std::vector<ffAnimator*> m_animators;

	std::map<std::string, ffBoneInfo> m_boneInfoMap;
	unsigned int m_boneCounter = 0;

	void update(float _deltaTime, int animationNumber);
	void Draw(Shader *shader, int animationNumber);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	ffMesh processMesh(aiMesh* mesh, const aiScene* scene);
	//vector<ffTexture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
	std::vector<ffTexture>	loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene);
	unsigned int TextureFromFile(const char *path, const string &directory);


	// skeleton animation
	void setVertexBoneData(ffVertex& _vertex, int _boneId, float _weight);
	void loadBoneWeightForVertices(std::vector<ffVertex>& _vertexArr, aiMesh* _pMesh, const aiScene* _pScene);

};

class ffTextureMananger
{
public:
	void SINGLE_OVER() {}
	unsigned int createTexture(std::string _path);
	unsigned int createTexture(std::string _path, std::string _dir);
	unsigned int createTextureFromMemory(std::string _path, unsigned char* _data, unsigned int _width, unsigned int _height);
private:
	SINGLE_INSTANCE(ffTextureMananger)
		ffTextureMananger() {}
	unsigned int makeGLTexture(ffImage* _pImage);

	std::map<std::string, unsigned int>	m_texMap;
};

