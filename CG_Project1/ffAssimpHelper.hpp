#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


class ffAssimpHelper
{
public:
	static glm::vec3 getGLMVec3(aiVector3D _inVec)
	{
		return glm::vec3(_inVec.x, _inVec.y, _inVec.z);
	}

	static glm::quat getGLMQuat(aiQuaternion _quat)
	{
		return glm::quat(_quat.w, _quat.x, _quat.y, _quat.z); // ????? why ????
	}


	static glm::mat4 getGLMMat4(aiMatrix4x4 from)
	{
		glm::mat4 to;

		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

		return to;
	}


};

