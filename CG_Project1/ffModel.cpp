#include "ffModel.h"

ffModel::ffModel(std::string path)
{
	loadModel(path);
}

void ffModel::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp error when loading " << path << std::endl;
		return;
	}

	//directory = path.substr(0, path.find_last_of('\\'));
	directory = path;
	std::cout << "Success!  Path: " << directory << std::endl;

	processNode(scene->mRootNode, scene);

	std::cout << "Animation size:" << std::endl;
	std::cout << scene->mNumAnimations << std::endl;

	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* _aiAnimation = scene->mAnimations[i];
		ffAnimation* _animation = new ffAnimation(_aiAnimation, scene->mRootNode, m_boneInfoMap, m_boneCounter);
		ffAnimator* _animator = new ffAnimator(_animation);
		m_animators.push_back(_animator);
	}

}

void ffModel::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children

	//std::cout << node->mName.data << std::endl;

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

ffMesh ffModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<ffVertex> vertices;
	vector<unsigned int> indices;
	vector<ffTexture> textures;

	// process vertex positions, normals, texcoords
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		ffVertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<ffTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<ffTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	loadBoneWeightForVertices(vertices, mesh, scene);

	return ffMesh(vertices, indices, textures);
}


std::vector<ffTexture>	ffModel::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene)
{
	std::vector<ffTexture> _texVec;

	for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
	{	
		ffTexture _tex;

		aiString _path;
		_mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, i), _path);

		if (const aiTexture* _aiTexture = _scene->GetEmbeddedTexture(_path.C_Str()))
		{
			std::cout << "if" << std::endl;
			std::cout << _path.C_Str() << std::endl;
			_tex.id = ffTextureMananger::getInstance()->createTextureFromMemory(_path.C_Str(), reinterpret_cast<unsigned char*>(_aiTexture->pcData), _aiTexture->mWidth, _aiTexture->mHeight);
			_tex.path = _path.C_Str();
			_tex.type = _typeName;
		}
		else
		{
			std::cout << "else" << std::endl;
			std::cout << _path.C_Str() << std::endl;
			_tex.id = ffTextureMananger::getInstance()->createTexture(_path.C_Str(), directory);
			_tex.path = _path.C_Str();
			_tex.type = _typeName;
		}
		_texVec.push_back(_tex);
	}
	return _texVec;
}



//unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
unsigned int ffModel::TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory.substr(0, directory.find_last_of('\\')) + "\\" + filename;

	std::cout << filename << std::endl;
	//filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::cout << "Texture succeeded to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void ffModel::update(float _deltaTime, int animationNumber)
{
	if (m_animators[animationNumber])
	{
		m_animators[animationNumber]->updateAnimation(_deltaTime, m_boneInfoMap);
	}
}



void ffModel::Draw(Shader * shader, int animationNumber)
{
	if (animationNumber >= 0)
	{
		auto _transform = m_animators[animationNumber]->getFinalBoneMatrices();

		for (unsigned int i = 0; i < _transform.size(); i++)
		{ 
			shader->SetMatrix(("_finalBoneMatrices[" + std::to_string(i) + "]").c_str(), _transform[i]);
		}
	}

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void ffModel::setVertexBoneData(ffVertex& _vertex, int _boneId, float _weight)
{
	unsigned int max_bone = 4;
	for (unsigned int i = 0; i < max_bone; i++)
	{
		if (_vertex.m_boneIdArr[i] < 0)
		{
			_vertex.m_boneWeightArr[i] = _weight;
			_vertex.m_boneIdArr[i] = _boneId;
			break;
		}
	}
}

void ffModel::loadBoneWeightForVertices(std::vector<ffVertex>& _vertexArr, aiMesh * _pMesh, const aiScene * _pScene)
{
	for (unsigned int _boneIndex = 0; _boneIndex < _pMesh->mNumBones; _boneIndex++)
	{
		int _boneId = -1;
		std::string _boneName = _pMesh->mBones[_boneIndex]->mName.C_Str();

		if (m_boneInfoMap.find(_boneName) == m_boneInfoMap.end())
		{
			ffBoneInfo _boneInfo;
			_boneInfo.m_id = m_boneCounter;
			_boneInfo.m_offsetMatrix = ffAssimpHelper::getGLMMat4(_pMesh->mBones[_boneIndex]->mOffsetMatrix);
			m_boneInfoMap[_boneName] = _boneInfo;

			_boneId = m_boneCounter;
			m_boneCounter++;
		}
		else
		{
			_boneId = m_boneInfoMap[_boneName].m_id;
		}

		assert(_boneId != -1);

		aiVertexWeight* _pWeights = _pMesh->mBones[_boneIndex]->mWeights;
		unsigned int _weightNum = _pMesh->mBones[_boneIndex]->mNumWeights;

		for (unsigned int _weightIndex = 0; _weightIndex < _weightNum; _weightIndex++)
		{
			int _vertexId = _pWeights[_weightIndex].mVertexId;
			float _vertexWeight = _pWeights[_weightIndex].mWeight;
		
			assert(_vertexId < _vertexArr.size());
			setVertexBoneData(_vertexArr[_vertexId], _boneId, _vertexWeight);
		}

	}

}




SINGLE_INSTANCE_SET(ffTextureMananger)

unsigned int ffTextureMananger::createTexture(std::string _path)
{
	std::map<std::string, unsigned int>::iterator _it = m_texMap.find(_path);
	if (_it != m_texMap.end())
	{
		return _it->second;
	}
	
	ffImage* _image = ffImage::readFromFile(_path.c_str());
	
	unsigned int _texID = makeGLTexture(_image);
	
	delete _image;
	m_texMap[_path] = _texID;
	
	return _texID;
}

unsigned int ffTextureMananger::createTextureFromMemory(std::string _path, unsigned char* _data, unsigned int _width, unsigned int _height)
{
	std::map<std::string, unsigned int>::iterator _it = m_texMap.find(_path);
	if (_it != m_texMap.end())
	{
		return _it->second;
	}
	ffImage* _image = ffImage::readFromMemory(_data, _width, _height);

	unsigned int _texID = makeGLTexture(_image);

	delete _image;
	m_texMap[_path] = _texID;
	return _texID;
}

unsigned int ffTextureMananger::makeGLTexture(ffImage* _pImage)
{
	if (!_pImage)
	{
		return 0;
	}
	unsigned int _texID = 0;
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pImage->getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return _texID;
}

unsigned int ffTextureMananger::createTexture(std::string _path, std::string _dir)
{
	std::string filepath = _dir.substr(0, _dir.find_last_of('\\')) + "\\" + _path;
	std::cout << "Loading from: " << filepath << std::endl;
	//return createTexture(_dir + '/' + _path);
	return createTexture(filepath);
}

